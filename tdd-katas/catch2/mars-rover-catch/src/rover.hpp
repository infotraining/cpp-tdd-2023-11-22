#ifndef ROVER_HPP
#define ROVER_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <ranges>
#include <string>
#include <vector>
#include <memory>


namespace TDD
{
    class World;
    enum Direction
    {
        N,
        W,
        S,
        E,
    };

    struct Position
    {
        int x;
        int y;
        Direction dir;

        bool operator==(const Position& rhs) const = default;

        friend std::ostream& operator<<(std::ostream& out, const Position& pos)
        {
            return out << "Position{" << pos.x << ", " << pos.y << ", " << pos.dir << "}'";
        }
    };

    enum Command
    {
        F,
        B,
        L,
        R
    };

    class Rover
    {
    public:
        Rover(int x, int y, Direction dir, std::optional<std::shared_ptr<World>> = std::nullopt);
        Rover(Position pos, std::optional<std::shared_ptr<World>> = std::nullopt);
        Position getPosition();
        bool process_command(std::vector<Command> commands);
    private:
        Position pos;
        std::optional<std::shared_ptr<World>> m_world;
    };



    class World
    {
        public:
        World(std::vector<Position> obstacle)
        {   
            m_obstacle = obstacle;
        }

        bool isEmpty(Position objects){
            for (const auto& objB : m_obstacle) {
                    if (objects.x == objB.x && objects.y == objB.y) {
                        return false; 
                    }
                }
            return true; 
       
        }
        private:
        std::vector<Position> m_obstacle; 
    };
};



#endif