#include <stdio.h>
#include <Windows.h> 
#include <iostream>
#include <functional>
#include <unordered_map>

using namespace std; 

void PrintWhile(const char* msg)
{
	int i = 100;
	while (i-- > 0)
	{
		printf(msg);
		Sleep(1000);
	}

} 
void main(int argc, char** argv) 
{
	if (argc < 2)
	{
		PrintWhile("no parameter\n");
	}
	for (int i = 0; i < argc; i++)
	{
		printf("argv[i] = %s\n", argv[i]);
	}
	PrintWhile("have parameter");
}

 