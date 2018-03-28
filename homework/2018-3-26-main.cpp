// 
// #include <iostream>
// #include <string>
// using namespace std;
// 
// template <typename T1, typename T2>
// auto add(T1 x, T2 y) -> decltype(x + y);
// 
// 
// template <typename T1, typename T2>
// auto add_labd = [](T1 x, T2 y) -> decltype(x + y) {return x + y; }
// 
// template <typename T>
// class MyClass {
// public:
// 	template <typename U>
// 	MyClass(const MyClass<U>& x);
// };
// 
// #include <map>
// 
// void f(pair<int, const char*>) { }
// 
// void g(pair<const int, string>) { }
// 
// void foo()
// {
// 	pair<int, const char*> p(42, "hello");
// 	f(p);
// 	g(p);
// }
// 
// int main()
// {
// 	foo();
// 	pair<int, double> p1(10, 1.1e-2);
// 
// 	pair<int, double> p2;
// 	p2 = make_pair(10, 2.22e-1);
// 
// 	pair<int, double> p3(p1);
// 
// 	cout.precision(3);
// 	cout << "The pair p1 is: (" << p1.first << ", "
// 		<< p1.second << " )." << endl;
// 	cout << "The pair p2 is: (" << p2.first << ", "
// 		<< p2.second << " )." << endl;
// 	cout << "The pair p3 is: (" << p3.first << ", "
// 		<< p3.second << " )." << endl;
// 
// 	map <int, int> m1;
// 	map <int, int>::iterator m1_Iter;
// 
// 	typedef pair <int, int> Map_Int_Pair;
// 
// 	m1.insert(Map_Int_Pair(1, 10));
// 	m1.insert(Map_Int_Pair(2, 20));
// 	m1.insert(Map_Int_Pair(3, 30));
// 
// 	cout << "The element pairs of the map m1 are:";
// 	for (m1_Iter = m1.begin(); m1_Iter != m1.end(); m1_Iter++)
// 		cout << " ( " << m1_Iter->first << ", "
// 		<< m1_Iter->second << " )";
// 	cout << "." << endl;
// 
// 	pair< map<int, int>::iterator, bool> pr1, pr2;
// 	pr1 = m1.insert(Map_Int_Pair(4, 40));
// 	pr2 = m1.insert(Map_Int_Pair(1, 10));
// 
// 	if (pr1.second == true)
// 	{
// 		cout << "The element (4,40) was inserted successfully in m1." << endl;
// 	}
// 	else
// 	{
// 		cout << "The element with a key value of\n"
// 			<< " ( (pr1.first) -> first ) = " << (pr1.first)->first
// 			<< " is already in m1,\n so the insertion failed." << endl;
// 	}
// 
// 	if (pr2.second == true)
// 	{
// 		cout << "The element (1,10) was inserted successfully in m1."
// 			<< endl;
// 	}
// 	else
// 	{
// 		cout << "The element with a key value of\n"
// 			<< " ( (pr2.first) -> first ) = " << (pr2.first)->first
// 			<< " is already in m1,\n so the insertion failed." << endl;
// 	}
// 	return 0;
// }
// 
// template <typename T1, typename T2>
// auto add(T1 x, T2 y) -> decltype(x + y)
// {
// 	return x + y;
// }