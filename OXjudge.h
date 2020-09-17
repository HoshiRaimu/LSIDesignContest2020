void Convolutional_layer(int (*)[1][3][3], double (*)[2][2], int, double (*)[1][1], int, int, int, int, int, int, int, double (*)[2][2][2], double (*)[2][2][2]);
void Pooling_layer(double (*)[2][2][2], int, int, int, int, int, double (*)[2][1][1]);
void Output_layer(double (*)[2][1][1], double (*)[2], double (*)[1], int, int, double (*)[2][1], double (*)[2][1]);
void Output_Backprop(double (*)[2][1], int (*)[6], double (*)[2][1][1], double (*)[2], int, int, double (*)[2][1], double (*)[2][2], double (*)[2][1], double *);
void Pooling_Backprop(double (*)[2][2][2], int, double (*)[2][1][1], double (*)[2][1], int, int, double (*)[2][2][2]);
void Convolutional_Backprop(int (*)[1][3][3], double (*)[2][2], int, int, double (*)[2][2][2], double(*)[2][2][2], int, int, int, double (*)[2][3][3], double (*)[2][2], double (*)[2][1][1]);