// -*- C++ -*-

#ifndef PARAGRAPH_LIST_H
#define PARAGRAPH_LIST_H

#include <iterator>

class Paragraph;

///
class ParagraphList {
public:
	///
	class iterator {
	public:
		///
		typedef std::bidirectional_iterator_tag iterator_category;
		///
		typedef Paragraph * value_type;
		///
		typedef ptrdiff_t difference_type;
		///
		typedef Paragraph * pointer;
		///
		typedef Paragraph & reference;
		///
		iterator();
		///
		iterator(value_type);
		///
		reference operator*();
		///
		pointer operator->();
		///
		iterator & operator++();
		///
		iterator operator++(int);
		///
		iterator & operator--();
		///
		iterator operator--(int);
	private:
		///
		Paragraph * ptr;
	};
	///
	ParagraphList();
	///
	iterator insert(iterator it, Paragraph * par);
	///
	void clear();
	///
	void erase(iterator it);
	///
	iterator begin();
	///
	iterator begin() const;
	///
	iterator end();
	///
	iterator end() const;
	///
	void set(Paragraph *);
	///
	void push_back(Paragraph *);
	///
	Paragraph const * back() const;
	///
	Paragraph * back();
	///
	int size() const;
	///
	bool empty() const;
private:
	///
	Paragraph * parlist;
};

///
bool operator==(ParagraphList::iterator const & i1,
		ParagraphList::iterator const & i2);
///
bool operator!=(ParagraphList::iterator const & i1,
		ParagraphList::iterator const & i2);

#endif
