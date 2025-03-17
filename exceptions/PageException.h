#pragma once

#include <exception>

class PageException : public std::exception {
public:
    const char * what() const noexcept override {
        return "invalid page number";
    };
};