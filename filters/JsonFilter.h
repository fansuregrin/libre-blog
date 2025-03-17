#pragma once

#include <drogon/HttpFilter.h>
#include "../dtos/ApiResponse.h"

using namespace drogon;


class JsonFilter : public HttpFilter<JsonFilter> {
public:
    JsonFilter() {}
    void doFilter(
        const HttpRequestPtr &req,
        FilterCallback &&fcb,
        FilterChainCallback &&fccb
    ) override;
};

