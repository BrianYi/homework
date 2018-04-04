/*
 *	Reverse Iterator(反向迭代器)
 *	1. operator++(为原operator--)
 *	2. operator--(为原operator++)
 */

// #include <iostream>
// #include <iterator>
// using namespace std;
// 
// int main()
// {
// 	int a[] = { 1,3,2,4,6,5 };
// 	reverse_iterator<int*> riter = rbegin(a);
// 	reverse_iterator<int*> riter_end = rend(a);
// 	while (riter != riter_end)
// 		cout << *riter++ << " ";
// 	cout << endl;
// 	
// 	return 0;
// }

/*
 *	remove用法：只是移动元素(remove vt.移动，迁移)，返回移动后的最后元素位置
 *	真正删除的话请用erase
 */
// #include <iostream>
// #include <iterator>
// #include <list>
// #include <algorithm>
// using namespace std;
// 
// int main()
// {
// 	list<int> coll;
// 	for (int i = 1; i <= 6; ++i)
// 	{
// 		coll.push_front(i);
// 		coll.push_back(i);
// 	}
// 
// 	cout << "pre: ";
// 	copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 
// 	auto it_end = remove(coll.begin(), coll.end(), 3);
// 
// 	cout << "post: ";
// 	copy(coll.begin(), it_end, ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 
// 	cout << "distance: " << distance(it_end, coll.end()) << endl;
// 
// 	return 0;
// }

/*
 *	关联型和无序型容器移除元素的方法
 */
// #include <iostream>
// #include <set>
// #include <unordered_set>
// #include <iterator>
// #include <algorithm>
// using namespace std;
// 
// int main()
// {
// 	cout << "==========unordered set (erase)==========" << endl;
// 	unordered_set<int> unorderedSet{ 1,3,2,5,6,7,8,9 };
// 	
// 	copy(unorderedSet.cbegin(), unorderedSet.cend(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 
// 	int num = unorderedSet.erase(3);
// 	cout << "number of removed elements: " << num << endl;
// 	
// 	copy(unorderedSet.cbegin(), unorderedSet.cend(), ostream_iterator<int>(cout, " "));
// 	cout << endl << endl;
// 
// 
// 	cout << "==========ordered set (erase)==========" << endl;
// 	set<int> orderedSet{ 1,3,2,5,6,7,8,9 };
// 
// 	copy(orderedSet.cbegin(), orderedSet.cend(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 
// 	num = orderedSet.erase(3);
// 	cout << "number of removed elements: " << num << endl;
// 
// 	copy(orderedSet.cbegin(), orderedSet.cend(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 	return 0;
// }

/*
 *	list remove
 *	顺序容器，remove只是单纯的移动元素(所以通常和erase搭配)
 *	xx.erase(remove(),xx.end());
 */
// #include <list>
// #include <iostream>
// #include <algorithm>
// #include <iterator>
// using namespace std;
// 
// int main()
// {
// 	list<int> coll;
// 
// 	for (int i = 1; i <= 6; ++i)
// 	{
// 		coll.push_front(i);
// 		coll.push_back(i);
// 	}
// 
// 	//coll.remove(4); // good performance
// 	coll.erase(remove(coll.begin(), coll.end(), 3), coll.end()); // bad performance
// 	copy(coll.begin(), coll.end(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 	return 0;
// }


/*
 *	transform
 */
// #include <algorithm>
// #include <iostream>
// #include <list>
// #include <iterator>
// using namespace std;
// 
// int main()
// {
// 	list<int> lst{ 5,1,3,2,4 }, lst2;
// 	copy(lst.begin(), lst.end(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 
// 	transform(lst.begin(), lst.end(), back_inserter(lst2), [](int val)->int { return val * val; });
// 	copy(lst2.begin(), lst2.end(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 	return 0;
// }


/*
 *	unary predicate(单参判断式) 
 *	binary predicate(双参判断式)
 */
// #include <vector>
// #include <iostream>
// #include <algorithm>
// #include <iterator>
// using namespace std;
// 
// struct DoubleElem
// {
// 	DoubleElem(int v1, int v2) :val1(v1), val2(v2) {}
// 	friend ostream& operator<<(ostream& out, const DoubleElem& de)
// 	{
// 		out << "[" << de.val1 << "," << de.val2 << "]";
// 		return out;
// 	}
// 	int val1;
// 	int val2;
// };
// 
// bool isDoubleOdd(const DoubleElem& de)	// unary predicate
// {
// 	return de.val1 % 2 && de.val2 % 2;
// }
// 
// 
// bool isDoubleOddComp(const DoubleElem& de1, const DoubleElem& de2)	// binary predicate
// {
// 	return de1.val1 < de2.val1 || (de1.val1 == de2.val1 && de1.val2 < de2.val2);
// }
// 
// int main()
// {
// 	vector<DoubleElem> vecDoubleElem{ DoubleElem(1,3), DoubleElem(4,1), DoubleElem(2,3), DoubleElem(3,3), DoubleElem(5,3) };
// 	copy_if(vecDoubleElem.begin(), vecDoubleElem.end(), ostream_iterator<DoubleElem>(cout, " "), isDoubleOdd);
// 	cout << endl;
//  	sort(vecDoubleElem.begin(), vecDoubleElem.end(), isDoubleOddComp);
//  	copy(vecDoubleElem.begin(), vecDoubleElem.end(), ostream_iterator<DoubleElem>(cout, " "));
// 	cout << endl;
// 	return 0;
// }