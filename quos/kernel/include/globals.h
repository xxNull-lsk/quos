#ifndef GLOBALS_H
#define GLOBALS_H
	#include "process.h"
	extern	BYTE far * g_pVideo;		// �Դ�λ��
	extern	BYTE g_byCursorRow;		// ��ǰ���λ�õ��к�
	extern	BYTE g_byCursorCol;		// ��ǰ���λ�õ��к�
	extern	BYTE g_byCurrColor;		// ��ǰ�ַ���ʾ��ɫ
	extern	BYTE g_byCurrProcessID;	// ��ǰ���еĽ��̺�
	extern	struct PCB g_Process[MAX_PROCESS];// ϵͳ�����н��̵���Ϣ

#endif //ifdef  GLOBALS_H