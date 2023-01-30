#include "pluginInterfaces.hpp"
#include "fileVisualizationPlugin.hpp"

#include <iostream>

int FileVisualizationPlugin::Execute(Result* in) {
    std::cout << "FileVisualizationPlugin is executed\n";
    FILE *out = fopen(in->methodName, "w+");
    if (out == NULL) {
        return errno;
    }

    int err = fprintf(out, "%s\n", in->methodName);
    if (err < 0) {
        return err;
    }

    for (auto &v : in->ret) {
        int err = fprintf(out, "%zu,\t%f,\t%f,\t%f\n", v.t, v.x1, v.x2, v.x3);
        if (err < 0) {
            return err;
        }
    }

    return 0;
}

extern "C" BasePluginInterface* registerPlugin()
{
    std::cout << "FileVisualizationPlugin is loaded\n";
    return new FileVisualizationPlugin();
}