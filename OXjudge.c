#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "OXjudge.h"

int main() {
    int MARU[3][3] = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    int BATU[3][3] = {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}};
    int filter_c[2][2][2] = {{{1, 1}, {1, -1}}, {{1, -1}, {-1, 1}}};
    int In[6][1][3][3] = {{{{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 0}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 1}, {1, 1, 0}}}, 
                          {{{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 0}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 1}, {0, 1, 0}, {1, 0, 0}}}};
    int t[2][6] = {{1, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 1}};
    int stride_c = 1;
    double bias_c[2][1][1] = {{{1}}, {{1}}};       
    int padding = 0;
    int stride_p = 2;
    double w_o[2][2] = {{1.5, -2.6}, {-4.2, -3.5}};
    double bias_o[2][1] = {{1}, {1}};
    double eta = 0.1;

    //Convolutinal_layerに関する変数
    int In_Line_CL, In_Low_CL;
    int filter_Line, filter_Low;
    int Out_Line_CL, Out_Low_CL, Out_Channel_CL;
    int size4;
    double ****z_c;
    double ****a_c;
    //--------------------------------

    //Pooling_layerに関する変数
    int In_Line_PL, In_Low_PL, In_Channel_PL, K;
    int Out_Line_PL, Out_Low_PL, Out_Channel_PL;
    double ****a_p;
    //--------------------------------

    //Output_layerに関する関数
    int line_OL, low_OL, channel_OL, k_OL;
    double ****z_o;
    double ****a_o;

    int i, j, k, l;
    int size1, size2, size3;


    //Convolutional_layer
    In_Line_CL = sizeof(In[0][0]) / sizeof(In[0][0][0]);          //Convolutianl_layerで行うべきだが、うまくいかないからここで行う
    In_Low_CL =  sizeof(In[0][0][0]) / sizeof(In[0][0][0][0]);

    filter_Line = sizeof(filter_c[0]) / sizeof(filter_c[0][0]);    
    filter_Low = sizeof(filter_c[0][0]) / sizeof(filter_c[0][0][0]);

    Out_Line_CL = (In_Line_CL - filter_Line) / stride_c + 1;
    Out_Low_CL = (In_Low_CL - filter_Low) / stride_c + 1;
    Out_Channel_CL = sizeof(filter_c) / sizeof(filter_c[0]);

    size4 = sizeof(In) / sizeof(In[0]);

    z_c = (double****)malloc(sizeof(double***) * size4);       //z_c,a_cの動的確保
    a_c = (double****)malloc(sizeof(double***) * size4);           
    for(i = 0; i < size4; i++) {
        z_c[i] = (double***)malloc(sizeof(double**) * Out_Channel_CL);
        a_c[i] = (double***)malloc(sizeof(double**) * Out_Channel_CL);
        for(j = 0; j < Out_Channel_CL; j++) {
            z_c[i][j] = (double**)malloc(sizeof(double*) * Out_Low_CL);
            a_c[i][j] = (double**)malloc(sizeof(double*) * Out_Low_CL);
            for(k = 0; k < Out_Low_CL; k++) {
                z_c[i][j][k] = (double*)malloc(sizeof(double) * Out_Line_CL);
                a_c[i][j][k] = (double*)malloc(sizeof(double) * Out_Line_CL);
            }
        }
    }

    Convolutional_layer(In, filter_c, stride_c, bias_c, padding, filter_Line, filter_Low, Out_Line_CL, Out_Low_CL, Out_Channel_CL, size4, a_c, z_c);




    //Pooling_layer
    K = size4;                   //K = sizeof(a_c) / sizeof(a_c[0]);
    In_Channel_PL = Out_Channel_CL;    //In_Channel_PL = sizeof(a_c[0]) / sizeof(a_c[0][0]);
    In_Low_PL = Out_Low_CL;            //In_Low_PL = sizeof(a_c[0][0]) / sizeof(a_c[0][0][0]);
    In_Line_PL = Out_Line_CL;          //In_Line_PL = sizeof(a_c[0][0][0]) / sizeof(a_c[0][0][0][0]);

    Out_Line_PL = (In_Line_PL - stride_p) / stride_p + 1;
    Out_Low_PL = (In_Low_PL -stride_p) / stride_p + 1;
    Out_Channel_PL = In_Channel_PL;

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




    //Output_Layer
    /*k_OL = K;
    channel_OL = Out_Channel_PL;
    low_OL = Out_Low_PL;
    line_OL = Out_Line_PL;

    z_o = (double****)malloc(sizeof(double***) * k_OL);       //z_o,a_oの動的確保
    a_o = (double****)malloc(sizeof(double***) * k_OL);           
    for(i = 0; i < k_OL; i++) {
        z_o[i] = (double***)malloc(sizeof(double**) * channel_OL);
        a_o[i] = (double***)malloc(sizeof(double**) * channel_OL);
        for(j = 0; j < channel_OL; j++) {
            z_o[i][j] = (double**)malloc(sizeof(double*) * 1);
            a_o[i][j] = (double**)malloc(sizeof(double*) * 1);
        }
    }

    Output_layer(a_p, w_o, bias_o, k_OL, channel_OL,low_OL, line_OL, z_o, a_o);*/
    //printf("%d %d %d %d \n", Out_Line_PL, Out_Low_PL, Out_Channel_PL, 5);

    for(i = 0; i < K; i++) {
        for(j = 0; j < Out_Channel_PL; j++) {
            for(k = 0; k < Out_Low_PL; k++) {
                for(l = 0; l < Out_Line_PL; l++) {
                    printf("%f   ", a_p[i][j][k][l]);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");

    for(i = 0; i < size4; i++) {               //z_c,a_cの解放
        for(j = 0; j < Out_Channel_CL; j++) {
            for(k = 0; k < Out_Low_CL; k++) {
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

    for(i = 0; i < K; i++) {                   //a_pの解放
        for(j = 0; j < Out_Channel_PL; j++) {
            for(k = 0; k < Out_Low_PL; k++) {
                free(a_p[i][j][k]);
            }
            free(a_p[i][j]);
        }
        free(a_p[i]);
    }
    free(a_p);

    for(i = 0; i < k_OL; i++) {               //z_o,a_oの解放
        for(j = 0; j < channel_OL; j++) {
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