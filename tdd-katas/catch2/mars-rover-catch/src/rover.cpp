#include "rover.hpp"
#include <stdlib.h>

using namespace TDD;

Rover::Rover(int x, int y, Direction dir, std::optional<std::shared_ptr<World>> world)
{
    pos.x = x;
    pos.y = y;
    pos.dir = dir;
    m_world = world;
}

Rover::Rover(Position pos, std::optional<std::shared_ptr<World>> world)
{
    this->pos = pos;
    m_world = world;
}

Position Rover::getPosition()
{
    return pos;
}

int math_mod(int a, int b)
{
    return (b + (a % b)) % b;
}

bool Rover::process_command(std::vector<Command> commands)
{

    for (auto command : commands)
    {
        int x_, y_;
        switch (this->pos.dir)
        {
        case N:
            x_ = 0;
            y_ = 1;
            break;
        case E:
            x_ = 1;
            y_ = 0;
            break;
        case W:
            x_ = -1;
            y_ = 0;
            break;
        case S:
            x_ = 0;
            y_ = -1;
            break;
        }
        int new_x = this->pos.x;
        int new_y = this->pos.y;

        switch (command)
        {
        case F:
            new_x += x_;
            new_y += y_;

            break;
        case B:
            new_x -= x_;
            new_y -= y_;

            break;
        case L:
        {
            auto tmp = static_cast<int>(pos.dir);
            tmp = (tmp + 1) % 4;
            this->pos.dir = static_cast<Direction>(tmp);
            break;
        }
        case R:
        {
            auto tmp = static_cast<int>(pos.dir);
            tmp = (4 + (tmp - 1) % 4) % 4;
            this->pos.dir = static_cast<Direction>(tmp);
            break;
        }
        }

        new_x = math_mod(new_x, 101);
        new_y = math_mod(new_y, 101);

        if (this->m_world && this->m_world->get()->isEmpty(Position(new_x, new_y, W)) == false)
        {
            return false;
        }
        
        this->pos.x = new_x;
        this->pos.y = new_y;
    }
    return true;
}
