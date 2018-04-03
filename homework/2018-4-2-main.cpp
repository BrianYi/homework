/*
 *	Blocking with Timer:
 *	1. sleep_for, sleep_until
 *	2. try_lock_for, try_lock_until
 *	3. wait_for, wait_until
 */

// #include <iostream>
// #include <chrono>
// #include <thread>
// #include <string>
// using namespace std;
// using namespace chrono;
// 
// void TimeNow()
// {
// 	time_t t = system_clock::to_time_t(system_clock::now());
// 	string st = ctime(&t);
// 	st.resize(st.size() - 1);
// 	cout << "NOW: " << st << endl;
// }
// 
// void SleepFor()
// {
// 	TimeNow();
// 	cout << "sleep for(3 seconds)" << endl;
// 	this_thread::sleep_for(chrono::seconds(3));
// 	TimeNow();
// 	cout << endl;
// }
// 
// void SleepUntil()
// {
// 	TimeNow();
// 	cout << "sleep until(3 seconds)" << endl;
// 	this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(3));
// 	TimeNow();
// 	cout << endl;
// }
// 
// int main()
// {
// 	cout << "==========sleep_for==========" << endl;
// 	SleepFor();
// 	cout << "==========sleep_until==========" << endl;
// 	SleepUntil();
// 	return 0;
// }


/*
 *	set: �ڲ����ú����(��ƽ�������������һ��)��Ĭ������׼���Բ����� < Ϊ����(Ĭ�ϵ���)
 *		(< ��ʾ˭С˭�ŵ�һ��)
 *	unordered_set: �ڲ����ù�ϣ��
 *	multiset: �ڲ����ú�����ӹ�ϣ��Ĭ������׼���Բ����� < Ϊ����(Ĭ�ϵ���)
 *  unordered_multiset: �ڲ����ù�ϣ��
 */

// #include <iostream>
// #include <set>
// #include <unordered_set>
// #include <functional>
// using namespace std;
// 
// int main()
// {
// 	/*
// 	 *	set
// 	 */
// 	cout << "==========set==========" << endl;
// 	// ����set
// 	cout << "���У�3,5,1,4,6" << endl;
// 	unordered_set<int> unorderedSet{ 3,5,1,4,6 };
// 	cout << "unordered_set<int>: ";
// 	for (const auto& elem : unorderedSet)
// 		cout << elem << " ";
// 	cout << endl;
// 
// 	// Ĭ�ϵ�������
// 	set<int> orderedIncrSet(unorderedSet.begin(), unorderedSet.end());
// 	cout << "set<int>: ";
// 	for (const auto& elem : orderedIncrSet)
// 		cout << elem << " ";
// 	cout << endl;
// 
// 	// ����greater(key compare function), �����ݼ�
// 	set<int, greater<int>> orderedDecrSet(unorderedSet.begin(), unorderedSet.end());
// 	cout << "set<int, greater<int>>: ";
// 	for (const auto& elem : orderedDecrSet)
// 		cout << elem << " ";
// 	cout << endl << endl;
// 	
// 
// 	/*
// 	 *	multiset
// 	 */
// 
// 	cout << "==========multiset==========" << endl;
// 	// ����multiset
// 	cout << "���У�1,1,3,5,3,2,2,5" << endl;
// 	unordered_multiset<int> unorderedMultiSet{ 1,1,3,5,3,2,2,5 };
// 	cout << "unordered_multiset<int>: ";
// 	for (const auto& elem : unorderedMultiSet)
// 		cout << elem << " ";
// 	cout << endl;
// 
// 	// ����multiset, Ĭ�ϵ�������
// 	multiset<int> orderedIncrMultiSet(unorderedMultiSet.begin(), unorderedMultiSet.end());
// 	cout << "multiset<int>: ";
// 	for (const auto& elem : orderedIncrMultiSet)
// 		cout << elem << " ";
// 	cout << endl;
// 
// 	// ����greater(key compare function), �����ݼ�
// 	multiset<int, greater<int>> orderedDecrMultiSet(unorderedMultiSet.begin(), unorderedMultiSet.end());
// 	cout << "multiset<int, greater<int>>: ";
// 	for (const auto& elem : orderedDecrMultiSet)
// 		cout << elem << " ";
// 	cout << endl;
// 	
// 
// 	return 0;
// }

/*
 *	�����������
 *	1. forward iterator
	(forward_list, unordered��set,multiset,map,multimap����������)
 *	2. bidirectional iterator
	(list,ordered��set,multiset,map,multimap����������)
 *	3. random-access iterator
	(vector,deque,array,string����������)
 *	4. input iterator
	(input stream��������)
 *	5. output iterator
	(inserter, output stream��������)
 */

// #include <iostream>
// #include <iterator>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <numeric>
// #include <sstream>
// using namespace std;
// 
// int main()
// {
// 	vector<string> coll;
// 	copy(istream_iterator<string>(cin), istream_iterator<string>(), back_inserter(coll));
// 	sort(coll.begin(), coll.end());
// 	unique_copy(coll.cbegin(), coll.cend(), ostream_iterator<string>(cout, " "));
// 
// 	istringstream str("0.1 0.2 0.3 0.4");
// 	partial_sum(istream_iterator<double>(str), istream_iterator<double>(), ostream_iterator<double>(cout, " "));
// 	return 0;
// }