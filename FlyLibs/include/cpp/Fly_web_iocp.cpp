#pragma once
#include <string>
#include <winsock2.h>   
#include <Windows.h>  
#include <vector>
#include <iostream> 
#pragma warning(disable:4996)

using namespace std;
#pragma comment(lib, "Ws2_32.lib")      // Socket������õĶ�̬���ӿ� 

#include "..\Fly_string.h"
#include "..\Fly_web.h"
#include "..\Fly_sys.h"
#include "..\Fly_debug.h"

 

namespace Fly_web {
	namespace IOCPs {
		namespace server {
			const int MessMaxLen = 1024;
			const int DataBuffSize = 4 * 1024;
			/**
			* �ṹ�����ƣ�PER_IO_DATA
			* �ṹ�幦�ܣ��ص�I/O��Ҫ�õ��Ľṹ�壬��ʱ��¼IO����
			**/
			typedef struct
			{
				OVERLAPPED overlapped;
				WSABUF databuff;
				char buffer[DataBuffSize];
				int BufferLen;
				int operationType;
				SOCKET socket;
			}PER_IO_OPERATEION_DATA, *LPPER_IO_OPERATION_DATA, *LPPER_IO_DATA, PER_IO_DATA;

			/**
			* �ṹ�����ƣ�PER_HANDLE_DATA
			* �ṹ��洢����¼�����׽��ֵ����ݣ��������׽��ֵı������׽��ֵĶ�Ӧ�Ŀͻ��˵ĵ�ַ��
			* �ṹ�����ã��������������Ͽͻ���ʱ����Ϣ�洢���ýṹ���У�֪���ͻ��˵ĵ�ַ�Ա��ڻطá�
			**/
			typedef struct
			{
				SOCKET socket;
				SOCKADDR_STORAGE ClientAddr;
				LPPER_IO_OPERATION_DATA PerIoData;
			}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;


			HANDLE  m_completionPort = NULL;
			SOCKET m_srvSocket = 0;
			std::vector<LPPER_HANDLE_DATA> m_vclientGroup;
			/**************************
			���������տͻ�����Ϣ��
			�����߳�
			**************************/
			DWORD WINAPI   ServerWorkThread(LPVOID CompletionPortID)
			{
				HANDLE CompletionPort = (HANDLE)CompletionPortID;
				DWORD BytesTransferred;
				LPOVERLAPPED IpOverlapped;
				LPPER_HANDLE_DATA PerHandleData = NULL;
				LPPER_IO_DATA PerIoData = NULL;
				DWORD RecvBytes;
				DWORD Flags = 0;
				BOOL bRet = false;

				while (m_completionPort) {
					bRet = GetQueuedCompletionStatus(m_completionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED*)&IpOverlapped, INFINITE);
					if (bRet == 0) {
						continue;
						//���ﲻ�ܷ��أ��������߳̾ͽ�����
						//return -1;  
					}
					if (!PerHandleData)
						continue;
					PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(IpOverlapped, PER_IO_DATA, overlapped);

					// ������׽������Ƿ��д�����   
					if (0 == BytesTransferred) {
						closesocket(PerHandleData->socket);
						GlobalFree(PerHandleData);
						GlobalFree(PerIoData);
						Fly_debug::Print("�ͻ����˳� --%d \n", PerHandleData->socket);
						continue;
					}
					//�õ��ͻ���SOCKET��Ϣ
					SOCKET sClientSocket = PerHandleData->socket;
					Fly_debug::Print("lenth:%d, message is %s \n", PerIoData->databuff.len, PerIoData->databuff.buf);

					//��Ӵ����Զ�����Ϣ					 
					send(sClientSocket, PerIoData->databuff.buf, PerIoData->databuff.len, 0);  // ������Ϣ   

					// Ϊ��һ���ص����ý�����I/O��������   
					ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED)); // ����ڴ�   
					PerIoData->databuff.len = 1024;
					PerIoData->databuff.buf = PerIoData->buffer;
					PerIoData->operationType = 0;    // read   
					WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &RecvBytes, &Flags, &(PerIoData->overlapped), NULL);
				}
				return  0;
			}

			DWORD getNumberOfProcess()
			{
				SYSTEM_INFO si;
				GetSystemInfo(&si);
				return si.dwNumberOfProcessors;
			}

			/**************************
			// ����IOCP���ں˶���
			**************************/
			bool  Init()
			{
				Fly_web::RAW::IniSocket();
				// ����IOCP���ں˶���   
				/**
				* ��Ҫ�õ��ĺ�����ԭ�ͣ�
				* HANDLE WINAPI CreateIoCompletionPort(
				*    __in   HANDLE FileHandle,     // �Ѿ��򿪵��ļ�������߿վ����һ���ǿͻ��˵ľ��
				*    __in   HANDLE ExistingCompletionPort, // �Ѿ����ڵ�IOCP���
				*    __in   ULONG_PTR CompletionKey,   // ��ɼ���������ָ��I/O��ɰ���ָ���ļ�
				*    __in   DWORD NumberOfConcurrentThreads // ��������ͬʱִ������߳�����һ���ƽ���CPU������*2
				* );
				**/
				m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
				if (NULL == m_completionPort) {    // ����IO�ں˶���ʧ��   
					Fly_debug::Print("CreateIoCompletionPort failed. Error:\n");
					return false;
				}

				// ���ڴ������ĺ������������߳�   
				int threadCounts = Fly_sys::CPU::getNumberOfProcess() * 2;
				for (DWORD i = 0; i < threadCounts; ++i) {
					// �����������������̣߳�������ɶ˿ڴ��ݵ����߳�   
					HANDLE ThreadHandle = CreateThread(NULL, 0, &ServerWorkThread, m_completionPort, 0, NULL);
					if (NULL == ThreadHandle) {
						Fly_debug::Print("Create Thread Handle failed. Error::\n");
					}
					else
						CloseHandle(ThreadHandle);
				}
				return true;
			}
			//������Դ 
			bool fIni()
			{
				if (!m_completionPort)
					return true;
				for (int i = 0; i < m_vclientGroup.size(); i++)
				{
					try {
						if (!closesocket(m_vclientGroup[i]->socket))
						{
							GlobalFree(m_vclientGroup[i]->PerIoData); //�ͷŷ������Դ
							GlobalFree(m_vclientGroup[i]);
						}
					}
					catch (...)
					{
						Fly_debug::Print("��Դ���ͷ�--error");
					}
				}
				CloseHandle(m_completionPort);
				closesocket(m_srvSocket);
				m_completionPort = NULL;
				m_srvSocket = 0;
				return true;
			}
			/*************************
			��ʼ��SOCKET��Ϣ
			IP=NULL  ����IP
			*************************/
			SOCKET InitSocket(int port, const char* IP = NULL)
			{
				// ������ʽ�׽���   
				SOCKET srvSocket = socket(AF_INET, SOCK_STREAM, 0);

				// ��SOCKET������   
				SOCKADDR_IN srvAddr;
				if (!IP)
					srvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
				else
					srvAddr.sin_addr.S_un.S_addr = inet_addr(IP);
				srvAddr.sin_family = AF_INET;
				srvAddr.sin_port = htons(port);
				int bindResult = bind(srvSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
				if (SOCKET_ERROR == bindResult) {
					return -1;
				}
				return srvSocket;
			}
			//����ʽ
			bool createIOCPServer(int port)
			{
				if (m_completionPort)
					return true;
				if (!Init())
					return false;
				m_srvSocket = InitSocket(port);
				// ��SOCKET����Ϊ����ģʽ    
				if (SOCKET_ERROR == listen(m_srvSocket, 10)) {
					closesocket(m_srvSocket);
					return false;
				}
				// ��ʼ����IO����   
				Fly_debug::Print("����������׼�����������ڵȴ��ͻ��˵Ľ���...");
				int icount = 0;
				while (m_completionPort) {
					PER_HANDLE_DATA * PerHandleData = NULL;
					SOCKADDR_IN saRemote;
					int RemoteLen;
					SOCKET acceptSocket;

					// �������ӣ���������ɶˣ����������AcceptEx()   
					RemoteLen = sizeof(saRemote);
					acceptSocket = accept(m_srvSocket, (SOCKADDR*)&saRemote, &RemoteLen);
					if (SOCKET_ERROR == acceptSocket) {   // ���տͻ���ʧ��    
						Fly_debug::Print("Accept Socket Error: %d", GetLastError());
						continue;
					}

					icount = 0;

					// �����������׽��ֹ����ĵ����������Ϣ�ṹ   
					PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));  // �ڶ���Ϊ���PerHandleData����ָ����С���ڴ�   
					PerHandleData->socket = acceptSocket;
					memcpy(&PerHandleData->ClientAddr, &saRemote, RemoteLen);

					// �������׽��ֺ���ɶ˿ڹ���   
					CreateIoCompletionPort((HANDLE)(PerHandleData->socket), m_completionPort, (DWORD)PerHandleData, 0);


					// ��ʼ�ڽ����׽����ϴ���I/Oʹ���ص�I/O����   
					// ���½����׽�����Ͷ��һ�������첽   
					// WSARecv��WSASend������ЩI/O������ɺ󣬹������̻߳�ΪI/O�����ṩ����       
					// ��I/O��������(I/O�ص�)   
					LPPER_IO_OPERATION_DATA PerIoData = NULL;
					PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATEION_DATA));
					ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED));
					PerIoData->databuff.len = 1024;
					PerIoData->databuff.buf = PerIoData->buffer;
					PerIoData->operationType = 0;    // read   

					DWORD RecvBytes;
					DWORD Flags = 0;
					PerHandleData->PerIoData = PerIoData;
					m_vclientGroup.push_back(PerHandleData);      // �������ͻ�������ָ��ŵ��ͻ������� 
					WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &RecvBytes, &Flags, &(PerIoData->overlapped), NULL);
					Fly_debug::Print("�ͻ��˽��� --%d", PerHandleData->socket);
				}
				fIni();
				Fly_debug::Print("IOCP������-�˳�");
				return true;
			}
			bool deleteIOCPServer()
			{
				fIni();
				return true;
			}
		}
	}
}