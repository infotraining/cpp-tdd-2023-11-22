#include "rover.hpp"

using namespace TDD;

Rover::Rover(int x, int y, char dir, const ObstacleDetector* detector)
    : m_x {x}, m_y {y}, m_dir {dir}, m_detector(detector)
{}

int Rover::getX() const
{
    return m_x;
}

int Rover::getY() const
{
    return m_y;
}

char Rover::getDir() const
{
    return m_dir;
}

void Rover::turn(char cmd)
{
    std::vector<char> directions_ {'N', 'E', 'S', 'W'};

    auto act_pos = std::find(directions_.begin(), directions_.end(), m_dir) - directions_.begin();

    if('R' == cmd)
    {
        if(act_pos == 3)
        {
            m_dir = directions_[0];
        }
        else
        {
            m_dir = directions_[++act_pos];
        }
    }
    else if('L' == cmd)
    {
        if(act_pos == 0)
        {
            m_dir = directions_[3];
        }
        else
        {
            m_dir = directions_[--act_pos];
        }
    }
}

void incrementPos(char cmd, int& coord, int inv)
{
    if (cmd == 'F')
    {
        coord += inv;
    }
    else if (cmd == 'B')
    {
        coord -= inv;
    }
}

void Rover::move(char cmd)
{
    m_hitObstacle = false;
    int targetX = m_x;
    int targetY = m_y;
    switch (m_dir)
    {
        case 'N':
        {
            incrementPos(cmd, targetY, 1);
            break;
        }
        case 'E':
        {
            incrementPos(cmd, targetX, 1);
            break;
        }
        case 'W':
        {
            incrementPos(cmd, targetX, -1);
            break;
        }
        case 'S':
        {
            incrementPos(cmd, targetY, -1);
            break;
        }
        default:
            break;
    }
    checkObstacle(targetX, targetY);
    if (!m_hitObstacle)
    {
        m_x = targetX;
        m_y = targetY;
    }
}

void Rover::run(const std::vector<char>& cmds)
{
    for (const auto cmd : cmds)
    {
        switch (cmd)
        {
            case 'F':
            case 'B':
                move(cmd);
                break;
            case 'L':
            case 'R':
                turn(cmd);
                break;
            default:
                break;
        }
    }
}

bool Rover::reportObstacle() const
{
    return m_hitObstacle;
}

void Rover::checkObstacle(int x, int y)
{
    if (m_detector)
    {
        m_hitObstacle = m_detector->is_obstacle(x, y);
    }
}