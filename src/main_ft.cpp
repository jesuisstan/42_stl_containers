#include "vector.hpp"
#include "map.hpp"
#include "stack.hpp"
#include "set.hpp"

#include <iostream>
#include <sys/time.h>

# define GREEN "\e[92m"
# define YELLOW "\e[93m"
# define BOLD "\e[1m"
# define USCORED "\x1b[4m"
# define RESET "\e[0m"

long long g_timestamp;

static long long get_timestamp(void) {
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec + (long long)tv.tv_sec * 1000000);
}

template <typename V>
static void printVec(V vec) {
	std::cout << GREEN << "{ " << RESET;
	for (size_t i = 0; i != vec.size(); ++i)
		std::cout << vec[i] << " ";
	std::cout << GREEN << "}" << RESET << std::endl;
}

template <typename M>
static void printMap(M container) {
	std::cout << GREEN << "{ " << RESET;
	typename M::iterator it = container.begin();
	typename M::iterator ite = container.end();
	for (; it != ite; ++it) {
		std::cout << "[" << it->first << ", " << it->second << "]; " ;
	}
	std::cout << GREEN << "}" << RESET << std::endl;
}

template <typename M>
static void printMapRev(M container) {
	std::cout << GREEN << "{ " << RESET;
	typename M::reverse_iterator it = container.rbegin();
	typename M::reverse_iterator ite = container.rend();
	for (; it != ite; ++it) {
		std::cout << "[" << it->first << ", " << it->second << "]; " ;
	}
	std::cout << GREEN << "}" << RESET << std::endl;
}

int main() {

	{
		std::cout << YELLOW BOLD << "----------- VECTOR TESTING -----------\n" << RESET;
		g_timestamp = get_timestamp();

		ft::vector<int> vec_my(5, 7);
		std::cout << "\nis vector empty? --> " << vec_my.empty();
		std::cout << "\nvector size = " << vec_my.size();
		std::cout << "\nvector capacity = " << vec_my.capacity();
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest range constructor:\n" << RESET;
		ft::vector<int> vec_my_new(vec_my.begin(), vec_my.end() - 2);
		std::cout << "\nis new vector empty? --> " << vec_my_new.empty();
		std::cout << "\nnew vector size = " << vec_my_new.size();
		std::cout << "\nnew vector capacity = " << vec_my_new.capacity();
		std::cout << "\nnew vector containes: ";
		printVec(vec_my_new);

		std::cout << USCORED << "\ntest vector.clear():\n" << RESET;
		vec_my.clear();
		std::cout << "\nvector size = " << vec_my.size();
		std::cout << "\nvector capacity = " << vec_my.capacity();
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.assign(10, 42):\n" << RESET;
		vec_my.assign(10, 42);
		std::cout << "\nvector size = " << vec_my.size();
		std::cout << "\nvector capacity = " << vec_my.capacity();
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.push_back(...):\n" << RESET;
		vec_my.push_back(21);
		vec_my.push_back(87);
		vec_my.push_back(99);

		std::cout << "\nvector size = " << vec_my.size();
		std::cout << "\nvector capacity = " << vec_my.capacity();
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.pop_back():\n" << RESET;
		vec_my.pop_back();
		std::cout << "\nvector size = " << vec_my.size();
		std::cout << "\nvector capacity = " << vec_my.capacity();
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.reserve(42):\n" << RESET;
		vec_my.reserve(42);
		std::cout << "\nvector size = " << vec_my.size();
		std::cout << "\nvector capacity = " << vec_my.capacity();
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.at(10):\n" << RESET;
		std::cout << "vector.at(10) = ";
		std::cout << vec_my.at(10);
		std::cout << std::endl;

		std::cout << USCORED << "\ntest vector[7]:\n" << RESET;
		std::cout << "vector[7] = ";
		std::cout << vec_my[7];
		std::cout << std::endl;

		std::cout << USCORED << "\ntest vector.insert(iterator pos, const_reference value) {vec.insert(it + 5, 666)}:\n" << RESET;
		ft::vector<int>::iterator it = vec_my.begin();
		vec_my.insert(it + 5, 666);
		std::cout << "\nvector containes: ";
		printVec(vec_my);
		
		std::cout << USCORED << "\ntest vector.insert(iterator pos, count - 3, what - 777):\n" << RESET;
		vec_my.insert(it, 3, 777);
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.insert(iterator pos, InputIt first, InputIt last):\n" << RESET;
		ft::vector<int> vec_to_copy_from_ft;
		vec_to_copy_from_ft.push_back(11);
		vec_to_copy_from_ft.push_back(33);
		vec_to_copy_from_ft.push_back(222);
		vec_to_copy_from_ft.push_back(7);
		vec_my.insert(it + 1, vec_to_copy_from_ft.begin(), vec_to_copy_from_ft.begin() + 3);
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest const_iterator vs. iterator:\n" << RESET;
		ft::vector<int>::const_iterator cit = vec_my.begin();
		if (cit >= it)
			std::cout << "\nvector iterators are comparable\n";
		ft::vector<int>::const_iterator cit2 = it;
		std::cout << "printing the 2nd element with const_iterator: " << *++cit2 << std::endl;

		std::cout << USCORED << "\ntesting iterator's operators +-...:\n" << RESET;

		ft::vector<int> test;
		for (size_t i = 10; i > 0; --i)
			test.push_back(i);
		std::cout << "test vector containes: ";
		printVec(test);
		ft::vector<int>::iterator itt = test.begin() + 5;
		std::cout << "it = vec.begin() + 5: " << *itt << std::endl;
		std::cout << "*(it + 3): " << *(itt + 3) << " / *it = " << *(itt) << std::endl;
		std::cout << "*it + 3: " << *itt + 3 << " / *it = " << *(itt) << std::endl;
		std::cout << "*(it - 2): " << *(itt - 2) << " / *it = " << *(itt) << std::endl;
		std::cout << "*it - 2: " << *itt - 2 << " / *it = " << *(itt) << std::endl;
		std::cout << "it += 3: " << *(itt += 3) << " / *it = " << *(itt) << std::endl;
		std::cout << "it -= 2: " << *(itt -= 2) << " / *it = " << *(itt) << std::endl;
		std::cout << "it++: " << *(itt++) << " / *it = " << *(itt) << std::endl;
		std::cout << "--it: " << *(--itt) << " / *it = " << *(itt) << std::endl;

		std::cout << USCORED << "\ntesting reverse iterators:\n" << RESET;
		ft::vector<int>::reverse_iterator rit = vec_my.rbegin();
		std::cout << "rbegin() = " << *rit << std::endl;
		ft::vector<int>::reverse_iterator rit2 = vec_my.rend();
		std::cout << "printing vector with reverse_iterator:";
		for(; rit != rit2; ++rit)
			std::cout << ' ' << *rit;
		std::cout << std::endl;
		
		std::cout << USCORED << "\ntest vector.erase:\n" << RESET;
		vec_my.erase(it + 1);
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.erase(iterator first, iterator last (3th)):\n" << RESET;
		vec_my.erase(it, it + 2);
		std::cout << "\nvector containes: ";
		printVec(vec_my);

		std::cout << USCORED << "\ntest vector.front():\n" << RESET;
		vec_my.front();
		std::cout << "vector front = " << vec_my.front() << std::endl;

		std::cout << USCORED << "\ntest vector.back():\n" << RESET;
		vec_my.front();
		std::cout << "vector back = " << vec_my.back() << std::endl;

		{
			std::cout << USCORED << "\ntest reverse_iterator VS. const_reverse_iterator:\n" << RESET;
			const int size = 5;
			ft::vector<int> vct(size);
			ft::vector<int>::reverse_iterator it = vct.rbegin();
			ft::vector<int>::const_reverse_iterator ite = vct.rbegin();

			for (int i = 0; i < size; ++i)
				it[i] = (size - i) * 5;

			it = it + 5;
			it = 1 + it;
			it = it - 4;
			std::cout << *(it += 2) << std::endl;
			std::cout << *(it -= 1) << std::endl;

			*(it -= 2) = 42;
			*(it += 2) = 21;

			std::cout << "const_ite +=/-=: " << *(ite += 2) << " | " << *(ite -= 2) << std::endl;
			std::cout << "(it == const_it): " << (ite == it) << std::endl;
			std::cout << "(const_ite - it): " << (ite - it) << std::endl;
			std::cout << "(ite + 3 == it): " << (ite + 3 == it) << std::endl;
			std::cout << "\nvector containes: ";
			printVec (vct);
		}

		std::cout << GREEN << "\ntotal time spent on ft::vector testing = " << get_timestamp() - g_timestamp << " μs" << RESET << std::endl;;

	}



	{
		std::cout << YELLOW BOLD << "----------- MAP TESTING -----------\n" << RESET;
		g_timestamp = get_timestamp();

		{	
			std::cout << USCORED << "\ntest filling map with []:\n" << RESET;
			ft::map<int, std::string> map_orig;
			map_orig[0] = "qqq";
			map_orig[1] = "aaa";
			map_orig[2] = "bbb";
			map_orig[5] = "ccc";
			std::cout << "map size = " << map_orig.size() << std::endl;
			std::cout << "map empty? = " << map_orig.empty() << std::endl;
			std::cout << "map maxsize = " << map_orig.max_size() << std::endl;
			std::cout << "\nmap containes: ";
			printMap(map_orig);
			std::cout << "\nreversed map containes: ";
			printMapRev(map_orig);
			std::cout << map_orig.at(1) << std::endl;
			//std::cout << map_orig.at(66) << std::endl;

			ft::map<int, std::string>::iterator it = map_orig.begin();
			it++;
			++it;
			std::cout << it->first << "\n";
		}

		{	
			std::cout << USCORED << "\ntest insert pair:\n" << RESET;
			ft::pair<int, std::string>			my_pair(8, "salut");
			ft::map<int, std::string>			test;

			test.insert(my_pair);
			test.insert(ft::pair<int, std::string>(-4, "cyber"));
			test.insert(ft::pair<int, std::string>(2, "punk"));
			test.insert(ft::pair<int, std::string>(3, "ain't"));
			test.insert(ft::pair<int, std::string>(746, "dead"));
			test.insert(ft::pair<int, std::string>(1, "yet"));
			std::cout << "\nmap containes: ";
			printMap(test);
			std::cout << "\nreversed map containes: ";
			printMapRev(test);
		}

		{
			std::cout << USCORED << "\ntest erase:\n" << RESET;
			ft::pair<int, std::string>			my_pair(8, "salut");
			ft::map<int, std::string>			mp;

			mp.insert(my_pair);
			mp.insert(ft::pair<int, std::string>(-4, "cyber"));
			mp.insert(ft::pair<int, std::string>(2, "punk"));
			mp.insert(ft::pair<int, std::string>(3, "ain't"));
			mp.insert(ft::pair<int, std::string>(746, "dead"));
			mp.insert(ft::pair<int, std::string>(1, "yet"));
			std::cout << "\nmap containes: ";
			printMap(mp);

			mp.erase(++mp.begin());
			std::cout << "\nafter mp.erase(++mp.begin()) map containes: ";
			printMap(mp);

			mp.erase(mp.begin());
			mp.erase(--mp.end());
			std::cout << "\nafter mp.erase(mp.begin()) && mp.erase(--mp.end()) map containes: ";
			printMap(mp);

			mp.erase(mp.begin(), ++(++mp.begin()));
			std::cout << "\nafter mp.erase(mp.begin(), ++(++mp.begin())) map containes: ";
			printMap(mp);

			mp[12] = "ONE";
			mp[13] = "TWO";
			mp[14] = "THREE";
			mp[15] = "FOUR";
			std::cout << "\nafter filling in refrashed map containes: ";
			printMap(mp);
			mp.erase(mp.begin(), mp.end());
			std::cout << "\nafter mp.erase(mp.begin(), mp.end()) map containes: ";
			printMap(mp);
		}

		{
			std::cout << USCORED << "\ntest reverse iterator:\n" << RESET;
			ft::pair<int, std::string>			my_pair(-42, "salut");
			ft::map<int, std::string>			mp;

			mp.insert(my_pair);
			mp.insert(ft::pair<int, std::string>(-4, "cyber"));
			mp.insert(ft::pair<int, std::string>(1, "punk"));
			mp.insert(ft::pair<int, std::string>(3, "ain't"));
			mp.insert(ft::pair<int, std::string>(21, "dead"));
			mp.insert(ft::pair<int, std::string>(746, "yet"));
			std::cout << "\nmap containes: ";
			printMap(mp);
			std::cout << "\nUsing reverse iterator map containes: ";
			printMapRev(mp);
		}


		std::cout << GREEN << "\ntotal time spent on ft::map testing = " << get_timestamp() - g_timestamp << " μs" << RESET << std::endl;;

}


	{
		std::cout << "----------- STACK TESTING -----------" << std::endl;
		g_timestamp = get_timestamp();

		ft::stack<int> stack_my;
		stack_my.push(1);
		stack_my.push(42);
		stack_my.push(0);
		stack_my.push(21);
		std::cout << USCORED << "\ntest stack.empty():\n" << RESET;
		std::cout << USCORED << "Is stack empty? -> " << std::boolalpha << stack_my.empty() << RESET << std::endl;
		
		std::cout << USCORED << "\ntest stack.pop() & top():\n" << RESET;
		std::cout << "stack top before pop = " << stack_my.top() << std::endl;
		stack_my.pop();
		stack_my.pop();
		std::cout << "stack top after pop = " << stack_my.top() << std::endl;
		
		std::cout << GREEN << "\ntotal time spent on ft::stack testing = " << get_timestamp() - g_timestamp << " μs" << RESET << std::endl;;
		
	}


	{
		std::cout << "----------- SET TESTING -----------" << std::endl;
		g_timestamp = get_timestamp();

	std::cout << "\ntest constructors and erasing\n";
	{
		ft::set<int, std::greater<int> >	s0;
		ft::set<int, std::less<int> >		s;

		std::cout << "empty set created size=" << s0.size() << "\n";
		for (int i = 0; i < 5; i++)
			s0.insert(i);
		std::cout << "fill set newsize=" << s0.size() << "\n\n";

		std::cout << "creating set with different Compare function\n";
		s.insert(s0.begin(), s0.end());
		std::cout << "first element of set<GREATER>: " << *s0.begin() << "\n";
		std::cout << "first element of set<LESS>: " << *s.begin() << "\n\n";

		ft::set<int, std::greater<int> >	s1(s0);
		std::cout << "copy set created size=" << s1.size() << "\n\n";

		std::cout << "...erasing copy to check deep copying...\n";
		s1.erase(s1.begin(), s1.end());
		std::cout << "after erasing copy_size=" << s1.size() << " original set size=" << s0.size() << "\n\n";

		ft::set<int, std::greater<int> >	s2 = s0;
		std::cout << "assigning set created size=" << s2.size() << "\n";
		std::cout << "...erasing set to check deep copying...\n";
		s2.erase(s2.begin(), s2.end());
		std::cout << "after erasing copy_size=" << s2.size() << " original set size=" << s0.size() << "\n\n";

		s0 = s2;
		std::cout << "assigning empty set to original set\n";
		std::cout << "after assigning copy_size=" << s0.size() << "\n\n";
	}

	std::cout << "\ntest insert and capacity\n";
	{
		ft::set<int>	s0;
		std::cout << "empty map created size=" << s0.size();
		std::cout << " empty=" << (s0.empty() ? "true" : "false") << "\n";

		for (int i = 0; i < 10000; i += 2)
			s0.insert(i);

		std::cout << "filled map newsize=" << s0.size();
		std::cout << " empty=" << (s0.empty() ? "true" : "false") << "\n";

		for (int i = 0; i < 10000; i++)
			s0.insert(i);
		std::cout << "filled map newsize=" << s0.size();
		std::cout << " empty=" << (s0.empty() ? "true" : "false") << "\n";

	}

	std::cout << "\ntest access and iterators\n";
	{
		ft::set<int>	s0;

		for (int i = 0; i < 20; i += 2)
			s0.insert(i);

		std::cout << "created non empty map, random access iterator \n";
		ft::set<int>::iterator	it = s0.begin();
		for (; it != s0.end(); ++it)
			std::cout << *it << " ";

		std::cout << "\niteration with reverse iterator \n";
		ft::set<int>::reverse_iterator	it2 = ++s0.rbegin();
		for (; it2 != s0.rend(); ++it2)
			std::cout << *it2 << " ";
		std::cout << "\n";

	}

	std::cout << "\ntest count()\n";
	{
		 ft::set<int> t;
		 t.insert(3);
		 t.insert(1);
		 t.insert(4);
		 t.insert(1);
		 t.insert(5);
		 std::cout << t.count(1) << ", " << t.count(2) << ".\n";
	}

	std::cout << "\ntest equal_range()\n";
	{
		ft::set<int> myset;
		for (int i=1; i<=5; i++) myset.insert(i*10);	// 10 20 30 40 50
		ft::pair<ft::set<int>::const_iterator,ft::set<int>::const_iterator> ret;
		ret = myset.equal_range(30);
		std::cout << "the lower bound points to: " << *ret.first << '\n';
		std::cout << "the upper bound points to: " << *ret.second << '\n';
	}

		std::cout << GREEN << "\ntotal time spent on ft::set testing = " << get_timestamp() - g_timestamp << " μs" << RESET << std::endl;;
	}

	return 0;
}
