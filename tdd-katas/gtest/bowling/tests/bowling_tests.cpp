#include <algorithm>
#include <string>
#include <memory>
#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bowling.hpp"

using namespace std;

class BowlingGame
{
public:
    int score() const
    {
        int result{};
        for(int i = 0; i < rolls_.size(); i += 2)
        {
            if (rolls_.at(i) + rolls_.at(i+1) == 10)
                result += rolls_.at(i+2);
            
            result += rolls_.at(i) + rolls_.at(i+1);
        }
        
        return result;
    }

    void roll(int pins)
    {
        rolls_.at(roll_index_++) = pins;
    }
private:
    int roll_index_{0};
    std::array<int, 20> rolls_ {};
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

    void roll_many(int rolls, int  pins)
    {
        for(int i = 0; i < rolls; ++i)
        {
            game.roll(pins);
        }
    }
};

TEST_F(BowlingGameTests, WhenGameStartsScoreIsZero)
{
    int result = game.score();       // Act

    ASSERT_EQ(result, 0);            // Assert
}

TEST_F(BowlingGameTests, WhenAllRollsZeroScoreIsZero)
{
    roll_many(20, 0);
    
    int result = game.score();       // Act

    ASSERT_EQ(result, 0);            // Assert
}   

TEST_F(BowlingGameTests, WhenRollsWithPinsScoreIsSumOfPins)
{
    roll_many(20, 1);

    int result = game.score();       // Act

    ASSERT_EQ(result, 20);           // Assert
} 

TEST_F(BowlingGameTests, WhenSpareNextRoundHasDoubledPoints)
{
    game.roll(1);
    game.roll(9);  // spare
    roll_many(18, 1);

    ASSERT_EQ(game.score(), 29);
} 

// TEST_F(BowlingGameTests, WhenSpareNextRollHasDoubledPoints)
// {
//     game.roll(1);
//     game.roll(9);
//     game.roll(0);
//     roll_many(17, 1);
//     ASSERT_EQ(result, 27);           // Assert
// } 
