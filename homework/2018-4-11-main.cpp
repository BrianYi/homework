/*
 *	array,vector,deque,string		                                                随机迭代器
 *	list,set,multiset,map,multimap	                                                双向迭代器
 *	forward_list,unordered_set,unordered_multiset,unordered_map,unordered_multimap	前向迭代器
 *	
 */
//#include <iterator>
//#include <vector>
//#include <unordered_map>
//#include <unordered_set>
//#include <string>
//#include <iostream>
//#include <forward_list>
//using namespace std;
//
//int main()
//{
//	vector<int> coll{ 1,2,3,4 };
//	auto iter          = ++coll.begin();
//	iter + 1;
//	while (next(iter) != coll  .end())
//	{
//		cout << *iter << " ";
//		advance(iter, 1);
//	}
//	
//	forward_list<int> coll2;
//	auto iter2 = coll2.begin();
//	iter2 + 1;						// +,-只可以用于random iterator，对于除random iterator以外的迭代器可以用next，prev来达到效果
//	next(iter2);                    
//	list<int> coll3;
//	auto iter3 = coll3.begin();
//	iter3 + 1;
//	cout << endl;
//	return 0;
//}


/*
 *	reverse_iterator
 */
//#include <iostream>
//#include <deque>
//#include <iterator>
//#include <algorithm>
//
//using namespace std;
//
//auto print = [](int elem)->void {cout << elem << ' '; };
//
//int main()
//{
//	deque<int> coll = { 1,2,3,4,5,6,7,8,9 };
//	deque<int>::const_iterator pos1;
//	pos1 = find(coll.cbegin(), coll.cend(), 2);
//
//	deque<int>::const_iterator pos2;
//	pos2 = find(coll.cbegin(), coll.cend(), 7);
//
//	for_each(pos1, pos2, print);
//	cout << endl;
//
//	deque<int>::const_reverse_iterator rpos1(pos1);
//	deque<int>::const_reverse_iterator rpos2(pos2);
//	if (pos1 == rpos1.base())
//		cout << "pos1 == rpos1.base()" << endl;
//	if (pos2 == rpos2.base())
//		cout << "pos2 == rpos2.base()" << endl;
//	for_each(rpos2, rpos1, print);
//	cout << endl;
//	return 0;
//}

/*
 *	ostream_iterator
 */
//#include <iostream>
//#include <iterator>
//#include <list>
//#include <sstream>
//#include <numeric>
//using namespace std;
//
//int main()
//{
//	ostream_iterator<int> intWriter(cout, " < ");
//
//	*intWriter = 42;
//	*intWriter = 77;
//	*intWriter = -5;
//	cout << endl;
//
//	list<int> coll{ 1,2,3,4,5 };
//	copy(coll.begin(), coll.end(), intWriter);
//	cout << endl;
//
//	istream_iterator<int> in_iter_end();	    // end-of-stream iterator
//	//istream_iterator<int> in_iter(cin);		    // istream iterator
//
//	istringstream str("0.1 0.2 0.3 0.4");
//	partial_sum(istream_iterator<double>(str), istream_iterator<double>(), ostream_iterator<double>(cout, " "));
//	cout << endl;
//	return 0;
//}