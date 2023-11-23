#include "rover.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/trompeloeil.hpp>
#include <ranges>
#include <string>

using namespace std;
using namespace TDD;

TEST_CASE("initialization-position-constructor")
{
    auto position = GENERATE(
        Position{0, 0, N},
        Position{22, 22, W});
    DYNAMIC_SECTION("Params, x: " << position.x << ", y: " << position.y << ", dir: " << position.dir)
    {
        Rover rover(position);
        REQUIRE(rover.getPosition() == position);
    }
}

TEST_CASE("initialization-xyz-constructor")
{
    auto position = GENERATE(
        Position{0, 0, N},
        Position{22, 22, W});

    DYNAMIC_SECTION("Params, x: " << position.x << ", y: " << position.y << ", dir: " << position.dir)
    {
        Rover rover(position.x, position.y, position.dir);
        REQUIRE(rover.getPosition() == position);
    }
}

TEST_CASE("MOVE_FORWARD_AND_BACKWARD")
{
    auto [command, result] = GENERATE(
        std::tuple(std::vector{F}, Position(0, 1, N)),
        std::tuple(std::vector{F, B}, Position(0, 0, N)),
        std::tuple(std::vector{B}, Position(0, 100, N)),
        std::tuple(std::vector{F, F, F}, Position(0, 3, N))

    );

    DYNAMIC_SECTION("Commands" << std::string(command.begin(), command.end()))
    {

    Rover rover(0, 0, N);
    rover.process_command(command);
    REQUIRE(rover.getPosition() == result);
    }
}

TEST_CASE("MOVE_LEFT_AND_RIGHT")
{
    auto [command, result] = GENERATE(
        std::tuple(std::vector{L}, Position(0, 0, W)),
        std::tuple(std::vector{L, R}, Position(0, 0, N)),
        std::tuple(std::vector{R}, Position(0, 0, E)),
        std::tuple(std::vector{L, L, L}, Position(0, 0, E))

    );

    DYNAMIC_SECTION("Commands" << std::string(command.begin(), command.end()))
    {

    Rover rover(0, 0, N);
    rover.process_command(command);
    REQUIRE(rover.getPosition() == result);
    }
}

TEST_CASE("WARP_EDGES")
{
    auto [command, result] = GENERATE(
        std::tuple(std::vector{B}, Position(0, 100, N)),
        std::tuple(std::vector{L, F}, Position(100, 0, W)),
        std::tuple(std::vector{B, F}, Position(0, 0, N)));

    DYNAMIC_SECTION("Commands" << std::string(command.begin(), command.end()))
    {

    Rover rover(0, 0, N);
    rover.process_command(command);
    REQUIRE(rover.getPosition() == result);
    }
}

TEST_CASE("WORLD_EMPTY_TEST")
{
    World world({Position(0, 1, N)});

    REQUIRE(world.isEmpty(Position(0, 1, N)) == false);
    REQUIRE(world.isEmpty(Position(0, 0, N)) == true);
}

TEST_CASE("OBJECT_TEST")
{
    auto [command, result, block] = GENERATE(
        std::tuple(std::vector{F}, Position(0, 0, N), std::vector{Position(0, 1, N)}),
        std::tuple(std::vector{F, F, F, F, F, F}, Position(0, 3, N), std::vector{Position(0, 4, N)})

    );

    DYNAMIC_SECTION("Commands" << std::string(command.begin(), command.end()))
    {
    auto world = std::make_shared<World>(block);

    Rover rover(0, 0, N, world);
    REQUIRE(rover.process_command(command) == false);
    REQUIRE(rover.getPosition() == result);
    }
}

/*
 N          +
W E        - +
 S          -
*/

// | y _ x