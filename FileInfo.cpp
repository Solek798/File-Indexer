//
// Created by felix on 01.12.25.
//

#include "FileInfo.h"

#include <boost/filesystem/operations.hpp>

#define COMPARE(type) \
switch (_sort_type) {\
        case ESortType::NAME:\
return this->_path.filename() type other._path.filename();\
case ESortType::SIZE: {\
const auto this_size = boost::filesystem::file_size(this->_path);\
const auto other_size = boost::filesystem::file_size(other._path);\
return this_size type other_size;}\
case ESortType::DATE:{\
const auto this_time = boost::filesystem::last_write_time(this->_path);\
const auto other_time = boost::filesystem::last_write_time(other._path);\
return this_time type other_time;}\
default:\
return this->_path type other._path;\
    }

FileInfo::FileInfo(const boost::filesystem::directory_entry &entry)
    : _path(entry.path()) { }


const boost::filesystem::path & FileInfo::get_path() const {
    return _path;
}

void FileInfo::set_sort_type(const ESortType sort_type) {
    _sort_type = sort_type;
}

bool FileInfo::operator==(const FileInfo &other) const {
    COMPARE(==)
}

bool FileInfo::operator!=(const FileInfo &other) const {
    COMPARE(!=)
}

bool FileInfo::operator<(const FileInfo &other) const {
    COMPARE(<)
}

bool FileInfo::operator<=(const FileInfo &other) const {
    COMPARE(<=)
}

bool FileInfo::operator>(const FileInfo &other) const {
    COMPARE(>)
}

bool FileInfo::operator>=(const FileInfo &other) const {
    COMPARE(>=)
}

#undef COMPARE
