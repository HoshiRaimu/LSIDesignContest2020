#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "OXjudge.h"

void Output_Backprop(double a_o[][2][1], int t[][6], double a_p[][2][1][1], double w_o[][2], int K, int channel, double dCda[][2][1], double dCdw[][2][2], double dCdb[][2][1], double *Ct) {
    double C[6][1];
    double tmp[2];
    double sum = 0;
    double dCdo[6][2][1];
    double dCdz[6][2][1];

    int i, j, k, l=0, s;

    for(i = 0; i < K; i++) {
        for(j = 0; j < channel; j++) {
            for(k = 0; k < 1; k++) {
                tmp[l++] = (t[j][i] - a_o[i][j][k]) * (t[j][i] - a_o[i][j][k]);
            }
        }
        C[i][0] = (tmp[0] + tmp[1]) / 2;
        sum += C[i][0];
        l = 0;
    }

    *Ct = sum;

    for(i = 0; i < K; i++) {
        for(j = 0; j < channel; j++) {
            for(k = 0; k < 1; k++) {
                dCdo[i][j][k] = a_o[i][j][k] - t[j][i];
                dCdz[i][j][k] = dCdo[i][j][k] * a_o[i][j][k] * (1 - a_o[i][j][k]);
                dCdb[i][j][k] = dCdz[i][j][k];
            }
        }
    }

    for(s = 0; s < K; s++) {
        for(i = 0; i < channel; i++) {
            dCdw[s][i][0] = dCdz[s][0][0] * a_p[s][i][0][0];
            dCdw[s][i][1] = dCdz[s][1][0] * a_p[s][i][0][0];
        }
    }

    for(s = 0; s < K; s++) {
        for(i = 0; i < channel; i++) {
            dCda[s][i][0] = (dCdz[s][0][0] * w_o[i][0] + dCdz[s][1][0] * w_o[i][1]) *a_p[s][i][0][0] * (1 - a_p[s][i][0][0]);
        }
    }
}
