#include <stdio.h>
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
int main(int argc, char *argv[])
{
	FILE * exeFile = 0;
	struct exeHead * pHead = 0;
    if(argc<1)
	{
		printf("Error[0]: No enough argvs.\n");
		return 1;
    }	
    exeFile = fopen(argv[1], "rb");
    if(exeFile == NULL)
	{
		printf("Error[1]: Can't open file.\n");
		return 1;
    }
	pHead = (struct exeHead *)malloc(sizeof(struct exeHead));
	fread(pHead, sizeof(struct exeHead), 1, exeFile);
	fclose(exeFile);

// ��֤�����Ƿ���Ч
	if(pHead->wMagic != 0x5A4D)
	{
		printf("Error[2]: This file isn't a dos-EXE program!");
		return 1;
	}

	printf("                 ����:\t%#X\n", pHead->wMagic);
	printf("���һ����ʹ�õ��ֽ���:\t%d\n", pHead->wSizeInLastBlock);
	printf("      512�ֽڿ�ĸ���:\t%d\n", pHead->wBlockCount);
	printf("         �ض�λ�����:\t%d\n", pHead->wReallocCount);
	printf("�ļ�ͷ����С����16�ֽ�Ϊ��λ��:\t%d\n", pHead->wHeadSize);
	printf("�����������С�ڴ���:\t%d\n", pHead->wMinAlloc);
	printf("�������������ڴ���:\t%d\n", pHead->wMaxAlloc);
	printf("      wInitSS:wInitSP =\t%04xh:%04xh\n", pHead->wInitSS, pHead->wInitSP);
	printf(" �ļ�У���(��������):\t%04xh\n", pHead->wChecksum);
	printf("      wInitCS:wInitIP =\t%04xh:%04xh\n", pHead->wInitIP, pHead->wInitCS);
	printf("     �ض�λ������λ��:\t%d\n", pHead->wFirstRealloc);
	printf("  �ص��ĸ���(����Ϊ0):\t%d\n", pHead->wNoverlay);
	return 0;
}