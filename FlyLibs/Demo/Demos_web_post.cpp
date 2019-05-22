#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <tuple>
#include <string>
  
#include "..\..\FlyLibs\include\Fly_CwriteLog.h"
#pragma comment(lib, "..\\..\\FlyLibs\\libs\\Debug\\FlyLibs.lib")
 
// httpPost.cpp : Defines the entry point for the console application.
//
 
#include "stdio.h"
#include <time.h>
#include <WinSock2.h>
#include <cstringt.h>
#pragma comment(lib, "Ws2_32.lib")

char* SendHttpRequest(char* weburl, char* url, char* detectorId, char* JsonData);

int main(int argc, char* argv[])
{

	char* JsonData = "[{\"a\":\"b\"}]";
	char weburl[] = "api.fox.boonw.com";
	char* url = "/v1/dictionary/index";
	char* Id = "aaa";
	char* re = SendHttpRequest(weburl, url, Id, JsonData);
	printf("%s\r\n", re);

	system("pause");

	return 0;
}

char* SendHttpRequest(char* weburl, char* url, char* detectorId, char* JsonData)
{
	time_t rawtime;
	struct tm * timeinfo;
	time_t t = time(&rawtime);
	long timestamp = t;

	/*char *pHttpPost = "POST https://api.fox.boonw.com/%s HTTP/1.1\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"Content-Length: %d\r\n"
		"Host:%s\r\n"
		"Connection: Keep-Alive\r\n"
		"Expect: 100-continue\r\n"
		//"Accept-Encoding: gzip,deflate\r\n"
		"\r\n%s";*/
	char* p = ":";
	char* web = strtok(weburl, p);
	int port = 443;
	char* ptr;
	if ((ptr = strtok(NULL, p)))
	{
		port = atoi(ptr);
	}

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return false;
	}


	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return false;
	}

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	HOSTENT* thisHost;
	char* sIp = NULL;
	thisHost = gethostbyname(web);
	if (!thisHost)
	{
		WSACleanup();
		return NULL;
	}
	sIp = inet_ntoa(*(struct in_addr*)*thisHost->h_addr_list);


	std::string strHttpHead;
	strHttpHead.append("POST");
	//strHttpHead.append(" https://api.fox.boonw.com");
	strHttpHead.append(" ");
	strHttpHead.append(url);
	strHttpHead.append(" HTTP/1.1\r\n");
	strHttpHead.append("Content-Type: text/html; charset=UTF-8\r\n");
	//strHttpHead.append("Accept-Language: cn\r\n");
	//strHttpHead.append("User-Agent: Mozilla/4.0\r\n");
	strHttpHead.append("Host: ");
	strHttpHead.append(sIp);
	strHttpHead.append("\r\n");
	strHttpHead.append("Connection:Close");
	strHttpHead.append("\r\n");

	//strHttpHead.append("Cache-Control: no-cache\r\n");
//	strHttpHead.append("Connection: Keep-Alive\r\n");
	if ("'1" == "POST")
	{
		std::string strData = "";
		char len[8] = { 0 };
		unsigned long iLen = strData.size();
		sprintf(len, "%lu", iLen);

		strHttpHead.append("Content-Type: application/x-www-form-urlencoded\r\n");
		strHttpHead.append("Content-Length: ");
		strHttpHead.append(len);
		strHttpHead.append("\r\n\r\n");
		strHttpHead.append(strData);
	}
	strHttpHead.append("\r\n\r\n"); 

	char strHttpPost[10240] = { 0 };
//	sprintf_s(strHttpPost, strHttpHead.c_str(), url, strlen(JsonData), weburl, JsonData);
	sprintf_s(strHttpPost,"%s", strHttpHead.c_str());


	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(sIp);//本机地址，服务器在本机开启
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);// 设置端口号

	char* content = NULL;
	int a = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//连接服务器
	if (a == 0)
	{
		send(sockClient, strHttpPost, strlen(strHttpPost), 0);

		char recvBuf[1025];

		while (true)
		{
			int r = recv(sockClient, recvBuf, 1024, 0);//接受数据
			if (r == 0)
				break;
			recvBuf[r] = 0;
			printf("\r\n");
			printf(recvBuf);
			if (r > 0)
			{
				char* p = strstr(recvBuf, "HTTP/1.1 200");
				if (p != NULL)
				{
					char* ptr = strstr(recvBuf, "\r\n\r\n");
					if (ptr != NULL)
					{
						char* p_start = strtok(ptr, "{");
						if (p_start != NULL)
						{
							char* p = strtok(NULL, "{");
							if (p != NULL)
								content = strtok(p, "}");
						}
					}

					closesocket(sockClient);//关闭连接
					return content;
				}

				p = strstr(recvBuf, "HTTP/1.1 400");
				if (p != NULL)
				{
					break;
				}
			}
		}
		closesocket(sockClient);//关闭连接
	}
	return content;
}
