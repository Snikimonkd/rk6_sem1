#include "pluginInterfaces.hpp"
#include "AdamsBashforth2ComputePlugin.hpp"

#include <iostream>

#include <stdlib.h>
#include <stdio.h>

// k1=0.577
const float kef1 = 0.577;
// k2=0.422
const float kef2 = 0.422;

float* function(float* x) {
    float* f = (float*)malloc(3*sizeof(float));

    // x1'=-k1*x1;
    f[0] = -kef1*x[0];
    // x2'=k1*x1-k2*x2;
    f[1] = kef1*x[0] - kef2*x[1];
    // x3'=k2*x2;
    f[2] = kef2*x[1];

    return f;
}

// Метод эйлера для вычесления 2-го значения
float* eulerKickStart(float* x1, float h) {
    float* x2 = (float*)malloc(3*sizeof(float));

    float* f = function(x2);

    x2[0] = x1[0] + h*f[0];
    x2[1] = x1[1] + h*f[1];
    x2[2] = x1[2] + h*f[2];

    free(f);

    return x2;
}

void move(float* x1, float* x2, float* x3, float* x4, float* x5) {
    x1[0] = x2[0];
    x1[1] = x2[1];
    x1[2] = x2[2];

    x2[0] = x3[0];
    x2[1] = x3[1];
    x2[2] = x3[2];

    x3[0] = x4[0];
    x3[1] = x4[1];
    x3[2] = x4[2];

    x4[0] = x5[0];
    x4[1] = x5[1];
    x4[2] = x5[2];
}

void ab4(float* x1, float* x2, float* x3, float* x4, float h) {
    float x5[3];

    float* f1 = function(x1);
    float* f2 = function(x2);
    float* f3 = function(x3);
    float* f4 = function(x4);

    x5[0] = x4[0] + h*(float(55)/float(44)*x4[0] - float(59)/float(24)*x3[0]+float(37)/float(24)*x2[0]-float(3)/float(8)*x1[0]);
    x5[1] = x4[1] + h*(float(55)/float(44)*x4[1] - float(59)/float(24)*x3[1]+float(37)/float(24)*x2[1]-float(3)/float(8)*x1[1]);
    x5[2] = x4[2] + h*(float(55)/float(44)*x4[2] - float(59)/float(24)*x3[2]+float(37)/float(24)*x2[2]-float(3)/float(8)*x1[2]);

    move(x1, x2, x3, x4, x5);

    free(f1);
    free(f2);
    free(f3);
    free(f4);
}

int AdamsBashforth2ComputePlugin::Execute(Result* in) {
    in->methodName = this->PluginName();
    std::cout << "AdamsBashforth4ComputePlugin is executed\n";
    float x1[3];

    x1[0] = 1;
    x1[1] = 0;
    x1[2] = 0;

    float tStart = 0;
    float tFinish = 20;
    float h = 10; 

    float* x2 = eulerKickStart(x1, h);
    float* x3 = eulerKickStart(x2, h);
    float* x4 = eulerKickStart(x3, h);

    for(float t = tStart; t < tFinish; t+=h) {
        values currentValue = {
            .t = tStart,
            .x1 = x1[0],
            .x2 = x1[1],
            .x3 = x1[2],
        };
        
        in->ret.push_back(currentValue);

        ab4(x1, x2, x3, x4, h);
    }

    free(x2);
    free(x3);
    free(x4);

    return 0;
}

extern "C" BasePluginInterface* registerPlugin()
{
    std::cout << "AdamsBashforth4ComputePlugin is loaded\n";
    return new AdamsBashforth2ComputePlugin();
}