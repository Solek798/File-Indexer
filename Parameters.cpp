//
// Created by felix on 13.11.25.
//

#include "Parameters.h"

#include <iostream>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

Parameters::Parameters(int argc, char **argv)
    : options("Allowed parameters")
{
    // an options_description defines what arguments can be passed to the program
    boost::program_options::options_description options("Allowed options");
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
    const bool is_not_directory = !entry.is_directory();
    const std::string filename = entry.path().filename().string();
    const bool matches_name = boost::regex_match(filename, *name_regex);

    return is_not_directory && matches_name;
}
