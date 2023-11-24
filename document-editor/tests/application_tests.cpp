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
        do 
        {
            console_.print("> Enter a command:");
            std::string cmd_text = console_.get_line();
            if (cmd_text == "Exit") break;
        } while (true);
    }
private:
    Console& console_;
};

TEST(ApplicationTests, DisplaysPromptForCommand)
{
    NiceMock<MockConsole> console;
    ON_CALL(console, get_line()).WillByDefault(Return("Exit"));
    EXPECT_CALL(console, print("> Enter a command:"));

    DocEditApp app(console);
    app.run();
}

TEST(ApplicationTests, GetLineFromConsole)
{
    NiceMock<MockConsole> console;
    ON_CALL(console, get_line()).WillByDefault(Return("Exit"));
    EXPECT_CALL(console, get_line()).Times(1);

    DocEditApp app(console);
    app.run();
}

TEST(Applicaton_MainLoop, ExitBreaksTheCommandLoop)
{
    MockConsole console;
    EXPECT_CALL(console, get_line()).WillOnce(Return("Exit"));
    EXPECT_CALL(console, get_line()).Times(3).WillRepeatedly(Return("cmd")).RetiresOnSaturation();

    DocEditApp app(console);
    app.run();   
}