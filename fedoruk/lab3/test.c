#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const float delta = 0.5;

int main() {
    FILE* f = fopen("in.txt", "w");
    if (f == NULL) {
        exit(-1);
    }

    for(float x = -7; x < 15; x += delta) {
        for(float y = -1; y < 15; y += delta) {
            if ((y - 6.) < sqrt(((x + 3.)*(x + 3.) / (25./4.) - 1.)*(-100./4.)) && (y - 6.) > -sqrt(((x + 3.)*(x + 3.) / (25./4.) - 1.)*(-100./4.))) {
                fprintf(f, "%f, %f, %f\n", x, y, 0.);
                continue;
            }

            if ((y - 3.) < sqrt(((x - 6.)*(x - 6.) / (100./4.) - 1.)*(-25./4.)) && (y - 3.) > -sqrt(((x - 6.)*(x - 6.) / (100./4.) - 1.)*(-25./4.))) {
                fprintf(f, "%f, %f, %f\n", x, y, 1.);
            }
        }
    }

    fclose(f);
}