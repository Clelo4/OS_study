#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<int> vec{1, 2, 3, 4, 5, 1, 0};
  auto bound = partition(vec.begin(), vec.end(),
                         [](const int& a) -> bool { return a % 2; });

  for_each(
      vec.begin(), bound,
      [](const decltype(vec)::value_type& val) -> void { cout << val << " "; });
  cout << endl;
  for_each(bound, vec.end(), [](const decltype(vec)::value_type& val) -> void {
    cout << val << " ";
  });

  return 0;
}
