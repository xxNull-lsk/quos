#ifndef PROCESS_H
	#include "include/defs.h"
	#include "include/file.h"
	#include "include/stdc.h"
	#define PROCESS_H
	#define MAX_PROCESS 5
	#define MAX_FILE 64
	#define PATH_SIZE 128
	
	struct fileNO
	{
		BYTE NO;
		File far * fileStruct;
	};
	struct PCB //���̿��ƿ�
	{
		BYTE byProcessNO;		// ���̺�
		//File far * file[MAX_FILE];	// �ý��̴򿪵��ļ���
		struct fileNO file[MAX_FILE];
		char strRootPath[PATH_SIZE];	// �ý��̵ĸ�Ŀ¼
		char strArg[PATH_SIZE];			// �����в���
		union REGS r;					// �ý��̵����л����Ĵ�����ֵ�����������л����������ڻ���֧�������л�
		struct SREGS sr;
		WORD IP;
		WORD SP;
		WORD BP;
	};
	struct exeHead
	{
		WORD wMagic;			// ����
		WORD wSizeInLastBlock;	// ���һ����ʹ�õ��ֽ���
		WORD wBlockCount;		// 512�ֽڿ�ĸ���
		WORD wReallocCount;		// �ض�λ�����
		WORD wHeadSize;			// �ļ�ͷ����С����16�ֽ�Ϊ��λ��
		WORD wMinAlloc;			// �����������С�ڴ���
		WORD wMaxAlloc;			// �������������ڴ���
		WORD wInitSS;			// ��ʼSS
		WORD wInitSP;			// ��ʼSP
		WORD wChecksum;			// �ļ�У���(�������ã�Ϊ0)
		WORD wInitIP;			// ��ʼIP
		WORD wInitCS;			// ��ʼCS
		WORD wFirstRealloc;		// �ض�λ������λ��(��һ���ض�λ��������ļ���ƫ��ֵ)
		WORD wNoverlay;			// �ص��ĸ���������Ϊ0
	};
	BOOL processManageInit();
	int exec(char far * strProgameName, char far * args);
#endif
