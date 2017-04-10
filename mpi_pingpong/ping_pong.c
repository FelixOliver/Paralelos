
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int PING_PONG_MAX = 10;

  // Iniciamos MPI
  MPI_Init(NULL, NULL);
  // declaro rank & size
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int ping_pong_c = 0;
  int rank_vecino = (rank + 1) % 2;
  ///
  printf("rank: %d \n",rank);
  printf("vecino rank: %d \n",rank_vecino);
  
  ///
  while (ping_pong_c < PING_PONG_MAX) {
    if (rank == ping_pong_c % 2) {
      // incrementamos ping_pong_c
      ping_pong_c++;
      MPI_Send(&ping_pong_c, 1, MPI_INT, rank_vecino, 0, MPI_COMM_WORLD);
      printf("%d envia e incrementa pingpong_c %d --> %d\n",
             rank, ping_pong_c, rank_vecino);
    } else {
      MPI_Recv(&ping_pong_c, 1, MPI_INT, rank_vecino, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      printf("%d recive pingpong_c %d de %d\n",
             rank, ping_pong_c, rank_vecino);
    }
  }
  MPI_Finalize();
}
