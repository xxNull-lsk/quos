#ifndef DISK_H
#define DISK_H
	typedef struct
	{
		BYTE  bySize;			// ���ݰ��ߴ�(16�ֽ�)	
		BYTE  byReserved;		// ==0
		WORD  wBlockNum;		// Ҫ��������ݿ����(������Ϊ��λ)
		DWORD dwBufferAddr;		// ���仺���ַ(segment:offset)
		DWORD dwBeginBlockLo;	// ������ʼ���Կ��ַ
		DWORD dwBeginBlockHi;	
	} DISK_ADDR_PACKET;

WORD diskReadSector(DWORD dwSector, WORD wCount, BYTE bDriveNO, BYTE far *pBuffer);//��Ӳ������
WORD diskWriteSector(DWORD dwSector, WORD wCount, BYTE bDriveNO, BYTE far *pBuffer);//дӲ������
WORD Int13Disk(BYTE bKey, DWORD dwLBA, WORD wCount, BYTE bDriveNO, void far *pBuf);//����BIOS�ж϶�дӲ������
#endif //ifdef  DISK_H