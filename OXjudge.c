#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "OXjudge.h"

int main() {
    int MARU[3][3] = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    int BATU[3][3] = {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}};
    double filter_c[2][2][2] = {{{1, 1}, {1, -1}}, {{1, -1}, {-1, 1}}};
    int In[6][1][3][3] = {{{{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 0}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 1}, {1, 1, 0}}}, 
                          {{{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 0}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 1}, {0, 1, 0}, {1, 0, 0}}}};
    int t[2][6] = {{1, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 1}};
    int stride_c = 1;
    double bias_c[2][1][1] = {{{1}}, {{1}}};       
    int padding = 0;
    int stride_p = 2;
    double w_o[2][2] = {{1.5, -2.6}, {-4.2, 3.5}};
    double bias_o[2][1] = {{1}, {1}};
    double eta = 0.1;

    //Convolutinal_layerに関する変数
    int In_Line_CL, In_Low_CL;
    int filter_Line, filter_Low;
    int Out_Line_CL, Out_Low_CL, Out_Channel_CL;
    int size4;
    double z_c[6][2][2][2];
    double a_c[6][2][2][2];
    //--------------------------------

    //Pooling_layerに関する変数
    int In_Line_PL, In_Low_PL, In_Channel_PL, K;
    int Out_Line_PL, Out_Low_PL, Out_Channel_PL;
    double a_p[6][2][1][1];
    //--------------------------------

    //Output_layerに関する関数
    int line_OL, low_OL, channel_OL, k_OL;
    double z_o[6][2][1];
    double a_o[6][2][1];
    //--------------------------------

    //Output_Backpropに関する変数
    int k_OB, channel_OB, low_OB, line_OB;
    double dz_o[6][2][1];
    double dw_o[6][2][2];
    double db_o[6][2][1];
    double Ct;

    double CT;  //matlabでは配列になっているがとりあえず変数で対応
    //--------------------------------

    //Pooling_Backpropに関する変数
    int k_PB, channel_PB;
    double dz_p[6][2][2][2];
    //--------------------------------

    //Convolutional_Backpropに関する変数
    int k_CB;
    int f_channel, z_k;
    double da_c[6][2][3][3];
    double dw_c[2][2][2];
    double db_c[6][2][1][1];
    //--------------------------------

    //Kenshoに関する変数
    int IN[6][1][3][3] = {{{{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}}, {{{1, 1, 1}, {1, 0, 1}, {1, 1, 0}}}, 
                          {{{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 0}, {0, 1, 0}, {1, 0, 1}}}, {{{1, 0, 1}, {0, 1, 0}, {1, 0, 0}}}};
    double Z_C[6][2][2][2];
    double A_C[6][2][2][2];
    double A_P[6][2][1][1];
    double Z_O[6][2][1];
    double A_O[6][2][1];
    char result;


    int i, j, k, l;
    int I;
    int size1, size2, size3;
    double sum = 0;

    FILE *fp;

    fp = fopen("result.txt", "w");
    if(fp == NULL) {
        printf("Cannot open file result.txt\n");
        exit(1);
    }

    for(I = 0; I < 5000; I++) {
        //Convolutional_layer
        In_Line_CL = sizeof(In[0][0]) / sizeof(In[0][0][0]);          //Convolutianl_layerで行うべきだが、うまくいかないからここで行う
        In_Low_CL =  sizeof(In[0][0][0]) / sizeof(In[0][0][0][0]);

        filter_Line = sizeof(filter_c[0]) / sizeof(filter_c[0][0]);    
        filter_Low = sizeof(filter_c[0][0]) / sizeof(filter_c[0][0][0]);

        Out_Line_CL = (In_Line_CL - filter_Line) / stride_c + 1;
        Out_Low_CL = (In_Low_CL - filter_Low) / stride_c + 1;
        Out_Channel_CL = sizeof(filter_c) / sizeof(filter_c[0]);

        size4 = sizeof(In) / sizeof(In[0]);

        Convolutional_layer(In, filter_c, stride_c, bias_c, padding, filter_Line, filter_Low, Out_Line_CL, Out_Low_CL, Out_Channel_CL, size4, a_c, z_c);

        //Pooling_layer
        K = size4;                         //K = sizeof(a_c) / sizeof(a_c[0]);
        In_Channel_PL = Out_Channel_CL;    //In_Channel_PL = sizeof(a_c[0]) / sizeof(a_c[0][0]);
        In_Low_PL = Out_Low_CL;            //In_Low_PL = sizeof(a_c[0][0]) / sizeof(a_c[0][0][0]);
        In_Line_PL = Out_Line_CL;          //In_Line_PL = sizeof(a_c[0][0][0]) / sizeof(a_c[0][0][0][0]);

        Out_Line_PL = (In_Line_PL - stride_p) / stride_p + 1;
        Out_Low_PL = (In_Low_PL -stride_p) / stride_p + 1;
        Out_Channel_PL = In_Channel_PL;

        Pooling_layer(a_c, stride_p, Out_Line_PL, Out_Low_PL, Out_Channel_PL, K, a_p);

        //Output_Layer
        k_OL = K;
        channel_OL = Out_Channel_PL;
        low_OL = Out_Low_PL;
        line_OL = Out_Line_PL;

        Output_layer(a_p, w_o, bias_o, k_OL, channel_OL, z_o, a_o);

        //Output_Backprop
        k_OB = k_OL;
        channel_OB = channel_OL;
        //low_OB = low_OL;
        //line_OB = low_OL;

        Output_Backprop(a_o, t, a_p, w_o, k_OB, channel_OB, dz_o, dw_o, db_o, &Ct);

        CT = Ct;
        fprintf(fp, "%f\n", CT);

        //Pooling_Backprop
        k_PB = K;
        channel_PB = Out_Channel_PL;

        Pooling_Backprop(a_c, stride_p, a_p, dz_o, k_PB, channel_PB, dz_p);


        //Convolutional_Backprop
        k_CB = sizeof(In) / sizeof(In[0]);                  //関数に配列を渡してからここの処理の行い方を知らないため、ここで行う
        f_channel = sizeof(filter_c) / sizeof(filter_c[0]);
        z_k = sizeof(dz_p) / sizeof(dz_p[0]);

        Convolutional_Backprop(In, filter_c, stride_c, padding, dz_p, a_c, k_CB, f_channel, z_k, da_c, dw_c, db_c);


        //Update Value of Output Layer
        for(i = 0; i < 2; i++) {
            for(j = 0; j < 2; j++) {
                for(k = 0; k < 6; k++) {
                    sum += dw_o[k][i][j];
                }
                w_o[i][j] = w_o[i][j] - sum * eta;
                sum = 0;
            }
        }

        for(i = 0; i < 2; i++) {
            for(j = 0; j < 1; j++) {
                for(k = 0; k < 6; k++) {
                    sum += db_o[k][i][j];
                }
                bias_o[i][j] = bias_o[i][j] - sum * eta;
                sum = 0;
            }
        }


        //Update Value of Convolutional Layer
        for(i = 0; i < 2; i++) {
            for(j = 0; j < 6; j++) {
                sum += db_c[j][i][0][0];
            }
            bias_c[i][0][0] = bias_c[i][0][0] - sum * eta;
            sum = 0;
        }

        for(i = 0; i < 2; i++) {
            for(j = 0; j < 2; j++) {
                for(k = 0; k < 2; k++) {
                    filter_c[i][j][k] = filter_c[i][j][k] - dw_c[i][j][k] * eta;
                }
            }
        }
    }

    fclose(fp);



    //Kensho
    
    Convolutional_layer(In, filter_c, stride_c, bias_c, padding, filter_Line, filter_Low, Out_Line_CL, Out_Low_CL, Out_Channel_CL, size4, A_C, Z_C);
    Pooling_layer(A_C, stride_p, Out_Line_PL, Out_Low_PL, Out_Channel_PL, K, A_P);
    Output_layer(A_P, w_o, bias_o, k_OL, channel_OL, Z_O, A_O);

    for(i = 0; i < 6; i++) {
        for(j = 0; j < 2; j++) {
            for(k = 0; k < 1; k++) {
                printf("%f ", A_O[i][j][k]);
            }
            printf("\n");
        }
        if(A_O[i][0][0] > A_O[i][1][0]) result = 'O';
        else result = 'X';

        switch(i+1) {
        case 1:
            printf("The %dst input is %c\n\n", i+1, result);
            break;
        case 2:
            printf("The %dnd input is %c\n\n", i+1, result);
            break;
        case 3:
            printf("The %drd input is %c\n\n", i+1, result);
            break;
        default:
            printf("The %dth input is %c\n\n", i+1, result);
            break;
        }

    }
    printf("\n");

    return 0;
}