/*
 *	array,vector,deque,string		随机迭代器
 *	list,set,multiset,map,multimap	双向迭代器
 *	forward_list,unordered_set,unordered_multiset,unordered_map,unordered_multimap	前向迭代器
 */
#include <iterator>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>

using namespace std;

int main()
{
	unordered_map<int, string> coll;
	coll.begin();

	return 0;
}