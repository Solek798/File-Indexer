//
// Created by felix on 13.11.25.
//

#include "Parameters.h"

#include <iostream>
#include <optional>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/date_time.hpp>

#include "FileInfo.h"


Parameters::Parameters(const int argc, char **argv)
    : _options("Allowed Options") {
    // an options_description defines what arguments can be passed to the program
    _options.add_options()
            ("help,h", "Display help message")
            ("path,p", boost::program_options::value<boost::filesystem::path>(),
             "Path to the folder that should be scanned")
            ("name,n", boost::program_options::value<std::string>(), "Name or regex of files to search for")
            ("min-size", boost::program_options::value<size_t>(), "Minimum file size in bytes")
            ("max-size", boost::program_options::value<size_t>(), "Maximum file size in bytes")
            ("before,b", boost::program_options::value<std::string>(),
             "Find files modified before the given date (YYYY-MM-DD)")
            ("after,a", boost::program_options::value<std::string>(),
             "Find files modified after the given date (YYYY-MM-DD)")
            ("sort,s", boost::program_options::value<std::string>(), "Sort by 'name', 'size' or 'date'");

    // makes the "path" option implicit
    boost::program_options::positional_options_description positional_options;
    positional_options.add("path", -1);

    // the parse_command_line function parses the arguments into usable format. the object itself is not ment to persistent
    const auto parser = boost::program_options::command_line_parser(argc, argv)
            .options(_options)
            .positional(positional_options)
            .run();

    // actually sore information into the map
    boost::program_options::store(parser, _map);

    _name_regex = _map.contains("name")
                     ? std::make_unique<boost::regex>(_map["name"].as<std::string>(), boost::regex::basic)
                     : nullptr;

    _min_size = _map.contains("min-size")
                   ? _map["min-size"].as<size_t>()
                   : 0;
    _max_size = _map.contains("max-size")
                   ? _map["max-size"].as<size_t>()
                   : 0;
}

void Parameters::check_runnable() const {
    // make the program exit if there is no path...
    if (!_map.contains("path"))
        throw std::invalid_argument("Path not specified");

    // ...or if there are no search criteria
    /*const auto options_count = map.count("name") +
                               map.count("min-size") +
                               map.count("max-size") +
                               map.count("before") +
                               map.count("after");

    if (options_count <= 0)
        throw std::invalid_argument("Search criteria not specified");*/
}

void Parameters::print_available() const {
    if (_map.contains("path"))
        std::cout << "Path: " << _map["path"].as<boost::filesystem::path>() << std::endl;
    if (_map.contains("name"))
        std::cout << "Name: " << _map["name"].as<std::string>() << std::endl;
    if (_map.contains("min-size"))
        std::cout << "Min Size: " << _map["min-size"].as<size_t>() << std::endl;
    if (_map.contains("max-size"))
        std::cout << "Maximum Size: " << _map["max-size"].as<size_t>() << std::endl;
    if (_map.contains("before"))
        std::cout << "Before: " << _map["before"].as<std::string>() << std::endl;
    if (_map.contains("after"))
        std::cout << "After: " << _map["after"].as<std::string>() << std::endl;
    std::cout << std::endl;
}

bool Parameters::contain_help() const {
    return _map.contains("help");
}

void Parameters::print_help() const {
    std::cout << _options << std::endl;
}

boost::filesystem::path Parameters::get_path() const {
    return _map["path"].as<boost::filesystem::path>();
}

ESortType Parameters::fetch_sort_type() const {
    if (!_map.contains("sort"))
        return ESortType::NONE;

    //TODO(FK): make this case insensitive
    const auto sort_string = _map["sort"].as<std::string>();

    if (sort_string == "name")
        return ESortType::NAME;
    else if (sort_string == "date")
        return ESortType::DATE;
    else if (sort_string == "size")
        return ESortType::SIZE;
    else
        throw std::invalid_argument("Invalid sort type specified: " + sort_string + ". Valid options are 'name', 'size' and 'date'.");
}

bool Parameters::passes_filter(const boost::filesystem::directory_entry &entry) const {
    // never pass directories
    if (entry.is_directory())
        return false;

    const bool check_name = _map.contains("name");
    const bool check_min_size = _map.contains("min-size");
    const bool check_max_size = _map.contains("max-size");
    const bool check_before = _map.contains("before");
    const bool check_after = _map.contains("after");

    // get data
    const auto filename = check_name
                              ? std::optional{entry.path().filename().string()}
                              : std::nullopt;
    const auto file_size = check_min_size || check_max_size
                               ? std::optional{boost::filesystem::file_size(entry.path())}
                               : std::nullopt;
    const auto unix_time = check_before | check_after
                               ? std::optional{boost::filesystem::last_write_time(entry.path())}
                               : std::nullopt;
    const auto last_modified = unix_time
                                   ? std::optional{boost::posix_time::from_time_t(unix_time.value()).date()}
                                   : std::nullopt;

    // check just the name via regex
    const bool matches_name = check_name ? boost::regex_match(filename.value(), *_name_regex) : true;

    // check minimum size
    const bool matches_min_size = check_min_size ? file_size.value_or(_min_size) >= _min_size : true;

    // check maximum size
    const bool matches_max_size = check_max_size ? file_size.value_or(_max_size) <= _max_size : true;

    // check before date
    const auto before_date = check_before
                                 ? std::optional{boost::gregorian::from_simple_string(_map["before"].as<std::string>())}
                                 : std::nullopt;
    const bool matches_before = check_before ? last_modified < before_date : true;

    // check after date
    const auto after_date = check_after
                                ? std::optional{boost::gregorian::from_simple_string(_map["after"].as<std::string>())}
                                : std::nullopt;
    const bool matches_after = check_after ? last_modified > after_date : true;

    return matches_name && matches_min_size && matches_max_size && matches_before && matches_after;
}
