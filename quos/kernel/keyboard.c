//�ļ�����keyboard.c
//���ܣ�
//	ͨ�ü�������
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- keyboard.obj keyboard.c
//  ��˹
//2007-6-3
#include "include/keyboard.h"
BYTE keyBroadState(void)
{
	BYTE byState;
	_asm
	{
		mov     ah, 02h
		int     16h
		mov     byState, al
	}
	return byState;
}
BYTE getKeyChar(void)
{
	return (BYTE)(getKeyCodeChar()<<8>>8);
}
BYTE getKeyCode(void)
{
	return (BYTE)(getKeyCodeChar()>>8);
}
WORD getKeyCodeChar(void)
{
	WORD wKey;
	WORD wZF = TRUE;
	_asm
	{
		mov     ah, 01h
		int     16h
		jnz		NoZero
		mov		wZF, FALSE
	NoZero://������ַ����룬���ػ�ȡ�����ַ������Ѽ��̻�������գ���Ϊ�жϲ����Զ����
		mov     wKey, ax

		//��ռ��̻�����
		mov		ax, 0x40
		mov		es, ax
		mov		ax, es:0x1a
		mov		es:0x1c, ax
	}
	
	if(wZF)
		return wKey;
	else
		return wZF;
}
BYTE getKeyCharWait(void)
{
	return (BYTE)(getKeyCodeCharWait()<<8>>8);
}
BYTE getKeyCodeWait(void)
{
	return (BYTE)(getKeyCodeCharWait()>>8);
}
WORD getKeyCodeCharWait(void)
{
	WORD wKey;
	_asm
	{
		mov     ah, 00h
		int     16h
		mov     wKey, ax
	}
	return wKey;
}
char CodeToChar(BYTE byCode)
{
	//switch(by)
		//case 1:return 
		return 0;
}