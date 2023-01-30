#include <stdlib.h>
#include <stdio.h>
#include "ab2.h"

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

    // float* x2 = eulerKickStart(x1, h);
    // free(x2);

void move(float* left, float* right) {
    left[0] = right[0];
    left[1] = right[1];
    left[2] = right[2];
}

void ab2(float* x1, float* x2, float h) {
    float x3[3];

    float* f1 = function(x1);
    float* f2 = function(x2);

    x3[0] = x2[0] + 1.5*h*f2[0] - 0.5*h*f1[0];
    x3[1] = x2[1] + 1.5*h*f2[1] - 0.5*h*f1[1];
    x3[2] = x2[2] + 1.5*h*f2[2] - 0.5*h*f1[2];

    move(x1, x2);
    move(x2, x3);

    free(f1);
    free(f2);
}

void solve(float* x1, float h, float tStart, float tFinish, const char* outputFileName) {
    fprintf(stdout, "Adams–Bashforth method is used\n");
    FILE *out = fopen(outputFileName, "w");

    if (out == NULL) {
        fprintf(stderr, "Error! Could not open file\n");
        exit(-1);
    }

    float* x2 = eulerKickStart(x1, h);

    size_t it = 1;
    for(float t = tStart; t < tFinish; t+=h) {
        int n = fprintf(out, "%zu,\t%f,\t%f,\t%f\n", it++, x1[0], x1[1], x1[2]);
        if (n < 1) {
            fprintf(stderr, "Error! Could not write to file\n");
            fclose(out);
            free(x2);
            exit(-1);
        }

        ab2(x1, x2, h);
    }

    free(x2);
    fclose(out);
}