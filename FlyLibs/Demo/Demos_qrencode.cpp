#include <stdio.h>
#include <Windows.h> 
#include <iostream>
#include <functional>
#include <unordered_map>

#include "./qrencode/include/qrencode.h"
#pragma comment(lib, ".//qrencode//lib//debug//qrencode")
using namespace std; 

int main(int argc, char *argv[])
{
	QRcode *qr = QRcode_encodeString("https://www.cnblogs.com/tiandsp/p/7440839.html", 1, QR_ECLEVEL_M, QR_MODE_8, 1);

	int width = qr->width;
	int height = width;
	for (int y = 0;y<height;y++)
	{
		for (int x = 0;x<width;x++)
		{
			if (qr->data[y*width + x] & 0x01)
				cout << "**";
			else
				cout << "  ";

		}
		cout << endl;
	}
	QRcode_free(qr);

	cin.get();
}