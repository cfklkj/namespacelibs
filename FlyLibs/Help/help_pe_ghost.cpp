
//----ghost 
//-��ԭ
//wsprintf(cmdStr, L"-clone,mode=pload,src=\"%s:1\",dst=\"%d:%d\" -sure -fro -crcignore -quiet \"",
// ghoPath, ptInfo.Did, ((ptInfo.isGPT && ptInfo.Pid - 1 > 0) ? ptInfo.Pid - 1 : ptInfo.Pid));

//-����
//	wsprintf(cmdStr, L"-sure -clone,mode=pdump,src=\"%d:%d\",dst=\"%s\" -z", ptInfo.Did, ((ptInfo.isGPT && ptInfo.Pid - 1  > 0) ? ptInfo.Pid - 1 : ptInfo.Pid), keepPath);

//-----xpԭ��
//printf("�����ͷ���Դ...\r\n");
////����i386�ļ�
//char Src[64];
//char Drt[64];
//sprintf(Drt, "%c:\\isojy", ToDriver[0]);
//FileDelete(Drt);
//sprintf(Src, "%c:\\i386", loadDisk[0]);
//sprintf(Drt, "%c:\\isojy\\i386", ToDriver[0]);
//FileCopy(Src, Drt);
//sprintf(Src, "%c:\\i386\\winnt.sif", loadDisk[0]);
//FileCopy(Src, Drt);

//printf("��Դ�ͷ����...\r\n");
//Sleep(500);
//printf("���ڰ�װ...\r\n");
////׼����װ
//char cmdStr[1024];
//sprintf(cmdStr, "/c %s\\isojy\\i386\\winnt32.exe /syspart:%s /dudisable  /s:.\\i386 /unattend:.\\i386\\winnt.sif /makelocalsource /tempdrive:%s", loadDisk, ToDriver, ToDriver);
//SystemCmd::WinExecWait(CMDPATH, cmdStr, DSHOW);
//printf("��װ���...\r\n"); 

//----imagex ԭ��
//sprintf(cmdStr, "/apply \"%s\" %s %c:", wimPath, imageNumber, ToDriver[0]);