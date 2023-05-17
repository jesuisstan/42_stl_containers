#ifndef SET_HPP
#define SET_HPP

#include <functional>
#include <algorithm>
#include <memory>
#include "tree.hpp"
#include "iterator_traits.hpp"
#include "reverse_iterator.hpp"

namespace ft {

template<class Key, class Compare = std::less<Key>, class Alloc = std::allocator<Key> >
class set {
	private:
		typedef ft::_Rb_tree<Key, Compare, Alloc>		set_tree;
		typedef typename set_tree::node					set_node;

	public:
		typedef Alloc									allocator_type;
		typedef size_t									size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef Key										key_type;
		typedef Key										value_type;
		typedef Compare									key_compare;
		typedef Compare									value_compare;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef typename allocator_type::pointer		pointer;
		typedef typename allocator_type::const_pointer	const_pointer;
		typedef typename set_tree::const_iterator		iterator;
		typedef typename set_tree::const_iterator		const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		set_tree		_set_tree;
		allocator_type	_set_alloc;

	public:
		explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _set_tree(comp), _set_alloc(alloc) {}

		template<class InputIterator>
		set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
		: _set_tree(comp), _set_alloc(alloc) {
			for (; first != last; first++) {
				_set_tree.insert(*first);
			}
		}

		set(const set& other) : _set_tree(other._set_tree), _set_alloc(other._set_alloc) {}
		~set() {}

		set& operator=(const set& other) {
			if (this == &other)
				return *this;
			_set_tree = other._set_tree;
			_set_alloc = other._set_alloc;
			return *this;
		}

		bool empty() const 											{ return _set_tree.empty(); }
		size_type size() const										{ return _set_tree.size(); }
		size_type max_size() const 									{ return _set_tree.max_size(); }
		allocator_type get_allocator() const 						{ return allocator_type(); }
		iterator begin() 											{ return _set_tree.begin(); }
		const_iterator begin() const 								{ return _set_tree.begin(); }
		iterator end()												{ return _set_tree.end(); }
		const_iterator end() const 									{ return _set_tree.end(); }
		reverse_iterator rbegin() 									{ return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const 						{ return const_reverse_iterator(end()); }
		reverse_iterator rend() 									{ return reverse_iterator(begin()); }
		const_reverse_iterator rend() const 						{ return const_reverse_iterator(begin()); }
		void clear()												{ _set_tree.clear(); }
		ft::pair<iterator, bool> insert(const value_type& val)		{ return _set_tree.insert(val); }
		iterator insert(iterator hint, const value_type& val)		{ return _set_tree.insert(hint, val); }
		key_compare key_comp() const 								{ return key_compare(); }
		value_compare value_comp() const 							{ return value_compare(); }
		void erase(iterator pos)									{ _set_tree.erase(pos); }
		size_type erase(const key_type& key) 						{ return _set_tree.erase(key); }
		void swap(set& other)										{ _set_tree.swap(other._set_tree); }

		template<class ItInput>
		void insert(ItInput first, ItInput last) {
			for(; first != last; first++)
				_set_tree.insert(*first);
		}

		void erase(iterator first, iterator last) { 
			while (first != last)
				first = _set_tree.erase(first);
		}

		size_type count(const key_type& key) const {
			if (!size())
				return 0;
			const_iterator it = _set_tree.find(key);
			return (!key_comp()(*it, key) && !key_comp()(key, *it)) ? 1 : 0;
		}

		iterator find(const key_type& key) {
			if (!size())
				return end();
			iterator it = _set_tree.find(key);
			return (!key_comp()(*it, key) && !key_comp()(key, *it)) ? it : end();
		}

		const_iterator find(const key_type& key) const {
			if (!size())
				return end();
			const_iterator it = _set_tree.find(key);
			return (!key_comp()(*it, key) && !key_comp()(key, *it)) ? it : end();
		}

		ft::pair<iterator, iterator> equal_range(const key_type& key) {
			iterator first = lower_bound(key);
			iterator second = upper_bound(key);
			return ft::make_pair<iterator, iterator>(first, second);
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
			const_iterator first = lower_bound(key);
			const_iterator second = upper_bound(key);
			return ft::make_pair<const_iterator, const_iterator>(first, second);
		}

		iterator lower_bound(const key_type& key) {
			if (!size()) return end();
			iterator	it = _set_tree.find(key);
			return (*it <key) ? ++it : it;
		}

		const_iterator lower_bound(const key_type& key) const {
			if (!size())
				return end();
			const_iterator	it = _set_tree.find(key);
			return (*it <key) ? ++it : it;
		}

		iterator upper_bound(const key_type& key) {
			if (!size())
				return end();
			iterator it = _set_tree.find(key);
			return (*it <= key) ? ++it : it;
		}

		const_iterator upper_bound(const key_type& key) const {
			if (!size())
				return end();
			const_iterator it = _set_tree.find(key);
			return  (*it <= key) ? ++it : it;
		}


	friend bool operator==(const ft::set<Key, Compare, Alloc>& lhs, const ft::set<Key, Compare, Alloc>& rhs) {
		return (lhs._set_tree == rhs._set_tree);
	}

	friend bool operator!=(const ft::set<Key, Compare, Alloc>& lhs, const ft::set<Key, Compare, Alloc>& rhs) {
		return (!(lhs == rhs));
	}

	friend bool operator<(const ft::set<Key, Compare, Alloc>& lhs, const ft::set<Key, Compare, Alloc>& rhs) {
		return (lhs._set_tree <rhs._set_tree);
	}

	friend bool operator>=(const ft::set<Key, Compare, Alloc>& lhs, const ft::set<Key, Compare, Alloc>& rhs) {
		return (!(lhs <rhs));
	}

	friend bool operator>(const ft::set<Key, Compare, Alloc>& lhs, const ft::set<Key, Compare, Alloc>& rhs) {
		return (rhs <lhs);
	}

	friend bool operator<=(const ft::set<Key, Compare, Alloc>& lhs, const ft::set<Key, Compare, Alloc>& rhs) {
		return (!(rhs <lhs));
	}
};

	template<class Key, class Compare, class Alloc>
	void swap(ft::set<Key, Compare, Alloc>& lhs, ft::set<Key, Compare, Alloc>& rhs) { lhs.swap(rhs); }
}

#endif
