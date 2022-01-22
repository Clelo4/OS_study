#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
inline void print(const T& vec) {
  for_each(
      vec.cbegin(), vec.cend(),
      [](const typename T::value_type& val) -> void { cout << val << " "; });
  cout << "end" << endl;
}

void fn() {
  string str;
  vector<decltype(str)> vec;
  while (getline(cin, str)) {
    if (str.size() == 0) break;
    vec.push_back(str);
  }
  cout << "Your input is: ";
  print(static_cast<add_const<decltype(vec)>::type>(vec));
  sort(vec.begin(), vec.end(), less<decltype(vec)::value_type>());
  auto unique_end = unique(vec.begin(), vec.end());
  print(vec);
  vec.erase(unique_end, vec.end());
  print(vec);
}

int main() {
  fn();
  return 0;
}
