// #include <algorithm>
// #include <iostream>
// #include <vector>
// #include <functional>
// #include <iterator>
// using namespace std;
// 
// class Pred
// {
// public:
// 	int x, y;
// 	Pred(int xx, int yy) :x(xx), y(yy) {}
// 	bool operator()(Pred& p) const { return p.x > x && p.y < y; }
// };
// 
// 
// int main()
// {
// 	vector<int> coll{ 3,2,1,4,5,6,11,12,13 };
// 	copy_if(coll.begin(), coll.end(), ostream_iterator<int>(cout, " "), bind(logical_not<int>(), bind(modulus<int>(),placeholders::_1,2)));
// 	cout << endl;
	// 	vector<Pred> coll{ Pred(2,2),Pred(3,3),Pred(1,1),Pred(1,3) };
// 	auto pos = find_if(coll.begin(), coll.end(), 
// 		bind(
// 			logical_and<bool>(), 
// 			bind(greater<int>(), placeholders::_1, 1), 
// 			bind(less<int>(), placeholders::_1, 2)
// 		)
// 	);
// 	cout << "[" << pos->x << "," << pos->y << "]" << endl;
// 	replace_if(coll.begin(), coll.end(), bind(modulus<int>(), placeholders::_1, 2), 10);
// 	copy(coll.begin(), coll.end(), ostream_iterator<int>(cout, " "));
// 	cout << endl;
// 	cout << bind(&Pred::x, placeholders::_1)(Pred(100, 101)) << endl;
// 	return 0;
// }