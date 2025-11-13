#include <iostream>
#include <boost/program_options.hpp>

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

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Hello, World!" << std::endl;
    return EXIT_SUCCESS;
}
