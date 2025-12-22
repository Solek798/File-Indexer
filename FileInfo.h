//
// Created by felix on 01.12.25.
//

#ifndef FILEINFO_H
#define FILEINFO_H

#include <boost/filesystem/directory.hpp>

enum class ESortType : uint8_t {
    NAME,
    SIZE,
    DATE,
    NONE
};

class FileInfo {
protected:
    boost::filesystem::path _path;
    static inline ESortType _sort_type = ESortType::NONE;

public:
    explicit FileInfo(const boost::filesystem::directory_entry &entry);

    [[nodiscard]] const boost::filesystem::path& get_path() const;

    static void set_sort_type(ESortType sort_type);

    bool operator==(const FileInfo &other) const;
    bool operator!=(const FileInfo &other) const;
    bool operator<(const FileInfo &other) const;
    bool operator<=(const FileInfo &other) const;
    bool operator>(const FileInfo &other) const;
    bool operator>=(const FileInfo &other) const;
};



#endif //FILEINFO_H
