                .MODEL  SMALL
                .386
_Shell_main			PROTO	NEAR
			Stack_Seg		equ 06000h
			Stack_Off		equ 0ffffh
	.data
	.CODE
				ORG     00h
				assume cs:@code ,ds:@data
   
Public $begin
$begin:

start:	   
    jmp     main
main:
	;��ʼ������
;	mov ax, Stack_Seg
;	mov ss, ax
;	mov sp, Stack_Off
	mov ax, @data
	mov es, ax
	mov ds, ax
	INVOKE _Shell_main
out_e:
;	mov al, 'e'; // ���һ����ĸ
;	mov ah, 09h
;	int 10h
    mov ah, 0Eh
    mov al, 'h'
    mov bl, 0Fh
    int 10h
    mov ah, 0Eh
    mov al, 'e'
    mov bl, 0Fh
    int 10h
    mov ah, 0Eh
    mov al, 'l'
    mov bl, 0Fh
    int 10h
    mov ah, 0Eh
    mov al, 'l'
    mov bl, 0Fh
    int 10h
    mov ah, 0Eh
    mov al, 'o'
    mov bl, 0Fh
    int 10h
    mov ah, 0Eh
    mov al, ' '
    mov bl, 0Fh
    int 10h
    mov ah, 0Eh
    mov al, ' '
    mov bl, 0Fh
    int 10h
	; ͣ��
	hlt
	nop
	; ����ϵͳ
    mov ax, 4c00h
    int 22h
	jmp out_e
    end	start