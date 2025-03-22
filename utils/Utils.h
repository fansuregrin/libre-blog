#pragma once

#include <string>
#include <regex>
#include <sstream>
#include <vector>
#include <drogon/utils/Utilities.h>
#include <json/json.h>
#include <jwt-cpp/traits/open-source-parsers-jsoncpp/traits.h>

using json_traits = jwt::traits::open_source_parsers_jsoncpp;

const std::string es256k_priv_key = R"(-----BEGIN EC PRIVATE KEY-----
MHQCAQEEII+btqvqU7jdCmlceokNXspmSnhrqbVcNO/gPaNnG9dSoAcGBSuBBAAK
oUQDQgAEO55sfkgK5AhGqNcwr0M5lIMaSV8DJt/LCldRlEwg401gT5hNG8etmrVf
1WP9ET2wwPOG1JjSrijxq0U9NZd2MQ==
-----END EC PRIVATE KEY-----)";

const std::string es256k_pub_key = R"(-----BEGIN PUBLIC KEY-----
MFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEO55sfkgK5AhGqNcwr0M5lIMaSV8DJt/L
CldRlEwg401gT5hNG8etmrVf1WP9ET2wwPOG1JjSrijxq0U9NZd2MQ==
-----END PUBLIC KEY-----)";

const auto emailPattern = std::regex(
    R"(^[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)*@(\w+)(\.(\w+))+$)"
);

const auto passwordPattern = std::regex(
    R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[\W_])(?!.*\s).{6,20}$)"
);

bool checkEmail(const std::string &email);
bool checkUsername(const std::string &username);
bool checkPassword(const std::string &password);
std::string encodePassword(const std::string &rawPassword);
bool verfiyPassword(const std::string &rawPassword, 
    const std::string &encodedPassword);

template <typename T>
std::string join(
    const std::vector<T> &v, 
    const std::string &open, 
    const std::string &close, 
    const std::string &sep
) {
    int n = v.size();
    if (n < 1) return "";
    std::ostringstream oss;
    oss << open;
    for (int i=0; i<n; ++i) {
        if (i > 0) {
            oss << sep;
        }
        oss << v[i];
    }
    oss << close;
    return oss.str();
}

template <typename T>
std::vector<T> split(const std::string &s, const std::string &sep) {
    std::vector<T> r;
    size_t start = 0;
    size_t end = s.find(sep);
    while (end != std::string::npos) {
        // assume string can be converted to type T
        r.emplace_back(s.substr(start, end - start));
        start = end + sep.length();
        end = s.find(sep, start);
    }
    r.emplace_back(s.substr(start, end - start));
    return r;
}

template <>
inline std::vector<int> split(const std::string &s, const std::string &sep) {
    std::vector<int> r;
    size_t start = 0;
    size_t end = s.find(sep);
    while (end != std::string::npos) {
        // assume string can be converted to type T
        r.emplace_back(std::stoi(s.substr(start, end - start)));
        start = end + sep.length();
        end = s.find(sep, start);
    }
    r.emplace_back(std::stoi(s.substr(start, end - start)));
    return r;
}

template <typename T>
Json::Value toJson(const std::vector<T> &v) {
    Json::Value r;
    for (const auto &e : v) {
        r.append(e.toJson());
    }
    return r;
}

template <>
inline Json::Value toJson(const std::vector<int> &v) {
    Json::Value r;
    for (const auto &e : v) {
        r.append(e);
    }
    return r;
}