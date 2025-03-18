#include "Utils.h"

bool checkEmail(const std::string &email) {
    if (email.empty()) return false;
    std::smatch res;
    return std::regex_match(email, res, emailPattern);
}

bool checkUsername(const std::string &username) {
    if (username.size() < 4 || username.size() > 16) return false;
    for (const char &c : username) {
        if (std::isspace(c)) {
            return false;
        }
    }
    return true;
}

bool checkPassword(const std::string &password) {
    if (password.empty()) return false;
    std::smatch res;
    return std::regex_match(password, res, passwordPattern);
}