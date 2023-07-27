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

//�۲��߻���
class IListener: public enable_shared_from_this<IListener>
{
public:
	//	@brief:   �¼�����
	virtual void onEvent(EventID id, EventArgs * pArgs)=0;
};

//�¼���������ͨ���������¼��ļ����ʹ���
class EventManager
{
private:
	typedef  list<weak_ptr<IListener>> LISTENER_LIST;
	unordered_map<EventID, LISTENER_LIST> m_listenerInfo;
public:
	//	@brief:   ע���¼�
	void reg(EventID id, weak_ptr<IListener> pListener);

	//	@brief:   �¼�����
	void fire(EventID id, EventArgs * pArgs);
};



//���������
class Listener : public IListener
{
public:
	//	@brief:   ���Խӿ�-����ע���¼�
	void doReg(EventID id, EventManager * pMgr);

	//	@brief:   �¼�����
	virtual void onEvent(EventID id, EventArgs * pArgs);
};


