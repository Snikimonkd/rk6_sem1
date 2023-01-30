#ifndef BASE_PLUGIN_HPP
#define BASE_PLUGIN_HPP

#include  <vector>

struct values {
    float t;
    float x1;
    float x2;
    float x3;
};

struct Result {
    const char* methodName;
    std::vector<values> ret;
};

class BasePluginInterface {
public:
    virtual const char* PluginType() const = 0;
    virtual const char* PluginName() const = 0;
    virtual int Execute(Result* in) = 0;
    virtual ~BasePluginInterface() {};
};

class ComputePluginInterface : public BasePluginInterface {
public:
    const char* PluginType() const {
        return "ComputePlugin";
    };
    virtual const char* PluginName() const = 0;
    virtual int Execute(Result* in) = 0;
};

class VisualizationPluginInterface : public BasePluginInterface {
public:
    const char* PluginType() const {
        return "VisualizationPlugin";
    };
    virtual const char* PluginName() const = 0;
    virtual int Execute(Result* in) = 0;
};

#endif