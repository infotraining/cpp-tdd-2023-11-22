#ifndef BOWLING_HPP
#define BOWLING_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <array>

namespace TDD
{
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
}

#endif