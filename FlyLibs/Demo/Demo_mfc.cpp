#include <stdio.h>
#include <Windows.h>
#include "..\include\fly_mfc.h" 
#include "..\include\fly_string.h" 
#pragma comment(lib, "..\\libs\\debug\\flyLibs")


#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi")
 
#define DemoMFC1
//¹ØÁª³ÌÐò
std::string RegWithExe(std::string& exePath, std::string protoName);

#ifdef DemoMFC
void main()
#else
void DemoMFC()
#endif // DEBUG
{  
	std::string Aico = "C:\\Users\\weile\\Pictures\\1.ico";
	std::string Bico = "C:\\Users\\weile\\Pictures\\2.ico";
	std::string tips = "Hello world!";
	Fly_mfc::NotifyIco::InitNotifyIco(NULL, NULL, tips, Aico, Bico);
	Fly_mfc::NotifyIco::ShowNotify();
	Fly_mfc::NotifyIco::StartNotify(true);
	getchar();
	Fly_mfc::NotifyIco::UnNotify();
} 