#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/path.hpp>

#include "FileInfo.h"
#include "Parameters.h"


int main(const int argc, char** argv) {
    try {
        const Parameters parameters(argc, argv);

        if (parameters.contain_help()) {
            parameters.print_help();
            return EXIT_SUCCESS;
        }

        const int error_code = parameters.check_runnable();
        if (error_code != EXIT_SUCCESS)
            return error_code;

        parameters.print_available();

        auto path = parameters.get_path();
        std::vector<FileInfo> files;

        /*for (const auto& part : path) {
            std::cout << part << std::endl;
        }*/

        for (const auto &entry: boost::filesystem::recursive_directory_iterator(path)) {
            if (parameters.passes_filter(entry))
                files.emplace_back(entry);
        }

        std::ranges::sort(files);

        for (const auto& file : files) {
            std::cout << file.get_path() << std::endl;
        }


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Hello, World!" << std::endl;
    return EXIT_SUCCESS;
}
