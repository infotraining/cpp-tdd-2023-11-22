#include <algorithm>
#include <string>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bowling.hpp"

using namespace std;

class BowlingGame
{
public:
    int score() const
    {
        return score_;
    }

    void roll(int pins)
    {
        score_ += pins;
    }
private:
    int score_ {0};

};

class BowlingGameTests : public ::testing::Test // Arrange
{   
protected:
    BowlingGame game;

    BowlingGameTests() 
    {
    }

    void SetUp() override
    {
        ASSERT_EQ(game.score(), 0);
    }

    // void TearDown() override
    // {
    // }
};

TEST_F(BowlingGameTests, WhenGameStartsScoreIsZero)
{
    int result = game.score();       // Act

    ASSERT_EQ(result, 0);            // Assert
}

TEST_F(BowlingGameTests, WhenAllRollsZeroScoreIsZero)
{
    for(int i = 0; i < 20; ++i)
    {
        game.roll(0);
    }

    int result = game.score();       // Act

    ASSERT_EQ(result, 0);            // Assert
}   

TEST_F(BowlingGameTests, WhenRollsWithPinsScoreIsSumOfPins)
{
    for(int i = 0; i < 20; ++i)
    {
        game.roll(1);
    }

    int result = game.score();       // Act

    ASSERT_EQ(result, 20);            // Assert
}  