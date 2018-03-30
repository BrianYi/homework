/*
 *	limits用法
 */
// #include <limits>
// #include <iostream>
// #include <string>
// using namespace std;
// 
// int main()
// {
// 	cout << boolalpha;
// 
// 	cout << "max(short): " << numeric_limits<short>::max() << endl;
// 	cout << "max(int): " << numeric_limits<int>::max() << endl;
// 	cout << "max(long): " << numeric_limits<long>::max() << endl;
// 	cout << endl;
// 
// 	cout << "max(float): " << numeric_limits<float>::max() << endl;
// 	cout << "max(double): " << numeric_limits<double>::max() << endl;
// 	cout << "max(long double): " << numeric_limits<long double>::max() << endl;
// 	cout << endl;
// 
// 	cout << "is_signed(char): " << numeric_limits<char>::is_signed << endl;
// 	cout << endl;
// 
// 	cout << "is_specialized(string): " << numeric_limits<string>::is_specialized << endl;
// }	

/*
 *	Type Trait用法: 编译时根据一个或多个template实参产出一个type或value
 */
// #include <type_traits>
// #include <iostream>
// using namespace std;
// 
// template <typename T>
// void foo(const T& val)
// {
// 	if (is_pointer<T>::value)
// 		cout << "foo() called for a pointer" << endl;
// 	else
// 		cout << "foo() called for a value" << endl;
// }
// 
// int main()
// {
// 	int c=3;
// 	int&& a = move(c);
// 	foo(*new int);
// 	return 0;
// }


/*
 *	Function Type Wrapper
 */
// #include <functional>
// #include <vector>
// #include <iostream>
// using namespace std;
// 
// function<void()> ret_func()
// {
// 	return []() { cout << "ret_func return a lambda expression." << endl; };
// }
// 
// int main()
// {
// 	vector<function<void(int, int)>> tasks;
// 	tasks.push_back([](int x, int y) {
// 		cout << "function 1: x = " << x << ", y = " << y << endl;
// 	});
// 	tasks.push_back([](int x, int y) {
// 		cout << "function 2: x = " << x << ", y = " << y << endl;
// 	});
// 	for (const auto& funObj : tasks)
// 		funObj(1, 2);
// 
// 	ret_func()();
// 	return 0;
// }


/*
 *	增补的“比较操作符”(Comparison Operator)
 *	!=、>、<=和>=四个比较操作符都是利用==和<完成
 *	rel_ops, utility文件
 */
// #include <utility>
// #include <iostream>
// #include <algorithm>
// using namespace std;
// 
// class X
// {
// public:
// 	X(int val) :fVal(val) {}
// 	int getVal() { return fVal; }
// public:
// 	bool operator== (const X& x) const { return x.fVal == fVal; }
// 	bool operator< (const X& x) const { return x.fVal < fVal; }
// private:
// 	int fVal;
// };
// 
// int main()
// {
// 	using namespace std::rel_ops;	// make !=,>,etc.available
// 	X x[]{ X(3),X(1),X(6),X(2),X(4),X(5) };
// 
// 	if (x[0] != x[1])
// 		cout << "x[0] not equal to x[1]" << endl;
// 
// 	if (x[0] > x[1])
// 		cout << "x[0] big than x[1]" << endl;
// 
// 	sort(begin(x), end(x));
// 
// 	for (auto val : x)
// 		cout << val.getVal() << endl;
// 	return 0;
// }



/*
 *	Class ratio<>编译器分数运算
 *	编译期捕获所有差错(分母为0或溢出等)
 *	可以大幅度降低运行时溢出的几率，因为任何ratio值总是会变为最简式(lowest term), 即4/10=>2/5
 *	ratio<intmax_t N, intmax_t D=1>, 分母默认1, intmax_t是signed的64bit类型(在cstdint文件中)
 */
// #include <ratio>
// #include <iostream>
// using namespace std;
// 
// int main()
// {
// 	typedef ratio<5, 30> FiveThirds;
// 	cout << FiveThirds::num << "/" << FiveThirds::den << endl; // 自动化为最简式1/6
// 
// 	ratio<0> zero;
// 	cout << zero.num << "/" << zero.den << endl;
// 
// 	ratio<4, -10> neg;
// 	cout << neg.num << "/" << neg.den << endl;
// 
// 
// 	// ratio_add
// 	ratio<3, 5> threeOfFive;
// 	ratio<2, 10> oneOfTen;
// 	cout << threeOfFive.num << "/" << threeOfFive.den << " + "
// 		<< oneOfTen.num << "/" << oneOfTen.den << " = "
// 		<< ratio_add<decltype(threeOfFive), decltype(oneOfTen)>::num 
// 		<< "/" << ratio_add<decltype(threeOfFive), decltype(oneOfTen)>::den <<endl;
// 
// 	// ratio_subtract
// 	cout << threeOfFive.num << "/" << threeOfFive.den << " - "
// 		<< oneOfTen.num << "/" << oneOfTen.den << " = "
// 		<< ratio_subtract<decltype(threeOfFive), decltype(oneOfTen)>::num
// 		<< "/" << ratio_subtract<decltype(threeOfFive), decltype(oneOfTen)>::den << endl;
// 
// 	// ratio_multiply
// 	cout << threeOfFive.num << "/" << threeOfFive.den << " * "
// 		<< oneOfTen.num << "/" << oneOfTen.den << " = "
// 		<< ratio_multiply<decltype(threeOfFive), decltype(oneOfTen)>::num
// 		<< "/" << ratio_multiply<decltype(threeOfFive), decltype(oneOfTen)>::den << endl;
// 
// 	// ratio_divide
// 	cout << threeOfFive.num << "/" << threeOfFive.den << " / "
// 		<< oneOfTen.num << "/" << oneOfTen.den << " = "
// 		<< ratio_divide<decltype(threeOfFive), decltype(oneOfTen)>::num
// 		<< "/" << ratio_divide<decltype(threeOfFive), decltype(oneOfTen)>::den << endl;
// 	return 0;
// }
// 
