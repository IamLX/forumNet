#include "HttpServer.h"
#include "UrlCodeSolution.h"

using namespace std;

Message HttpServer::RecvRequestData()
{
	cout << "HttpServer::RecvRequestData" << endl;
	Message msg;
	MsgQueueManager::GetInstance().PopMsg("TCPDataRecv", msg);
	return msg;

}

void HttpServer::HttpParser(string data, Http_Header* pthttp_Header)
{
	cout << "HttpServer::HttpParser" << endl;
	string crlf("\r\n"), crlfcrlf("\r\n\r\n");
	size_t prev = 0, next = 0, pos_colon;
	string key, value;

	//����http���������ʼ��
	if ((next = data.find(crlf, prev)) != string::npos)
	{
		string first_line(data.substr(prev, next - prev));
		prev = next;
		stringstream sstream(first_line);
		sstream >> (pthttp_Header->method);
		sstream >> (pthttp_Header->url);
		sstream >> (pthttp_Header->version);
	}
	else
	{
		perror("Error in httpPraser: http_request isn't complete: \\r\\n");
	}
	size_t pos_crlfcrlf;
	if (( pos_crlfcrlf = data.find(crlfcrlf, prev)) == string::npos)
	{
		perror("Error in httpPraser: http_request isn't complete: \\r\\n\\r\\n");
	}

	while (prev != pos_crlfcrlf)
	{
		next = data.find(crlf, prev + 2);
		pos_colon = data.find(":", prev + 2);
		key = data.substr(prev + 2, pos_colon - prev-2);
		value = data.substr(pos_colon + 2, next-pos_colon-2);
		prev = next;
		pthttp_Header->header.insert(pair<string, string>(key, value));
	}
	//body json��ʽ���ֵ���ȡ
	pthttp_Header->body = data.substr(pos_crlfcrlf+4);
	//cout << "pthttp_Header->body" << pthttp_Header->body << endl;
	//pthttp_Header->body.erase(0, 1);
	//return pthttp_Header->body;
}

/*JSON�������� �汾V0.1 ���ڣ�2017.11.17 ������ǰ�İ汾  ���ʹ������2017.12.02*/
/*����TCP���ݣ�JSON���������ཨ�����񣬷ַ�ִ��*/
//void HttpServer::DataHandler(string httpMessage)
//{
//	//cout << "in the function: 'HttpServer::DataHandler' " << endl;
//	//url����
//	string stemp(httpMessage);
//	httpMessage = UrlDecode(stemp);
//	//cout << "log_string decode result:\n" << httpMessage << endl;
//
//	/*JSON Parse*/
//	cJSON *root, *RequestType;
//	root = cJSON_Parse(httpMessage.c_str());
//	if (!root)
//	{
//		printf("cJSON_Parse error!!\n");
//		return;
//	}
//	RequestType = cJSON_GetObjectItem(root,"Type");
//
//	//��׳�ԣ����json��ʽ���ԣ��쳣
//	string tempString = RequestType->valuestring;//RequestType->valuestring;???
//	//HomePage����json����
//	if (tempString == "HomePageRequest")
//	{
//		cJSON *userID, *accessToken;
//
//		//RequestContent = cJSON_GetObjectItem(root, "RequestContent");
//		//cout << "****** Request: homepage_request" << endl;
//		userID = cJSON_GetObjectItem(root, "userID");
//		//cout << "useID:" << userID->valuestring << endl;
//		accessToken = cJSON_GetObjectItem(root, "accessToken");
//		//cout << "accessToken:" << accessToken->valuestring << endl;
//		//json�������������ݽ����������Tsak�����������������
//		
//		//����HomePage����
//		//cout << "****** JOB: build a new Task" << endl;
//		Task homePageRequestTask;
//		homePageRequestTask.TaskType = "HomePageRequest";
//		homePageRequestTask.userID = userID->valuestring;
//		homePageRequestTask.accessToken = accessToken->valuestring;
//
//		AuthenticationServer authenticationServer;
//		if ( authenticationServer.CheckUser(homePageRequestTask))
//		{
//			//cout << "****** TaskExcutor create" << endl;
//			TaskExcutor taskExcutor;
//			_ResponseData = UrlEncode(taskExcutor.Run(homePageRequestTask));	
//		}
//	}
//	else if(tempString == "TopicRequest")
//	{
//
//	}
//	else
//	{
//
//	}
//	cJSON_Delete(root);
//}

/* �汾v0.2 ����2017.12.02 */
void HttpServer::DataHandler(Http_Header* pthttp_Header)
{
	if (pthttp_Header->method == "POST")
	{
		//ҵ���߼�����
		//url����
		string stemp(pthttp_Header->body);		
		string httpBody = UrlDecode(stemp);
		TaskManager taskManager;
		taskManager.CreateTask(httpBody);
		taskManager.TaskScheduling();
		_ResponseData = UrlEncode(taskManager.GetScheduleResult());
		//cout << "debug0" << endl;
	}
}

void HttpServer::CreateResponseHeader()
{
	cout << "HttpServer::CreateResponseHeader" << endl;
	_ResponseHeader = "HTTP/1.1 200 OK\r\nContent-Type:application/json\r\n\r\n";
	//"HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1048576\r\n\r\n123456";
}

void HttpServer::GetResponseBody()
{
}

void HttpServer::SendResonseData()
{
	cout << "HttpServer::SendResonseData" << endl;
	//��TCP���ݲ���MsgMap
	Message httpResponseMsg;
	httpResponseMsg.Setfd(_clientSocket);
	httpResponseMsg.SetData(_ResponseHeader + _ResponseData);
	//cout << "debug123"<< _ResponseData << endl;
	string MapName("TCPDataSend");
	MsgMapManager::GetInstance().InsertMsg(MapName, _clientSocket, httpResponseMsg);
	cout << "_clientSocket: " << _clientSocket << endl;
	cout << "MapSize: " << MsgMapManager::GetInstance().GetMapSize(MapName) << endl;
}

void HttpServer::StartHttpServer()
{
	cout << "StartHttpServer" << endl;
	_HttpServerState = running;
	Message msg;
	Http_Header pthttp_Header;
	while (true)
	{
		//cout << "StartHttpServer" << endl;
		//cout << "+++++ QueueSize: " << MsgQueueManager::GetInstance().GetQueueSize("TCPDataRecv") << endl;
		msg = RecvRequestData();

		++_HttpCount;
		cout << "*************HttpCout********* " << _HttpCount << endl;
		_clientSocket = msg.Getfd();
		//Http���Ľ���������������ͷ���û�����
		HttpParser(msg.GetData(), &pthttp_Header);
		//�û����ݴ�������Json���������õ����������Ȼ�����������л���ֱ�ӽ������ݿ����
		if (pthttp_Header.url == "/FUCK.html")
		{
			//cout << "**********come from test computer" << endl;
			CreateResponseHeader();
			//��ȡ��Ӧ����
			//GetResponseBody();
			//�������ݣ�����MsgMap
			SendResonseData();
			continue;
		}
		else
		{
			//cout << "**********come from android" << endl;
			DataHandler(&pthttp_Header);
			//����Http��Ӧ��ͷ
			//cout << "debug0" << endl;
			CreateResponseHeader();
			//��ȡ��Ӧ����
			//GetResponseBody();
			//�������ݣ�����MsgMap
			SendResonseData();
		}

	}
}

void HttpServer::StopHttpServer()
{
}

void HttpServer::HttpServerCallback(HttpServer * ptHttpServer)
{
	ptHttpServer->StartHttpServer();
}
