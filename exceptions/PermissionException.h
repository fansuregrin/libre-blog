#pragma once

#include <exception>

class PermissionException : public std::exception {
public:
    const char * what() const noexcept override {
        return "permission denied";
    }
};