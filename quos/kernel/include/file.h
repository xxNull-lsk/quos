#ifndef FILE_H
#define FILE_H

	#include "include/defs.h"
	#include "fat12.h"

	#define FILE_ATTR_RDONLY	1	//ֻ���ļ�
	#define FILE_ATTR_ISDIR		2	//Ŀ¼�ļ�
	#define FILE_ATTR_SYSTEM	4	//ϵͳ�ļ����������û�ֱ�ӷ��ʡ�
	#define FILE_ATTR_HIDDEN	8	//�����ļ�

	#define FILE_NAME_SIZE 8
	#define FILE_EXT_SIZE 3

	#define FILE_READ 0x1
	#define FILE_WRITE 0x2

	// ƫ�Ʊ�־
	#define SEEK_CUR 0
	#define SEEK_SET 1
	#define SEEK_END 2

	typedef unsigned short FILE;

	// FILE�ṹ
	typedef struct File
	{
		char strName[FILE_NAME_SIZE];	// �ļ���
		char strExt[FILE_EXT_SIZE];		// ��չ��
		BYTE byMode;		// �ļ���д��ʽ
		BYTE byAttribute;	// �ļ�����
		DWORD dwSize;		// �ļ���С
		DWORD dwPos;		// ��дλ��
		BYTE byDevNO;		// �����豸��
		WORD wTime;				// ���һ���޸�ʱ��
		WORD wDate;				// ���һ���޸�����
		WORD wFirstCus;	// �����еĿ�ʼ�غ�
		struct FileOpts far * pOpts;	// �ļ�ϵͳ��������
		struct FAT12Parameter far * pFSPar;	// �ļ�ϵ������Ϣ
	}File;

	// �ļ���������
	typedef struct FileOpts{
		File far * (*Open) (char far * strFileName, BYTE byMode);
		void (*Close) (File far * pFile);
		DWORD (*Read) (BYTE far * pDest, DWORD dwSize, DWORD dwCount, File far * pFile);
		DWORD (*Seek) (File far * pFile, DWORD dwOffset, BYTE by0rigin);
		DWORD (*Write) (BYTE far * pSrc, DWORD dwSize, DWORD dwCount, File far * pFile);
		BOOL (*Rename) (char far * strName, File far * pFile);
		BOOL (*Delete) (File far * pFile);
		BOOL (*Attribute) (File far * pFile, BYTE byAttribute);
	}FileOpts;
	BYTE allocFileNo(File far * pFile);// �ӵ�ǰ�����з���һ���ļ���
	void freeFileNo(BYTE byFileNO);// �ͷŵ�ǰ���̵�һ���ļ���
	File far * getFileStruct(BYTE byFileNO);
	FILE fopen(char far * strFileName, char far * strMode);
	DWORD fread(BYTE far * pDest, DWORD dwSize, DWORD dwCount, FILE fileNo);
	DWORD fseek(FILE fileNo, DWORD dwOffset, BYTE by0rigin);
	void fclose(FILE fileNo);
	DWORD fwrite(BYTE far * pSrc, DWORD dwSize, DWORD dwCount, FILE fileNo);
	void fls(const char far * path);
	BOOL fdelete(FILE fileNo);
	BOOL frename(char far * strName, FILE fileNo);
	BOOL fattribute(BYTE byAttribute, FILE fileNo);
#endif //ifdef  FILE_H