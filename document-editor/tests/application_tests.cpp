#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "document.hpp"
#include "mocks/mocks.hpp"

using namespace ::testing;
using namespace std::literals;

class DocEditApp
{
public:
    DocEditApp(Console& console) : console_{console}
    {}

    void run()
    {
        console_.print("> Enter a command:");
    }
private:
    Console& console_;
};

TEST(ApplicationTests, DisplaysPromptForCommand)
{
    MockConsole console;
    EXPECT_CALL(console, print("> Enter a command:"));

    DocEditApp app(console);
    app.run();
}