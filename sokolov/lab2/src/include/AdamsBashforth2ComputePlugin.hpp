#ifndef ADAMS_BASHFORTH_2_COMPUTE_PLUGIN_HPP
#define ADAMS_BASHFORTH_2_COMPUTE_PLUGIN_HPP

#include <string.h>
#include "pluginInterfaces.hpp"

class AdamsBashforth2ComputePlugin : public ComputePluginInterface {
public:
    const char* PluginName() const {
        return "AdamsBashforth2ComputePlugin";
    };
    int Execute(Result* in);
    ~AdamsBashforth2ComputePlugin() {};
};

#endif