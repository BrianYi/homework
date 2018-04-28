#pragma once
/*
 *	别偷懒，每个都默写一遍并详细理解用法
 */

#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
#include <iostream>
#include <string>
using namespace std;

template <typename Container>
inline void INSERT_ELEMENTS(Container& container, int first, int last)
{
	for (int i = first; i <= last; ++i)
	{
		container.insert(container.end(), i);
	}
}

template <typename Container>
inline void PRINT_ELEMENTS(const Container& container, const string& optstr = "")
{
	cout << optstr;
	for (auto elem : container)
		cout << elem << " ";
	cout << endl;
}

template <typename Container>
inline void PRINT_MAPPED_ELEMENTS(const Container& coll, const string& optstr = "")
{
	cout << optstr;
	for (auto elem : coll)
	{
		cout << '[' << elem.first << ',' << elem.second << "] ";
	}
	cout << endl;
	return ;
}
