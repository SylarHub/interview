#include "EventManager.h"



void EventManager::fire(EventID id, EventArgs * pArgs)
{
	auto iter = m_listenerInfo.find(id);
	if (iter != m_listenerInfo.end())
	{
		LISTENER_LIST & l = iter->second;
		for (auto iterFun = l.begin(); iterFun != l.end();)
		{
			shared_ptr<IListener> p = (*iterFun).lock();
			if (p)
			{
				p->onEvent(id, pArgs);
				++iterFun;
			}
			else
			{
				//指针失效
				l.erase(iterFun++);
			}
		}
	}
}
void EventManager::reg(EventID id, weak_ptr<IListener> pListener)
{
	auto iter = m_listenerInfo.find(id);
	if (iter != m_listenerInfo.end())
	{
		LISTENER_LIST & l = iter->second;
		l.push_back(pListener);
	}
	else
	{
		LISTENER_LIST l;
		l.push_back(pListener);
		m_listenerInfo[id] = l;
	}
}

void Listener::doReg(EventID id, EventManager * pMgr)
{
	if (!pMgr) return;
	weak_ptr<IListener> w = weak_from_this();
	pMgr->reg(id, w);
}

void Listener::onEvent(EventID id, EventArgs * pArgs)
{
	cout << __FUNCTION__ <<"事件触发，id=" << id << endl;
}

/*
外部调用代码:
	EventManager manager;
	
	shared_ptr<Listener> p1(new Listener);
	p1->doReg(enEventID_1, &manager);

	manager.fire(enEventID_1, nullptr);
*/

