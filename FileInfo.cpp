//
// Created by felix on 01.12.25.
//

#include "FileInfo.h"


FileInfo::FileInfo(const boost::filesystem::directory_entry &entry)
    : path(entry.path()) { }

const boost::filesystem::path & FileInfo::get_path() const {
    return path;
}

bool FileInfo::operator==(const FileInfo &other) const {
    return this->path == other.path;
}

bool FileInfo::operator!=(const FileInfo &other) const {
    return this->path != other.path;
}

bool FileInfo::operator<(const FileInfo &other) const {
    return this->path < other.path;
}

bool FileInfo::operator<=(const FileInfo &other) const {
    return this->path <= other.path;
}

bool FileInfo::operator>(const FileInfo &other) const {
    return this->path > other.path;
}

bool FileInfo::operator>=(const FileInfo &other) const {
    return this->path >= other.path;
}
