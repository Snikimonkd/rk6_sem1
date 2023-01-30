#include <stdlib.h>
#include <stdio.h>
#include "rk4.h"

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

void rk4(float* x, float h) {
    float* k1 = function(x);

    // k2
    float buf[3];
    buf[0] = x[0] + k1[0]/2.0;
    buf[1] = x[1] + k1[1]/2.0;
    buf[2] = x[2] + k1[2]/2.0;
    float* k2 = function(buf);

    // k3
    buf[0] = x[0] + k2[0]/2.0;
    buf[1] = x[1] + k2[1]/2.0;
    buf[2] = x[0] + k2[0]/2.0;
    float* k3 = function(buf);

    // k4
    buf[0] = x[0] + k3[0];
    buf[1] = x[1] + k3[1];
    buf[2] = x[0] + k3[0];
    float* k4 = function(buf);

    x[0] = x[0] + 1.0/6.0*h*(k1[0] + 2.0*k2[0] + 2.0*k3[0] + k4[0]);
    x[1] = x[1] + 1.0/6.0*h*(k1[1] + 2.0*k2[1] + 2.0*k3[1] + k4[1]);
    x[2] = x[2] + 1.0/6.0*h*(k1[2] + 2.0*k2[2] + 2.0*k3[2] + k4[2]);

    free(k1);
    free(k2);
    free(k3);
}

void solve(float* x, float h, float tStart, float tFinish, const char* outputFileName) {
    fprintf(stdout, "Runge–Kutta method is used\n");
    FILE *out = fopen(outputFileName, "w");

    if (out == NULL) {
        fprintf(stderr, "Error! Could not open file\n"); 
        exit(-1);
    }

    size_t it = 1;
    for(float t = tStart; t < tFinish; t+=h) {
        int n = fprintf(out, "%zu,\t%f,\t%f,\t%f\n", it++, x[0], x[1], x[2]);
        if (n < 0) {
            fprintf(stderr, "Error! Could not write to file\n");
            fclose(out);
            exit(-1);
        }

        rk4(x, h);
    }
    
    fclose(out);
}