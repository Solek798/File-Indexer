//
// Created by felix on 13.11.25.
//

#ifndef PARAMETER_H
#define PARAMETER_H

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/regex.hpp>
#include "DefaultDefinitions.h"


enum class ESortType : uint8_t;

namespace boost::filesystem {
    class directory_entry;
    class path;
}

class Parameters {
protected:
    // map where the parsed option information is stored
    boost::program_options::variables_map _map;
    boost::program_options::options_description _options;

    up<boost::regex> _name_regex;

    size_t _min_size;
    size_t _max_size;

public:
    Parameters(int argc, char** argv);

    // checks if critical parameters are present. throws exception if not
    void check_runnable() const;
    // debug method
    void print_available() const;

    [[nodiscard]] bool contain_help() const;
    void print_help() const;

    [[nodiscard]] boost::filesystem::path get_path() const;

    [[nodiscard]] ESortType fetch_sort_type() const;

    [[nodiscard]] bool passes_filter(const boost::filesystem::directory_entry &entry) const;
};



#endif //PARAMETER_H
