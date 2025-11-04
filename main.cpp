#include <iostream>
#include <boost/program_options.hpp>

int main(const int argc, char** argv) {
    // an options_description defines what arguments can be passed to the program
    boost::program_options::options_description description("Allowed options");
    description.add_options()
        ("help,h", "Display help message");

    // map where the parsed option information is stored
    boost::program_options::variables_map map;
    // the parse_command_line function parses the arguments into usable format. the object itself is not ment to persistent
    const auto parser = boost::program_options::parse_command_line(argc, argv, description);
    // actually sore information into the map
    boost::program_options::store(parser, map);

    if (map.contains("help")) {
        std::cout << description << std::endl;
        return 0;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}