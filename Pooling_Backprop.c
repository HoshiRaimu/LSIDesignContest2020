#include <stdio.h>
#include "OXjudge.h"

void Pooling_Backprop(double a_c[][2][2][2], int stride, double a_p[][2][1][1], double dz_o[][2][1], int K, int channel, double dz_p[][2][2][2]) {
    int i, j, k, l;
    double da2[6][2][2][2];
    double A_P[6][2][2][2];

    //reshape
    for(i = 0; i < K; i++) {
        for(j = 0; j < channel; j++) {
            da2[i][j][0][0] = dz_o[i][j][0];
        }
    }

    //repelem
    for(i = 0; i < K; i++) {
        for(j = 0; j < channel; j++) {
            da2[i][j][0][1] = da2[i][j][0][0];
            da2[i][j][1][0] = da2[i][j][0][0];
            da2[i][j][1][1] = da2[i][j][0][0];

            A_P[i][j][0][0] = a_p[i][j][0][0];
            A_P[i][j][0][1] = a_p[i][j][0][0];
            A_P[i][j][1][0] = a_p[i][j][0][0];
            A_P[i][j][1][1] = a_p[i][j][0][0];
        }
    }

    for(i = 0; i < K; i++) {
        for(j = 0; j < channel; j++) {
            if(a_c[i][j][0][0] == A_P[i][j][0][0]) dz_p[i][j][0][0] = da2[i][j][0][0];
            else dz_p[i][j][0][0] = 0;

            if(a_c[i][j][0][1] == A_P[i][j][0][1]) dz_p[i][j][0][1] = da2[i][j][0][1];
            else dz_p[i][j][0][1] = 0;

            if(a_c[i][j][1][0] == A_P[i][j][1][0]) dz_p[i][j][1][0] = da2[i][j][1][0];
            else dz_p[i][j][1][0] = 0;

            if(a_c[i][j][1][1] == A_P[i][j][1][1]) dz_p[i][j][1][1] = da2[i][j][1][1];
            else dz_p[i][j][1][1] = 0;
        }
    }
}
