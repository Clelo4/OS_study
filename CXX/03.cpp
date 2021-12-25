/**
 *
 * @brief Effective C++ 03 尽可能使用const
 * @file 03.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-23
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <string>

class TextBlock {
 private:
  std::string text;
  mutable std::size_t length;

 public:
  TextBlock(const std::string& str) : text(str) {}
  // constness成员函数可以重载
  const char& operator[](std::size_t position) const {
    length = text.length();
    return text[position];
  }
  char& operator[](std::size_t position) { return text[position]; }
  /**
   * Compile error: unctions that differ only in their return type cannot
   * beoverloaded
   */
  // const char& operator[](std::size_t position) { return text[position]; }

  const std::size_t& fn() const {
    // more code here
    return length;
  }
  // 警告：不要在const成员函数中调用非const成员函数
  std::size_t& fn() {
    return const_cast<std::size_t&>(
        (static_cast<const TextBlock*>(this))->fn());
  }
};

int main() {
  TextBlock tb("Hello");
  const TextBlock ctb("Hello");
  std::cout << tb[0] << std::endl;
  std::cout << ctb[0] << std::endl;
  return 0;
}
