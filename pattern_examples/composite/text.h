#include <memory>
#include <iostream>
#include <list>
#include <algorithm>

struct ITextGlif
{
	using ptr_t = std::shared_ptr<ITextGlif>;
	using char_t = char;
	using stream_t = std::basic_ostream<char_t>;
	
	virtual void print(stream_t & = std::cout) 
	{
		throw std::runtime_error("ITextGlif: cannot print anything.");
	}

	virtual void remove(ptr_t const &glif_ptr) 
	{
		throw std::runtime_error("ITextGlif: cannot remove anything.");
	}

	virtual void add(ptr_t const &glif_ptr) 
	{
		throw std::runtime_error("ITextGlif: cannot add anything.");
	}

	virtual void replace(ptr_t const &old_glif_ptr, ptr_t const &new_glif_ptr)
	{
		throw std::runtime_error("ITextGlif: cannot replace anything.");
	}

	virtual ~ITextGlif() = 0;
};

// Declaring ITextGlif as abstract class need definition of pure destructor:
ITextGlif::~ITextGlif() {}

class Letter : public ITextGlif
{
private:
	char_t _data;
public:
	Letter(char_t data) : _data(data) {}

	void print(stream_t &stream = std::cout) final {
		stream << _data;
	}

	~Letter() final = default;
};

class CompositeText : public ITextGlif
{
private:
	std::list<ptr_t> _glif_ptrs;
public:
	CompositeText() = default;
	~CompositeText() final = default;

	void remove(ptr_t const &glif_ptr) final {
		_glif_ptrs.remove(glif_ptr);
	}

	void add(ptr_t const &glif_ptr) final {
		_glif_ptrs.push_back(glif_ptr);
	}

	void replace(ptr_t const &old_glif_ptr, ptr_t const &new_glif_ptr) final
	{
		std::replace(_glif_ptrs.begin(), _glif_ptrs.end(), old_glif_ptr, new_glif_ptr);
	}

	void print(stream_t &stream = std::cout) final {
		for (auto glif_ptr : _glif_ptrs) {
			glif_ptr->print(stream);
		}
	}
};

using text_ptr_t = ITextGlif::ptr_t;
using char_t = ITextGlif::char_t;

text_ptr_t makeSymbol(char_t c)
{
	return std::make_shared<Letter>(c);
}

text_ptr_t makeCompositeText()
{
	return std::make_shared<CompositeText>();
}