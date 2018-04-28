//#include <iostream>
//#include <algorithm>
//#include <list>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <iterator>
//#include <functional>
//using namespace std;
//
//int main()
//{
//	ifstream infile("wordlist.txt", ios::in);
//	ofstream outfile("longest.txt", ios::out | ios::trunc);
//	string str;
//	list<string> wordlist;
//	while (getline(infile, str, '\n'))
//	{
//		stringstream strstream(str);
//		istream_iterator<string> iter(strstream);
//		istream_iterator<string> iter_end;
//		while (iter != iter_end)
//		{
//			string word = *iter;
//			word.resize(word.length() - 1);
//			if (all_of(word.begin(), word.end(), ::isdigit))
//			{
//				advance(iter, 1);
//				wordlist.push_back(*iter);
//				break;
//			}
//		}
//		wordlist.sort(greater<string>());
//	}
//
//	infile.close();
//	outfile.close();
//}