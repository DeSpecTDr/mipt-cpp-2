#include <exception>
#include <iostream>
#include <sstream>
#include <string>

// задача 1
class MathException : public std::exception {
 private:
  std::string message;

 public:
  MathException() : message("math error") {}
  MathException(const std::string& msg) : message(msg) {}

  const char* what() const noexcept override { return message.c_str(); }
};

class OverflowError : public MathException {
 public:
  OverflowError() : MathException("overflow error") {}
};

class UnderflowError : public MathException {
 public:
  UnderflowError() : MathException("underflow error") {}
};

int divide(int x, int y) {
  if (y == 0) throw OverflowError();
  return x / y;
}

// задача 2
template <typename T>
struct ControlBlock {
  T* object;
  size_t ref_count;

  ControlBlock(T* obj) : object(obj), ref_count(1) {}

  ~ControlBlock() { delete object; }
};

// скопировано наполовину из smart_pointers.md
template <typename T>
class shared_ptr {
 private:
  T* ptr;
  ControlBlock<T>* control;

 public:
  shared_ptr() : ptr(nullptr), control(nullptr) {}

  explicit shared_ptr(T* p)
      : ptr(p), control(p ? new ControlBlock<T>(p) : nullptr) {}

  shared_ptr(const shared_ptr& other) : ptr(other.ptr), control(other.control) {
    if (control) {
      ++control->ref_count;
    }
  }

  shared_ptr& operator=(const shared_ptr& other) {
    if (this != &other) {
      release();
      ptr = other.ptr;
      control = other.control;
      if (control) {
        ++control->ref_count;
      }
    }
    return *this;
  }

  ~shared_ptr() { release(); }

  T& operator*() const { return *ptr; }

  T* operator->() const { return ptr; }

  T* get() const { return ptr; }

  size_t use_count() const { return control ? control->ref_count : 0; }

  bool unique() const { return use_count() == 1; }

  explicit operator bool() const { return ptr != nullptr; }

  void reset() {
    release();
    ptr = nullptr;
    control = nullptr;
  }

  void reset(T* p) {
    release();
    ptr = p;
    control = p ? new ControlBlock<T>(p) : nullptr;
  }

 private:
  void release() {
    if (control) {
      --control->ref_count;
      if (control->ref_count == 0) {
        delete control;
      }
    }
  }

  template <typename U>
  friend shared_ptr<U> make_shared();

  template <typename U, typename... Args>
  friend shared_ptr<U> make_shared(Args&&... args);
};

template <typename T>
shared_ptr<T> make_shared() {
  return shared_ptr<T>(new T());
}

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
  return shared_ptr<T>(new T(std::forward<Args>(args)...));
}

// задача 3
class bad_from_string : public std::exception {
 private:
  std::string message;

 public:
  bad_from_string() : message("bad_from_string error") {}

  const char* what() const noexcept override { return message.c_str(); }
};

template <class T>
T from_string(std::string const& s) {
  std::istringstream is(s);
  T tmp;
  is >> std::noskipws >> tmp;

  if (is.fail() || !is.eof()) {
    throw bad_from_string();
  }

  return tmp;
}
