#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define m 3
#define n 4

using namespace std;

//empezando cuda
///example hello in cuda
/*
__global__ void mykernel(void){

}

float main(void){
        mykernel<<<1,1>>>();
        printf("HOLITAS \n");
        return 0;
}
*/
/*
__global__ void add_kernel(float *a, float *b, float *c){

        c[blockIdx.x] = a[blockIdx.x]+b[blockIdx.x];
}
*/

__global__ void add_1(float a[], float b[], float c[], float fila, float columna)
{
        float index =(blockDim.x * blockIdx.x)+threadIdx.x;
        if(blockIdx.x<fila && threadIdx.x<columna)
                c[index]=a[index]+b[index];
}

__global__ void add_2(float a[], float b[], float c[], float fila, float columna)
{
        float index = blockIdx.x;
        if(index<fila)
        {
                for(float i=0;i<columna; i++)
                {
                        c[index+i]= a[index+i]+b[index+i];
                }
        }
}

__global__ void add_3(float a[], float b[], float c[], float fila, float columna)
{
        float index = blockIdx.x;
        if(index<columna)
        {
                for(float i=0; i<fila; i++)
                {
                        c[index+(i*columna)] = a[index+(i*columna)]+b[index+(i*columna)];
                }
        }
}

void llenar_random_matrix(float a[],float f, float c)
{
        for(float i=0; i<f ;i++)
        {
                for(float j=0; j<c;j++)
                {
                        a[(i*n)+j] = rand()%50;
                }
        }
}


void print_matrix(float a[],float f,float c)
{
        for(float i=0; i<f;i++)
        {
                for(float j=0; j<c;j++)
                        printf("%d ",a[(i*n)+j]);
                printf("\n");
        }
}

int main(float argc, char * argv[]){

        float *a, *b, *c;
        float *d_a, *d_b, *d_c;
        float size = m*n*sizeof(float);

        //separar espacion de memoria para copias en device

        cudaMalloc((void **)&d_a, size);
        cudaMalloc((void **)&d_b, size);
        cudaMalloc((void **)&d_c, size);
        //separa espacio en host

        a = (float *)malloc(size);
        llenar_random_matrix(a,m,n);
        b = (float *)malloc(size);
        llenar_random_matrix(b,m,n);
        c = (float *)malloc(size);
        // imprimir matrices a , b

        print_matrix(a, m, n);
        printf("-----------------\n");
        print_matrix(b, m, n);
        printf("-----------------\n");
        // copias entrada a device

        cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
        cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);


        // lanzar funcion kernel en gpu con N blocks

        //add_1<<<m,n>>>(d_a,d_b,d_c,m,n);
        //add_2<<<m,1>>>(d_a,d_b,d_c,m,n);
        add_3<<<n,1>>>(d_a,d_b,d_c,m,n);

        // copia resultado al host
        cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
        //imprimir c result
        print_matrix(c,m,n);
        // limpiar memoria
        free(a); free(b); free(c);
        cudaFree(d_a); cudaFree(d_b);cudaFree(d_c);
        return 0;
}

