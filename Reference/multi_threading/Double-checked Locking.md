Double-checked Locking，严格意义上来讲不属于无锁范畴，无论什么时候当临界区中的代码仅仅需要加锁一次，同时当其获取锁的时候必须是线程安全的，此时就可以利用 Double-checked Locking 模式来减少锁竞争和加锁载荷。目前Double-checkedLocking已经广泛应用于单例 (Singleton)模式中。
Double-checked Locking有以下特点：
Double-checked Locking模式是Singleton的多线程版本。 Double-checked Locking模式依旧会使用锁——临界区锁定，不要以为可以避免使用锁。 Double-checked Locking解决的问题是：当多个线程存在访问临界区企图时，保证了临界区只需要访问一次。

以Singleton为例，为了防止多次分配，通常Singleton的实现方式是：
``` Java
Class singleton
{
	singleton* get_instance()
	{ 
		if (instance == 0)
		{
			lock();
			if (instance == 0 )
			{
				instance = new singleton;
			}
			unlock();
		}
		return instance;
	}
}
```
C++11版本：
``` C++
#include <mutex>
#include <optional> // Since C++17

// Singleton.h
class Singleton {
 public:
  static Singleton* GetInstance();
 private:
  Singleton() = default;

  static std::optional<Singleton> s_instance;
  static std::once_flag s_flag;
};

// Singleton.cpp
std::optional<Singleton> Singleton::s_instance;
std::once_flag Singleton::s_flag{};

Singleton* Singleton::GetInstance() {
  std::call_once(Singleton::s_flag,
                 []() { s_instance.emplace(Singleton{}); });
  return &*s_instance;
}
```
非C++11版本：
``` C++
#include <atomic>
#include <mutex>

class Singleton {
 public:
  static Singleton* GetInstance();

 private:
  Singleton() = default;

  static std::atomic<Singleton*> s_instance;
  static std::mutex s_mutex;
};

Singleton* Singleton::GetInstance() {
  Singleton* p = s_instance.load(std::memory_order_acquire);
  if (p == nullptr) { // 1st check
    std::lock_guard<std::mutex> lock(s_mutex);
    p = s_instance.load(std::memory_order_relaxed);
    if (p == nullptr) { // 2nd (double) check
      p = new Singleton();
      s_instance.store(p, std::memory_order_release);
    }
  }
  return p;
}
```
## 参考：https://cloud.tencent.com/developer/article/1682545、https://en.wikipedia.org/wiki/Double-checked_locking