;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.1.0 #7066 (Feb 26 2012) (Linux)
; This file was generated Sat Dec  1 15:06:17 2012
;--------------------------------------------------------
	.module test1
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _seg_position
	.globl _display_seg
	.globl _s
	.globl _time
	.globl _t
	.globl _c
	.globl _b
	.globl _main
	.globl _display
	.globl _delay
	.globl _TF2
	.globl _EXF2
	.globl _RCLK
	.globl _TCLK
	.globl _EXEN2
	.globl _TR2
	.globl _C_T2
	.globl _CP_RL2
	.globl _T2CON_7
	.globl _T2CON_6
	.globl _T2CON_5
	.globl _T2CON_4
	.globl _T2CON_3
	.globl _T2CON_2
	.globl _T2CON_1
	.globl _T2CON_0
	.globl _PT2
	.globl _ET2
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
	.globl _TH2
	.globl _TL2
	.globl _RCAP2H
	.globl _RCAP2L
	.globl _T2CON
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
	.globl __c
	.globl _i
	.globl _count
	.globl _t_l
	.globl _t_h
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
_T2CON	=	0x00c8
_RCAP2L	=	0x00ca
_RCAP2H	=	0x00cb
_TL2	=	0x00cc
_TH2	=	0x00cd
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
_ET2	=	0x00ad
_PT2	=	0x00bd
_T2CON_0	=	0x00c8
_T2CON_1	=	0x00c9
_T2CON_2	=	0x00ca
_T2CON_3	=	0x00cb
_T2CON_4	=	0x00cc
_T2CON_5	=	0x00cd
_T2CON_6	=	0x00ce
_T2CON_7	=	0x00cf
_CP_RL2	=	0x00c8
_C_T2	=	0x00c9
_TR2	=	0x00ca
_EXEN2	=	0x00cb
_TCLK	=	0x00cc
_RCLK	=	0x00cd
_EXF2	=	0x00ce
_TF2	=	0x00cf
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; overlayable bit register bank
;--------------------------------------------------------
	.area BIT_BANK	(REL,OVR,DATA)
bits:
	.ds 1
	b0 = bits[0]
	b1 = bits[1]
	b2 = bits[2]
	b3 = bits[3]
	b4 = bits[4]
	b5 = bits[5]
	b6 = bits[6]
	b7 = bits[7]
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_t_h::
	.ds 1
_t_l::
	.ds 1
_count::
	.ds 2
_i::
	.ds 2
__c::
	.ds 2
_c_sloc0_1_0:
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
	ljmp	_b
	.ds	5
	reti
	.ds	7
	ljmp	_c
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
;	test1.c:31: int count=29;//33;
	mov	_count,#0x1D
	mov	(_count + 1),#0x00
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
;	test1.c:2: void delay(int i)
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
;	test1.c:5: for(a=0;a<i;a++)
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
;	test1.c:7: for(b=0;b<120;b++);
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
;	test1.c:5: for(a=0;a<i;a++)
	inc	r4
	cjne	r4,#0x00,00104$
	inc	r5
	sjmp	00104$
00108$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'display'
;------------------------------------------------------------
;i                         Allocated to registers r6 r7 
;------------------------------------------------------------
;	test1.c:36: void display(int i)
;	-----------------------------------------
;	 function display
;	-----------------------------------------
_display:
	mov	r6,dpl
	mov	r7,dph
;	test1.c:46: P0=255;
	mov	_P0,#0xFF
;	test1.c:47: P2 = seg_position[2];
	mov	dptr,#(_seg_position + 0x0002)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test1.c:48: P0=display_seg[i%1000/100];
	mov	__modsint_PARM_2,#0xE8
	mov	(__modsint_PARM_2 + 1),#0x03
	mov	dpl,r6
	mov	dph,r7
	push	ar7
	push	ar6
	lcall	__modsint
	mov	__divsint_PARM_2,#0x64
	mov	(__divsint_PARM_2 + 1),#0x00
	lcall	__divsint
	mov	r4,dpl
	mov	r5,dph
	pop	ar6
	pop	ar7
	mov	a,r4
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r5
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	test1.c:49: delay(2);
	mov	dptr,#0x0002
	push	ar7
	push	ar6
	lcall	_delay
	pop	ar6
	pop	ar7
;	test1.c:50: P0=255;
	mov	_P0,#0xFF
;	test1.c:51: P2 = seg_position[1];
	mov	dptr,#(_seg_position + 0x0001)
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test1.c:52: P0=display_seg[i%100/10];
	mov	__modsint_PARM_2,#0x64
	mov	(__modsint_PARM_2 + 1),#0x00
	mov	dpl,r6
	mov	dph,r7
	push	ar7
	push	ar6
	lcall	__modsint
	mov	__divsint_PARM_2,#0x0A
	mov	(__divsint_PARM_2 + 1),#0x00
	lcall	__divsint
	mov	r4,dpl
	mov	r5,dph
	pop	ar6
	pop	ar7
	mov	a,r4
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r5
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	test1.c:53: delay(2);
	mov	dptr,#0x0002
	push	ar7
	push	ar6
	lcall	_delay
	pop	ar6
	pop	ar7
;	test1.c:54: P0=255;
	mov	_P0,#0xFF
;	test1.c:55: P2 = seg_position[0];
	mov	dptr,#_seg_position
	clr	a
	movc	a,@a+dptr
	mov	_P2,a
;	test1.c:56: P0=display_seg[i%10];
	mov	__modsint_PARM_2,#0x0A
	mov	(__modsint_PARM_2 + 1),#0x00
	mov	dpl,r6
	mov	dph,r7
	lcall	__modsint
	mov	r6,dpl
	mov	r7,dph
	mov	a,r6
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r7
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	test1.c:57: delay(2);
	mov	dptr,#0x0002
	ljmp	_delay
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;	test1.c:63: main()
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	test1.c:70: IE=0x8a;
	mov	_IE,#0x8A
;	test1.c:71: TMOD=0x11;
	mov	_TMOD,#0x11
;	test1.c:119: set_bit(P2,4,0);
	mov	r7,_P2
	anl	ar7,#0xEF
	mov	_P2,r7
;	test1.c:126: i=0;
;	test1.c:127: _c=0;
	clr	a
	mov	_i,a
	mov	(_i + 1),a
	mov	__c,a
	mov	(__c + 1),a
;	test1.c:128: TH1=(65536-50000)/256;
	mov	_TH1,#0x3C
;	test1.c:129: TL1=(65536-50000)%256;
	mov	_TL1,#0xB0
;	test1.c:130: TR1=1;
	setb	_TR1
;	test1.c:131: while(1)
00102$:
;	test1.c:133: display(i);
	mov	dpl,_i
	mov	dph,(_i + 1)
	lcall	_display
;	test1.c:134: i++;
	inc	_i
	clr	a
	cjne	a,_i,00110$
	inc	(_i + 1)
00110$:
;	test1.c:135: set_bit(P2,4,!get_bit(P2,4));
	mov	a,#0x10
	anl	a,_P2
	mov	r7,a
	cjne	a,#0x01,00111$
00111$:
	clr	a
	rlc	a
	mov	r7,a
	jz	00106$
	mov	a,#0x10
	orl	a,_P2
	mov	r7,a
	sjmp	00107$
00106$:
	mov	r6,_P2
	mov	a,#0xEF
	anl	a,r6
	mov	r7,a
00107$:
	mov	_P2,r7
	sjmp	00102$
;------------------------------------------------------------
;Allocation info for local variables in function 'b'
;------------------------------------------------------------
;	test1.c:160: void b(void) interrupt 1
;	-----------------------------------------
;	 function b
;	-----------------------------------------
_b:
	push	acc
	push	ar7
	push	ar6
	push	psw
	mov	psw,#0x00
;	test1.c:163: set_bit(P2,4,!get_bit(P2,4));
	mov	a,#0x10
	anl	a,_P2
	mov	r7,a
	cjne	a,#0x01,00106$
00106$:
	clr	a
	rlc	a
	mov	r7,a
	jz	00103$
	mov	a,#0x10
	orl	a,_P2
	mov	r7,a
	sjmp	00104$
00103$:
	mov	r6,_P2
	mov	a,#0xEF
	anl	a,r6
	mov	r7,a
00104$:
	mov	_P2,r7
;	test1.c:164: TH0=t_h;
	mov	_TH0,_t_h
;	test1.c:165: TL0=t_l;
	mov	_TL0,_t_l
	pop	psw
	pop	ar6
	pop	ar7
	pop	acc
	reti
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;------------------------------------------------------------
;Allocation info for local variables in function 'c'
;------------------------------------------------------------
;sloc0                     Allocated with name '_c_sloc0_1_0'
;------------------------------------------------------------
;	test1.c:169: void c(void) interrupt 3
;	-----------------------------------------
;	 function c
;	-----------------------------------------
_c:
	push	bits
	push	acc
	push	b
	push	dpl
	push	dph
	push	(0+7)
	push	(0+6)
	push	(0+5)
	push	(0+4)
	push	(0+3)
	push	(0+2)
	push	(0+1)
	push	(0+0)
	push	psw
	mov	psw,#0x00
;	test1.c:171: TH1=(65536-50000)/256;
	mov	_TH1,#0x3C
;	test1.c:172: TL1=(65536-50000)%256;
	mov	_TL1,#0xB0
;	test1.c:173: _c+=50;
	mov	a,#0x32
	add	a,__c
	mov	__c,a
	clr	a
	addc	a,(__c + 1)
	mov	(__c + 1),a
;	test1.c:175: if(_c<time[i])return;
	mov	a,_i
	add	a,_i
	mov	r6,a
	mov	a,(_i + 1)
	rlc	a
	mov	r7,a
	mov	a,r6
	add	a,#_time
	mov	dpl,a
	mov	a,r7
	addc	a,#(_time >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	r6,a
	inc	dptr
	clr	a
	movc	a,@a+dptr
	mov	r7,a
	mov	r4,__c
	mov	r5,(__c + 1)
	clr	c
	mov	a,r4
	subb	a,r6
	mov	a,r5
	subb	a,r7
	jnc	00102$
	ljmp	00109$
00102$:
;	test1.c:176: _c=0;
	clr	a
	mov	__c,a
	mov	(__c + 1),a
;	test1.c:177: if(s[i]==0)TR0=0;
	mov	a,_i
	add	a,#_s
	mov	dpl,a
	mov	a,(_i + 1)
	addc	a,#(_s >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	r7,a
	jnz	00104$
	clr	_TR0
00104$:
;	test1.c:178: t_h=(65536-(t[s[i]]))/256;
	mov	a,r7
	add	a,r7
	mov	r5,a
	mov	dptr,#_t
	movc	a,@a+dptr
	xch	a,r5
	inc	dptr
	movc	a,@a+dptr
	mov	r6,a
	clr	a
	mov	r4,a
	mov	r3,a
	clr	c
	subb	a,r5
	mov	_c_sloc0_1_0,a
	clr	a
	subb	a,r6
	mov	(_c_sloc0_1_0 + 1),a
	mov	a,#0x01
	subb	a,r4
	mov	(_c_sloc0_1_0 + 2),a
	clr	a
	subb	a,r3
	mov	(_c_sloc0_1_0 + 3),a
	clr	a
	mov	__divslong_PARM_2,a
	mov	(__divslong_PARM_2 + 1),#0x01
	mov	(__divslong_PARM_2 + 2),a
	mov	(__divslong_PARM_2 + 3),a
	mov	dpl,_c_sloc0_1_0
	mov	dph,(_c_sloc0_1_0 + 1)
	mov	b,(_c_sloc0_1_0 + 2)
	mov	a,(_c_sloc0_1_0 + 3)
	push	ar7
	lcall	__divslong
	mov	r0,dpl
	mov	_t_h,r0
;	test1.c:179: t_l=(65536-(t[s[i]]))%256;
	clr	a
	mov	__modslong_PARM_2,a
	mov	(__modslong_PARM_2 + 1),#0x01
	mov	(__modslong_PARM_2 + 2),a
	mov	(__modslong_PARM_2 + 3),a
	mov	dpl,_c_sloc0_1_0
	mov	dph,(_c_sloc0_1_0 + 1)
	mov	b,(_c_sloc0_1_0 + 2)
	mov	a,(_c_sloc0_1_0 + 3)
	lcall	__modslong
	mov	r3,dpl
	mov	r4,dph
	mov	r5,b
	mov	r6,a
	pop	ar7
	mov	_t_l,r3
;	test1.c:180: TH0=t_h;
	mov	_TH0,_t_h
;	test1.c:181: TL0=t_l;
	mov	_TL0,_t_l
;	test1.c:182: if(s[i]!=0)TR0=1;
	mov	a,r7
	jz	00106$
	setb	_TR0
00106$:
;	test1.c:185: i+=1;
	inc	_i
	clr	a
	cjne	a,_i,00118$
	inc	(_i + 1)
00118$:
;	test1.c:186: if(i>=count)i=0;
	clr	c
	mov	a,_i
	subb	a,_count
	mov	a,(_i + 1)
	xrl	a,#0x80
	mov	b,(_count + 1)
	xrl	b,#0x80
	subb	a,b
	jc	00109$
	clr	a
	mov	_i,a
	mov	(_i + 1),a
00109$:
	pop	psw
	pop	(0+0)
	pop	(0+1)
	pop	(0+2)
	pop	(0+3)
	pop	(0+4)
	pop	(0+5)
	pop	(0+6)
	pop	(0+7)
	pop	dph
	pop	dpl
	pop	b
	pop	acc
	pop	bits
	reti
	.area CSEG    (CODE)
	.area CONST   (CODE)
_t:
	.byte #0x00,#0x00	; 0
	.byte #0xBC,#0x03	; 956
	.byte #0x54,#0x03	; 852
	.byte #0xF7,#0x02	; 759
	.byte #0xCC,#0x02	; 716
	.byte #0x7E,#0x02	; 638
	.byte #0x38,#0x02	; 568
	.byte #0xFA,#0x01	; 506
	.byte #0xDE,#0x01	; 478
	.byte #0xAA,#0x01	; 426
	.byte #0x7B,#0x01	; 379
	.byte #0x00,#0x00	; 0
_time:
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
	.byte #0xF4,#0x01	; 500
_s:
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x04	; 4
	.db #0x03	; 3
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x05	; 5
	.db #0x04	; 4
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x08	; 8
	.db #0x06	; 6
	.db #0x04	; 4
	.db #0x03	; 3
	.db #0x02	; 2
	.db #0x00	; 0
	.db #0x07	; 7
	.db #0x07	; 7
	.db #0x06	; 6
	.db #0x04	; 4
	.db #0x05	; 5
	.db #0x04	; 4
	.db #0x00	; 0
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
	.db #0x06	; 6
	.db #0x05	; 5
	.db #0x03	; 3
	.db #0x07	; 7
	.db #0x00	; 0
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
