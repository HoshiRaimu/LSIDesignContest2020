#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Output_layer(double (*)[2][1][1], double (*)[2], double (*)[1], int, int, int, int, double ***, double ***);

int main() {
    double a_p[6][2][1][1] ={0.9820, 0.8808, 0.9820, 0.8808, 0.9820, 0.8808, 0.9526, 0.9526, 0.9526, 0.9526, 0.9526, 0.9526};
    double w_o[2][2] = {{1.5, -2.6}, {-4.2, 3.5}};
    double bias_o[2][1] = {{1}, {1}};
    double ***z_o;
    double ***a_o;
    int line, low, channel, K;

    int i, j, k;

    K = 6;
    channel = 2;
    low = 1;
    line = 1;

    z_o = (double***)malloc(sizeof(double**) * K);       //z_o,a_oの動的確保
    a_o = (double***)malloc(sizeof(double**) * K);           
    for(i = 0; i < K; i++) {
        z_o[i] = (double**)malloc(sizeof(double*) * channel);
        a_o[i] = (double**)malloc(sizeof(double*) * channel);
        for(j = 0; j < channel; j++) {
            z_o[i][j] = (double*)malloc(sizeof(double) * 1);
            a_o[i][j] = (double*)malloc(sizeof(double) * 1);
        }
    }

    Output_layer(a_p, w_o, bias_o, K, channel, low, line, z_o, a_o);

    for(i = 0; i < K; i++) {
        for(j = 0; j < channel; j++) {
            for(k = 0; k < 1; k++) {
                printf("%f  ", a_o[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");

    for(i = 0; i < K; i++) {               //z_o,a_oの解放
        for(j = 0; j < channel; j++) {
            free(z_o[i][j]);
            free(a_o[i][j]);
        }
        free(z_o[i]);
        free(a_o[i]);
    }
    free(z_o);
    free(a_o);

    return 0;
}

void Output_layer(double a_p[6][2][1][1], double w_o[2][2], double bias_o[2][1], int K, int channel, int low, int line, double ***z_o, double ***a_o) {
    int i, j, k, s;
    double tmp1, tmp2;

    //zeros
    for(i = 0; i < K; i++) {
        for(j = 0; j < channel; j++) {
            for(k = 0; k < 1; k++) {
                z_o[i][j][k] = 0;
            }
        }
    }

    for(s = 0; s < K; s++) {
        z_o[s][0][0] = a_p[s][0][0][0] * w_o[0][0] + a_p[s][1][0][0] * w_o[1][0] + bias_o[0][0];
        z_o[s][1][0] = a_p[s][0][0][0] * w_o[0][1] + a_p[s][1][0][0] * w_o[1][1] + bias_o[1][0];

        tmp1 = z_o[s][0][0];
        tmp2 = z_o[s][1][0];
        a_o[s][0][0] = 1 / (1 + exp(-tmp1));
        a_o[s][1][0] = 1 / (1 + exp(-tmp2));
            
    }
}