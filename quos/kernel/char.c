//�ļ�����char.c
//���ܣ�
//	�ַ����롢���
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- char.obj char.c
//  ��˹
//2007-6-3 
#include "include/stdc.h"
#include "include/keyboard.h"
#include "include/screen.h"
//�ж��Ƿ��ǿ���ʾ��ASCII��
BOOL isASCII(char inChar)
{
	if((inChar >= 32 && inChar <= 126) || inChar == 9)
		return 1;
	else
		return 0;
}
char getchar()
{
	char temp1 = getch();
	char temp2;
	putchar(temp1);
	temp2 = getch();
	while(temp2 != '\n' && temp2 != '\r')
	{
		temp1 = temp2;
		putchar(temp1);
		temp2 = getch();
	}
	putchar('\n');
	putchar('\r');
	return temp1;
}
char getch()
{
	return (char)getKeyCharWait();//���ü�������
}
void putchar(char c)
{
	screenShowChar(c);// ������Ļ����
}