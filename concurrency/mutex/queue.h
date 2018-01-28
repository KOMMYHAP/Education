#include <queue_base.h>
#include <mutex>

enum class Policy {
	_S_single,
	_S_mutex
};

template <class T, Policy policy>
class queue : private queue_base<T>
{
public:
	typedef queue<T> self;

	queue()
		: queue_base() {}

	queue(queue const &other)
		: queue_base(static_cast<queue_base const &>(other)) {}

	self& operator=(queue const &other) 
	{
		if (this == &other) return *this;

		this->operator=(static_cast<queue_base const &>(other));

		return *this;
	}

	queue(queue &&other)
		: queue_base(static_cast<queue&&>(other)) {}

	queue& operator=(queue &&other)
	{
		if (this == &other)	return *this;

		this->operator=(static_cast<queue_base&&>(other));

		return *this;
	}

	void push(data_t const &) override throw(std::bad_alloc);

	queue_iterator pop() override throw(std::bad_alloc);
};