#include <stdio.h>
#include "OXjudge.h"

void Convolutional_Backprop(int In[][1][3][3], double filter[][2][2], int stride, int padding, double dz_p[][2][2][2], double a_c[][2][2][2], int K, int f_channel, int z_k, double da1[][2][3][3], double dw[][2][2], double db[][2][1][1]) {
    int channel, line, low;
    int f_line, f_low;
    int z_channel, z_line, z_low;
    int h, w, c;

    double dz2[6][2][2][2];
    double d[6][2][2][2];
    double D[6][2][4][4];
    int df[2][2][2];
    double a[6][2][2][2];

    int h_start, w_start;
    double sum = 0;
    int i, j, k, l, m, n, o, p;

    channel = sizeof(In[0]) / sizeof(In[0][0]);
    line = sizeof(In[0][0]) / sizeof(In[0][0][0]);
    low = sizeof(In[0][0][0]) / sizeof(In[0][0][0][0]);

    f_line = sizeof(filter[0]) / sizeof(filter[0][0]);
    f_low = sizeof(filter[0][0]) / sizeof(filter[0][0][0]);

    z_channel = sizeof(dz_p[0]) / sizeof(dz_p[0][0]);
    z_line = sizeof(dz_p[0][0]) / sizeof(dz_p[0][0][0]);
    z_low = sizeof(dz_p[0][0][0]) / sizeof(dz_p[0][0][0][0]);

    for(i = 0; i < z_k; i++) {
        for(j = 0; j < z_channel; j++) {
            for(k = 0; k < z_line; k++) {
                for(l = 0; l < z_low; l++) {
                    dz2[i][j][k][l] = dz_p[i][j][k][l] * a_c[i][j][k][l] * (1 - a_c[i][j][k][l]);
                    d[i][j][k][l] = dz2[i][j][k][l];
                    sum += dz2[i][j][k][l];
                }
            }
            db[i][j][0][0] = sum;
            sum = 0;
        }
    }

    h = z_line + (stride - 1) * (z_line - 1);
    w = z_low + (stride - 1) * (z_low - 1);
    c = z_channel;

    //dz2 = reshape(dz2,[f_line,f_low,f_channel,K])はなくても大丈夫（入力が3x3の場合）

    //padarray
    for(i = 0; i < z_k; i++) {
        for(j = 0; j < z_channel; j++) {
            for(k = 0; k < h+2*(f_line-padding-1); k++) {
                for(l = 0; l < w+2*(f_low-padding-1); l++) {
                    if((f_line-padding-1 <= k && k < f_line-padding-1+h) && (f_low-padding-1 <= l && l < f_low-padding-1+w)) {
                        D[i][j][k][l] = d[i][j][k-(f_line-padding-1)][l-(f_low-padding-1)];
                    } else {
                        D[i][j][k][l] = 0;
                    }
                }
            }
        }
    }

    //rot90を2回、つまり180度回転
    for(i = 0; i < f_channel; i++) {
        for(j = 0; j < f_line; j++) {
            for(k = 0; k < f_low; k++) {
                df[i][j][k] = filter[i][f_line-1-j][f_low-1-k];
            }
        }
    }


    for(k = 0; k < f_channel; k++) {
        for(j = 0; j < low; j++) {
            for(i = 0; i < line; i++) {
                for(l = 0; l < K; l++) {
                    for(m = 0; m < c; m++) {
                        for(n = i; n <= i+f_low-1; n++) {
                            for(o = j; o <= j+f_line-1; o++) {
                                a[l][m][n-i][o-j] = D[l][m][n][o] * df[k][n-i][o-j];
                                sum += a[l][m][n-i][o-j];
                            }
                        }
                    }
                    da1[l][k][i][j] = sum;
                    sum = 0;
                }
            }
        }
    }

    //In = padarray(In,[padding padding],0,'both');はpaddingが0のためやる必要なし

    for(k = 0; k < f_channel; k++) {
        for(j = 0; j < f_low; j++) {
            for(i = 0; i < f_line; i++) {
                h_start = (i - 1) * stride + 1;
                w_start = (j - 1) * stride + 1;
                for(l = 0; l < K; l++) {
                    for(m = h_start; m <= h_start+z_line-1; m++) {
                        for(n = w_start; n <= w_start+z_low-1; n++) {
                            sum += dz2[l][k][m-h_start][n-w_start] * In[l][0][m][n];
                        }
                    }
                }
                dw[k][i][j] = sum;
                sum = 0;
            }
        }
    }
}