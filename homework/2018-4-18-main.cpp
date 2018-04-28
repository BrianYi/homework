#include <iostream>
#include <iterator>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

int main()
{
	vector<int> coll(9);
	iota(coll.begin(), coll.end(), 1);
	default_random_engine dre;
	cout << "==========��ʱ��������������==========" << endl;
	for (int i = 0; i < 9; i++)
	{
		shuffle(coll.begin(), coll.end(), default_random_engine());
		for (auto& elem : coll)
			cout << elem << " ";
		cout << endl;
	}
	cout << endl;
	
	cout << "==========������ʱ��������������==========" << endl;
	for (int i = 0; i < 9; i++)
	{
		shuffle(coll.begin(), coll.end(), dre);
		for (auto& elem : coll)
			cout << elem << " ";
		cout << endl;
	}
	return 0;
}