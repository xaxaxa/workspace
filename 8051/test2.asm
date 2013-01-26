;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.1.0 #7066 (Feb 26 2012) (Linux)
; This file was generated Sat Dec  1 15:04:55 2012
;--------------------------------------------------------
	.module test2
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _display_seg6
	.globl _display_seg5
	.globl _display_seg4
	.globl _display_seg3
	.globl _display_seg2
	.globl _display_seg
	.globl _main
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
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
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
;i                         Allocated to registers r6 r7 
;a                         Allocated to registers r4 r5 
;b                         Allocated to registers r2 r3 
;------------------------------------------------------------
;	test2.c:16: void delay(int i)
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
;	test2.c:19: for(a=0;a<i;a++)
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
;	test2.c:21: for(b=0;b<120;b++);
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
;	test2.c:19: for(a=0;a<i;a++)
	inc	r4
	cjne	r4,#0x00,00104$
	inc	r5
	sjmp	00104$
00108$:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;i                         Allocated to registers r6 r7 
;i2                        Allocated to registers r4 r5 
;------------------------------------------------------------
;	test2.c:32: void main()
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	test2.c:34: int i=0;
	mov	r6,#0x00
	mov	r7,#0x00
;	test2.c:35: int i2=0;
	mov	r4,#0x00
	mov	r5,#0x00
;	test2.c:37: P2=0xFE;
	mov	_P2,#0xFE
;	test2.c:39: set_bit(P2,0,1);
	orl	_P2,#0x01
;	test2.c:40: while(1)
00109$:
;	test2.c:42: P1=~display_seg6[i];
	mov	a,r6
	add	a,#_display_seg6
	mov	dpl,a
	mov	a,r7
	addc	a,#(_display_seg6 >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	r3,a
	cpl	a
	mov	_P1,a
;	test2.c:43: P0=display_seg[i2];
	mov	a,r4
	add	a,#_display_seg
	mov	dpl,a
	mov	a,r5
	addc	a,#(_display_seg >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	_P0,a
;	test2.c:44: while(!get_bit(P2,0));
00101$:
	mov	a,_P2
	jnb	acc.0,00101$
;	test2.c:46: i++;
	inc	r6
	cjne	r6,#0x00,00119$
	inc	r7
00119$:
;	test2.c:47: /*if(tmp==0)*/i2++;
	inc	r4
	cjne	r4,#0x00,00120$
	inc	r5
00120$:
;	test2.c:48: if(display_seg6[i]==0x11)i=0;
	mov	a,r6
	add	a,#_display_seg6
	mov	dpl,a
	mov	a,r7
	addc	a,#(_display_seg6 >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	mov	r3,a
	cjne	r3,#0x11,00105$
	mov	r6,#0x00
	mov	r7,#0x00
00105$:
;	test2.c:49: if(i2>9)i2=0;
	clr	c
	mov	a,#0x09
	subb	a,r4
	clr	a
	xrl	a,#0x80
	mov	b,r5
	xrl	b,#0x80
	subb	a,b
	jnc	00107$
	mov	r4,#0x00
	mov	r5,#0x00
00107$:
;	test2.c:52: delay(200);
	mov	dptr,#0x00C8
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	_delay
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
	sjmp	00109$
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
_display_seg2:
	.db #0xFE	; 254
	.db #0xFD	; 253
	.db #0xFB	; 251
	.db #0xF7	; 247
	.db #0xEF	; 239
	.db #0xDF	; 223
_display_seg3:
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x08	; 8
	.db #0x10	; 16
	.db #0x20	; 32
	.db #0x40	; 64
	.db #0x80	; 128
_display_seg4:
	.db #0xFF	; 255
	.db #0x90	; 144
	.db #0x90	; 144
	.db #0x90	; 144
	.db #0x00	; 0
	.db #0xFF	; 255
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0xFF	; 255
	.db #0x00	; 0
	.db #0x18	; 24
	.db #0x24	; 36
	.db #0x42	; 66	'B'
	.db #0x81	; 129
	.db #0x00	; 0
	.db #0xFF	; 255
	.db #0x04	; 4
	.db #0x0A	; 10
	.db #0x11	; 17
	.db #0x00	; 0
_display_seg5:
	.db #0x88	; 136
	.db #0x44	; 68	'D'
	.db #0x22	; 34
	.db #0x11	; 17
_display_seg6:
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x08	; 8
	.db #0x10	; 16
	.db #0x20	; 32
	.db #0x40	; 64
	.db #0x80	; 128
	.db #0x81	; 129
	.db #0x82	; 130
	.db #0x84	; 132
	.db #0x88	; 136
	.db #0x90	; 144
	.db #0xA0	; 160
	.db #0xC0	; 192
	.db #0xC1	; 193
	.db #0xC2	; 194
	.db #0xC4	; 196
	.db #0xC8	; 200
	.db #0xD0	; 208
	.db #0xE0	; 224
	.db #0xE1	; 225
	.db #0xE2	; 226
	.db #0xE4	; 228
	.db #0xE8	; 232
	.db #0xF0	; 240
	.db #0xF1	; 241
	.db #0xF2	; 242
	.db #0xF4	; 244
	.db #0xF8	; 248
	.db #0xF9	; 249
	.db #0xFA	; 250
	.db #0xFC	; 252
	.db #0xFD	; 253
	.db #0xFE	; 254
	.db #0xFF	; 255
	.db #0x11	; 17
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)