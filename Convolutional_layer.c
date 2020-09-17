#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "OXjudge.h"

void Convolutional_layer(int In[][1][3][3], double filter[][2][2], int stride, double bias[][1][1], int padding, int filter_Line, int filter_Low, int Out_Line, int Out_Low, int Out_Channel, int size4, double a[][2][2][2], double z[][2][2][2]) {
    int LINE, LOW, OUT;

    int i, j, k, l, I, J;
    int channel, low, line;
    int cnt = 0;
    double sum = 0;
    double tmp;

    //zeros
    for(i = 0; i < size4; i++) {
        for(j = 0; j < Out_Channel; j++) {
            for(k = 0; k < Out_Low; k++) {
                for(l = 0; l < Out_Line; l++) {
                    z[i][j][k][l] = 0;
                }
            }
        }
    }

    for(k = 0; k < size4; k++) {
        for(channel = 0; channel < Out_Channel; channel++) {
            for(low = 0; low < Out_Low; low++) {
                for(line = 0; line < Out_Line; line++) {
                    LINE = (line-1) * stride + 1; 
                    LOW = (low-1) * stride + 1;
                    for(i = LINE; i <= LINE+filter_Line-1; i++) {
                        for(j = LOW; j <= LOW+filter_Low-1; j++) {
                            sum += In[k][0][i][j] * filter[channel][i-LINE][j-LOW];
                        }
                    }
                    z[k][channel][line][low] = sum;
                    sum = 0;
                }
            }
        }
    }

    for(i = 0; i < size4; i++) {
        for(j = 0; j < Out_Channel; j++) {
            for(k = 0; k < Out_Low; k++) {
                for(l = 0; l < Out_Line; l++) {
                    z[i][j][k][l] = z[i][j][k][l] + bias[cnt%2][0][0];   
                }
            }
            cnt++;
        }
    }

    for(i = 0; i < size4; i++) {
        for(j = 0; j < Out_Channel; j++) {
            for(k = 0; k < Out_Low; k++) {
                for(l = 0; l < Out_Line; l++) {
                    tmp = z[i][j][k][l];
                    a[i][j][k][l] = 1 / (1 + exp(-tmp));
                }
            }
        }
    }
}
