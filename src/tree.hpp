/*
ABOUT:
	RBTree - https://sd.blackball.lv/library/Introduction_to_Algorithms_Third_Edition_(2009).pdf
			 https://cs.kangwon.ac.kr/~leeck/file_processing/red_black_tree.pdf

	Properties:
	1. A node is either red or black.
	2. The root is black.	
	3. All leaves are the same color as the root.
	4. Both children of every red node are black.
	5. Every simple path from a given node to any of its descendant leaves contains the same number of black nodes.
*/

#ifndef TREE_HPP
#define TREE_HPP

#include "iterator_traits.hpp"
#include "pair.hpp"
#include "tree_iterator.hpp"
#include "utils.hpp"

#include <limits>
#include <iostream>

namespace ft {

enum _Rb_tree_color { BLACK, RED };

template<class T, class Compare = std::less<T> >
class node {
public:
	typedef T			value_type;
	typedef Compare		value_compare;
	node*				parent;
	node* 				child[2];

private:
	value_type			_value;
	value_compare		_comp;
	int					_color;

public:
	node(const value_type& value = value_type(), const value_compare& comp = value_compare()) : parent(), child(), _value(value), _comp(comp), _color(RED) {}
	node(const node& other) : parent(), child(), _value(other._value), _comp(other._comp), _color(other.getColor()) {}
	~node() {}

	value_compare value_comp() const	{ return _comp; }
	const int& getColor() const 		{ return _color; }
	void changeColor() 					{ _color = !_color; }
	void changeColor(int color) 		{ _color = color; }

	node& operator=(const node& other) {
		if (this == &other)
			return *this;
		_value(other._value);
		return *this;
	}

	value_type&	operator*() { return _value; }
	
	friend bool operator==(const node& lhs, const node& rhs) { return (!(lhs < rhs) && !(rhs < lhs)); }
	friend bool operator!=(const node &lhs, const node &rhs) { return (!(lhs == rhs)); }
	friend bool operator<(const node &lhs, const node &rhs) { return (lhs.value_comp()(lhs._value, rhs._value)); }
	friend bool operator>=(const node &lhs, const node &rhs) { return (!(lhs < rhs)); }
	friend bool operator>(const node &lhs, const node &rhs) { return (lhs.value_comp()(rhs._value, lhs._value)); }
	friend bool operator<=(const node &lhs, const node &rhs) { return (!(lhs > rhs)); }
};



template<class T, class Compare = std::less<T>, class Alloc = std::allocator<T> >
class _Rb_tree {
	public:
		typedef T															value_type;
		typedef Compare														value_compare;
		typedef size_t														size_type;
		typedef ft::node<value_type, value_compare>							node;
		typedef tree_iterator< node, value_type*>							iterator;
		typedef tree_iterator< node, const value_type*>						const_iterator;
		typedef typename Alloc::template rebind<node>::other				allocator_type;
		typedef typename ft::iterator_traits<iterator>::difference_type		difference_type;

	private:
		node*				_root;
		node*				_lastNode;
		size_type			_size;
		value_compare		_tree_comp;
		allocator_type		_tree_alloc;

	public:
		explicit _Rb_tree(const value_compare& comp = value_compare(), const allocator_type& alloc = allocator_type()) : _root(), _size(), _tree_comp(comp), _tree_alloc(alloc) { 
			_lastNode = _tree_alloc.allocate(1);
		}

		_Rb_tree(const _Rb_tree& other) : _root(), _size(), _tree_comp(other._tree_comp), _tree_alloc(other._tree_alloc) {
			_lastNode = _tree_alloc.allocate(1);
			*this = other;
		}

		~_Rb_tree() {
			if (_root)
				_deleteTreeFrom(_root);
			_tree_alloc.deallocate(_lastNode, 1);
		}

		_Rb_tree& operator=(const _Rb_tree& other) {
			if (this == &other)
				return *this;
			if (_root != NULL)
				_deleteTreeFrom(_root);
			_root = _copyTreeFrom(other.root());
			_lastNode->parent = _root;
			_size = other.size();
			return *this;
		}

		iterator begin() {
			node* _first = _root;
			while (_first && _first->child[ LEFT ])
				_first = _first->child[ LEFT ];
			return iterator(_first, _lastNode);
		}

		const_iterator begin() const {
			node* _first = _root;
			while (_first && _first->child[ LEFT ])
				_first = _first->child[ LEFT ];
			return const_iterator(_first, _lastNode);
		}

		iterator end()				{ return iterator(NULL, _lastNode); }
		const_iterator end() const	{ return const_iterator(NULL, _lastNode); }
		size_type size() const		{ return _size; }
		bool empty() const			{ return size() ? false : true; }
		size_type max_size() const	{ return std::min<size_type>(std::numeric_limits< difference_type >::max(), get_allocator().max_size()); }

		void clear() {
			_deleteTreeFrom(root());
			_root = NULL;
			_lastNode->parent = NULL;
			_size = 0;
		}

		ft::pair<iterator, bool> insert(const value_type& value) {
			iterator it = find(value);
			if (it == end())
				return ft::pair<iterator, bool>(_insertRoot(value), true);
			if (!value_comp()(*it, value) && !value_comp()(value, *it))
				return ft::pair<iterator, bool>(it, false);
			_size++;
			node* n = _insertNew(it.base(), value);
			_insertFixUp(n);
			return ft::pair<iterator, bool>(iterator(n, _lastNode), true);
		}

		size_type erase(const value_type& value) { 
			iterator it = find(value);
			if (it == end() || value_comp()(*it, value) || value_comp()(value, *it))
				return 0;
			erase(it);
			return 1;
		}

		iterator erase(const_iterator it) {
			node* _ptr = it.base();
			_size--;
			if (_ptr == _root && !_size) {
				_tree_alloc.destroy(_ptr);
				_tree_alloc.deallocate(_ptr, 1);
				_root = NULL;
				_lastNode->parent = NULL;
				return end();
			}
			node* next = (++it).base();
			if (_isInnerNode(_ptr)) 
				_swapNodes(_ptr, next);
			_deleteFixUp(_ptr);
			_tree_alloc.destroy(_ptr);
			_tree_alloc.deallocate(_ptr, 1);
			return iterator(next, _lastNode);
		}

		iterator insert(const_iterator hint, const value_type& value) {
			if (begin() == end()) // if empty tree
				return _insertRoot(value);
			if (hint == end()) {
				if (value_comp()(*(--end()), value)) {// if last element is less than value -> insert in last position
					node* n = _insertNew((--end()).base(), value);
					_insertFixUp(n);
					++_size;
					return iterator(n, _lastNode);
				}
				else
					return insert(value).first;
			}
			if (*hint == value) // if hint has the same key as value -> do nothing
				return iterator(hint.base(), hint.getLastNode());
			if (hint == begin() && value_comp()(value, *hint)) { // if value less than 1st element -> insert in first position.
				node* n = _insertNew((begin()).base(), value);
				_insertFixUp(n);
				++_size;
				return iterator(n, _lastNode);
			}
			
			/* hint points to node comparing more than value,
				hint's inorder predecessor points to node comparing lower than value,
				search for insert position from hint's pred ptr, then insert. */
			if (value_comp()(value, *hint) && value_comp()(*(--hint), value)) {
				node* n = _insertNew(_findInSubtree(hint.base(), value), value);
				_insertFixUp(n);
				++_size;
				return iterator(n, _lastNode);
			}
			return insert(value).first;
		}

		void swap(_Rb_tree& other) {
			if (this->get_allocator() == other.get_allocator()) {
				std::swap(_root, other._root);
				std::swap(_lastNode, other._lastNode);
				std::swap(_size, other._size);
			}
			else {
				_Rb_tree tmp = *this;
				*this = other;
				other = tmp;
			}
		}

		node* root(void) const								{ return _root; }
		value_compare value_comp(void) const				{ return _tree_comp; }
		allocator_type get_allocator(void) const			{ return _tree_alloc; }
		iterator find(const value_type& value)				{ node *tmp = _findInSubtree(_root, value); return iterator(tmp, _lastNode); }
		const_iterator find(const value_type& value) const	{ node *tmp = _findInSubtree(_root, value); return const_iterator(tmp, _lastNode); }

	private:

		void _rotate(node* old_root, int dir) {
			node* new_root = old_root->child[ !dir ];
			if (!new_root)
				return ;
			if (old_root == root()) {
				_root = new_root;
				_lastNode->parent = _root;
			} else
				old_root->parent->child[ _makeSelfie(old_root) ] = new_root;
			new_root->parent = old_root->parent;
			old_root->parent = new_root;
			old_root->child[ !dir ] = new_root->child[ dir ];
			new_root->child[ dir ] = old_root;
			if (old_root->child[ !dir ] != NULL)
				old_root->child[ !dir ]->parent = old_root;
		}

		node* _findInSubtree(node* start, const value_type& value) const {
			if (!start)
				return NULL;
			node* next;
			while (value_comp()(*(*start), value) || value_comp()(value, *(*start))) {
				if (value_comp()(*(*start), value))
					next = start->child[ RIGHT ];
				else
					next = start->child[ LEFT ];
				if (!next)
					break ;
				start = next;
			}
			return start;
		}


		void _swapNodes(node* lhs, node* rhs) {
			node* tmp[3] = { lhs->parent, lhs->child[ LEFT ], lhs->child[ RIGHT ] };
			int node_id_lhs = -1;
			int node_id_rhs = -1;
			if (lhs->parent)
				node_id_lhs = _makeSelfie(lhs);
			if (rhs->parent)
				node_id_rhs = _makeSelfie(rhs);

			int lhs_color = lhs->getColor();
			lhs->changeColor(rhs->getColor());
			rhs->changeColor(lhs_color);

			lhs->parent = (rhs->parent == lhs) ? rhs : rhs->parent;
			lhs->child[ LEFT ] = rhs->child[ LEFT ];
			lhs->child[ RIGHT ] = rhs->child[ RIGHT ];

			rhs->parent = tmp[0];
			rhs->child[ LEFT ] = (tmp[1] == rhs) ? lhs : tmp[1];
			rhs->child[ RIGHT ] = (tmp[2] == rhs) ? lhs : tmp[2];

			if (lhs->parent)
				lhs->parent->child[ node_id_rhs ] = lhs;
			if (lhs->child[ LEFT ])
				lhs->child[ LEFT ]->parent = lhs;
			if (lhs->child[ RIGHT ])
				lhs->child[ RIGHT ]->parent = lhs;

			if (rhs->parent)
				rhs->parent->child[ node_id_lhs ] = rhs;
			if (rhs->child[ LEFT ])
				rhs->child[ LEFT ]->parent = rhs;
			if (rhs->child[ RIGHT ])
				rhs->child[ RIGHT ]->parent = rhs;

			if (lhs == root()) {
				_root = rhs;
				_lastNode->parent = rhs;
			}
		}

		iterator _insertRoot(const value_type& value) {
			node _value(value, _tree_comp);
			_root = _tree_alloc.allocate(1);
			_tree_alloc.construct(_root, _value);
			_lastNode->parent = _root;
			_root->changeColor();
			_size++;
			return iterator(_root, _lastNode);
		}

		node* _insertNew(node* parentNode, const value_type& value) {
			node tmp(value, _tree_comp);
			node* newNode = _tree_alloc.allocate(1);
			_tree_alloc.construct(newNode, tmp);
			newNode->parent = parentNode;
			parentNode->child[ (*newNode < *parentNode) ? LEFT : RIGHT ] = newNode;
			return newNode;
		}

		void _insertFixUp(node* n) {
			/* node is root -> change color from RED to BLACK */
			if (n == _root) {
				n->changeColor(BLACK);
				return ;
			}

			/* parent of node is BLACK, tree is balanced -> return */
			if (n->parent->getColor() == BLACK) {
				return ;
			}

			void (_Rb_tree::*_insertFixUpList[3])(node *) = {
				&_Rb_tree::_insertFixUp_3,
				&_Rb_tree::_insertFixUp_4,
				&_Rb_tree::_insertFixUp_5
			};
			int case_id = (_whatIsTheColor(_getUncle(n)) == BLACK);
			if (case_id)
				case_id += (_makeSelfie(n) == _makeSelfie(n->parent));
			(this->*_insertFixUpList[ case_id ])(n);
		}

		/* parent of node and uncle of node are RED -> repaint them, 
			if grandparent != root -> repaint grandparent of node (to maintain property 5)
			recursive call in grandparent of node to check if the tree is balanced */
		void _insertFixUp_3(node* n) {
			_getParent(n)->changeColor();
			_getUncle(n)->changeColor();
			if (_getGrandParent(n) != root())
				_getGrandParent(n)->changeColor();
			_insertFixUp(_getGrandParent(n));
		}

		/* parent of node is RED, uncle of node is BLACK, node and parent of node do not share same node-id (LEFT or RIGHT),
			_rotate Parent in opposite direction, then call case 5 with old-parent of node */
		void _insertFixUp_4(node* n) {
			node* tmp = n->parent;
			_rotate(n->parent, _makeSelfie(n->parent));
			_insertFixUp(tmp);
		}

		/* parent of node is RED, uncle of node is BLACK, n and parent of node do share same child id,
			_rotate grandparent of node in opposite direction, change colors of Parent and Grandparent */
		void _insertFixUp_5(node* n) {
			_getParent(n)->changeColor();
			_getGrandParent(n)->changeColor();
			_rotate(_getGrandParent(n), !_makeSelfie(n));
		}


		/* 
		 *
		 * case 1: node to delete is RED, make parent's access to node NULL
		 * case 2: node to delete is BLACK and has a RED child (RED means non-leaf, and other child is leaf),
		 * 		   changeColor child and remove node (rewrite parent and child's access)
		 * case 3: check recursive cases 
		 * 
		 */

		void _deleteFixUp(node* n) {
			void (_Rb_tree::*_deleteFixUpList[2])(node *) = {
				&_Rb_tree::_deleteFixUp_1,
				&_Rb_tree::_deleteFixUp_2
			};
			int	childID;
			if (n->child[ RIGHT ])
				childID = RIGHT;
			else
				childID = LEFT;
			int	caseID = !(n->getColor() == RED);

			if (caseID)
				caseID += !(_whatIsTheColor(n->child[ childID ]));
			
			if (caseID == 1) // node to delete is RED -> make parent's access to node NULL
				n = n->child[ childID ];
			
			/* node to delete is BLACK and has a RED child (RED means non-leaf and other child is leaf) ->
				-> change color of child & remove node (rewrite parent and child's access) */
			if (caseID == 2) {
				_deleteFixUpCase(n);
				if (n->parent)
					n->parent->child[ _makeSelfie(n) ] = NULL;
			}
			else // check recursive cases
				(this->*_deleteFixUpList[ caseID ])(n);
		}

		void _deleteFixUpCase(node* n) {
			if (n == root()) {
				n->changeColor(BLACK);
				return ;
			}
			void (_Rb_tree::*_deleteFixUpCaseList[4])(node *) = {
				&_Rb_tree::_deleteFixUpCase1,
				&_Rb_tree::_deleteFixUpCase2,
				&_Rb_tree::_deleteFixUpCase3,
				&_Rb_tree::_deleteFixUpCase4
			};
			int	case_id = (_whatIsTheColor(_getSibling(n)) == BLACK);
			if (case_id && _whatIsTheColor(_getFarChild(n)) == BLACK) {
				case_id += (_whatIsTheColor(_getCloseChild(n)) == RED) ? 1 : 2;
			}
			(this->*_deleteFixUpCaseList[ case_id ])(n);
		}

		void _deleteFixUp_1(node* n) { 
			n->parent->child[ _makeSelfie(n) ] = NULL;
		}

		void _deleteFixUp_2(node* child) { 
			node* n = child->parent;
			child->changeColor();
			child->parent = n->parent;
			if (n == root()) {
				_root = child;
				_lastNode->parent = _root;
			} else
				n->parent->child[ _makeSelfie(n) ] = child;
			child->parent = n->parent;
		}

		/* sibling of node is RED,
		swap sibling of node and parent of node colors,
		_rotate parent of node in node's direction,
		then check next case on same node */
		void _deleteFixUpCase1(node* n) {
			_getSibling(n)->changeColor(n->parent->getColor());
			n->parent->changeColor(RED);
			_rotate(n->parent, _makeSelfie(n));
			_deleteFixUpCase(n);
		}

		/* sibling of node is BLACK, far child is RED,
		swap sibling of node and parent's colors,
		change far child Color to BLACK,
		_rotate parent of node to node's direction. */
		void _deleteFixUpCase2(node* n) {
			_getSibling(n)->changeColor(n->parent->getColor());
			n->parent->changeColor(BLACK);
			_getFarChild(n)->changeColor(BLACK);
			_rotate(n->parent, _makeSelfie(n));
		}

		/* sibling of node is BLACK, far child is BLACK, close child is RED,
		swap close child and sibling of node colors,
		_rotate sibling of node in opposite direction to node,
		then call case 4. */
		void _deleteFixUpCase3(node* n) {
			_getCloseChild(n)->changeColor(BLACK);
			_getSibling(n)->changeColor(RED);
			_rotate(_getSibling(n), !_makeSelfie(n));
			_deleteFixUpCase4(n);
		}

		/* sibling of node subtree (sibling, far child, close child) is BLACK,
		change sibling of node color to RED,
		change parent of node color to BLACK,
		if parent of node was already BLACK check next case in parent of node. */
		void _deleteFixUpCase4(node* n) {
			_getSibling(n)->changeColor(RED);
			if (n->parent->getColor() == BLACK)
				_deleteFixUpCase(n->parent);
			else
				n->parent->changeColor();
			return;
		}


		int _makeSelfie(node* nd) const { 
			if (nd->parent->child[ LEFT ] == nd)
				return LEFT;
			return RIGHT;
		}

		bool _isInnerNode(node* nd) const { return (nd->child[ RIGHT ] && nd->child[ LEFT ]); }
		node* _getParent(node* nd) const { return nd->parent; }
		node* _getGrandParent(node* nd) const { return (_getParent(nd->parent)); }
		node* _getUncle(node* nd) const { return (_getGrandParent(nd)->child[ !_makeSelfie(nd->parent) ]); }
		node* _getSibling(node* nd) const { return (_getParent(nd)->child[ !_makeSelfie(nd) ]); }
		node* _getCloseChild(node* nd) const { return (_getSibling(nd)->child[ _makeSelfie(nd) ]); }
		node* _getFarChild(node* nd) const { return (_getSibling(nd)->child[ !_makeSelfie(nd) ]); }

		int _whatIsTheColor(node* nd) const {
			if (!nd || nd->getColor() == BLACK)
				return BLACK;
			return RED;
		}

		node* _copyTreeFrom(node* n_cpy) {
			if (!n_cpy)
				return NULL;
			node* _new_n = _tree_alloc.allocate(1);
			_tree_alloc.construct(_new_n, *n_cpy);
			_new_n->child[ LEFT ] = _copyTreeFrom(n_cpy->child[ LEFT ]);
			if (_new_n->child[ LEFT ])
				_new_n->child[ LEFT ]->parent = _new_n;
			_new_n->child[ RIGHT ] = _copyTreeFrom(n_cpy->child[ RIGHT ]);
			if (_new_n->child[ RIGHT ])
				_new_n->child[ RIGHT ]->parent = _new_n;
			return _new_n;
		}

		void _deleteTreeFrom(node* n_del) {
			if (!n_del)
				return ;
			_deleteTreeFrom(n_del->child[ LEFT ]);
			_deleteTreeFrom(n_del->child[ RIGHT ]);
			_tree_alloc.destroy(n_del);
			_tree_alloc.deallocate(n_del, 1);
		}
	};

	template<class T, class Compare, class Alloc>
	bool operator==(const ft::_Rb_tree< T, Compare, Alloc >& lhs, const ft::_Rb_tree< T, Compare, Alloc >& rhs) {
		if (lhs.size() != rhs.size())
			return false;
		return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<class T, class Compare, class Alloc>
	bool operator!=(const ft::_Rb_tree< T, Compare, Alloc >& lhs, const ft::_Rb_tree< T, Compare, Alloc >& rhs) {
		return (!(lhs == rhs));
	}

	template<class T, class Compare, class Alloc>
	bool operator<(const ft::_Rb_tree< T, Compare, Alloc >& lhs, const ft::_Rb_tree< T, Compare, Alloc >& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class T, class Compare, class Alloc>
	bool operator<=(const ft::_Rb_tree< T, Compare, Alloc >& lhs, const ft::_Rb_tree< T, Compare, Alloc >& rhs) {
		return (lhs == rhs || lhs < rhs);
	}

	template<class T, class Compare, class Alloc>
	bool operator>(const ft::_Rb_tree< T, Compare, Alloc >& lhs, const ft::_Rb_tree< T, Compare, Alloc >& rhs) {
		return (rhs < lhs);
	}

	template<class T, class Compare, class Alloc>
	bool operator>=(const ft::_Rb_tree< T, Compare, Alloc >& lhs, const ft::_Rb_tree< T, Compare, Alloc >& rhs) {
		return (lhs > rhs || lhs == rhs);
	}
}

#endif
