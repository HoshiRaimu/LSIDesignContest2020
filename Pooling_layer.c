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


/*int main() {
    int In_Line_PL, In_Low_PL, In_Channel_PL, K;
    int Out_Line_PL, Out_Low_PL, Out_Channel_PL;
    double ****a_p;
    int stride_p = 2;
    double ****a_c;
    double a_c[6][2][2][2] = {{{{0.9, 0.8}, {0.8, 0.8}}, {{0.5, 0.8}, {0.8, 0.5}}}, 
                  {{{0.98, 0.95}, {0.8, 0.7}}, {{0.5, 0.7}, {0.8, 0.7}}}, 
                  {{{0.98, 0.8}, {0.8, 0.95}}, {{0.5, 0.8}, {0.8, 0.2}}}, 
                  {{{0.7, 0.9}, {0.9, 0.7}}, {{0.9, 0.2}, {0.2, 0.9}}}, 
                  {{{0.7, 0.8}, {0.9, 0.7}}, {{0.9, 0.5}, {0.2, 0.9}}}, 
                  {{{0.7, 0.9}, {0.9, 0.8}}, {{0.9, 0.2}, {0.2, 0.8}}}};
    int i, j, k, l;

    K = 6;                   //K = sizeof(a_c) / sizeof(a_c[0]);
    In_Channel_PL = 2;    //In_Channel_PL = sizeof(a_c[0]) / sizeof(a_c[0][0]);
    In_Low_PL = 1;            //In_Low_PL = sizeof(a_c[0][0]) / sizeof(a_c[0][0][0]);
    In_Line_PL = 1;          //In_Line_PL = sizeof(a_c[0][0][0]) / sizeof(a_c[0][0][0][0]);

    Out_Line_PL = (In_Line_PL - stride_p) / stride_p + 1;
    Out_Low_PL = (In_Low_PL -stride_p) / stride_p + 1;
    Out_Channel_PL = In_Channel_PL;

    a_c = (double****)malloc(sizeof(double***) * 6);           
    for(i = 0; i < 6; i++) {
        a_c[i] = (double***)malloc(sizeof(double**) * 2);
        for(j = 0; j < 2; j++) {
            a_c[i][j] = (double**)malloc(sizeof(double*) * 2);
            for(k = 0; k < 2; k++) {
                a_c[i][j][k] = (double*)malloc(sizeof(double) * 2);
            }
        }
    }

    a_p = (double****)malloc(sizeof(double***) * K);       //a_pの動的確保          
    for(i = 0; i < K; i++) {
        a_p[i] = (double***)malloc(sizeof(double**) * Out_Channel_PL);
        for(j = 0; j < Out_Channel_PL; j++) {
            a_p[i][j] = (double**)malloc(sizeof(double*) * Out_Low_PL);
            for(k = 0; k < Out_Low_PL; k++) {
                a_p[i][j][k] = (double*)malloc(sizeof(double) * Out_Line_PL);
            }
        }
    }



    Pooling_layer(a_c, stride_p, Out_Line_PL, Out_Low_PL, Out_Channel_PL, K, a_p);


    for(i = 0; i < K; i++) {
        for(j = 0; j < In_Channel_PL; j++) {
            for(k = 0; k < In_Low_PL; k++) {
                for(l = 0; l < In_Line_PL; l++) {
                    printf("%f  ", a_p[i][j][k][l]);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n\n");
    }
    printf("\n");


    for(i = 0; i < 6; i++) {               //z_c,a_cの解放
        for(j = 0; j < 2; j++) {
            for(k = 0; k < 2; k++) {
                free(a_c[i][j][k]);
            }
            free(a_c[i][j]);
        }
        free(a_c[i]);
    }
    free(a_c);

    for(i = 0; i < 6; i++) {
        for(j = 0; j < 2; j++) {
            for(k = 0; k < 1; k++) {
                free(a_p[i][j][k]);
            }
            free(a_p[i][j]);
        }
        free(a_c[i]);
    }
    free(a_c);

    return 0;

}*/

