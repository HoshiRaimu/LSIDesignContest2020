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

    //Pooling_Backpropにかんする変数
    int k_PB, channel_PB;
    double dz_p[6][2][2][2];
    //--------------------------------

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

    Convolutional_layer(In, filter_c, stride_c, bias_c, padding, filter_Line, filter_Low, Out_Line_CL, Out_Low_CL, Out_Channel_CL, size4, a_c, z_c);

    //Pooling_layer
    K = size4;                   //K = sizeof(a_c) / sizeof(a_c[0]);
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

    //Pooling_Backprop
    k_PB = K;
    channel_PB = Out_Channel_PL;

    Pooling_Backprop(a_c, stride_p, a_p, dz_o, k_PB, channel_PB, dz_p);


    for(i = 0; i < 6; i++) {
        for(j = 0; j < 2; j++) {
            for(k = 0; k < 2; k++) {
                for(l = 0; l < 2; l++) {
                    printf("%f ", dz_p[i][j][k][l]);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");


    return 0;

}