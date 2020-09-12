void Convolutional_layer(int (*)[1][3][3], int (*)[2][2], int, double (*)[1][1], int, int, int, int, int, int, int, double ****, double ****);
void Pooling_layer(double ****, int, int, int, int, int, double ****);
void Output_layer(double ****, double (*)[2], double (*)[1], int, int, double ***, double ***);
void Output_Backprop(double ***, int (*)[6], double ****, double (*)[2], int, int, double (*)[2][1], double (*)[2][2], double (*)[2][1], double *);