//
// Created by felix on 01.12.25.
//

#ifndef FILEINFO_H
#define FILEINFO_H

#include <boost/filesystem/directory.hpp>


class FileInfo {
protected:
    boost::filesystem::path path;

public:
    explicit FileInfo(const boost::filesystem::directory_entry &entry);

    [[nodiscard]] const boost::filesystem::path& get_path() const;

    bool operator==(const FileInfo &other) const;
    bool operator!=(const FileInfo &other) const;
    bool operator<(const FileInfo &other) const;
    bool operator<=(const FileInfo &other) const;
    bool operator>(const FileInfo &other) const;
    bool operator>=(const FileInfo &other) const;
};



#endif //FILEINFO_H
