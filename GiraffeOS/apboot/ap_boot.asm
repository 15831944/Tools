;
; PROJECT:         PAC
; FILE:            ap_boot.asm
; PURPOSE:         Boot code for application processors
; UPDATE HISTORY:  Created 2012-12-16
;
; The Boot Code Start With 0x00080000
; The Data On 0x00081000

; Segment selectors
;
%define KERNEL_CS	(0x08)			;index=1 TI=0 R=0		  1  0  00
%define KERNEL_DS	(0x10)			;index=2 TI=0 R=0		 10  0  00
%define MAIN_CS		(0x18)			;index=3 TI=0 R=0		 11  0  00
%define MAIN_DS		(0x20)			;index=4 TI=0 R=0		100  0  00


; 16 bit code
BITS 16								; �л�Ϊ16bitģʽ

BOOT_START:
	cli

	in		ax,	92h					; ����A20
	or		ax, 00000010b
	out		92h, ax

	xor		ax,	ax					; ���ax,ds,ss�Ĵ���
	mov		ds,	ax
	mov		ss,	ax

	mov		ax,	08000H				; ����Ҫ���ʵĶ�Ϊ0x00080000
	mov		ds,	ax

	mov		eax, 100H + BOOT_START
	lgdt	[eax]

	mov		eax, cr0				; �����������Ӣ������˼���ǿ���CPU�ı���ģʽ��
	or		eax, 1				
	mov		cr0, eax

	jmp dword KERNEL_CS:000080000H + BOOT_32MODE - BOOT_START


; 32 bit code
BITS 32									; �л���32λģʽ

BOOT_32MODE:
	mov		ax, MAIN_DS
	mov		ds, ax
	mov		fs, ax
	mov		gs, ax
	mov		ss, ax
	mov		esp, 00F000000h				; ���ó���ջָ��Ϊ0xDF000000

	mov		ax, KERNEL_DS
	mov		es, ax

	mov 	eax, 000080108H 			;��ǰGDT��ַ
	mov 	ebx, 007C08000h				;GDTĿ���ַ077C08000h
	mov		ecx, 000000028h				;����ѭ����40	
gdtloop:	  
es	mov 	dl, [eax]
ds	mov		[ebx], dl					;��eax��ǰ��ֵ��д��ebx��ָ��ĵ�ַ
	inc		ebx							;bxָ��ĵ�ַ++	
	inc 	eax
	loop	gdtloop						;ѭ��ִ��  

	mov		eax, 000080102H
	mov 	ebx, 000080180H
es	mov		ebx, [ebx]
	add 	ebx, 007C08000h
es	mov     [eax],ebx

	mov		eax, 000080100H
es	lgdt	[eax]

	mov		ax, MAIN_DS
	mov		es, ax

	jmp		MAIN_CS:0				; ��ת��������ں���0xC0000000
	hlt
