// Refrence: https://isocpp.org/wiki/faq/templates#template-proxies

#ifndef TEMPLATE_PROXY_H_
#define TEMPLATE_PROXY_H_

#include <list>
#include <vector>
template <typename T>
struct wrap_vector {
  typedef std::vector<T> type;
};
template <typename T>
struct wrap_list {
  typedef std::list<T> type;
};
template <typename T, template <typename> class C>
struct AA {
  typename C<T>::type data;  // trick to use a proxy
};

void test_template_proxy() {
  AA<int, wrap_vector> avi;
  AA<double, wrap_list> adl;
}

#endif  // TEMPLATE_PROXY_H_