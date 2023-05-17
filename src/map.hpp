

#ifndef MAP_HPP
#define MAP_HPP

#include "tree.hpp"
#include "iterator_traits.hpp"
#include "reverse_iterator.hpp"
#include "utils.hpp"

namespace ft {

template< class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<Key> >
class map {
	public:
		typedef T										mapped_type;
		typedef Key										key_type;
		typedef ft::pair<const key_type, mapped_type>	value_type;
		typedef Compare									key_compare;

		class value_compare : public std::binary_function< key_type, value_type, bool > {
			friend class map;

			protected:
				key_compare comp;
				value_compare(Compare c) : comp(c) { }
			public:
				bool operator() (const value_type& x, const value_type& y) const { return comp(x.first, y.first); }
		};

		typedef typename Alloc::template rebind<value_type>::other					allocator_type;
		typedef typename allocator_type::pointer									pointer;
		typedef typename allocator_type::const_pointer								const_pointer;
		typedef typename allocator_type::reference									reference;
		typedef typename allocator_type::const_reference							const_reference;
		typedef typename ft::_Rb_tree<value_type, value_compare>::iterator			iterator;
		typedef typename ft::_Rb_tree<value_type, value_compare>::const_iterator	const_iterator;
		typedef typename ft::reverse_iterator<iterator>								reverse_iterator;
		typedef typename ft::reverse_iterator<const_iterator>						const_reverse_iterator;
		typedef typename ft::_Rb_tree<value_type, value_compare>::difference_type	difference_type;
		typedef typename ft::_Rb_tree<value_type, value_compare>::size_type			size_type;

	private:
		ft::_Rb_tree<value_type, value_compare>	_map_tree;
		allocator_type							_map_alloc;

	public:
		explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) :
			_map_tree(value_compare(comp)),
			_map_alloc(alloc) {}

		template<class InputIterator>
		map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) :
			_map_tree(value_compare(comp)), _map_alloc(alloc) { insert(first, last); }

		map (const map& other) : _map_tree(other._map_tree), _map_alloc(other._map_alloc) {}

		~map() {}

		map& operator=(const map& other) {
			if (this == &other)
				return *this;
			_map_tree = other._map_tree;
			_map_alloc = other._map_alloc;
			return *this;
		}

		bool empty(void) const						{ return _map_tree.empty(); }
		size_type size(void) const					{ return _map_tree.size(); }
		iterator begin(void) 						{ return _map_tree.begin(); }
		const_iterator begin (void) const			{ return _map_tree.begin(); }
		iterator end(void) 							{ return _map_tree.end(); }
		const_iterator end (void) const 			{ return _map_tree.end(); }
		reverse_iterator rbegin(void) 				{ return reverse_iterator(end()); }
		const_reverse_iterator rbegin (void) const	{ return const_reverse_iterator(end()); }
		reverse_iterator rend(void)					{ return reverse_iterator(begin()); }
		const_reverse_iterator rend (void) const	{ return const_reverse_iterator(begin()); }
		size_type max_size(void) const				{ return _map_tree.max_size(); }
		void clear(void)							{ _map_tree.clear(); }
		key_compare key_comp(void) const			{ return key_compare(); }
		value_compare value_comp(void) const		{ return value_compare(key_compare()); }
		allocator_type get_allocator(void) const	{ return allocator_type(); }
		void swap(map& other)						{ _map_tree.swap(other._map_tree); }


		mapped_type& operator[](const key_type& key) {
			value_type	value(key, mapped_type());
			ft::pair< iterator, bool > insert_ret = _map_tree.insert(value);
			return insert_ret.first->second;
		}

		mapped_type& at(const key_type& key) {
			value_type	value(key, mapped_type());
			iterator it = _map_tree.find(value);
			if (it == end())
				throw (std::out_of_range("map"));
			if (value_comp() (*it, value) || value_comp() (value, *it))
				throw (std::out_of_range("map"));
			return it->second;
		}

		const mapped_type& at(const key_type& key) const {
			value_type		value(key, mapped_type());
			const_iterator	it = _map_tree.find(value);
			if (it == end() || *it != value)
				throw (std::out_of_range("map"));
			return it->second;
		}

		ft::pair<iterator, bool> insert(const value_type& val) { return _map_tree.insert(val); }

		iterator insert(iterator hint, const value_type& val) { return _map_tree.insert(hint, val); }

		template< class InputIterator >
		void insert(InputIterator first, InputIterator last) {
			for (; first != last; first++)
				_map_tree.insert(*first);
		}

		void erase(iterator position) { _map_tree.erase(position); }

		size_type erase(const key_type& key) {
			value_type	value(key, mapped_type());
			return _map_tree.erase(value);
		}
	
		void erase(iterator first, iterator last) {
			while (first != last)
				first = _map_tree.erase(first);
		}

		iterator find(const key_type& k) {
			if (!size()) return end();
			value_type	value(k, mapped_type());
			iterator it = _map_tree.find(value);
			return (!value_comp()(*it, value) && !value_comp()(value, *it)) ? it : end();
		}

		const_iterator find(const key_type& k) const {
			if (!size())
				return end();
			value_type 		value(k, mapped_type());
			const_iterator	it = _map_tree.find(value);
			return (!value_comp()(*it, value) && !value_comp()(value, *it)) ? it : end();		
		}

		size_type count(const key_type& k) const {
			if (!size())
				return 0;
			value_type		value(k, mapped_type());
			const_iterator	it = _map_tree.find(value);
			return (!value_comp()(*it, value) && !value_comp()(value, *it)) ? 1 : 0;
		}

		iterator lower_bound(const key_type& k) {
			if (!size())
				return end();
			value_type	value(k, mapped_type());
			iterator	it = _map_tree.find(value);
			return value_comp()(*it, value) ? ++it : it;
		}

		const_iterator lower_bound(const key_type& k) const {
			if (!size())
				return end();
			value_type		value(k, mapped_type());
			const_iterator	it = _map_tree.find(value);
			return value_comp()(*it, value) ? ++it : it;
		}

		iterator upper_bound(const key_type& k) {
			if (!size())
				return end();
			value_type	value(k, mapped_type());
			iterator	it = _map_tree.find(value);
			return !value_comp()(value, *it) ? ++it : it;
		}

		const_iterator upper_bound(const key_type& k) const {
			if (!size())
				return end();
			value_type		value(k, mapped_type());
			const_iterator	it = _map_tree.find(value);
			return !value_comp()(value, *it) ? ++it : it;
		}

		ft::pair< iterator, iterator > equal_range(const key_type& k) {
			iterator first = lower_bound(k);
			iterator second = upper_bound(k);
			return ft::make_pair< iterator, iterator >(first, second);
		}

		ft::pair< const_iterator, const_iterator > equal_range(const key_type& k) const {
			const_iterator first = lower_bound(k);
			const_iterator second = upper_bound(k);
			return ft::make_pair< const_iterator, const_iterator >(first, second);
		}

		friend bool operator==(const map< Key, T, Compare, Alloc >& lhs, const map< Key, T, Compare, Alloc >& rhs) {
			return (lhs._map_tree == rhs._map_tree);
		}

		friend bool operator!=(const map< Key, T, Compare, Alloc >& lhs, const map< Key, T, Compare, Alloc >& rhs) {
			return !(lhs == rhs);
		}

		friend bool operator<(const map< Key, T, Compare, Alloc >& lhs,  const map< Key, T, Compare, Alloc >& rhs) {
			return (lhs._map_tree < rhs._map_tree);
		}

		friend bool operator<=(const map< Key, T, Compare, Alloc >& lhs, const map< Key, T, Compare, Alloc >& rhs) {
			return (lhs < rhs || lhs == rhs);
		}

		friend bool operator>(const map< Key, T, Compare, Alloc >& lhs, const map< Key, T, Compare, Alloc >& rhs) {
			return (rhs < lhs);
		}

		friend bool operator>=(const map< Key, T, Compare, Alloc >& lhs, const map< Key, T, Compare, Alloc >& rhs) {
			return (lhs > rhs || lhs == rhs);
		}
};

template< class Key, class T, class Compare, class Alloc >
void swap(ft::map< Key, T, Compare, Alloc>& lhs, ft::map< Key, T, Compare, Alloc>& rhs) { lhs.swap(rhs); }

}

#endif
