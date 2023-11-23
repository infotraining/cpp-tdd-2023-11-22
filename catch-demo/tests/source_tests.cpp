#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include "source.hpp"

using namespace std;

TEST_CASE("pushing items to vector", "[vector][push]")
{
    vector<int> vec{ 3, 2 };

    CHECK(vec.size() == 1);
    CHECK(vec.capacity() == 1);

    SECTION("push_back")
    {               
        vec.push_back(1);

        SECTION("size is increased")
        {
            REQUIRE(vec.size() == 2);
        }

        SECTION("capacity can be increased")
        {
            REQUIRE(vec.capacity() >= 2);
        }
    }

    SECTION("clear")
    {
        vec.clear();

        SECTION("size is zero")
        {
            REQUIRE(vec.size() == 0);
        }

        SECTION("capacity is not changed")
        {
            REQUIRE(vec.capacity() == 0);
        }
    }
}

SCENARIO("pushing items to vector")
{
    GIVEN("vector with data")          // Arrange - fixture
    {
        vector<int> vec = { 1, 2, 3 };
        auto prev_size = vec.size();

        WHEN("push_back() an item")    // Act
        {
            vec.push_back(4);

            THEN("size is increased")  // Assert
            {
                REQUIRE(vec.size() - prev_size == 1);
            }

            THEN("capacity can be increased")
            {
                REQUIRE(vec.capacity() >= 2);
            }
        }
    }
}

TEST_CASE("Parametrized test")
{
	auto param = GENERATE(
		std::tuple(std::vector{1, 2, 3}, 3),
		std::tuple(std::vector{1, 2, 3, 4}, 4),
		std::tuple(std::vector{1, 2, 3, 4, 5}, 5)
	);

	auto [vec, expected_size] = param;

	REQUIRE(vec.size() == expected_size);
}

TEST_CASE("<> in REQUIRE")
{
    std::vector<int> vec = {1, 2, 3};
    REQUIRE(vec == std::vector<int>{1, 2, 3});
}