#include <stdio.h>
#include <omp.h>

int main (int argc, char *argv[]) {
  int max = 10000; 
  int sum = 0;
  #pragma omp parallel for
    for (int i = 1; i <= max; i++)
      sum = sum + i;
  printf ("%d\n", sum);
  return 0;
}
