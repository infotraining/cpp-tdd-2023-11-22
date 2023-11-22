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
            if (is_spare(i))
                result += spare_bonus(i);
            
            result += frame_score(i);
        }
        
        return result;
    }

    void roll(int pins)
    {
        rolls_.at(roll_index_++) = pins;
    }
private:
    static constexpr int max_pins_in_frame = 10;

    int roll_index_{0};
    std::array<int, 20> rolls_ {};

    bool is_spare(int roll_index) const
    {
        return rolls_.at(roll_index) + rolls_.at(roll_index+1) == max_pins_in_frame;
    }

    int frame_score(int roll_index) const
    {
        return rolls_.at(roll_index) + rolls_.at(roll_index+1);
    }

    size_t spare_bonus(size_t roll_index) const {
        return rolls_[roll_index + 2];
}
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

    void roll_spare()
    {
        game.roll(1);
        game.roll(9);
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
    roll_spare();
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
