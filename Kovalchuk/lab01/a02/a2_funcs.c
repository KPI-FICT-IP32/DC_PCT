#include <math.h>


double sin_series_term(int term_number, double value) {
  double product = term_number % 2 ? (-1) : 1;
  for (int k = 1; k <= 2*term_number + 1; k++) {
    product *= value / k;
  }
  return product;
}


double cos_series_term(int term_number, double value) {
  double product = term_number % 2 ? (-1) : 1;
  for (int k = 1; k <= 2*term_number; k++) {
    product *= value / k;
  }
  return product;
}


double sinh_series_term(int term_number, double value) {
  double product = 1;
  for (int k = 1; k <= 2*term_number + 1; k++) {
    product *= value / k;
  }
  return product;
}


double cosh_series_term(int term_number, double value) {
  double product = 1;
  for (int k = 1; k <= 2*term_number; k++) {
    product *= value / k;
  }
  return product;
}
