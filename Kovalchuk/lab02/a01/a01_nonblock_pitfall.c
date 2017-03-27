#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

const int MY_TAG = 42;
const int SIZE = 100000;


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

/*
 * The program is designed exactly for 2 processes.
 * An array of 5 elements is passed from process 0 to process 1
 * using non-blocking transfer. Process 1 starts receiving data
 * earlier than process 0 starts its transmission.
 */
int main(int argc, char* argv[]) {
  int np;
  int rank;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0) {
    printf("Task 0.  Generating data\n");
    int data[SIZE]; // Data to and transfer
    for (int i = 0; i < SIZE; ++i) { data[i] = i; } // Initialize dummy data

    MPI_Request send_req; /* Transfer (send) handle */

    printf("Task 0.  MPI_Isend()\n");
    MPI_Isend(&data, SIZE, MPI_INT, 1, MY_TAG, MPI_COMM_WORLD, &send_req);
    /* Now buffer 'data' is not available */
    printf("Task 0.  Sending data...\n");

    printf("Task 0.  Modifying data.\n");
    /* Do a weird fizzbuzz on array */
    for (int i = 0; i < SIZE; ++i) {
      data[i] = weirdFizzBuzz(data[i]);
    }

    printf("Task 0.  Computation finished.\n");

    /* Wait for transfer completion */
    MPI_Wait(&send_req, MPI_STATUS_IGNORE);
  }
  if(rank == 1) {
    int data[SIZE]; /* Receive buffer */

    printf("Task 1.  Generating verification data\n");
    int verify[SIZE];  /* verification */
    for (int i = 0; i < SIZE; ++i) { verify[i] = i; } 

    MPI_Request recv_req; /* Transfer (receive) handle */
    printf("Task 1.  MPI_Irecv()\n");
    MPI_Irecv(&data, SIZE, MPI_INT, 0, MY_TAG, MPI_COMM_WORLD, &recv_req);
    /* Now buffer 'data' is not available */
    printf("Task 1.  Recieving data...\n");
    
    /* Wait for transfer completion */
    MPI_Wait(&recv_req, MPI_STATUS_IGNORE);

    printf("Task 1.  Data recieved\n");
    /* Verify data */
    char is_valid = 1;
    char step_valid;
    for(int i = 0; i < SIZE; i++) {
      step_valid = (data[i] == verify[i]);
      if (!step_valid) {
        printf("Task 1. Expected %d, but received %d at position %d\n", verify[i], data[i], i);
      }
      is_valid &= step_valid;
    }
    printf("Task 1. Data valid: %s\n", is_valid ? "true" : "false");
  }

  MPI_Finalize();
  return 0;
}
