#pragma once
#include <string> 
#include <Windows.h>
#include <shellapi.h>

namespace Fly_algorithm
{ 
	namespace MD4
	{
		//字符串MD4加密
		std::string encode(std::string & strString);
		//MD4解密字符串
		std::string decode(std::string & encode_string);
	}
	namespace MD5
	{
		//MD5加密 --单向加密不可逆，加密后的字符串长度相等
		//字符串md5
		std::string encode(const std::string& strString);
		//文件md5
		std::string encode_file(const std::string& path);
	}
	namespace RC4
	{ 
		//加解密
		//keyStr  --密码
		//strString  --加解密字符串
		std::string encode_decode(std::string& keyStr, std::string& strString);
	}
	namespace Base64
	{
		//加密
		std::string encode(std::string const& strString);
		//解密
		std::string decode(std::string const& encoded_string);
	} 
}
