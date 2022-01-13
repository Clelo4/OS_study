/**
 * @file template_for_unique_ptr.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2022-01-13
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */
#include <iostream>
#include <memory>
#include <string>

using namespace std;

template <typename T, typename Deleter>
class My_unique_ptr {
 public:
  My_unique_ptr() : ptr(nullptr) {}
  explicit My_unique_ptr(T *t) : ptr(t) {}
  ~My_unique_ptr() {
    del(ptr);
    ptr = nullptr;
  }
  My_unique_ptr &operator=(nullptr_t t) {
    static_assert(t == nullptr, "operator = nullptr");
    return *this;
  }
  My_unique_ptr(const My_unique_ptr &) = delete;
  My_unique_ptr &operator=(const My_unique_ptr &) = delete;
  operator bool() const;
  T &operator*();
  T *get() { return ptr; }
  void swap(My_unique_ptr &b);

  T *release();
  void reset();
  void reset(T *q);
  void reset(nullptr_t t);

 private:
  T *ptr;
  Deleter del;
};

template <typename T, typename Deleter>
void swap(My_unique_ptr<T, Deleter> &a, My_unique_ptr<T, Deleter> &b) {
  a.swap(b);
}

template <typename T, typename Deleter>
inline My_unique_ptr<T, Deleter>::operator bool() const {
  return ptr == nullptr;
}

template <typename T, typename Deleter>
inline T &My_unique_ptr<T, Deleter>::operator*() {
  return *this;
}

template <typename T, typename Deleter>
inline void My_unique_ptr<T, Deleter>::swap(My_unique_ptr &b) {
  using std::swap;
  swap(*this->ptr, b.ptr);
}

template <typename T, typename Deleter>
inline T *My_unique_ptr<T, Deleter>::release() {
  T *ret = ptr;
  ptr = nullptr;
  return ret;
}

template <typename T, typename Deleter>
inline void My_unique_ptr<T, Deleter>::reset() {
  del(ptr);
  ptr = nullptr;
}

template <typename T, typename Deleter>
inline void My_unique_ptr<T, Deleter>::reset(T *q) {
  decltype(ptr) save = ptr;
  ptr = q;
  del(save);
}

template <typename T, typename Deleter>
inline void My_unique_ptr<T, Deleter>::reset(nullptr_t t) {
  static_assert(t == nullptr, "t must be nullptr");
  decltype(ptr) save = ptr;
  ptr = nullptr;
  del(save);
}

template <typename T>
struct IntDeleter {
  IntDeleter() = default;
  ~IntDeleter() = default;
  void operator()(T *t) {
    cout << "IntDeleter" << endl;
    delete t;
  }
};

int main() {
  My_unique_ptr<int, IntDeleter<int>> t(new int(12));
  My_unique_ptr<int, IntDeleter<int>> t1(new int(13));
  // t1 = t;
  return 0;
}
