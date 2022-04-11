//�ļ�����process.c
//���ܣ�
//	���̹�����صĺ���
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- process.obj process.c
//  ��˹
//2007-6-16
#include "include/process.h"
#include "include/file.h"
#include "include/stdc.h"
#include "include/errorNO.h"
#include "include/memManage.h"
BYTE g_byCurrProcessID = 0;		// ��ǰ���еĽ��̺�
struct PCB g_Process[MAX_PROCESS];// ϵͳ�����н��̵���Ϣ
BOOL processManageInit()
{
	BYTE i = 0;
	g_byCurrProcessID = 0;
	for(i=0; i<MAX_PROCESS; i++)
		memset(lineToFar(farToLine((struct PCB far *)g_Process) + i), 0, sizeof(struct PCB));

	// ��ʼ��ϵͳ����
	strcpy((char far *)g_Process[0].strRootPath, "a:\\");
}
struct PCB far * allocProcessNO()
{
	int i = 1;
	for(i=1; i<MAX_PROCESS; i++)
	{
		if(g_Process[i].byProcessNO==0)
		{
			g_Process[i].byProcessNO = i;
			return (struct PCB far *)&g_Process[i];
		}
	}
	return 0;
}
void freeProcessNO(BYTE byProcessNO)
{	
	if(g_Process[byProcessNO].byProcessNO == 0)
	{// �ͷ�û��ʹ�õĽ��̿��ƿ��ϵͳ���̿��ƿ飬���ǲ�����ģ��϶���BUG����Ȼ��Ҳ�п����Ǳ�����
		bug(BUG_freeProcessNO_1);
	}
	else if(g_Process[byProcessNO].byProcessNO == byProcessNO)
	{
		g_Process[byProcessNO].byProcessNO = 0;
	}
	else
	{// ������̺żȲ���0Ҳ������ȷ��ֵ���ǿ϶�ϵͳ���ƻ��ˣ�û�а취ֻ��������
		panic(PANIC_freeProcessNO_1);
	}
	return 0;
}
int exec(char far * strProgameName, char far * args)
{
	DWORD dwProgSize = 0, dwHeadSize = 0, dwBodySize = 0;
    WORD far *pBody = 0, far *pProgFile = 0, far *p = 0, far *pReallocTable = 0;
	BYTE far *pBuf = 0;
    WORD i = 0, wSeg = 0, wOff = 0;
	struct exeHead far *pHead = 0;
	struct PCB far *pPCB;
	BYTE byRet = 0;
	FILE FileNO = 0;
	
	// ����������ڴ�
	FileNO = fopen(strProgameName, "r");
	if(!FileNO)
	{
		#ifdef DEBUG_PROCESS
		puts("\texec:Error when open the progame file.\n");
		#endif
		goto error;
	}
	dwProgSize = fseek(FileNO, 0, SEEK_END);
	fseek(FileNO, 0, SEEK_SET);
	pBuf = memMalloc(dwProgSize+0x10);// ������16���ֽڣ��Է�ֹ����ָ����ȡ�ļ������
	if(!pBuf)
	{
		#ifdef DEBUG_PROCESS
		puts("\texec:Error when alloc the progame space.\n");
		#endif
		goto error;
	}

	wSeg = FP_SEG(pBuf);
	wOff = FP_OFF(pBuf);
	// �����ڴ�ָ��
	if(wOff)
	{
		wSeg++;
		pProgFile = MK_FP(wSeg, 0);
	}
	else
	{
		pProgFile = pBuf;
	}
	byRet = fread(pProgFile, dwProgSize, 1, FileNO);
	if(!byRet)
	{
		#ifdef DEBUG_PROCESS
		puts("\texec:Error when read the progame to memory.\n");
		#endif
		goto errorFree;
	}
	fclose(FileNO);


	// ��������
	pHead = (struct exeHead far *)pProgFile;
	// ��֤�����Ƿ���Ч
	if(pHead->wMagic != 0x5A4D)
	{
		// Ҳ����˵��COM����Ҳ���������ֱ����ת��ȥ�Ϳ�����
		#ifdef DEBUG_PROCESS
		printf("\texec:This file isn't a dos-EXE program! pHead->wMagic=0x%x\n", pHead->wMagic);
		#endif
		goto ToUserProcess;
	}
	
		#ifdef DEBUG_PROCESS
		printf("\texec:This file is a dos-EXE program! pHead->wMagic=0x%x\n", pHead->wMagic);
		#endif
	//����exeͷ�Ĵ�С
	dwHeadSize = pHead->wHeadSize * 0x10;
	dwBodySize = dwProgSize - dwHeadSize;
	pBody = lineToFar(farToLine(pProgFile) + dwHeadSize);
	// ����ض������⣬����Ҫ�ض���ĵط����ϳ���Ķε�ַ
	pReallocTable = (WORD far *)(pHead->wFirstRealloc + (DWORD)pHead);
    for (i=0; i<pHead->wReallocCount; i++)
    {
        p = (WORD far *)lineToFar(farToLine(pBody) + (DWORD)(pReallocTable[2*i] + pReallocTable[2*i+1] * 0x10));
				#ifdef DEBUG_PROCESS
				printf("%d[%ld]  *p=%04x   ", i, (DWORD)(pReallocTable[2*i] + pReallocTable[2*i+1] * 0x10), *(WORD far *)p, farToLine(p));
				#endif
				*p = *p + wSeg + 0x20;
				#ifdef DEBUG_PROCESS
				printf("%04x  %04x\n", *(WORD far *)p, wSeg);
				#endif
    }
	// �����û�����Ŀ��λ��
	wSeg = FP_SEG(pBody);
	wOff = FP_OFF(pBody);

ToUserProcess:
	// �������̹�����Ϣ
	pPCB = allocProcessNO();
	if(!pPCB)
	{
		#ifdef DEBUG_PROCESS
		puts("\texec:Error when alloc the process NO.\n");
		#endif
		goto errorFree;
	}
	strcpy((char far *)pPCB->strRootPath, "a:\\");
	strcpy((char far *)pPCB->strArg, args);
	// �޸ĵ�ǰ���̺�
	g_byCurrProcessID = pPCB->byProcessNO;
	#ifdef DEBUG_PROCESS
	printf("\texec:jmp to user progame 0x%04x:0x%04x\n", wSeg, wOff);
	#endif
	_asm
	{
		;// ���ֵ�ǰ״̬���Ա���û����̷��ص�ʱ��ָ�
		mov g_Process[0].r.x.ax, ax
		mov g_Process[0].r.x.bx, bx
		mov g_Process[0].r.x.cx, cx
		mov g_Process[0].r.x.dx, dx
		mov g_Process[0].r.x.si, si
		mov g_Process[0].SP, sp
		mov g_Process[0].BP, bp
		mov g_Process[0].r.x.di, di
		pushf
		pop g_Process[0].r.x.cflag
		push ds
		pop g_Process[0].sr.ds
		push es
		pop g_Process[0].sr.es
		push cs
		pop g_Process[0].sr.cs
		push ss
		pop g_Process[0].sr.ss
		mov g_Process[0].IP, offset RETURN_ADD
		
		// ��ת���û�����
		push wSeg;
		push wOff;
		retf
	RETURN_ADD:
		mov byRet, al
	}
	// �ͷ��û����̵Ĺ�����Ϣ��
	freeProcessNO(g_byCurrProcessID);
	
	// �޸ĵ�ǰ���̺�Ϊϵͳ����
	g_byCurrProcessID = 0;

	// �ͷŵ��û�������ʹ�õ��ڴ�
	memFree(pBuf);
	#ifdef DEBUG_PROCESS
	printf("\n\texec:return from user progame %p\n", pProgFile);
	getch();
	#endif
	// ����ϵͳ
	return byRet;

errorFree:
	memFree(pProgFile);
error:
	return -1;
}