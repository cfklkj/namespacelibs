#pragma once
#include <string> 

namespace Fly_web {
	namespace RAW {
		//��ʼ������
		bool IniSocket();
		//�ͷŻ���
		void UnInitSocket();
		//�������� 
		std::string GetInfo(const int socket);
		//�Ƿ�����
		bool isLinkInternet();
		//����ַ��ȡIP Port
		std::string GetIpPortfromUrl(const char* URl); 
		//����ַ��ȡIP Port �������:9080
		std::string GetIpPortfromUrl(const char* URl); 

	} 
	namespace Test {
		void Init();
	}

	namespace TCPs {
		namespace server {
			//���������׽���
			bool Init(int port);
			//�ȴ��ͻ���
			//�ȴ��ͻ���
			unsigned long WaitClientCome();
			//�ȴ��ͻ�������
			std::string WaitClientInfo(const int clientID, char* rstBuff, int rstBuffLen);
			//��������
			bool SendInfo(const int clientID, const char* msg, int lenth);
		}
		namespace client {
			//�����ͻ����׽���
			bool Init(int port, char* IP);
			//��������
			std::string WaitServerMsg();
			//��������  parentID --����init�߳�ID
			bool SendInfo(const int parentID, const char* msg, int lenth);
		}
	}
	namespace SETs {
		//�Ƿ�������IP
		// LAN IP's
		// -------------------------------------------
		//	0.*								"This" Network
		//	10.0.0.0 - 10.255.255.255		Class A
		//	172.16.0.0 - 172.31.255.255		Class B
		//	192.168.0.0 - 192.168.255.255	Class C
		bool IsLANIP(unsigned int nIP);
		//���û��������
		//���淽������ cmd.exe /c ipconfig /release | /renew 
		bool LinkWeb(bool isDiskconnect);
		//��ȡ������ַ
		std::string GetMac();
	}
	namespace HTMLs {
		//��ȡ��ַ����
		std::string GetUrlInfo(const char* Url);
		//��ַ����
		//body = "" �� eg:id=12&ip=1234   --""  ���ж� ��ַ�Ƿ���� ��
		std::string UrlRequest(const char * url, const char * body,const char * fun_type = "POST", const char * accept_type = "html/text", const char * connection_type = "Close");
	}
	namespace IOCPs {
		//ע��---�ͻ���ʹ�� TCPs::lient  ����
		namespace server {
			//����ʽ   ��������ֻ��ת����Ϣ---ת����Ϣ������һ���ͻ���ȥ�����������߼�
			bool createIOCPServer(int port);
		}
	}
}
 