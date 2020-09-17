#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "OXjudge.h"

void Output_layer(double a_p[][2][1][1], double w_o[2][2], double bias_o[2][1], int K, int channel, double z_o[][2][1], double a_o[][2][1]) {
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
