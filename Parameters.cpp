//
// Created by felix on 13.11.25.
//

#include "Parameters.h"

#include <iostream>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>


Parameters::Parameters(const int argc, char **argv)
    : options("Allowed Options")
{
    // an options_description defines what arguments can be passed to the program
    options.add_options()
        ("help,h", "Display help message")
        ("path,p", boost::program_options::value<boost::filesystem::path>(), "Path to the folder that should be scanned")
        ("name,n", boost::program_options::value<std::string>(), "Name or regex of files to search for")
        ("min-size", boost::program_options::value<size_t>(), "Minimum file size in bytes")
        ("max-size", boost::program_options::value<size_t>(), "Maximum file size in bytes (0 for no limit)")
        ("before,b", boost::program_options::value<std::string>(), "Find files modified before the given date (YYYY-MM-DD)")
        ("after,a", boost::program_options::value<std::string>(), "Find files modified after the given date (YYYY-MM-DD)");

    // makes the "path" option implicit
    boost::program_options::positional_options_description positional_options;
    positional_options.add("path", -1);

    // the parse_command_line function parses the arguments into usable format. the object itself is not ment to persistent
    const auto parser = boost::program_options::command_line_parser(argc, argv)
                                                                      .options(options)
                                                                      .positional(positional_options)
                                                                      .run();

    // actually sore information into the map
    boost::program_options::store(parser, map);

    name_regex = map.contains("name")
                 ? std::make_unique<boost::regex>(map["name"].as<std::string>(), boost::regex::basic)
                 : nullptr;

    min_size = map.contains("min-size")
               ? map["min-size"].as<size_t>()
               : 0;
    max_size = map.contains("max-size")
               ? map["max-size"].as<size_t>()
               : 0;
}

int Parameters::check_runnable() const {
    // make the program exit if there is no path...
    if (!map.contains("path")) {
        std::cerr << "Error: No path specified!" << std::endl;
        return EXIT_FAILURE;
    }

    // ...or if there are no search criteria
    const auto options_count = map.count("name") +
                        map.count("min-size") +
                        map.count("max-size") +
                        map.count("before") +
                        map.count("after");
    if (options_count <= 0) {
        std::cerr << "Error: No search criteria specified!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Parameters::print_available() const {
    if (map.contains("path"))
        std::cout << "Path: " << map["path"].as<boost::filesystem::path>() << std::endl;
    if (map.contains("name"))
        std::cout << "Name: " << map["name"].as<std::string>() << std::endl;
    if (map.contains("min-size"))
        std::cout << "Min Size: " << map["min-size"].as<size_t>() << std::endl;
    if (map.contains("max-size"))
        std::cout << "Maximum Size: " << map["max-size"].as<size_t>() << std::endl;
    if (map.contains("before"))
        std::cout << "Before: " << map["before"].as<std::string>() << std::endl;
    if (map.contains("after"))
        std::cout << "After: " << map["after"].as<std::string>() << std::endl;
    std::cout << std::endl;
}

bool Parameters::contain_help() const {
    return map.contains("help");
}

void Parameters::print_help() const {
    std::cout << options << std::endl;
}

boost::filesystem::path Parameters::get_path() const {
    return map["path"].as<boost::filesystem::path>();
}

bool Parameters::passes_filter(const boost::filesystem::directory_entry &entry) const {
    // never pass directories
    if (entry.is_directory())
        return false;

    // check just the name via regex
    const std::string filename = entry.path().filename().string();
    const bool matches_name = map.contains("name") ? boost::regex_match(filename, *name_regex) : true;

    // check minimum size
    const size_t file_size = boost::filesystem::file_size(entry.path());
    const bool matches_min_size = map.contains("min-size") ? file_size >= min_size : true;

    // check maximum size
    const bool matches_max_size = map.contains("max-size") ? file_size <= max_size : true;

    return matches_name && matches_min_size && matches_max_size;
}
