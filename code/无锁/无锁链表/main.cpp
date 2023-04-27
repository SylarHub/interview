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

template<typename T>
class LockFreeList
{
private:
	struct node
	{
		T val;
		struct node * next;
		node(const T& v)
		{
			val = v;
			next = nullptr;
		}
	};
	atomic<node *> m_head;		//头节点
	atomic<int> m_count;		//节点数量

public:
	void casPut(T val)
	{
		node * pNewNode = new node(val);
		pNewNode->next = m_head.load();
		while (!m_head.compare_exchange_weak(pNewNode->next, pNewNode));
		++m_count;
	}
	size_t size()const { return m_count; }
};

LockFreeList<int> lfDdata;
atomic<int> putValue;
void fun(int count)
{
	for (int i = 0; i < count; ++i)
	{
		lfDdata.casPut(i);
	}
}
int main()
{
	int num = 0;
	int count = 0;
	cin >> num >> count;
	vector<thread> vecThread;
	for (int i = 0; i < num; ++i)
	{
		vecThread.push_back(thread(fun,count));
	}
	for (int i = 0; i < num; ++i)
	{
		vecThread[i].join();
	}

	cout << "队列总长度:" << lfDdata.size()<< endl;

	return 0;
}