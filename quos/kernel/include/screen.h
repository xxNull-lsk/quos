#ifndef SCREEN_H
#define SCREEN_H
	#define MAX_COL 79
	#define MAX_ROW 24
	void screenInit(BYTE byModeNO);//�ı�ģʽ��ʼ��
	void screenShowChar(char ch);
	void update_cursor();
	void screenUp(BYTE byCount);//�Ͼ���
	void screenClear(void);//�ı�ģʽ����
	void setScreenColor(BYTE byColor);
	BYTE getScreenColor();
	void getComebackScreenColor();
#endif //ifdef  SCREEN_H