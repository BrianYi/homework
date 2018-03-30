/*
 *	Clock和Timer
 *	duration(时间段):  指在某时间单位上的一个明确tick(片刻数)
 *	timepoint(时间点): 一个duration和一个epoch(起始点)的组合
 */

// #include <iostream>
// #include <chrono>
// #include <iomanip>
// using namespace std;
// using namespace chrono;
// 
// template <typename T1, typename T2 = ratio<1i64>>
// ostream& operator<< (ostream& out, duration<T1, T2>& d)
// {
// 	out << d.count();
// 	typedef ratio<1, 1000000000> NanoUnit;
// 	typedef ratio<1, 1000000> MicroUnit;
// 	typedef ratio<1, 1000> MiliUnit;
// 	typedef ratio<1> SecondUnit;
// 	typedef ratio<60> MinuteUnit;
// 	typedef ratio<60 * 60> HourUnit;
// 	typedef ratio<24 * 60 * 60> DayUnit;
// 	if (is_same<typename T2, NanoUnit>::value)
// 		cout << " nano";
// 	else if (is_same<typename T2, MicroUnit>::value)
// 		cout << " micro";
// 	else if (is_same<typename T2, MiliUnit>::value)
// 		cout << " mili";
// 	else if (is_same<typename T2, SecondUnit>::value)
// 		cout << " second";
// 	else if (is_same<typename T2, MinuteUnit>::value)
// 		cout << " minute";
// 	else if (is_same<typename T2, HourUnit>::value)
// 		cout << " hour";
// 	else if (is_same<typename T2, DayUnit>::value)
// 		cout << " day";
// 	else
// 		cout << "don't know the type." << endl;
// 	cout << (d.count() > 1 ? "s" : "");
// 	return out;
// }
// 
// int main()
// {
// 	duration<int, nano> sevenNanos(7);
// 	duration<int, micro> fiveMicros(5);
// 	duration<int, milli> twelveMilis(12);
// 	duration<int> sixtySeconds(60);
// 	duration<int, ratio<60>> fiveMinutes(5);
// 	duration<int, ratio<3600>> oneHour(1);
// 	duration<int, ratio<24 * 3600>> oneDay(1);
// 	cout << "==========duration基本用法==========" << endl;
// 	cout << sevenNanos << " is " << sevenNanos << " seconds" << endl;
// 	cout << fiveMicros << " is " << " seconds" << endl;
// 	cout << twelveMilis << " is " << " seconds" << endl;
// 	cout << sixtySeconds << " is " << seconds(sixtySeconds).count() << " seconds" << endl;
// 	cout << fiveMinutes << " is " << seconds(fiveMinutes).count() << " seconds" << endl;
// 	cout << oneHour << " is " << seconds(oneHour).count() << " seconds" << endl;
// 	cout << oneDay << " is " << seconds(oneDay).count() << " seconds" << endl;
// 	cout << endl;
// 
// 	// duration_cast
// 	cout << "==========duration_cast==========" << endl;
// 	cout << oneDay << " is: " << endl;
// 	cout << duration_cast<hours>(oneDay).count() << " hours\n"
// 		<< duration_cast<seconds>(oneDay).count() << " seconds\n";
// 	cout << endl;
// 
// 	// print time format
// 	cout << "==========time format==========" << endl;
// 	seconds secs(hours(24) + minutes(12) + seconds(20));
// 	hours hh = duration_cast<hours>(secs);
// 	minutes mm = duration_cast<minutes>(secs % hours(1));
// 	seconds ss = duration_cast<seconds>(secs % minutes(1));
// 	cout << secs << " is " << setfill('0')
// 		<< setw(2) << hh.count() << "::"
// 		<< setw(2) << mm.count() << "::"
// 		<< setw(2) << ss.count() << endl;
// 	return 0;
// }


/*
 *	1. system_clock
 *	2. steady_clock
 *	3. high_resolution_clock
 *	to_time_t 将timepoint转换为一个传统C和POSIX提供的时间类型time_t的对象, 通常为自UNIX epoch起, 自1970年1月1日起开始计算的秒数
 *	ctime将local time zone纳入考虑, 只有system_clock具有to_time_t函数
 *	asctime则是UTC时间(全球标准时间)
 */
// #include <iostream>
// #include <chrono>
// #include <string>
// using namespace std;
// using namespace chrono;
// 
// void precision_type(long long den)
// {
// 	if (den >= nano::den)
// 		cout << double(nano::den) / den << " nanoseconds";
// 	else if (den >= micro::den)
// 		cout << double(micro::den) / den << " microseconds";
// 	else if (den >= milli::den)
// 		cout << double(milli::den) / den << " milliseconds";
// 	else if (den >= centi::den)
// 		cout << double(centi::den) / den << " centiseconds";
// 	else if (den >= deci::den)
// 		cout << double(deci::den) / den << " denseconds";
// 	else if (den == 1)
// 		cout << " seconds";
// 	else
// 		cout << " don't know the type.";
// 	cout << endl;
// }
// 
// int main()
// {
// 	// Clock(时钟)
// 	cout << boolalpha;
// 	cout << "==========Clock Precision==========" << endl;
// 	cout << "system_clock: " << system_clock::period::num << "/" << system_clock::period::den << " seconds, ";
// 	precision_type(system_clock::period::den);
// 	cout << "steady ? " << system_clock::is_steady << endl;
// 	cout << "high_resolution_clock: " << high_resolution_clock::period::num << "/" << high_resolution_clock::period::den << " seconds, ";
// 	precision_type(high_resolution_clock::period::den);
// 	cout << "steady ? " << high_resolution_clock::is_steady << endl;
// 	cout << "steady_clock: " << steady_clock::period::num << "/" << steady_clock::period::den << " seconds, ";
// 	precision_type(steady_clock::period::den);
// 	cout << "steady ? " << steady_clock::is_steady << endl;
// 	cout << endl;
// 
// 	time_t t;
// 	char buf[512] = {};
// 	cout << "==========to_time_t or from_time_t==========" << endl;
// 	t = system_clock::to_time_t(system_clock::time_point());
// 	ctime_s(buf, sizeof(buf), &t);
// 	cout << "[time_point]system_clock[calendar time]: " << buf << endl;
// 
// 	t = system_clock::to_time_t(system_clock::now());
// 	ctime_s(buf, sizeof(buf), &t);
// 	cout << "[now]system_clock[calendar time]: " << buf << endl;
// 	
// 	time_point<high_resolution_clock>().time_since_epoch();
// 	/* 不知道为啥会崩溃, 基本跟书上的代码一样 */
// // 	t = system_clock::to_time_t(system_clock::time_point::min());
// // 	ctime_s(buf, sizeof(buf), &t);
// // 	cout << "[time_point::min]system_clock[calendar time]: " << buf << endl;
// 
// 	/* 不知道为啥会崩溃, 基本跟书上的代码一样 */
// // 	t = system_clock::to_time_t(system_clock::time_point::max());
// // 	ctime_s(buf, sizeof(buf), &t);
// // 	cout << "[time_point::max]system_clock[calendar time]: " << buf << endl;
// 	return 0;
// }


/*
 *	time_point用法, to_time_t用法
 */
// #include <chrono>
// #include <iostream>
// #include <string>
// #include <ctime>
// using namespace std;
// using namespace chrono;
// 
// string asString(const system_clock::time_point& tp)
// {
// 	time_t t = system_clock::to_time_t(tp);
// 	string ts = ctime(&t);
// 	ts.resize(ts.size() - 1);
// 	return ts;
// }
// 
// int main()
// {
// 	typedef duration<int, ratio<24 * 3600>> Day;
// 	
// 	// process the epoch of this system clock
// 	time_point<system_clock> tp;
// 	cout << "epoch: " << asString(tp) << endl;
// 
// 	// add one day, 23 hours, 55 minutes
// 	tp += Day(1) + hours(23) + minutes(55);
// 	cout << "later: " << asString(tp) << endl;
// 
// 	// process difference from epoch in minutes and days
// 	auto diff = tp - system_clock::time_point();
// 	cout << "diff: "
// 		<< duration_cast<seconds>(diff).count() << " seconds = "
// 		<< duration_cast<minutes>(diff).count() << " minutes(about) = "
// 		<< duration_cast<hours>(diff).count() << " hours(about) = "
// 		<< duration_cast<Day>(diff).count() << " days(about)" << endl;
// 
// 	return 0;
// }

/*
 *	mktime, from_time_t
 */