//�ļ�����time.c
//���ܣ�
//	��ȡCMOS�����ʱ����Ϣ���Ժ󻹿��������������ú���
// ���뷽����
//		cl /nologo /c /Gs /G3 /w /Gx- time.obj time.c
//  ��˹
//2007-6-3 

#include "include/stdc.h"
#include "include/time.h"

// ��cmos�д�ŵ���ֵ��bcd����Ҫת��һ�²���ʹ��
BYTE bcd_to_bin(BYTE val)
{
	if(val>10)
		return ((val&15) + (val>>4)*10);
	else
		return val;
}
BYTE getSecond(void)
{
	outb(0x70, 0x00);
	return bcd_to_bin(inb(0x71));
}
BYTE getMinute(void)
{
	outb(0x70, 0x02);
	return bcd_to_bin(inb(0x71));
}
BYTE getHour(void)
{
	outb(0x70, 0x04);
	return bcd_to_bin(inb(0x71));
}
BYTE getWeek(void)
{
	outb(0x70, 0x06);
	return bcd_to_bin(inb(0x71));
}
BYTE getDay(void)
{
	outb(0x70, 0x07);
	return bcd_to_bin(inb(0x71));
}
BYTE getMonth(void)
{
	outb(0x70, 0x08);
	return bcd_to_bin(inb(0x71));
}
WORD getYear(void)
{
	WORD wYear = getCentury()*100;
	outb(0x70, 0x09);
	wYear += bcd_to_bin(inb(0x71));
	return wYear;
}
BYTE getCentury(void)
{
	outb(0x70, 0x32);
	return bcd_to_bin(inb(0x71));
}
//��CMOS�л�ȡʱ����Ϣ
void cmosTime(void)
{
	char buf[10];
	BYTE time_ss = getSecond(), time_mm = getMinute(), time_hh = getHour();

	if(time_hh<10)puts("0");
	itoa(time_hh, (char far *)buf);
	puts(buf);
	puts(":");

	if(time_mm<10)puts("0");
	itoa(time_mm, (char far *)buf);
	puts(buf);
	puts(":");

	if(time_ss<10)puts("0");
	itoa(time_ss, (char far *)buf);
	puts(buf);
}
//��CMOS�л�ȡ������Ϣ
void cmosDate(void)
{
	char buf[10];
	BYTE date_week = getWeek(), date_day = getDay(), date_month = getMonth(), date_century = getCentury();
	WORD date_year = getYear();

	if(date_century<10)puts("0");
	itoa(date_year, (char far *)buf);
	puts(buf);
	puts(" ");

	if(date_month<10)puts("0");
	itoa(date_month, (char far *)buf);
	puts(buf);
	puts(" ");

	if(date_day<10)puts("0");
	itoa(date_day, (char far *)buf);
	puts(buf);
	puts(" ");

	switch (date_week)
	{
		case 0:puts("Sunday");break;
		case 1:puts("Monday");break;
		case 2:puts("Tuesday");break;
		case 3:puts("Wednesday");break;
		case 4:puts("Thursday");break;
		case 5:puts("Friday");break;
		case 6:puts("Saturday");break;
	}	
}