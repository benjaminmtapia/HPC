# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <pmmintrin.h>
# include <immintrin.h>

int* readFile(char* fileName, int M, int N);
void writeOut(int** image, int M, int R, char* fileName);
int** imageToMatrix(int* image,int M, int N);
float* getDistances(float deltaR,int R);
float* getAngles(float deltaTheta,int M);
int** umbralization(int** houghMatrix, int M, int R, int U);
int** houghMatrix(int M, int N);