//�ļ�����disk.c
//���ܣ�
//	�ṩ��Ӳ�̵Ķ�д����
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- disk.obj disk.c
//  ��˹
//2007-6-11
#include "include/stdc.h"
#include "include/disk.h"
#include "include/errorNO.h"

WORD diskReadSector(DWORD dwSector, WORD wCount, BYTE bDriveNO, BYTE far *pBuffer)
{
	return Int13Disk(0x42, dwSector, wCount, bDriveNO, pBuffer);
}

WORD diskWriteSector(DWORD dwSector, WORD wCount, BYTE bDriveNO, BYTE far *pBuffer)
{
	return Int13Disk(0x43, dwSector, wCount, bDriveNO, pBuffer);
}

WORD Int13Disk(BYTE bKey, DWORD dwLBA, WORD wCount, BYTE bDriveNO, void far *pBuf)
{
	union REGS r;
	struct SREGS sr;
	DISK_ADDR_PACKET pPackage;
    BYTE far *p = (BYTE far *)&pPackage;
	memset((union REGS far *)&r, 0, sizeof(union REGS));
	memset((struct SREGS far *)&sr, 0, sizeof(struct SREGS));
	#ifdef DEBUG_DISK
	{
		printf("\tInt13Disk: bKey=0x%x  dwLBA=%ld wCount=%d\n\tbDriveNO=%d  pBuf=0x%lx\n", bKey, dwLBA, wCount, bDriveNO, farToLine(pBuf));
	}
	#endif
	if(bDriveNO<DISK_A)
	{
		BYTE byTemp, s;
		dwLBA++;//��ΪLBA�Ǵ�1��ʼ�ģ������ϵĶ��̷�ʽ�Ǵ�0��ʼ��
		byTemp = dwLBA/18;//��Щ����Ӧ�ô��豸��Ϣ�����������18��512
		s = dwLBA%18;
		if(byTemp)s++;
		//printf("pBuf:%s\n",pBuf);
		//getch();
		return Int13Floppy(bKey-0x40, wCount, byTemp>>1, s, byTemp&1, bDriveNO, pBuf);
	}
// �����������Ƿ�֧����չ��
	r.h.ah = 0x41;
	r.x.bx = 0x55aa;
	int86(0x13, (union REGS far *)&r, (union REGS far *)&r);
	if(r.x.bx!=0xaa55 || (r.x.cflag&FLAG_CARRY))
	{
		return CAN_NOT_USER_LBA;
	}
    pPackage.bySize			= sizeof(pPackage);
	pPackage.byReserved		= 0;
	pPackage.wBlockNum		= wCount;
    pPackage.dwBufferAddr	= pBuf;
	pPackage.dwBeginBlockLo	= dwLBA;
	pPackage.dwBeginBlockHi	= 00;
	sr.ds					= FP_SEG(p);
    r.x.si					= FP_OFF(p);
	r.h.ah					= bKey;
	r.h.dl					= bDriveNO;
	
	int86x(0x13, (union REGS far *)&r, (union REGS far *)&r, (struct SREGS far *)&sr);
//    _asm
//    {
//		mov     ah, 41h     ;// ��д
//		mov		bx, 55aah
//		mov		dl, 80h     ;// Ӳ��
//
//		pushf
//		pop		wf;
//		int		13h
//		pushf
//		pop		r.x.cflag;
//		mov		r.x.ax, ax
//		mov		r.x.bx, bx
//		mov		r.x.cx, cx
//	}
//r.h.ah = 0x41;
//r.x.bx = 0x55aa;
//int86x(0x13, (union REGS far *)&r, (union REGS far *)&r, (struct SREGS far *)&sr);
	#ifdef DEBUG_DISK
	{
		printf("\tInt13Disk: return: r.x.ax=0x%x r.x.cflag=%b\n", r.x.ax, r.x.cflag);
		getch();
	}
	#endif
	return r.x.ax;
}