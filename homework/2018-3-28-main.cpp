// 
// #include <iostream>
// #include <string>
// #include <vector>
// #include <memory>
// using namespace std;
// 
// class Person : public enable_shared_from_this<Person>
// {
// public:
//     string name;
//     shared_ptr<Person> mother;
//     shared_ptr<Person> father;
//     vector<weak_ptr<Person>> kids;
// 
//     Person(const string& n, shared_ptr<Person> m = nullptr, shared_ptr<Person> f = nullptr) :
//         name(n), mother(m), father(f) {}
//     void setParentsAndTheirKids(shared_ptr<Person> m = nullptr, shared_ptr<Person> f = nullptr)
//     {
//         mother = m;
//         father = f;
//         if (m != nullptr)
//             m->kids.push_back(shared_from_this());
//         if (f != nullptr)
//             f->kids.push_back(shared_from_this());
//     }
//     ~Person()
//     {
//         cout << "delete " << name << endl;
//     }
// };
// 
// shared_ptr<Person> initFamily(const string& name)
// {
//     shared_ptr<Person> mom(new Person(name + "'s mom")); // mom strong-ref 1 
//     shared_ptr<Person> dad(new Person(name + "'s dad")); // dad strong-ref 1
//     shared_ptr<Person> kid(new Person(name, mom, dad)); // mom strong-ref 2, dad strong-ref 2, kid strong-ref 1
//     mom->kids.push_back(kid); // mom strong-ref 2, dad strong-ref 2, kid strong-ref 2
//     dad->kids.push_back(kid); // mom strong-ref 2, dad strong-ref 2, kid strong-ref 3
//     return kid; // mom strong-ref 1, dad strong-ref 1, kid strong-ref 2
// }
// 
// int main()
// {
//     // 	shared_ptr<Person> p = initFamily("nico"); // mom strong-ref 1, dad strong-ref 1, kid strong-ref 3
//     // 
//     // 	cout << "nico's family exist" << endl;
//     // 	cout << "- nico is shared " << p.use_count() << " times" << endl;
//     // 	cout << "- name of 1st kid of nico's mom: "
//     // 		<< p->mother->kids[0].lock()->name << endl;
//     // 
//     // 	p = initFamily("jim");
//     // 	cout << "jim's family exists" << endl;
// 
// 	// shared_ptr和weak_ptr之间的转化
// 	//     shared_ptr<string> sp(new string("hi"));
// 	// 	cout << sp << endl;
// 	// 	weak_ptr<string> wp = sp;
// 	//     sp.reset();
// 	//     cout << wp.use_count() << endl;
// 	//     cout << boolalpha << wp.expired() << endl;
// 	//     shared_ptr<string> p(wp);
// 	
// 	// 获取shared_ptr中用户自定义的destructor
// 	// 	auto del = [](int *p) { delete p; };
// 	// 	shared_ptr<int> p(new int, del);
// 	// 	decltype(del)* pd = get_deleter<decltype(del)>(p);
// 
// 
// 	// static_pointer_cast的用法
// // 	struct base { int val; };
// // 	struct derived :public base {};
// // 	shared_ptr<base> sp0(new derived);
// // 	shared_ptr<derived> sp1 = static_pointer_cast<derived>(sp0);
// // 	sp0->val = 3;
// // 	cout << "sp1->val == " << sp1->val << endl;
// 
// 	// weak_ptr的use_count()返回的对象时被shared_ptr拥有的次数
// 	// 一般而言，shared_ptr并非线程安全的，不过可以如下实现线程安全
// 	// 注意：并发访问的是pointer而非其指向的值
// // 	struct A { int a; float b; };
// // 	shared_ptr<A> ptr1;
// // 	shared_ptr<A> ptr2{ new A };
// // 	atomic_store(&ptr1, ptr2); // 将ptr2赋值给ptr1
// 
// 	// unique_ptr: 
// 	// 无论正常或异常结束，unique_ptr所指向的对象都会被自动销毁
// 	// unique_ptr的必要条件是只有一个拥有者
// // 	struct ClassA { ~ClassA() { cout << "Destructor called!" << endl; } };
// // 	unique_ptr<ClassA> ptr(new ClassA);
// //     ptr = make_unique<ClassA>();
// // 	ptr = nullptr; // 效果与ptr.reset();相同
// // 
// // 	unique_ptr<int, void(*)(int*)> up(new int[10], [](int *p) { delete[]p; });
// 
//     return 0;
// }