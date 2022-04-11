; �ļ�����boot.asm
; ���ܣ�
;	�����̸�Ŀ¼�ҵ�setp.bin�ļ������ز�������
; ���룺nasmw boot.asm -o boot.bin
; �ο����Լ�����д����ϵͳ�����
;   ��˹ 
; 2007-5-28

                    org  07c00h ; Bios���ڿ�����ʱ���booter���ص�0x7C00����������
 
;================================================================================================
; һЩ����
; ----------------------------------------------------------------------
BaseOfStack         equ 07c00h  ; ��ջ����ַ�������ַ��ջ��,��ջ�����λ����͵�ַ��������Ϊ���ǵ�booter��0x7c00�ĸߵ�ַ��512���ֽڣ����Բ���������

;segOfSetup         equ  09000h ; setup.bin �����ص��ڴ������λ�õĶε�ַ
OffsetOfSetup       equ  00h    ; setup.bin �����ص��ڴ������λ�õ�ƫ�Ƶ�ַ

; -------------------------------------------------------------------------
; ���� FAT12 ͷ��һЩ�������壬���ͷ��Ϣ�ı䣬����ĳ�������ҲҪ����Ӧ�ı�
; -------------------------------------------------------------------------
FATSz               equ 9   ; BPB_FATSz16
RootDirSectors      equ 14  ; ��Ŀ¼ռ�ÿռ�: RootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytsPerSec �C 1)) / BPB_BytsPerSec; ��������մ˹�ʽ�������
SectorNoOfRootDirectory equ 19  ; Root Directory �ĵ�һ�������� = BPB_RsvdSecCnt + (BPB_NumFATs * FATSz)
SectorNoOfFAT1      equ 1   ; FAT1 �ĵ�һ��������   = BPB_RsvdSecCnt
DeltaSectorNo       equ 17  ; DeltaSectorNo = BPB_RsvdSecCnt + (BPB_NumFATs * FATSz) - 2
                            ; �ļ��Ŀ�ʼSector�� = DirEntry�еĿ�ʼSector�� + ��Ŀ¼ռ��Sector��Ŀ + DeltaSectorNo
;================================================================================================
    jmp short START
    nop             ; Ϊ������fat�ļ�ϵͳ��ƫ��Ҫ�����nop�ز�����
 
; FAT12�ļ�ϵͳ��ʶͷ
; ----------------------------------------------------------------------
    BS_OEMName      DB 'Allan   '   ; OEM String, ���� 8 ���ֽ�

    BPB_BytsPerSec  DW 512      ; ÿ�����ֽ���
    BPB_SecPerClus  DB 1        ; ÿ�ض�������
    BPB_RsvdSecCnt  DW 1        ; Boot ��¼ռ�ö�������
    BPB_NumFATs     DB 2        ; ���ж��� FAT ��
    BPB_RootEntCnt  DW 224      ; ��Ŀ¼�ļ������ֵ
    BPB_TotSec16    DW 2880     ; �߼���������
    BPB_Media       DB 0xF0     ; ý��������
    BPB_FATSz16     DW 9        ; ÿFAT������
    BPB_SecPerTrk   DW 18       ; ÿ�ŵ�������
    BPB_NumHeads    DW 2        ; ��ͷ��(����)
    BPB_HiddSec     DD 0        ; ����������
    BPB_TotSec32    DD 0        ; �������������ڵ���256*256��

    BS_DrvNum       DB 0        ; �ж� 13 ����������
    BS_Reserved1    DB 0        ; δʹ��
    BS_BootSig      DB 29h      ; ��չ������� (29h)
    BS_VolID        DD 0        ; �����к�
    BS_VolLab       DB 'QUOS1.00   '; ���, ���� 11 ���ֽ�
    BS_FileSysType  DB 'FAT12   '   ; �ļ�ϵͳ����, ���� 8���ֽ�  
;------------------------------------------------------------------------
sizeOfSetup         DB  01h
segOfSetup			DW  00
START:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, BaseOfStack
	; ����setup���������ص�λ��
	int 12h
	inc ax
	sub ax, [sizeOfSetup]
	sub ax, 30h ; Ϊ�˼��ظ�Ŀ¼������Ҫ��Щ�ڴ�
	mov bx, 40h
	mul bx
	mov word [segOfSetup], ax
    ; ��ʼ����ĻΪ��80 x 25  16ɫ�ı�
    mov ax, 03h
    int 10h

    mov dh, 0
    call DispStr; ��ʾ�ַ���"Booting  "
    
    xor ah, ah  ; ��
    xor dl, dl  ; �� ������λ
    int 13h     ; ��
    
; �ڸ�Ŀ¼Ѱ�� setup.bin���ҵ������������ת��ȥ���У�����Ҳ����ͻ������ʾ��ϢȻ��ִ��һ����ѭ��
    mov word [wSectorNo], SectorNoOfRootDirectory
SEARCH_IN_ROOT_DIR_BEGIN:
    cmp word [wRootDirSizeForLoop], 0   ; ��
    jz  NO_setupBIN                     ; �� �жϸ�Ŀ¼���ǲ����Ѿ�����
    dec word [wRootDirSizeForLoop]      ; �� ��������ʾû���ҵ� setup.bin
    mov ax, [segOfSetup]
    mov es, ax
    mov bx, OffsetOfSetup   ; es:bx = segOfSetup:OffsetOfSetup
    mov ax, [wSectorNo]     ; ��Ŀ¼�е�ĳ Sector ��-->ax
    mov cl, 1
    call    ReadSector

    mov si, SetupFileName   ; ��ds:siָ��SetupFileName
    mov di, OffsetOfSetup   ; ��es:diָ��segOfSetup:0100 = segOfSetup*10h+100
    cld
    mov dx, 10h
SEARCH_FOR_setupBIN:
    cmp dx, 0                               ; ��ѭ����������,
    jz  GOTO_NEXT_SECTOR_IN_ROOT_DIR        ; ������Ѿ�������һ�� Sector,
    dec dx                                  ; ����������һ�� Sector
    mov cx, 11 ; ������Ҫ�Ƚ�11���ַ�����ΪFAT12�ļ�ϵͳ�е��ļ�����һ����8.3���涨��Ҳ����8�ַ����ļ�����3���ַ�����չ��
CMP_FILENAME:
    cmp cx, 0
    jz  FILENAME_FOUND    ; ����Ƚ���11���ַ�ȫ�����, ��ʾ�ҵ��ˡ�
    dec cx
    lodsb
    cmp al, byte [es:di]
    jz  CMP_NEXT_CHAR   ; ƥ����һ���ַ�
    jmp DIFFERENT       ; ֻҪ����һ����ƥ����ַ��ͱ����� DirectoryEntry ��������Ҫ�ҵ�
CMP_NEXT_CHAR:
    inc di
    jmp CMP_FILENAME    ; ����ѭ�����Ƚ���һ���ַ�

DIFFERENT:
    and di, 0FFE0h                  ; else ��    di &= E0 Ϊ������ָ����Ŀ��ͷ
    add di, 20h                     ;      ��
    mov si, SetupFileName           ;      �� di += 20h  ��һ��Ŀ¼��Ŀ
    jmp SEARCH_FOR_setupBIN         ;      ��

GOTO_NEXT_SECTOR_IN_ROOT_DIR:
    add word [wSectorNo], 1
    jmp SEARCH_IN_ROOT_DIR_BEGIN

NO_setupBIN:
    mov dh, 2
    call    DispStr     ; ��ʾ�ַ���"No SETUP."
HALT:
    hlt
    jmp HALT            ; û���ҵ�setup.bin����ѭ��������

FILENAME_FOUND:           ; �ҵ�setup.bin��������������
    mov ax, RootDirSectors
    and di, 0FFE0h      ; di -> ��ǰ��Ŀ�Ŀ�ʼ
    add di, 01Ah        ; di -> �� Sector
    mov cx, word [es:di]
    push cx             ; ����� Sector �� FAT �е����
    add cx, ax
    add cx, DeltaSectorNo   ; ������ʱ cl ������ setup.bin ����ʼ������ (�� 0 ��ʼ�������)
    mov ax, [segOfSetup]
    mov es, ax              ; es <- segOfSetup
    mov bx, OffsetOfSetup   ; bx <- OffsetOfSetup   ����, es:bx = segOfSetup:OffsetOfSetup = segOfSetup * 10h + OffsetOfSetup
    mov ax, cx              ; ax <- Sector ��

GO_ON_LOADING_FILE:
    ;push    ax          ; ÿ�ζ�һ������ǰ���һ����.��
    ;push    bx 
    ;mov ah, 0Eh
    ;mov al, '.'
    ;mov bl, 0Fh
    ;int 10h
    ;pop bx
    ;pop ax

    mov cl, 1
    call ReadSector
    pop ax              ; ȡ���� Sector �� FAT �е����
    call GetFATEntry
    cmp ax, 0FFFh       ; �Ƿ��Ѿ��������
    jz  FILE_LOADED
    push ax             ; ���� Sector �� FAT �е����
    mov dx, RootDirSectors
    add ax, dx
    add ax, DeltaSectorNo
    add bx, [BPB_BytsPerSec]
    jmp GO_ON_LOADING_FILE

FILE_LOADED:
    mov dh, 1
    call DispStr     ; ��ʾ�ַ���"Ready."

; ����������ɣ���ʼ��ת��setup.binִ��
	; 
    ;jmp es:OffsetOfSetup
mov es,[segOfSetup]
	push es
	push 0
	retf
;============================================================================
; ����
;----------------------------------------------------------------------------
wRootDirSizeForLoop dw  RootDirSectors  ; Root Directory ռ�õ�������, ��ѭ���л�ݼ�����.
wSectorNo           dw  0               ; Ҫ��ȡ��������
bOdd                db  0               ; ��������ż��
SetupFileName       db  "SETUP   BIN", 0; setup.bin ֮�ļ���
MessageLength       equ 9               ; ÿ����Ϣ9�ֽ�, �������ÿո���
BootMessage:        db  "Booting  "     ; ��� 0��������ʾ��Ϣ
Message1            db  "Ready. ", 0dh , 0ah; ��� 1������setup.bin�����Ϣ
Message2            db  "No SETUP."         ; ��� 2��û���ҵ�setup.bin��ʾ��Ϣ
;============================================================================


;----------------------------------------------------------------------------
; ������: DispStr
;----------------------------------------------------------------------------
; ����:
;   ��ʾһ���ַ���, ������ʼʱ dh ��Ӧ�����ַ������(0-based)
DispStr:
    mov ax, MessageLength
    mul dh
    add ax, BootMessage
    mov bp, ax          ; ��
    mov ax, ds          ; �� ES:BP = ����ַ
    mov es, ax          ; ��
    mov cx, MessageLength; CX = �ַ����ĳ���
    mov ax, 01301h
    mov bx, 0007h       ; BH = ҳ�ţ�BL = ��ʾ����---�ڵװ���(07h)
    mov dl, 0
    int 10h
    ret


;----------------------------------------------------------------------------
; ������: ReadSector
;----------------------------------------------------------------------------
; ����:
;   �ӵ� ax �� Sector ��ʼ, �� cl �� Sector ���� es:bx ��
ReadSector:
    ; -----------------------------------------------------------------------
    ; �������������������ڴ����е�λ�� (������ -> �����, ��ʼ����, ��ͷ��)
    ; -----------------------------------------------------------------------
    ; ��������Ϊ x
    ;                           �� ����� = y >> 1
    ;       x           �� �� y ��
    ; -------------- => ��      �� ��ͷ�� = y & 1
    ;  ÿ�ŵ�������     ��
    ;                   �� �� z => ��ʼ������ = z + 1
    push    bp
    mov bp, sp
    sub esp, 2          ; �ٳ������ֽڵĶ�ջ���򱣴�Ҫ����������: byte [bp-2]

    mov byte [bp-2], cl
    push bx
    mov bl, [BPB_SecPerTrk] ; bl: ����
    div bl          ; y �� al ��, z �� ah ��
    inc ah          ; z ++
    mov cl, ah          ; cl = ��ʼ������
    mov dh, al          ; dh = y
    shr al, 1           ; y >> 1 (��ʵ�� y/BPB_NumHeads, ����BPB_NumHeads=2)
    mov ch, al          ; ch = �����
    and dh, 1           ; dh & 1 = ��ͷ��
    pop bx
    ; ����, "�����, ��ʼ����, ��ͷ��" ȫ���õ���
    mov dl, [BS_DrvNum]     ; �������� (0 ��ʾ A ��)
.GoOnReading:
    mov ah, 2
    mov al, byte [bp-2]     ; ��al������
    int 13h
    jc  .GoOnReading        ; �����ȡ���� CF �ᱻ��Ϊ 1, ��ʱ�Ͳ�ͣ�ض�, ֱ����ȷΪֹ

    add esp, 2
    pop bp

    ret

;----------------------------------------------------------------------------
; ������: GetFATEntry
;----------------------------------------------------------------------------
; ����:
;   �ҵ����Ϊ ax �� Sector �� FAT �е���Ŀ, ������� ax ��
;   ��Ҫע�����, �м���Ҫ�� FAT �������� es:bx ��, ���Ժ���һ��ʼ������ es �� bx
GetFATEntry:
    push    es
    push    bx
    push    ax
    mov ax, [segOfSetup] ; ��
    sub ax, 0100h       ; �� �� segOfSetup �������� 4K �ռ����ڴ�� FAT
    mov es, ax          ; ��
    pop ax
    mov byte [bOdd], 0
    mov bx, 3
    mul bx          ; dx:ax = ax * 3
    mov bx, 2
    div bx          ; dx:ax / 2  ==>  ax <- ��, dx <- ����
    cmp dx, 0
    jz  EVEN
    mov byte [bOdd], 1
EVEN:;ż��
    xor dx, dx          ; ���� ax ���� FATEntry �� FAT �е�ƫ����. ���������� FATEntry ���ĸ�������(FATռ�ò�ֹһ������)
    mov bx, [BPB_BytsPerSec]
    div bx          ; dx:ax / BPB_BytsPerSec  ==>   ax <- ��   (FATEntry ���ڵ���������� FAT ��˵��������)
                    ;               dx <- ���� (FATEntry �������ڵ�ƫ��)��
    push    dx
    mov bx, 0           ; bx <- 0   ����, es:bx = (segOfSetup - 100):00 = (segOfSetup - 100) * 10h
    add ax, SectorNoOfFAT1  ; �˾�ִ��֮��� ax ���� FATEntry ���ڵ�������
    mov cl, 2
    call    ReadSector      ; ��ȡ FATEntry ���ڵ�����, һ�ζ�����, �����ڱ߽緢������, ��Ϊһ�� FATEntry ���ܿ�Խ��������
    pop dx
    add bx, dx
    mov ax, [es:bx]
    cmp byte [bOdd], 1
    jnz EVEN_2
    shr ax, 4
EVEN_2:
    and ax, 0FFFh

GET_FAT_ENRY_OK:

    pop bx
    pop es
    ret
;----------------------------------------------------------------------------

times   510-($-$$)  db  0   ; ���ʣ�µĿռ䣬ʹ���ɵĶ����ƴ���ǡ��Ϊ512�ֽ�
dw  0xaa55              ; ������־
