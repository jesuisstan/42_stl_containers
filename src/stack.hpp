/*
ABOUT:
	stack - https://en.cppreference.com/w/cpp/container/stack
*/

#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft {
	
	template <class T, class Container = ft::vector<T> >
	class stack {
		Container _container;
	public:
		typedef	Container											container_type;
		typedef typename Container::value_type						value_type;
		typedef typename Container::size_type						size_type;
		typedef typename Container::reference						reference;
		typedef typename Container::const_reference					const_reference;

		explicit stack(const Container &cont = Container())			{ _container = cont; };
		stack(const stack &other) 									{ _container = other._container; };
		~stack() 													{};
		stack& operator=(const stack &other) {
			if (this == &other)
				return *this;
			_container = other._container;
			return *this;
		};
		reference top()												{ return _container.back(); };
		const_reference top() const									{ return _container.back(); };
		bool empty() const 											{ return _container.empty(); };
		size_type size() const 										{ return _container.size(); };
		void push(const value_type &value) 							{ _container.push_back(value); };
		void pop()													{ _container.pop_back(); };
		friend bool operator==(const stack &lhs, const stack &rhs)	{ return lhs._container == rhs._container; };
		friend bool operator!=(const stack &lhs, const stack &rhs)	{ return lhs._container != rhs._container; };
		friend bool operator<=(const stack &lhs, const stack &rhs)	{ return lhs._container <= rhs._container; };
		friend bool operator>=(const stack &lhs, const stack &rhs)	{ return lhs._container >= rhs._container; };
		friend bool operator<(const stack &lhs, const stack &rhs)	{ return lhs._container < rhs._container; };
		friend bool operator>(const stack &lhs, const stack &rhs)	{ return lhs._container > rhs._container; };
	};
}

#endif
