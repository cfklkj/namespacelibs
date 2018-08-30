#include "QR.h"
using namespace std;

int main() {
	setfontsize(hout, L"新宋体", 15);
	setconsoleborder(hout, 120, 50, 50);
	setcolor(hout, 0, 15);
	int level;//level表示二维码的大小
	int i, lenth;
	unsigned char readin[N] = { '\0' };
	int txt = 0;
	while (1) {
		cout << "请输入要生成的文本或网址：" << endl;
		cin >> readin;
		for (i = 0; readin[i]; i++) {
			if (readin[i] > 127)
				txt++;
		}
		if (i - txt / 2 > 100) {
			cout << "文本长度超出限制！请重新输入！" << endl;
			memset(readin, '\0', N);
		}
		else
			break;
	}
	int codenum = i + txt / 2;
	lenth = code_level(i - txt + txt / 2 * 3, &level);  //lenth为数据长度
	char data[334 * 8 + 1] = { '0','1','0','0' };  //存放数据
	code ccode[71][71];  //level1大小的空间，再存储一个白色边框
	code_locate(level, ccode);
	/*code_bit(readin, data, codenum, lenth);
	code_poly_data(data, level, ccode);
	code_mask(ccode, level);
	code_format(ccode, level);*/
	if (level > 6) {
		setfontsize(hout, L"新宋体", 9);
		setconsoleborder(hout, 150, 80, 80);
	}
	code_show(ccode, level); 
	//cout << level;
}