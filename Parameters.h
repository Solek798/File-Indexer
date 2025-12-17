//
// Created by felix on 13.11.25.
//

#ifndef PARAMETER_H
#define PARAMETER_H

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/regex.hpp>
#include "DefaultDefinitions.h"


namespace boost::filesystem {
    class directory_entry;
    class path;
}

class Parameters {
protected:
    // map where the parsed option information is stored
    boost::program_options::variables_map map;
    boost::program_options::options_description options;

    up<boost::regex> name_regex;

public:
    Parameters(int argc, char** argv);

    // TODO(FK): make this use custom exception
    // checks if critical parameters are present. prints error when not. returns EXIT_SUCCESS when ok.
    [[nodiscard]] int check_runnable() const;
    // debug method
    void print_available() const;

    [[nodiscard]] bool contain_help() const;
    void print_help() const;

    [[nodiscard]] boost::filesystem::path get_path() const;

    [[nodiscard]] bool passes_filter(const boost::filesystem::directory_entry &entry) const;
};



#endif //PARAMETER_H
