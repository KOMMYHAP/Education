#include <cassert>
#include <functional> /* std::ref */

#include "queue.h"

class error
{
public:
	error(char const *msg) noexcept
		: _msg()
	{
		add(msg);
	}

	~error() = default;

	void add(std::string const &msg) noexcept
	{
		try {
			_msg += msg;
		} catch(std::bad_alloc const &err) {
			std::cerr << "[error::add(\""
				<< (msg.length() < 10 ? msg : msg.substr(0, 7) + std::string("..."))
				<< "\")]: " << err.what() << std::endl;
		}
	}

	// Returns true if error happened, false otherwise
	operator bool() const noexcept
	{
		return _msg.length() != 0;
	}

	std::string const & what() const noexcept 
	{
		return _msg;
	}
private:
	std::string _msg;
};

typedef std::vector<error> errors_t;

template <Lock_policy lp>
errors_t test_push() 
{
	throw std::runtime_error();
}

template<>
errors_t test_push<_S_single>()
{
	errors_t errors{};
	// sequentially adding elements from 0 to %amount:
	auto pushing = [](queue<element_t> &q, auto amount) {
		for (decltype(amount) i = 0; i < amount; ++i) {
			q.push(static_cast<element_t>(i));
		}
	};

	queue<element_t> q {};

	auto const amount = 1000;
	typedef decltype(amount) size_t;
	pushing(q, amount);

	for (size_t i = 0; i < amount; ++i) {
		auto it = q.get(i);
		if (*it != i) {
			errors.emplace_back("[test_push]: error in typically cycle");
		}
	}

	size_t i = 0;
	for (auto const &elem : q) {
		if (i != elem) {
			errors.emplace_back("[test_push]: error in range-base cycle");
		}
		++i;
	}

	return false;
}

template <>
bool test_push<_S_mutex>()
{
	typedef int element_t;
	auto pushing = [](queue<element_t> &q, auto amount) {
		for (decltype(amount) i = 0; i < amount; ++i) {
			q.push(static_cast<element_t>(i));
		}
	};

	queue<element_t> q {};
	// <WTF>
	std::thread t1(pushing, std::ref(q), 100);
	std::thread t2(pushing, std::ref(q), 100);
	// </WTF>

	t1.join();
	t2.join();

	return false;
}

bool test()
{
	bool success = true;

	auto errors = test_push<_S_single>();

	std::size_t const max_output_amount = 10;
	bool so_much = errors.size() > max_output_amount;
	std::size_t to = so_much ? errors.size() : max_output_amount;

	if (so_much) {
		std::cerr << "More than " << max_output_amount << " errors!" << std::endl;
	}
	for (std::size_t from = 0; from < to; ++from) {
		std::cerr << errors.what() << std::endl;
	}

	return errors.size() == 0;
}

int main()
{
	assert(test());
}