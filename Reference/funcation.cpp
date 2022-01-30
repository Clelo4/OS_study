/**
 * @file funcation.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace J {

template <typename T>
T mod(const T& a, const T& b) {
  return a % b;
}
template <typename T>
class add final {
 public:
  add() = default;
  T operator()(const T& a, const T& b) const;
};

template <typename T>
inline T add<T>::operator()(const T& a, const T& b) const {
  return a + b;
}

};  // namespace J

int main() {
  using namespace J;
  using namespace std;
  unordered_map<string, function<int(int, int)>> calc_map;
  calc_map.insert({"mod", mod<int>});
  calc_map.insert({"add", add<int>()});
  cout << calc_map["mod"](13, 2) << endl;
  return 0;
};
