;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
; This file was generated Sun May 13 19:22:03 2012
;--------------------------------------------------------
	.module test3
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _seg_position
	.globl _display_seg
	.globl _timer
	.globl _main
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
	.globl _i
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
_display_i_1_1:
	.ds 4
_display_d1_1_1:
	.ds 1
_display_d2_1_1:
	.ds 1
_display_d3_1_1:
	.ds 1
_display_sloc0_1_0:
	.ds 4
_i::
	.ds 4
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
	reti
	.ds	7
	ljmp	_timer
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
;	test3.c:84: unsigned long int i=0;
	clr	a
	mov	_i,a
	mov	(_i + 1),a
	mov	(_i + 2),a
	mov	(_i + 3),a
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
;	test3.c:6: void delay(int i)
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
;	test3.c:9: for(a=0;a<i;a++)
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
;	test3.c:11: for(b=0;b<120;b++);
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
;	test3.c:9: for(a=0;a<i;a++)
	inc	r4
	cjne	r4,#0x00,00104$
	inc	r5
	sjmp	00104$
00108$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'display'
;------------------------------------------------------------
;i                         Allocated with name '_display_i_1_1'
;d0                        Allocated to registers 
;d1                        Allocated with name '_display_d1_1_1'
;d2                        Allocated with name '_display_d2_1_1'
;d3                        Allocated with name '_display_d3_1_1'
;sloc0                     Allocated with name '_display_sloc0_1_0'
;------------------------------------------------------------
;	test3.c:21: void display(unsigned long int i)
;	-----------------------------------------
;	 function display
;	-----------------------------------------
_display:
	mov	_display_i_1_1,dpl
	mov	(_display_i_1_1 + 1),dph
	mov	(_display_i_1_1 + 2),b
	mov	(_display_i_1_1 + 3),a
;	test3.c:25: unsigned char d1=0xFF;
	mov	_display_d1_1_1,#0xFF
;	test3.c:26: unsigned char d2=0x7F;
	mov	_display_d2_1_1,#0x7F
;	test3.c:27: unsigned char d3=0xFF;
	mov	_display_d3_1_1,#0xFF
;	test3.c:29: if(i/600>=60*100)
	mov	__divulong_PARM_2,#0x58
	mov	(__divulong_PARM_2 + 1),#0x02
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	r1,dpl
	mov	r0,dph
	mov	r7,b
	mov	r6,a
	clr	c
	mov	a,r1
	subb	a,#0x70
	mov	a,r0
	subb	a,#0x17
	mov	a,r7
	subb	a,#0x00
	mov	a,r6
	subb	a,#0x00
	jnc	00118$
	ljmp	00110$
00118$:
;	test3.c:31: i=i/60/60/100*1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/6000*10000;
	mov	__divulong_PARM_2,#0x00
	mov	(__divulong_PARM_2 + 1),#0x97
	mov	(__divulong_PARM_2 + 2),#0xFF
	mov	(__divulong_PARM_2 + 3),#0xFF
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	__mullong_PARM_2,dpl
	mov	(__mullong_PARM_2 + 1),dph
	mov	(__mullong_PARM_2 + 2),b
	mov	(__mullong_PARM_2 + 3),a
	mov	dptr,#0x4240
	mov	b,#0x0F
	clr	a
	lcall	__mullong
	mov	_display_sloc0_1_0,dpl
	mov	(_display_sloc0_1_0 + 1),dph
	mov	(_display_sloc0_1_0 + 2),b
	mov	(_display_sloc0_1_0 + 3),a
	mov	__modulong_PARM_2,#0x40
	mov	(__modulong_PARM_2 + 1),#0x7E
	mov	(__modulong_PARM_2 + 2),#0x05
	mov	(__modulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__modulong
	mov	__divulong_PARM_2,#0x70
	mov	(__divulong_PARM_2 + 1),#0x17
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	lcall	__divulong
	mov	__mullong_PARM_2,dpl
	mov	(__mullong_PARM_2 + 1),dph
	mov	(__mullong_PARM_2 + 2),b
	mov	(__mullong_PARM_2 + 3),a
	mov	dptr,#0x2710
	clr	a
	mov	b,a
	lcall	__mullong
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	mov	a,r6
	add	a,_display_sloc0_1_0
	mov	_display_i_1_1,a
	mov	a,r7
	addc	a,(_display_sloc0_1_0 + 1)
	mov	(_display_i_1_1 + 1),a
	mov	a,r0
	addc	a,(_display_sloc0_1_0 + 2)
	mov	(_display_i_1_1 + 2),a
	mov	a,r1
	addc	a,(_display_sloc0_1_0 + 3)
	mov	(_display_i_1_1 + 3),a
;	test3.c:32: i/=10000;
	mov	__divulong_PARM_2,#0x10
	mov	(__divulong_PARM_2 + 1),#0x27
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	_display_i_1_1,dpl
	mov	(_display_i_1_1 + 1),dph
	mov	(_display_i_1_1 + 2),b
	mov	(_display_i_1_1 + 3),a
	ljmp	00111$
00110$:
;	test3.c:37: else if(i/60>=60*100)
	mov	__divulong_PARM_2,#0x3C
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	clr	c
	mov	a,r6
	subb	a,#0x70
	mov	a,r7
	subb	a,#0x17
	mov	a,r0
	subb	a,#0x00
	mov	a,r1
	subb	a,#0x00
	jnc	00119$
	ljmp	00107$
00119$:
;	test3.c:39: i=i/60/60/100*1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/6000*10000 + i%6000;
	mov	__divulong_PARM_2,#0x00
	mov	(__divulong_PARM_2 + 1),#0x97
	mov	(__divulong_PARM_2 + 2),#0xFF
	mov	(__divulong_PARM_2 + 3),#0xFF
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	__mullong_PARM_2,dpl
	mov	(__mullong_PARM_2 + 1),dph
	mov	(__mullong_PARM_2 + 2),b
	mov	(__mullong_PARM_2 + 3),a
	mov	dptr,#0x4240
	mov	b,#0x0F
	clr	a
	lcall	__mullong
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	mov	__modulong_PARM_2,#0x40
	mov	(__modulong_PARM_2 + 1),#0x7E
	mov	(__modulong_PARM_2 + 2),#0x05
	mov	(__modulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	lcall	__modulong
	mov	__divulong_PARM_2,#0x70
	mov	(__divulong_PARM_2 + 1),#0x17
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	lcall	__divulong
	mov	__mullong_PARM_2,dpl
	mov	(__mullong_PARM_2 + 1),dph
	mov	(__mullong_PARM_2 + 2),b
	mov	(__mullong_PARM_2 + 3),a
	mov	dptr,#0x2710
	clr	a
	mov	b,a
	lcall	__mullong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	mov	a,r2
	add	a,r6
	mov	r6,a
	mov	a,r3
	addc	a,r7
	mov	r7,a
	mov	a,r4
	addc	a,r0
	mov	r0,a
	mov	a,r5
	addc	a,r1
	mov	r1,a
	mov	__modulong_PARM_2,#0x70
	mov	(__modulong_PARM_2 + 1),#0x17
	mov	(__modulong_PARM_2 + 2),#0x00
	mov	(__modulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
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
	mov	a,r2
	add	a,r6
	mov	_display_i_1_1,a
	mov	a,r3
	addc	a,r7
	mov	(_display_i_1_1 + 1),a
	mov	a,r4
	addc	a,r0
	mov	(_display_i_1_1 + 2),a
	mov	a,r5
	addc	a,r1
	mov	(_display_i_1_1 + 3),a
;	test3.c:40: i/=1000;
	mov	__divulong_PARM_2,#0xE8
	mov	(__divulong_PARM_2 + 1),#0x03
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	_display_i_1_1,dpl
	mov	(_display_i_1_1 + 1),dph
	mov	(_display_i_1_1 + 2),b
	mov	(_display_i_1_1 + 3),a
;	test3.c:41: d2=0xFF;
	mov	_display_d2_1_1,#0xFF
;	test3.c:42: d1=0x7F;
	mov	_display_d1_1_1,#0x7F
;	test3.c:43: d3=0x7F;
	mov	_display_d3_1_1,#0x7F
	ljmp	00111$
00107$:
;	test3.c:45: else if(i>=60000)
	clr	c
	mov	a,_display_i_1_1
	subb	a,#0x60
	mov	a,(_display_i_1_1 + 1)
	subb	a,#0xEA
	mov	a,(_display_i_1_1 + 2)
	subb	a,#0x00
	mov	a,(_display_i_1_1 + 3)
	subb	a,#0x00
	jnc	00120$
	ljmp	00104$
00120$:
;	test3.c:47: i=i/6000*10000+i%6000;
	mov	__divulong_PARM_2,#0x70
	mov	(__divulong_PARM_2 + 1),#0x17
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	__mullong_PARM_2,dpl
	mov	(__mullong_PARM_2 + 1),dph
	mov	(__mullong_PARM_2 + 2),b
	mov	(__mullong_PARM_2 + 3),a
	mov	dptr,#0x2710
	clr	a
	mov	b,a
	lcall	__mullong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	__modulong_PARM_2,#0x70
	mov	(__modulong_PARM_2 + 1),#0x17
	mov	(__modulong_PARM_2 + 2),#0x00
	mov	(__modulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	__modulong
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	mov	a,r6
	add	a,r2
	mov	_display_i_1_1,a
	mov	a,r7
	addc	a,r3
	mov	(_display_i_1_1 + 1),a
	mov	a,r0
	addc	a,r4
	mov	(_display_i_1_1 + 2),a
	mov	a,r1
	addc	a,r5
	mov	(_display_i_1_1 + 3),a
;	test3.c:48: i/=100;
	mov	__divulong_PARM_2,#0x64
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	_display_i_1_1,dpl
	mov	(_display_i_1_1 + 1),dph
	mov	(_display_i_1_1 + 2),b
	mov	(_display_i_1_1 + 3),a
	ljmp	00111$
00104$:
;	test3.c:53: else if(i>=6000)
	clr	c
	mov	a,_display_i_1_1
	subb	a,#0x70
	mov	a,(_display_i_1_1 + 1)
	subb	a,#0x17
	mov	a,(_display_i_1_1 + 2)
	subb	a,#0x00
	mov	a,(_display_i_1_1 + 3)
	subb	a,#0x00
	jnc	00121$
	ljmp	00111$
00121$:
;	test3.c:56: i=i/6000*10000+i%6000;
	mov	__divulong_PARM_2,#0x70
	mov	(__divulong_PARM_2 + 1),#0x17
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	__mullong_PARM_2,dpl
	mov	(__mullong_PARM_2 + 1),dph
	mov	(__mullong_PARM_2 + 2),b
	mov	(__mullong_PARM_2 + 3),a
	mov	dptr,#0x2710
	clr	a
	mov	b,a
	lcall	__mullong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	__modulong_PARM_2,#0x70
	mov	(__modulong_PARM_2 + 1),#0x17
	mov	(__modulong_PARM_2 + 2),#0x00
	mov	(__modulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	__modulong
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	mov	a,r6
	add	a,r2
	mov	_display_i_1_1,a
	mov	a,r7
	addc	a,r3
	mov	(_display_i_1_1 + 1),a
	mov	a,r0
	addc	a,r4
	mov	(_display_i_1_1 + 2),a
	mov	a,r1
	addc	a,r5
	mov	(_display_i_1_1 + 3),a
;	test3.c:57: i/=10;
	mov	__divulong_PARM_2,#0x0A
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	_display_i_1_1,dpl
	mov	(_display_i_1_1 + 1),dph
	mov	(_display_i_1_1 + 2),b
	mov	(_display_i_1_1 + 3),a
;	test3.c:58: d2=0xFF;
	mov	_display_d2_1_1,#0xFF
;	test3.c:59: d1=0x7F;
	mov	_display_d1_1_1,#0x7F
;	test3.c:60: d3=0x7F;
	mov	_display_d3_1_1,#0x7F
00111$:
;	test3.c:62: P0=255;
	mov	_P0,#0xFF
;	test3.c:64: P2 = seg_position[3];
	mov	dptr,#(_seg_position + 0x0003)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test3.c:65: P0=display_seg[i/1000] & d3;
	mov	__divulong_PARM_2,#0xE8
	mov	(__divulong_PARM_2 + 1),#0x03
	mov	(__divulong_PARM_2 + 2),#0x00
	mov	(__divulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	a,r2
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r3
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	r2,a
	mov	a,_display_d3_1_1
	anl	a,r2
	mov	_P0,a
;	test3.c:66: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
;	test3.c:68: P0=255;
	mov	_P0,#0xFF
;	test3.c:69: P2 = seg_position[2];
	mov	dptr,#(_seg_position + 0x0002)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test3.c:70: P0=display_seg[i%1000/100] & d2;
	mov	__modulong_PARM_2,#0xE8
	mov	(__modulong_PARM_2 + 1),#0x03
	mov	(__modulong_PARM_2 + 2),#0x00
	mov	(__modulong_PARM_2 + 3),#0x00
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__modulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	__divulong_PARM_2,#0x64
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	a,r2
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r3
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	r2,a
	mov	a,_display_d2_1_1
	anl	a,r2
	mov	_P0,a
;	test3.c:71: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
;	test3.c:72: P0=255;
	mov	_P0,#0xFF
;	test3.c:73: P2 = seg_position[1];
	mov	dptr,#(_seg_position + 0x0001)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test3.c:74: P0=display_seg[i%100/10] & d1;
	mov	__modulong_PARM_2,#0x64
	clr	a
	mov	(__modulong_PARM_2 + 1),a
	mov	(__modulong_PARM_2 + 2),a
	mov	(__modulong_PARM_2 + 3),a
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__modulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	__divulong_PARM_2,#0x0A
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	a,r2
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r3
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	r2,a
	mov	a,_display_d1_1_1
	anl	a,r2
	mov	_P0,a
;	test3.c:75: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
;	test3.c:76: P0=255;
	mov	_P0,#0xFF
;	test3.c:77: P2 = seg_position[0];
	mov	dptr,#_seg_position
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test3.c:78: P0=display_seg[i%10] & d0;
	mov	__modulong_PARM_2,#0x0A
	clr	a
	mov	(__modulong_PARM_2 + 1),a
	mov	(__modulong_PARM_2 + 2),a
	mov	(__modulong_PARM_2 + 3),a
	mov	dpl,_display_i_1_1
	mov	dph,(_display_i_1_1 + 1)
	mov	b,(_display_i_1_1 + 2)
	mov	a,(_display_i_1_1 + 3)
	lcall	__modulong
	mov	r2,dpl
	mov	r3,dph
	mov	a,r2
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r3
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	test3.c:79: delay(2);
	mov	dptr,#0x0002
	ljmp	_delay
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;------------------------------------------------------------
;	test3.c:85: void main()
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	test3.c:88: IE=0x8a;
	mov	_IE,#0x8A
;	test3.c:89: TMOD=0x11;
	mov	_TMOD,#0x11
;	test3.c:90: TH0=(65536-10000)/256;
	mov	_TH0,#0xD8
;	test3.c:91: TL0=(65536-10000)%256;
	mov	_TL0,#0xF0
;	test3.c:92: TR0=1;
	setb	_TR0
;	test3.c:93: while(1)
00102$:
;	test3.c:97: display(i);
	mov	dpl,_i
	mov	dph,(_i + 1)
	mov	b,(_i + 2)
	mov	a,(_i + 3)
	lcall	_display
	sjmp	00102$
;------------------------------------------------------------
;Allocation info for local variables in function 'timer'
;------------------------------------------------------------
;------------------------------------------------------------
;	test3.c:102: void timer() interrupt 1
;	-----------------------------------------
;	 function timer
;	-----------------------------------------
_timer:
	push	acc
	push	psw
	mov	psw,#0x00
;	test3.c:104: TH0=(65536-10000)/256;
	mov	_TH0,#0xD8
;	test3.c:105: TL0=(65536-10000)%256;
	mov	_TL0,#0xF0
;	test3.c:106: i++;
	inc	_i
	clr	a
	cjne	a,_i,00103$
	inc	(_i + 1)
	cjne	a,(_i + 1),00103$
	inc	(_i + 2)
	cjne	a,(_i + 2),00103$
	inc	(_i + 3)
00103$:
	pop	psw
	pop	acc
	reti
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
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
