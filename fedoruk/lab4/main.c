#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const size_t num = 295;

const float x0 = 1;
const float b = 1;
const float nu = 0.5;
const float eps = 0.08;

float _wheightedSum(float x0, float x1, float x2, float w0, float w1, float w2) {
    return x0*w0 + x1*w1 + x2*w2;
}

void wheightedSum(float x0, float* x1, float* x2, float w0, float w1, float w2, float* u) {
    for(size_t k = 0; k < num; k++) {
        u[k] = _wheightedSum(x0, x1[k], x2[k], w0, w1, w2);
    }
}

float _unipolarSigmoid(float u) {
    float y = 1 / (1 + exp(-b * u));
    return y;
}

void unipolarSigmoid(float* u, float* y) {
    for(size_t k = 0; k < num; k++) {
        y[k] = _unipolarSigmoid(u[k]);
    }
}

float targetFunction(size_t p, float* y, float* d) {
    float E = 0;
    for(size_t k = 0; k < p; k++) {
        E += 1/2*pow((y[k]-d[k]), 2);
    }

    return E;
}

float weightAdjustment(float w, float x, float y, float d) {
    float w1 = w - x*(y - d);
    return w1;
}

float mod(float x) {
    if (x > 0) {
        return x;
    }

    return -x;
}

float terminationCondition(float x, float y, float d) {
    float r = mod(x*(y - d));
    return r;
}

float getRand(float delta) {
    return (float) rand() / RAND_MAX * (2*delta) - delta;
}

void readFile(float* x1, float* x2, float* y) {
    FILE* f = fopen("in.txt", "r");
    if (f == NULL) {
        exit(-1);
    }

    for(size_t i = 0; i < num; i++) {
        fscanf(f,"%f, %f, %f\n",&x1[i], &x2[i], &y[i]);
    }

    fclose(f);
}

int main() {
    FILE* f = fopen("data.plot", "w");
    if (f == NULL) {
        exit(-1);
    }

    float* x1 = (float*)malloc(num * sizeof(float)); 
    float* x2 = (float*)malloc(num * sizeof(float));
    float* d = (float*)malloc(num * sizeof(float));
    float* u = (float*)malloc(num * sizeof(float));
    float* y = (float*)malloc(num * sizeof(float));

    readFile(x1, x2, d);

    float w0 = getRand(sqrt(3)/2);
    float w1 = getRand(2/sqrt(3));
    float w2 = getRand(2/sqrt(3));

    printf("%f\n%f\n%f\n", w0, w1, w2);

    for(size_t i = 0; i < num; i++) {
        fscanf(f,"%f",&x1[i]);
        fscanf(f,"%f",&x2[i]);
        fscanf(f,"%f",&y[i]);
    }

    fprintf(f, "set terminal gif animate\nset output 'out.gif'\nset xrange [-7:15]\nset yrange [-1:15]\nset xzeroaxis lt -1\nset yzeroaxis lt -1\nset grid\nset object 1 ellipse center -3, 6 size 5, 10 fs empty bo 3\nset object 2 ellipse center 6, 3 size 10, 5 fs empty bo 3\n");
    int t = 0;
    while(t >= 0) {
        t++;
        wheightedSum(x0, x1, x2, w0, w1, w2, u);
        unipolarSigmoid(u, y);

        fprintf(f, "f(x)=((%f)+(%f)*x)/(%f)\n", w0, w1, -1*w2);
        fprintf(f, "plot '-' with points palette title 't=%d', f(x) with lines\n", t);
        for(size_t k = 0; k < num; k++) {
            fprintf(f, "%f %f %f\n", x1[k], x2[k], y[k]);
        }
        fprintf(f, "e\npause 0.0001\n");

        int ok = -1;
        for(size_t k = 0; k < num; k++) {
            // на наборе входных данных k все ок
            if (terminationCondition(x0, y[k], d[k]) < eps) {
                if (terminationCondition(x1[k], y[k], d[k]) < eps) {
                    if (terminationCondition(x2[k], y[k], d[k]) < eps) {
                        continue;
                    }
                }
            }

            ok = k;
            break;
        }

        if (ok != -1) {
            // если не ок то пересчитываем w и начинаем заново
            w0 = weightAdjustment(w0, x0, y[ok], d[ok]);
            w1 = weightAdjustment(w1, x1[ok], y[ok], d[ok]);
            w2 = weightAdjustment(w2, x2[ok], y[ok], d[ok]);
        } else {
            break;
        }
    }

    fclose(f);

    free(x1);
    free(x2);
    free(d);
    free(u);
    free(y);

    printf("%d\n", t);
    printf("%f\n%f\n%f\n", w0, w1, w2);

    return 0;
}