#include<iostream>
//
using namespace std;
#include <stdlib.h>
#include <ctime>
////#include <windows.h>
////#include <winsock.h>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <bitset>
#include <stack>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <sys/types.h>
#include <stdarg.h>
#include <cmath>
#include <set>
#include <crtdbg.h>
#include "assert.h"
#include <deque>
#include<queue>
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
//#include "def.h"
#include <functional>
#include "Solution.h"
#include <condition_variable>
#include <thread>
#include "windows.h"
#include <atomic>
using namespace std;

class DataQueue
{
private:
	queue<int> q;
	const int MAXQUEUESIZE;
	condition_variable condNotEmpty;		//条件变量--队列非空
	mutex m;												//互斥锁--互斥队列访问
	condition_variable condNotFull;			//条件变量--队列不满
public:
	DataQueue() :MAXQUEUESIZE(2) {}
	void push(int i)
	{
		//生产者：等待信号--队列未满 , 然后插入数据后发信号通知 队列非空. 
		unique_lock<mutex> lock(m);
		while (q.size() >= MAXQUEUESIZE)
		{
			condNotFull.wait(lock);
		}
		q.push(i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		cout << "生产者" << this_thread::get_id() << "生产数据:" << i << ",剩余数据" << q.size() << endl;
		condNotEmpty.notify_all();
	}
	void pop()
	{
		//消费者：等待信号--队列非空 , 然后插入数据后发信号通知 队列非满. 
		unique_lock<mutex> lock(m);
		while (q.empty())
		{
			condNotEmpty.wait(lock);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "消费者" << this_thread::get_id() << "消费数据:" << q.front() << ",剩余数据" << q.size() - 1 << endl;
		q.pop();
		condNotFull.notify_all();
	}
};

DataQueue myDataQueue;
atomic<int> produceCount;
void produce()
{
	while (1)
	{
		myDataQueue.push(++produceCount);
		this_thread::sleep_for(chrono::microseconds(200));
	}
}
void consume()
{
	while (1)
	{
		myDataQueue.pop();
		this_thread::sleep_for(chrono::microseconds(200));
	}
}
#define THREAD_NUM 3
int main()
{
	vector<thread> vecConsumer;
	vector<thread> vecProducer;
	int threadNumConsumer;
	int threadNumProducer;
	cout << "输入消费者线程数量：";
	cin >> threadNumConsumer;
	cout << "输入生产者线程数量：";
	cin >> threadNumProducer;

	for (int i = 0; i < threadNumConsumer; ++i)
	{
		vecConsumer.emplace_back(thread(consume));
	}
	for (int i = 0; i < threadNumProducer; ++i)
	{
		vecProducer.emplace_back(thread(produce));
	}

	//等待
	for (int i = 0; i < threadNumConsumer; ++i)
	{
		vecConsumer[i].join();
	}
	for (int i = 0; i < threadNumProducer; ++i)
	{
		vecProducer[i].join();
	}

	return 0;
}