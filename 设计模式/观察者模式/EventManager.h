#pragma once
#include <iostream>
#include <functional>
#include <unordered_map>
#include <list>
using namespace std;

typedef int EventID;
struct EventArgs
{
	int argsLens;
	void * args;
};

enum enEventID
{
	enEventID_1,
	enEventID_2,
};

//观察者基类
class IListener: public enable_shared_from_this<IListener>
{
public:
	//	@brief:   事件到来
	virtual void onEvent(EventID id, EventArgs * pArgs)=0;
};

//事件管理器，通过它进行事件的监听和触发
class EventManager
{
private:
	typedef  list<weak_ptr<IListener>> LISTENER_LIST;
	unordered_map<EventID, LISTENER_LIST> m_listenerInfo;
public:
	//	@brief:   注册事件
	void reg(EventID id, weak_ptr<IListener> pListener);

	//	@brief:   事件触发
	void fire(EventID id, EventArgs * pArgs);
};



//具体监听者
class Listener : public IListener
{
public:
	//	@brief:   测试接口-用于注册事件
	void doReg(EventID id, EventManager * pMgr);

	//	@brief:   事件触发
	virtual void onEvent(EventID id, EventArgs * pArgs);
};


