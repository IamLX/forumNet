#include "TaskExcutor.h"



TaskExcutor::TaskExcutor()
{
	
}


TaskExcutor::~TaskExcutor()
{
}

vector<HomePageContentStr> TaskExcutor::GetDataFromDB(const Task& task)
{
	//��ʱģ�����ݿ����
	//��ʱ����
	//cout << "in the function: 'TaskExcutor::GetDataFromDB' " << endl;
	//select
	map<string, int> typeToSize;
	typeToSize["HomePageRequest"] = 3;
	typeToSize["HomePageRefresh"] = 2;
	typeToSize["HomePageLoadMore"] = 3;

	int userId = atoi(task.userID.c_str());
	int contentSize = typeToSize[task.taskType];

	HomePageContentStr homePageContentStr;
	vector<HomePageContentStr> homePageContentArr(contentSize, homePageContentStr);
	for (int i = 0; i < contentSize; ++i)
	{
		//cout << "Debug DB" << contentSize << endl;
		homePageContentArr[i].username = "����������";
		homePageContentArr[i].userprofile = "��������ϢѧԺ/������ҫ������";
		
		if (task.taskType == "HomePageRequest")
			homePageContentArr[i].title = "Ϊʲô�����������ϵ���";
		else if(task.taskType == "HomePageRefresh")
			homePageContentArr[i].title = "RefreshΪʲô�����������ϵ���";
		else
			homePageContentArr[i].title = "LoadMoreΪʲô�����������ϵ���";
		
		homePageContentArr[i].profile = "��Ϊһ��ǿ�ط�ʦ������Ϊ��������ȫ������ʤ���ϵ���λ�ã�Ϊʲô��һ���ϵ����Ѿ����ҿӡ��������������ܣ��������Ķ�λ�Ƿ�ʦ���ڸİ��ǿ�󣬰������˺���Ҫ��Դ�ڼ��ܵ��ͷ��Լ������ĵ�������Ч���������ڳ�װ�ϣ�׷�����Ȼ�Ǹ߷���������װ�����ر����ܹ���ϱ���Ч����װ�����Կ����Լ۱���ߡ�";
		homePageContentArr[i].comments = "23";
		homePageContentArr[i].replys = "4";
		homePageContentArr[i].care = "26";
		homePageContentArr[i].resourceId = "12345";
	}
	return homePageContentArr;
}

string TaskExcutor::Run(Task task)
{
	//cout << "in the function: 'TaskExcutor::Run' " << endl;
	if (task.taskType == "HomePageRequest" || 
		task.taskType == "HomePageRefresh" ||
		task.taskType == "HomePageLoadMore")
	{
		//cout << "task.taskType" << task.taskType << endl;
		return CreateHomePage(task);
	}
	else if(task.taskType == "DynamicPageRequest")
	{
		return CreateDynamicPage(task);
	}
	else
	{
		string responseData("Type error");
		return responseData;
	}
}

/*����HomePage*/
string TaskExcutor::CreateHomePage(const Task& task)
{
	vector<HomePageContentStr> homePageContentArr;
	homePageContentArr = GetDataFromDB(task);
	int contentSize = homePageContentArr.size();
	cJSON *homePageContent, *contentArray;
	homePageContent = cJSON_CreateObject();
	if (!homePageContent)
	{
		cout << "construct json homePageContent fail" << endl;
	}
	cJSON_AddItemToObject(homePageContent, "type", cJSON_CreateString("HomePageResponse"));
	contentArray = cJSON_CreateArray();
	if (!contentArray)
	{
		cout << "build json child fail" << endl;
	}
	cJSON_AddItemToObject(homePageContent, "Content", contentArray);

	//����homePageContent
	cJSON* pContentArr[contentSize];
	for (int i = 0; i < contentSize; ++i)
	{
		//�����ݿ��ȡ�����ҳ���ݽṹ��
		//GetDataFromDB(homePageContentStr);
		//cout << "����homePageContent" << endl;
		pContentArr[i] = cJSON_CreateObject();
		if (!pContentArr[i])
		{
			cout << "build json pContentArr[i] fail" << endl;
		}
		//��pContentArr[i]��������
		cJSON_AddItemToObject(pContentArr[i], "username", cJSON_CreateString(homePageContentArr[i].username.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "userprofile", cJSON_CreateString(homePageContentArr[i].userprofile.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "title", cJSON_CreateString(homePageContentArr[i].title.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "profile", cJSON_CreateString(homePageContentArr[i].profile.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "comments", cJSON_CreateString(homePageContentArr[i].comments.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "replys", cJSON_CreateString(homePageContentArr[i].replys.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "care", cJSON_CreateString(homePageContentArr[i].care.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "resourceId", cJSON_CreateString(homePageContentArr[i].resourceId.c_str()));
		//��contnet����contentArray����
		cJSON_AddItemToArray(contentArray, pContentArr[i]);
	}
	//homePageContentת��Ϊ�ַ���������۲�
	char *pts_HomePageContent;
	pts_HomePageContent = cJSON_PrintUnformatted(homePageContent);
	string HomePageContent = pts_HomePageContent;
	//cout << "return homepage data:" << endl;
	//cout << cJSON_Print(homePageContent) << endl;
	free(pts_HomePageContent);
	cJSON_Delete(homePageContent);
	return HomePageContent;
}

string TaskExcutor::CreateDynamicPage(const Task & task)
{
	return string();
}
