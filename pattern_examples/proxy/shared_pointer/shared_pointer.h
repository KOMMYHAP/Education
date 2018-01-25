// interface for shared, unique, weak, etc pointers.
template <class T>
class pointer_base
{
public:
	typedef T data_t;
private:
	data_t *_data_ptr;
protected:
	pointer_base(data_t pointer) noexcept
		: _data_ptr(pointer) {}

	// default c-tor initializes pointer with nullptr:
	pointer_base() : _data_ptr(nullptr) {} noexcept

	// copy c-tor copies other's pointer 
	pointer_base(pointer_base const &other) noexcept
		: _data_ptr(other._data_ptr) {}

	// move c-tor moves other's pointer
	pointer_base(pointer_base &&other) noexcept
		: _data_ptr(other._data_ptr) 
	{
		other._data_ptr = nullptr;
	}

	// copy c-tor simply copies other's pointer but
	// not removes old data
	pointer_base& operator=(pointer_base const &other) noexcept
	{
		if (this == &other) return *this;
		_data_ptr = other._data_ptr;
		return *this;
	}

	// move c-tor simply moves other's pointer but
	// not removes old data
	pointer_base& operator=(pointer_base &&other) noexcept
	{
		if (this == &other) return *this;
		_data_ptr = other._data_ptr;
		other._data_ptr = nullptr;
		return *this;
	}

	~pointer_base() noexcept
	{}

	data_t* operator->() const noexcept
	{
		return data_ptr;
	}

	data_t& operator*() const noexcept
	{
		return *data_ptr;
	}

	bool is_empty() const noexcept
	{
		return _data_ptr == nullptr;
	}
}
/*
class shared_counter
{
public:
	using size_t = unsigned int;
private:
	size_t _counter {0};
public:
	shared_counter() noexcept
		: _counter(1) {}

	shared_counter(shared_counter const &) = delete;
	shared_counter& operator=(shared_counter const &) = delete;

	shared_counter(shared_counter &&other) noexcept
		: _counter(other._counter)
	{
		other._counter = 0;
	}

	shared_counter& operator=(shared_counter &&other) noexcept
	{
		if (this == &other) return *this;

		_counter = other._counter;
		other._counter = 0;

		return *this;		
	}

	~shared_counter() = default;

	inline
	size_t count() const noexcept
	{
		return _counter;
	}

	inline
	bool is_zero() const noexcept
	{
		return _counter == 0;
	}

	inline
	void inc() noexcept
	{
		++_counter;
	}

	inline
	void dec() noexcept
	{
		--_counter;
	}
};
template <class T>
class shared_pointer
{
public:
	using data_t = T;
private:
	data_t *_data_ptr {nullptr};
	mutable shared_counter *_counter_ptr {nullptr};

//TODO: need read about "State" pattern...
	void safe_remove() noexcept
	{
		// decrease counter:
		if (!_counter_ptr->is_empty()) {
			_counter_ptr->dec();
		}
		// if counter equals zero, than delete data;
		if (_counter_ptr->is_empty()) {
			delete _data_ptr;
			_data_ptr = nullptr;
		}
	}

	void remove_old_and_copy_new(shared_pointer const &other) noexcept
	{
		safe_remove();
		if (!other._counter_ptr->is_empty()) {
			// coping data pointer:
			_data_ptr = other._data_ptr;
			// changing counters:
			_counter_ptr = other._counter_ptr;
		}
	}

	void reset() noexcept
	{
		_data_ptr = nullptr;
		_counter_ptr = nullptr;
	}
public:
	shared_pointer()
		: _data_ptr(nullptr)
		, _counter_ptr(new shared_counter) {}

	explicit
	shared_pointer(data_t *data_ptr)
		: _data_ptr(data_ptr)
		, _counter_ptr(new shared_counter) {}

	shared_pointer(shared_pointer const &other) 
		: _data_ptr(other._data_ptr)
		, _counter_ptr(other._counter_ptr)
	{
		if (_data_ptr != nullptr) {
			_counter_ptr->inc();
		}
	}

	shared_pointer(shared_pointer &&other)
		: _data_ptr(other._data_ptr)
		, _counter(other._counter)
	{
		other.reset();
	}

	shared_pointer<T>& operator=(shared_pointer const &other)
	{
		if (this == &other) return *this;

		remove_old_and_copy_new(other);

		return *this;
	}

	shared_pointer<T>& operator=(shared_pointer &&other)
	{
		if (this == &other) return *this;

		remove_old_and_copy_new(other);
		other.reset();

		return *this;
	}

	~shared_pointer()
	{
		safe_remove();
	}

	T* operator->() const noexcept
	{
		return _data_ptr;
	}

	T& operator*() const noexcept
	{
		return *_data_ptr;
	}

	size_t use_count() const noexcept
	{
		return _counter;
	}
};
*/