; �ļ�����setup.asm
; ���ܣ�
;	�����̸�Ŀ¼�ҵ�kernel.exe��������������ע�������kernel���ջ���dos exe��ʽ�ģ�������Ҫ������������С�
; ���뷽����
;		ML /c /Fo setup.obj setup.asm
;   ��˹
; 2007-5-31
				.MODEL  TINY
                .386
;----------------------------------------------------------------------------------------------------
;===============================================================================
;									һЩ����
;===============================================================================
Kernel_Seg			equ  07000h ; kernel.exe �����ص��ڴ������λ�õĶε�ַ
Kernel_Off			equ  00h    ; kernel.exe �����ص��ڴ������λ�õ�ƫ�Ƶ�ַ
Kernel_PhyAddr		equ Kernel_Seg * 10h   ; kernel.exe �����ص��ڴ������λ�õ������ַ

; ���� FAT12 ͷ��һЩ�������壬���ͷ��Ϣ�ı䣬����ĳ�������ҲҪ����Ӧ�ı�
FAT_Sz				equ 9   ; BPB_FATSz16
RootDir_Sectors_Count	equ 14  ; ��Ŀ¼ռ�ÿռ�: RootDir_Sectors_Count = ((BPB_RootEntCnt * 32) + (Byts_Per_Sector �C 1)) / Byts_Per_Sector; ��������մ˹�ʽ�������
RootDir_SectorNo	equ 19  ; Root Directory �ĵ�һ�������� = BPB_RsvdSecCnt + (BPB_NumFATs * FAT_Sz)
FAT1_SectorsNo		equ 1   ; FAT1 �ĵ�һ��������   = BPB_RsvdSecCnt
Delta_SectorNo		equ 17  ; Delta_SectorNo = BPB_RsvdSecCnt + (BPB_NumFATs * FAT_Sz) - 2
                            ; �ļ��Ŀ�ʼSector�� = DirEntry�еĿ�ʼSector�� + ��Ŀ¼ռ��Sector��Ŀ + Delta_SectorNo
Byts_Per_Sector		equ 512 ; ÿ�����ֽ���
Sector_Per_Track	equ 18  ; ÿ�ŵ�������
Drive_NO			equ 0   ; �ж� 13 ����������
;----------------------------------------------------------------------------------------------------

	.data
;----------------------------------------------------------------------------------------------------
	wRootDirSizeForLoop dw  RootDir_Sectors_Count  ; Root Directory ռ�õ�������, ��ѭ���л�ݼ�����.
	wSectorNo           dw  0               ; Ҫ��ȡ��������
	bOdd                db  0               ; ��������ż��
	strFileName            db  "KERNEL  EXE", 0; �ļ���
	strLoading	        db  "Loading kernel    "	; ��� 0�������ں��ļ���ʾ��Ϣ
	Loading_Len	        equ	$-offset strLoading		; ��� 0�ַ�������
	strOKHead			db  "        ["				; ��� 1�����������Ϣ
	OKHead_Len			equ	$-offset strOKHead		; ��� 1�ַ�������
	strOKTail			db  "]", 0dh, 0ah			; ��� 1�����������Ϣ
	OKTail_Len			equ	$-offset strOKTail		; ��� 1�ַ�������
	strNoKernel			db  0dh, 0ah, "Fatal[1]: No kernel.exe!"			; ��� 2��û���ҵ�
	NoKernel_Len		equ	$-offset strNoKernel				; ��� 2�ַ�������
	strMemTooSamll		db  0dh, 0ah, "Fatal[2]: The memory is too small!"			; ��� 2��û���ҵ�
	MemTooSamll_Len		equ	$-offset strMemTooSamll				; ��� 2�ַ�������
;----------------------------------------------------------------------------------------------------
	.code
				ORG     00h
				assume cs:@code ,ds:@data

Public $begin
$begin:
;----------------------------------------------------------------------------------------------------
main:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, ax
; �����Ƿ����㹻���ڴ�
	int 12h
	cmp ax, 254h ; �ں������128kb���ֻᱻ���ص�70000h���������ڴ�������95000h(ϵͳ��ջ��������Ҫ5000h)���У�95000h=0x254kb
	ja CanLoadKernel

	mov cx, MemTooSamll_Len
	mov ax, offset strMemTooSamll
	call DispStr
HALT1:
    hlt
    jmp HALT1  ; ����
 
CanLoadKernel:
	mov cx, Loading_Len
    mov ax, offset strLoading
    call DispStr; ��ʾ�ַ���"Loading kernel.exe"

	call LoadKernel

	; ����kernel.exe�ɹ�
	mov cx, OKHead_Len
    mov ax, offset strOKHead
    call DispStr

	mov	al, 'O'
	mov	ah, 09h
	mov bh, 0
	mov	bl, 0ah
	mov	cx, 01
	int	 10h

;//��ȡ���λ��
	mov		ah, 03
	mov		bh, 00
	int		10h
;//�������ù��λ��
	inc		dl
	mov		ah, 02
	mov		bh, 00
	int		10h

	mov	al, 'K'
	mov	ah, 09h
	mov bh, 0
	mov	bl, 0Ah
	mov	cx, 01
	int	 10h

;//��ȡ���λ��
	mov		ah, 03
	mov		bh, 00
	int		10h
;//�������ù��λ��
	inc		dl
	mov		ah, 02
	mov		bh, 00
	int		10h

	mov cx, OKTail_Len
    mov ax, offset strOKTail
    call DispStr
	; ��ת���ں�ȥ
	push Kernel_Seg
	push Kernel_Off
	retf


;----------------------------------------------------------------------------------------------------
;===============================================================================
; ��������LoadKernel
; ��������
; ���أ���
; ���ã�
; 	�ҵ�kernel.exe��������
;===============================================================================
LoadKernel proc near
    mov wSectorNo, RootDir_SectorNo
SEARCH_IN_ROOT_DIR_BEGIN:
    cmp wRootDirSizeForLoop, 0   ; ��
    jz  NO_KERNEL                ; �� �жϸ�Ŀ¼���ǲ����Ѿ�����
    dec wRootDirSizeForLoop      ; �� ��������ʾû���ҵ� setup.bin
    mov ax, Kernel_Seg
    mov es, ax
    mov bx, Kernel_Off   ; ����Ŀ¼��FAT�����Kernel_Seg:Kernel_Off��
    mov ax, wSectorNo     ; ��Ŀ¼�е�ĳ Sector ��-->ax
    mov cl, 1
    call    ReadSector

    mov si, offset strFileName   ; ��ds:siָ��FileName
    mov di, Kernel_Off
    cld
    mov dx, 10h
SEARCH_KERNEL_FILE:
    cmp dx, 0                               ; ��ѭ����������,
    jz  GOTO_NEXT_SECTOR_IN_ROOT_DIR        ; ������Ѿ�������һ�� Sector,
    dec dx                                  ; ����������һ�� Sector
    mov cx, 11 ; ������Ҫ�Ƚ�11���ַ�����ΪFAT12�ļ�ϵͳ�е��ļ�����һ����8.3���涨��Ҳ����8�ַ����ļ�����3���ַ�����չ��
CMP_FILENAME:
    cmp cx, 0
    jz  FILENAME_FOUND    ; ����Ƚ���11���ַ�ȫ�����, ��ʾ�ҵ��ˡ�
    dec cx
    lodsb
    cmp al, es:[di]
    jz  CMP_NEXT_CHAR   ; ƥ����һ���ַ�
    jmp DIFFERENT       ; ֻҪ����һ����ƥ����ַ��ͱ����� DirectoryEntry ��������Ҫ�ҵ�
CMP_NEXT_CHAR:
    inc di
    jmp CMP_FILENAME    ; ����ѭ�����Ƚ���һ���ַ�

DIFFERENT:
    and di, 0FFE0h                  ; else ��    di &= E0 Ϊ������ָ����Ŀ��ͷ
    add di, 20h                     ;      ��
    mov si, offset strFileName           ;      �� di += 20h  ��һ��Ŀ¼��Ŀ
    jmp SEARCH_KERNEL_FILE         ;      ��

GOTO_NEXT_SECTOR_IN_ROOT_DIR:
    inc wSectorNo
    jmp SEARCH_IN_ROOT_DIR_BEGIN

NO_KERNEL:
	mov ax, offset strNoKernel
	mov cx, NoKernel_Len
    call    DispStr
HALT:
    hlt
    jmp HALT            ; û���ҵ��ļ�����ѭ��������

FILENAME_FOUND:           ; �ҵ��ļ���������������
    mov ax, RootDir_Sectors_Count
    and di, 0FFE0h      ; di -> ��ǰ��Ŀ�Ŀ�ʼ
    add di, 01Ah        ; di -> �� Sector
    mov cx, es:[di]
    push cx             ; ����� Sector �� FAT �е����
    add cx, ax
    add cx, Delta_SectorNo   ; ������ʱ cl ������ kernel.exe ����ʼ������ (�� 0 ��ʼ�������)
    mov ax, Kernel_Seg
    mov es, ax              ; es <- Kernel_Seg
    mov bx, Kernel_Off   ; bx <- Kernel_Off   ����, es:bx = Kernel_Seg:Kernel_Off
    mov ax, cx              ; ax <- Sector ��

GO_ON_LOADING_FILE:

    mov cl, 1
    call ReadSector
    pop ax              ; ȡ���� Sector �� FAT �е����
    call GetFATEntry
    cmp ax, 0FFFh       ; �Ƿ��Ѿ��������
    jz  FILE_LOADED
    push ax             ; ���� Sector �� FAT �е����
    mov dx, RootDir_Sectors_Count
    add ax, dx
    add ax, Delta_SectorNo
    add bx, Byts_Per_Sector
    jmp GO_ON_LOADING_FILE
FILE_LOADED:
	ret
LoadKernel		endp
;----------------------------------------------------------------------------------------------------
;===============================================================================
; ��������DispStr
; ������
;	cx �ַ����ĳ���
;	ax �ַ���ƫ�Ƶ�ַ
; ���أ���
; ���ã�
; 	��ʾһ���ַ���
;===============================================================================
DispStr		proc	near
	push es
	push bp
	; ��ȡ�кź��к�
	push cx
	push ax
	mov ah, 03h
	mov bh, 00h
	int 10h
	pop ax
	pop cx
	; ����ַ���
    mov bp, ax          ; ��
    mov ax, ds          ; �� ES:BP = ����ַ
    mov es, ax          ; ��
    mov ax, 1301h
    mov bx, 0007h       ; BH = ҳ�ţ�BL = ��ʾ����---�ڵװ���(07h)
    int 10h
	pop bp
	pop es
	ret
DispStr		endp

;----------------------------------------------------------------------------
; ������: ReadSector
;----------------------------------------------------------------------------
; ����:
;   �ӵ� ax �� Sector ��ʼ, �� cl �� Sector ���� es:bx ��
ReadSector		proc	near
    ; -----------------------------------------------------------------------
    ; �������������������ڴ����е�λ�� (������ -> �����, ��ʼ����, ��ͷ��)
    ; -----------------------------------------------------------------------
    ; ��������Ϊ x
    ;                           �� ����� = y >> 1
    ;       x           �� �� y ��
    ; -------------- => ��      �� ��ͷ�� = y & 1
    ;  ÿ�ŵ�������     ��
    ;                   �� �� z => ��ʼ������ = z + 1
    push bp
    mov bp, sp
    sub esp, 2          ; �ٳ������ֽڵĶ�ջ���򱣴�Ҫ����������: byte [bp-2]

    mov [bp-2], cl
    push bx
    mov bl, Sector_Per_Track ; bl: ����
    div bl          ; y �� al ��, z �� ah ��
    inc ah          ; z ++
    mov cl, ah          ; cl = ��ʼ������
    mov dh, al          ; dh = y
    shr al, 1           ; y >> 1 (��ʵ�� y/BPB_NumHeads, ����BPB_NumHeads=2)
    mov ch, al          ; ch = �����
    and dh, 1           ; dh & 1 = ��ͷ��
    pop bx
    ; ����, "�����, ��ʼ����, ��ͷ��" ȫ���õ���
    mov dl, Drive_NO     ; �������� (0 ��ʾ A ��)
GoOnReading:
    mov ah, 2
    mov al, [bp-2]     ; ��al������
    int 13h
    jc  GoOnReading        ; �����ȡ���� CF �ᱻ��Ϊ 1, ��ʱ�Ͳ�ͣ�ض�, ֱ����ȷΪֹ

    add esp, 2
    pop bp

    ret
ReadSector		endp
;----------------------------------------------------------------------------
; ������: GetFATEntry
;----------------------------------------------------------------------------
; ����:
;   �ҵ����Ϊ ax �� Sector �� FAT �е���Ŀ, ������� ax ��
;   ��Ҫע�����, �м���Ҫ�� FAT �������� es:bx ��, ���Ժ���һ��ʼ������ es �� bx
GetFATEntry		proc	near
    push es
    push bx
    push ax
    mov ax, Kernel_Seg ; ��
    sub ax, 0100h       ; �� �� Kernel_Seg �������� 4K �ռ����ڴ�� FAT
    mov es, ax          ; ��
    pop ax
    mov bOdd, 0
    mov bx, 3
    mul bx          ; dx:ax = ax * 3
    mov bx, 2
    div bx          ; dx:ax / 2  ==>  ax <- ��, dx <- ����
    cmp dx, 0
    jz  OOOO
    mov bOdd, 1
OOOO:
;ż��
    xor dx, dx          ; ���� ax ���� FATEntry �� FAT �е�ƫ����. ���������� FATEntry ���ĸ�������(FATռ�ò�ֹһ������)
    mov bx, Byts_Per_Sector
    div bx          ; dx:ax / Byts_Per_Sector  ==>   ax <- ��   (FATEntry ���ڵ���������� FAT ��˵��������)
                    ;               dx <- ���� (FATEntry �������ڵ�ƫ��)��
    push dx
    mov bx, 0           ; bx <- 0   ����, es:bx = (BaseOfSetup - 100):00 = (BaseOfSetup - 100) * 10h
    add ax, FAT1_SectorsNo  ; �˾�ִ��֮��� ax ���� FATEntry ���ڵ�������
    mov cl, 2
    call    ReadSector      ; ��ȡ FATEntry ���ڵ�����, һ�ζ�����, �����ڱ߽緢������, ��Ϊһ�� FATEntry ���ܿ�Խ��������
    pop dx
    add bx, dx
    mov ax, es:[bx]
    cmp bOdd, 1
    jnz EVEN_2
    shr ax, 4
EVEN_2:
    and ax, 0FFFh

GET_FAT_ENRY_OK:

    pop bx
    pop es
    ret
GetFATEntry		endp
;----------------------------------------------------------------------------------------------------
end	main