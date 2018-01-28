#include <iostream>
#include <utility>

class Messanger
{
private:
	std::string _buffer;
	std::string _name;

	void print() const noexcept
	{
		std::cout << _buffer << "\n";
	}

	void domain_print() noexcept
	{
		if (_name.length() != 0) {
			_buffer += "[";
			_buffer += _name;
			_buffer += "]";
		} else {
			_buffer += "[\n";
		}
	}

public:
	Messanger() noexcept
		: _buffer()
		, _name()
	{
		domain_print();
	}

	Messanger(std::string msg) noexcept
		: _buffer() 
		, _name(std::move(msg))
	{
		domain_print();
		add(" started.\n");
	}

	Messanger(Messanger const &) = delete;
	Messanger& operator=(Messanger const &) = delete;

	Messanger(Messanger &&) = delete;
	Messanger& operator=(Messanger &&) = delete;

	~Messanger() noexcept
	{
		domain_print();
		add(" finished.\n");
		print();
	}

	void add(std::string const &msg) noexcept
	{
		try {
			_buffer += msg;
		} catch(std::bad_alloc const &err) {
			std::cerr << err.what() << std::endl;
		}
	}
};

template <class T>
class Object
{
public:
	typedef T data_t;

	Object() 
		: _data_ptr(nullptr) 
	{
		Messanger log {"Default c-tor"};
	}

	Object(Object const &other)
		: _data_ptr(nullptr) 
	{
		Messanger log{"Copy c-tor"};

		if (other._data_ptr != nullptr) {
			log.add("Coping: new data;\n");
			_data_ptr = new data_t(*other._data_ptr);
		} else {
			log.add("Coping: nothing;\n");
		}
	}

	Object& operator=(Object const &other)
	{
		Messanger log{"Assignment operator"};

		if (this == &other) {
			log.add("Warning: equality objects;\n");
			return *this;
		}

		if (_data_ptr != nullptr) {
			log.add("Deleting: old data;\n");
			delete _data_ptr;
			_data_ptr = nullptr;
		} 

		if (other._data_ptr != nullptr) {
			log.add("Coping: new data;\n");
			try {
				_data_ptr = new data_t(*other._data_ptr);
			} catch(std::bad_alloc const &err) {
				log.add("Error: ");
				log.add(err.what());
				log.add("\n");
			}
		} else { // if (other._data_ptr == nullptr)
			log.add("Coping: nothing;\n");
		}

		return *this;
	}

	Object(Object &&other)
		: _data_ptr(other._data_ptr) 
	{
		Messanger log {"Move c-tor"};
		other._data_ptr = nullptr;
	}

	Object& operator=(Object &&other)
	{
		Messanger log {"Move operator"};

		if (this == &other) {
			log.add("Warning: equality objects;\n");
			return *this;
		}

		if (_data_ptr != nullptr) {
			log.add("Deleting: old data;\n");
			delete _data_ptr;
			_data_ptr = nullptr;
		}
		if (other._data_ptr != nullptr) {
			log.add("Moving: moving data pointer;\n");
			_data_ptr = other._data_ptr;
			other._data_ptr = nullptr;
		}

		return *this;
	}

private:
	data_t *_data_ptr {nullptr};
};

void test_copy()
{
	Messanger log {"TEST COPY"};

	log.add("Creating tested object;\n");
	Object<int> obj{}; // default c-tor

	log.add("Testing: copy c-tor;\n");
	Object<int> other {obj}; // copy c-tor

	log.add("Testing: assignment operator;\n");
	obj = other; // assignment operator

	log.add("Checking error: copy equal object;\n");
	obj = obj;
}

void test_move()
{
	Messanger log {"TEST MOVE"};

	log.add("Creating tested object;\n");
	Object<int> obj{};

	log.add("Testing: move c-tor;\n");
	Object<int> other {std::move(obj)};

	log.add("Testing: move operator;\n");
	obj = std::move(other);

	log.add("Checking error: move equal object;\n");
	obj = std::move(obj);
}

int main()
{
	test_copy();
	test_move();
}