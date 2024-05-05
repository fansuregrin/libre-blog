#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <trantor/utils/Logger.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

using json_traits = jwt::traits::nlohmann_json;

const std::string es256k_priv_key = R"(-----BEGIN EC PRIVATE KEY-----
MHQCAQEEII+btqvqU7jdCmlceokNXspmSnhrqbVcNO/gPaNnG9dSoAcGBSuBBAAK
oUQDQgAEO55sfkgK5AhGqNcwr0M5lIMaSV8DJt/LCldRlEwg401gT5hNG8etmrVf
1WP9ET2wwPOG1JjSrijxq0U9NZd2MQ==
-----END EC PRIVATE KEY-----)";

const std::string es256k_pub_key = R"(-----BEGIN PUBLIC KEY-----
MFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEO55sfkgK5AhGqNcwr0M5lIMaSV8DJt/L
CldRlEwg401gT5hNG8etmrVf1WP9ET2wwPOG1JjSrijxq0U9NZd2MQ==
-----END PUBLIC KEY-----)";

bool verifyUserToken(const std::string &token, int &userId);

#endif