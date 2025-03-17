#pragma once

#include <string>

struct Password {
    std::string oldPassword;
    std::string newPassword;

    Password() {}

    Password(const std::string &oldPasswd, const std::string &newPasswd) {
        oldPassword = oldPasswd;
        newPassword = newPasswd;
    }
    
    Password(Password &&other) {
        oldPassword = other.oldPassword;
        newPassword = other.newPassword;
    }
};