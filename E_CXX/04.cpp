/**
 *
 * @brief Effective C++ 04 确定对象在使用前已经初始化
 * @file 04.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-23
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <list>
#include <string>

class PhoneNumber {};
class ABEntry {
 public:
  ABEntry(const std::string& name, const std::string& address,
          const std::list<PhoneNumber>& phones)
      : length(0) {
    // 下面都是赋值，不是初始化
    theName = name;
    theAddress = address;
    thePhones = phones;
    numTimesConsulted = 0;
    /**
     * cannot assign to non-static data member 'length' with const-qualified
     * type 'const int'
     */
    // length = 0;
  }
  // member initialization list
  ABEntry(const std::string& address, const std::list<PhoneNumber>& phones)
      : theAddress(address),
        thePhones(phones),
        numTimesConsulted(0),
        length(0) {}

 private:
  //  class的成员总是以其声明的次序被初始化，即使他们在member initialization
  //  list中以不同的次序出现（很不幸这是合法的）
  std::string theName;
  std::string theAddress;
  std::list<PhoneNumber> thePhones;
  const int length;
  // 手动初始化built-in对象，因为C++不一定保证初始化它们
  int numTimesConsulted;
};

int main() { return 0; }
