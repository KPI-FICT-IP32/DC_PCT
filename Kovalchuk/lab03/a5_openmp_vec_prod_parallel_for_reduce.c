#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[]) {
  if (argc < 5) {
    fprintf(stderr, "\e]31mUsage: a5 <mul1> <mul2> <size> <num_threads>\e]39m\n");
    return 1;
  }
  long a = atoi(argv[1]);  // The multiplier for the first vector
  long b = atoi(argv[2]);  // The multiplier for the second vector
  long N = atoi(argv[3]);  // The size of vectors
  long ordered_threads = atoi(argv[4]); // The number of ordered processes

  long *x = malloc(sizeof(long) * N);
  long *y = malloc(sizeof(long) * N);

  /* Initialization */
  for(long i = 0; i < N; i++) {
    x[i] = a;  // The first vector
    y[i] = b;  // The second vector
  }

  long scalar_product = 0;
  double start_time = omp_get_wtime();

#pragma omp parallel num_threads(ordered_threads) default(none) shared(N, ordered_threads, x, y) reduction(+:scalar_product)
#pragma omp for schedule(static)
  for(long i = 0; i < N; i++) {
    scalar_product  = scalar_product + x[i] * y[i];
  }

  double end_time = omp_get_wtime();  
  printf("Time: %f\n", end_time-start_time);

  printf("Scalar product(reduction for - good): vec(x) * vec(y) = %li\n", scalar_product);
  free(x);
  free(y);

  return 0;
}

