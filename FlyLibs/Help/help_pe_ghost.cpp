
//----ghost 
//-还原
//wsprintf(cmdStr, L"-clone,mode=pload,src=\"%s:1\",dst=\"%d:%d\" -sure -fro -crcignore -quiet \"",
// ghoPath, ptInfo.Did, ((ptInfo.isGPT && ptInfo.Pid - 1 > 0) ? ptInfo.Pid - 1 : ptInfo.Pid));

//-备份
//	wsprintf(cmdStr, L"-sure -clone,mode=pdump,src=\"%d:%d\",dst=\"%s\" -z", ptInfo.Did, ((ptInfo.isGPT && ptInfo.Pid - 1  > 0) ? ptInfo.Pid - 1 : ptInfo.Pid), keepPath);

//-----xp原版
//printf("正在释放资源...\r\n");
////拷贝i386文件
//char Src[64];
//char Drt[64];
//sprintf(Drt, "%c:\\isojy", ToDriver[0]);
//FileDelete(Drt);
//sprintf(Src, "%c:\\i386", loadDisk[0]);
//sprintf(Drt, "%c:\\isojy\\i386", ToDriver[0]);
//FileCopy(Src, Drt);
//sprintf(Src, "%c:\\i386\\winnt.sif", loadDisk[0]);
//FileCopy(Src, Drt);

//printf("资源释放完成...\r\n");
//Sleep(500);
//printf("正在安装...\r\n");
////准备安装
//char cmdStr[1024];
//sprintf(cmdStr, "/c %s\\isojy\\i386\\winnt32.exe /syspart:%s /dudisable  /s:.\\i386 /unattend:.\\i386\\winnt.sif /makelocalsource /tempdrive:%s", loadDisk, ToDriver, ToDriver);
//SystemCmd::WinExecWait(CMDPATH, cmdStr, DSHOW);
//printf("安装完成...\r\n"); 

//----imagex 原版
//sprintf(cmdStr, "/apply \"%s\" %s %c:", wimPath, imageNumber, ToDriver[0]);