#if 0

template <class T>
struct RootType { typedef T type; };

template <class T>
struct RootType<T &> { typedef T type; };

template <class T>
struct RootType<T const &> { typedef T type; };

template <class T>
struct RootType<T &&> { typedef T type; };

#endif
