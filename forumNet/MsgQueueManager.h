#pragma once
#include <map>
#include "MsgQueue.h"
#include "Message.h"

using namespace std;

/*��������Ϣ����*/
class MsgQueueManager
{
public:
	//��ȡ����ʵ��
	static MsgQueueManager& GetInstance();
	MsgQueueManager(const MsgQueueManager&) = delete;
	const MsgQueueManager& operator=(const MsgQueueManager&) = delete;
	virtual ~MsgQueueManager();
	void Register(string name, MsgQueue* msgQueue);
	void PostMsg(string name, Message message);
	void PopMsg(string name, Message& message);
	int GetQueueSize(string name);

private:
	std::map <string, std::shared_ptr<MsgQueue>> _MsgQueues;
	//������
	MsgQueueManager();
};

