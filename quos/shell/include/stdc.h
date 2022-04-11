#ifndef STDC_H
	#define STDC_H
	#include "defs.h"
	#include "stdarg.h"

	typedef unsigned short FILE;

	#define FILE_NAME_SIZE 8
	#define FILE_EXT_SIZE 3

	//Ԥ������ɫֵ
	#define COLOR_BLACK		0X00
	#define COLOR_GRAY		0X07
	#define COLOR_BLUE		0x09
	#define COLOR_GREEN		0x0A
	#define COLOR_READ		0x0C
	#define COLOR_YELLOR	0X0E
	#define COLOR_WHITE		0X0F
	//#define ���		0X0D
	//#define ��			0X0B

	// ƫ�Ʊ�־
	#define SEEK_CUR 0
	#define SEEK_SET 1
	#define SEEK_END 2

	// ��־λ
	#define FLAG_CARRY		0X01

	// ��������
	#define FLOPPY_A 00
	#define DISK_A 0x80

	struct WORDREGS {
		WORD ax;
		WORD bx;
		WORD cx;
		WORD dx;
		WORD si;
		WORD di;
		WORD cflag;
		};

	struct BYTEREGS {
		BYTE al, ah;
		BYTE bl, bh;
		BYTE cl, ch;
		BYTE dl, dh;
		};

	union REGS {
		struct WORDREGS x;
		struct BYTEREGS h;
		};

	struct SREGS {
		WORD es;
		WORD cs;
		WORD ss;
		WORD ds;
		};
//	struct ALL_REGS
//	{
//		union REGS r;
//		struct SREGS s;
//	};

	// mem.c
	void memcpy(void far *pDest, const void far *pSrc, WORD wCount);
	WORD memcmp(void far *pDest, const void far *pSrc, WORD wCount);
	void memset(void far *pDest, BYTE byChar, WORD wCount);
	void far *malloc(WORD wSize);
	void free(void far * pMem);
	// string.c
	void puts(const char far * pStr);
	char far * gets(char far * pStrOut);
	int strcmp(const char far * pStr1, const char far * pStr2);
	WORD strlen(const char far * pStr);
	char far * strcpy(char far * pStrOut, const char far * pStrIn);
	char far * strncpy(char far * pStrOut, const char far * pStrIn, DWORD count);
	char far * strcat(char far * pStrOut, const char far * pStrIn);
	char far * strLTrim(char far * pStr);// ȥ���ַ�����ߵĿո�
	DWORD strnchr(const char far * pStrOut, char ch, DWORD len);
	// char.c
	BOOL isASCII(char CharIn);// �ж��Ƿ��ǿ���ʾ��ASCII��
	char getchar();
	char getch();
	void putchar(char CharIn);
	// portio.c
	void outb(WORD port, BYTE databyte);
	void outw(WORD port, WORD dataword);
	BYTE inb(WORD port);
	WORD inw(WORD port);

	// convert.c
	WORD atoi(const char far * pStrIn);
	DWORD atol(const char far * pStrIn);
	char far * itoa(WORD wNumber, char far * pStrOut);
	char far * ltoa(DWORD dwNumber, char far * pStrOut);
	DWORD xtol(char far * pStrOut);// ��һ��16�����������ַ�����ʾ��ת����10������
	void far * lineToFar(DWORD dwIn);//���Ե�ַת���ɳ�ָ���ַ
	DWORD farToLine(const void far * pIn);//��ָ���ַת�������Ե�ַ
	char far * strTOSuper(char far * pString);
	char far * strTOLower(char far * pString);
	char super(char ch);
	char lower(char ch);

	// int86.c
	int int86(int iIntNO, const union REGS far * pRegsIn, union REGS far* pRegsOut);
	int int86x(int iIntNO, const union REGS far* pRegsIn, union REGS far* pRegsOut, struct SREGS far* pSregs);

	// printf.c
	int printf(char far * pStrFormat, ...);
	static char far * number(char far * str, int num, int base, int size, int precision, int type);
	static char far * numberl(char far * str, long num, int base, int size, int precision, int type);

	// math.c
	DWORD pow(WORD x, WORD y);// ����x��y�η�

	// file.c
	FILE fopen(char far * strFileName, char far * strMode);
	DWORD fread(BYTE far * pDest, WORD wSize, BYTE byCount, FILE fileNo);
	void fclose(FILE fileNo);
	DWORD fseek(FILE fileNo, DWORD dwOffset, BYTE by0rigin);
	void fls(char far * strPathName);
	BOOL frename(FILE fileNo, char far * strFileName);
	BOOL fdelect(FILE fileNo);
#endif //STDC_H