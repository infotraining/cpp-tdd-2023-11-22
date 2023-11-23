#include "recently_used_list.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <deque>
#include <stdexcept>

using namespace ::testing;

class RecentlyUsedList
{
private:
    int max_capacity_;
    std::deque<std::string> strList_{};

public:
    RecentlyUsedList(int max_capacity = 5)
    {
        max_capacity_ = max_capacity;
        strList_ = {};
    }

    int size() const
    {
        return strList_.size();
    }

    void add(const std::string& str)
    {
        if (str.empty())
        {
            return;
        }

        // Avoiding duplicates
        auto it = std::find(strList_.cbegin(), strList_.cend(), str);

        // Removing duplicate from previous position
        if (it != strList_.cend())
        {
            strList_.erase(it);
        }

        strList_.push_front(str);

        // Removing last element if max_capacity reached
        if (strList_.size() > max_capacity_)
        {
            strList_.erase(strList_.end() - 1);
        }
    }

    const std::string& operator[](int index) const
    {
        return strList_.at(index);
    }

    std::string& operator[](int index)
    {
        return strList_.at(index);
    }
};

class RecentlyUsedListTest : public ::testing::Test
{
protected:
    RecentlyUsedList list;
};

struct RUL_DefaultInitialized : RecentlyUsedListTest
{
};

TEST_F(RUL_DefaultInitialized, ListIsEmpty)
{
    EXPECT_EQ(list.size(), 0);
}

TEST_F(RUL_DefaultInitialized, WhenItemAddedListIsNotEmpty)
{
    std::string item = "aaa";
    list.add(item);
    EXPECT_EQ(list.size(), 1);
}

struct RUL_WithItems : RecentlyUsedListTest
{
    void SetUp() override
    {
        list.add("aaa");
        list.add("bbb");
    }
};

// LIFO Order a, b, c -> c, b, a

TEST_F(RUL_WithItems, WhenAddingDuplicateSizeIsNotChanged)
{
    size_t prev_size = list.size();
    list.add("bbb");
    EXPECT_EQ(list.size(), prev_size);
}

struct RUL_Indexing : RUL_WithItems
{
};

TEST_F(RUL_Indexing, ElementsCanBeLookedUpByTheirIndices)
{
    EXPECT_EQ(list[0], "bbb");
}

TEST_F(RUL_Indexing, WhenDuplicateAddedLIFOOrderRearranged)
{
    EXPECT_EQ(list[1], "aaa");
    list.add("aaa");
    EXPECT_EQ(list[0], "aaa");
    EXPECT_EQ(list.size(), 2);
}

TEST_F(RUL_Indexing, ExceptionIsRaisedWhenGivenIndexGreaterThanListSize)
{
    EXPECT_THROW(list[3], std::out_of_range);
}

TEST_F(RUL_DefaultInitialized, NullStringCannotBeAdded)
{
    list.add("");
    EXPECT_EQ(list.size(), 0);
}

struct RUL_params
{
    const char* description;
    std::vector<std::string> items;
    std::tuple<int, std::string, std::string> expected;
};

class RUL_WithItemsParamTests : public ::testing::TestWithParam<RUL_params>
{
protected:
    RecentlyUsedList list{5};
};

RUL_params params[] = {
    {"List with max capacity", {"aba", "aca", "ada", "aea", "afa", "aga"}, std::tuple{5, "aga", "aca"}},
    {"Adding 4 elements", {"ada", "aea", "afa", "aga"}, std::tuple{4, "aga", "ada"}},
    {"Adding 4 elements with one duplicate", {"aba", "aca", "aba", "aea"}, std::tuple{3, "aea", "aca"}},
    {"Adding 4 elements with one empty string", {"aba", "aca", "aea", ""}, std::tuple{3, "aea", "aba"}}};

TEST_P(RUL_WithItemsParamTests, ListWithSetMaxCapacityParams)
{
    const auto& param = GetParam();
    for (const auto& item : param.items)
        list.add(item);

    auto [size, first, last] = param.expected;

    EXPECT_EQ(list.size(), size); // Assert
    EXPECT_EQ(list[0], first);
    EXPECT_EQ(list[size - 1], last);
}

INSTANTIATE_TEST_SUITE_P(PackOfTests, RUL_WithItemsParamTests, ::testing::ValuesIn(params));

// TEST(RecentlyUsedList, ListWithSetMaxCapacity)
// {
//     const auto capacity = 5;                 // Arrange
//     RecentlyUsedList list{5};

//     list.add("aba");                         // Act
//     list.add("aca");
//     list.add("ada");
//     list.add("aea");
//     list.add("afa");
//     list.add("aga");

//     EXPECT_EQ(list.size(), capacity);        // Assert
//     EXPECT_EQ(list[0], "aga");
//     EXPECT_EQ(list[capacity - 1], "aca");
// }
