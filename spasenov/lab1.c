#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "/usr/local/opt/libomp/include/omp.h"

float dx = 0.0008;
float dy = 0.0008;
float a1 = 1;
float dt;
float t = 10.;

#define NUM_THREADS 8

float *createMatrix(int m, int n) {
    return (float *) malloc(m * n * sizeof(float));
}

void initMatrix(float *matrix, int m, int n) {
    int cornerBorderHor = ceil(m / 2.);
    int cornerBorderVer = ceil(n * 5. / 8.);

    // левая граница
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < m; i++) {
        matrix[i * n] = 200.;
    }

    // Нижняя граница
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < n; i++) {
        matrix[(m - 1) * n + i] = 50.0;
    }

    // Верхняя граница
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < cornerBorderVer; i++) {
        matrix[i] = 100.0;
    }

    // Правая граница
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = cornerBorderHor; i < m; i++) {
        matrix[i * n + n - 1] = 100.0;
    }

    // Скругленный угол
    int cornerRadius = ceil(m / 2.);
    int cornerCenterX = ceil(n * 5. / 8.);
    int cornerCenterY = ceil(m / 2.);
    #pragma omp parallel for collapse(2) num_threads(NUM_THREADS)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            float xCornerDist = cornerCenterX - j;
            float yCornerDist = cornerCenterY - i;
            if (j >= cornerCenterX && i <= cornerCenterY) {
                if (ceil(sqrt(xCornerDist * xCornerDist + yCornerDist * yCornerDist)) == cornerRadius - 1) {
                    matrix[i * n + j] = 100.0;
                }
            }
        }
    }
}

// Получение состояния системы в следующий момент времени
float *mkr(float *matrix, int m, int n) {
    float cornerCenterX = n * 5. / 8.;
    float cornerCenterY = m / 2.;
    int cornerRadius = ceil(m / 2.);
    int cornerRadius_sqr = cornerRadius * cornerRadius;

    float circleCenterX = n / 4.;
    float circleCenterY = m / 3.;
    int circleRadius = ceil(m / 6.);
    int circleRadius_sqr = circleRadius * circleRadius;

    float *nextStep = createMatrix(m, n);
    initMatrix(nextStep, m, n);

    #pragma omp parallel for collapse(2) num_threads(NUM_THREADS)
    for (int i = 1; i < m - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
            // Скругленный угол
            if (j > cornerCenterX && i < cornerCenterY) {
                float xCornerDist = cornerCenterX - j;
                float yCornerDist = cornerCenterY - i;
                int xCornerDistCeil = ceil(cornerCenterX) - j;
                int yCornerDistCeil = ceil(cornerCenterY) - i;
                int dist = ceil(sqrt(xCornerDistCeil * xCornerDistCeil + yCornerDistCeil * yCornerDistCeil));

                nextStep[i * n + j] = matrix[i * n + j];

                if (dist < cornerRadius - 2) {
                    nextStep[i * n + j] += dt * a1 * (matrix[i * n + j + 1] - 2 * matrix[i * n + j] +
                                                                   matrix[i * n + j - 1]) / (dx * dx);
                    nextStep[i * n + j] += dt * a1 * (matrix[(i + 1) * n + j] - 2 * matrix[i * n + j] +
                                                                   matrix[(i - 1) * n + j]) / (dy * dy);
                    continue;
                // Околол границы неправильной формы
                } else if (dist == cornerRadius - 2) {
                    if ((xCornerDistCeil + 1)*(xCornerDistCeil + 1) + yCornerDistCeil*yCornerDistCeil <= cornerRadius_sqr) {
                        float mu = fmin(1, fabs(sqrt(cornerRadius_sqr - cornerCenterY*cornerCenterY) - xCornerDist));
                        nextStep[i * n + j] += 2 * dt * a1 * (matrix[i * n + j + 1] - (mu + 1) * matrix[i * n + j] + mu * matrix[i * n + j - 1])
                                / (mu * (mu + 1) * dx * dx);
                    } else {
                    	nextStep[i * n + j] += dt * a1 * (matrix[i * n + j + 1] - 2 * matrix[i * n + j] +
                                                                   matrix[i * n + j - 1]) / (dx * dx);
                    }

                    if (xCornerDistCeil*xCornerDistCeil + (yCornerDistCeil + 1)*(yCornerDistCeil + 1) <= cornerRadius_sqr) {
                    	float lambda = fmin(1, fabs(sqrt(cornerRadius_sqr - cornerCenterX*cornerCenterX) - yCornerDist));
                      	nextStep[i * n + j] += 2 * dt * a1 * (matrix[(i + 1) * n + j] - (lambda + 1) * matrix[i * n + j] + lambda * matrix[(i - 1) *n + j])
                       	/ (lambda * (lambda + 1) * dy * dy);
                    } else {
                    	nextStep[i * n + j] += dt * a1 * (matrix[(i + 1) * n + j] - 2 * matrix[i * n + j] +
                                                                   matrix[(i - 1) * n + j]) / (dy * dy);
                    }
                }
            } else { // Отверстие
                        nextStep[i * n + j] = matrix[i * n + j];
                        nextStep[i * n + j] += dt * a1 * (matrix[i * n + j + 1] - 2 * matrix[i * n + j] +
                                                                   matrix[i * n + j - 1]) / (dx * dx);
                        nextStep[i * n + j] += dt * a1 * (matrix[(i + 1) * n + j] - 2 * matrix[i * n + j] +
                                                                   matrix[(i - 1) * n + j]) / (dy * dy);
            }
        }
    }

    return nextStep;
}



void printMatrix(float *matrix, int m, int n) {
    FILE* f;
    char filename[64];
    sprintf(filename, "result.txt");
    f = fopen(filename,"w");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(f, "%6.1f", matrix[i * n + j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

void printStepIntoFile(float *matrix, int m, int n, int t) {
    FILE* f;
    char filename[64];
    sprintf(filename, "data/time%d.txt", (int)(t));
    f = fopen(filename,"w");

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
                fprintf(f,"%lf %lf %lf\n", j * 8.0/(n - 1), 6. - i * 6./(m - 1), matrix[i * n + j]);
        }
    }

    fclose(f);
}

void findMinMax(float* matrix, int m, int n, float *min, float *max) {
    float minTemp = 1000.0;
    float maxTemp = -1000.0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[n * i + j] >= maxTemp) {
                maxTemp = matrix[n * i + j];
            }
            if (matrix[n * i + j] <= minTemp && matrix[n * i + j] > 10.0) {
                minTemp = matrix[n * i + j];
            }
        }
    }

    *max = maxTemp;
    *min = minTemp;
}

void generatePlot(int iterations) {
    FILE* f;
    f = fopen("plot.txt","w");

    fprintf(f,"set terminal png size 1100,750\n");
    fprintf(f,"set output 'temperature_field.png'\n");
    fprintf(f,"set palette defined ( 0 \"blue\", 1 \"cyan\", 2 \"green\", 3 \"yellow\", 4 \"red\" )\n");
    fprintf(f,"list=system('ls data | sort -V')\n");
    fprintf(f,"set xrange[0:8]\n");
    fprintf(f,"set yrange[0:6]\n");
    fprintf(f,"plot 'data/time%d.txt' u 1:2:3 w image notitle\n", iterations);

    fclose(f);  
}

void generatePlotGif(int iterations) {
    FILE* f;
    f = fopen("plot_gif.txt","w");

    fprintf(f,"set terminal gif animate delay 15 size 1100,750\n");
    fprintf(f,"set output 'temperature_field.gif'\n");
    fprintf(f,"set palette defined ( 0 \"blue\", 1 \"cyan\", 2 \"green\", 3 \"yellow\", 4 \"red\" )\n");
    fprintf(f,"list=system('ls data | sort -V')\n");
    fprintf(f,"set xrange[0:8]\n");
    fprintf(f,"set yrange[0:6]\n");
    fprintf(f,"do for [i=0:%d:10] {\n", iterations);
    fprintf(f,"\tplot sprintf('data/time%%d.txt',i) u 1:2:3 w image notitle\n}\n");

    fclose(f); 
}

int main() {
    int M = 6 / dx + 1;
    int N = 8 / dy + 1;
    dt = dx*dy/4./a1;

    float *currentState = createMatrix(M, N);

    initMatrix(currentState, M, N);
    int iterations = t / dt;

    for (int i = 0; i < 500; i++) {
        // printStepIntoFile(currentState, M, N, i);
        float *newState = mkr(currentState, M, N);
        free(currentState);
        currentState = newState;
    }
    // printStepIntoFile(currentState, M, N, iterations);
    // printMatrix(currentState, M, N);

    for(size_t i =0; i < 10; i++) {
        printf("%f\n", currentState[i]);
    }

    float min = 1000, max = -1000;
    // findMinMax(currentState, M, N, &min, &max);
    // printf("Min = %f    Max = %f\n", min, max);

    // generatePlot(iterations);
    // generatePlotGif(iterations);

    return 0;
}
