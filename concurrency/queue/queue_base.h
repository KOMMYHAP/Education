#include <mutex>
#include <iterator>

template <class T>
class node
{
public:
	typedef T data_t;
	typedef node<T> node_t;

	typedef std::shared_ptr<data_t> data_ptr_t;
	typedef std::shared_ptr<node_t> node_ptr_t;
private:
	data_ptr_t _data_ptr {nullptr};
	node_ptr_t _next_ptr {nullptr};
public:
	node() = default;

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

enum class Lock_policy {
	_S_single,
	_S_mutex
};

// forward declaration for iterator
template <class T> class iterator<T>;

template <class T, Lock_policy policy>
struct queue_access_base
{
	typedef node<T> node_t;
	typedef iterator<T> queue_iterator;

	virtual void push(T const &) throw(std::bad_alloc)
	{
		auto const *__this = static_cast<queue_base<T, policy> const *>(this);
		queue_iterator cur = __this->begin(), next = __this->begin();

		if (__this->size() == 0) {
			_first = new node_t(data);
			return;
		}

		++next;
		while (next != __this->end()) {
			++cur;
			++next;
		}
		cur->next() = new node_t(data);
	}

	virtual queue_iterator pop() throw(std::bad_alloc)
	{
		auto const *__this = static_cast<queue_base<T, policy> const *>(this);
		queue_iterator cur = __this->begin(), next = __this->begin();
		if (__this->size() == 0) {
			return __this->end();
		}

		++next;
		while (next != __this->end()) {
			++cur;
			++next;
		}
		return cur;
	}
}

// forward declaration for queue_access;
template <class T, Lock_policy policy>
struct queue_access;

template <class T, _S_single>
struct queue_access 
	: public queue_access_base<T, _S_single> {};

template <class T, _S_mutex>
struct queue_access 
	: public queue_access_base<T, _S_mutex>
	, public std::mutex
{
public:
	typedef T data_t;

	void push(data_t const &data) override throw(std::bad_alloc)
	{
		std::lock_guard<std::mutex> guard(static_cast<std::mutex &>(*this));
		static_cast<queue_access_base<T>*>(this)->push(data);
	}

	virtual pop() override throw(std::bad_alloc)
	{
		std::lock_guard<std::mutex> guard(static_cast<std::mutex &>(*this));
		return static_cast<queue_access_base<T>*>(this)->pop();
	}
};

template <class T, Lock_policy policy>
class queue_base : public queue_access<T, policy>
{
public:
	typedef unsigned int size_t;
	typedef T data_t;
	//typedef queue_base<T, policy> self;
	typedef iterator<T> queue_iterator;

	friend class iterator<T>;

	queue_base()
		: _first(nullptr) {}

	queue_base(queue_base const &other) throw(std::bad_alloc)
		: _first(nullptr)
	{
		try {
			for (auto const &data : other) {
				push(data);
			}
		} catch(std::bad_alloc const &err) {
			std::cerr << err.what() << std::endl;	
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

	queue_iterator
	get(size_t i) const throw(std::out_of_range)
	{	
		if (i >= size()) throw std::out_of_range();

		queue_iterator result = begin();
		while (i-- > 0) {
			++result;
		}

		return result;
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
		_node_ptr = _node_ptr->next();
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