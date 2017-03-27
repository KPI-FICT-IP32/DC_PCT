#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>

const int MY_TAG = 42;
const int SIZE = 500000;


int weirdFizzBuzz(int i) {
  char is_fizz = !(i % 3);
  char is_buzz = !(i % 5);

  if (is_fizz && is_buzz) {
    return 0;
  } else if (is_fizz) {
    return -3;
  } else if (is_buzz) {
    return -5;
  } else {
    return i;
  }
}

int main(int argc, char* argv[]) {
  int np;
  int rank;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  struct timespec ts_start;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);

  if(rank == 0) {
    printf("Task 0. Generating data\n");
    int data[SIZE]; // Data to process and transfer
    for (int i = 0; i < SIZE; ++i) { data[i] = i; } // Initialize dummy data

    printf("Task 0.  MPI_Send()\n");
    MPI_Send(&data, SIZE, MPI_INT, 1, MY_TAG, MPI_COMM_WORLD);
    /* Now buffer 'data' is not available */
    printf("Task 0.  Sending data...\n");

    int reverse[SIZE];
    for (int i = 0; i < SIZE; ++i) {
      reverse[i] = SIZE - i - 1;
    }

    int result[SIZE];
    for (int i = 0; i < SIZE; ++i) {
      result[i] = weirdFizzBuzz(data[i]) + reverse[i];
    }

    printf("Task 0.  Computation finished.\n");
    printf("Task 0.  First 10 elements: %d %d %d %d %d %d %d %d %d %d\n",
           result[0], result[1], result[2], result[3], result[4],
           result[5], result[6], result[7], result[8], result[9]); 
  }
  if(rank == 1) {
    int data[SIZE]; /* Receive buffer */
    MPI_Status status;

    printf("Task 1. Generating verification data\n");
    int mul[SIZE];
    for (int i = 0; i < SIZE; ++i) { mul[i] = i % 2 ? 0 : i / 2; } // Initialize dummy data

    printf("Task 1.  MPI_Recv()\n");
    printf("Task 1.  Recieving data...\n");
    MPI_Recv(&data, SIZE, MPI_INT, 0, MY_TAG, MPI_COMM_WORLD, &status);
    /* Now buffer 'data' is not available */
    
    printf("Task 1.  Data recieved.\n");

    printf("Task 1.  Calculating the result\n");
    int result[SIZE];
    for (int i = 0; i < SIZE; ++i) {
      result[i] = mul[i] * data[i];
    }
    printf("Task 1.  First 10 elements: %d %d %d %d %d %d %d %d %d %d\n",
           result[0], result[1], result[2], result[3], result[4],
           result[5], result[6], result[7], result[8], result[9]); 
  }

  struct timespec ts_end;
  clock_gettime(CLOCK_MONOTONIC, &ts_end);
  long nano_start = ts_start.tv_sec * 1000000000 + ts_start.tv_nsec;
  long nano_end = ts_end.tv_sec * 1000000000 + ts_end.tv_nsec;
  printf("Task %d. Time used %ld ns\n", rank, nano_end - nano_start);

  MPI_Finalize();
  return 0;
}
