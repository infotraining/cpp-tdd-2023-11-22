#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <map>

#include "console.hpp"
#include "command.hpp"

class DocEditApp
{
public:
    DocEditApp(Console& console) : console_{console}
    {}

    void run()
    {
        do 
        {
            console_.print("> Enter a command:");
            std::string cmd_text = console_.get_line();
            if (cmd_text == "Exit") break;

            if (commands_.count(cmd_text))
            {
                commands_[cmd_text]->execute();
            }
        } while (true);
    }

    void add_command(std::string name, std::unique_ptr<Command> cmd)
    {
        commands_.emplace(std::move(name), std::move(cmd));
    }
private:
    Console& console_;
    std::map<std::string, std::unique_ptr<Command>> commands_;
};

#endif // APPLICATION_HPP
