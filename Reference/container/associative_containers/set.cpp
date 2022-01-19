/**
 * @file set.cpp
 * @author chengjunjie (chengjunjie@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>

using namespace std;

template <typename _Tp>
struct ccomp {
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

int main() {
  set<int, ccomp<int>> si;
  si.insert(1);
  si.insert(1);
  si.insert(2);
  si.insert(2);
  si.insert(2);
  si.insert(-1);
  auto range = si.equal_range(1);
  assert(*range.first == 1);
  // In set containers, the keys to sort the elements are the values themselves,
  // therefore value_comp and its sibling member function key_comp are
  // equivalent.
  auto kcomp = si.key_comp();
  auto vcomp = si.value_comp();
  cout << kcomp(11, 2) << endl;
  si.erase(si.lower_bound(1), si.upper_bound(1));
  for_each(si.begin(), si.end(),
           [](const int& a) -> void { cout << a << endl; });
  // assert(range.second == si.end());
  return 0;
}
