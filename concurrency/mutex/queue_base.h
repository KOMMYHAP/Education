#include <iterator>

template <class T>
class node
{
public:
	typedef T data_t;
	typedef node<T> node_t;
private:
	data_t *_data_ptr {nullptr};
	node_t *_next_ptr {nullptr};
public:
	node() = default;

	node(data_t *data_ptr, node_t *next_node_ptr)
		: _next_ptr(next_node_ptr)
		, _data_ptr(data_ptr) {}

	node(node const &other) 
		: _data_ptr(nullptr)
		, _next_ptr(nullptr)
	{
		if (other._data_ptr != nullptr) {
			_data_ptr = new data_t(*other._data_ptr);
		}

		if (other._next_ptr != nullptr) {
			_next_ptr = node_t(*_next_ptr);
		}
	}

	node<T>& operator=(node const &other)
	{
		if (this == &other) return *this;
		
		delete _data_ptr;
		if (other._data_ptr != nullptr) {
			_data_ptr = new data_t(*other._data_ptr);
		} else {
			_data_ptr = nullptr;
		}

		delete _next_ptr;
		if (other._next_ptr != nullptr) {
			_next_ptr = node_t(*_next_ptr);
		} else {
			_next_ptr = nullptr;
		}

		return *this;
	}

	node(node &&other) 
		: _data_ptr(other._data_ptr)
		, _next_ptr(other._next_ptr)
	{
		other._data_ptr = nullptr;
		other._next_ptr = nullptr;
	}

	node<T>& operator=(node &&other)
	{
		if (this == &other) return *this;

		delete _data_ptr;
		_data_ptr = other._data_ptr;
		other._data_ptr = nullptr;

		delete _next_ptr;
		_next_ptr = other._next_ptr;
		other._next_ptr = nullptr;

		return *this;
	}

	~node()
	{
		delete _data_ptr;
		delete _next_ptr;
	}

	data_t *data() const noexcept
	{
		return _data_ptr;
	}

	node_t *next() const noexcept
	{
		return _next_ptr;
	}
};

template <class T>
class queue_base
{
public:
	typedef node<T> node_t;
	typedef unsigned int size_t;
	typedef T data_t;
	//typedef queue_base<T, policy> self;
	typedef iterator<T> queue_iterator;

	friend class iterator<T>;
protected:
	queue_base()
		: _first(nullptr) {}

	queue_base(queue_base const &other) throw(std::bad_alloc)
		: _first(nullptr)
	{
		if (other._first != nullptr) {
			_first = new node_t(*other._first);
		}
	}

	self& operator=(queue_base const &other) throw(std::bad_alloc)
	{
		if (this == &other) return *this;

		delete _first;
		if (other._first != nullptr) {
			_first = new node_t(*other._first);
		} else {
			_first = nullptr;
		}

		return *this;
	}

	queue_base(queue_base &&other)
		: _first(other._first)
	{
		other._first = nullptr;
	}

	queue_base& operator=(queue_base &&other)
	{
		if (this == &other) return *this;
		_first = other._first;
		other._first = nullptr;
		return *this;
	}

	virtual ~queue_base()
	{
		delete _first;
	}

public:
	virtual void push(data_t const &data) throw(std::bad_alloc)
	{
		queue_iterator cur = begin(), next = begin();

		if (size() == 0) {
			_first = new node_t(data);
			return;
		}

		++next;
		while (next != end()) {
			++cur;
			++next;
		}
		cur->next() = new node_t(data);
	}

	virtual queue_iterator pop() throw(std::bad_alloc)
	{
		queue_iterator cur = begin(), next = begin();
		if (size() == 0) {
			return end();
		}

		++next;
		while (next != end()) {
			++cur;
			++next;
		}
		return cur;
	}

	queue_iterator begin() const noexcept
	{
		return queue_iterator(_first);
	}

	queue_iterator end() const noexcept
	{
		return nullptr;
	}

	queue_iterator::difference_type
	size() const noexcept
	{
		return std::distance(begin(), end());
	}

private:
	node_t *_first;
};

template <class T>
using queue<T> = queue_base<T, _S_single>;

template <class T>
using queue_async<T> = queue_base<T, _S_mutex>;

template <class T>
struct iterator
{
	typedef iterator<T> self_t;
	typedef node<T> node_t;

	typedef std::ptrdiff_t difference_type;
	typedef typename std::remove_cv<T>::type value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef std::input_iterator_tag iterator_category;

	iterator(node_t *node_ptr = nullptr)
		: _node_ptr(node_ptr) { }

	reference operator*() const noexcept
    { 
    	return *_node_ptr; 
    }

  	pointer operator->() const noexcept
	{ 
		return _node_ptr; 
	}

	self_t& operator++() noexcept
	{	
		_node_ptr = _node_ptr->next;
		return *this;
    }

	self_t operator++(int) noexcept
	{
		self_t tmp = *this;
		_node_ptr = _node_ptr->next();
		return tmp;
	}

	bool operator==(const self_t &other) const noexcept
	{ 
		return _node_ptr == other._node_ptr;
	}

	bool operator!=(const self_t &other) const noexcept
	{ 
		return _node_ptr != other._node_ptr;
	}

	node_t *_node_ptr;
};