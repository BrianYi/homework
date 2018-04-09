///*
// *	set的自定义运行时比较规则 
// */
//#include <set>
//#include <iostream>
//using namespace std;
//
//class RuntimeCmp
//{
//public:
//	enum cmp_mode { normal, reverse };
//	cmp_mode mode;
//public:
//	RuntimeCmp(cmp_mode m = normal) :mode(m) {}
//
//	template <typename T>
//	bool operator() (const T& t1, const T& t2) const
//	{
//		return mode == normal ? t1 < t2 : t2 < t1;
//	}
//	bool operator==(const RuntimeCmp& rc) const
//	{
//		return mode == rc.mode;
//	}
//};
//
//
//int main()
//{
//	using IntSet = set<int, RuntimeCmp> ;
//	IntSet coll1(RuntimeCmp::reverse);
//	coll1.insert({ 4, 7, 5, 1, 6, 2, 5 });
//	for (auto& elem : coll1)
//		cout << elem << " ";
//	cout << endl;
//
//	if (coll1.key_comp() == coll1.value_comp())
//		cout << "key_comp() equal to value_comp()" << endl;
//	
//	return 0;
//}

/*
 *	map的自定义运行时比较规则
 */
#include <map>
#include <iostream>
#include <string>
using namespace std;

class RuntimeCmp
{
public:
	enum cmp_mode { normal, reverse };
	cmp_mode mode;
public:
	RuntimeCmp(cmp_mode m = normal) :mode(m) {}

	template <typename T>
	bool operator() (const T& t1, const T& t2) const
	{
		return mode == normal ? t1 < t2 : t2 < t1;
	}
	bool operator==(const RuntimeCmp& rc) const
	{
		return mode == rc.mode;
	}
};

int main()
{
	RuntimeCmp runTimeCmp(RuntimeCmp::normal);
	map<int, string, RuntimeCmp> coll1(runTimeCmp);
	coll1.insert({ {1,"hello"},{3,"how"},{2,"much"},{4,"what"} });
	for (auto& elem : coll1)
		cout << "(" << elem.first << ",\"" << elem.second << "\")" << " ";
	cout << endl;
	runTimeCmp.mode = RuntimeCmp::reverse;
	for (auto& elem : coll1)
		cout << "(" << elem.first << ",\"" << elem.second << "\")" << " ";
	cout << endl;

// 	if (coll1.key_comp() == coll1.value_comp())
// 		cout << "key_comp() equal to value_comp()" << endl;

	return 0;
}