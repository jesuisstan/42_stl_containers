/*
	Random-access iterators allow to access elements
	at an arbitrary offset position relative to the element they point to.
*/

#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

#include "iterator_traits.hpp"
#include "utils.hpp"

namespace ft {
	template <typename T>
	class random_access_iterator : ft::iterator<ft::random_access_iterator_tag, T> {
	public:
		typedef T*		pointer;
		typedef T&		reference;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::iterator_category		iterator_category;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::value_type			value_type;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::difference_type		difference_type;

	private:
		pointer _data;

	public:
		random_access_iterator() : _data(NULL) {}
		random_access_iterator(pointer item) : _data(item) {}
		random_access_iterator(const random_access_iterator& other) : _data(other._data) {}
		virtual ~random_access_iterator() {}

		random_access_iterator &operator=(const random_access_iterator& other) {
			if (this == &other)
				return (*this);
			this->_data = other._data;
			return (*this);
		}

		pointer base() const 										{ return (_data); }
		reference operator *() const 								{ return (*_data); }
		pointer operator ->() 										{ return &(operator*()); }
		random_access_iterator& operator ++()						{ _data++; return (*this); }
		random_access_iterator& operator --() 						{_data--; return (*this); }
		random_access_iterator operator ++(int) 					{ random_access_iterator tmp(*this); operator++(); return (tmp); }
		random_access_iterator operator --(int) 					{ random_access_iterator tmp(*this); operator--(); return tmp; }
		random_access_iterator operator +(difference_type n) const	{ return (_data + n); }
		random_access_iterator operator -(difference_type n) const	{ return (_data - n); }
		random_access_iterator& operator +=(difference_type n)		{ _data += n; return (*this); }
		random_access_iterator& operator -=(difference_type n) 		{ _data -= n; return (*this); }
		reference operator[](difference_type n) const				{ return (*(operator+(n))); }
		operator random_access_iterator<const T> () const			{ return (random_access_iterator<const T>(this->_data)); }
	};

	template <typename T>
	typename ft::random_access_iterator<T>::difference_type
	operator ==(const ft::random_access_iterator<T> lhs, const ft::random_access_iterator<T> rhs) {
		return (lhs.base() == rhs.base());
	}

	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type
	operator ==(const ft::random_access_iterator<T_L> lhs, const ft::random_access_iterator<T_R> rhs) {
		return (lhs.base() == rhs.base());
	}

	template <typename T>
	typename ft::random_access_iterator<T>::difference_type
	operator !=(const ft::random_access_iterator<T> lhs, const ft::random_access_iterator<T> rhs) {
		return (lhs.base() != rhs.base());
	}

	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type
	operator !=(const ft::random_access_iterator<T_L> lhs, const ft::random_access_iterator<T_R> rhs) {
		return (lhs.base() != rhs.base());
	}

	template <typename T>
	typename ft::random_access_iterator<T>::difference_type
	operator <(const ft::random_access_iterator<T> lhs, const ft::random_access_iterator<T> rhs) {
		return (lhs.base() < rhs.base());
	}

	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type
	operator <(const ft::random_access_iterator<T_L> lhs, const ft::random_access_iterator<T_R> rhs) {
		return (lhs.base() < rhs.base());
	}

	template <typename T>
	typename ft::random_access_iterator<T>::difference_type
	operator >(const ft::random_access_iterator<T> lhs, const ft::random_access_iterator<T> rhs) {
		return (lhs.base() > rhs.base());
	}

	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type
	operator >(const ft::random_access_iterator<T_L> lhs, const ft::random_access_iterator<T_R> rhs) {
		return (lhs.base() > rhs.base());
	}

	template <typename T>
	typename ft::random_access_iterator<T>::difference_type
	operator <=(const ft::random_access_iterator<T> lhs, const ft::random_access_iterator<T> rhs) {
		return (lhs.base() <= rhs.base());
	}

	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type
	operator <=(const ft::random_access_iterator<T_L> lhs, const ft::random_access_iterator<T_R> rhs) {
		return (lhs.base() <= rhs.base());
	}

	template <typename T>
	typename ft::random_access_iterator<T>::difference_type
	operator >=(const ft::random_access_iterator<T> lhs, const ft::random_access_iterator<T> rhs) {
		return (lhs.base() >= rhs.base());
	}

	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type
	operator >=(const ft::random_access_iterator<T_L> lhs, const ft::random_access_iterator<T_R> rhs) {
		return (lhs.base() >= rhs.base());
	}

	template<typename T>
	ft::random_access_iterator<T> operator +(typename ft::random_access_iterator<T>::difference_type n,
											typename ft::random_access_iterator<T>& it) {
			return (&(*it) + n);
		}

	template <typename T>
	typename ft::random_access_iterator<T>::difference_type
	operator -(const ft::random_access_iterator<T> lhs, const ft::random_access_iterator<T> rhs) {
		return (lhs.base() - rhs.base());
	}

	template<typename T_L, typename T_R>
	typename ft::random_access_iterator<T_L>::difference_type
	operator -(const ft::random_access_iterator<T_L> lhs, const ft::random_access_iterator<T_R> rhs) {
		return (lhs.base() - rhs.base());
	}

}

#endif