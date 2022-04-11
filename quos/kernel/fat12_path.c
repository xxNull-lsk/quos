//�ļ�����fat12.c
//���ܣ�
//	FAT12�ļ�ϵͳ��غ���
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- fat12.obj fat12.c
//  ��˹
//2007-6-16

#include "include/stdc.h"
#include "include/fat12.h"
#include "include/fat12_path.h"
#include "include/disk.h"
#include "include/floppy.h"
#include "include/File.h"
#include "include/memManage.h"
#include "include/panic.h"
#include "include/errorNO.h"
#include "include/time.h"

void testh()
{
	BYTE far * p = 0, far *w;
	BYTE far * pAdd =  (BYTE far *)malloc(200);
	while(1)
	{
		int j = 8;
		puts("Input a memory address:0x");
		p = lineToFar(xtol(gets(pAdd)));
		w = p;
		if(strlen(pAdd)==0)break;
		while(j--)
		{
			int i = 16;
			BYTE far * temp = w;
			printf("0x%05lx\t", farToLine(w));
			while(i--)
			{
				printf("%02x ", (BYTE)*w);
				w++;
			}
			puts("; ");
			for(i=16; i>0; i--)
			{
				if(*temp>=21 && *temp<=125)
					putchar(*temp);
				else
					putchar('.');
				temp++;
			}
			puts("\n");
		}
	}
	free(pAdd);
}
// �Ӷ�Ӧ��Ŀ¼���ҵ�ָ�����ļ�
#ifndef FSPar
	#define FSPar pFile->pFSPar
#endif
WORD fat12_GetDirSector(const File far * pFile, BYTE far *pBuf, DWORD far *pDirEntry)
{
	BOOL bRet;
	// ע�⣺�������֧��Ŀ¼
	WORD wRootSectorNO = FSPar->byNumFats * FSPar->wSecPerFat + FSPar->wSecReserved;
	WORD wSectorNO = wRootSectorNO;
	struct FAT12DirEntry far * pp;
	int i = 0, j = 0;
	// �Ӹ�Ŀ¼��һ������һ�����������ļ�
	while(1)
	{
		// �Ӹ�Ŀ¼���Ҹ��ļ�	
		bRet = diskReadSector(wSectorNO - 1, 1, pFile->byDevNO, pBuf);
		if(!bRet)
		{
			#ifdef DEBUG_FAT12
			printf("\tfat12_ReWorkDir: Error when read sector[%d] in drive[%x]!\n", wSectorNO, pFile->byDevNO);
			#endif
			return FALSE;
		}
		pp = (struct FAT12DirEntry far *)pBuf;
		//testh();
		for(j=0; j<8; j++)
		{
			if(memcmp((char far *)pFile->strName, (char far *)(pp), FILE_NAME_SIZE+FILE_EXT_SIZE)==0 )
			{
				//�ҵ����ļ���
				*pDirEntry = (DWORD)pp;
				return wSectorNO;
			}
			pp++;
		}
		wSectorNO++;
		i++;
		if(i>(FSPar->wRootUnits/8))
		{
			break;
		}
	}
	return FALSE;
}
struct FAT12DirEntry far * fat12_GetDirSectorNO(char far * path, WORD NO, BYTE far *pBuf)
{
	BOOL bRet;
	// ע�⣺�������֧��Ŀ¼
	struct FAT12Parameter far * pFSPar = getFat12Par();
	WORD wRootSectorNO = pFSPar->byNumFats * pFSPar->wSecPerFat + pFSPar->wSecReserved;
	WORD wSectorNO = wRootSectorNO + NO;
	struct FAT12DirEntry far * pp;
	int i = 0, j = 0;
	// ��ȡ������
	bRet = diskReadSector(wSectorNO - 1, 1, FLOPPY_A, pBuf);
	if(!bRet)
	{
		#ifdef DEBUG_FAT12
		printf("\tfat12_ReWorkDir: Error when read sector[%d] in drive[%x]!\n", wSectorNO, pFile->byDevNO);
		#endif
		return NULL;
	}
	pp = (struct FAT12DirEntry far *)pBuf;
	if(*(BYTE far *)pp == 0)
		return NULL;
	else
		return pp;
}
