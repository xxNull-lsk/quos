#ifndef ERRORNO_H
#define ERRORNO_H

	#define OK 1

	// �����ڴ�ʧ�ܣ��ڴ治��
	#define NO_MEMORY -1
	// û��Ȩ��ִ�иò���
	#define NO_RIGHT -2
	// ��Ч����
	#define INVALIDATION -3
	// ����������֧��int13��չ��д
	#define CAN_NOT_USER_LBA -4
	// ���ļ�ʧ��
	#define OPEN_FILE_FAILE -5
	// ��ȡ�ļ�ʧ��
	#define READ_FILE_FAILE -6
	// SEEK����
	#define SEEK_FILE_FAILE -7
	

	// ϵͳ��ʹ�õ�BUG���
	#define BUG_file_freeFileNo_1 1
	#define BUG_file_getFileStruct_1 2
	#define BUG_fat12_fSeek_1 3
	#define BUG_freeProcessNO_1 4


	// ϵͳ��ʹ�õ�����������
	#define PANIC_file_freeFileNo_1 1
	#define PANIC_freeProcessNO_1 2

#endif // ERRORNO_H
