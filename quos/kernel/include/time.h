#ifndef TIME_H
#define TIME_H

	void cmosTime(void);//��ʾ��CMOS�л�ȡ��ʱ����Ϣ
	void cmosDate(void);//��ʾ��CMOS�л�ȡ��������Ϣ
	BYTE getSecond(void);//��CMOS�л�ȡ��ǰ��
	BYTE getMinute(void);//��CMOS�л�ȡ��ǰ����
	BYTE getHour(void);//��CMOS�л�ȡ��ǰСʱ
	BYTE getWeek(void);//��CMOS�л�ȡ��ǰ����ֵ
	BYTE getDay(void);//��CMOS�л�ȡ��ǰ��
	BYTE getMonth(void);//��CMOS�л�ȡ��ǰ�·�ֵ
	WORD getYear(void);//��CMOS�л�ȡ��ǰ���ֵ
	BYTE getCentury(void);//��CMOS�л�ȡ��ǰ����ֵ
#endif //ifdef  TIME_H