#include <stdio.h>
#include <Windows.h>
#include "..\include\fly_web.h" 
#include "..\include\fly_string.h" 
#pragma comment(lib, "..\\libs\\debug\\flyLibs")
//#pragma comment(lib, "User32")
#define DemoWeb1
#ifdef DemoWeb
void main()
#else
void DemoWeb()
#endif // DEBUG
{
	if(1 == 2)
	{
		std::string rst = Fly_web::HTMLs::GetUrlInfo("https://www.baidu.com");
		printf("%s\n", rst.c_str());
		rst = Fly_web::HTMLs::UrlRequest("https://www.baidu.com/s", "tn=80035161_2_dg&wd=getlasterror+12006");
		if (Fly_string::FindSub(rst.c_str(), "charset=gb2312"))
		{
			printf("%s\n", Fly_string::Utf8ToGBK(rst.c_str()));
		}
		else
			printf("%s\n", rst.c_str());
		getchar();
	}

	if(1 == 2)
	{
		Fly_web::IOCPs::server::createIOCPServer(5678);
	}
	if (Fly_web::TCPs::client::Init(5678, "127.0.0.1"))
	{
		Fly_web::TCPs::client::SendInfo(GetCurrentThreadId(), "Hello IOCPs", strlen("Hello IOCPs"));
		std::string str = Fly_web::TCPs::client::WaitServerMsg();
		printf("get Msg from server -%s", str.c_str());
		getchar();
	}

}

 