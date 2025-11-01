#ifndef ANS_HANDLER_HPP
#define ANS_HANDLER_HPP

#include <string>

struct AnswerHandler {
  double x;
  double result;
  void print(const std::string& function_name) const;
};

#endif
