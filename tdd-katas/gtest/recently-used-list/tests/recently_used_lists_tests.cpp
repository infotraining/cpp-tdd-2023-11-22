#include <algorithm>
#include <string>

#include "recently_used_list.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;
using namespace TDD;


class RecentlyUsedListFixture : public ::testing::Test {
protected:
    StringList list{ 10 };
    RecentlyUsedListFixture() {        
    }

	void addManyStrsToList(uint8_t count)
	{
		for (int i = 0; i < count; i++)
		{
			list.addToList("Test_" + std::to_string(i));
		}
	}
};

struct RUL_DefaultInitialized : RecentlyUsedListFixture
{};

TEST_F(RUL_DefaultInitialized, WhenListDefaultIntializedSizeIsZero)
{
    EXPECT_EQ(list.getList().size(), 0);
}

TEST_F(RUL_DefaultInitialized, WhenStrAddedToListSizeIsIncreasedByOne){
    list.addToList("Test_1");

    EXPECT_EQ(list.getList().size(), 1);
}

struct RUL_WithStrings : RecentlyUsedListFixture
{
    void SetUp() override
    {
        list.addToList("Test_4");
        list.addToList("Test_2");
        list.addToList("Test_3");
    }
};

TEST_F(RUL_WithStrings, WhenManyStringsAddedAreStoredInLIFOOrder)
{    
    EXPECT_THAT(list.getList(), ElementsAre("Test_3", "Test_2", "Test_4"));
}

TEST_F(RUL_WithStrings, WhenReferringByIndexElementIsReturned)
{
    EXPECT_EQ(list.getStrByIndex(1), "Test_2");

}

struct RUL_Duplicates : RUL_WithStrings {};

TEST_F(RUL_Duplicates, WhenDuplicateAddedItIsMovedToBeginning)
{
    list.addToList("Test_2");

	EXPECT_THAT(list.getList(), ElementsAre("Test_2", "Test_3", "Test_4"));
}

TEST_F(RUL_Duplicates, WhenDuplicateAddedSizeIsNotChanged)
{
    auto prev_size = list.getList().size();

    list.addToList("Test_2");

	EXPECT_EQ(list.getList().size(), prev_size);
}

struct RUL_Exceptions : RUL_WithStrings {};

TEST_F(RUL_Exceptions, WhenAddingEmptyStringExceptionIsThrown)
{
    EXPECT_THROW({
        try
        {
             list.addToList("");
        }
        catch (const MyException& e)
        {

            std::cout << "Empty string not allowed";
            throw;
        }
        }, MyException);
}

struct RUL_Overflow : RecentlyUsedListFixture {};

TEST_F(RUL_Overflow, WhenMaxSizeExceededTheOldestIsDropped) {
    uint8_t size = list.getMaxSize();

    addManyStrsToList(size);

    list.addToList("Test_" + std::to_string(size));

    EXPECT_EQ(list.getList().at(size-1), "Test_1");

}

TEST_F(RUL_Overflow, WhenMaxSizeExceededTheNewestIsAdded) {
    uint8_t size = list.getMaxSize();

    addManyStrsToList(size);

    list.addToList("Test_" + std::to_string(size));

    EXPECT_EQ(list.getList().at(0), "Test_" + std::to_string(size));

}

TEST_F(RUL_Overflow, WhenMaxSizeExceededListSizeIsNotChanged) {
    uint8_t size = list.getMaxSize();

    addManyStrsToList(size);

    list.addToList("Test_" + std::to_string(size));

    EXPECT_EQ(list.getList().size(), size);

}
