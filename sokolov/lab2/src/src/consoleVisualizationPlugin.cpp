#include "pluginInterfaces.hpp"
#include "consoleVisualizationPlugin.hpp"

#include <iostream>

int ConsoleVisualizationPlugin::Execute(Result* in) {
    std::cout << "ConsoleVisualizationPlugin is executed\n";
    size_t it = 1;
    for (auto &v : in->ret) {
        int err = printf("%zu,\t%f,\t%f,\t%f\n", v.t, v.x1, v.x2, v.x3);
        if (err < 0) {
            return err;
        }
    }

    return 0;
}

extern "C" BasePluginInterface* registerPlugin()
{
    std::cout << "ConsoleVisualizationPlugin is loaded\n";
    return new ConsoleVisualizationPlugin();
}