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
	condition_variable condEmpty;		//��������--���п�
	mutex mEmpty;							//������--���п�

	condition_variable condFull;			//��������--������
	mutex mFull;									//������--������
	//condition_variable condEmpty;

public:
	DataQueue() :MAXQUEUESIZE(2) {}
	void push(int i)
	{
		//�����ߣ��ȴ��ź�--����δ�� , Ȼ��������ݺ��ź�֪ͨ ���зǿ�. 
		unique_lock<mutex> lFull(mFull);
		bool ready = q.size() < MAXQUEUESIZE;
		condFull.wait(lFull, [ready]() {return ready;});
		unique_lock<mutex> lEmpty(mEmpty);
		q.push(i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		cout << "������" << this_thread::get_id() << "��������:" << i ;
		cout << ",ʣ������" << q.size() << endl;

		condEmpty.notify_one();
	}
	void pop()
	{
		//�����ߣ��ȴ��ź�--���зǿ� , Ȼ��������ݺ��ź�֪ͨ ���з���. 
		unique_lock<mutex> lEmpty(mEmpty);
		bool ready = !q.empty();
		condEmpty.wait(lEmpty, [ready]() {return ready;});
		unique_lock<mutex> lFull(mFull);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "������" << this_thread::get_id() << "��������:" << q.front();
		q.pop();
		cout << ",ʣ������" << q.size() << endl;
		
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
		tCosumer[i] = thread(consume);		//�ƶ����� 
	}
	for (int i = 0; i < 1; ++i)
	{
		tProducer[i].join();
		tCosumer[i].join();
	}

	return 0;
}