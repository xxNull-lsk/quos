#ifndef COMMAND_H
#define COMMAND_H
	char far *getParameter(char far * strCommand, int iCommandLen, char far * strArg);//��ȡ�����в���
	void commandVer();//��ʾ�汾��Ϣ
	void commandHelp(const char far *strCommand);//��ʾ������Ϣ
	void commandType(char far * strCommand);//��ʾ�ı��ļ�����
	void commandDir();
	void commandMv(char far * strCommand);
	void commandDel(char far * strCommand);
#endif //ifdef  COMMAND_H