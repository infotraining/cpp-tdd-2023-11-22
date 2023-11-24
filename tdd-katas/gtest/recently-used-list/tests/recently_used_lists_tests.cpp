#include <algorithm>
#include <string>
#include <memory>

#include "recently_used_list.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;

class CRecentlyUsedList
{
public:
    CRecentlyUsedList()
    {
    }
    const size_t size() const
    {
        return the_best_list_.size();
    }

    void push(const std::string& elem)
    {
        for (auto it = the_best_list_.begin(); it != the_best_list_.end();++it)
        {
            if (*it == elem)
            {
                the_best_list_.erase(it);
                break;
            }
        } 
    
        the_best_list_.emplace_back(elem);
    }

    string pop()
    {
        if(!the_best_list_.empty())
        {
            string value = the_best_list_[the_best_list_.size()-1];
            the_best_list_.pop_back();
            return  value;
        }
        return {};
    }

    string lookup(const size_t index) const
    {
        if(index < the_best_list_.size())
            return the_best_list_[index];
        return {};
    }
    
private:
    std::vector<std::string>the_best_list_;
};

struct RUL_DefaultInitialized : ::testing::Test
{
    CRecentlyUsedList list;
};

TEST_F(RUL_DefaultInitialized, is_empty)
{
    CRecentlyUsedList list;
    EXPECT_EQ(list.size(), 0);
}

TEST_F(RUL_DefaultInitialized, when_string_is_added_size_is_incremented_by_one)
{
    CRecentlyUsedList list;
    list.push("red");
    EXPECT_EQ(list.size(), 1);
}

struct RUL_WithItems : RUL_DefaultInitialized
{
    void SetUp() override
    {
        list.push("red");
        list.push("blue");
    }
};

TEST_F(RUL_WithItems, last_added_string_is_first_taken_out)
{
    EXPECT_EQ(list.pop(), "blue");
    EXPECT_EQ(list.pop(), "red");
}

TEST_F(RUL_WithItems, return_empty_string_if_list_is_empty)
{
    list.pop();
    list.pop();
    EXPECT_EQ(list.pop(), string(""));
}

struct RUL_Indexing : RUL_WithItems {};

TEST_F(RUL_Indexing, list_element_can_be_accessed_by_zero_based_index)
{
    list.push("green");
    EXPECT_EQ(list.lookup(1), "blue");
}

TEST_F(RUL_Indexing, lookup_return_empty_string_for_invalid_index)
{
    CRecentlyUsedList list;
    EXPECT_EQ(list.lookup(0), "");
    list.push("red");
    list.push("blue");
    EXPECT_EQ(list.lookup(2), "");
    EXPECT_EQ(list.lookup(-1), "");
}

struct RUL_Duplicates : RUL_WithItems {};

TEST_F(RUL_Duplicates, inserting_duplicate_size_is_not_changed)
{
    list.push("red");

    EXPECT_EQ(list.size(), 2);
}

TEST_F(RUL_Duplicates, duplicate_is_moved_to_front)
{
    list.push("red");

    EXPECT_EQ(list.lookup(0), "blue");
    EXPECT_EQ(list.lookup(1), "red");
}
