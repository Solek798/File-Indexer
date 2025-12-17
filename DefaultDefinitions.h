//
// Created by felix on 02.12.25.
//

#ifndef DEFAULTDEFINITIONS_H
#define DEFAULTDEFINITIONS_H
#include <memory>

template<typename T> using sp = std::shared_ptr<T>;
template<typename T> using up = std::unique_ptr<T>;

//template<typename T> using ms = std::make_shared<T>;

#endif //DEFAULTDEFINITIONS_H
