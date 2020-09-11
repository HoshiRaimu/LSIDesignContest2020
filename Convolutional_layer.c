#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "OXjudge.h"

void Convolutional_layer(int In[][1][3][3], int filter[][2][2], int stride, double bias[][1][1], int padding, int filter_Line, int filter_Low, int Out_Line, int Out_Low, int Out_Channel, int size4, double ****a, double ****z) {

    //int bias[2][3][3] = {{{1, 1, 1}}, {{1, 1, 1}}};      //配列の大きさはまだ決定ではない
    int LINE, LOW, OUT;

    int i, j, k, l, I, J;
    int channel, low, line;
    int cnt = 0;
    int sum = 0;
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
                    for(i = LOW; i <= LOW+filter_Low-1; i++) {
                        for(j = LINE; j <= LINE+filter_Line-1; j++) {
                            I = i - LOW; 
                            J = j - LINE;
                            sum += In[k][0][i][j] * filter[channel][I][J];
                        }
                    }
                    z[k][channel][low][line] = sum;
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

/*int main() {
    int filter_c[2][2][2] = {{{1, 1}, {1, -1}}, {{1, -1}, {-1, 1}}};
    int In[6][1][3][3] = {{{{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 0}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 1}, {1, 1, 0}}}, 
                          {{{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 0}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 1}, {0, 1, 0}, {1, 0, 0}}}};
    int stride_c = 1;
    int bias_c[2][1][1] = {{{1}}, {{1}}};
    int padding = 0;
    int ****z_c;
    double ****a_c;

    int size1, size2, size3;
    int i, j, k, l;

    int In_Line, In_Low;
    int filter_Line, filter_Low;
    int Out_Line, Out_Low, Out_Channel;
    int size4;

    In_Line = sizeof(In[0][0]) / sizeof(In[0][0][0]);          //1次元の大きさと2次元の大きさを出している
    In_Low =  sizeof(In[0][0][0]) / sizeof(In[0][0][0][0]);

    filter_Line = sizeof(filter_c[0]) / sizeof(filter_c[0][0]);    
    filter_Low = sizeof(filter_c[0][0]) / sizeof(filter_c[0][0][0]);

    Out_Line = (In_Line - filter_Line) / stride_c + 1;
    Out_Low = (In_Low - filter_Low) / stride_c + 1;
    Out_Channel = sizeof(filter_c) / sizeof(filter_c[0]);

    size4 = sizeof(In) / sizeof(In[0]);

    z_c = (int****)malloc(sizeof(int***) * size4);       //zの動的確保
    a_c = (double****)malloc(sizeof(double***) * size4);           
    for(i = 0; i < size4; i++) {
        z_c[i] = (int***)malloc(sizeof(int**) * Out_Channel);
        a_c[i] = (double***)malloc(sizeof(double**) * Out_Channel);
        for(j = 0; j < Out_Channel; j++) {
            z_c[i][j] = (int**)malloc(sizeof(int*) * Out_Low);
            a_c[i][j] = (double**)malloc(sizeof(double*) * Out_Low);
            for(k = 0; k < Out_Low; k++) {
                z_c[i][j][k] = (int*)malloc(sizeof(int) * Out_Line);
                a_c[i][j][k] = (double*)malloc(sizeof(double) * Out_Line);
            }
        }
    }

    Convolutional_layer(In, filter_c, stride_c, bias_c, padding, In_Line, In_Low, filter_Line, filter_Low, Out_Line, Out_Low, Out_Channel, size4, a_c, z_c);


    for(i = 0; i ,size4; i++) {
        for(j = 0; j < Out_Channel; j++) {
            for(k = 0; k < Out_Low; k++) {
                for(l = 0; l < Out_Line; l++) {
                    printf("%f   ", a_c[i][j][k][l]);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n\n");

    for(i = 0; i < size4; i++) {
        for(j = 0; j < Out_Channel; j++) {
            for(k = 0; k < Out_Low; k++) {
                free(z_c[i][j][k]);
                free(a_c[i][j][k]);
            }
            free(z_c[i][j]);
            free(a_c[i][j]);
        }
        free(z_c[i]);
        free(a_c[i]);
    }
    free(z_c);
    free(a_c);

    return 0;
}*/

