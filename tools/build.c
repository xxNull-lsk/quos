#include <stdio.h>
#define PRINT_SIZE(string, size)	do{printf(string);\
									if(size/1024 <= 0){\
										printf("%ld WORD.\n" , size);\
									}else if(size/1024/1024 <= 0){\
										printf("%.2f KB.\n" , (float)size/1024);\
									}else{\
										printf("%.2f MB.\n" , (float)size/1024/1024);\
									}}while(0)
#define BYTE	unsigned char
#define WORD	unsigned short
#define DWORD	unsigned long
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
void outArgv(char * strProgrameName)
{
	printf("argv:\t%s srcFile descFile BaseSegmentAddress", strProgrameName);
}
int main(int argc, char *argv[])
{
	DWORD dwFileSize = 0, dwHeadSize = 0, dwBodySize = 0;
    WORD *pBody = 0, *pFile = 0, *p = 0, *pReallocTable = 0;
    WORD wBaseSegment = 0, i = 0;
	FILE *fileSrc = 0, *fileDesc = 0;
	struct exeHead *pHead = 0;
    if(argc <= 3)
	{
		printf("Error[0]: Not enough argvs!\n");
		outArgv(argv[0]);
		return 1;
    }
	wBaseSegment = (WORD)atoi(argv[3]);
	if(wBaseSegment == 0)
	{
		printf("Error[1]: BaseSegmentAddress is error! It must be a number.\n");
		outArgv(argv[0]);
		return 1;
	}
    fileSrc = fopen(argv[1], "rb");
    if(fileSrc == NULL)
	{
		printf("Error[2]: Can't open source file!\n");
		outArgv(argv[0]);
		return 1;
    }
    fseek(fileSrc, 0L, SEEK_END);
	dwFileSize = ftell(fileSrc);
    fseek(fileSrc, 0L, SEEK_SET);
	
// ����exe�ļ�
	pFile = (WORD *)malloc(dwFileSize);
	if(pFile == NULL)
	{
		printf("Error[4]: Not enough memory!\n");
		return 1;
	}
	fread(pFile, dwFileSize, 1, fileSrc);
	fclose(fileSrc);

	pHead = (struct exeHead *)pFile;
// ��֤�����Ƿ���Ч
	if(pHead->wMagic != 0x5A4D)
	{
		printf("Error[5]: This file isn't a dos-EXE program!\n");
		return 1;
	}
//����exeͷ�Ĵ�С
	dwHeadSize = pHead->wHeadSize * 0x10;
	dwBodySize = dwFileSize - dwHeadSize;
	pBody = (WORD *)((DWORD)pFile + dwHeadSize);
// ����ض������⣬����Ҫ�ض���ĵط����ϳ���Ķε�ַ
	pReallocTable = (WORD *)(pHead->wFirstRealloc + (DWORD)pHead);
    for (i=0; i<pHead->wReallocCount; i++)
    {
        p = (WORD *)((DWORD)pBody + pReallocTable[2*i] + pReallocTable[2*i+1] * 0x10);
		printf("%d[%ld]  %04x  ", i, pReallocTable[2*i] + pReallocTable[2*i+1] * 0x10, *p);
		*(WORD *)p += wBaseSegment;
		printf("%04x\n", *p);
#ifdef __DEBUG__
		getchar();
#endif
    }
// ��ת����ĳ��򱣴浽Ŀ���ļ���ȥ
    fileDesc = fopen(argv[2], "wb+");
    if(fileDesc==NULL)
	{
		printf("Error[3]: Can't open desc file!\n");
		outArgv(argv[0]);
		return 1;
    }
	fwrite(pBody, dwBodySize, 1, fileDesc);
	fclose(fileDesc);
	PRINT_SIZE("     Source file's size :", dwFileSize);
	PRINT_SIZE("Destination file's size :", dwBodySize);
	return 0;
}