#ifndef FAT12_H
#define FAT12_H

	#define ATTR_READ_ONLY 0x01
	#define ATTR_HIDDEN 0x02
	#define ATTR_SYSTEM 0x04
	#define ATTR_VOLUME_ID 0x08
	#define ATTR_DIRECTORY 0x10
	#define ATTR_ARCHIVE 0x20

	#define FAT_BUF_LEN fat12Par.wBytPerSec*2
	// FAT12 Ŀ¼��
	struct FAT12DirEntry
	{
		char strFileName[8];	// �ļ���
		char strFileExt[3];		// �ļ���չ��
		BYTE byFileAttrib;		// �ļ�����
		BYTE byReserved[10];	// ����
		WORD wTime;				// ���һ���޸�ʱ��
		WORD wDate;				// ���һ���޸�����
		WORD wStartClus;		// ��ʼ�غ�
		DWORD dwFileSize;		// �ļ���С�����ֽ�Ϊ��λ
	};

	//FAT12 �ļ�ϵͳ����
	struct FAT12Parameter{
		char strOEM[8];		//OEM��Ϣ	0x0003
		WORD wBytPerSec;	//ÿ�����ֽ���	0x000B
		BYTE bySecPerClu;	//ÿcluster��������	0x000D
		WORD wSecReserved;	//��������	0x000E
		BYTE byNumFats;		//FAT����Ŀ	0x0010
		WORD wRootUnits;	//��Ŀ¼����	0x0011
		WORD wSecSmall;		//��������������256*256��0x0013
		BYTE byMedia;		//ý�������� 0xF8��ʾӲ��	0x0015
		WORD wSecPerFat;	//ÿFAT������	0x0016
		WORD wSecPerTra;	//ÿ�ŵ�������	0x0018
		WORD wHeads;		//��ͷ��(������)	0x000A
		DWORD dwSecHidden;	//��������	0x001C
		DWORD dwSecBig;		//�������������ڵ���256*256��	0x0020
		BYTE byDrive;		//���������ţ� 00���� 0x80ΪӲ�� 0x0024
		BYTE byReserved;	//����
		BYTE byExBootSig;	//��չ������־λ
		DWORD dwVolume;		//���к�
		char strLabel[11];	//��ǩ
		char strFSType[8];	//�ļ�ϵͳ����
	};

	BOOL fat12_ReadSuper(BYTE byDriveNO);
	BOOL fat12_Init(BYTE byDriveNO);
	BOOL fat12_Path(char far * strPath,struct File far * pFile);
	struct File far * fat12_fOpen(char far * strFileName, BYTE byMode);
	void fat12_fClose(struct File far * pFile);
	DWORD fat12_fRead(BYTE far * pDest, DWORD dwSize, DWORD dwCount, struct File far * pFile);
	DWORD fat12_fSeek(struct File far * pFile, DWORD dwOffset, BYTE by0rigin);
	DWORD fat12_fWrite(BYTE far * pSrc, DWORD dwSize, DWORD dwCount, struct File far * pFile);
	BOOL fat12_fRename(char far * strName, struct File far * pFile);
	BOOL fat12_fDelect(struct File far * pFile);
	BOOL fat12_fAttribute(struct File far * pFile, BYTE attribute);
	BOOL fat12_ReWorkDir(struct File far * pFile);
	struct FAT12Parameter far * getFat12Par();
#endif //ifdef  FAT12_H