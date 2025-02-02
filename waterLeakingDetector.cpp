#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<unordered_map>
#include<cstdio>
#include<sstream>
#include<ctime>
#include<functional>
using namespace std;

#define wData pair<string, double>
#define ump unordered_map
#define mp make_pair

const double minPrec = 1e-1;
vector<wData> userData;

struct tm_hash {
	size_t operator()(const tm& t) const {
		return std::hash<int>()(t.tm_year) ^
			std::hash<int>()(t.tm_mon) ^
			std::hash<int>()(t.tm_mday) ^
			std::hash<int>()(t.tm_hour) ^
			std::hash<int>()(t.tm_min) ^
			std::hash<int>()(t.tm_sec);
	}
};
struct tm_equal {
	bool operator()(const tm& lhs, const tm& rhs) const {
		return lhs.tm_year == rhs.tm_year &&
			lhs.tm_mon == rhs.tm_mon &&
			lhs.tm_mday == rhs.tm_mday &&
			lhs.tm_hour == rhs.tm_hour &&
			lhs.tm_min == rhs.tm_min &&
			lhs.tm_sec == rhs.tm_sec;
	}
};

double getIdleFlow(vector<wData>& data) {
	ump<double, int> freq;

	double nowPrec = 1e-9;
	double idleFlow = 0.0;
	double lstFreq = 1e9;
	double lstFrac = -1e9;

	while (nowPrec <= minPrec) {
		freq.clear();

		for (auto v : data) {
			double rv = round(v.second / nowPrec) * nowPrec;
			freq[rv]++;
		}

		double nowIF = 0.0;
		int maxFreq = 0;
		for (auto v : freq) {
			if (v.second > maxFreq) {
				nowIF = v.first;
				maxFreq = v.second;
			}
		}

		if (maxFreq / lstFreq > lstFrac) {
			lstFrac = double(maxFreq / lstFreq);
			idleFlow = nowIF;
		}

		//cout << "nowIF: " << nowIF << ", maxFreq: " << maxFreq << endl;
		//cout << maxFreq / lstFreq << endl;

		lstFreq = maxFreq * 1.0;
		nowPrec *= 10.0;
	}
	return idleFlow;
}

tm stringToTime(string timeStr) {
	tm tmTime = {};
	int mm, dd, yy, hh, min, sec;

	sscanf(timeStr.c_str(), "%d/%d/%d %d:%d:%d", &mm, &dd, &yy, &hh, &min, &sec);

	tmTime.tm_mon = mm - 1;
	tmTime.tm_mday = dd;
	tmTime.tm_year = yy;
	tmTime.tm_hour = hh;
	tmTime.tm_min = min;
	tmTime.tm_sec = sec;
	return tmTime;
}

unordered_map<tm, vector<wData>, tm_hash, tm_equal> days;

int main() {
	FILE* file = freopen("data.txt", "r", stdin);
	if (!file) {
		cout << "Failed to open data.txt. Please check the file path and try again." << endl;
		fclose(stdin);
		freopen("CON", "r", stdin);
		system("pause");
		return -1; 
	}

	cout << "Fetching data..." << endl;
	int cnt = 0;
	string l;
	while (getline(cin, l)) {
		stringstream ss(l);
		string date;
		double v;
		getline(ss, date, ',');
		ss >> v;
		userData.push_back(mp(date, v));
		cnt++;
		if (cnt % 100000 == 0) {
			cout << "Currently " << cnt << " data points have been loaded.\r";
		}
	}
	system("cls");
	fclose(stdin);
	freopen("CON", "r", stdin);

	cout << "Sorting data..." << endl;
	cnt = 0;
	for (auto v : userData) {
		tm tmp = stringToTime(v.first);
		tmp.tm_hour = tmp.tm_min = tmp.tm_sec = 0;
		days[tmp].push_back(v); cnt++;
		if (cnt % 100000 == 0) {
			cout << "Currently " << cnt << " data points have been sorted.\r";
		}
	}
	system("cls");

	double lstIdelFlow = 114514.0;

	for (auto day : days) {
		cout << "Calculating " << day.first.tm_year << "/" << day.first.tm_mon << "/" << day.first.tm_mday;
		double now = getIdleFlow(day.second);
		cout << "\tidelFlow: " << now;
		if (lstIdelFlow == 114514) {
			cout << "\t+0%" << endl;
		}
		else {
			cout << '\t';
			double delta = (now - lstIdelFlow) / lstIdelFlow;
			cout << (delta > 0 ? "\033[31m+" : "\033[32m") << delta * 100 << "%\t\033[0m";
			if (now / lstIdelFlow >= 3) {
				cout << "\033[31mWARNING WARNING WARNING\033[0m";
			}
			//for (int i = 0; i < now / lstIdelFlow; i++) {
			//	cout << ((lstIdelFlow < now) ? "\033[31m■\033[0m" : "\033[32m■\033[0m");
			//}
			cout << endl;
		}
		lstIdelFlow = now;
	}
	system("pause");
	return 0;
}
