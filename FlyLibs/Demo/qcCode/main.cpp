#include "QR.h"
using namespace std;

int main() {
	setfontsize(hout, L"������", 15);
	setconsoleborder(hout, 120, 50, 50);
	setcolor(hout, 0, 15);
	int level;//level��ʾ��ά��Ĵ�С
	int i, lenth;
	unsigned char readin[N] = { '\0' };
	int txt = 0;
	while (1) {
		cout << "������Ҫ���ɵ��ı�����ַ��" << endl;
		cin >> readin;
		for (i = 0; readin[i]; i++) {
			if (readin[i] > 127)
				txt++;
		}
		if (i - txt / 2 > 100) {
			cout << "�ı����ȳ������ƣ����������룡" << endl;
			memset(readin, '\0', N);
		}
		else
			break;
	}
	int codenum = i + txt / 2;
	lenth = code_level(i - txt + txt / 2 * 3, &level);  //lenthΪ���ݳ���
	char data[334 * 8 + 1] = { '0','1','0','0' };  //�������
	code ccode[71][71];  //level1��С�Ŀռ䣬�ٴ洢һ����ɫ�߿�
	code_locate(level, ccode);
	/*code_bit(readin, data, codenum, lenth);
	code_poly_data(data, level, ccode);
	code_mask(ccode, level);
	code_format(ccode, level);*/
	if (level > 6) {
		setfontsize(hout, L"������", 9);
		setconsoleborder(hout, 150, 80, 80);
	}
	code_show(ccode, level); 
	//cout << level;
}