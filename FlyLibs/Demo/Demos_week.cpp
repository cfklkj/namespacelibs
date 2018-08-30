#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
  
#include "D:\document\me\namespacelibs\FlyLibs\include\Fly_CwriteLog.h"
#pragma comment(lib, "D:\\document\\me\\namespacelibs\\FlyLibs\\libs\\Debug\\FlyLibs.lib")

void main()
{
	CWriteLog log;
	log.open(".\\test.log");
	log.writeLine("Hellolsdkfjsldf");
	log.close();
}