/**
 * @brief 尽可能延后变量定义的出现时间
 * @file 26.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-26
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <string>

template <typename T>
void log(const T& t) {
  std::cout << t << std::endl;
}

std::string encryptPassword(const std::string& password) {
  using namespace std;
  // string encrypted;
  if (password.length() < 1024) {
    throw logic_error("password is too short");
  }
  // 使用copy构造函数比使用默认构造函数+copy赋值操作更快
  string encrypted(password);
  return encrypted;
}

int main() { return 0; }
