#include <queue_base.h>
#include <mutex>

template <class T>
using queue<T> = queue_base<T, _S_single>;

template <class T>
using queue_async<T> = queue_base<T, _S_mutex>;