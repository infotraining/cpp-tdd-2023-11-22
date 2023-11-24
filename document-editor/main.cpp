#include <iostream>

#include "application.hpp"
#include "command.hpp"
#include "console.hpp"
#include <boost/di.hpp>

using namespace std;
namespace di = boost::di;

int main()
{
    // Terminal console;
    // Document doc;

    // DocEditApp app{console};

    // auto print_cmd = std::make_unique<PrintCmd>(doc, console);
    // auto add_cmd = std::make_unique<AddCmd>(doc, console);

    // app.add_command("Print", std::move(print_cmd));
    // app.add_command("AddText", std::move(add_cmd));

    // app.run();

    auto injector = di::make_injector(
        di::bind<Console>().to<Terminal>()
    );

    DocEditApp app = injector.create<DocEditApp>();

    auto print_cmd = injector.create<std::unique_ptr<PrintCmd>>();
    auto add_cmd = injector.create<std::unique_ptr<AddCmd>>();

    app.add_command("Print", std::move(print_cmd));
    app.add_command("AddText", std::move(add_cmd));

    app.run();
}