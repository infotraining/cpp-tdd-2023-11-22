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
    class ObstacleDetector
    {
    public:
        virtual bool is_obstacle(int x, int y) const = 0;
    };
    
    class PlanetInterface
    {
        virtual std::tuple<int, int, char> getEndPosition(
            char currentDir, int currentX,
            int currentY, char targetDir) = 0;
    };

    class Rover
    {
    public:
        Rover(int x, int y, char dir, const ObstacleDetector* detector);
        void run(const std::vector<char>& cmds);
        void move(char cmd);
        void turn(char cmd);
        int getX() const;
        int getY() const;
        char getDir() const;
        bool reportObstacle() const;
    private:
        int m_x;
        int m_y;
        char m_dir;
        bool m_hitObstacle = false;
        const ObstacleDetector* m_detector;
        void checkObstacle(int x, int y);
    };

    
}

#endif