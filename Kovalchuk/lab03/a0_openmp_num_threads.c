#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "\e[31mUsage: a0 <x> <ordered_threads>\e[39m\n");
    return 1;
  }
  double start_time = omp_get_wtime();

  int x = atoi(argv[1]);
  int ordered_threads = atoi(argv[2]);

  // Case 1
#pragma omp parallel if(x < 0) num_threads(5)
  printf("part 1 (parallel if(x < 0) num_threads(5))\n");

  // Case 2
#pragma omp parallel if(x > 0) num_threads(3)
  printf("part 2 (parallel if(x > 0) num_threads(3))\n");

  int get_threads = omp_get_num_threads();
  printf ("Threads (get 1): %d\n", get_threads);
  int max_threads = omp_get_max_threads();
  printf ("Threads (max): %d\n", max_threads);

  // Case 3
#pragma omp parallel 
  {
    printf("part 3 (parallel)\n");
    printf ("Threads (get 2): %d\n", get_threads);
  }

  // Case 4
  omp_set_num_threads (ordered_threads);
  printf ("Threads (ordered): %d\n", ordered_threads);

  // Case 5  
#pragma omp parallel num_threads(ordered_threads)
  printf("part 4 (parallel num_threads(ordered_threads))\n");

  double end_time = omp_get_wtime();  
  printf("Time: %f\n", end_time-start_time);

  return 0;
}

