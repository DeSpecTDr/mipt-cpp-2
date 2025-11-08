#include <cassert>
#include <new>

template <typename T, unsigned D>
class Grid {
 public:
  using value_type = T;
  using size_type = unsigned;

  template <typename TT, unsigned DD>
  friend class Grid;

 private:
  T* data;
  size_type dims[D];
  size_type total_size;

  size_type get_index(size_type const* indices) const {
    auto index = indices[0];
    for (size_type i = 1; i < D; ++i) {
      index = index * dims[i] + indices[i];
    }
    return index;
  }

  void clear() {
    if (data) {
      for (size_type i = 0; i < total_size; ++i) {
        data[i].~T();
      }
      ::operator delete(data);
      data = nullptr;
    }
  }

 public:
  template <typename... Args>
  Grid(Args... args) {
    size_type temp[] = {static_cast<size_type>(args)...};
    for (size_type i = 0; i < D; ++i) {
      dims[i] = temp[i];
    }
    total_size = 1;
    for (size_type i = 0; i < D; ++i) {
      total_size *= dims[i];
    }
    data = static_cast<T*>(::operator new(sizeof(T) * total_size));
    if constexpr (sizeof...(Args) == D) {
      for (size_type i = 0; i < total_size; ++i) {
        new (data + i) T();
      }
    } else if constexpr (sizeof...(Args) == D + 1) {
      T fill_value = static_cast<T>(temp[D]);
      for (size_type i = 0; i < total_size; ++i) {
        new (data + i) T(fill_value);
      }
    }
  }

  Grid() : data(nullptr), dims{}, total_size(0) {}

  ~Grid() { clear(); }

  Grid(Grid const& other) : total_size(other.total_size) {
    for (size_type i = 0; i < D; ++i) dims[i] = other.dims[i];
    data = static_cast<T*>(::operator new(sizeof(T) * total_size));
    for (size_type i = 0; i < total_size; ++i) {
      new (data + i) T(other.data[i]);
    }
  }

  Grid(Grid&& other) noexcept : data(other.data), total_size(other.total_size) {
    for (size_type i = 0; i < D; ++i) dims[i] = other.dims[i];
    other.data = nullptr;
    other.total_size = 0;
    for (size_type i = 0; i < D; ++i) other.dims[i] = 0;
  }

  Grid& operator=(Grid const& other) {
    if (this == &other) return *this;
    clear();
    for (size_type i = 0; i < D; ++i) dims[i] = other.dims[i];
    total_size = other.total_size;
    data = static_cast<T*>(::operator new(sizeof(T) * total_size));
    for (size_type i = 0; i < total_size; ++i) {
      new (data + i) T(other.data[i]);
    }
    return *this;
  }

  Grid& operator=(Grid&& other) noexcept {
    if (this == &other) return *this;
    clear();
    for (size_type i = 0; i < D; ++i) dims[i] = other.dims[i];
    data = other.data;
    total_size = other.total_size;
    other.data = nullptr;
    other.total_size = 0;
    for (size_type i = 0; i < D; ++i) other.dims[i] = 0;
    return *this;
  }

  template <typename... Args>
  T operator()(Args... args) const {
    size_type indices[] = {static_cast<size_type>(args)...};
    return data[get_index(indices)];
  }

  template <typename... Args>
  T& operator()(Args... args) {
    size_type indices[] = {static_cast<size_type>(args)...};
    return data[get_index(indices)];
  }

  Grid<T, D - 1> operator[](size_type idx) { return slice(idx); }

  Grid<T, D - 1> operator[](size_type idx) const { return slice(idx); }

 private:
  Grid<T, D - 1> slice(size_type idx) const {
    size_type slice_size = 1;
    for (size_type i = 1; i < D; ++i) slice_size *= dims[i];
    size_type offset = idx * slice_size;
    Grid<T, D - 1> result;
    for (size_type i = 0; i < D - 1; ++i) result.dims[i] = dims[i + 1];
    result.total_size = slice_size;
    result.data = static_cast<T*>(::operator new(sizeof(T) * slice_size));
    for (size_type i = 0; i < slice_size; ++i) {
      new (result.data + i) T(data[offset + i]);
    }
    return result;
  }
};

template <typename T>
class Grid<T, 1> {
 public:
  using value_type = T;
  using size_type = unsigned;

  template <typename TT, unsigned DD>
  friend class Grid;

 private:
  T* data;
  size_type dims[1];
  size_type total_size;

  void clear() {
    if (data) {
      for (size_type i = 0; i < total_size; ++i) {
        data[i].~T();
      }
      ::operator delete(data);
      data = nullptr;
    }
  }

 public:
  Grid() : data(nullptr), dims{0}, total_size(0) {}

  Grid(size_type size) : dims{size}, total_size(size) {
    data = static_cast<T*>(::operator new(sizeof(T) * total_size));
    for (size_type i = 0; i < total_size; ++i) {
      new (data + i) T();
    }
  }

  Grid(size_type size, T const& t) : dims{size}, total_size(size) {
    data = static_cast<T*>(::operator new(sizeof(T) * total_size));
    for (size_type i = 0; i < total_size; ++i) {
      new (data + i) T(t);
    }
  }

  ~Grid() { clear(); }

  Grid(Grid const& other) : dims{other.dims[0]}, total_size(other.total_size) {
    data = static_cast<T*>(::operator new(sizeof(T) * total_size));
    for (size_type i = 0; i < total_size; ++i) {
      new (data + i) T(other.data[i]);
    }
  }

  Grid(Grid&& other) noexcept
      : data(other.data), dims{other.dims[0]}, total_size(other.total_size) {
    other.data = nullptr;
    other.total_size = 0;
    other.dims[0] = 0;
  }

  Grid& operator=(Grid const& other) {
    if (this == &other) return *this;
    clear();
    dims[0] = other.dims[0];
    total_size = other.total_size;
    data = static_cast<T*>(::operator new(sizeof(T) * total_size));
    for (size_type i = 0; i < total_size; ++i) {
      new (data + i) T(other.data[i]);
    }
    return *this;
  }

  Grid& operator=(Grid&& other) noexcept {
    if (this == &other) return *this;
    clear();
    data = other.data;
    dims[0] = other.dims[0];
    total_size = other.total_size;
    other.data = nullptr;
    other.total_size = 0;
    other.dims[0] = 0;
    return *this;
  }

  T operator()(size_type idx) const { return data[idx]; }

  T& operator()(size_type idx) { return data[idx]; }

  T& operator[](size_type idx) { return data[idx]; }

  T operator[](size_type idx) const { return data[idx]; }
};

int main() {
  Grid<float, 3> const g3(2, 3, 4, 1.0f);
  assert(1.0f == g3(1, 1, 1));

  Grid<float, 2> g2(2, 5, 2.0f);
  assert(2.0f == g2(1, 1));

  g2 = g3[1];
  assert(1.0f == g2(1, 1));

  return 0;
}
