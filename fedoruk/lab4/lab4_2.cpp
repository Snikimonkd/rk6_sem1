#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int N=3;  //количество входных сигналов + 1
const int L=4;  // количество нейронов + 1
const int M=2;  // количество выходов + 1
const int K=1000; // количество точек
//Веса
double W1[L][N];
double W2[M][L];
//Аргументы функции активации
double U1[L][K];
double U2[M][K];
//Входы-выходы
double X[N][K];
double D[M][K];
double G[L][K];
double Y[M][K];

double nu=0.001;
double b=1;
int cycle=2000;
double func(double x,double y) {
	return sin(x) + sin(y);
}

double get_rand_num(double min, double max) {
  double val;
  val = rand() % (int)pow(10, 3);
  val = min + (val / pow(10, 3)) * (max - min);
  return val;
}

void init() {
    // int k;
    // double x,y;
    // for(k=0;k<K;k++) {
    //     x=get_rand_num(-1,1);
	// 	y=get_rand_num(-1,1);
    //     X[1][k]=x;
	// 	X[2][k]=y;
    //     D[1][k]=func(x,y);
    // }
    float sK = sqrt(K);
    int k = 0;
    for(float x1 = -3; x1 < 3; x1+=(6./sK)) {
        for(float x2 = -3; x2 < 3; x2+=(6./sK)) {
            X[1][k] = x1;
            X[2][k] = x2;
            D[1][k] = func(x1, x2);
            k++;
        }
    }
}

//функция активации
double f(double u) {
    return 1./(1.+exp(-b*u));
}

double dfdu(double u) {
    return b*f(u)*(1. - f(u));
}

void find_W() {
    int i,j,k,l,t;
    double grad1[L][N];
	double grad2[M][L];
    double sum_M[M];
    double E;
    double s;
    for (k=0; k<K; k++) {
        for(l=0; l<L; l++) {
            for(i=1; i<M; i++) {
                grad2[i][l]=(Y[i][k]-D[i][k])*dfdu(U2[i][k])*G[l][k];  
			}
        }
        for(l=0; l<L; l++) {
            sum_M[l]=0;
        }

        for(l=0; l<L; l++) {
            for(i=1;i<M;i++) {
			    sum_M[l]=sum_M[l]+(Y[i][k]-D[i][k])*dfdu(U2[i][k])*W2[i][l]*dfdu(U1[l][k]);
            }

            for(j=0; j<N; j++) {
                grad1[l][j]=sum_M[l]*X[j][k];
            }
        }

		for(l=1; l<L; l++) {
			for(j=0; j<N; j++) {
				W1[l][j]=W1[l][j]-nu*grad1[l][j];
            }
        }

		for(i=1; i<M; i++) {
			for(l=0; l<L; l++) {
				W2[i][l]=W2[i][l]-nu*grad2[i][l];
            }
        }
	}
}

void gnuplot() {
    int k;
    FILE* original = fopen("original", "w");
    FILE* aprox = fopen("aprox", "w");
    for(k=0; k<K; k++) {
        fprintf(original, "%f %f %f\n", X[1][k], X[2][k], D[1][k]);
        fprintf(aprox, "%f %f %f\n", X[1][k], X[2][k], Y[1][k]);
    }

    fclose(original);
    fclose(aprox);

    int sK = int(sqrt(k));
    FILE* plot = fopen("plot", "w");
    fprintf(plot, "set dgrid3d\n");
    fprintf(plot, "set dgrid3d %d, %d\n", sK, sK);
	fprintf(plot, "splot \"original\" u 1:2:3 with lines, \"aprox\" u 1:2:3 with lines\n");

    fclose(plot);
}

int main() {
    int i,j,k,l,t;
    //инициализация весов
    for(l=0; l<L; l++) {
        for(j=0; j<N; j++) {
            W1[l][j]=get_rand_num(-sqrt(L-1)/2,sqrt(L-1)/2);
        }
    }

    for(i=0; i<M; i++) {
        for(l=0; l<L; l++) {
            W2[i][l]=0;
        }
    }

    //инициализация входных сигналов
    for(k=0;k<K;k++) {
        X[0][k]=1;
        G[0][k]=1;
    }

    init();

    for(t=0;t<cycle;t++) {
        for (k=0; k<K; k++) {
            for(l=1;l<L;l++) {
                double sum_N=0;
                for(j=0;j<N;j++) {
                    sum_N=sum_N+W1[l][j]*X[j][k];
                }

                U1[l][k]=sum_N;
                G[l][k]=f(sum_N);
            }
            for(i=1;i<M;i++) {
                double sum_L=0;
                for(l=0;l<L;l++) {
                    sum_L=sum_L+W2[i][l]*G[l][k];
                }
                U2[i][k]=sum_L;
				Y[i][k]=sum_L;
            }
        }

        //пересчитываем веса
        find_W();
    }

    for(k=0; k<K; k++) {
        printf("x=%f, y=%f, D=%f, Y=%f\n", X[1][k], X[2][k], D[1][k], Y[1][k]);
    }

    gnuplot();
}
