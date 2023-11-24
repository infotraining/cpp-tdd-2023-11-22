#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <queue>

#include "document.hpp"
#include "application.hpp"
#include "command.hpp"
#include "mocks/mocks.hpp"

using namespace ::testing;
using namespace std::literals;

class MockCommand : public Command
{
public:
    MOCK_METHOD(void, execute, (), (override));
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

TEST_F(ApplicationMainLoop, ExecutesCommand)
{
    auto cmd = std::make_unique<MockCommand>();
    
    EXPECT_CALL(*cmd, execute()).Times(1);
    EXPECT_CALL(console, get_line()).WillOnce(Return("Cmd")).RetiresOnSaturation();

    DocEditApp app(console);
    app.add_command("Cmd", std::move(cmd));
    app.run();
}

//////////////////////////////////////////////
// Command Print



TEST(PrintCmdTests, ExecutePrintsDocContentInConsole)
{
    Document doc{"abc"};
    MockConsole console;

    PrintCmd cmd{doc, console};
    
    EXPECT_CALL(console, print("[abc]"));
    cmd.execute();
}



//////////////////////////////////////////////
// Command AddText

TEST(AddCmdTests, Execute)
{
    Document doc{"abc"};   
    MockConsole console;

    AddCmd cmd{doc, console};  
    
    EXPECT_CALL(console, print("AddText:"));
    EXPECT_CALL(console, get_line()).WillOnce(Return("def"));
    cmd.execute();  

    ASSERT_EQ(doc.text(), "abcdef"); 
}


//////////////////////////////////////////////
TEST(GMockTests, RetiresOnSaturation)
{
    MockConsole console;

    //EXPECT_CALL(console, get_line()).Times(1).WillRepeatedly(Return("Exit"));
    EXPECT_CALL(console, print(_)).WillRepeatedly(Return());
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

//////////////////////////////////////////////////
// Callbacks

template <typename T>
using Event = std::function<void(T)>;

struct EventQueue
{
    std::queue<Event<int>> events;

    void push(Event<int> e)
    { 
        events.push(e);
    }

    void run()
    {
        events.front()(42);   
    }
};



void fire_on_timeout(Event<int> e)
{
    e(42);
}

TEST(EventQueueTests, EventIsFired)
{
    EventQueue eq;

    std::optional<int> value;
    Event<int> callback = [&value](int x) { value = x; };
    eq.push(callback);

    eq.run();

    ///////// barrier

    ASSERT_TRUE(value.has_value());
    ASSERT_EQ(*value, 42);
}