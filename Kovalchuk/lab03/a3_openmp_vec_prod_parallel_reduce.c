#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[]) {
  if (argc < 5) {
    fprintf(stderr, "\e]31mUsage: a3 <mul1> <mul2> <size> <num_threads>\e]39m\n");
    return 1;
  }

  long a = atoi(argv[1]);  // The multiplier for the first vector
  long b = atoi(argv[2]);  // The multiplier for the second vector
  long N = atoi(argv[3]);  // The size of vectors
  long ordered_threads = atoi(argv[4]); // The number of ordered processes

  long *x = malloc(sizeof(long) * N);
  long *y = malloc(sizeof(long) * N);;

  for(long i = 0; i < N; i++) { /* Initialization */
    x[i] = a;  // The first vector
    y[i] = b;  // The second vector
  }

  long scalar_product = 0;
  double start_time = omp_get_wtime();
#pragma omp parallel num_threads(ordered_threads) default(none) shared(N, ordered_threads, x, y) reduction(+:scalar_product)
  {
    long task_id = omp_get_thread_num();
    long H = N / ordered_threads;
    long start_index = task_id * H;
    long end_index = start_index + H - 1;
    for(long i = start_index; i <= end_index; i++) {
      scalar_product = scalar_product + x[i] * y[i];
    }
  }
  double end_time = omp_get_wtime();  
  printf("Time: %f\n", end_time-start_time);

  printf("Scalar product(by reduction): vec(x) * vec(y) = %li\n", scalar_product);
  free(x);
  free(y);

  return 0;
}

