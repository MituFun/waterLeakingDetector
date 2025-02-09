#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

// 生成模拟时间戳
pair<tm, string> generateTimestamp(int second) {
	time_t baseTime = time(nullptr); // 当前时间为基准
	baseTime += second; // 增加秒数
	tm* timeInfo = localtime(&baseTime); // 转换为本地时间

	ostringstream timestamp;
	timestamp << setfill('0') << setw(2) << (timeInfo->tm_mon + 1) << "/"
		<< setw(2) << timeInfo->tm_mday << "/"
		<< (timeInfo->tm_year + 1900) << " "
		<< setw(2) << timeInfo->tm_hour << ":"
		<< setw(2) << timeInfo->tm_min << ":"
		<< setw(2) << timeInfo->tm_sec;
	return make_pair(*timeInfo, timestamp.str());
}

// 模拟水表数据的生成
vector<pair<pair<tm, string>, double>> generateWaterMeterData(int totalSeconds, double idleFlow, double maxUserFlow, int userActivityRate) {
	data.reserve(totalSeconds);
	srand(time(nullptr)); // 随机种子

	int day = -1;
	for (int i = 0; i < totalSeconds; ++i) {
		double currentFlow = idleFlow; // 默认是空转流量
		if (rand() % 100 < userActivityRate) {
			// 随机决定是否模拟用户用水
			double userFlow = idleFlow + (rand() / (double)RAND_MAX) * (maxUserFlow - idleFlow);
			currentFlow += userFlow;
		}
		// 模拟测量误差，加入微小波动
		double measurementNoise = ((rand() / (double)RAND_MAX) - 0.5) * 0.001; // ±0.0005 的噪声
		currentFlow += measurementNoise;

		// 生成时间戳并存储
		data.emplace_back(generateTimestamp(i), currentFlow);
		cout << entry.first.second << "," << fixed << setprecision(8) << entry.second << endl;
		if (data.back().first.first.tm_mday != day) {
			cout << "Generating " << data.back().first.first.tm_mon + 1 << "/" << data.back().first.first.tm_mday << "/" << data.back().first.first.tm_year + 1900 << "\r";
			day = data.back().first.first.tm_mday;
		}
	}
	return data;
}

// 主函数
int main() {
	int totalSeconds = 10000000; // 总共生成的数据点数（秒）
	double idleFlow = 0.3; // 空转流量
	double maxUserFlow = 5.0; // 最大用户用水流量
	int userActivityRate = 20; // 用户用水的发生概率（百分比）

	// 生成水表数据
	vector<pair<pair<tm, string>, double>> data = generateWaterMeterData(totalSeconds, idleFlow, maxUserFlow, userActivityRate);

	freopen("Data.txt", "w", stdout);
	int day = -1;
	// 打印水表数据
	for (const auto& entry : data) {
		if (entry.first.first.tm_mday != day) {
			fclose(stdout);
			freopen("CON", "w", stdout);
			cout << "Generating " << entry.first.first.tm_mon + 1 << "/" << entry.first.first.tm_mday << "/" << entry.first.first.tm_year + 1900 << "\r";
			freopen("Data.txt", "w", stdout);
			day = entry.first.first.tm_mday;
		}
	}

	return 0;
}
