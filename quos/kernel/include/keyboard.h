#ifndef KEYBOARD_H
#define KEYBOARD_H
	#include "defs.h"

	#define KEY_SCOLL_OPEN	0x10
	#define KEY_NUM_OPEN	0x20
	#define KEY_CAPS_OPEN	0x40
	#define KEY_INSERT_OPEN	0x80

	#define KEY_RSHIFT_OPEN	0x1
	#define KEY_LSHIFT_OPEN	0x2
	#define KEY_CTRL_OPEN	0x4
	#define KEY_ALT_OPEN	0x8

	#define KEY_ESC			01
	#define KEY_ENTER		28
	#define KEY_TAB			15
	#define KEY_HOME		71
	#define KEY_END			79

	BYTE keyBroadState(void);		//��ȡ����״̬
	BYTE getKeyCode(void);			//��ȡ��ǰ������ɨ����(������)
	BYTE getKeyChar(void);			//��ȡ��ǰ������ASCII��(������)
	WORD getKeyCodeChar(void);		//��ȡ��ǰ������ASCII���ɨ����(������)
	BYTE getKeyCharWait(void);		//��ȡ��ǰ������ɨ����(����)
	BYTE getKeyCodeWait(void);		//��ȡ��ǰ������ASCII��(����)
	WORD getKeyCodeCharWait(void);	//��ȡ��ǰ������ASCII���ɨ����(����)


#endif //ifdef  KEYBOARD_H