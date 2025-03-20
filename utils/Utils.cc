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

static std::string _encode(const std::string &s, const std::string &salt) {
    using namespace drogon::utils;
    return getSha256(getSha1(s + salt)) + salt;
}

std::string encodePassword(const std::string &rawPassword) {
    using namespace drogon::utils;
    auto salt = getMd5(genRandomString(12));
    // string lenth: 64 + 32 = 96
    return _encode(rawPassword, salt);
}

bool verfiyPassword(
    const std::string &rawPassword, 
    const std::string &encodedPassword
) {
    auto salt = encodedPassword.substr(64);
    return _encode(rawPassword, salt) == encodedPassword;
}