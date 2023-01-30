#include "dlfcn.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "main.hpp"

const char* dllEnding = ".dylib";

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

int main(int argc, char *argv[]) {
    std::vector<std::string> dlls;
    for (const auto& dirEntry : recursive_directory_iterator("src/")) {
        if (strstr(dirEntry.path().c_str(), dllEnding) != NULL) {
            dlls.push_back(dirEntry.path().c_str());
        }
    }

    std::vector<BasePluginInterface*> computePlugins;
    std::vector<BasePluginInterface*> visualizationPlugins;
    std::vector<void*> libs;

    for (auto &v : dlls) {
        void* lib = dlopen(v.c_str(), RTLD_LAZY);
        if (!lib) {
            fprintf(stderr, "Error: %s\n", dlerror());
            return -1;
        }

        libs.push_back(lib);

        typeRegisterPlugin* regPLG = reinterpret_cast<typeRegisterPlugin*>(dlsym(lib, "registerPlugin"));
        if (!regPLG) {
            fprintf(stderr, "Error: %s\n", dlerror());
            dlclose(lib);
            return -1;
        }

        BasePluginInterface* buf = regPLG();
        if (strcmp(buf->PluginType(), "ComputePlugin") == 0) {
            computePlugins.push_back(buf);
        }
        if (strcmp(buf->PluginType(), "VisualizationPlugin") == 0) {
            visualizationPlugins.push_back(buf);
        }
    }

    for (auto &compute : computePlugins) {
        Result in;
        int err = compute->Execute(&in);
        if (err != 0) {
            return -1;
        }

        for (auto &visualization : visualizationPlugins) {
            int err = visualization->Execute(&in);
            if (err != 0) {
                return -1;
            }
        }
    }

    for (auto &v : libs) {
        dlclose(v);
    }

    return 0;
}