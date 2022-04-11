//�ļ�����memManage.c
//���ܣ�
//	�ڴ������غ���
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- memManage.obj memManage.c
//  ��˹
//2007-6-7
#include "include/memManage.h"
#include "include/stdc.h"
#include "../include/quos.h"
#include "include/errorNO.h"
extern	BYTE g_byCurrProcessID;
void memInit(void)
{
	MCB far * pMCB = 0;
	// �ֹ�����������ڴ�ʹ���������
	pMCB = (MCB far *)lineToFar(FRIST_MCB);
	pMCB->front = NULL;
	pMCB->next = NULL;
	pMCB->bSate = FREE;
	// pMCB->byProcessID = 0; 
	pMCB->dwSize = MAX_MEM_SIZE - sizeof(MCB);
	// ��װ�ڴ�����ж�

}
//�����ڴ�
void far * memMalloc(WORD wSize)
{
	MCB far * pMCB = (MCB far *)lineToFar(FRIST_MCB);
	do
	{
		if(pMCB->bSate==USED)
		{
			pMCB = pMCB->next;
		}
		if((pMCB->bSate==FREE))
		{
			if(pMCB->dwSize < sizeof(MCB))
			{
				continue;
			}
			else if((pMCB->dwSize-wSize)<=sizeof(MCB))// ����ÿ��ڴ�ʣ����ڴ治���ڷ��ˣ��Ǿ�һ��ֵ���
			{
				//BYTE far * pProcessID = (BYTE far *)lineToFar(CURR_BY_PROCESS_NO);
				pMCB->byProcessID = g_byCurrProcessID;
				pMCB->bSate = USED;
				return (MCB far *)lineToFar(farToLine(pMCB) + sizeof(MCB));
			}
			else// if((pMCB->dwSize-wSize)>sizeof(MCB))
			{
				MCB far * pOldMCB = pMCB->next;
				DWORD dwOldSize = pMCB->dwSize;
				//BYTE far * pProcessID = (BYTE far *)lineToFar(CURR_BY_PROCESS_NO);

				pMCB->byProcessID = g_byCurrProcessID;
				pMCB->bSate = USED;
				pMCB->dwSize = wSize;
				pMCB->next = (MCB far *)lineToFar(farToLine(pMCB) + sizeof(MCB) + wSize);

				pMCB->next->front = pMCB;
				pMCB->next->next = pOldMCB;
				pMCB->next->bSate = FREE;
				//pMCB->next->byProcessID = 0; 
				pMCB->next->dwSize = dwOldSize - wSize - sizeof(MCB);
				return (MCB far *)lineToFar(farToLine(pMCB) + sizeof(MCB));
			}
		}
	}while (pMCB->next != NULL);// ֻҪû�е������β����һֱ������
	return NULL; // û���ҵ�
}

//�ͷ��ڴ�
int memFree(const void far * pIn)
{
	MCB far * pMCB = (MCB far *)lineToFar(farToLine(pIn) - sizeof(MCB));
	//BYTE far * pProcessID = (BYTE far *)lineToFar(CURR_BY_PROCESS_NO);

	MCB far * pFront = pMCB->front;
	MCB far * pNext = pMCB->next;
	
	//����ͷ��ߺ������߲���ͬһ�����̣��Ͳ������ͷ�
	if(pMCB->byProcessID != g_byCurrProcessID) return NO_RIGHT;
	//������ͷŵ��ڴ��û�б�����ʹ�ù����Ͳ������ͷ�
	if(pMCB->bSate != USED) return INVALIDATION;
	if(pFront)
	{
		if(pFront->bSate == FREE)// ���ǰһ���ڴ����ڲ����ǿ��еģ��Ͱ�Ҫ�ͷŵ��ڴ�������ǰһ������һ��
		{
			pFront->dwSize += pMCB->dwSize + sizeof(MCB);
			pFront->next = pNext;
			pMCB->bSate = UNKNOW;
			if(pNext)
				pNext->front = pFront;
		}
	}
	if(pNext)
	{
		if(pNext->bSate == FREE)// �����һ���ڴ����ڲ����ǿ��е�
		{
			if(pMCB->bSate == UNKNOW)// ����Ѿ���ǰ��ĺ���һ����,��ô�Ͱ�ǰһ��ͺ�һ�����һ��
			{
				pFront->dwSize += pNext->dwSize + sizeof(MCB);
				pFront->next = pNext->next;
				if(pNext->next)
					pNext->next->front = pFront;
				return OK;
			}
			else// ���û�и�ǰһ������һ��,��ֱ�Ӹ���һ������һ���ͷ�
			{
				pMCB->bSate = FREE;
				//pMCB->byProcessID = 0;
				pMCB->dwSize += pNext->dwSize + sizeof(MCB);
				pMCB->next = pNext->next;
				if(pNext->next)
					pNext->next->front = pFront;
				return OK;
			}
		}
	}
	//���ǰ��ĺ���Ķ����ǿ��е��ڴ��,��ֱ���ͷ���
	pMCB->bSate = FREE;
	//pMCB->byProcessID = 0;
	return OK;
}