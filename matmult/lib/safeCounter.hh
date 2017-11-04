#include <condition_variable>
#include <mutex>

template <typename T>
class threadsafe_counter {
private:
  T c;
  mutable mutex mut;

public:
  threadsafe_counter() {}
  threadsafe_counter(T value) {
    c(value);
  }
  void initialize(T value) {
    lock_guard<mutex> lk(mut);
    c = value;
  }
  void increment() {
    lock_guard<mutex> lk(mut);
    c++;
  }
  T &get() {
    lock_guard<mutex> lk(mut);
    return c;
  }
};
