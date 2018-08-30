#define _CRT_SECURE_NO_WARNINGS
#include "QR.h"
using namespace std;

/*选择二维码level,n byte*/
int code_level(int n, int *level) {
	n = 8 * n + 16;
	int lenth[13] = { 128,224,352,512,688,864,992,1232,1456,1728,2032,2320,2672 };
	for (int i = 0; i < 13; i++) {
		if (n <= lenth[i]) {
			*level = i + 1;
			return lenth[i];
		}
	}
	return -1;
}

/*外加白色边框设置*/
void code_border(int n, code ccode[71][71]) {
	for (int i = 0; i < 19 + 4 * n; i++) {
		if (i == 0 || i == 18 + 4 * n)
			for (int j = 0; j < 19 + 4 * n; j++) {
				ccode[i][j].show = 0;
				ccode[i][j].ifempty = -1;
			}
		ccode[i][0].show = 0;
		ccode[i][18 + 4 * n].show = 0;
		ccode[i][0].ifempty = -1;
		ccode[i][18 + 4 * n].ifempty = -1;
	}
}

/*初始设置的区域*/
void code_locate(int level, code ccode[71][71]) {
	code_border(level, ccode);
	draw_pos(1, 1, ccode);
	draw_pos(1, 11 + level * 4, ccode);
	draw_pos(11 + level * 4, 1, ccode);    //大框框
	pos_side(level, ccode);  //大框边上的半个白框
	if (level > 1)      //画小框框
		draw_box(9 + 4 * level, 9 + 4 * level, ccode);
	if (level > 6) {        //画小框框
		draw_box((1 + 4 * level) / 2 + 7, 5, ccode);
		draw_box((1 + 4 * level) / 2 + 7, (1 + 4 * level) / 2 + 7, ccode);
		draw_box(5, (1 + 4 * level) / 2 + 7, ccode);
		draw_box(17 + 4 * level - 8, (1 + 4 * level) / 2 + 7, ccode);
		draw_box((1 + 4 * level) / 2 + 7, 17 + 4 * level - 8, ccode);
	}
	for (int i = 9; i < 10 + 4 * level; i++) {  //那两个横竖的条条
		if (ccode[i][7].ifempty != -1) {
			ccode[i][7].show = i % 2;
			ccode[i][7].ifempty = -1;
		}
		if (ccode[7][i].ifempty != -1) {
			ccode[7][i].show = i % 2;
			ccode[7][i].ifempty = -1;
		}
	}
	setsolid(ccode, level);

}

/*大框*/
void draw_pos(int x, int y, code ccode[71][71]) {
	int i, j;
	for (i = 0; i < 7; i++) {     //黑，横
		ccode[x][y + i].show = 1;
		ccode[x][y + i].ifempty = -1;
		ccode[x + 6][y + i].show = 1;
		ccode[x + 6][y + i].ifempty = -1;
	}
	for (i = x + 1; i < x + 6; i++) {   //黑，竖
		ccode[i][y].show = 1;
		ccode[i][y].ifempty = -1;
		ccode[i][y + 6].show = 1;
		ccode[i][y + 6].ifempty = -1;
	}
	for (i = x + 2; i < x + 5; i++)   //中心，黑
		for (j = y + 2; j < y + 5; j++) {
			ccode[i][j].show = 1;
			ccode[i][j].ifempty = -1;
		}
	for (i = x + 1; i < x + 6; i++)
		for (j = y + 1; j < y + 6; j++)
			if (ccode[i][j].ifempty != -1) {
				ccode[i][j].show = 0;
				ccode[i][j].ifempty = -1;
			}
}

/*小框*/
void draw_box(int x, int y, code ccode[71][71]) {
	int i;
	for (i = y; i < y + 5; i++) {
		ccode[x][i].show = 1;
		ccode[x][i].ifempty = -1;
		ccode[x + 4][i].show = 1;
		ccode[x + 4][i].ifempty = -1;
	}
	for (i = 0; i < 3; i++) {
		ccode[x + 1 + i][y].show = 1;
		ccode[x + 1 + i][y].ifempty = -1;
		ccode[x + 1 + i][y + 4].show = 1;
		ccode[x + 1 + i][y + 4].ifempty = -1;
	}
	for (i = 0; i < 3; i++) {
		ccode[x + 1 + i][y + 1].show = 0;
		ccode[x + 1 + i][y + 1].ifempty = -1;
		ccode[x + 1 + i][y + 3].show = 0;
		ccode[x + 1 + i][y + 3].ifempty = -1;
	}
	ccode[x + 2][y + 2].show = 1;
	ccode[x + 2][y + 2].ifempty = -1;
	ccode[x + 1][y + 2].show = 0;
	ccode[x + 1][y + 2].ifempty = -1;
	ccode[x + 3][y + 2].show = 0;
	ccode[x + 3][y + 2].ifempty = -1;
}

/*定位框边上的白框*/
void pos_side(int version, code ccode[71][71]) {
	int i;
	for (i = 0; i < 8; i++) {
		ccode[i + 1][8].show = 0;
		ccode[i + 1][8].ifempty = -1;
		ccode[i + 1][10 + version * 4].show = 0;
		ccode[i + 1][10 + version * 4].ifempty = -1;
		ccode[i + 10 + version * 4][8].show = 0;
		ccode[i + 10 + version * 4][8].ifempty = -1;
	}
	for (i = 0; i < 7; i++) {
		ccode[8][i + 1].show = 0;
		ccode[8][i + 1].ifempty = -1;
		ccode[10 + version * 4][i + 1].show = 0;
		ccode[10 + version * 4][i + 1].ifempty = -1;
		ccode[8][i + 11 + version * 4].show = 0;
		ccode[8][i + 11 + version * 4].ifempty = -1;
	}
}

/*数据转化*/
void code_bit(unsigned char *readin, char *data, int lenth1, int lenth2) {
	int n;
	char temp[9] = { '\0' };

	_itoa(lenth1, temp, 2); //将字符数转换为二进制
	n = strlen(temp);
	for (int i = 4; i < 12 - n; i++)
		data[i] = '0';
	strncpy(&(data[12 - n]), temp, n);  //前12字节完成

	unsigned char *lpUTF8Str = NULL;
	int Len;   //转化后的长度/byte
	Len = code_GBKToUTF8(readin, NULL, NULL);
	lpUTF8Str = (unsigned char*)calloc((Len), sizeof(char));
	if (lpUTF8Str == NULL)
		return;
	Len = code_GBKToUTF8(readin, lpUTF8Str, Len);
	for (int i = 0; i < Len - 1; i++) {
		//memset(temp, '0', 8);
		for (int j = 0; j < 8; j++)
			temp[j] = '0';
		code_single(temp, lpUTF8Str[i]);
		strncpy(&(data[12 + 8 * i]), temp, 8);  //数据编码完成
	}
	strncpy(&(data[strlen(data)]), "0000\0", 5);
	int sin_dou = 1;
	while ((int)strlen(data) < lenth2) {
		n = strlen(data);
		if (sin_dou % 2 > 0)
			strncpy(&(data[n]), "11101100\0", 9);
		else
			strncpy(&(data[n]), "00010001\0", 9);
		sin_dou++;                           //补位完成
	}
	if (lpUTF8Str)
		free(lpUTF8Str);
}

/*将单个字符转化成二进制形式*/
void code_single(char temp[9], unsigned char word) {
	_itoa(word, temp, 2);
	int n = strlen(temp);
	if (n < 8)
		for (int i = 0; i < 8; i++) {
			if (n - i - 1 < 0)
				temp[7 - i] = '0';
			else
				temp[7 - i] = temp[n - 1 - i];
		}
}

int alphato(int n)
{
	int N = n % 255;
	return AlphaTo[N];
}

int powerof(int n)
{
	return PowerOf[n - 1];
}

/*求链表表长*/
int lenth(polynomail &L) {
	polynomail p = L->next;
	int i = 0;
	while (p) {
		i++;
		p = p->next;
	}
	return i;
}

/*生成数据多项式并计算纠错码*/
void code_poly_data(char *data, int level, code ccode[71][71]) {
	polynomail L = (polynomail)malloc(sizeof(polynode)), Born = (polynomail)malloc(sizeof(polynode));
	polynomail p_data = (polynomail)malloc(sizeof(polynode)), p_ec = (polynomail)malloc(sizeof(polynode));//分别分块按顺序存放数据和纠错码
	polynomail mid = (polynomail)malloc(sizeof(polynode));
	if (L == NULL || Born == NULL || p_data == NULL || p_ec == NULL || mid == NULL)
		return;
	L->prior = L->next = Born->prior = Born->next = NULL;
	p_data->prior = p_data->next = p_ec->prior = p_ec->next = NULL;
	mid->prior = mid->next = NULL;
	int n;  //第一组的数据长度
	int total = block[level - 1].total;   //总块数
	int piece[2] = { block[level - 1].piece[0],block[level - 1].piece[1] };  //将数据分为两组，每组的块数
	int divide[2][2] = { block[level - 1].divide[0][0],block[level - 1].divide[0][1],
		block[level - 1].divide[1][0],block[level - 1].divide[1][1] };  //每块的数据长度和纠错码长

	code_poly_born(Born, divide[0][1]);     //生成生成多项式（多项式乘法）从前往后，指数依次降低

	for (int i = 0; i < total; i++) {
		if (i < piece[0]) {
			code_line(L, &data[i * 8 * divide[0][0]], divide[0][0], divide[0][1] + divide[0][0] - 1, 1);//这里的L中存放的不只有数据，还有与生成多项式相匹配的补项
			code_line(mid, &data[i * 8 * divide[0][0]], divide[0][0], divide[0][1] + divide[0][0] - 1, 0);//mid中存储数据
		}
		else {
			n = piece[0] * 8 * divide[0][0];
			code_line(L, &data[n + (i - piece[0]) * 8 * divide[1][0]], divide[1][0], divide[1][1] + divide[1][0] - 1, 1);
			code_line(mid, &data[n + (i - piece[0]) * 8 * divide[1][0]], divide[1][0], divide[1][1] + divide[1][0] - 1, 0);
		}     //数据多项式M(x)已生成
		if (i == 0)
			poly_copy(p_data, mid);
		else {
			poly_insert(p_data, mid, i);
		}
		code_poly_divide(Born, L); //多项式除法求余数
		if (i == 0) {
			p_ec->next = L->next;
			L->next->prior = p_ec;
			L->next = NULL;
		}
		else
			poly_insert(p_ec, L, i);
	}     //新的数据链表和纠错码链表生成完毕（按在二维码中的存放顺序）L链接的部分给了p_ec，L只剩下头结点
	n = (divide[0][0] + divide[0][1])*piece[0] + (divide[1][0] + divide[1][1])*piece[1];
	char alldata[8000] = { '\0' };
	//把链表中的数据转成二进制存入数组
	data_change(p_ec, p_data, alldata);
	code_fill(ccode, alldata, level);
	free(L);
	if (lenth(mid) != 0)
		poly_del(mid);
	poly_del(Born);
	poly_del(p_data);
	poly_del(p_ec);
	free(mid);
	free(Born);
	free(p_data);
	free(p_ec);
}

/*字符数组内八位“二进制”转十进制,返回十进制值*/
int code_bina(char *data) {
	int num = 0x0, n;
	for (int i = 0; i < 8; i++) {
		n = ((int)data[i] - 48);
		n = n << (7 - i);
		num = num ^ n;
	}
	return num;
}

/*数据链表生成,n为链表长度（不包括头结点）,maxexp为最高次幂*/
int code_line(polynomail &L, char *data, int n, int maxexp, int choose) {
	polynomail s = NULL, p = L;
	for (int i = 0; i < n; i++) {
		s = (polynomail)malloc(sizeof(polynode));
		if (s == NULL)
			return -1;
		s->coef = code_bina(&data[i * 8]);
		s->expofa = powerof(s->coef);
		s->expn = maxexp - i;  //从前往后指数降低
		s->prior = p;
		p->next = s;
		p = s;
	}
	if (choose == 1)
		for (int i = 0; i < maxexp - n + 1; i++) {
			s = (polynomail)malloc(sizeof(polynode));
			if (s == NULL)
				return -1;
			s->coef = 0;
			s->expofa = 0;
			s->expn = maxexp - n - i;  //从前往后指数降低
			s->prior = p;
			p->next = s;
			p = s;
		}
	p->next = NULL;
	return 1;
}

/*生成初始多项式，纠错码字数为10*/
void poly_initial(polynomail &newpoly) {
	int poly10[11][3] = { { 1,0,10 },{ 216,251,9 },{ 194,67,8 },
	{ 159,46,7 },{ 111,61,6 },{ 199,118,5 },{ 94,70,4 },
	{ 95,64,3 },{ 113,94,2 },{ 157,32,1 },{ 193,45,0 } };//每行的三列分别表示系数、a的幂、指数
	polynomail p = newpoly, s;
	p = newpoly;
	for (int i = 0; i < 11; i++) {
		s = (polynomail)malloc(sizeof(polynode));
		if (s == NULL)
			return;
		s->coef = poly10[i][0];
		s->expofa = poly10[i][1];
		s->expn = poly10[i][2];
		s->next = p->next;
		s->prior = p;
		p->next = s;
		p = s;
	}
}

/*生成多项式的生成（多项式乘法） version 1-13,纠错级别M  maxexp为最高次幂*/
int code_poly_born(polynomail &Born, int maxexp) {
	polynomail p, newpoly, s, q = Born;
	newpoly = (polynomail)malloc(sizeof(polynode));
	if (newpoly == NULL)
		return -1;
	newpoly->next = newpoly->prior = NULL;
	poly_initial(newpoly);
	for (int i = 10; i < maxexp; i++) {
		p = newpoly->next;
		q = Born;
		//计算newpoly*（n2+2^n),将生成的新链表存入Born
		while (p) {
			s = (polynomail)malloc(sizeof(polynode));
			if (s == NULL)
				return -1;
			s->coef = p->coef;
			s->expn = p->expn + 1;
			s->expofa = p->expofa;
			q->next = s;
			s->prior = q;
			q = s;
			s = (polynomail)malloc(sizeof(polynode));
			if (s == NULL)
				return -1;
			s->expn = p->expn;
			s->expofa = (p->expofa + i) % 255;
			s->coef = alphato(s->expofa);
			q->next = s;
			s->prior = q;
			q = s;
			p = p->next;
		}
		q->next = NULL;
		code_U(Born);  //合并指数相同的项，并释放其中一个的空间
		poly_del(newpoly);//释放newpoly的空间,保留头结点
		newpoly->next = Born->next;
		Born->next->prior = newpoly;
		Born->next = NULL; //Born变成新的已有多项式newpoly
	}
	Born->next = newpoly->next;
	newpoly->next->prior = Born;
	free(newpoly); //释放newpoly
	return 1;
}

/*将指数相同项合并,在生成链表时，指数相同的项已经相邻，依次释放即可*/
void code_U(polynomail &Born) {
	polynomail p = Born->next->next, s;
	while (p->next) {
		p->coef = p->coef^p->next->coef;
		p->expofa = powerof(p->coef);
		s = p->next;  //抓住被释放的结点
		p->next = s->next;
		s->next->prior = p;
		free(s);
		p = p->next;
	}
}

/*释放链表，保留头结点*/
void poly_del(polynomail &L) {
	polynomail p = L->next, r;
	while (p) {
		r = p->next;
		free(p);
		p = r;
	}
	L->next = NULL;
}

/*多项式除法   L是数据多项式，Born是生成多项式*/
void code_poly_divide(polynomail &Born, polynomail &L) {
	polynomail p, q = L->next, r, copy;   //生成多项式不可变，复制一份到copy中
	int difval;  //数据多项式的最高位指数和生成多项式的最高位指数的差值

	copy = (polynomail)malloc(sizeof(polynode));
	if (copy == NULL)
		return;
	copy->next = copy->prior = NULL;
	while (L->next->expn >= Born->next->expn) {   //数据多项式的最高次幂比生成多项式小
		poly_copy(copy, Born);
		p = copy->next;
		q = L->next;
		difval = q->expn - p->expn;
		while (p) {
			p->expn += difval;
			p->expofa = (q->expofa + p->expofa) % 255;
			p->coef = alphato(p->expofa);
			p = p->next;   //q不动
		}
		//在copy后补上系数为
		p = copy->next;
		while (q&&p) {
			q->coef = p->coef^q->coef;
			if (q->coef == 0 && p == copy->next) {       //消去L中系数为0的项
				r = q->next;
				q->prior->next = q->next;
				if (q->next != NULL)
					q->next->prior = q->prior;
				free(q);
				q = r;
			}
			else {
				q->expofa = powerof(q->coef);
				q = q->next;
			}
			p = p->next;
		}
		poly_del(copy);
	}
	free(copy);   //释放copy
}

/*复制拷贝链表到链表*/
void poly_copy(polynomail &copy, polynomail &Born) {
	polynomail s, p = Born->next, q = copy;
	while (p) {
		s = (polynomail)malloc(sizeof(polynode));
		if (s == NULL)
			return;
		s->coef = p->coef;
		s->expn = p->expn;
		s->expofa = p->expofa;
		q->next = s;
		s->prior = q;
		q = s;
		p = p->next;
	}
	q->next = NULL;
}

/*每i个结点后插入一个结点(poly_de中插入L)*/
void poly_insert(polynomail &poly_de, polynomail &L, int i) {
	polynomail p = poly_de, q = L->next, r = p, s;
	while (p&&q) {
		for (int k = 0; k < i&&p; k++) {
			r = p;   //r指向p的前一项
			p = p->next;
		}
		if (!p)
			break;
		else {
			q->prior->next = q->next;
			s = q->next;  //抓住q的后继
			if (q->next)
				q->next->prior = q->prior;
			if (p->next)
				p->next->prior = q;
			q->next = p->next;
			p->next = q;
			q->prior = p;   //插入操作完成
			p = q;
			q = s;
		}
	}
	if (!p&&q) {
		r->next = q;
		q->prior = r;
	}      //操作完成，此时L为空
	L->next = NULL;
}

/*转存链表数据到数组*/
void data_change(polynomail &EC, polynomail &data, char *all) {
	polynomail p = data->next;
	char temp[9] = { '\0' };
	int i = 0, n, sign = 0;
	while (p) {
		_itoa(p->coef, temp, 2); //将系数转换为二进制
		n = strlen(temp);
		for (int k = n - 1, j = 0; k > -1; k--, j++)
			temp[7 - j] = temp[k];
		for (int k = 0; k < 8 - n; k++)
			temp[k] = '0';
		strncpy(&(all[8 * i]), temp, 9);
		memset(temp, '\0', 8);
		i++;
		p = p->next;
		if (p == NULL&&sign == 0) {
			p = EC->next;
			sign = 1;
		}
	}
	n = strlen(all);
	if (n >= 352 && n <= 1376) {
		char temp[8] = { "0000000" };
		strncpy(&all[n], temp, 8);
	}
}

/*检查是否有空位*/
int fix(code ccode[71][71], int version) {
	int t = 0;
	for (int i = 1; i < 17 + 4 * version; i++) {
		for (int j = 0; j < 17 + 4 * version; j++) {
			if (ccode[i][j].ifempty != 0 && ccode[i][j].ifempty != -1) {
				t++;
				cout << i << "  " << j << endl;
			}
		}
	}
	return t;
}

/*转存数组数据到二维码框架*/
void code_fill(code ccode[71][71], char *data, int version) {
	int x = version * 4 + 17, y = version * 4 + 17, dire = -1;//dire的值表示上移或下移
	int skip = 0;   //定框竖线所占区域
	int n = strlen(data);
	for (int i = 0; data[i]; i++) {
		if (ccode[x][y].ifempty != -1) {
			ccode[x][y].show = data[i] - 48;
			ccode[x][y].ifempty = 0;
		}
		int end = 0;
		while (1) {
			if ((y - skip) % 2 == 1) {
				if ((x == 1 && dire == -1) || (x == version * 4 + 17 && dire == 1)) {
					y = y - 1;
					break;
				}
				for (int j = dire; x + j > 0 && x + j < version * 4 + 18; j += dire) {
					if (ccode[x][y - 1].ifempty != -1) {
						y = y - 1;
						end = 1;
						break;
					}
					else if (ccode[x + j][y].ifempty != -1) {
						x = x + j;
						end = 1;
						break;
					}
					else if (x + j == 0 || x + j == version * 4 + 18) {
						y = y - 1;
						if (x + j == 1)
							dire = 1;
						else
							dire = -1;
						break;
					}
				}
				if (end == 1)
					break;
			}
			else {
				if (x == 1 && dire == -1) {
					y = y - 1;
					dire = 1;
					break;
				}
				else if (x == version * 4 + 17 && dire == 1) {
					y = y - 1;
					dire = -1;
					if (y == 9)
						x = 9 + 4 * version;
					break;
				}
				for (int j = dire; x + j > 0 && x + j < version * 4 + 18; j += dire) {
					if (ccode[x + j][y + 1].ifempty != -1) {
						x += j;
						y = y + 1;
						end = 1;
						break;
					}
					else if (ccode[x + j][y].ifempty != -1) {
						x = x + j;
						end = 1;
						break;
					}
					else if (x + j == 1 || x + j == version * 4 + 17) {
						if (version > 6 && y == 8 + 4 * version) {
							y = y - 2;
							x = 1;
							dire = 1;
							end = 1;
							break;
						}
						y = y - 1;
						if (y == 7) {
							skip = 1;
							y--;
						}
						if (x + j == 1)
							dire = 1;
						else
							dire = -1;
						end = 1;
						break;
					}
				}
				if (end == 1)
					break;
			}
		}
	}
	//cout << fix(ccode, version) << endl;
}

/*输出图形界面*/
void code_show(code ccode[71][71], int version) {
	//setconsoleborder(hout, 120, 80, 80);
	//setfontsize(hout, L"点阵字体", 8);
	for (int i = 0; i < 19 + version * 4; i++) {
		for (int j = 0; j < 19 + version * 4; j++) {
			if (ccode[i][j].show == 0) {
				setcolor(hout, 15, 15);
				cout << "  ";
			}
			else if (ccode[i][j].show == 1) {
				setcolor(hout, 0, 0);
				cout << "  ";
			}
		}
		cout << endl;
	}
	setcolor(hout, 0, 15);
}

/*将模式信息等位置填充固定*/
void setsolid(code ccode[71][71], int version) {
	ccode[10 + 4 * version][9].show = 1;
	ccode[10 + 4 * version][9].ifempty = -1;   //小黑点
	for (int i = 1; i <= 17 + version * 4; i++) {
		ccode[9][i].ifempty = -1;
		ccode[i][9].ifempty = -1;
		if (i == 9)
			i = i + 4 * version;  //跳过数据
	}
	if (version > 6) {     //大于版本6要固定版本信息位
		for (int i = 7 + 4 * version; i < 10 + 4 * version; i++) {
			for (int j = 1; j < 7; j++) {
				ccode[i][j].ifempty = -1;
				ccode[j][i].ifempty = -1;
			}
		}
	}
}

/*掩码的实现*/
void code_mask(code ccode[71][71], int version) {
	for (int i = 1; i < 18 + 4 * version; i++)
		for (int j = 1; j < 18 + 4 * version; j++)
			/*if ((i + j - 2) % 2 == 0 && ccode[i][j].ifempty != -1)
			ccode[i][j].show ^= 1;
			else if (ccode[i][j].ifempty != -1)
			ccode[i][j].show ^= 0;*/
			if ((j - 1) % 3 == 0 && ccode[i][j].ifempty != -1)
				ccode[i][j].show ^= 1;
			else if (ccode[i][j].ifempty != -1)
				ccode[i][j].show ^= 0;
}

/*格式信息和版本信息*/
void code_format(code ccode[71][71], int version) {
	//char format[16] = "101010000010010";
	char format[16] = "101111001111100";
	int k = 0;
	char infor[7][19] = { "000111110010010100","001000010110111100","001001101010011001","001010010011010011",
		"001011101111110110","001100011101100010","001101100001000111" };
	for (int i = 0; i < 15; i++) {
		if (i >= 6 && i < 8)
			k = 2 + i;
		else if (i >= 8)
			k = 4 * version + 3 + i;
		else
			k = i + 1;
		ccode[k][9].show = format[14 - i] - 48;
	}
	for (int i = 0; i < 15; i++) {
		if (i == 8)
			k = 16 - i;
		else if (i > 8)
			k = 15 - i;
		else
			k = 17 + 4 * version - i;
		ccode[9][k].show = format[14 - i] - 48;
	}                              //格式信息填充完成
	k = 17;
	if (version > 6) {       //版本信息
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				ccode[7 + 4 * version + j][i + 1].show = infor[version - 7][k] - 48;
				ccode[i + 1][7 + 4 * version + j].show = infor[version - 7][k] - 48;
				k--;
			}
		}
	}
}