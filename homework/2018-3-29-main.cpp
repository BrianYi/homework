/*
 *	limits�÷�
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
 *	Type Trait�÷�: ����ʱ����һ������templateʵ�β���һ��type��value
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
 *	�����ġ��Ƚϲ�������(Comparison Operator)
 *	!=��>��<=��>=�ĸ��Ƚϲ�������������==��<���
 *	rel_ops, utility�ļ�
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
 *	Class ratio<>��������������
 *	�����ڲ������в��(��ĸΪ0�������)
 *	���Դ���Ƚ�������ʱ����ļ��ʣ���Ϊ�κ�ratioֵ���ǻ��Ϊ���ʽ(lowest term), ��4/10=>2/5
 *	ratio<intmax_t N, intmax_t D=1>, ��ĸĬ��1, intmax_t��signed��64bit����(��cstdint�ļ���)
 */
// #include <ratio>
// #include <iostream>
// using namespace std;
// 
// int main()
// {
// 	cout << "/* ratio����÷� */" << endl << endl;
// 	// ratio_add
// 	ratio<3, 5> threeOfFive;
// 	ratio<2, 10> oneOfTen;
// 
// 	cout << "==========ratio��������==========" << endl;
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
// 	cout << endl;
// 
// 
// 	// ��С�Ƚ�
// 	cout << boolalpha;
// 	cout << "==========ratio�Ƚ�(==,!=,<,<=,>,>=)==========" << endl;
// 
// 	// ratio_less
// 	cout << threeOfFive.num << "/" << threeOfFive.den << " < "
// 		<< oneOfTen.num << "/" << oneOfTen.den << " ? ==> "
// 		<< ratio_less<decltype(threeOfFive), decltype(oneOfTen)>::value << endl;
// 
// 	// ratio_greater
// 	cout << threeOfFive.num << "/" << threeOfFive.den << " > "
// 		<< oneOfTen.num << "/" << oneOfTen.den << " ? ==> "
// 		<< ratio_greater<decltype(threeOfFive), decltype(oneOfTen)>::value << endl;
// 
// 	// ratio_equal
// 	cout << threeOfFive.num << "/" << threeOfFive.den << " = "
// 		<< oneOfTen.num << "/" << oneOfTen.den << " ? ==> "
// 		<< ratio_equal<decltype(threeOfFive), decltype(oneOfTen)>::value << endl;
// 	cout << endl;
// 
// 	// Ԥ�����ratio��λ
// 	cout << "==========Ԥ�����ratio��λ==========" << endl;
// 	cout << atto::num << "/" << atto::den << endl;
// 	cout << femto::num << "/" << femto::den << endl;
// 	cout << pico::num << "/" << pico::den << endl;
// 	cout << nano::num << "/" << nano::den << endl;
// 	cout << micro::num << "/" << micro::den << endl;
// 	cout << milli::num << "/" << milli::den << endl;
// 	cout << centi::num << "/" << centi::den << endl;
// 	cout << deci::num << "/" << deci::den << endl;
// 	cout << deca::num << "/" << deca::den << endl;
// 	cout << hecto::num << "/" << hecto::den << endl;
// 	cout << kilo::num << "/" << kilo::den << endl;
// 	cout << mega::num << "/" << mega::den << endl;
// 	cout << giga::num << "/" << giga::den << endl;
// 	cout << tera::num << "/" << tera::den << endl;
// 	cout << peta::num << "/" << peta::den << endl;
// 	cout << exa::num << "/" << exa::den << endl;
// 	return 0;
// }
// 
