/*
 *	stream iterator, 使用advance
 */
//#include <iostream>
//#include <iterator>
//#include <string>
//#include <algorithm>
//
//using namespace std;
//
//int main()
//{
//	istream_iterator<string> cinPos(cin);
//	ostream_iterator<string> coutPos(cout, " ");
//
//	while (cinPos != istream_iterator<string>())
//	{
//		advance(cinPos, 2);
//
//		if (cinPos != istream_iterator<string>())
//			*coutPos = *cinPos;	// or *coutPos++ = *cinPos++
//	}
//
//	return 0;
//}

/*
 *	move iterator
 */
//#include <iostream>
//#include <iterator>
//#include <string>
//#include <vector>
//
//using namespace std;
//
//int main()
//{
//	vector<string> coll{ "hello","how","are","you" },coll2;
//	copy(make_move_iterator(coll.begin()), make_move_iterator(coll.end()), back_inserter(coll2));
//	for (auto& elem : coll2)
//		cout << elem << " ";
//	cout << endl;
//
//	for (auto& elem : coll)
//		cout << elem << " ";
//	cout << endl;
//
//	return 0;
//}


/*
 *	iterator trait
 */

//template<typename ForwardIterator>
//void shift_left(ForwardIterator beg, ForwardIterator end)
//{
//	typedef typename iterator_traits<ForwardIterator>::value_type value_type;
//
//	if (beg != end)
//	{
//		value_type tmp(*beg);
//
//	}
//}
//
//template <typename Iterator>
//typename iterator_traits<Iterator>::difference_type distance(Iterator pos1, Iterator pos2)
//{
//	return distance(pos1, pos2, typename iterator_traits<Iterator>::iterator_category());
//}
//
//// for random iterator
//template <typename RndIterator>
//typename iterator_traits<RndIterator>::difference_type distance(RndIterator pos1, RndIterator pos2, random_access_iterator_tag)
//{
//	return pos2 - pos1;
//}
//
//// for other iterator
//template <typename InIterator>
//typename iterator_traits<InIterator>::difference_type distance(InIterator pos1, InIterator pos2, input_iterator_tag)
//{
//	typename iterator_traits<InIterator>::difference_type d;
//	for (d = 0; pos1 != pos2; ++pos1, ++d) {}
//	return d;
//}


/*
 *	自定义迭代器类型
 */
//#include <iostream>
//#include <iterator>
//
//using namespace std;
//
//template <typename Container>
//class asso_insert_iterator : public iterator<bidirectional_iterator_tag, typename Container::value_type>
//{
//protected:
//	Container& container;
//public:
//	explicit asso_insert_iterator(Container& c) :container(c) {}
//	asso_insert_iterator<Container>& operator= (const typename Container::value_type& value)
//	{
//		container.insert(value);
//		return *this;
//	}
//
//	asso_insert_iterator<Container>& operator*() { return *this; }
//
//	asso_insert_iterator<Container>& operator++() { return *this; }
//
//	asso_insert_iterator<Container>& operator++(int) { return *this; }
//
//};
//
//template<typename Container>
//inline asso_insert_iterator<Container> asso_inserter(Container& c)
//{
//	return asso_insert_iterator<Container>(c);
//}

/*
 *	function object 和 binder
 */
//#include <functional>
//#include <iostream>
//#include <iterator>
//#include <algorithm>
//#include <string>
//#include <vector>
//using namespace std::placeholders;
//using namespace std;
//
//char myToupper(char c)
//{
//	locale loc;
//	return use_facet<ctype<char>>(loc).toupper(c);
//}
//
//class Person
//{
//	string name;
//public:
//	Person(const string& n) :name(n) {}
//	void print()const { cout << name << endl; }
//	void print2(const string& prefix) const { cout << prefix << name << endl; }
//};
//
//int main()
//{
//	cout << "==========example one(绑定内部function object)==========" << endl;
//	cout << "10 x 6 / 3 + sqr(2): " << bind(plus<int>(), bind(divides<int>(), bind(multiplies<int>(), 10, 6), 3), bind(multiplies<int>(),2,2))() << endl;
//	cout << endl;
//
//	cout << "==========example two(绑定全局函数)==========" << endl;
//	string s("Internationalization");
//	string sub("Nation");
//
//	string::iterator pos;
//	pos = search(s.begin(), s.end(), sub.begin(), sub.end(), bind(equal_to<char>(), bind(myToupper, _1), bind(myToupper, _2)));
//	// bind(equal_to<char>(), bind(myToupper, _1), bind(myToupper, _2))
//	// 等价于 myToupper(param1) == myToupper(param2)
//	if (pos != s.end())
//		cout << "\"" << sub << "\" is part of \"" << s << "\"" << endl;
//
//	cout << "==========example three(绑定成员函数)==========" << endl;
//	vector<Person> coll{ Person("Tick"),Person("Trick"),Person("Track") };
//
//	for_each(coll.begin(), coll.end(), bind(&Person::print, _1));
//	cout << endl;
//
//	for_each(coll.begin(), coll.end(), bind(&Person::print2, _1, "Person: "));
//	bind(&Person::print2, _1, "bind onece: ")(Person("nico"));
//
//	return 0;
//}


/*
 *	generic and numeric algorithmn
 */
//#include <iostream>
//#include <algorithm>
//#include <iterator>
//#include <list>
//#include <string>
//#include <sstream>
//#include <numeric>
//#include <iomanip>
//using namespace std;
//
//template <typename Container>
//void print_container(Container& c, string endStr = "")
//{
//	for (const auto& elem : c)
//		cout << elem << " ";
//	cout << endStr;
//}
//
//int main()
//{
//	// for_each
//	list<string> coll{ "0.1 0.3 0.62", "1 0.5 0.5 0.03", "1 2 3 0.6" };
//	for_each(coll.begin(), coll.end(), [](const auto& elem) { 
//		istringstream str(elem);
//		cout << elem << ": " << accumulate(istream_iterator<double>(str), istream_iterator<double>(), 0.0) << endl;
//	});
//
//	// count_if
//	cout << "contains value which is bigger than 1: " << count_if(coll.begin(), coll.end(), [](const auto& elem) {
//		istringstream str(elem);
//		istream_iterator<double> iter(str);
//		istream_iterator<double> iter_end;
//		while (iter != iter_end)
//		{
//			if (*iter >= 1)
//				return true;
//			advance(iter, 1);
//		}
//		return false;
//	}) << endl;
//
//	// minmax_element
//	for_each(coll.begin(), coll.end(), [](const auto& elem) {
//		istringstream str(elem);
//		istream_iterator<double> iter(str);
//		istream_iterator<double> iter_end;
//		auto minmax_elem = minmax_element(iter, iter_end);
//		cout << setiosflags(ios::left);
//		cout << setw(20) << elem
//			<< setw(10) << "min is " << setw(10) << *minmax_elem.first
//			<< setw(10) << "max is " << setw(10) << *minmax_elem.second << endl;
//	});
//
//	// generate
//	generate(coll.begin(), coll.end(), []() { return "asd"; });
//	print_container(coll, "\n");
//
//
//	return 0;
//}