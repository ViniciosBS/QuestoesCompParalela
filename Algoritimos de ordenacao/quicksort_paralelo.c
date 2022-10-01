#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int compare (const char *str1, const char *str2) {
  int len1 = strlen (str1);
  int len2 = strlen (str2);
  if (len1 == len2) return strcmp (str1, str2);
  return len1 - len2;
}

void par_qsort (char **data, int lo, int hi,
                int (*compare)(const char *, const char*)) {
  if (lo > hi) return;
  int l = lo;
  int h = hi;
  char *p = data[(hi + lo) / 2];
  while (l <= h) {
    while (compare (data[l], p) < 0) l++;
    while (compare (data[h], p) > 0) h--;
    if (l <= h) {
      char *tmp = data[l]; data[l] = data[h]; data[h] = tmp;
      l++; h--;
    }
  }
  #pragma omp task final(h - lo < 1000)
    par_qsort (data, lo, h, compare);
  #pragma omp task final(hi - l < 1000)
    par_qsort (data, l, hi, compare);
}

int main (int argc, char *argv[]) {
  int seed = 0;
  int num_strings = 100;
  char **strings;
  srandom (seed);
  strings = (char**)malloc (num_strings * sizeof (char*));
  for (int s = 0; s < num_strings; s++) {
    int len = random() % 64;
    strings[s] = (char*)malloc ((len + 1) * sizeof (char));
    for (int c = 0; c < len; c++)
      strings[s][c] = 'A' + random() % 26;
    strings[s][len] = '\0';
  }
  #pragma omp parallel
    #pragma omp single
      par_qsort (strings, 0, num_strings - 1, compare);
  return 0;
}
