#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <functional>
#include <pthread.h>
#include "cJSON.h"
#include "Task.h"
#include "Message.h"
#include "MsgQueueManager.h"
#include "MsgMapManager.h"
#include "TaskExcutor.h"
#include "TaskManager.h"



typedef struct _Http_Header {
	string method;
	string url;
	string version;
	map<string, string> header;
	string body;
}Http_Header;

using namespace std;
class HttpServer
{
public:
	HttpServer() : _HttpServerState(stop), _HttpCount(0) {}
	virtual ~HttpServer() {}
	//��TcpData���л�ȡ����Message
	Message RecvRequestData();
	//Http���Ľ���������������ͷ���û�����
	void HttpParser(string data, Http_Header* pthttp_Header);
	//�û����ݴ�������Json���������õ����������Ȼ�����������л���ֱ�ӽ������ݿ����
	//void DataHandler(string httpMessage);
	void DataHandler(Http_Header* pthttp_Header);
	//����Http��Ӧ��ͷ
	void CreateResponseHeader();
	//��ȡ��Ӧ����
	void GetResponseBody();
	//�������ݣ�����MsgMap
	void SendResonseData();

	void StartHttpServer();
	void StopHttpServer();
	static void HttpServerCallback(HttpServer* ptHttpServer);

private:
	long long _HttpCount;
	int _clientSocket;
	string _RequestHeader;
	string _RequestData;
	string _ResponseHeader;
	string _ResponseData;
	string _HttpServerName;

	enum HttpServerState
	{
		stop, running
	}_HttpServerState;


};

