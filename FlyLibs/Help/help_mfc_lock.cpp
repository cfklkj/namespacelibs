
��ʼ����
CRITICAL_SECTION para;
InitializeCriticalSection(&para);

DeleteCriticalSection(&para);

//ʹ��
EnterCriticalSection(&para);
������
LeaveCriticalSection(&para);

