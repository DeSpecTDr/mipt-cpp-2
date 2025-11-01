#include "../include/ans_handler.hpp"

#include <iomanip>
#include <iostream>

void AnswerHandler::print(const std::string& function_name) const {
  std::cout << std::fixed << std::setprecision(10);
  std::cout << function_name << "(" << x << ") = " << result << std::endl;
}
