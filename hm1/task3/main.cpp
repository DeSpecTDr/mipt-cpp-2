#include "include/my_math.hpp"

int main() {
  double x = 1.0;

  AnswerHandler sin_result = my_sin(x);
  AnswerHandler cos_result = my_cos(x);
  AnswerHandler tan_result = my_tan(x);

  sin_result.print("sin");
  cos_result.print("cos");
  tan_result.print("tan");

  return 0;
}
