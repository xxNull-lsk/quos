//�ļ�����systemInt.c
//���ܣ�
//	ϵͳ�ж���غ���
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- systemInt.obj systemInt.c
//  ��˹
//2007-6-8
#include "include/stdc.h"
#include "include/memManage.h"
#include "include/sysInt.h"
#include "include/globals.h"
#include "include/file.h"
#include "include/errorNO.h"
//// ��ʼ���ж�
//void sysInt_init()
//{	
//	void sysInt();
//	BOOL  (far * pSysInt)();
//	WORD wSegAddress, wOffsetAddress;
//	pSysInt = (void far *)sysInt;	
//    wSegAddress = FP_SEG(pSysInt);
//    wOffsetAddress = FP_OFF(pSysInt);
//	
//    //��װ�ж�
//    _asm
//    {
//		mov ax, 0
//		mov es, ax
//        mov ax, wOffsetAddress
//        mov es:[21h*4], ax		;// ƫ����
//        mov ax, wSegAddress
//        mov es:[21h*4+2], cs	;// �ε�ַ
//	}
//}
// �жϵ��ô���
void sysInt_int22(WORD addSeg, WORD addOff)
{
	union REGS r;
	struct SREGS sr;
	WORD far * userAge;

	userAge = MK_FP(addSeg, addOff);
	r.x.cflag = *userAge;
	userAge++;
	r.x.ax = *userAge;
	userAge++;
	r.x.bx = *userAge;
	userAge++;
	r.x.cx = *userAge;
	userAge++;
	r.x.dx = *userAge;
	userAge++;
	r.x.di = *userAge;
	userAge++;
	r.x.si = *userAge;
	userAge++;
	//r.x.bp = *userAge;
	userAge++;
	//r.x.sp = *userAge;
	userAge++;
	sr.cs = *userAge;
	userAge++;
	sr.ds = *userAge;
	userAge++;
	sr.es = *userAge;
	userAge++;
	sr.ss = *userAge;

	int22_handler((union REGS far*)&r, (struct SREGS far*)&sr);
	
	userAge = MK_FP(addSeg, addOff);
	*userAge = r.x.cflag;
	userAge++;
	*userAge = r.x.ax;
	userAge++;
	*userAge = r.x.bx;
	userAge++;
	*userAge = r.x.cx;
	userAge++;
	*userAge = r.x.dx;
	userAge++;
	*userAge = r.x.di;
	userAge++;
	*userAge = r.x.si;
	userAge++;
	//*userAge = r.x.bp; 
	userAge++;
	//*userAge = r.x.sp;
	userAge++;
	*userAge = sr.cs;
	userAge++;
	*userAge = sr.ds;
	userAge++;
	*userAge = sr.es;
	userAge++;
	*userAge = sr.ss;
}
void int22_handler(union REGS far* pR, struct SREGS far* pSR)
{
	switch(pR->h.ah)
	{
		case 0x0:
			return;
		case 0x20:// ���ļ�
		{
			char far * strFileName = MK_FP(pSR->es, pR->x.dx);
			//printf("strFileName=%s, ds:dx=0x%0x:0x%0x\n",strFileName, pSR->es, pR->x.dx);
			//getch();
			pR->x.bx = fopen(strFileName, 0);
			//printf("FileN0=%d\n",pR->x.bx);
			//getch();
			if(pR->x.bx)// �򿪳ɹ�
			{
				pR->x.ax = 0;
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.ax = OPEN_FILE_FAILE;
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x21://���ļ�
		{
			//DWORD fread(BYTE far * pDest, DWORD dwSize, DWORD dwCount, FILE fileNo)
			char far * pDest = MK_FP(pSR->es, pR->x.dx);
			pR->x.ax = fread(pDest, pR->x.cx, pR->h.al, pR->x.bx);
			//printf("FileLen=%d  pDest=%p\n",pR->x.ax, pDest);
			//getch();
			if(pR->x.ax)// ��ȡ�ɹ�
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.ax = READ_FILE_FAILE;
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x22:// �ر��ļ�
		{
			//printf("sysInt:FileN0=%d\n",pR->x.bx);
			fclose(pR->x.bx);
			if(pR->x.ax)// �ɹ�
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x23:// seek�ļ�
		{
			//DWORD fseek(FILE fileNo, DWORD dwOffset, BYTE by0rigin)
			DWORD dwOffset = ((DWORD)pR->x.dx<<16) + pR->x.cx;
			//printf("sysInt:FileN0=%d\ttype=%d\n",pR->x.bx, pR->h.al);
			pR->x.ax = fseek(pR->x.bx, dwOffset, pR->h.al);
			if(pR->x.ax)// �ɹ�
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x24://�г�ָ��Ŀ¼�е��ļ�
		{
			char far * strPathName = MK_FP(pSR->es, pR->x.dx);
			fls(strPathName);
			pR->x.cflag &= !FLAG_CARRY;
			break;
		}
		case 0x25://Ϊָ���ļ�����
		{
			char far * strFileName = MK_FP(pSR->es, pR->x.dx);
			pR->x.ax = frename(strFileName, pR->x.bx);
			if(pR->x.ax)// �ɹ�
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x26: //ɾ��ָ���ļ�
		{
			pR->x.ax = fdelete(pR->x.bx);
			if(pR->x.ax)// �ɹ�
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x48:// �����ڴ�
		{
			void far * pMen = memMalloc(pR->x.bx);
			if(pMen)// �ɹ�
			{
				pR->x.bx = FP_OFF(pMen);
				pSR->es = FP_SEG(pMen);
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.ax = NO_MEMORY;
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x49:// �ͷ��ڴ�
		{
			int i = memFree(lineToFar((DWORD)pSR->es * 0x10 + pR->x.bx));
			if(i == OK)
			{
				pR->x.cflag &= 0xfffe;
			}
			else
			{
				pR->x.ax = i;
				pR->x.cflag |= 1;
			}
			break;
		}
		case 0x4C:// ���û����򷵻�ϵͳ����
		{
			BYTE retValue = pR->h.al;
			_asm
			{
				mov sp, g_Process[0].SP
				mov bp, g_Process[0].BP
				push g_Process[0].sr.ss
				pop ss
				push g_Process[0].sr.ds
				pop ds
				push es
				pop g_Process[0].sr.es
				mov ax, g_Process[0].r.x.ax
				mov bx, g_Process[0].r.x.bx
				mov cx, g_Process[0].r.x.cx
				mov dx, g_Process[0].r.x.dx
				mov si, g_Process[0].r.x.si
				mov di, g_Process[0].r.x.di
				push g_Process[0].r.x.cflag
				popf
				mov al, retValue
				push g_Process[0].sr.cs;
				push g_Process[0].IP;
				retf
			}
			break;
		}
	}
}