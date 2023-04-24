## 随机数库
[c++11随机数库](https://blog.csdn.net/CSDNwei/article/details/113865349)
C++11提供了随机数引擎和分布类。   引擎有若干中生成随机数方式，可以生成一个随机数。分布类可以按照正太分布、均匀分布等方式生成一个对应分布下的随机数。

以前常用的rand可以看做是随机数，但是在rand() % m用于生成0，m-1的随机数时就会出错，此时分布有问题。用此方式生成的随机数不随机了。
c++11中提供了分布类，用于更好生成各种分布。
注意：
1：在函数中定义引擎的话，最好就定义一次，在全局地方或者设置为局部变量
2：引擎种子可以用真随机数 std::rand_device.
测试改动
下面代码可以用于测试随机情况:

```

#include<iostream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <list>
#include <map>
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
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <random>
#include <windows.h>  
using namespace std;
int maxCount = 100000000;
int needProp = 2500;
int maxProp = 10000;
void RandTest()
{
	int meetCount = 0;
	for (int i = 0; i < maxCount ; ++i)
	{
		if (rand() % maxProp < needProp)
		{
			meetCount++;
		}
	}
	cout << "rand:" <<meetCount * 1.0 / maxCount * maxProp << endl;
}

void EngineRand()
{
	static bool randSeed = false;
	static default_random_engine e;
	static uniform_int_distribution<int> u(0,maxProp-1);
	if (!randSeed)
	{
		randSeed = true;
		e.seed(std::random_device());
	}
	int meetCount = 0;
	for (int i = 0; i < maxCount ; ++i)
	{
		if (u(e) < needProp)
		{
			meetCount++;
		}
	}
	cout << "EngineRand:" <<meetCount * 1.0 / maxCount * maxProp<< endl;
}
int main()
{
	srand(time(nullptr));
	cout << "输入循环测试数量:";
	cin >> maxCount;
	cout << "输入总概率:";
	cin >> maxProp;
	while (true)
	{
		cout << endl << "输入要计算的概率:"; 
		cin >> needProp ;
		RandTest();
		EngineRand();
	}
	
	EngineRand();

	return 0;
}
```
