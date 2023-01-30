#include <stdio.h>
#include <string.h>
#include "dlfcn.h"

const char* ab2Method = "ab2";
const char* ab2LibLocation = "ab2/libab2.dylib";

const char* rk4Method = "rk4";
const char* rk4LibLocation = "rk4/librk4.dylib";

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "You need to pass two arguments to function\n");
        return -1;
    }

    const char* method = argv[1];
    const char* libLocation;
    if (strcmp(method, ab2Method) == 0) {
        libLocation = ab2LibLocation;
    }
    if (strcmp(method, rk4Method) == 0) {
        libLocation = rk4LibLocation;
    }
    if (strcmp(libLocation, "") == 0) {
        fprintf(stderr, "You need to chose one of two methods: ab2 or rk4\n");
        return -1;
    }

    const char* fileName = argv[2];

    void* lib;
    void (*solve)(float* x, float h, float tStart, float tFinish, const char* outputFileName);

    lib = dlopen(libLocation, RTLD_LAZY);
    if (!lib) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return -1;
    }

    *(void**)(&solve) = dlsym(lib, "solve");
    if (!solve) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(lib);
        return -1;
    }

    float x[3];
    // x1(0)=1;
    x[0] = 1;
    // x2(0)=0;
    x[1] = 0;
    // x3(0)=0;
    x[2] = 0;

    float tStart = 0;
    float tFinish = 20;
    float h = 0.001;

    solve(x, h, tStart, tFinish, fileName);
    dlclose(lib);

    return 0;
}