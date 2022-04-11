#ifndef MEMMANAGE_H
	#define MEMMANAGE_H
	#include "defs.h"
	#define FREE 0
	#define USED 1
	#define UNKNOW 2
// ��һ��MCB�ĵ�ַ
	#define FRIST_MCB 0x500
	#define MAX_MEM_SIZE 0x70000 - FRIST_MCB
	typedef struct memControlBlock
	{
		struct memControlBlock far * front;
		struct memControlBlock far * next;
		BOOL bSate;		// �ڴ�ʹ��״̬��
		BYTE byProcessID;	// �ڴ��������̺�
		DWORD dwSize;	// ʵ�ʿ����ڴ�Ĵ�С��������MCB�Ĵ�С
	}MCB;
	void memInit(void);
	void far * memMalloc(WORD wSize);
	int memFree(const void far * pIn);
#endif //ifdef  MEMMANAGE_H