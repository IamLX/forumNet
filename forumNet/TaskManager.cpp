#include "TaskManager.h"

using namespace std;


TaskManager::~TaskManager()
{
}

void TaskManager::CreateTask(string httpBody)
{
	//cout << "in the function: 'TaskManager::CreateTask' " << endl;
	string key, value;
	map<string, string> requestForm;
	size_t pos_and = 0, pos_equal = 0, prev = 0, next = 0;
	string key_array[5] = { "type", "id", "accessToken", "resourceId", "resourceType"};
	/*"""type""=""HomePageRequest""
	""id""=
	""accessToken""
	""resourceId"" = ""NoID""
	��resourceType��=""NoTYPE"""
	*/
	for (int i = 0; i < 5; ++i)
	{
		key = key_array[i];
		prev = httpBody.find(key, 0);
		pos_equal = httpBody.find("=", prev);
		if ((pos_and = httpBody.find("&", prev)) != string::npos)
		{
			value = httpBody.substr(pos_equal + 1, pos_and - pos_equal - 1);
		}
		else
		{
			value = httpBody.substr(pos_equal + 1);
		}
		cout << "value"<< value << endl;
		requestForm.insert(pair<string, string>(key, value));
	}
	Task task;
	task.taskType = requestForm["type"];
	task.userID = requestForm["id"];
	task.accessToken = requestForm["accessToken"];
	task.resourceId = requestForm["resourceId"];
	task.resourceType = requestForm["resourceType"];
	
	_taskQueue.push(task);
}

/*������ȣ�ѡ�����񣬽���ִ����*/
void TaskManager::TaskScheduling()
{
	//cout << "in the function: 'TaskManager::TaskScheduling' " << endl;
	Task task = _taskQueue.front();
	_taskQueue.pop();
	//��֤����
	AuthenticationServer authenticationServer;
	if (authenticationServer.CheckUser(task))
	{
		TaskExcutor taskExcutor;
		string responseData = taskExcutor.Run(task);
		_scheduleResult.push(responseData);
	}
	else
	{
		//���µ�½
		string responseData = "post info error,try again";
		_scheduleResult.push(responseData);
	}

}

string TaskManager::GetScheduleResult()
{
	//cout << "in the function: 'TaskManager::GetScheduleResult' " << endl;
	string scheduleResult = _scheduleResult.front();
	_scheduleResult.pop();
	return scheduleResult;
}
