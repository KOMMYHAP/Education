#include <queue_base.h>

template <class T>
using queue<T> = queue_base<T, _S_single>;

template <class T>
using queue_async<T> = queue_base<T, _S_mutex>;