#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define m 2048
#define n 2048
//#define size 5

#define tile_width 16

using namespace std;

/*
__global__ void add_1(int a[], int b[], int c[], int fila, int columna)
{
        int index =(blockDim.x * blockIdx.x)+threadIdx.x;
        if(blockIdx.x<fila && threadIdx.x<columna)
                c[index]=a[index]+b[index];
}
*/

__global__ void mult_matrix(int a[],int b[], int c[],int fila,int columna)
{
        int row = blockIdx.y*blockDim.y+threadIdx.y;
        int col = blockIdx.x*blockDim.x+threadIdx.x;

        if((row<fila)&&(col<columna))
        {
                int Pvalue = 0;
                for(int k=0;k<fila;++k)
                {
                        Pvalue += a[row*fila+k]*b[k*columna+col];
                }
                c[row*fila+col] = Pvalue;
        }
}


__global__ void mult_matrix_tile(int a[], int b[], int c[],int fila,int columna){
        __shared__ int a_ds[tile_width][tile_width];
        __shared__ int b_ds[tile_width][tile_width];

        int bx = blockIdx.x; int by=blockIdx.y;
        int tx = threadIdx.x; int ty=threadIdx.y;

        int row = by * tile_width + ty;
        int col = bx * tile_width + tx;

        float Pvalue = 0;
        for(int ph=0;ph<fila/tile_width;++ph)
        {
                a_ds[ty][tx] = a[row*fila+ ph*tile_width + tx];
                b_ds[ty][tx] = b[(ph*tile_width+ty)*fila+col];
                __syncthreads();

                for(int k=0;k<tile_width;++k)
                {
                        Pvalue += a_ds[ty][k]*b_ds[k][tx];
                }
                __syncthreads();
        }
        c[row*fila+col] = Pvalue;
}

__global__ void mult_matrix_tile_2(int a[], int b[], int c[],int fila,int columna){
        __shared__ int a_ds[tile_width][tile_width];
        __shared__ int b_ds_0[tile_width][tile_width];
        __shared__ int b_ds_1[tile_width][tile_width];
        __shared__ int b_ds_2[tile_width][tile_width];
        __shared__ int b_ds_3[tile_width][tile_width];

        const uint bx = blockIdx.x; 
        const uint by=blockIdx.y;
        
        const uint tx = threadIdx.x; 
        const uint ty=threadIdx.y;

        const uint row = by * tile_width + ty;

        const uint col_0 = (4*bx+0 )* tile_width + tx;
        const uint col_1 = (4*bx+1 )* tile_width + tx;
        const uint col_2 = (4*bx+2 )* tile_width + tx;
        const uint col_3 = (4*bx+3 )* tile_width + tx;


        float Pvalue_0 = 0.0f, Pvalue_1 = 0.0f, Pvalue_2 = 0.0f, Pvalue_3 = 0.0f; 
        for(uint ph=0;ph<fila/tile_width;++ph)
        {
                a_ds[ty][tx] = a[row*fila+ ph*tile_width + tx];

                b_ds_0[ty][tx] = b[(ph*tile_width+ty)*fila+col_0];
                b_ds_1[ty][tx] = b[(ph*tile_width+ty)*fila+col_1];
                b_ds_2[ty][tx] = b[(ph*tile_width+ty)*fila+col_2];
                b_ds_3[ty][tx] = b[(ph*tile_width+ty)*fila+col_3];
                
                __syncthreads();

                for(uint k=0;k<tile_width;++k)
                {
                        Pvalue_0 += a_ds[ty][k]*b_ds_0[k][tx];
                        Pvalue_1 += a_ds[ty][k]*b_ds_1[k][tx];
                        Pvalue_2 += a_ds[ty][k]*b_ds_2[k][tx];
                        Pvalue_3 += a_ds[ty][k]*b_ds_3[k][tx];
                }
                __syncthreads();
        }
        c[row*fila+col_0] = Pvalue_0;
        c[row*fila+col_1] = Pvalue_1;
        c[row*fila+col_2] = Pvalue_2;
        c[row*fila+col_3] = Pvalue_3;
}

void llenar_random_matrix(int a[],int f ,int c)
{
        for(int i=0; i<f ;i++)
        {
                for(int j=0; j<c;j++)
                {
                        a[(i*n)+j] = rand()%3;
                }
        }
}

void print_matrix(int a[],int f,int c)
{
        for(int i=0; i<f;i++)
        {
                for(int j=0; j<c;j++)
                        printf("%d ",a[(i*n)+j]);
                printf("\n");
        }
}


int main(int argc, char * argv[]){

        int *a, *b, *c;
        int *d_a, *d_b, *d_c;


        int size = m*n*sizeof(int);

        //separar espacion de memoria para copias en device

        float gpu_elapsed_time_ms, cpu_elapsed_time_ms;

        // some events to count the execution time
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // start to count execution time of GPU version
        cudaEventRecord(start, 0);

        cudaMalloc((void **)&d_a, size);
        cudaMalloc((void **)&d_b, size);
        cudaMalloc((void **)&d_c, size);
        //separa espacio en host

        a = (int *)malloc(size);
        llenar_random_matrix(a,m,n);
        b = (int *)malloc(size);
        llenar_random_matrix(b,m,n);
        c = (int *)malloc(size);
        // imprimir matrices a , b

        print_matrix(a, m, n);
        printf("-----------------\n");
        print_matrix(b, m, n);
        printf("-----------------\n");
        // copias entrada a device

        cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
        cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

        // lanzar funcion kernel en gpu con N blocks

       
        unsigned int grid_rows = (m)/tile_width;
        unsigned int grid_cols = (n)/tile_width;

        //printf("m+tile -1: %d\n",m+tile_width-1);
        printf("g_row: %d\n",grid_rows);
        printf("g_col: %d\n",grid_cols);

        dim3 dimGrid(grid_cols, grid_rows);
        dim3 dimBlock(tile_width,tile_width);
        mult_matrix_tile<<<dimGrid,dimBlock>>>(d_a,d_b,d_c,m,n);
        //mult_matrix<<<dimGrid,dimBlock>>>(d_a,d_b,d_c,m,n);
        // add_3<<<n,1>>>(d_a,d_b,d_c,m,n);

        // copia resultado al host
        cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
        //imprimir c result

        cudaEventRecord(stop, 0);
        cudaEventSynchronize(stop);

        // compute time elapse on GPU computing
        cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);
        printf("Time elapsed on matrix multiplication of GPU: %f : \n",  gpu_elapsed_time_ms);

        print_matrix(c,m,n);
        // limpiar memoria
        free(a); free(b); free(c);
        cudaFree(d_a); cudaFree(d_b);cudaFree(d_c);
        return 0;
}

//// tiled Time elapsed on matrix multiplication of GPU: 1.258784 :

/// normal 
