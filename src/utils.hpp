/*
ABOUT:
	lexicographical_compare - https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/container/vector.html
	type_traits				- https://cplusplus.com/reference/type_traits/integral_constant/
	enable_if				- https://en.cppreference.com/w/cpp/types/enable_if
							- https://cplusplus.com/reference/type_traits/enable_if/
	is_integral				- https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/types/integral_constant.html
	equal					- https://en.cppreference.com/w/cpp/algorithm/equal
	lexico					- https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
*/

#ifndef UTILS_HPP
#define UTILS_HPP

namespace  ft {

// The type T is enabled as member type enable_if::type if Cond is true
// Otherwise, enable_if::type is not defined.
	template< bool B, class T = void >
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> {
		typedef T type;
	};


//Trait class that identifies whether T is an integral type.
	template<class T, bool v>
	struct integral_constant {
		static const bool value = v;
		typedef T value_type;
		typedef integral_constant type;
		operator value_type() const { return value; }
	};

	template <class T>
	struct is_integral : public integral_constant<T, false> {};
// specifications:
		template <> struct is_integral<bool> : public integral_constant<bool, true> {};
		template <> struct is_integral<char> : public integral_constant<bool, true> {};
		template <> struct is_integral<signed char> : public integral_constant<bool, true> {};
		template <> struct is_integral<unsigned char> : public integral_constant<bool, true> {};
		template <> struct is_integral<wchar_t> : public integral_constant<bool, true> {};
		template <> struct is_integral<short> : public integral_constant<bool, true> {};
		template <> struct is_integral<unsigned short> : public integral_constant<bool, true> {};
		template <> struct is_integral<int> : public integral_constant<bool, true> {};
		template <> struct is_integral<unsigned int> : public integral_constant<bool, true> {};
		template <> struct is_integral<long> : public integral_constant<bool, true> {};
		template <> struct is_integral<unsigned long> : public integral_constant<bool, true> {};
		template <> struct is_integral<long long> : public integral_constant<bool, true> {};
		template <> struct is_integral<unsigned long long> : public integral_constant<bool, true> {};


	template<class T>
	struct is_const : public integral_constant<bool, false> {};

	template<class T>
	struct is_const<const T> : public integral_constant<bool, true> {};

	template<class T>
	struct is_pointer : public integral_constant<bool, false> {};

	template<class T>
	struct is_pointer<T*> : public  integral_constant<bool, true> {};

	template<class T>
	struct is_pointer<T* const> : public integral_constant<bool, true> {};


	template<class InputIt1, class InputIt2 >
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}

// BinaryPredicate indicates whether the elements are considered to match in the context of this function
	template<class InputIt1, class InputIt2, class BinaryPredicate >
	bool equal(InputIt1 first1, InputIt1 last1,
				InputIt2 first2, BinaryPredicate p) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!p(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}


//	TRUE if [first1, last1] lexicographically less [first2, last2]
	template<class InputIt1, class InputIt2>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2)
	{
		for ( ; (first1 != last1) && (first2 != last2); first1++, first2++ ) {
			if (*first1 < *first2) return true;
			if (*first2 < *first1) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template<class InputIt1, class InputIt2, class Compare>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2,
								Compare comp)
	{
		for ( ; (first1 != last1) && (first2 != last2); first1++, first2++ ) {
			if (comp(*first1, *first2)) return true;
			if (comp(*first2, *first1)) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}
}

#endif
