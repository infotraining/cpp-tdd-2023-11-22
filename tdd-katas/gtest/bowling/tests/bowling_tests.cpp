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
        int roll_index{};
        for(int i = 0; i < frames_count; ++i)
        {
            auto [current_frame_score, frame_size] = frame_score(roll_index);
            result += current_frame_score;
            
            if (is_strike(roll_index))
            {
                result += strike_bonus(roll_index);
            }
            else if (is_spare(roll_index))
            {
                 result += spare_bonus(roll_index);                
            }
            roll_index += frame_size;
        }
        
        return result;
    }

    void roll(int pins)
    {
        rolls_.at(current_roll++) = pins;
    }
private:
    static constexpr int max_pins_in_frame = 10;
    static constexpr int frames_count = 10;

    int current_roll{0};
    std::array<int, 21> rolls_ {};

    bool is_spare(int roll_index) const
    {
        return rolls_.at(roll_index) + rolls_.at(roll_index+1) == max_pins_in_frame;
    }

    bool is_strike(int roll_index) const
    {
        return rolls_.at(roll_index) == max_pins_in_frame;
    }

    std::pair<int, int> frame_score(int roll_index) const
    {
        if (is_strike(roll_index))
        {
            return std::pair{max_pins_in_frame, 1};
        }

        return std::pair{rolls_.at(roll_index) + rolls_.at(roll_index+1), 2};
    }

    size_t spare_bonus(size_t roll_index) const 
    {
        return rolls_[roll_index + 2];
    }

    size_t strike_bonus(size_t roll_index) const 
    {
        return rolls_[roll_index + 1] + rolls_[roll_index + 2];
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

    void roll_strike()
    {
        game.roll(10);
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

TEST_F(BowlingGameTests, WhenRollsWithNoSpareOrStrikeScoreIsSumOfPins)
{
    roll_many(20, 1);

    int result = game.score();       // Act

    ASSERT_EQ(result, 20);           // Assert
} 

TEST_F(BowlingGameTests, WhenSpareNextRollIsCountedTwice)
{
    roll_spare();
    game.roll(1);
    roll_many(17, 2);

    ASSERT_EQ(game.score(), 46);
} 

TEST_F(BowlingGameTests, WhenSpareButNextRollZeroScoreIsAsSumOfPins)
{
    game.roll(0);
    game.roll(8);
    game.roll(2);
    roll_many(17, 1);
    ASSERT_EQ(game.score(), 27);     // Assert
} 

TEST_F(BowlingGameTests, WhenStrikeTwoNextRollsAreCountedTwice)
{
    roll_strike();
    game.roll(3);
    game.roll(6);
    roll_many(16, 1);

    ASSERT_EQ(game.score(), 44);
}

TEST_F(BowlingGameTests, WhenSpareInLastFrameThenBonusAsExtraRoll)
{
    roll_many(18, 1);
    roll_spare();
    game.roll(6);

    ASSERT_EQ(game.score(), 34);
}

TEST_F(BowlingGameTests, WhenStrikeInLastFrameThenBonusAsExtraTwoRoll)
{
    roll_many(18, 1);
    roll_strike();
    game.roll(6);
    game.roll(7);

    ASSERT_EQ(game.score(), 41);
}

TEST_F(BowlingGameTests, PerfectGame)
{
    for(int i = 0; i < 12; ++i)
        roll_strike();

    ASSERT_EQ(game.score(), 300);
}

struct BowlingGameParams
{
    const char* description;
    std::vector<size_t> rolls;
    unsigned int score;
};

class BowlingGameParamTests : public ::testing::TestWithParam<BowlingGameParams>
{
protected:
    BowlingGame game; // SUT

};

TEST_P(BowlingGameParamTests, RealExamplesWithScore)
{    
    const auto& param = GetParam();
	for (const auto& roll : param.rolls)
		game.roll(roll);

	ASSERT_EQ(game.score(), param.score) << param.description;
}

BowlingGameParams params[] = {
    {"simple game - all ones", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 20},
    {"simple game - different rolls", {0, 8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 27},
    {"strike & spare", {10, 4, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 47},
    { "all spares & strike", {1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 10}, 119}
};

INSTANTIATE_TEST_SUITE_P(PackOfBowlingTests, BowlingGameParamTests, ::testing::ValuesIn(params));

