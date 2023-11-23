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

TEST_CASE("When rover is created it has given position and direction", "[creation]")
{
    Rover rover(0,0,'N',nullptr);

    REQUIRE(rover.getX() == 0);
    REQUIRE(rover.getY() == 0);
    REQUIRE(rover.getDir() == 'N');
}

TEST_CASE("When turn do not change position", "[turn]")
{
    Rover rover(10, 20, 'N', nullptr);

    rover.turn('R');

    REQUIRE(rover.getX() == 10);
    REQUIRE(rover.getY() == 20);
}

TEST_CASE("When invalid turn cmd do not turn or move", "[turn]")
{
    Rover rover(10, 20, 'N', nullptr);

    rover.turn('X');

    REQUIRE(rover.getX() == 10);
    REQUIRE(rover.getY() == 20);
    REQUIRE(rover.getDir() == 'N');
}

TEST_CASE("When turn change direction", "[turn]")
{
    auto [startDir, turn, expectedDir]  = GENERATE(
        std::tuple{'N', 'L', 'W'},
        std::tuple('N', 'R', 'E'),
        std::tuple{'E', 'L', 'N'},
        std::tuple{'E', 'R', 'S'},
        std::tuple{'S', 'L', 'E'},
        std::tuple{'S', 'R', 'W'},
        std::tuple{'W', 'L', 'S'},
        std::tuple{'W', 'R', 'N'}
    );

    DYNAMIC_SECTION("When turning " << turn << " from dir "
                                    << startDir 
                                    << " direction is " 
                                    << expectedDir)
    {
        Rover rover(0, 0, startDir, nullptr);

        rover.turn(turn);

        CHECK(rover.getDir() == expectedDir);
    }
}

TEST_CASE("When move change position accordingly to direction", "[movement]")
{
    auto [startPosX, startPosY, startDir, move, expectedPosX, expectedPosY]  = GENERATE(
        std::tuple{0, 0, 'N', 'F', 0, 1},
        std::tuple{0, 0, 'E', 'F', 1, 0},
        std::tuple{0, 0, 'S', 'F', 0, -1},
        std::tuple{0, 0, 'W', 'F', -1, 0},
        std::tuple{0, 0, 'N', 'B', 0, -1},
        std::tuple{0, 0, 'E', 'B', -1, 0},
        std::tuple{0, 0, 'S', 'B', 0, 1},
        std::tuple{0, 0, 'W', 'B', 1, 0}
    );

    DYNAMIC_SECTION("When moving " << move << " from position: X:" << startPosX <<", Y:" << startPosY << ", direction: " << startDir
                                << " expected position is: X:" << expectedPosX <<", Y:" << expectedPosY)
    {
        Rover rover(startPosX, startPosY, startDir, nullptr);

        rover.move(move);

        CHECK(rover.getX() == expectedPosX);
        CHECK(rover.getY() == expectedPosY);
    }
}

TEST_CASE("When move do not change direction", "[movement]")
{
    Rover rover(10, 20, 'N', nullptr);

    rover.move('F');

    REQUIRE(rover.getDir() == 'N');
}

TEST_CASE("When invalid move cmd do not turn or move", "[movement]")
{
    Rover rover(10, 20, 'N', nullptr);

    rover.move('X');

    REQUIRE(rover.getX() == 10);
    REQUIRE(rover.getY() == 20);
    REQUIRE(rover.getDir() == 'N');
}

TEST_CASE("When given array of cmds rover change it's possition", "[position]")
{
    Rover rover(0, 0, 'N', nullptr);

    rover.run({'R', 'R', 'F', 'F', 'B', 'L'});

    REQUIRE(rover.getX() == 0);
    REQUIRE(rover.getY() == -1);
    REQUIRE(rover.getDir() == 'E');
}

TEST_CASE("When given array of invalid cmds rover doen't change it's possition", "[position]")
{
    Rover rover(1, 2, 'W', nullptr);

    rover.run({'X', 'Y', 'Z'});

    REQUIRE(rover.getX() == 1);
    REQUIRE(rover.getY() == 2);
    REQUIRE(rover.getDir() == 'W');
}

class TestObstacleDetector : public ObstacleDetector
{
public:
    TestObstacleDetector(bool reportObstacle) : m_reportObstacle (reportObstacle)
    {}

    bool is_obstacle(int x, int y) const
    {
        return m_reportObstacle;
    }
    
    void set_ReportObstacle(bool reportObstacle)
    {
        m_reportObstacle = reportObstacle;
    }
private:
    bool m_reportObstacle;
};

TEST_CASE("When no obstacle rover moves on", "[obsatcle]")
{
    TestObstacleDetector obstacle_detector(false);

    Rover rover(0, 0, 'N', &obstacle_detector);

    rover.run({'F', 'F'});

    REQUIRE(rover.getX() == 0);
    REQUIRE(rover.getY() == 2);
    REQUIRE(rover.getDir() == 'N');
}

TEST_CASE("When obstacle rover doesn't moves on", "[obsatcle]")
{
    TestObstacleDetector obstacle_detector(true);

    Rover rover(0, 0, 'N', &obstacle_detector);

    rover.run({'F', 'F'});

    REQUIRE(rover.getX() == 0);
    REQUIRE(rover.getY() == 0);
    REQUIRE(rover.getDir() == 'N');
}

TEST_CASE("When obstacle rover reports it", "[obsatcle]")
{
    TestObstacleDetector obstacle_detector(true);
    Rover rover(0, 0, 'N', &obstacle_detector);
    rover.run({'F', 'F'});

    REQUIRE(rover.reportObstacle() == true);
    REQUIRE(rover.getX() == 0);
    REQUIRE(rover.getY() == 0);
    REQUIRE(rover.getDir() == 'N');
}

TEST_CASE("When new run commands is provided obstacle report is cleared", "[obsatcle]")
{
    TestObstacleDetector obstacle_detector(true);
    Rover rover(0, 0, 'N', &obstacle_detector);
    rover.run({'F', 'F'});
    obstacle_detector.set_ReportObstacle(false);
    rover.run({'F', 'F'});

    REQUIRE(rover.reportObstacle() == false);
    REQUIRE(rover.getX() == 0);
    REQUIRE(rover.getY() == 2);
    REQUIRE(rover.getDir() == 'N');
}
