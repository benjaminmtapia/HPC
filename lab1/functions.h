# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <pmmintrin.h>
# include <immintrin.h>
# include <time.h>
#include <fcntl.h>

#include <math.h>

int* readFile(char* fileName, int M, int N);
void writeOut(int** image, int M, int R, char* fileName);
int** imageToMatrix(int* image,int M, int N);
float* getDistances(float deltaR,int R);
float* getAngles(float deltaTheta,int M);
int** umbralization(int** houghMatrix, int M, int R, int U,float deltaTheta, float deltaR);
int** houghMatrix(int M, int N);
void write_image(int **matriz, char *OUTPUT_PATH, int T, int R);
int **read_image(char *PATH_FILE, int M, int N);

