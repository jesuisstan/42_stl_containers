/*
ABOUT:
	vector - https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/container/vector.html
			 http://www.cplusplus.com/reference/vector/vector/?kw=vector
*/

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef> // for types
#include <ios>

#include "iterator_traits.hpp"
#include "random_access_iterator.hpp"
#include "reverse_iterator.hpp"
#include "utils.hpp"

namespace ft {
	template < class T, class Alloc = std::allocator<T> >
	class vector {
	public:
		typedef T															value_type;
		typedef Alloc														allocator_type;
		typedef typename allocator_type::pointer							pointer;
		typedef typename allocator_type::const_pointer						const_pointer;
		typedef typename allocator_type::reference							reference;
		typedef typename allocator_type::const_reference					const_reference;
		typedef ft::random_access_iterator<value_type>						iterator;
		typedef ft::random_access_iterator<const value_type>				const_iterator;
		typedef ft::reverse_iterator<iterator>								reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>						const_reverse_iterator;
		typedef typename ft::iterator_traits<iterator>::difference_type		difference_type; 
		typedef typename allocator_type::size_type							size_type;

	private:
		allocator_type	_alloc;
		pointer			_begin;
		pointer			_end;
		pointer			_edge;

		void Destroy(pointer first, pointer last) {
			for (; first != last; first++)
				_alloc.destroy(first);
		};

		template <class Iter>
		pointer Copy(Iter first, Iter last, pointer current) {
			pointer begin = current;
			try {
				for (; first != last; ++current, ++first)
					_alloc.construct(current, *first);
			}
			catch (...) {
				Destroy(begin, current);
				throw ;
			}
			return current;
		};

	public:
// "explicit" -> it cannot be used for implicit conversions and copy-initialization
		explicit vector(const allocator_type& alloc = allocator_type())
		: _alloc(alloc), _begin(NULL), _end(NULL), _edge(NULL) {}

		explicit vector(size_type count, const value_type& value = value_type(), const allocator_type& alloc = allocator_type())
		: _alloc(alloc), _begin(NULL), _end(NULL), _edge(NULL) {
			_begin = _alloc.allocate(count);
			_edge = _begin + count;
			_end = _begin;
			for(; count--; _end++)
				_alloc.construct(_end, value);
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL) : _alloc(alloc) {
			difference_type dist = ft::distance(first, last);
			_begin = _alloc.allocate(dist);
			_edge = _begin + dist;
			_end = _begin;
			for(; dist--; _end++)
				_alloc.construct(_end, *first++);
		}

		vector(const vector& other) : _alloc(other._alloc), _begin(NULL), _end(NULL), _edge(NULL) {
			this->insert(this->begin(), other.begin(), other.end());
		}

		~vector() {
			this->clear();
			_alloc.deallocate(_begin, this->capacity());
		}

		vector &operator = (const vector& other) {
			if (other == *this)
				return *this;
			this->clear();
			this->insert(this->end(), other.begin(), other.end());
			return *this;
		}

		void resize(size_type count, value_type value = value_type()) {
			if (count > this->max_size())
				throw (std::length_error("vector::resize"));
			else if (count < this->size())
				erase(this->begin() + count, this->end());
			else
				this->insert(this->end(), count - this->size(), value);
		}

		void reserve(size_type new_cap)
		{
			if (this->max_size() < new_cap)
				throw (std::length_error("vector::reserve"));
			else if (this->capacity() < new_cap)
			{
				pointer copy = _alloc.allocate(new_cap);
				try {
					Copy(this->begin(), this->end(), copy);
				}
				catch (...) {
					_alloc.deallocate(copy, new_cap);
					throw ;
				}
				if (_begin != 0)
				{
					Destroy(_begin, _end);
					_alloc.deallocate(_begin, _edge - _begin);
				}
				_edge = copy + new_cap;
				_end = copy + this->size();
				_begin = copy;
			}
		}

		iterator begin()								{ return _begin; };
		const_iterator begin() const					{ return _begin; }
		iterator end() 									{ if (this->empty()) return (this->begin()); return (_end); }
		const_iterator end() const						{ if (this->empty()) return (this->begin()); return (_end); }
		reverse_iterator rbegin()						{ return reverse_iterator(this->end()); }
		const_reverse_iterator rbegin() const 			{ return reverse_iterator(this->end()); }
		reverse_iterator rend()							{ return (reverse_iterator(this->begin())); }
		const_reverse_iterator rend() const 			{ return (reverse_iterator(this->begin())); }
		size_type size(void) const 						{ return (this->_end - this->_begin); }
		size_type capacity (void) const 				{ return (this->_edge - this->_begin); }
		bool empty (void) const 						{ return (size() == 0 ? true : false); }
		size_type max_size(void) const 					{ return allocator_type().max_size(); }
		reference operator[] (size_type n)				{ return *(_begin + n); }
		const_reference operator[] (size_type n) const	{ return *(_begin + n); }
		reference front () 								{ return *_begin; }
		const_reference front () const 					{ return *_begin; }
		reference back ()								{ return *(_end - 1); }
		const_reference back () const					{ return *(_end - 1); }

		reference at(size_type pos) {
			if (pos >= this->size())
				throw (std::out_of_range("vector"));
			return (*this)[pos];
		}

		const_reference at(size_type pos) const {
			if (pos >= this->size())
				throw (std::out_of_range("vector"));
			return (*this)[pos];
		}

		void push_back(const value_type& value) {
			if (_end == _edge) {
				int new_capacity = (this->size() > 0) ? (int)(this->size() * 2) : 1;
				this->reserve(new_capacity);
			}
			_alloc.construct(_end, value);
			_end++;
		}

		void pop_back() {
			_alloc.destroy(&this->back());
			_end--;
		}

		void assign(size_type count, const value_type& value) {
			this->clear();
			if (count == 0)
				return ;
			if (this->capacity() >= count) {
				for(; count--; _end++)
					_alloc.construct(_end , value);
			}
			else {
				_alloc.deallocate(_begin, this->capacity());
				_begin = _alloc.allocate(count);
				_end = _begin;
				_edge = _begin + count;
				for(; count--; _end++)
					_alloc.construct(_end , value);
			}
		}

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL) {
			this->clear();
			size_type dist = ft::distance(first, last);
			if (this->capacity() >= dist) {
				for(; &(*first) != &(*last); first++, _end++)
					_alloc.construct(_end, *first);
			}
			else {
				pointer new_begin = pointer();
				pointer new_end = pointer();
				pointer new_edge = pointer();
				new_begin = _alloc.allocate(dist);
				new_end = new_begin;
				new_edge = new_begin + dist;
				for(; &(*first) != &(*last); first++, new_end++)
					_alloc.construct(new_end, *first);
				_alloc.deallocate(_begin, this->capacity());
				_begin = new_begin;
				_end = new_end;
				_edge = new_edge;
			}
		}

		iterator insert(iterator pos, const value_type& value) {
			size_type length_to_pos = &(*pos) - _begin;
			if (size_type(_edge - _end) >= this->size() + 1) {
				for (size_type i = 0; i < length_to_pos; i++)
					_alloc.construct(_end - i, *(_end - i - 1));
				_end++;
				_alloc.construct(&(*pos), value);
			}
			else {
				pointer new_begin = pointer();
				pointer new_end = pointer();
				pointer new_edge = pointer();
				int new_capacity = (this->size() * 2 > 0) ? this->size() * 2 : 1; 
				new_begin = _alloc.allocate(new_capacity);
				new_end = new_begin + this->size() + 1;
				new_edge = new_begin + new_capacity;
				for (size_type i = 0; i < length_to_pos; i++)
					_alloc.construct(new_begin + i, *(_begin + i));
				_alloc.construct(new_begin + length_to_pos, value);
				for (size_type j = 0; j < this->size() - length_to_pos; j++)
					_alloc.construct(new_end - j - 1, *(_end - j - 1));
				for (size_type x = 0; x < this->size(); x++)
					_alloc.destroy(_begin + x);
				if (_begin)
					_alloc.deallocate(_begin, this->capacity());
				_begin = new_begin;
				_end = new_end;
				_edge = new_edge;
			}
			return iterator(_begin + length_to_pos);
		}

		void insert(iterator pos, size_type count, const value_type& value) {
			if (count == 0)
				return ;
			if (count > this->max_size())
				throw (std::length_error("vector::insert (fill)"));
			size_type length_to_pos = &(*pos) - _begin;
			if (size_type(_edge - _end) >= count) {
				for (size_type i = 0; i < this->size() - length_to_pos; i++)
					_alloc.construct(_end - i + (count - 1), *(_end - i - 1));
				_end += count;
				for(; count; count--)
					_alloc.construct(&(*pos) + (count - 1), value);
			}
			else {
				pointer new_begin = pointer();
				pointer new_end = pointer();
				pointer new_edge = pointer();
				int new_capacity = (this->capacity() > 0) ? (int)(this->size() * 2) : 1;
				new_begin = _alloc.allocate(new_capacity);
				new_edge = new_begin + new_capacity;
				if (size_type(new_edge - new_begin) < this->size() + count) {
					if (new_begin)
						_alloc.deallocate(new_begin, new_begin - new_edge);
					new_capacity = this->size() + count;
					new_begin = _alloc.allocate(new_capacity);
					new_end = new_begin + this->size() + count;
					new_edge = new_begin + new_capacity;
				}
				new_end = new_begin + this->size() + count;
				for (int i = 0; i < (&(*pos) - _begin); i++)
					_alloc.construct(new_begin + i, *(_begin + i));
				for (size_type n = 0; n < count; n++)
					_alloc.construct(new_begin + length_to_pos + n, value);
				for (size_type j = 0; j < (this->size() - length_to_pos); j++)
					_alloc.construct(new_end - j - 1, *(_end - j - 1));
				for (size_type x = 0; x < this->size(); x++)
					_alloc.destroy(_begin + x);
				if (_begin)
					_alloc.deallocate(_begin, this->capacity());
				_begin = new_begin;
				_end = new_end;
				_edge = new_edge;
			}
		}

		template <class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL) {
			size_type dist = ft::distance(first, last);
			if (size_type(_edge - _end) >= dist) {
				for(size_type i = 0; i < this->size() - (&(*pos) - _begin); i++)
					_alloc.construct(_end - i + (dist - 1), *(_end - i - 1));
				_end += dist;
				for (; &(*first) != &(*last); first++, pos++)
					_alloc.construct(&(*pos), *first);
			}
			else {
				pointer new_begin = pointer();
				pointer new_end = pointer();
				pointer new_edge = pointer();
				new_begin = _alloc.allocate(this->size() * 2);
				new_edge = new_begin + (this->size() * 2);
				new_end = new_begin + this->size() + dist;
				if (size_type(new_edge - new_begin) < this->size() + dist) {
					if (new_begin)
						_alloc.deallocate(new_begin, new_edge - new_begin);
					new_begin = _alloc.allocate(this->size() + dist);
					new_end = new_begin + this->size() + dist;
					new_edge = new_end;
				}
				for (int i = 0; i < &(*pos) - _begin; i++)
					_alloc.construct(new_begin + i, *(_begin + i));
				for (int j = 0; &(*first) != &(*last); first++, j++)
					_alloc.construct(new_begin + (&(*pos) - _begin) + j, *first);
				for (size_type n = 0; n < this->size() - (&(*pos) - _begin); n++)
					_alloc.construct(new_begin + (&(*pos) - _begin) + dist + n, *(_begin + (&(*pos) - _begin) + n));
				for (size_type x = 0; x < this->size(); x++)
					_alloc.destroy(_begin + x);
				_alloc.deallocate(_begin, this->capacity());
				_begin = new_begin;
				_end = new_end;
				_edge = new_edge;
			}
		}

		iterator erase(iterator pos) {
			pointer ret_pos = &(*pos);
			if (&(*pos) + 1 == _end)
				_alloc.destroy(&(*pos));
			else {
				_alloc.destroy(&(*pos));
				for (int i = 0; i < _end - &(*pos) - 1; i++) {
					_alloc.construct(&(*pos) + i, *(&(*pos) + i + 1));
					_alloc.destroy(&(*pos) + i + 1);
				}
			}
			_end -= 1;
			return iterator(ret_pos);
		}

		iterator erase(iterator first, iterator last) {
			pointer ret_pos = &(*first);
			for (; &(*first) != &(*last); first++)
				_alloc.destroy(&(*first));
			for (int i = 0; i < _end - &(*last); i++) {
				_alloc.construct(ret_pos + i, *(&(*last) + i));
				_alloc.destroy(&(*last) + i);
			}
			_end -= (&(*last) - ret_pos);
			return iterator(ret_pos);
		}

		void swap(vector& other) {
			if (other == *this)
				return;
			pointer tmp_begin = other._begin;
			pointer tmp_end = other._end;
			pointer tmp_edge = other._edge;
			allocator_type tmp_alloc = other._alloc;
			other._begin = this->_begin;
			other._end = this->_end;
			other._edge = this->_edge;
			other._alloc = this->_alloc;
			this->_begin = tmp_begin;
			this->_end = tmp_end;
			this->_edge = tmp_edge;
			this->_alloc = tmp_alloc;
		}

		void clear() {
			size_type tmp_size = this->size();
			for (size_type i = 0; i < tmp_size; i++) {
				_end--;
				_alloc.destroy(_end);
			}
		}
	};

	template <class T, class Alloc>
	inline
	bool operator == (const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs) {
		if (lhs.size() != rhs.size())
			return false;
		for (size_t i = 0; i < lhs.size(); ++i) {
			if (lhs[i] == rhs[i]) { continue; }
			return false;
		}
		return true;
	}

	template <class T, class Alloc>
	inline
	bool operator != (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs == rhs); }
	
	template <class T, class Alloc>
	inline
	bool operator < (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

	template <class T, class Alloc>
	inline
	bool operator <= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(rhs < lhs); }

	template <class T, class Alloc>
	inline
	bool operator > (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return rhs < lhs; }

	template <class T, class Alloc>
	inline
	bool operator >= (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs < rhs); }


	template <class T, class Alloc>
	void swap (vector<T,Alloc>& one, vector<T,Alloc>&other) { one.swap(other); }
}

#endif
