;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
; This file was generated Sun May 13 21:12:21 2012
;--------------------------------------------------------
	.module test5
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _seg_position
	.globl _display_seg
	.globl _main
	.globl _func
	.globl _display
	.globl _get_digits
	.globl _get_digit
	.globl _pow
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
	.globl _get_digit_PARM_2
	.globl _pow_PARM_2
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (DATA)
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
	.area RSEG    (DATA)
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
_pow_PARM_2:
	.ds 4
_pow_a_1_1:
	.ds 4
_get_digit_PARM_2:
	.ds 1
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
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
;i                         Allocated to registers r2 r3 
;a                         Allocated to registers r4 r5 
;b                         Allocated to registers r6 r7 
;------------------------------------------------------------
;	test5.c:7: void delay(int i)
;	-----------------------------------------
;	 function delay
;	-----------------------------------------
_delay:
	ar2 = 0x02
	ar3 = 0x03
	ar4 = 0x04
	ar5 = 0x05
	ar6 = 0x06
	ar7 = 0x07
	ar0 = 0x00
	ar1 = 0x01
	mov	r2,dpl
	mov	r3,dph
;	test5.c:10: for(a=0;a<i;a++)
	mov	r4,#0x00
	mov	r5,#0x00
00104$:
	clr	c
	mov	a,r4
	subb	a,r2
	mov	a,r5
	xrl	a,#0x80
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
	jnc	00108$
;	test5.c:12: for(b=0;b<120;b++);
	mov	r6,#0x78
	mov	r7,#0x00
00103$:
	dec	r6
	cjne	r6,#0xff,00117$
	dec	r7
00117$:
	mov	a,r6
	orl	a,r7
	jnz	00103$
;	test5.c:10: for(a=0;a<i;a++)
	inc	r4
	cjne	r4,#0x00,00104$
	inc	r5
	sjmp	00104$
00108$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'pow'
;------------------------------------------------------------
;b                         Allocated with name '_pow_PARM_2'
;a                         Allocated with name '_pow_a_1_1'
;tmp                       Allocated to registers r6 r7 r0 r1 
;------------------------------------------------------------
;	test5.c:22: unsigned long int pow(unsigned long int a, unsigned long int b)
;	-----------------------------------------
;	 function pow
;	-----------------------------------------
_pow:
	mov	_pow_a_1_1,dpl
	mov	(_pow_a_1_1 + 1),dph
	mov	(_pow_a_1_1 + 2),b
	mov	(_pow_a_1_1 + 3),a
;	test5.c:24: unsigned long int tmp=1;
	mov	r6,#0x01
	mov	r7,#0x00
	mov	r0,#0x00
	mov	r1,#0x00
;	test5.c:25: while(b>0)
	mov	r2,_pow_PARM_2
	mov	r3,(_pow_PARM_2 + 1)
	mov	r4,(_pow_PARM_2 + 2)
	mov	r5,(_pow_PARM_2 + 3)
00101$:
	mov	a,r2
	orl	a,r3
	orl	a,r4
	orl	a,r5
	jz	00103$
;	test5.c:27: tmp=tmp*a;
	mov	__mullong_PARM_2,_pow_a_1_1
	mov	(__mullong_PARM_2 + 1),(_pow_a_1_1 + 1)
	mov	(__mullong_PARM_2 + 2),(_pow_a_1_1 + 2)
	mov	(__mullong_PARM_2 + 3),(_pow_a_1_1 + 3)
	mov	dpl,r6
	mov	dph,r7
	mov	b,r0
	mov	a,r1
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	__mullong
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	test5.c:28: b--;
	dec	r2
	cjne	r2,#0xff,00101$
	dec	r3
	cjne	r3,#0xff,00101$
	dec	r4
	cjne	r4,#0xff,00101$
	dec	r5
	sjmp	00101$
00103$:
;	test5.c:30: return tmp;
	mov	dpl,r6
	mov	dph,r7
	mov	b,r0
	mov	a,r1
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'get_digit'
;------------------------------------------------------------
;d                         Allocated with name '_get_digit_PARM_2'
;i                         Allocated to registers r2 r3 r4 r5 
;------------------------------------------------------------
;	test5.c:32: unsigned char get_digit(unsigned long int i, unsigned char d)
;	-----------------------------------------
;	 function get_digit
;	-----------------------------------------
_get_digit:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	test5.c:35: return i%(pow(10,d)*10)/pow(10,d);
	mov	r6,_get_digit_PARM_2
	mov	r7,#0x00
	mov	r0,#0x00
	mov	r1,#0x00
	mov	_pow_PARM_2,r6
	mov	(_pow_PARM_2 + 1),r7
	mov	(_pow_PARM_2 + 2),r0
	mov	(_pow_PARM_2 + 3),r1
	mov	dptr,#(0x0A&0x00ff)
	clr	a
	mov	b,a
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	lcall	_pow
	mov	__mullong_PARM_2,dpl
	mov	(__mullong_PARM_2 + 1),dph
	mov	(__mullong_PARM_2 + 2),b
	mov	(__mullong_PARM_2 + 3),a
	mov	dptr,#(0x0A&0x00ff)
	clr	a
	mov	b,a
	lcall	__mullong
	mov	__modulong_PARM_2,dpl
	mov	(__modulong_PARM_2 + 1),dph
	mov	(__modulong_PARM_2 + 2),b
	mov	(__modulong_PARM_2 + 3),a
	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	lcall	__modulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	mov	_pow_PARM_2,r6
	mov	(_pow_PARM_2 + 1),r7
	mov	(_pow_PARM_2 + 2),r0
	mov	(_pow_PARM_2 + 3),r1
	mov	dptr,#(0x0A&0x00ff)
	clr	a
	mov	b,a
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	_pow
	mov	__divulong_PARM_2,dpl
	mov	(__divulong_PARM_2 + 1),dph
	mov	(__divulong_PARM_2 + 2),b
	mov	(__divulong_PARM_2 + 3),a
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	ljmp	__divulong
;------------------------------------------------------------
;Allocation info for local variables in function 'get_digits'
;------------------------------------------------------------
;i                         Allocated to registers r2 r3 r4 r5 
;tmp                       Allocated to registers r6 
;------------------------------------------------------------
;	test5.c:37: unsigned char get_digits(unsigned long int i)
;	-----------------------------------------
;	 function get_digits
;	-----------------------------------------
_get_digits:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	test5.c:40: while(i>0)
	mov	r6,#0x00
00101$:
	mov	a,r2
	orl	a,r3
	orl	a,r4
	orl	a,r5
	jz	00103$
;	test5.c:42: tmp++;
	inc	r6
;	test5.c:43: i=i/10;
	mov	__divulong_PARM_2,#0x0A
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	push	ar6
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	pop	ar6
	sjmp	00101$
00103$:
;	test5.c:45: return tmp;
	mov	dpl,r6
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'display'
;------------------------------------------------------------
;i                         Allocated to registers r2 r3 r4 r5 
;d0                        Allocated to registers 
;d1                        Allocated to registers 
;d2                        Allocated to registers 
;d3                        Allocated to registers r6 
;------------------------------------------------------------
;	test5.c:47: void display(unsigned long int i)
;	-----------------------------------------
;	 function display
;	-----------------------------------------
_display:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	test5.c:53: unsigned char d3=0xFF;
	mov	r6,#0xFF
;	test5.c:88: P0=255;
	mov	_P0,#0xFF
;	test5.c:89: P2 = seg_position[3];
	mov	dptr,#(_seg_position + 0x0003)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test5.c:90: if(i>=10000)
	clr	c
	mov	a,r2
	subb	a,#0x10
	mov	a,r3
	subb	a,#0x27
	mov	a,r4
	subb	a,#0x00
	mov	a,r5
	subb	a,#0x00
	jc	00102$
;	test5.c:92: d3=0x7F;
	mov	r6,#0x7F
00102$:
;	test5.c:94: P0=display_seg[get_digit(i,3)] & d3;
	mov	_get_digit_PARM_2,#0x03
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	lcall	_get_digit
	mov	r7,dpl
	pop	ar6
	mov	a,r7
	mov	dptr,#_display_seg
	movc	a,@a+dptr
	mov	r7,a
	mov	a,r6
	anl	a,r7
	mov	_P0,a
;	test5.c:95: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	test5.c:97: P0=255;
	mov	_P0,#0xFF
;	test5.c:98: P2 = seg_position[2];
	mov	dptr,#(_seg_position + 0x0002)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test5.c:99: P0=display_seg[get_digit(i,2)] & d2;
	mov	_get_digit_PARM_2,#0x02
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	_get_digit
	mov	a,dpl
	mov	dptr,#_display_seg
	movc	a,@a+dptr
	mov	_P0,a
;	test5.c:100: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	test5.c:101: P0=255;
	mov	_P0,#0xFF
;	test5.c:102: P2 = seg_position[1];
	mov	dptr,#(_seg_position + 0x0001)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test5.c:103: P0=display_seg[get_digit(i,1)] & d1;
	mov	_get_digit_PARM_2,#0x01
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	_get_digit
	mov	a,dpl
	mov	dptr,#_display_seg
	movc	a,@a+dptr
	mov	_P0,a
;	test5.c:104: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	test5.c:105: P0=255;
	mov	_P0,#0xFF
;	test5.c:106: P2 = seg_position[0];
	mov	dptr,#_seg_position
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test5.c:107: P0=display_seg[get_digit(i,0)] & d0;
	mov	_get_digit_PARM_2,#0x00
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	_get_digit
	mov	a,dpl
	mov	dptr,#_display_seg
	movc	a,@a+dptr
	mov	_P0,a
;	test5.c:108: delay(2);
	mov	dptr,#0x0002
	ljmp	_delay
;------------------------------------------------------------
;Allocation info for local variables in function 'func'
;------------------------------------------------------------
;------------------------------------------------------------
;	test5.c:112: void func()
;	-----------------------------------------
;	 function func
;	-----------------------------------------
_func:
;	test5.c:114: P2=~P2;
	mov	a,_P2
	cpl	a
	mov	_P2,a
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;f                         Allocated to registers 
;------------------------------------------------------------
;	test5.c:117: void main()
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	test5.c:121: while(1)
00102$:
;	test5.c:123: display(get_digits(1234567));
	mov	dptr,#0xD687
	mov	b,#0x12
	clr	a
	lcall	_get_digits
	mov	r2,dpl
	mov	r3,#0x00
	mov	r4,#0x00
	mov	r5,#0x00
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	_display
	sjmp	00102$
	.area CSEG    (CODE)
	.area CONST   (CODE)
_display_seg:
	.db #0xC0
	.db #0xF9
	.db #0xA4
	.db #0xB0
	.db #0x99
	.db #0x92
	.db #0x82
	.db #0xF8
	.db #0x80
	.db #0x90
	.db #0x7F
_seg_position:
	.db #0xFE
	.db #0xFD
	.db #0xFB
	.db #0xF7
	.db #0xF0
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
