#ifndef CONSOLE_VISUALIZATION_PLUGIN_HPP
#define CONSOLE_VISUALIZATION_PLUGIN_HPP

#include <string.h>
#include "pluginInterfaces.hpp"

class ConsoleVisualizationPlugin : public VisualizationPluginInterface {
public:
    const char* PluginName() const {
        return "ConsoleVisualizationPlugin";
    };
    int Execute(Result* in);
    ~ConsoleVisualizationPlugin() {};
};

#endif