#include <stdio.h>
#include <stdlib.h>
#include "OXjudge.h"

void Pooling_layer(double In[][2][2][2], int stride, int Out_Line, int Out_Low, int Out_Channel, int K, double a[][2][1][1]){
    int line, low, channel, LINE, LOW;
    double max = 0;

    int i, j, k, l;

    //zeros
    for(i = 0; i < K; i++) {
        for(j = 0; j < Out_Channel; j++) {
            for(k = 0; k < Out_Low; k++) {
                for(l = 0; l < Out_Line; l++) {
                    a[i][j][k][l] = 0;
                }
            }
        }
    }

    for(line = 0; line < Out_Line; line++) {
        for(low = 0; low < Out_Low; low++) {
            for(channel = 0; channel < Out_Channel; channel++) {
                LINE = (line - 1) * stride + 2;     //本当は+1
                LOW = (low - 1) * stride + 2;       //本当は+1
                for(i = 0; i < K; i++) {
                    for(j = LOW; j <= LOW+stride-1; j++) {
                        for(k = LINE; k <= LINE+stride-1; k++) {
                            if(max < In[i][channel][j][k]) max = In[i][channel][j][k];
                        }
                    }
                    a[i][channel][low][line] = max;
                    max = 0;
                }
            }
        }
    }
}