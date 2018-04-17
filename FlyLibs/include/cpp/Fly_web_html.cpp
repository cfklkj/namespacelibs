#include "..\Fly_web.h"
#include "..\Fly_string.h"
#include "..\Fly_debug.h"

#include <Windows.h>
#include <wininet.h>
#pragma   comment   (lib,   "wininet.lib")
#pragma comment(lib, "Mpr")
#pragma warning(disable:4996)
  
namespace Fly_web
{ 
	namespace HTMLs {
		const int MAXHTMLsBUFF = 1024;
		//��ȡ��ַ����
		std::string GetUrlInfo(const char* Url)
		{ 
			bool isGet = false;
			std::string rstStr = "";
			HINTERNET hSession = InternetOpen("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (hSession != NULL)
			{
				HINTERNET handle2 = InternetOpenUrl(hSession, Url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
				if (handle2 != NULL)
				{
					char Temp[MAXHTMLsBUFF + 1];
					ULONG Number = 1;
					bool isgb2312 = false;
					while (Number > 0)
					{
						isGet = true;
						InternetReadFile(handle2, Temp, MAXHTMLsBUFF, &Number);
						if (!Number) //add -- 2017-11-17
						{
							break;
						}
						Temp[Number] = 0;	
						if (isgb2312 || Fly_string::FindSub(Temp, "charset=gb2312"))
						{
							isgb2312 = true; 
							rstStr += Temp; 
						}else
							rstStr += Fly_string::Utf8ToGBK(Temp);
					}
					InternetCloseHandle(handle2);
				}
				else
				{
					Fly_debug::Print("GetUrlInfo error:%d\n", GetLastError());
				}
				InternetCloseHandle(hSession);
				hSession = NULL;
			}
			return rstStr;
		} 
		//phpͨ��--�������ļ�--�ɱ��浽����buff 
		//fun_type = "POST" : "GET"
		//accept_type = "html/text"
		//connection_type =  "Close"
		std::string UrlRequest(const char * url, const char * body, const char * fun_type, const char * accept_type, const char * connection_type)
		{			 
			RAW::IniSocket();
			char mac[64] = { 0 }; 
			 
			std::string IPPort = RAW::GetIpPortfromUrl(url);
			std::string IP = IPPort;
			int port = 80;  //html  Ĭ�϶˿�80
			if (Fly_string::FindSub(IPPort.c_str(), ":"))  //www.baidu.com:8090
			{
				IP = Fly_string::GetSubStr(IPPort.c_str(), ':', 1);
				port = atoi(Fly_string::GetSubStr(IPPort.c_str(), ':', 2).c_str());
			}
			//connect
			struct sockaddr_in serv_socket;
			memset(&serv_socket, 0, sizeof(struct sockaddr_in));
			serv_socket.sin_family = AF_INET;
			serv_socket.sin_port = htons(port);
			serv_socket.sin_addr.s_addr = inet_addr(IP.c_str()); //������IP��ַ  
			SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			int	flag = connect(sockfd, (struct sockaddr *)&serv_socket, sizeof(serv_socket)); //������HTTP��������TCP���� 
			if (flag < 0) {
				Fly_debug::Print("connect error!!! ip:%s\tport:%d\tflag = %d\n", IP.c_str(), port, flag);
				closesocket(sockfd);
				RAW::IniSocket();
				return "";
			}
			Fly_debug::Print("connect OK %s\n", IP.c_str());  
			int bodyLen = body ? strlen(body) : 1;
			char * request = (char *)malloc(MAXHTMLsBUFF + bodyLen);
			if (body)
				sprintf(request, "%s %s HTTP/1.1\r\nAccept: %s\r\nHost: %s:%d\r\nConnection: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s",
					fun_type, url, accept_type, IP.c_str(), port, connection_type, strlen(body), body);
			else
				sprintf(request, "%s %s HTTP/1.1\r\nAccept: %s\r\nHost: %s:%d\r\nConnection: %s\r\n\r\n",
					fun_type, url, accept_type, IP.c_str(), port, connection_type);//����������֯�����ͷ������ӡ���Ľ���������¿�ͷ��д

			send(sockfd, request, strlen(request), 0);//write  
			Fly_debug::Print("request---%s\r\n", request); 
			free(request); 

			return RAW::GetInfo(sockfd);  
		}
	}
}