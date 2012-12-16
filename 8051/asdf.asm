;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.1.0 #7066 (Feb 26 2012) (Linux)
; This file was generated Sat Dec  1 16:13:19 2012
;--------------------------------------------------------
	.module asdf
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _seg_position
	.globl _display_seg
	.globl _main
	.globl _asd
	.globl _display
	.globl _delay
	.globl _CY
	.globl _AC
	.globl _F0
	.globl _RS1
	.globl _RS0
	.globl _OV
	.globl _F1
	.globl _P
	.globl _PS
	.globl _PT1
	.globl _PX1
	.globl _PT0
	.globl _PX0
	.globl _RD
	.globl _WR
	.globl _T1
	.globl _T0
	.globl _INT1
	.globl _INT0
	.globl _TXD
	.globl _RXD
	.globl _P3_7
	.globl _P3_6
	.globl _P3_5
	.globl _P3_4
	.globl _P3_3
	.globl _P3_2
	.globl _P3_1
	.globl _P3_0
	.globl _EA
	.globl _ES
	.globl _ET1
	.globl _EX1
	.globl _ET0
	.globl _EX0
	.globl _P2_7
	.globl _P2_6
	.globl _P2_5
	.globl _P2_4
	.globl _P2_3
	.globl _P2_2
	.globl _P2_1
	.globl _P2_0
	.globl _SM0
	.globl _SM1
	.globl _SM2
	.globl _REN
	.globl _TB8
	.globl _RB8
	.globl _TI
	.globl _RI
	.globl _P1_7
	.globl _P1_6
	.globl _P1_5
	.globl _P1_4
	.globl _P1_3
	.globl _P1_2
	.globl _P1_1
	.globl _P1_0
	.globl _TF1
	.globl _TR1
	.globl _TF0
	.globl _TR0
	.globl _IE1
	.globl _IT1
	.globl _IE0
	.globl _IT0
	.globl _P0_7
	.globl _P0_6
	.globl _P0_5
	.globl _P0_4
	.globl _P0_3
	.globl _P0_2
	.globl _P0_1
	.globl _P0_0
	.globl _B
	.globl _ACC
	.globl _PSW
	.globl _IP
	.globl _P3
	.globl _IE
	.globl _P2
	.globl _SBUF
	.globl _SCON
	.globl _P1
	.globl _TH1
	.globl _TH0
	.globl _TL1
	.globl _TL0
	.globl _TMOD
	.globl _TCON
	.globl _PCON
	.globl _DPH
	.globl _DPL
	.globl _SP
	.globl _P0
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0	=	0x0080
_SP	=	0x0081
_DPL	=	0x0082
_DPH	=	0x0083
_PCON	=	0x0087
_TCON	=	0x0088
_TMOD	=	0x0089
_TL0	=	0x008a
_TL1	=	0x008b
_TH0	=	0x008c
_TH1	=	0x008d
_P1	=	0x0090
_SCON	=	0x0098
_SBUF	=	0x0099
_P2	=	0x00a0
_IE	=	0x00a8
_P3	=	0x00b0
_IP	=	0x00b8
_PSW	=	0x00d0
_ACC	=	0x00e0
_B	=	0x00f0
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0_0	=	0x0080
_P0_1	=	0x0081
_P0_2	=	0x0082
_P0_3	=	0x0083
_P0_4	=	0x0084
_P0_5	=	0x0085
_P0_6	=	0x0086
_P0_7	=	0x0087
_IT0	=	0x0088
_IE0	=	0x0089
_IT1	=	0x008a
_IE1	=	0x008b
_TR0	=	0x008c
_TF0	=	0x008d
_TR1	=	0x008e
_TF1	=	0x008f
_P1_0	=	0x0090
_P1_1	=	0x0091
_P1_2	=	0x0092
_P1_3	=	0x0093
_P1_4	=	0x0094
_P1_5	=	0x0095
_P1_6	=	0x0096
_P1_7	=	0x0097
_RI	=	0x0098
_TI	=	0x0099
_RB8	=	0x009a
_TB8	=	0x009b
_REN	=	0x009c
_SM2	=	0x009d
_SM1	=	0x009e
_SM0	=	0x009f
_P2_0	=	0x00a0
_P2_1	=	0x00a1
_P2_2	=	0x00a2
_P2_3	=	0x00a3
_P2_4	=	0x00a4
_P2_5	=	0x00a5
_P2_6	=	0x00a6
_P2_7	=	0x00a7
_EX0	=	0x00a8
_ET0	=	0x00a9
_EX1	=	0x00aa
_ET1	=	0x00ab
_ES	=	0x00ac
_EA	=	0x00af
_P3_0	=	0x00b0
_P3_1	=	0x00b1
_P3_2	=	0x00b2
_P3_3	=	0x00b3
_P3_4	=	0x00b4
_P3_5	=	0x00b5
_P3_6	=	0x00b6
_P3_7	=	0x00b7
_RXD	=	0x00b0
_TXD	=	0x00b1
_INT0	=	0x00b2
_INT1	=	0x00b3
_T0	=	0x00b4
_T1	=	0x00b5
_WR	=	0x00b6
_RD	=	0x00b7
_PX0	=	0x00b8
_PT0	=	0x00b9
_PX1	=	0x00ba
_PT1	=	0x00bb
_PS	=	0x00bc
_P	=	0x00d0
_F1	=	0x00d1
_OV	=	0x00d2
_RS0	=	0x00d3
_RS1	=	0x00d4
_F0	=	0x00d5
_AC	=	0x00d6
_CY	=	0x00d7
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_main_i_1_1:
	.ds 2
_main_a_1_1:
	.ds 2
_main_b_1_1:
	.ds 3
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
	.area	OSEG    (OVR,DATA)
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG	(DATA)
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	lcall	_main
;	return from main will lock up
	sjmp .
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'delay'
;------------------------------------------------------------
;i                         Allocated to registers r6 r7 
;a                         Allocated to registers r4 r5 
;b                         Allocated to registers r2 r3 
;------------------------------------------------------------
;	asdf.c:5: void delay(int i)
;	-----------------------------------------
;	 function delay
;	-----------------------------------------
_delay:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	mov	r6,dpl
	mov	r7,dph
;	asdf.c:8: for(a=0;a<i;a++)
	mov	r4,#0x00
	mov	r5,#0x00
00104$:
	clr	c
	mov	a,r4
	subb	a,r6
	mov	a,r5
	xrl	a,#0x80
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jnc	00108$
;	asdf.c:10: for(b=0;b<120;b++);
	mov	r2,#0x78
	mov	r3,#0x00
00103$:
	dec	r2
	cjne	r2,#0xFF,00117$
	dec	r3
00117$:
	mov	a,r2
	orl	a,r3
	jnz	00103$
;	asdf.c:8: for(a=0;a<i;a++)
	inc	r4
	cjne	r4,#0x00,00104$
	inc	r5
	sjmp	00104$
00108$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'display'
;------------------------------------------------------------
;i                         Allocated to registers r4 r5 r6 r7 
;------------------------------------------------------------
;	asdf.c:20: void display(unsigned long int i)
;	-----------------------------------------
;	 function display
;	-----------------------------------------
_display:
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
;	asdf.c:30: P0=255;
	mov	_P0,#0xFF
;	asdf.c:31: P2 = seg_position[3];
	mov	dptr,#(_seg_position + 0x0003)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	asdf.c:32: P0=display_seg[(i%(unsigned long)10000)/(unsigned long)1000];
	mov	__modulong_PARM_2,#0x10
	mov	(__modulong_PARM_2 + 1),#0x27
	clr	a
	mov	(__modulong_PARM_2 + 2),a
	mov	(__modulong_PARM_2 + 3),a
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r7
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	__modulong
	mov	r0,dpl
	mov	r1,dph
	mov	r2,b
	mov	r3,a
	mov	__divulong_PARM_2,#0xE8
	mov	(__divulong_PARM_2 + 1),#0x03
	clr	a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,r0
	mov	dph,r1
	mov	b,r2
	mov	a,r3
	lcall	__divulong
	mov	r0,dpl
	mov	r1,dph
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
	mov	a,r0
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r1
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	asdf.c:33: delay(2);
	mov	dptr,#0x0002
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	_delay
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
;	asdf.c:34: P0=255;
	mov	_P0,#0xFF
;	asdf.c:35: P2 = seg_position[2];
	mov	dptr,#(_seg_position + 0x0002)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	asdf.c:36: P0=display_seg[(i%(unsigned long)1000)/(unsigned long)100];
	mov	__modulong_PARM_2,#0xE8
	mov	(__modulong_PARM_2 + 1),#0x03
	clr	a
	mov	(__modulong_PARM_2 + 2),a
	mov	(__modulong_PARM_2 + 3),a
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r7
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	__modulong
	mov	r0,dpl
	mov	r1,dph
	mov	r2,b
	mov	r3,a
	mov	__divulong_PARM_2,#0x64
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,r0
	mov	dph,r1
	mov	b,r2
	mov	a,r3
	lcall	__divulong
	mov	r0,dpl
	mov	r1,dph
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
	mov	a,r0
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r1
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	asdf.c:37: delay(2);
	mov	dptr,#0x0002
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	_delay
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
;	asdf.c:38: P0=255;
	mov	_P0,#0xFF
;	asdf.c:39: P2 = seg_position[1];
	mov	dptr,#(_seg_position + 0x0001)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	asdf.c:40: P0=display_seg[(i%(unsigned long)100)/10];
	mov	__modulong_PARM_2,#0x64
	clr	a
	mov	(__modulong_PARM_2 + 1),a
	mov	(__modulong_PARM_2 + 2),a
	mov	(__modulong_PARM_2 + 3),a
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r7
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	__modulong
	mov	r0,dpl
	mov	r1,dph
	mov	r2,b
	mov	r3,a
	mov	__divulong_PARM_2,#0x0A
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,r0
	mov	dph,r1
	mov	b,r2
	mov	a,r3
	lcall	__divulong
	mov	r0,dpl
	mov	r1,dph
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
	mov	a,r0
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r1
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	asdf.c:41: delay(2);
	mov	dptr,#0x0002
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	_delay
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
;	asdf.c:42: P0=255;
	mov	_P0,#0xFF
;	asdf.c:43: P2 = seg_position[0];
	mov	dptr,#_seg_position
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	asdf.c:44: P0=display_seg[i%10];
	mov	__modulong_PARM_2,#0x0A
	clr	a
	mov	(__modulong_PARM_2 + 1),a
	mov	(__modulong_PARM_2 + 2),a
	mov	(__modulong_PARM_2 + 3),a
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r7
	lcall	__modulong
	mov	r4,dpl
	mov	r5,dph
	mov	a,r4
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r5
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	asdf.c:45: delay(2);
	mov	dptr,#0x0002
	ljmp	_delay
;------------------------------------------------------------
;Allocation info for local variables in function 'asd'
;------------------------------------------------------------
;i                         Allocated to registers r6 r7 
;------------------------------------------------------------
;	asdf.c:50: void asd() {
;	-----------------------------------------
;	 function asd
;	-----------------------------------------
_asd:
;	asdf.c:52: while(1) {
00102$:
;	asdf.c:53: P0=~P0;
	mov	a,_P0
	cpl	a
	mov	_P0,a
;	asdf.c:54: P1=~P1;
	mov	a,_P1
	cpl	a
	mov	_P1,a
;	asdf.c:55: P2=~P2;
	mov	a,_P2
	cpl	a
	mov	_P2,a
;	asdf.c:56: P3=~P3;
	mov	a,_P3
	cpl	a
	mov	_P3,a
;	asdf.c:57: for(i=0;i<10;i++) {}
	mov	r6,#0x0A
	mov	r7,#0x00
00106$:
	dec	r6
	cjne	r6,#0xFF,00113$
	dec	r7
00113$:
	mov	a,r6
	orl	a,r7
	jnz	00106$
	sjmp	00102$
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;i                         Allocated with name '_main_i_1_1'
;a                         Allocated with name '_main_a_1_1'
;b                         Allocated with name '_main_b_1_1'
;------------------------------------------------------------
;	asdf.c:60: unsigned int main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	asdf.c:65: P0=0xff;
	mov	_P0,#0xFF
;	asdf.c:66: P1=0xff;
	mov	_P1,#0xFF
;	asdf.c:67: P2=0xff;
	mov	_P2,#0xFF
;	asdf.c:68: P3=0xff;
	mov	_P3,#0xFF
;	asdf.c:69: a=&asd;
	mov	_main_a_1_1,#_asd
	mov	(_main_a_1_1 + 1),#(_asd >> 8)
;	asdf.c:70: b=(int*)a;
	mov	r3,_main_a_1_1
	mov	r4,(_main_a_1_1 + 1)
	mov	r5,#0x80
;	asdf.c:71: *b=5432;
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	mov	a,#0x38
	lcall	__gptrput
	inc	dptr
	mov	a,#0x15
	lcall	__gptrput
;	asdf.c:72: for(i=0;i<100;i++)
	mov	_main_i_1_1,#0x64
	mov	(_main_i_1_1 + 1),#0x00
00106$:
;	asdf.c:73: display(*b);
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	mov	r0,a
	inc	dptr
	lcall	__gptrget
	mov	r2,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	dpl,r0
	mov	dph,r2
	mov	b,r6
	push	ar5
	push	ar4
	push	ar3
	lcall	_display
	pop	ar3
	pop	ar4
	pop	ar5
	dec	_main_i_1_1
	mov	a,#0xFF
	cjne	a,_main_i_1_1,00119$
	dec	(_main_i_1_1 + 1)
00119$:
;	asdf.c:72: for(i=0;i<100;i++)
	mov	a,_main_i_1_1
	orl	a,(_main_i_1_1 + 1)
	jnz	00106$
;	asdf.c:74: *b=1234;
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	mov	a,#0xD2
	lcall	__gptrput
	inc	dptr
	mov	a,#0x04
	lcall	__gptrput
;	asdf.c:75: for(i=0;i<100;i++)
	mov	_main_i_1_1,#0x64
	mov	(_main_i_1_1 + 1),#0x00
00109$:
;	asdf.c:76: display(*b);
	mov	dpl,r3
	mov	dph,r4
	mov	b,r5
	lcall	__gptrget
	mov	r1,a
	inc	dptr
	lcall	__gptrget
	mov	r0,a
	rlc	a
	subb	a,acc
	mov	r2,a
	mov	dpl,r1
	mov	dph,r0
	mov	b,r2
	push	ar5
	push	ar4
	push	ar3
	lcall	_display
	pop	ar3
	pop	ar4
	pop	ar5
	dec	_main_i_1_1
	mov	a,#0xFF
	cjne	a,_main_i_1_1,00121$
	dec	(_main_i_1_1 + 1)
00121$:
;	asdf.c:75: for(i=0;i<100;i++)
	mov	a,_main_i_1_1
	orl	a,(_main_i_1_1 + 1)
	jnz	00109$
;	asdf.c:77: a();
	mov	dpl,_main_a_1_1
	mov	dph,(_main_a_1_1 + 1)
	lcall	__sdcc_call_dptr
;	asdf.c:78: while(1)
00102$:
	sjmp	00102$
	.area CSEG    (CODE)
	.area CONST   (CODE)
_display_seg:
	.db #0xC0	; 192
	.db #0xF9	; 249
	.db #0xA4	; 164
	.db #0xB0	; 176
	.db #0x99	; 153
	.db #0x92	; 146
	.db #0x82	; 130
	.db #0xF8	; 248
	.db #0x80	; 128
	.db #0x90	; 144
	.db #0x7F	; 127
_seg_position:
	.db #0xFE	; 254
	.db #0xFD	; 253
	.db #0xFB	; 251
	.db #0xF7	; 247
	.db #0xF0	; 240
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
