#define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>
#include <drogon/drogon.h>
#include "../utils/Utils.h"

DROGON_TEST(Md5Test)
{
    auto e1 = drogon::utils::getMd5("123");
    auto e2 = drogon::utils::getMd5("something111");
    std::cout << e1 << ", len: " << e1.size() << std::endl;
    std::cout << e2 << ", len: " << e2.size() << std::endl;
}

DROGON_TEST(Sha256Test)
{
    auto e1 = drogon::utils::getSha256("123");
    auto e2 = drogon::utils::getSha256("something111");
    std::cout << e1 << ", len: " << e1.size() << std::endl;
    std::cout << e2 << ", len: " << e2.size() << std::endl;
}

DROGON_TEST(PasswordTest) {
    std::string p = "Pw123#";
    auto e = encodePassword(p);
    std::cout << "Pw123# => " << e << std::endl;
    assert(verfiyPassword(p, e));
}

int main(int argc, char** argv) 
{
    using namespace drogon;

    std::promise<void> p1;
    std::future<void> f1 = p1.get_future();

    // Start the main loop on another thread
    std::thread thr([&]() {
        // Queues the promise to be fulfilled after starting the loop
        app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
        app().run();
    });

    // The future is only satisfied after the event loop started
    f1.get();
    int status = test::run(argc, argv);

    // Ask the event loop to shutdown and wait
    app().getLoop()->queueInLoop([]() { app().quit(); });
    thr.join();
    return status;
}
