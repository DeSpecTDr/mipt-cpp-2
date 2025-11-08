#include <cassert>
#include <new>

template <typename T>
class Grid final {
 public:
  using value_type = T;
  using size_type = unsigned;

 private:
  T *data;
  size_type y_size, x_size;

  Grid(T *data, size_type y_size, size_type x_size)
      : data(data), y_size(y_size), x_size(x_size) {}

  // task 1:
  void clear() {
    if (data) {
      for (size_type i = 0; i < y_size * x_size; ++i) {
        data[i].~T();
      }
      ::operator delete(data);  // ыффективность
      data = nullptr;
    }
  }

 public:
  Grid(T const &t) : y_size(1), x_size(1) {
    data = static_cast<T *>(::operator new(sizeof(T)));
    new (data) T(t);
  }

  Grid(size_type y_size, size_type x_size) : y_size(y_size), x_size(x_size) {
    data = static_cast<T *>(::operator new(sizeof(T) * y_size * x_size));
    for (size_type i = 0; i < y_size * x_size; ++i) {
      new (data + i) T();
    }
  }

  Grid(size_type y_size, size_type x_size, T const &t)
      : y_size(y_size), x_size(x_size) {
    data = static_cast<T *>(::operator new(sizeof(T) * y_size * x_size));
    for (size_type i = 0; i < y_size * x_size; ++i) {
      new (data + i) T(t);
    }
  }

  ~Grid() { clear(); }

  Grid(Grid<T> const &other) : y_size(other.y_size), x_size(other.x_size) {
    data = static_cast<T *>(::operator new(sizeof(T) * y_size * x_size));
    for (size_type i = 0; i < y_size * x_size; ++i) {
      new (data + i) T(other.data[i]);
    }
  }

  Grid(Grid<T> &&other) noexcept  // ыффективность
      : data(other.data), y_size(other.y_size), x_size(other.x_size) {
    other.data = nullptr;
    other.y_size = 0;
    other.x_size = 0;
  }

  Grid<T> &operator=(Grid<T> const &other) {
    if (this == &other) return *this;
    clear();
    y_size = other.y_size;
    x_size = other.x_size;
    data = static_cast<T *>(::operator new(sizeof(T) * y_size * x_size));
    for (size_type i = 0; i < y_size * x_size; ++i) {
      new (data + i) T(other.data[i]);
    }
    return *this;
  }

  Grid<T> &operator=(Grid<T> &&other) noexcept {
    if (this == &other) return *this;
    clear();
    data = other.data;
    y_size = other.y_size;
    x_size = other.x_size;
    other.data = nullptr;
    other.y_size = 0;
    other.x_size = 0;
    return *this;
  }

  // task 2:
  class RowProxy {
    T *row_data;
    size_type x_size;

   public:
    RowProxy(T *row_data, size_type x_size)
        : row_data(row_data), x_size(x_size) {}

    T &operator[](size_type x_idx) { return row_data[x_idx]; }
    T operator[](size_type x_idx) const { return row_data[x_idx]; }
  };

  RowProxy operator[](size_type y_idx) {
    return RowProxy(data + y_idx * x_size, x_size);
  }
  RowProxy operator[](size_type y_idx) const {
    return RowProxy(data + y_idx * x_size, x_size);
  }

  // base:
  T operator()(size_type y_idx, size_type x_idx) const {
    return data[y_idx * x_size + x_idx];
  }

  T &operator()(size_type y_idx, size_type x_idx) {
    return data[y_idx * x_size + x_idx];
  }

  Grid<T> &operator=(T const &t) {
    for (auto it = data, end = data + x_size * y_size; it != end; ++it) *it = t;
    return *this;
  }

  size_type get_y_size() const { return y_size; }
  size_type get_x_size() const { return x_size; }
};

int main() {
  Grid<float> g(3, 2, 0.0f);
  assert(3 == g.get_y_size());
  assert(2 == g.get_x_size());

  using gsize_t = Grid<float>::size_type;

  for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
      assert(0.0f == g[y_idx][x_idx]);

  for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
      g[y_idx][x_idx] = 1.0f;

  for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
      assert(1.0f == g(y_idx, x_idx));

  return 0;
}
