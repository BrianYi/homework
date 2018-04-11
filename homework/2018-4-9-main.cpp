/*
 *	set的自定义运行时比较规则 
 */
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
//#include <map>
//#include <iostream>
//#include <string>
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
//int main()
//{
//	RuntimeCmp runTimeCmp(RuntimeCmp::normal);
//	map<int, string, RuntimeCmp> coll1(runTimeCmp);
//	coll1.insert({ {1,"hello"},{3,"how"},{2,"much"},{4,"what"} });
//	for (auto& elem : coll1)
//		cout << "(" << elem.first << ",\"" << elem.second << "\")" << " ";
//	cout << endl;
//	for (auto& elem : coll1)
//		cout << "(" << elem.first << ",\"" << elem.second << "\")" << " ";
//	cout << endl;
//
//	return 0;
//}

/*
 *	综合实例map,string运行期指定排序准则
 */
// #include <iomanip>
// #include <map>
// #include <string>
// #include <algorithm>
// #include <cctype>
// #include <iostream>
// 
// using namespace std;
// 
// class RuntimeStringCmp
// {
// public:
// 	enum cmp_mode { normal, nocase };
// private:
// 	const cmp_mode mode;
// 
// 	static bool nocase_compare(char c1, char c2) { return toupper(c1) < toupper(c2); }
// public:
// 	RuntimeStringCmp(cmp_mode m = normal) :mode(m) {}
// 	bool operator()(const string& s1, const string& s2) const
// 	{
// 		if (mode == normal)
// 			return s1 < s2;
// 		else
// 			return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), nocase_compare);
// 	}
// };
// 
// using StringStringMap = map<string, string, RuntimeStringCmp>;
// 
// void fillAndPrint(StringStringMap& coll);
// 
// int main()
// {
// 	// 区分大小写，排序从小到大
// 	StringStringMap coll1;
// 	fillAndPrint(coll1);
// 
// 	RuntimeStringCmp ignorecase(RuntimeStringCmp::nocase);
// 
// 	// 不区分大小写，排序从小到大
// 	StringStringMap coll2(ignorecase);
// 	fillAndPrint(coll2);
// 
// 	return 0;
// }
// 
// void fillAndPrint(StringStringMap& coll)
// {
// 	coll["Deutschaland"] = "Germany";
// 	coll["deutsch"] = "German";
// 	coll["Haken"] = "snag";
// 	coll["arbeiten"] = "work";
// 	coll["Hund"] = "dog";
// 	coll["gehen"] = "go";
// 	coll["Unternehmen"] = "enterprise";
// 	coll["unternehmen"] = "undertake";
// 	coll["gehen"] = "walk";
// 	coll["Bestatter"] = "undertaker";
// 
// 	cout.setf(ios::left, ios::adjustfield);
// 	for (const auto& elem : coll)
// 	{
// 		cout << setw(15) << elem.first << " " << elem.second << endl;
// 	}
// 	cout << endl;
// }