/**
 * @brief 少做转型动作
 * @file 27.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-26
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <string>

class Window {
 public:
  explicit Window(int s) : size(s), count(0) {}
  virtual int onResize() {
    ++count;
    ++size;
    return count;
  }

 protected:
  int size;
  int count;
};

class SpecialWindow : public Window {
 public:
  SpecialWindow(int s) : Window(s) {}
  virtual int onResize() {
    // 愚蠢的用法：这里调用的是*this对象的base class部分的一个临时副本
    // static_cast<Window>(*this).onResize();
    // 正确的用法：这里调用的才是*this对象的base class部分
    Window::onResize();
    std::cout << Window::size << std::endl;
    std::cout << Window::count << std::endl;
    return count;
  }
};

int main() {
  double d1;
  int a = (int)d1;  // C风格转型
  int b = int(d1);  // 函数式风格转型
  SpecialWindow s(12);
  std::cout << s.onResize() << std::endl;
  Window *ptr = &s;
  // 安全向下转型（dynamic_cast效率低下，应避免使用）
  dynamic_cast<SpecialWindow *>(ptr)->onResize();
  return 0;
}
