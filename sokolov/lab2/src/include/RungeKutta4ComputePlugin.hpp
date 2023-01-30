#ifndef RUNGE_KUTTA_4_COMPUTE_PLUGIN_HPP
#define RUNGE_KUTTA_4_COMPUTE_PLUGIN_HPP

#include <string.h>
#include "pluginInterfaces.hpp"

class RungeKutta4ComputePlugin : public ComputePluginInterface {
public:
    const char* PluginName() const {
        return "RungeKutta4ComputePlugin";
    };
    int Execute(Result* in);
    ~RungeKutta4ComputePlugin() {};
};

#endif