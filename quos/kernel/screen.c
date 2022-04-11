//�ļ�����screes.c
//���ܣ�
//	��Ļ����
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- screes.obj screes.c
//  ��˹
//2007-6-16
#include "include/stdc.h"
#include "include/screen.h"

BYTE far * g_pVideo = 0;		// �Դ�λ��
BYTE g_byCursorRow = 0;			// ��ǰ���λ�õ��к�
BYTE g_byCursorCol = 0;			// ��ǰ���λ�õ��к�
BYTE g_byCurrColor = COLOR_GRAY;// ��ǰ�ַ���ʾ��ɫ
BYTE g_bySystemColor = COLOR_GRAY;// �������ڱ���
void setScreenColor(BYTE byColor)
{
	g_byCurrColor = byColor;
}
BYTE getScreenColor()
{
	return g_byCurrColor;
}
void getComebackScreenColor()
{
	g_byCurrColor = g_bySystemColor;
}

//�ı�ģʽ��ʼ��
void screenInit(BYTE byModeNO)
{
	WORD far * pCursor = lineToFar(0x450);
	_asm
	{
		mov al, byModeNO
		mov ah, 00
		int 10h
	}
	// ��ʼ���������
	g_byCursorRow = *pCursor>>8;
	g_byCursorCol = (*pCursor&0xff) * 2;
	g_pVideo = lineToFar(0xb8000);
	g_byCurrColor = COLOR_GRAY;	
}

void screenShowChar(char ch)
{
	WORD far * pCursor = lineToFar(0x450);
	DWORD dwCharAdd = 0;
	g_byCursorRow = *pCursor >> 8;
	g_byCursorCol = (*pCursor & 0xff) * 2;
	dwCharAdd = g_byCursorRow * (MAX_COL + 1) * 2 + g_byCursorCol;
	// ������λ��
	switch (ch)
	{
		case '\n':
			g_byCursorRow++;
			break;
		case '\r':
			g_byCursorCol=0;
			break;
		case '\b':
			if(g_byCursorCol>=2)
			{
				g_byCursorCol-=2;
			}
			else
			{
				g_byCursorRow--;
				g_byCursorCol = MAX_COL * 2;
			}
			break;
		default:
			g_pVideo[dwCharAdd] = ch;
			dwCharAdd++;
			g_pVideo[dwCharAdd] = g_byCurrColor;
			dwCharAdd++;
			g_byCursorCol += 2;
	}
	update_cursor();
}
void update_cursor()
{
	// ����Ƿ�Խ��
	if(g_byCursorCol > MAX_COL * 2)// ���д���
	{
		g_byCursorCol=0;
		g_byCursorRow++;
	}
	if(g_byCursorRow > 24)// ��������
	{
		screenUp(1);
	}
	_asm
	{
		mov ah, 2
		mov bh, 0
		mov dh, g_byCursorRow
		mov dl, g_byCursorCol
		shr dl, 1
		int 10h
	}
}
//�Ͼ���
void screenUp(BYTE byCount)
{
	_asm
	{
		mov	ah, 6
		mov	al, byCount
		mov	bh, g_byCurrColor
		mov	ch, 1 ;// ������һ�в�����
		mov	cl, 0
		mov dh, MAX_ROW
		mov dl, MAX_COL
		int 10h
	}
	g_byCursorRow--;
	update_cursor();
}
//�ı�ģʽ����
void screenClear(void)
{
	_asm
	{
		mov	ah, 6
		mov	al, 0
		mov	bh, 7
		mov	ch, 0
		mov	cl, 0
		mov dh, MAX_ROW
		mov dl, MAX_COL
		int 10h
	}
	g_byCursorCol = 0;
	g_byCursorRow = 0;
	update_cursor();
}