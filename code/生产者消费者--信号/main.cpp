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
	condition_variable condNotEmpty;		//��������--���зǿ�
	mutex m;												//������--������з���
	condition_variable condNotFull;			//��������--���в���
public:
	DataQueue() :MAXQUEUESIZE(2) {}
	void push(int i)
	{
		//�����ߣ��ȴ��ź�--����δ�� , Ȼ��������ݺ��ź�֪ͨ ���зǿ�. 
		unique_lock<mutex> lock(m);
		while (q.size() >= MAXQUEUESIZE)
		{
			condNotFull.wait(lock);
		}
		q.push(i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		cout << "������" << this_thread::get_id() << "��������:" << i << ",ʣ������" << q.size() << endl;
		condNotEmpty.notify_all();
	}
	void pop()
	{
		//�����ߣ��ȴ��ź�--���зǿ� , Ȼ��������ݺ��ź�֪ͨ ���з���. 
		unique_lock<mutex> lock(m);
		while (q.empty())
		{
			condNotEmpty.wait(lock);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "������" << this_thread::get_id() << "��������:" << q.front() << ",ʣ������" << q.size() - 1 << endl;
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
	cout << "�����������߳�������";
	cin >> threadNumConsumer;
	cout << "�����������߳�������";
	cin >> threadNumProducer;

	for (int i = 0; i < threadNumConsumer; ++i)
	{
		vecConsumer.emplace_back(thread(consume));
	}
	for (int i = 0; i < threadNumProducer; ++i)
	{
		vecProducer.emplace_back(thread(produce));
	}

	//�ȴ�
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