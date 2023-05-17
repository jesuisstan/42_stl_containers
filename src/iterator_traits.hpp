/*
ABOUT:
	iterator tags	-	https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/iterator/iterator_tags.html
	iterator_traits	-	https://en.cppreference.com/w/cpp/iterator/iterator_traits

std::iterator_traits is the type trait class that provides uniform interface
to the properties of LegacyIterator types.
This makes it possible to implement algorithms only in terms of iterators.
*/

#ifndef ITERATOR_HPP
#define ITERATOR_HPP

namespace ft {

	struct	input_iterator_tag {};
	struct	output_iterator_tag {};
	struct	forward_iterator_tag : public input_iterator_tag {};
	struct	bidirectional_iterator_tag : public forward_iterator_tag {};
	struct	random_access_iterator_tag : public bidirectional_iterator_tag {};


	template<class Iterator>
	class iterator_traits {
	public:
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::iterator_category	iterator_category;
	};

	template<class T>
	class iterator_traits<T*> {
	public:
		typedef T										value_type;
		typedef T*										pointer;
		typedef T&										reference;
		typedef std::ptrdiff_t							difference_type;
		typedef ft::random_access_iterator_tag			iterator_category;
	};

	template<class T>
	class iterator_traits<const T*> {
	public:
		typedef T										value_type;
		typedef const T*								pointer;
		typedef const T&								reference;
		typedef std::ptrdiff_t							difference_type;
		typedef ft::random_access_iterator_tag			iterator_category;
	};


	template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
	class iterator {
	public:
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};


	template<class InputIterator>
	typename ft::iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last) {
		typename ft::iterator_traits<InputIterator>::difference_type ret = 0;
		for(; first != last; first++, ret++);
		return ret;
	};
}

#endif
