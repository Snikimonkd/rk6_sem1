#ifndef FILE_VISUALIZATION_PLUGIN_HPP
#define FILE_VISUALIZATION_PLUGIN_HPP

#include <string.h>
#include "pluginInterfaces.hpp"

class FileVisualizationPlugin : public VisualizationPluginInterface {
public:
    const char* PluginName() const {
        return "FileVisualizationPlugin";
    };
    int Execute(Result* in);
    ~FileVisualizationPlugin() {};
};

#endif