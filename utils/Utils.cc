#include "Utils.h"

bool verifyUserToken(const std::string &token, int &userId) {
    try {
        auto decoded = jwt::decode<json_traits>(token);
        jwt::verify<json_traits>()
            .allow_algorithm(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key))
            .with_issuer("drogon")
            .verify(decoded);
        userId = decoded.get_payload_claim("uid").as_integer();
        return true;
    } catch (const std::exception &ex) {
        LOG_DEBUG << ex.what();
        return false;
    }
}