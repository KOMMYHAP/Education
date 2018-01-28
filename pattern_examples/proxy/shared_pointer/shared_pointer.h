// interface for shared, unique, weak, etc pointers.
template <class T>
class pointer_base
{
public:
	typedef T data_t;
private:
	data_t *_data_ptr;
protected:
	/** @param pointer: pointer on existing data
		@brief: copy pointer
	*/
	pointer_base(data_t *pointer) noexcept
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
	virtual 
	pointer_base& operator=(pointer_base const &other) noexcept
	{
		if (this == &other) return *this;
		_data_ptr = other._data_ptr;
		return *this;
	}

	// move c-tor simply moves other's pointer but
	// not removes old data
	virtual 
	pointer_base& operator=(pointer_base &&other) noexcept
	{
		if (this == &other) return *this;
		_data_ptr = other._data_ptr;
		other._data_ptr = nullptr;
		return *this;
	}

	// however, pure d-tor need empty definition
	virtual ~pointer_base() noexcept = 0;

	virtual data_t* operator->() const noexcept
	{
		return data_ptr;
	}

	virtual data_t& operator*() const noexcept
	{
		return *data_ptr;
	}

public:
	inline
	bool is_null() const noexcept
	{
		return _data_ptr == nullptr;
	}

	inline 
	bool operator!() const noexcept
	{
		return is_null();
	}

	inline
	operator bool() const noexcept
	{
		return !is_null();
	}
};

// empty definition for pure d-tor:
pointer_base::~pointer_base() {}

/**
	@brief:
		Every instance created increases counter
		and every instance destructed decrease counter;
*/
template <class T>
class counter
{
public:
	typedef T size_t;
private:
	size_t *_counter_ptr {nullptr};
public:
	counter() noexcept
		: _counter_ptr(new size_t(1)) {}

	counter(counter const &other) noexcept
		: _counter_ptr(other._counter_ptr) 
	{
		++(*_counter_ptr);
	}

	counter(counter &&other) noexcept
		: _counter_ptr(other._counter_ptr)
	{
		other._counter_ptr = nullptr;
	}

	counter& operator=(counter const &other) noexcept
	{
		if (this == &other) return *this;

		// decreases old counter:
		--(*_counter_ptr);
		// changes own counter on others counter:
		_counter_ptr = other._counter_ptr;
		// increases new counter:
		++(*_counter_ptr);

		return *this;
	}

	counter& operator=(counter &&other) noexcept
	{
		if (this == &other) return *this;

		// decreases old counter:
		--(*_counter_ptr);
		// changes own counter on others counter:
		_counter_ptr = other._counter_ptr;
		// remove others counter:
		other._counter_ptr = nullptr;

		return *this;
	}

	~counter()
	{
		if (*_counter_ptr == 0) {
			delete _counter_ptr;
		}
	}

	inline
	size_t use_count() const noexcept
	{
		return *_counter_ptr;
	}
};

template <class T>
class shared_pointer
	: public pointer_base<T>
{
public:
	typedef unsigned int size_t;
private:
	counter<size_t> _counter;
public:
	typedef T data_t;

	shared_pointer()
		: pointer_base()
		, counter() {}

	shared_pointer(data_t *pointer)
		: pointer_base(pointer)
		, counter() {}

	shared_pointer(shared_pointer const &other)
		: pointer_base(static_cast<const &pointer_base>(&other))
		, 
};

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