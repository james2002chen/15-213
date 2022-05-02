/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
             tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char trans_0_desc[] = "block 8x8";
void trans_0(int M, int N, int A[N][M], int B[M][N])
{
    int ii,jj,i,j,bi=8,bj=8;
    for(ii=0;ii<N;ii+=bi){
        for(jj=0;jj<M;jj+=bj){
            for(i=ii;i<ii+bi;i++)
                for(j=jj;j<jj+bj;j++){
                    B[j][i] = A[i][j];
                }  
        } 
    }   
}


char trans_1_desc[] = "block 8x8, diagonal skip";
void trans_1(int M, int N, int A[N][M], int B[M][N])
{
    int ii,jj,i,j,temp=0,bi=8,bj=8;
    i=temp;
    for(ii=0;ii<N;ii+=bi){
        for(jj=0;jj<M;jj+=bj){
            for(j=jj;j<jj+bj;j++){
                for(i=ii;i<ii+bi;i++){
                    if(i==j) continue;
                    B[j][i]=A[i][j];
                } 
            }       
        } 
    }
    for(i=0;i<N;i++) B[i][i] = A[i][i];
}

char trans_2_desc[] = "block 8x8, diagonal temp_val";
void trans_2(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, v1, v2, v3, v4, v5, v6, v7, v8;
		for (i = 0; i < 32; i += 8)
			for(j = 0; j < 32; j += 8)
				for(k = i; k < (i + 8); ++k)
				{
					v1 = A[k][j];
					v2 = A[k][j+1];
					v3 = A[k][j+2];
					v4 = A[k][j+3];
					v5 = A[k][j+4];
					v6 = A[k][j+5];
					v7 = A[k][j+6];			
					v8 = A[k][j+7];
					B[j][k] = v1;
					B[j+1][k] = v2;
					B[j+2][k] = v3;
					B[j+3][k] = v4;
					B[j+4][k] = v5;
					B[j+5][k] = v6;
					B[j+6][k] = v7;
					B[j+7][k] = v8;
				}
}

char trans_3_desc[] = "block 8x8, diagonal complete save, 256 ideal";
void trans_3(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32)
    {
	int i, j, x1, x2, x3, x4, x5, x6, x7, x8, x, y;
	for (i = 0; i < N; i += 8)
		for (j = 0; j < N; j += 8)
		{
			if (i == j)
			{
				x=i;
				x1=A[x][j];x2=A[x][j+1];x3=A[x][j+2];x4=A[x][j+3];
				x5=A[x][j+4];x6=A[x][j+5];x7=A[x][j+6];x8=A[x][j+7];
 
				B[x][j]=x1;B[x][j+1]=x2;B[x][j+2]=x3;B[x][j+3]=x4;
				B[x][j+4]=x5;B[x][j+5]=x6;B[x][j+6]=x7;B[x][j+7]=x8;
 
				x1=A[x+1][j];x2=A[x+1][j+1];x3=A[x+1][j+2];x4=A[x+1][j+3];
				x5=A[x+1][j+4];x6=A[x+1][j+5];x7=A[x+1][j+6];x8=A[x+1][j+7];
 
				B[x+1][j]=B[x][j+1];B[x][j+1]=x1;
 
				B[x+1][j+1]=x2;B[x+1][j+2]=x3;B[x+1][j+3]=x4;
				B[x+1][j+4]=x5;B[x+1][j+5]=x6;B[x+1][j+6]=x7;B[x+1][j+7]=x8;
 
				x1=A[x+2][j];x2=A[x+2][j+1];x3=A[x+2][j+2];x4=A[x+2][j+3];
				x5=A[x+2][j+4];x6=A[x+2][j+5];x7=A[x+2][j+6];x8=A[x+2][j+7];
 
				B[x+2][j]=B[x][j+2];B[x+2][j+1]=B[x+1][j+2];
				B[x][j+2]=x1;B[x+1][j+2]=x2;B[x+2][j+2]=x3;
				B[x+2][j+3]=x4;B[x+2][j+4]=x5;B[x+2][j+5]=x6;B[x+2][j+6]=x7;B[x+2][j+7]=x8;
 
				x1=A[x+3][j];x2=A[x+3][j+1];x3=A[x+3][j+2];x4=A[x+3][j+3];
				x5=A[x+3][j+4];x6=A[x+3][j+5];x7=A[x+3][j+6];x8=A[x+3][j+7];
 
				B[x+3][j]=B[x][j+3];B[x+3][j+1]=B[x+1][j+3];B[x+3][j+2]=B[x+2][j+3];
				B[x][j+3]=x1;B[x+1][j+3]=x2;B[x+2][j+3]=x3;B[x+3][j+3]=x4;
				B[x+3][j+4]=x5;B[x+3][j+5]=x6;B[x+3][j+6]=x7;B[x+3][j+7]=x8;
 
				x1=A[x+4][j];x2=A[x+4][j+1];x3=A[x+4][j+2];x4=A[x+4][j+3];
				x5=A[x+4][j+4];x6=A[x+4][j+5];x7=A[x+4][j+6];x8=A[x+4][j+7];
 
				B[x+4][j]=B[x][j+4];B[x+4][j+1]=B[x+1][j+4];B[x+4][j+2]=B[x+2][j+4];B[x+4][j+3]=B[x+3][j+4];
				B[x][j+4]=x1;B[x+1][j+4]=x2;B[x+2][j+4]=x3;B[x+3][j+4]=x4;B[x+4][j+4]=x5;
				B[x+4][j+5]=x6;B[x+4][j+6]=x7;B[x+4][j+7]=x8;
 
				x1=A[x+5][j];x2=A[x+5][j+1];x3=A[x+5][j+2];x4=A[x+5][j+3];
				x5=A[x+5][j+4];x6=A[x+5][j+5];x7=A[x+5][j+6];x8=A[x+5][j+7];
 
				B[x+5][j]=B[x][j+5];B[x+5][j+1]=B[x+1][j+5];B[x+5][j+2]=B[x+2][j+5];B[x+5][j+3]=B[x+3][j+5];B[x+5][j+4]=B[x+4][j+5];
				B[x][j+5]=x1;B[x+1][j+5]=x2;B[x+2][j+5]=x3;B[x+3][j+5]=x4;B[x+4][j+5]=x5;B[x+5][j+5]=x6;
				B[x+5][j+6]=x7;B[x+5][j+7]=x8;
 
				x1=A[x+6][j];x2=A[x+6][j+1];x3=A[x+6][j+2];x4=A[x+6][j+3];
				x5=A[x+6][j+4];x6=A[x+6][j+5];x7=A[x+6][j+6];x8=A[x+6][j+7];
 
				B[x+6][j]=B[x][j+6];B[x+6][j+1]=B[x+1][j+6];B[x+6][j+2]=B[x+2][j+6];B[x+6][j+3]=B[x+3][j+6];
				B[x+6][j+4]=B[x+4][j+6];B[x+6][j+5]=B[x+5][j+6];
				B[x][j+6]=x1;B[x+1][j+6]=x2;B[x+2][j+6]=x3;B[x+3][j+6]=x4;B[x+4][j+6]=x5;B[x+5][j+6]=x6;
				B[x+6][j+6]=x7;B[x+6][j+7]=x8;
 
				x1=A[x+7][j];x2=A[x+7][j+1];x3=A[x+7][j+2];x4=A[x+7][j+3];
				x5=A[x+7][j+4];x6=A[x+7][j+5];x7=A[x+7][j+6];x8=A[x+7][j+7];
 
				B[x+7][j]=B[x][j+7];B[x+7][j+1]=B[x+1][j+7];B[x+7][j+2]=B[x+2][j+7];B[x+7][j+3]=B[x+3][j+7];
				B[x+7][j+4]=B[x+4][j+7];B[x+7][j+5]=B[x+5][j+7];B[x+7][j+6]=B[x+6][j+7];
				B[x][j+7]=x1;B[x+1][j+7]=x2;B[x+2][j+7]=x3;B[x+3][j+7]=x4;B[x+4][j+7]=x5;B[x+5][j+7]=x6;B[x+6][j+7]=x7;
				B[x+7][j+7]=x8;
			}
			else
			{
				for(x = i; x < (i + 8); ++x)
					for(y = j; y < (j + 8); ++y)
						B[y][x] = A[x][y];
			}
		}
    }
}
/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    //registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans_0, trans_0_desc); 
    //registerTransFunction(trans_1, trans_1_desc); 
    //registerTransFunction(trans_2, trans_2_desc); 
    registerTransFunction(trans_3, trans_3_desc); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

