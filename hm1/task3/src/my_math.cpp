#include "../include/my_math.hpp"

#include <cmath>

AnswerHandler my_sin(double x) {
  AnswerHandler result;
  result.x = x;

  double sum = 0.0;
  double term = x;
  int n = 1;

  while (std::abs(term) > 1e-10) {
    sum += term;
    term *= -x * x / ((2 * n) * (2 * n + 1));
    n++;
  }

  result.result = sum;
  return result;
}

AnswerHandler my_cos(double x) {
  AnswerHandler result;
  result.x = x;

  double sum = 0.0;
  double term = 1.0;
  int n = 1;

  while (std::abs(term) > 1e-10) {
    sum += term;
    term *= -x * x / ((2 * n - 1) * (2 * n));
    n++;
  }

  result.result = sum;
  return result;
}

AnswerHandler my_tan(double x) {
  AnswerHandler result;
  result.x = x;

  AnswerHandler sin_result = my_sin(x);
  AnswerHandler cos_result = my_cos(x);

  if (std::abs(cos_result.result) < 1e-10) {
    result.result = 0.0;
  } else {
    result.result = sin_result.result / cos_result.result;
  }

  return result;
}
