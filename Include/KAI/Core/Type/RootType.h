template <class T>
struct RootType { typedef T type; }

template <T>
struct RootType<T &> { typedef T type; }

template <T>
struct RootType<T const &> { typedef T type; }

template <T>
struct RootType<T &&> { typedef T type; }

