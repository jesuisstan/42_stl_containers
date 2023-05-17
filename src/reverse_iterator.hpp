/*
ABOUT:
	reverse_iterator - https://en.cppreference.com/w/cpp/iterator/reverse_iterator
*/

#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

#include "iterator_traits.hpp"
#include "utils.hpp"

namespace ft {
	template<class Iterator>
	class reverse_iterator {
	public:
		typedef Iterator iterator_type;
		typedef typename ft::iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename ft::iterator_traits<Iterator>::value_type			value_type;
		typedef typename ft::iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename ft::iterator_traits<Iterator>::reference			reference;

	protected:
		iterator_type _current;

	public:
		reverse_iterator() : _current(NULL) {}
		explicit reverse_iterator(iterator_type it) : _current(it) {}
		~reverse_iterator() {}

		template<class It>
		reverse_iterator (const reverse_iterator<It>& other) : _current(other.base()) { }

		template<class It>
		reverse_iterator& operator=(const reverse_iterator<It>& other) {
			_current = other.base();
			return *this;
		}

		iterator_type base() const						{ return _current; }
		reference operator*() const						{ iterator_type	tmp(_current); return *(--tmp); }
		pointer operator->() const						{ return &(operator*()); }
		reverse_iterator& operator++()					{ --_current; return *this; }
		reverse_iterator& operator--()					{ ++_current; return *this; }
		reverse_iterator operator++(int)				{ reverse_iterator _tmp(*this); --_current; return _tmp; }
		reverse_iterator operator--(int) 				{ reverse_iterator _tmp(*this); ++_current; return _tmp; }
		reverse_iterator operator+(difference_type n)	{ reverse_iterator _tmp(*this); _tmp += n; return _tmp; }
		reverse_iterator operator-(difference_type n)	{ reverse_iterator _tmp(*this); _tmp -= n; return _tmp; }
		reverse_iterator& operator+=(difference_type n)	{ _current -= n; return *this; }
		reverse_iterator& operator-=(difference_type n)	{ _current += n; return *this; }
		reference operator[] (difference_type n) const	{ return _current[ - n - 1]; }
	};

	template<class It1, class It2>
	bool operator==(const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs) {
		return (lhs.base() == rhs.base());
	}

	template<class It1, class It2>
	bool operator!=(const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs) {
		return (lhs.base() != rhs.base());
	}

	template<class It1, class It2>
	bool operator<(const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs) {
		return (lhs.base() > rhs.base());
	}

	template<class It1, class It2>
	bool operator>(const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs) {
		return (lhs.base() < rhs.base());
	}

	template<class It1, class It2>
	bool operator<=(const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs) {
		return (lhs.base() >= rhs.base());
	}

	template<class It1, class It2>
	bool operator>=(const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs) {
		return (lhs.base() <= rhs.base());
	}

	template<class It1>
	reverse_iterator<It1> operator+(typename iterator_traits<It1>::difference_type n, const reverse_iterator<It1>& it) {
		reverse_iterator<It1> _tmp(it);
		_tmp += n;
		return _tmp;
	}

	template<class It1, class It2>
	reverse_iterator<It1> operator+(typename iterator_traits<It1>::difference_type n, const reverse_iterator<It2>& it) {
		reverse_iterator<It1> _tmp(it);
		_tmp += n;
		return _tmp;
	}

	template<class It1, class It2>
	typename iterator_traits<It1>::difference_type operator-(const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs) {
		return rhs.base() - lhs.base();
	}
}

#endif
