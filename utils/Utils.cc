#include "Utils.h"

bool verifyUserToken(const std::string &token) {
    try {
        auto decoded = jwt::decode<json_traits>(token);
        jwt::verify<json_traits>()
            .allow_algorithm(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key))
            .with_issuer("drogon")
            .verify(decoded);
        return true;
    } catch (const std::exception &ex) {
        LOG_DEBUG << ex.what();
        return false;
    }
}

bool checkEmail(const std::string &email) {
    if (email.empty()) return false;
    std::smatch res;
    return std::regex_match(email, res, emailPattern);
}

bool checkUsername(const std::string &username) {
    if (username.size() < 1) return false;
    if (std::isspace(*username.cbegin()) || std::isspace(*username.crbegin())) {
        return false;
    }
    return true;
}

bool checkPassword(const std::string &password) {
    if (password.size() < 6) return false;
    for (const auto &ch:password) {
        if (std::isspace(ch)) {
            return false;
        }
    } 
    return true;
}