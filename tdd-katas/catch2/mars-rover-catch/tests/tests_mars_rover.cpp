#include "rover.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/trompeloeil.hpp>
#include <ranges>
#include <string>
#include <iostream>

using namespace std;
using namespace TDD;

class ObstacleDetector
{
public:
    virtual bool isObstacle(uint32_t x, uint32_t y) = 0;
};


struct MockObstacleDetector : public ObstacleDetector
{
    MockObstacleDetector(std::vector<std::pair<uint32_t, uint32_t>> _obstacles = {}): obstacles(_obstacles) {};
    std::vector<std::pair<uint32_t, uint32_t>> coords;

    bool isObstacle(uint32_t x, uint32_t y)
    {
        coords.push_back(std::pair(x, y));
        return obstacles.cend() != std::find_if(obstacles.cbegin(), obstacles.cend(), 
            [x,y](const std::pair<uint32_t, uint32_t>& obstacle)
            {
                return x==obstacle.first && y==obstacle.second;
            });
    }

    std::vector<std::pair< uint32_t, uint32_t>> obstacles;
};


class Planet
{
public:
    Planet(uint32_t _sizeX, uint32_t _sizeY, std::vector<std::pair< uint32_t, uint32_t>> _obstacles = {}) : sizeX(_sizeX), sizeY(_sizeY), obstacles(_obstacles){}
    ~Planet() = default;

    std::pair<uint32_t, uint32_t> getMaxXY() const { return {sizeX-1, sizeY-1};}
    bool isObstacle(uint32_t x, uint32_t y) const
    {
        return obstacles.cend() != std::find_if(obstacles.cbegin(), obstacles.cend(), 
            [x,y](const std::pair<uint32_t, uint32_t>& obstacle)
            {
                return x==obstacle.first && y==obstacle.second;
            });
    }
private:
    uint32_t sizeX;
    uint32_t sizeY;
    std::vector<std::pair< uint32_t, uint32_t>> obstacles;
};

class MarsRover 
{
public:
    struct State
    {
        uint32_t x;
        uint32_t y;
        char direction;

        State(uint32_t _x, uint32_t _y, char _direction) : x(_x), y(_y), direction(std::toupper(_direction)){}

        friend std::ostream& operator<<(std::ostream& out, const State& state)
        {
            return out << "State{" << state.x << ", " << state.y << ", " << state.direction << "}";
        }

        bool operator==(const State& other) const = default;
    };
    MarsRover(const State& _state, const Planet& _planet, ObstacleDetector& _obstacleDetector) : state(_state), planet(_planet), obstacleDetector(_obstacleDetector)  {};
    ~MarsRover() = default;
    const State& getState() { return state;}
    State command(const std::string& commands) 
    {
        for (const char command : commands)
        {
            auto res = true;
            
            switch(std::toupper(command))
            {
                case 'F': 
                    res = stepForward();
                break;
                case 'B': 
                    res = stepBackward();
                break;
                case 'L': 
                    rotateLeft();
                break;
                case 'R':
                    rotateRight();
                break;
            }
            if(!res)
            {
                break;
            }
        }
        return state;
    };
    
    bool stepForward()
    {
        auto [maxX, maxY] = planet.getMaxXY();
        auto newX = state.x;
        auto newY= state.y;
        switch (state.direction)
        {
            case 'N':
                newY--;
                break;
            case 'S':
                newY++;
                break;
            case 'W':
                newX--;
                break;
            case 'E':
                newX++;
                break;
        }
        if(std::numeric_limits<uint32_t>::max() == newX) newX = maxX;
        else if (maxX+1 == newX) newX = 0;
        if(std::numeric_limits<uint32_t>::max() == newY) newY = maxY;
        else if (maxY+1 == newY) newY = 0;

        if(!obstacleDetector.isObstacle(newX, newY))
        {
            state.x = newX;
            state.y = newY;
            return true;
        }
        return false;
    }

    bool stepBackward()
    {
        rotateLeft();
        rotateLeft();
        auto res = stepForward();
        rotateRight();
        rotateRight();

        return res;
    }

    void rotateLeft()
    {
        switch (state.direction)
        {
            case 'N': state.direction = 'W'; break;
            case 'W': state.direction = 'S'; break;
            case 'S': state.direction = 'E'; break;
            case 'E': state.direction = 'N'; break;
        }
    }

    void rotateRight()
    {
        switch (state.direction)
        {
            case 'N': state.direction = 'E'; break;
            case 'W': state.direction = 'N'; break;
            case 'S': state.direction = 'W'; break;
            case 'E': state.direction = 'S'; break;
        }
    }

private:
    State state;
    const Planet& planet;
    ObstacleDetector& obstacleDetector;
};

TEST_CASE("MarsRover")
{
    SECTION("after init - reports initial state")
    {
        auto [x, y, direction] = GENERATE(std::tuple{1u, 1u, 'W'}, std::tuple{0u, 0u, 'N'}, std::tuple{0u, 0u, 'S'}, std::tuple{0u, 0u, 'E'});   
        
        Planet planet{10, 10};
        MockObstacleDetector mockObstacleDetector;
        MarsRover r({x, y, direction}, planet, mockObstacleDetector);

        REQUIRE(r.getState() == MarsRover::State{x,y, direction});
    }   

    SECTION("Basic moves")
    {
        Planet planet{10, 10};
        MockObstacleDetector mockObstacleDetector;
        auto [initState, command, expectedState] = GENERATE(
            std::tuple{MarsRover::State{1u, 1u, 'N'}, "F", MarsRover::State{1u, 0u, 'N'}},
            std::tuple{MarsRover::State{1u, 1u, 'W'}, "F", MarsRover::State{0u, 1u, 'W'}},
            std::tuple{MarsRover::State{1u, 1u, 's'}, "F", MarsRover::State{1u, 2u, 'S'}},
            std::tuple{MarsRover::State{1u, 1u, 'E'}, "F", MarsRover::State{2u, 1u, 'E'}},

            std::tuple{MarsRover::State{2u, 2u, 'N'}, "B", MarsRover::State{2u, 3u, 'N'}},
            std::tuple{MarsRover::State{2u, 2u, 'W'}, "B", MarsRover::State{3u, 2u, 'W'}},
            std::tuple{MarsRover::State{2u, 2u, 's'}, "B", MarsRover::State{2u, 1u, 'S'}},
            std::tuple{MarsRover::State{2u, 2u, 'E'}, "B", MarsRover::State{1u, 2u, 'E'}},

            std::tuple{MarsRover::State{3u, 3u, 'N'}, "L", MarsRover::State{3u, 3u, 'W'}},
            std::tuple{MarsRover::State{3u, 3u, 'W'}, "L", MarsRover::State{3u, 3u, 'S'}},
            std::tuple{MarsRover::State{3u, 3u, 's'}, "L", MarsRover::State{3u, 3u, 'E'}},
            std::tuple{MarsRover::State{3u, 3u, 'E'}, "L", MarsRover::State{3u, 3u, 'N'}},

            std::tuple{MarsRover::State{4u, 4u, 'N'}, "R", MarsRover::State{4u, 4u, 'E'}},
            std::tuple{MarsRover::State{4u, 4u, 'W'}, "R", MarsRover::State{4u, 4u, 'N'}},
            std::tuple{MarsRover::State{4u, 4u, 's'}, "R", MarsRover::State{4u, 4u, 'W'}},
            std::tuple{MarsRover::State{4u, 4u, 'E'}, "R", MarsRover::State{4u, 4u, 'S'}}
            );   

        MarsRover r(initState, planet, mockObstacleDetector);
        r.command(command);
        REQUIRE(r.getState() == expectedState);
    }
}

TEST_CASE("Wrapping edges")
{
    Planet planet(10u,10u);
    MockObstacleDetector mockObstacleDetector;
    auto [initState, command, expectedState] = GENERATE(
        std::tuple{MarsRover::State{0u, 2u, 'W'}, "F", MarsRover::State{9u, 2u,'W'}},
        std::tuple{MarsRover::State{9u, 3u, 'E'}, "F", MarsRover::State{0u, 3u,'E'}},
        std::tuple{MarsRover::State{4u, 0u, 'N'}, "F", MarsRover::State{4u, 9u,'N'}},
        std::tuple{MarsRover::State{5u, 9u, 'S'}, "F", MarsRover::State{5u, 0u,'S'}}
        );   

    MarsRover r(initState, planet, mockObstacleDetector);
    r.command(command);
    REQUIRE(r.getState() == expectedState);
}

TEST_CASE("Planet obstacles")
{
    Planet planet(5u, 5u, {{1u,2u}, {3u,4u}});
    for (auto x = 0u; x < 5u; ++x)
    {
        for (auto y = 0u; y < 5u; ++y)
        {
            REQUIRE(planet.isObstacle(x,y) == ((x==1u && y==2u) || (x==3u && y==4u)));
        }
    }
}

TEST_CASE("Rover Obstacle")
{
    Planet planet(10u,10u);
    
    SECTION("no obstacle")
    {
        MockObstacleDetector obstacle_detector{};

        MarsRover r(MarsRover::State{2u, 2u, 'N'}, planet, obstacle_detector);

        r.command("F");
        SECTION("rover moves on")
        {
            REQUIRE(r.getState() == MarsRover::State{2u, 1u, 'N' });
        }

        SECTION("asks for obstacle")
        {
            REQUIRE(obstacle_detector.coords.size() == 1);
            REQUIRE(obstacle_detector.coords.back() == std::pair{2u, 1u});

            r.command("F");
            SECTION("rover moves on one more time")
            {
                REQUIRE(r.getState() == MarsRover::State{2u, 0u, 'N' });
            }

            SECTION("asks for obstacle one more time")
            {
                REQUIRE(obstacle_detector.coords.size() == 2);
                REQUIRE(obstacle_detector.coords.back() == std::pair{2u, 0u});

            }
        }
    }
    SECTION("obstacle")
    {
        MockObstacleDetector obstacle_detector{{{1u, 9u}}};
        MarsRover r(MarsRover::State{2u, 9u, 'W'}, planet, obstacle_detector);

       
        SECTION("rover moves on into the obstacle")
        {
            REQUIRE(r.command("F") == MarsRover::State{2u, 9u, 'W' });    

            SECTION("asks for obstacle")
            {
                REQUIRE(obstacle_detector.coords.size() == 1);
                REQUIRE(obstacle_detector.coords.back() == std::pair{1u, 9u});
            }
        }

        SECTION("rover moves on into the obstacle in the midle of the sequence")
        {
            REQUIRE(r.command("BLLBBRBLRLFFFFFF") == MarsRover::State{2u, 9u, 'E' });    

            SECTION("asks for obstacle")
            {
                REQUIRE(obstacle_detector.coords.size() == 3);
                REQUIRE(obstacle_detector.coords.back() == std::pair{1u, 9u});
            }
        }

    }
}
