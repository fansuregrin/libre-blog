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

static std::string hashWithSalt(const std::string &s, const std::string &salt) {
    using namespace drogon::utils;
    std::string h1 = getSha1(s + salt);
    std::string h2 = getSha256(s + h1 + salt);
    return h2 + salt;
}

std::string encodePassword(const std::string &rawPassword) {
    using namespace drogon::utils;
    auto salt = getMd5(genRandomString(12));
    // string lenth: 64 + 32 = 96
    return hashWithSalt(rawPassword, salt);
}

bool verfiyPassword(
    const std::string &rawPassword, 
    const std::string &encodedPassword
) {
    auto salt = encodedPassword.substr(64);
    return hashWithSalt(rawPassword, salt) == encodedPassword;
}