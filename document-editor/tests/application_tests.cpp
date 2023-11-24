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

struct ApplicationMainLoop : ::testing::Test
{
    MockConsole console;
    DocEditApp app{console};

    void SetUp() override
    {
        EXPECT_CALL(console, print(_)).WillRepeatedly(Return());
        EXPECT_CALL(console, get_line()).WillOnce(Return("Exit"s)).RetiresOnSaturation();
    }
};

TEST_F(ApplicationMainLoop, DisplaysPromptForCommand)
{
    EXPECT_CALL(console, print("> Enter a command:"));

    DocEditApp app(console);
    app.run();
}

TEST_F(ApplicationMainLoop, GetLineFromConsole)
{
    EXPECT_CALL(console, get_line()).Times(1).RetiresOnSaturation();

    DocEditApp app(console);
    app.run();
}

TEST_F(ApplicationMainLoop, ExitBreaksTheCommandLoop)
{
    EXPECT_CALL(console, get_line()).Times(3).WillRepeatedly(Return("cmd")).RetiresOnSaturation();

    DocEditApp app(console);
    app.run();   
}

//////////////////////////////////////////////
TEST(GMockTests, RetiresOnSatureation)
{
    MockConsole console;

    //EXPECT_CALL(console, get_line()).Times(1).WillRepeatedly(Return("Exit"));
    EXPECT_CALL(console, print(_)).WillRepeatedly(Return());
    // EXPECT_CALL(console, get_line()).Times(3).WillRepeatedly(Return("Line"));
    EXPECT_CALL(console, get_line()).WillOnce(Return("Line1"))
        .WillOnce(Return("Line2"))
        .WillOnce(Return("Line3"))
        .WillRepeatedly(Return("Exit"));

    ASSERT_EQ(console.get_line(), "Line1");
    ASSERT_EQ(console.get_line(), "Line2");
    ASSERT_EQ(console.get_line(), "Line3");
    ASSERT_EQ(console.get_line(), "Exit");
    ASSERT_EQ(console.get_line(), "Exit");
    ASSERT_EQ(console.get_line(), "Exit");
    ASSERT_EQ(console.get_line(), "Exit");
}