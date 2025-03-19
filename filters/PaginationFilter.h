#pragma once

#include <drogon/HttpFilter.h>
#include "../dtos/ApiResponse.h"

using namespace drogon;

class PaginationFilter : public HttpFilter<PaginationFilter> {
public:
    PaginationFilter() {}
    
    void doFilter(
        const HttpRequestPtr &req, FilterCallback &&fcb, 
        FilterChainCallback &&fccb) override;
};