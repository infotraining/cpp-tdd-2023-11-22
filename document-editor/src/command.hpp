#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "clipboard.hpp"
#include "console.hpp"
#include "document.hpp"
#include <memory>
#include <stack>

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class PrintCmd : public Command
{
    Document& doc_;
    Console& console_;
public:
    PrintCmd(Document& doc, Console& console) : doc_{doc}, console_{console}
    {        
    }

    void execute() override
    {
        std::string content = "[" + doc_.text() + "]";
        console_.print(content);
    }
};

class AddCmd : public Command
{
    Document& doc_;
    Console& console_;
public:
    AddCmd(Document& doc, Console& console) : doc_{doc}, console_{console}
    {        
    }

    void execute() override
    {
        console_.print("AddText:");
        std::string content = console_.get_line();
        doc_.add_text(content);
    }
};


#endif // COMMAND_HPP
