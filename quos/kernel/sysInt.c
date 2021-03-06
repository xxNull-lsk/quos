//文件名：systemInt.c
//功能：
//	系统中断相关函数
// 编译方法：
//		cl /nologo /c /Gs /G3 /w /Gx- systemInt.obj systemInt.c
//  如斯
//2007-6-8
#include "include/stdc.h"
#include "include/memManage.h"
#include "include/sysInt.h"
#include "include/globals.h"
#include "include/file.h"
#include "include/errorNO.h"
//// 初始化中断
//void sysInt_init()
//{	
//	void sysInt();
//	BOOL  (far * pSysInt)();
//	WORD wSegAddress, wOffsetAddress;
//	pSysInt = (void far *)sysInt;	
//    wSegAddress = FP_SEG(pSysInt);
//    wOffsetAddress = FP_OFF(pSysInt);
//	
//    //安装中断
//    _asm
//    {
//		mov ax, 0
//		mov es, ax
//        mov ax, wOffsetAddress
//        mov es:[21h*4], ax		;// 偏移量
//        mov ax, wSegAddress
//        mov es:[21h*4+2], cs	;// 段地址
//	}
//}
// 中断调用处理
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
		case 0x20:// 打开文件
		{
			char far * strFileName = MK_FP(pSR->es, pR->x.dx);
			//printf("strFileName=%s, ds:dx=0x%0x:0x%0x\n",strFileName, pSR->es, pR->x.dx);
			//getch();
			pR->x.bx = fopen(strFileName, 0);
			//printf("FileN0=%d\n",pR->x.bx);
			//getch();
			if(pR->x.bx)// 打开成功
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
		case 0x21://读文件
		{
			//DWORD fread(BYTE far * pDest, DWORD dwSize, DWORD dwCount, FILE fileNo)
			char far * pDest = MK_FP(pSR->es, pR->x.dx);
			pR->x.ax = fread(pDest, pR->x.cx, pR->h.al, pR->x.bx);
			//printf("FileLen=%d  pDest=%p\n",pR->x.ax, pDest);
			//getch();
			if(pR->x.ax)// 读取成功
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
		case 0x22:// 关闭文件
		{
			//printf("sysInt:FileN0=%d\n",pR->x.bx);
			fclose(pR->x.bx);
			if(pR->x.ax)// 成功
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x23:// seek文件
		{
			//DWORD fseek(FILE fileNo, DWORD dwOffset, BYTE by0rigin)
			DWORD dwOffset = ((DWORD)pR->x.dx<<16) + pR->x.cx;
			//printf("sysInt:FileN0=%d\ttype=%d\n",pR->x.bx, pR->h.al);
			pR->x.ax = fseek(pR->x.bx, dwOffset, pR->h.al);
			if(pR->x.ax)// 成功
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x24://列出指定目录中的文件
		{
			char far * strPathName = MK_FP(pSR->es, pR->x.dx);
			fls(strPathName);
			pR->x.cflag &= !FLAG_CARRY;
			break;
		}
		case 0x25://为指定文件改名
		{
			char far * strFileName = MK_FP(pSR->es, pR->x.dx);
			pR->x.ax = frename(strFileName, pR->x.bx);
			if(pR->x.ax)// 成功
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x26: //删除指定文件
		{
			pR->x.ax = fdelete(pR->x.bx);
			if(pR->x.ax)// 成功
			{
				pR->x.cflag &= !FLAG_CARRY;
			}
			else
			{
				pR->x.cflag |= FLAG_CARRY;
			}
			break;
		}
		case 0x48:// 申请内存
		{
			void far * pMen = memMalloc(pR->x.bx);
			if(pMen)// 成功
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
		case 0x49:// 释放内存
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
		case 0x4C:// 从用户程序返回系统进程
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
