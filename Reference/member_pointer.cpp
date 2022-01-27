/**
 * @file member_pointer.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <cassert>
#include <functional>
#include <string>
#include <type_traits>

using namespace std;

namespace J {

class Screen {
 public:
  typedef std::string::size_type pos;
  using Action = char (Screen::*)(Screen::pos, Screen::pos) const;
  using Direct = void (Screen::*)();

  char get_cursor() const { return contents[cursor]; }
  char get() const;
  char get(pos ht, pos wd) const { return 'a'; }

  static const std::string Screen::*const_data() { return &Screen::contents; }
  static std::string Screen::*data() { return &Screen::contents; }

  std::string contents;

  enum class MOVE_ENUM : uint8_t {
    up = 0,
    right = 1,
    down = 3,
    left = 4,
  };

  Screen& move(MOVE_ENUM direct) {
    (this->*Menu[static_cast<int>(direct)])();
    return *this;
  };

 private:
  pos cursor;
  pos height, width;
  Screen* backup = nullptr;  // 类内初始化
  static Direct Menu[4];
  void up(){};
  void down(){};
  void left(){};
  void right(){};
};
Screen::Direct Screen::Menu[4] = {&Screen::up, &Screen::right, &Screen::down,
                                  &Screen::left};

};  // namespace J

int main() {
  using namespace J;
  const std::string Screen::*constPData = &Screen::contents;
  auto pData = &Screen::contents;
  constPData = Screen::const_data();
  Screen s1;
  assert(&(s1.*pData) == &(s1.contents));
  s1.*pData = 12;
  printf("%p\n", &(s1.*pData));
  printf("%d\n",
         is_const<remove_reference<decltype(s1.*constPData)>::type>::value);

  Screen::Action fn = &Screen::get;
  // Action fn = Screen::get;  // error
  (s1.*fn)(1, 2);
  return 0;
}
