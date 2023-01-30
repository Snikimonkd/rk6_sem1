#include <stdio.h>

int main() {
    float T[6] = {35.763, 19.031, 13.103, 13.748, 13.910, 14.416};
    float T0[6];
    float S[6];
    float E[6];
    float W[6];

    float p[6] = {1, 2, 4, 8, 12, 16};

    for(size_t i = 0; i < 6; i++) {
        printf("t=%f\t", T[i]);
        T0[i] = (p[i])*T[i] - T[0];
        printf("t0=%f\t", T0[i]);
        S[i] = ((p[i])*T[0])/(T0[i] + T[0]);
        printf("S=%f\t", S[i]);
        E[i] = S[i]/(p[i]);
        printf("E=%f\t", E[i]);
        W[i] = E[i]/(1 - E[i])*((p[i]) * T[i] - T[0]);
        printf("W=%f\n", W[i]);
    }

    return 0;
}