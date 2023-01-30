#ifndef ADAMS_BASHFORTH_4_COMPUTE_PLUGIN_HPP
#define ADAMS_BASHFORTH_4_COMPUTE_PLUGIN_HPP

#include <string.h>
#include "pluginInterfaces.hpp"

class AdamsBashforth4ComputePlugin : public ComputePluginInterface {
public:
    const char* PluginName() const {
        return "AdamsBashforth4ComputePlugin";
    };
    int Execute(Result* in);
    ~AdamsBashforth4ComputePlugin() {};
};

#endif