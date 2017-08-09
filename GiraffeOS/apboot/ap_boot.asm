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
BITS 16								; 切换为16bit模式

BOOT_START:
	cli

	in		ax,	92h					; 开启A20
	or		ax, 00000010b
	out		92h, ax

	xor		ax,	ax					; 清空ax,ds,ss寄存器
	mov		ds,	ax
	mov		ss,	ax

	mov		ax,	08000H				; 设置要访问的段为0x00080000
	mov		ds,	ax

	mov		eax, 100H + BOOT_START
	lgdt	[eax]

	mov		eax, cr0				; 根据下面这段英语，差不多意思就是开启CPU的保护模式，
	or		eax, 1				
	mov		cr0, eax

	jmp dword KERNEL_CS:000080000H + BOOT_32MODE - BOOT_START


; 32 bit code
BITS 32									; 切换到32位模式

BOOT_32MODE:
	mov		ax, MAIN_DS
	mov		ds, ax
	mov		fs, ax
	mov		gs, ax
	mov		ss, ax
	mov		esp, 00F000000h				; 设置程序栈指针为0xDF000000

	mov		ax, KERNEL_DS
	mov		es, ax

	mov 	eax, 000080108H 			;当前GDT地址
	mov 	ebx, 007C08000h				;GDT目标地址077C08000h
	mov		ecx, 000000028h				;设置循环次40	
gdtloop:	  
es	mov 	dl, [eax]
ds	mov		[ebx], dl					;将eax当前的值，写入ebx所指向的地址
	inc		ebx							;bx指向的地址++	
	inc 	eax
	loop	gdtloop						;循环执行  

	mov		eax, 000080102H
	mov 	ebx, 000080180H
es	mov		ebx, [ebx]
	add 	ebx, 007C08000h
es	mov     [eax],ebx

	mov		eax, 000080100H
es	lgdt	[eax]

	mov		ax, MAIN_DS
	mov		es, ax

	jmp		MAIN_CS:0				; 跳转到程序入口函数0xC0000000
	hlt
