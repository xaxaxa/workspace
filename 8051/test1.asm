;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
; This file was generated Sun May 13 18:20:02 2012
;--------------------------------------------------------
	.module test1
	.optsdcc -mmcs51 --model-large
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _c
	.globl _b
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
	.globl _count
	.globl _seg_position
	.globl _display_seg
	.globl _s
	.globl _time
	.globl _t
	.globl __c
	.globl _i
	.globl _t_l
	.globl _t_h
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
_c_sloc0_1_0:
	.ds 4
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area OSEG    (OVR,DATA)
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
_delay_i_1_1:
	.ds 2
_t_h::
	.ds 1
_t_l::
	.ds 1
_display_i_1_1:
	.ds 2
_i::
	.ds 2
__c::
	.ds 2
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
_t::
	.ds 24
_time::
	.ds 66
_s::
	.ds 29
_display_seg::
	.ds 11
_seg_position::
	.ds 5
_count::
	.ds 2
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
;i                         Allocated with name '_delay_i_1_1'
;a                         Allocated with name '_delay_a_1_1'
;b                         Allocated with name '_delay_b_1_1'
;------------------------------------------------------------
;	test1.c:2: void delay(int i)
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
	mov	r2,dph
	mov	a,dpl
	mov	dptr,#_delay_i_1_1
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
;	test1.c:5: for(a=0;a<i;a++)
	mov	dptr,#_delay_i_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
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
;	test1.c:7: for(b=0;b<120;b++);
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
;i                         Allocated with name '_display_i_1_1'
;------------------------------------------------------------
;	test1.c:36: void display(int i)
;	-----------------------------------------
;	 function display
;	-----------------------------------------
_display:
	mov	r2,dph
	mov	a,dpl
	mov	dptr,#_display_i_1_1
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
;	test1.c:46: P0=255;
	mov	_P0,#0xFF
;	test1.c:47: P2 = seg_position[2];
	mov	dptr,#(_seg_position + 0x0002)
	movx	a,@dptr
	mov	_P2,a
;	test1.c:48: P0=display_seg[i%1000/100];
	mov	dptr,#_display_i_1_1
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#__modsint_PARM_2
	mov	a,#0xE8
	movx	@dptr,a
	inc	dptr
	mov	a,#0x03
	movx	@dptr,a
	mov	dpl,r2
	mov	dph,r3
	push	ar2
	push	ar3
	lcall	__modsint
	mov	r4,dpl
	mov	r5,dph
	mov	dptr,#__divsint_PARM_2
	mov	a,#0x64
	movx	@dptr,a
	clr	a
	inc	dptr
	movx	@dptr,a
	mov	dpl,r4
	mov	dph,r5
	lcall	__divsint
	mov	r4,dpl
	mov	r5,dph
	mov	a,r4
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r5
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	_P0,a
;	test1.c:49: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
	pop	ar3
	pop	ar2
;	test1.c:50: P0=255;
	mov	_P0,#0xFF
;	test1.c:51: P2 = seg_position[1];
	mov	dptr,#(_seg_position + 0x0001)
	movx	a,@dptr
	mov	_P2,a
;	test1.c:52: P0=display_seg[i%100/10];
	mov	dptr,#__modsint_PARM_2
	mov	a,#0x64
	movx	@dptr,a
	clr	a
	inc	dptr
	movx	@dptr,a
	mov	dpl,r2
	mov	dph,r3
	push	ar2
	push	ar3
	lcall	__modsint
	mov	r4,dpl
	mov	r5,dph
	mov	dptr,#__divsint_PARM_2
	mov	a,#0x0A
	movx	@dptr,a
	clr	a
	inc	dptr
	movx	@dptr,a
	mov	dpl,r4
	mov	dph,r5
	lcall	__divsint
	mov	r4,dpl
	mov	r5,dph
	mov	a,r4
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r5
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	_P0,a
;	test1.c:53: delay(2);
	mov	dptr,#0x0002
	lcall	_delay
	pop	ar3
	pop	ar2
;	test1.c:54: P0=255;
	mov	_P0,#0xFF
;	test1.c:55: P2 = seg_position[0];
	mov	dptr,#_seg_position
	movx	a,@dptr
	mov	_P2,a
;	test1.c:56: P0=display_seg[i%10];
	mov	dptr,#__modsint_PARM_2
	mov	a,#0x0A
	movx	@dptr,a
	clr	a
	inc	dptr
	movx	@dptr,a
	mov	dpl,r2
	mov	dph,r3
	lcall	__modsint
	mov	r2,dpl
	mov	r3,dph
	mov	a,r2
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r3
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	_P0,a
;	test1.c:57: delay(2);
	mov	dptr,#0x0002
	ljmp	_delay
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
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
	anl	_P2,#0xEF
;	test1.c:126: i=0;
	mov	dptr,#_i
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
;	test1.c:127: _c=0;
	mov	dptr,#__c
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
;	test1.c:128: TH1=(65536-50000)/256;
	mov	_TH1,#0x3C
;	test1.c:129: TL1=(65536-50000)%256;
	mov	_TL1,#0xB0
;	test1.c:130: TR1=1;
	setb	_TR1
;	test1.c:131: while(1)
00102$:
;	test1.c:133: display(i);
	mov	dptr,#_i
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dpl,r2
	mov	dph,r3
	lcall	_display
;	test1.c:134: i++;
	mov	dptr,#_i
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#_i
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
	clr	a
	addc	a,r3
	inc	dptr
	movx	@dptr,a
;	test1.c:135: set_bit(P2,4,!get_bit(P2,4));
	mov	a,#0x10
	anl	a,_P2
	mov	r2,a
	cjne	a,#0x01,00110$
00110$:
	clr	a
	rlc	a
	mov	r2,a
	jz	00106$
	mov	a,#0x10
	orl	a,_P2
	mov	r2,a
	sjmp	00107$
00106$:
	mov	r3,_P2
	mov	a,#0xEF
	anl	a,r3
	mov	r2,a
00107$:
	mov	_P2,r2
	sjmp	00102$
;------------------------------------------------------------
;Allocation info for local variables in function 'b'
;------------------------------------------------------------
;------------------------------------------------------------
;	test1.c:160: void b(void) interrupt 1
;	-----------------------------------------
;	 function b
;	-----------------------------------------
_b:
	push	acc
	push	dpl
	push	dph
	push	ar2
	push	ar3
	push	psw
	mov	psw,#0x00
;	test1.c:163: set_bit(P2,4,!get_bit(P2,4));
	mov	a,#0x10
	anl	a,_P2
	mov	r2,a
	cjne	a,#0x01,00106$
00106$:
	clr	a
	rlc	a
	mov	r2,a
	jz	00103$
	mov	a,#0x10
	orl	a,_P2
	mov	r2,a
	sjmp	00104$
00103$:
	mov	r3,_P2
	mov	a,#0xEF
	anl	a,r3
	mov	r2,a
00104$:
	mov	_P2,r2
;	test1.c:164: TH0=t_h;
	mov	dptr,#_t_h
	movx	a,@dptr
	mov	_TH0,a
;	test1.c:165: TL0=t_l;
	mov	dptr,#_t_l
	movx	a,@dptr
	mov	_TL0,a
	pop	psw
	pop	ar3
	pop	ar2
	pop	dph
	pop	dpl
	pop	acc
	reti
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
	push	(0+2)
	push	(0+3)
	push	(0+4)
	push	(0+5)
	push	(0+6)
	push	(0+7)
	push	(0+0)
	push	(0+1)
	push	psw
	mov	psw,#0x00
;	test1.c:171: TH1=(65536-50000)/256;
	mov	_TH1,#0x3C
;	test1.c:172: TL1=(65536-50000)%256;
	mov	_TL1,#0xB0
;	test1.c:173: _c+=50;
	mov	dptr,#__c
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#__c
	mov	a,#0x32
	add	a,r2
	movx	@dptr,a
	clr	a
	addc	a,r3
	inc	dptr
	movx	@dptr,a
;	test1.c:175: if(_c<time[i])return;
	mov	dptr,#_i
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	ar4,r2
	xch	a,r4
	add	a,acc
	xch	a,r4
	rlc	a
	mov	r5,a
	mov	a,r4
	add	a,#_time
	mov	dpl,a
	mov	a,r5
	addc	a,#(_time >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#__c
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	clr	c
	mov	a,r6
	subb	a,r4
	mov	a,r7
	subb	a,r5
	jnc	00102$
	ljmp	00109$
00102$:
;	test1.c:176: _c=0;
	mov	dptr,#__c
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
;	test1.c:177: if(s[i]==0)TR0=0;
	mov	a,r2
	add	a,#_s
	mov	dpl,a
	mov	a,r3
	addc	a,#(_s >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r4,a
	jnz	00104$
	clr	_TR0
00104$:
;	test1.c:178: t_h=(65536-(t[s[i]]))/256;
	push	ar2
	push	ar3
	mov	a,r4
	add	a,r4
	add	a,#_t
	mov	dpl,a
	clr	a
	addc	a,#(_t >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	clr	a
	mov	r7,a
	mov	r0,a
	clr	c
	subb	a,r5
	mov	_c_sloc0_1_0,a
	clr	a
	subb	a,r6
	mov	(_c_sloc0_1_0 + 1),a
	mov	a,#0x01
	subb	a,r7
	mov	(_c_sloc0_1_0 + 2),a
	clr	a
	subb	a,r0
	mov	(_c_sloc0_1_0 + 3),a
	mov	dptr,#__divslong_PARM_2
	clr	a
	movx	@dptr,a
	inc	dptr
	mov	a,#0x01
	movx	@dptr,a
	inc	dptr
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
	mov	dpl,_c_sloc0_1_0
	mov	dph,(_c_sloc0_1_0 + 1)
	mov	b,(_c_sloc0_1_0 + 2)
	mov	a,(_c_sloc0_1_0 + 3)
	push	ar4
	lcall	__divslong
	mov	r1,dpl
	mov	dptr,#_t_h
	mov	a,r1
	movx	@dptr,a
;	test1.c:179: t_l=(65536-(t[s[i]]))%256;
	mov	dptr,#__modslong_PARM_2
	clr	a
	movx	@dptr,a
	inc	dptr
	mov	a,#0x01
	movx	@dptr,a
	inc	dptr
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
	mov	dpl,_c_sloc0_1_0
	mov	dph,(_c_sloc0_1_0 + 1)
	mov	b,(_c_sloc0_1_0 + 2)
	mov	a,(_c_sloc0_1_0 + 3)
	push	ar1
	lcall	__modslong
	mov	r2,dpl
	mov	r5,b
	mov	r6,a
	pop	ar1
	pop	ar4
	mov	dptr,#_t_l
	mov	a,r2
	movx	@dptr,a
;	test1.c:180: TH0=t_h;
	mov	_TH0,r1
;	test1.c:181: TL0=t_l;
	mov	_TL0,r2
;	test1.c:182: if(s[i]!=0)TR0=1;
	pop	ar3
	pop	ar2
	mov	a,r4
	jz	00106$
	setb	_TR0
00106$:
;	test1.c:185: i+=1;
	mov	dptr,#_i
	mov	a,#0x01
	add	a,r2
	movx	@dptr,a
	clr	a
	addc	a,r3
	inc	dptr
	movx	@dptr,a
;	test1.c:186: if(i>=count)i=0;
	mov	dptr,#_i
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#_count
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	clr	c
	mov	a,r2
	subb	a,r4
	mov	a,r3
	xrl	a,#0x80
	mov	b,r5
	xrl	b,#0x80
	subb	a,b
	jc	00109$
	mov	dptr,#_i
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
00109$:
	pop	psw
	pop	(0+1)
	pop	(0+0)
	pop	(0+7)
	pop	(0+6)
	pop	(0+5)
	pop	(0+4)
	pop	(0+3)
	pop	(0+2)
	pop	dph
	pop	dpl
	pop	b
	pop	acc
	pop	bits
	reti
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
__xinit__t:
	.byte #0x00,#0x00
	.byte #0xBC,#0x03
	.byte #0x54,#0x03
	.byte #0xF7,#0x02
	.byte #0xCC,#0x02
	.byte #0x7E,#0x02
	.byte #0x38,#0x02
	.byte #0xFA,#0x01
	.byte #0xDE,#0x01
	.byte #0xAA,#0x01
	.byte #0x7B,#0x01
	.byte #0x00,#0x00
__xinit__time:
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
	.byte #0xF4,#0x01
__xinit__s:
	.db #0x01
	.db #0x01
	.db #0x02
	.db #0x01
	.db #0x04
	.db #0x03
	.db #0x00
	.db #0x01
	.db #0x01
	.db #0x02
	.db #0x01
	.db #0x05
	.db #0x04
	.db #0x00
	.db #0x01
	.db #0x01
	.db #0x08
	.db #0x06
	.db #0x04
	.db #0x03
	.db #0x02
	.db #0x00
	.db #0x07
	.db #0x07
	.db #0x06
	.db #0x04
	.db #0x05
	.db #0x04
	.db #0x00
__xinit__display_seg:
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
__xinit__seg_position:
	.db #0x06
	.db #0x05
	.db #0x03
	.db #0x07
	.db #0x00
__xinit__count:
	.byte #0x1D,#0x00
	.area CABS    (ABS,CODE)
