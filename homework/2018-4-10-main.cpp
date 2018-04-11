/*
 *	unordered map或unordered multimap可以拥有任何类型的Key和T,前提是它们满足如下条件：
 *	1. Key和value都是copyable或movable
 *	2. Key可被“等价准则”拿来比较
 *	第二或第三template参数默认都采用hash<>,这是个function object
 *	读三或第四template参数用来定义等价准则：这是一个predicate，默认为equal_to<>，会以operator==比较两个元素
 *	第四或第五template参数用来定义内存模型，默认内存模型为allocator
 */
// #include <functional>
// #include <string>
// #include <iostream>
// #include <unordered_map>
// using namespace std;
// 
// template <typename T>
// struct HashCmp
// {
// 	size_t operator()(const int&) const
// 	{
// 		return 1;
// 	}
// };
// 
// size_t HashCmpFunc(const int&) { return 1; }
// 
// int main()
// {
// 	/*
// 	 *	hash function object的使用
// 	 */
// 	hash<char> a;
// 	for (int i = 0; i < 10; i++)
// 		cout << a(i) << " ";
// 	cout << endl;
// 	for (int i = 0; i < 10; i++)
// 		cout << a(i) << " ";
// 	cout << endl;
// 
// 	/*
// 	 *	1. 自定义hash function object
// 	 *	2. bucket count与bucket size间的关系
// 	 *	   bucket count: 表示这个hash表有多少个bucket可以使用
// 	 *	   bucket size: 这个是带个key为参数的，代表与key相对应的bucket内存放了多少元素(采用linked list，单链或双链取决于实现)
// 	 */
// 	//unordered_map<int, string, HashCmp<int>, bool(*)(const int&, const int&)> coll2(2, HashCmp<int>(), [](const int& key1, const int& key2)->bool { return key1 == key2; });
// 	//unordered_map<int, string, function<decltype(HashCmpFunc)>> coll2(2, HashCmpFunc);
// 	//unordered_map<int, string, size_t(*)(const int&)> coll2(2, HashCmpFunc);
// 	
// 	// hash function和equivalence criterion function完全采用lambda
// 	cout << "==========My hash function with equivalence criterion==========" << endl;
// 	unordered_map<int, string, size_t(*)(const int&), bool(*)(const int&, const int&)> coll2(
// 		8, 
// 		[](const int&)->size_t { return 1; }, 
// 		[](const int& key1, const int& key2)->bool { return key1 == key2; });
// 	coll2.insert({ { 1, "a" },{ 3,"b" },{ 5,"c" },{ 7,"d" },{ 9,"e" },{ 11,"f" },{ 13,"g" },{ 17,"h" } });
// 	cout << "bucket count: " << coll2.bucket_count() << endl;
// 	cout << "current load_factor: " << coll2.load_factor() << endl;
// 	cout << "current max_load_factor: " << coll2.max_load_factor() << endl;
// 	bool bDoublyLinked = (typeid(iterator_traits<decltype(coll2)::iterator>::iterator_category) ==
// 		typeid(bidirectional_iterator_tag));
// 	if (bDoublyLinked)
// 		cout << "chaining style: doubly-linked" << endl;
// 	else
// 		cout << "chaining style: singly-linked" << endl;
// 	for (size_t idx = 0; idx < coll2.bucket_count(); ++idx)
// 	{
// 		cout << "bucket " << idx << " size: " << coll2.bucket_size(idx) << ", values: ";
// 		for (auto iter = coll2.begin(idx); iter != coll2.end(idx); ++iter)
// 		{
// 			cout << "[" << iter->first << "," << iter->second << "]";
// 			if (next(iter) != coll2.end(idx))
// 				cout << (bDoublyLinked ? " <-> " : " -> ");
// 		}
// 		cout << endl;
// 	}
// 	cout << endl;
// 	
// 	cout << "==========Default hash function with equivalence criterion==========" << endl;
// 	unordered_map<int, string> coll(8); 
// 	coll.insert({ {1, "a"}, { 3,"b" }, { 5,"c" }, { 7,"d" }, { 9,"e" }, { 11,"f" }, { 13,"g" }, { 17,"h" } });
// 	cout << "bucket count: " << coll.bucket_count() << endl;
// 	cout << "current load_factor: " << coll.load_factor() << endl;
// 	cout << "current max_load_factor: " << coll.max_load_factor() << endl;
// 	bDoublyLinked = (typeid(iterator_traits<decltype(coll2)::iterator>::iterator_category) ==
// 		typeid(bidirectional_iterator_tag));
// 	if (bDoublyLinked)
// 		cout << "chaining style: doubly-linked" << endl;
// 	else
// 		cout << "chaining style: singly-linked" << endl;
// 	for (size_t idx = 0; idx < coll.bucket_count(); ++idx)
// 	{
// 		cout << "bucket " << idx << " size: " << coll.bucket_size(idx) << ", values: ";
// 		for (auto iter = coll.begin(idx); iter != coll.end(idx); ++iter)
// 		{
// 			cout << "[" << iter->first << "," << iter->second << "]";
// 			if (next(iter) != coll.end(idx))
// 				cout << (bDoublyLinked ? " <-> " : " -> ");
// 		}
// 		cout << endl;
// 	}
// 	cout << endl;
// 	return 0;
// }


/*
 *	共享对象
 */
// #include <string>
// #include <set>
// #include <algorithm>
// #include <memory>
// #include <iostream>
// #include <deque>
// #include <vector>
// 
// using namespace std;
// 
// class Item
// {
// 	string name;
// 	double price;
// public:
// 	Item(const string& n, double p = 0) :name(n), price(p) {}
// 	string getName() const { return name; }
// 	void setName(const string& n) { name = n; }
// 	double getPrice() const { return price; }
// 	void setPrice(double p) { price = p; }
// };
// 
// template <typename Coll>
// void printItems(const string& msg, const Coll& coll)
// {
// 	cout << msg << endl;
// 	for (const auto& elem : coll)
// 		cout << " " << elem->getName() << ": "
// 		<< elem->getPrice() << endl;
// }
// 
// int main()
// {
// 	/*
// 	 *	在容器中使用shared_ptr来共享对象
// 	 */
// 	cout << "==========shared_ptr==========" << endl;
// 	using ItemPtr = shared_ptr<Item>;
// 	set<ItemPtr> allItems;
// 	deque<ItemPtr> bestsellers;
// 
// 	bestsellers = { ItemPtr(new Item("Kong Yize", 20.10)),
// 		ItemPtr(new Item("A Midsummer Night's Dream", 14.99)),
// 		ItemPtr(new Item("The Maltese Falcon", 9.88)) };
// 	allItems = { ItemPtr(new Item("Water", 0.44)),
// 			ItemPtr(new Item("Pizza", 2.22)) };
// 	allItems.insert(bestsellers.begin(), bestsellers.end());
// 
// 	printItems("bestsellers:", bestsellers);
// 	printItems("all:", allItems);
// 	cout << endl;
// 
// 	for_each(bestsellers.begin(), bestsellers.end(), [](ItemPtr& elem) { elem->setPrice(elem->getPrice() * 2); });
// 
// 	bestsellers[1] = *(find_if(allItems.begin(), allItems.end(), [](ItemPtr elem) {return elem->getName() == "Pizza"; }));
// 
// 	bestsellers[0]->setPrice(44.77);
// 
// 	printItems("bestsellers:", bestsellers);
// 	printItems("all:", allItems);
// 	cout << endl;
// 
// 	/*
// 	 *	在容器中使用reference_wrapper来共享对象
// 	 *	1. 需要使用get()来引用对象
// 	 *	2. 但通过range-based for循环可以如下访问，无序使用get()来引用对象
// 	 *		
// 	 *		// book为const Item& 类型
// 	 *		for (const Item& book : books)
// 	 *			cout << book.getName() << endl;
// 	 *		
// 	 *		// book为const reference_wrapper<Item>& 类型
// 	 *		for (const auto& book : books)
// 	 *			cout << book.get().getName() << endl;
// 	 */
// 	cout << "==========reference_wrapper==========" << endl;
// 	vector<reference_wrapper<Item>> books;
// 	Item f("Faust", 12.99);
// 	books.push_back(f);
// 
// 	for (const auto& book : books)
// 	{
// 		cout << book.get().getName() << ": "
// 			<< book.get().getPrice() << endl;
// 	}
// 
// 	f.setPrice(9.99);
// 	cout << books[0].get().getPrice() << endl;
// 
// 	for (const Item& book : books)
// 	{
// 		cout << book.getName() << ": " << book.getPrice() << endl;
// 	}
// 	cout << endl;
// 	return 0;
// }
