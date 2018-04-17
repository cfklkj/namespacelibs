#pragma once
#include <string> 
#include <Windows.h>
#include <shellapi.h>

namespace Fly_algorithm
{ 
	namespace MD4
	{
		//�ַ���MD4����
		std::string encode(std::string & strString);
		//MD4�����ַ���
		std::string decode(std::string & encode_string);
	}
	namespace MD5
	{
		//MD5���� --������ܲ����棬���ܺ���ַ����������
		//�ַ���md5
		std::string encode(const std::string& strString);
		//�ļ�md5
		std::string encode_file(const std::string& path);
	}
	namespace RC4
	{ 
		//�ӽ���
		//keyStr  --����
		//strString  --�ӽ����ַ���
		std::string encode_decode(std::string& keyStr, std::string& strString);
	}
	namespace Base64
	{
		//����
		std::string encode(std::string const& strString);
		//����
		std::string decode(std::string const& encoded_string);
	} 
}
