#include <stdio.h>
#include <Windows.h>
#include "..\include\fly_algorithm.h" 
#include "..\include\fly_string.h" 
#pragma comment(lib, "..\\libs\\debug\\flyLibs")
//#pragma comment(lib, "User32")
#define DemoAlgorithm1
#ifdef DemoAlgorithm
void main()
#else
void DemoDemoAlgorithm()
#endif // DEBUG
{ 
	std::string str = "Hello world! --- the world is changed ,ready to chanllenge";
	std::string strEncode = Fly_algorithm::Base64::encode(str);
	printf("base64 encode--%s\n%s\n", str.c_str(), strEncode.c_str()); 
	std::string strDecode = Fly_algorithm::Base64::decode(strEncode);
	printf("base64 decode--%s\n%s\n", strEncode.c_str(), strDecode.c_str());

	strEncode = Fly_algorithm::MD4::encode(str);
	printf("Md4 encode--%s\n%s\n", str.c_str(), strEncode.c_str());
	strDecode = Fly_algorithm::MD4::decode(strEncode);
	printf("Md4 decode--%s\n%s\n", strEncode.c_str(), strDecode.c_str());

	std::string key = "my keys";
	strEncode = Fly_algorithm::RC4::encode_decode(key, str);
	printf("RC4 encode--%s\n%s\n", str.c_str(), strEncode.c_str());
	strDecode = Fly_algorithm::RC4::encode_decode(key, strEncode);
	printf("RC4 decode--%s\n%s\n", strEncode.c_str(), strDecode.c_str());

	getchar();
}

 