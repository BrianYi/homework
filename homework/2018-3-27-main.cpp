// C-M C-S 折叠当前块, C-M C-e 展开当前块
// #include <iostream>
// #include <utility>
// #include <tuple>
// #include <string>
// #include <vector>
// #include <memory>
// using namespace std;

// struct Foo
// {
// 	Foo(tuple<int, float>)
// 	{
// 		cout << "Constructed a Foo from a tuple\n";
// 	}
// 	Foo(int, float)
// 	{
// 		cout << "Constructed a Foo from an int and a float\n";
// 	}
// };

// class Foo {
// public:
// 	Foo(tuple<int, float>)
// 	{
// 		cout << "Foo::Foo(tuple)" << endl;
// 	}
// 	template <typename ...Args>
// 	Foo(Args ...args)
// 	{
// 		cout << "Foo::Foo(args...)" << endl;
// 	}
// 	Foo(int a, float b)
// 	{
// 		cout << a << " " << b << endl;
// 	}
// };

// int main()
// {
// 	pair<int, float> t(piecewise_construct, make_tuple(10), make_tuple(2.22));
// 
// 	pair<char, char> p = make_pair('x', 'y');
// 	char c;
// 	tie(ignore, c) = p;
// 	cout << c << endl;
// 	
// 	return 0;
// }

// int main()
// {
// 	shared_ptr<string> pNico(new string("nico"));
// 	shared_ptr<string> pJutta(new string("jutta"));
// 
// 	(*pNico)[0] = 'N';
// 	pJutta->replace(0, 1, "J");
// 	
// 	shared_ptr<string> pStr;
// 	pStr.reset(new string("hello"));
// 
// 	vector<shared_ptr<string>> whoMadeCoffee;
// 	whoMadeCoffee.push_back(pJutta);
// 	whoMadeCoffee.push_back(pJutta);
// 	whoMadeCoffee.push_back(pNico);
// 	whoMadeCoffee.push_back(pJutta);
// 	whoMadeCoffee.push_back(pNico);
// 
// 	for (auto ptr : whoMadeCoffee)
// 		cout << *ptr << endl;
// 	cout << endl;
// 
// 	*pNico = "Nicolai";
// 
// 	for (auto ptr : whoMadeCoffee)
// 		cout << *ptr << " ";
// 	cout << endl;
// 
// 	cout << "use_count: " << whoMadeCoffee[0].use_count() << endl;
// 
// 
// 	shared_ptr<int> p(new int[10], default_delete<int[]>());
// }

/*
 *	shared_ptr循环引用导致内存泄露
 */

// struct A
// {
// 	shared_ptr<A> ptr;
// 	int val;
// 	A(int inVal):val(inVal){}
// 	~A()
// 	{
// 		cout << "Destructor: value " << val << endl;
// 	}
// };
// 
// int main()
// {
// 	shared_ptr<A> ptr1(new A(1)); // strong ref 是1
// 	ptr1->ptr = ptr1; // strong ref 是2
// 	cout << ptr1.use_count() << endl;
// 	
// 	return 0;
// 	/*
// 	 *	ptr1释放，ref count减1，但此时仍为1，因此未能进行内存释放，导致泄露
// 	 */
// }


