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
	condition_variable condEmpty;		//条件变量--队列空
	mutex mEmpty;							//互斥锁--队列空

	condition_variable condFull;			//条件变量--队列满
	mutex mFull;									//互斥锁--队列满
	//condition_variable condEmpty;

public:
	DataQueue() :MAXQUEUESIZE(2) {}
	void push(int i)
	{
		//生产者：等待信号--队列未满 , 然后插入数据后发信号通知 队列非空. 
		unique_lock<mutex> lFull(mFull);
		bool ready = q.size() < MAXQUEUESIZE;
		condFull.wait(lFull, [ready]() {return ready;});
		unique_lock<mutex> lEmpty(mEmpty);
		q.push(i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		cout << "生产者" << this_thread::get_id() << "生产数据:" << i ;
		cout << ",剩余数据" << q.size() << endl;

		condEmpty.notify_one();
	}
	void pop()
	{
		//消费者：等待信号--队列非空 , 然后插入数据后发信号通知 队列非满. 
		unique_lock<mutex> lEmpty(mEmpty);
		bool ready = !q.empty();
		condEmpty.wait(lEmpty, [ready]() {return ready;});
		unique_lock<mutex> lFull(mFull);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "消费者" << this_thread::get_id() << "消费数据:" << q.front();
		q.pop();
		cout << ",剩余数据" << q.size() << endl;
		
		condFull.notify_all();
	}
};

DataQueue myDataQueue;
atomic<int> produceCount;
void produce()
{
	while (1)
	{
		myDataQueue.push(++produceCount);
		this_thread::sleep_for(chrono::seconds(3));
	}
}
void consume()
{
	while (1)
	{
		myDataQueue.pop();
		//this_thread::sleep_for(chrono::microseconds(200));
	}
}
int main()
{
	thread tCosumer[1];
	thread tProducer[1];

	for (int i = 0; i < 1; ++i)
	{
		tProducer[i] = thread(produce);
		tCosumer[i] = thread(consume);		//移动语义 
	}
	for (int i = 0; i < 1; ++i)
	{
		tProducer[i].join();
		tCosumer[i].join();
	}

	return 0;
}