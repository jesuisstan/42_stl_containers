#ifndef TREE_ITERATOR_HPP
#define TREE_ITERATOR_HPP

#include "iterator_traits.hpp"
#include "utils.hpp"

#define LEFT 0
#define RIGHT 1

namespace ft {

template<class Node, class T>
class tree_iterator {
	public:
		typedef std::bidirectional_iterator_tag						iterator_category;
		typedef typename ft::iterator_traits< T >::value_type		value_type;
		typedef typename ft::iterator_traits< T >::difference_type	difference_type;
		typedef typename ft::iterator_traits< T >::pointer			pointer;
		typedef typename ft::iterator_traits< T >::reference		reference;

	private:
		Node* _current;
		Node* _lastNode;

	public:
		explicit tree_iterator(Node* n_ptr = NULL, Node* lastNode = NULL) : _current(n_ptr), _lastNode(lastNode) {}

		tree_iterator(const tree_iterator& other) { 
			_current = other.base();
			_lastNode = other.getLastNode();
		}

		template<class U>
		tree_iterator(const tree_iterator<Node, U*>& other,
						typename ft::enable_if<!ft::is_const<U>::value, U>::type* = NULL) {
			_current = other.base();
			_lastNode = other.getLastNode();
		}

		~tree_iterator() {}

		tree_iterator& operator=(const tree_iterator& other) {
			if (this == &other)
				return *this;
			_current = other._current;
			_lastNode = other._lastNode;
			return *this;
		}

		Node* base() const				{ return _current; }
		Node* getLastNode() const		{ return _lastNode; }
		reference operator*() const		{ return *(*_current); }
		pointer operator->() const		{ return &(*(*_current)); }
		tree_iterator operator++(int)	{ tree_iterator tmp(*this); ++(*this); return tmp; }
		tree_iterator operator--(int)	{ tree_iterator tmp(*this); --(*this); return tmp; }

		tree_iterator& operator++() {
			if (_current->child[ RIGHT ]) {
				_current = _current->child[ RIGHT ];
				while (_current->child[ LEFT ])
					_current = _current->child[ LEFT ];
				return *this;
			}
			while (_current->parent) {
				if (*_current < *_current->parent)
					break;
				_current = _current->parent;
			}
			_current = _current->parent;
			return *this;
		}

		tree_iterator& operator--() {
			if (_current == NULL) {
				_current = _lastNode->parent;
				while (_current->child[ RIGHT ])
					_current = _current->child[ RIGHT ];
				return *this;
			}
			if (_current->child[ LEFT ]) {
				_current = _current->child[ LEFT ];
				while (_current->child[ RIGHT ])
					_current = _current->child[ RIGHT ];
				return *this;
			}
			while (*_current < *_current->parent) {
				_current = _current->parent;
			}
			_current = _current->parent;
			return *this;
		}

		friend bool operator==(const tree_iterator& lhs, const tree_iterator& rhs)	{ return (lhs._current == rhs._current && lhs._lastNode == rhs._lastNode); }
		friend bool operator!=(const tree_iterator& lhs, const tree_iterator& rhs)	{ return !(lhs == rhs); }
	};
}

#endif
