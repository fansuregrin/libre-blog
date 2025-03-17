#include <drogon/drogon.h>
#include "handlers/ExceptionHandler.h"

int main() {
    drogon::app()
        .setExceptionHandler(exceptionHandler)
        .loadConfigFile("./config.json")
        .run();
    
    return 0;
}
