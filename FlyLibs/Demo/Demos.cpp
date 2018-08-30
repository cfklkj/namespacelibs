#include <stdio.h>
#include <Windows.h> 
#include <iostream>
#include <functional>
#include <unordered_map>
#include <map>

using namespace std; 

class Test {
public:
	void Add(int a, int b);
	void Delete(int c, int d); 
	void Mult(int c, int d);
	void init();
	void RunMap(int index);
private:
 	std::unordered_map<int, void(Test::*)(int, int)> m_FuncMaps;
	std::unordered_map<int, function<void(int, int)>> m_FuncMaps2; 
};

void Test::init()
{
	m_FuncMaps[1] = &Test::Add;
	m_FuncMaps[2] = &Test::Delete;
 	m_FuncMaps2[3] = bind(&Test::Mult, this, placeholders::_1, placeholders::_2);  //方法2
} 
void Test::Mult(int c, int d)
{
	printf("is Mult");
}
void Test::Add(int a, int b)
{
	printf("is Add");
}
void Test::Delete(int a, int b)
{
	printf("is Delete");
}

void Test::RunMap(int index)
{
	auto funcIter = m_FuncMaps.find(index);
	if (funcIter != m_FuncMaps.end())  //方法1
	{
		(this->*funcIter->second)(1, 1);
	}
	auto funcIter2 = m_FuncMaps2.find(index);
	if (funcIter2 != m_FuncMaps2.end())  //方法2
	{
		(funcIter2->second)(1, 1);
	} 
}

string temp()
{
	string temp = "123123";
	string temp2 = "";
	if (1 > 5)
	{
		return temp;
	}
	return temp2;
}
struct TEST {
	int id;
	int ip;
};
std::vector<TEST> m_test;
std::map<int, TEST> m_mapTest;
void set(int id)
{
	int size = m_mapTest.size();
	for (int i = 5; i > 0; i--)
	{
		TEST test;
		test.id = id;
		test.ip = i;
		m_mapTest[size++] = test;
		//m_test.push_back(test);
	}
}
const TEST* get(int index)
{
	for (auto& iter : m_test)
	{
		if(iter.id == index)
		return &iter;

	}
	return NULL;
}

void del(int id)
{
//	for (auto iter : m_mapTest)
		for (auto iter = m_mapTest.begin();iter != m_mapTest.end();)

	{
		if (iter->second.id == id)
		{ 
			int id = iter->first;
			iter++;
			m_mapTest.erase(id);
		}else
			iter++;
	}
}

#include <time.h>
void main() 
{
	set(2);
	set(3);
	set(4);
	del(3);
	getchar();

	time_t currentTime;
	time(&currentTime);
	struct tm p;
	localtime_s(&p, &currentTime);
	 

	char str[200];

	char hour[3];

	char min[3];

	char sec[3];

	  localtime_s(&p, &currentTime);
	   

	//set();
	for (int i = 0; i < 5; i++)
	{
		const TEST* ret = get(i);
		if(ret)
			printf("%d--%d,%d\n", i, ret->id, ret->ip);
	}
	getchar();
	return;

	string abc = temp();
	printf(abc.c_str());
	getchar();
	return;
	Test test;
	test.init();
	int i = 0;
	while (1)
	{
		printf("%d-----", ++i);
		test.RunMap(i%3+1);
		Sleep(1000);
		printf("\n");
	}
	test.RunMap(2);
	test.RunMap(3);
	getchar(); 
}

