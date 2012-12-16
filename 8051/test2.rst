                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 3.1.0 #7066 (Feb 26 2012) (Linux)
                              4 ; This file was generated Sat Dec  1 15:04:55 2012
                              5 ;--------------------------------------------------------
                              6 	.module test2
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _display_seg6
                             13 	.globl _display_seg5
                             14 	.globl _display_seg4
                             15 	.globl _display_seg3
                             16 	.globl _display_seg2
                             17 	.globl _display_seg
                             18 	.globl _main
                             19 	.globl _delay
                             20 	.globl _TF2
                             21 	.globl _EXF2
                             22 	.globl _RCLK
                             23 	.globl _TCLK
                             24 	.globl _EXEN2
                             25 	.globl _TR2
                             26 	.globl _C_T2
                             27 	.globl _CP_RL2
                             28 	.globl _T2CON_7
                             29 	.globl _T2CON_6
                             30 	.globl _T2CON_5
                             31 	.globl _T2CON_4
                             32 	.globl _T2CON_3
                             33 	.globl _T2CON_2
                             34 	.globl _T2CON_1
                             35 	.globl _T2CON_0
                             36 	.globl _PT2
                             37 	.globl _ET2
                             38 	.globl _CY
                             39 	.globl _AC
                             40 	.globl _F0
                             41 	.globl _RS1
                             42 	.globl _RS0
                             43 	.globl _OV
                             44 	.globl _F1
                             45 	.globl _P
                             46 	.globl _PS
                             47 	.globl _PT1
                             48 	.globl _PX1
                             49 	.globl _PT0
                             50 	.globl _PX0
                             51 	.globl _RD
                             52 	.globl _WR
                             53 	.globl _T1
                             54 	.globl _T0
                             55 	.globl _INT1
                             56 	.globl _INT0
                             57 	.globl _TXD
                             58 	.globl _RXD
                             59 	.globl _P3_7
                             60 	.globl _P3_6
                             61 	.globl _P3_5
                             62 	.globl _P3_4
                             63 	.globl _P3_3
                             64 	.globl _P3_2
                             65 	.globl _P3_1
                             66 	.globl _P3_0
                             67 	.globl _EA
                             68 	.globl _ES
                             69 	.globl _ET1
                             70 	.globl _EX1
                             71 	.globl _ET0
                             72 	.globl _EX0
                             73 	.globl _P2_7
                             74 	.globl _P2_6
                             75 	.globl _P2_5
                             76 	.globl _P2_4
                             77 	.globl _P2_3
                             78 	.globl _P2_2
                             79 	.globl _P2_1
                             80 	.globl _P2_0
                             81 	.globl _SM0
                             82 	.globl _SM1
                             83 	.globl _SM2
                             84 	.globl _REN
                             85 	.globl _TB8
                             86 	.globl _RB8
                             87 	.globl _TI
                             88 	.globl _RI
                             89 	.globl _P1_7
                             90 	.globl _P1_6
                             91 	.globl _P1_5
                             92 	.globl _P1_4
                             93 	.globl _P1_3
                             94 	.globl _P1_2
                             95 	.globl _P1_1
                             96 	.globl _P1_0
                             97 	.globl _TF1
                             98 	.globl _TR1
                             99 	.globl _TF0
                            100 	.globl _TR0
                            101 	.globl _IE1
                            102 	.globl _IT1
                            103 	.globl _IE0
                            104 	.globl _IT0
                            105 	.globl _P0_7
                            106 	.globl _P0_6
                            107 	.globl _P0_5
                            108 	.globl _P0_4
                            109 	.globl _P0_3
                            110 	.globl _P0_2
                            111 	.globl _P0_1
                            112 	.globl _P0_0
                            113 	.globl _TH2
                            114 	.globl _TL2
                            115 	.globl _RCAP2H
                            116 	.globl _RCAP2L
                            117 	.globl _T2CON
                            118 	.globl _B
                            119 	.globl _ACC
                            120 	.globl _PSW
                            121 	.globl _IP
                            122 	.globl _P3
                            123 	.globl _IE
                            124 	.globl _P2
                            125 	.globl _SBUF
                            126 	.globl _SCON
                            127 	.globl _P1
                            128 	.globl _TH1
                            129 	.globl _TH0
                            130 	.globl _TL1
                            131 	.globl _TL0
                            132 	.globl _TMOD
                            133 	.globl _TCON
                            134 	.globl _PCON
                            135 	.globl _DPH
                            136 	.globl _DPL
                            137 	.globl _SP
                            138 	.globl _P0
                            139 ;--------------------------------------------------------
                            140 ; special function registers
                            141 ;--------------------------------------------------------
                            142 	.area RSEG    (ABS,DATA)
   0000                     143 	.org 0x0000
                    0080    144 _P0	=	0x0080
                    0081    145 _SP	=	0x0081
                    0082    146 _DPL	=	0x0082
                    0083    147 _DPH	=	0x0083
                    0087    148 _PCON	=	0x0087
                    0088    149 _TCON	=	0x0088
                    0089    150 _TMOD	=	0x0089
                    008A    151 _TL0	=	0x008a
                    008B    152 _TL1	=	0x008b
                    008C    153 _TH0	=	0x008c
                    008D    154 _TH1	=	0x008d
                    0090    155 _P1	=	0x0090
                    0098    156 _SCON	=	0x0098
                    0099    157 _SBUF	=	0x0099
                    00A0    158 _P2	=	0x00a0
                    00A8    159 _IE	=	0x00a8
                    00B0    160 _P3	=	0x00b0
                    00B8    161 _IP	=	0x00b8
                    00D0    162 _PSW	=	0x00d0
                    00E0    163 _ACC	=	0x00e0
                    00F0    164 _B	=	0x00f0
                    00C8    165 _T2CON	=	0x00c8
                    00CA    166 _RCAP2L	=	0x00ca
                    00CB    167 _RCAP2H	=	0x00cb
                    00CC    168 _TL2	=	0x00cc
                    00CD    169 _TH2	=	0x00cd
                            170 ;--------------------------------------------------------
                            171 ; special function bits
                            172 ;--------------------------------------------------------
                            173 	.area RSEG    (ABS,DATA)
   0000                     174 	.org 0x0000
                    0080    175 _P0_0	=	0x0080
                    0081    176 _P0_1	=	0x0081
                    0082    177 _P0_2	=	0x0082
                    0083    178 _P0_3	=	0x0083
                    0084    179 _P0_4	=	0x0084
                    0085    180 _P0_5	=	0x0085
                    0086    181 _P0_6	=	0x0086
                    0087    182 _P0_7	=	0x0087
                    0088    183 _IT0	=	0x0088
                    0089    184 _IE0	=	0x0089
                    008A    185 _IT1	=	0x008a
                    008B    186 _IE1	=	0x008b
                    008C    187 _TR0	=	0x008c
                    008D    188 _TF0	=	0x008d
                    008E    189 _TR1	=	0x008e
                    008F    190 _TF1	=	0x008f
                    0090    191 _P1_0	=	0x0090
                    0091    192 _P1_1	=	0x0091
                    0092    193 _P1_2	=	0x0092
                    0093    194 _P1_3	=	0x0093
                    0094    195 _P1_4	=	0x0094
                    0095    196 _P1_5	=	0x0095
                    0096    197 _P1_6	=	0x0096
                    0097    198 _P1_7	=	0x0097
                    0098    199 _RI	=	0x0098
                    0099    200 _TI	=	0x0099
                    009A    201 _RB8	=	0x009a
                    009B    202 _TB8	=	0x009b
                    009C    203 _REN	=	0x009c
                    009D    204 _SM2	=	0x009d
                    009E    205 _SM1	=	0x009e
                    009F    206 _SM0	=	0x009f
                    00A0    207 _P2_0	=	0x00a0
                    00A1    208 _P2_1	=	0x00a1
                    00A2    209 _P2_2	=	0x00a2
                    00A3    210 _P2_3	=	0x00a3
                    00A4    211 _P2_4	=	0x00a4
                    00A5    212 _P2_5	=	0x00a5
                    00A6    213 _P2_6	=	0x00a6
                    00A7    214 _P2_7	=	0x00a7
                    00A8    215 _EX0	=	0x00a8
                    00A9    216 _ET0	=	0x00a9
                    00AA    217 _EX1	=	0x00aa
                    00AB    218 _ET1	=	0x00ab
                    00AC    219 _ES	=	0x00ac
                    00AF    220 _EA	=	0x00af
                    00B0    221 _P3_0	=	0x00b0
                    00B1    222 _P3_1	=	0x00b1
                    00B2    223 _P3_2	=	0x00b2
                    00B3    224 _P3_3	=	0x00b3
                    00B4    225 _P3_4	=	0x00b4
                    00B5    226 _P3_5	=	0x00b5
                    00B6    227 _P3_6	=	0x00b6
                    00B7    228 _P3_7	=	0x00b7
                    00B0    229 _RXD	=	0x00b0
                    00B1    230 _TXD	=	0x00b1
                    00B2    231 _INT0	=	0x00b2
                    00B3    232 _INT1	=	0x00b3
                    00B4    233 _T0	=	0x00b4
                    00B5    234 _T1	=	0x00b5
                    00B6    235 _WR	=	0x00b6
                    00B7    236 _RD	=	0x00b7
                    00B8    237 _PX0	=	0x00b8
                    00B9    238 _PT0	=	0x00b9
                    00BA    239 _PX1	=	0x00ba
                    00BB    240 _PT1	=	0x00bb
                    00BC    241 _PS	=	0x00bc
                    00D0    242 _P	=	0x00d0
                    00D1    243 _F1	=	0x00d1
                    00D2    244 _OV	=	0x00d2
                    00D3    245 _RS0	=	0x00d3
                    00D4    246 _RS1	=	0x00d4
                    00D5    247 _F0	=	0x00d5
                    00D6    248 _AC	=	0x00d6
                    00D7    249 _CY	=	0x00d7
                    00AD    250 _ET2	=	0x00ad
                    00BD    251 _PT2	=	0x00bd
                    00C8    252 _T2CON_0	=	0x00c8
                    00C9    253 _T2CON_1	=	0x00c9
                    00CA    254 _T2CON_2	=	0x00ca
                    00CB    255 _T2CON_3	=	0x00cb
                    00CC    256 _T2CON_4	=	0x00cc
                    00CD    257 _T2CON_5	=	0x00cd
                    00CE    258 _T2CON_6	=	0x00ce
                    00CF    259 _T2CON_7	=	0x00cf
                    00C8    260 _CP_RL2	=	0x00c8
                    00C9    261 _C_T2	=	0x00c9
                    00CA    262 _TR2	=	0x00ca
                    00CB    263 _EXEN2	=	0x00cb
                    00CC    264 _TCLK	=	0x00cc
                    00CD    265 _RCLK	=	0x00cd
                    00CE    266 _EXF2	=	0x00ce
                    00CF    267 _TF2	=	0x00cf
                            268 ;--------------------------------------------------------
                            269 ; overlayable register banks
                            270 ;--------------------------------------------------------
                            271 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     272 	.ds 8
                            273 ;--------------------------------------------------------
                            274 ; internal ram data
                            275 ;--------------------------------------------------------
                            276 	.area DSEG    (DATA)
                            277 ;--------------------------------------------------------
                            278 ; overlayable items in internal ram 
                            279 ;--------------------------------------------------------
                            280 	.area	OSEG    (OVR,DATA)
                            281 ;--------------------------------------------------------
                            282 ; Stack segment in internal ram 
                            283 ;--------------------------------------------------------
                            284 	.area	SSEG	(DATA)
   0008                     285 __start__stack:
   0008                     286 	.ds	1
                            287 
                            288 ;--------------------------------------------------------
                            289 ; indirectly addressable internal ram data
                            290 ;--------------------------------------------------------
                            291 	.area ISEG    (DATA)
                            292 ;--------------------------------------------------------
                            293 ; absolute internal ram data
                            294 ;--------------------------------------------------------
                            295 	.area IABS    (ABS,DATA)
                            296 	.area IABS    (ABS,DATA)
                            297 ;--------------------------------------------------------
                            298 ; bit data
                            299 ;--------------------------------------------------------
                            300 	.area BSEG    (BIT)
                            301 ;--------------------------------------------------------
                            302 ; paged external ram data
                            303 ;--------------------------------------------------------
                            304 	.area PSEG    (PAG,XDATA)
                            305 ;--------------------------------------------------------
                            306 ; external ram data
                            307 ;--------------------------------------------------------
                            308 	.area XSEG    (XDATA)
                            309 ;--------------------------------------------------------
                            310 ; absolute external ram data
                            311 ;--------------------------------------------------------
                            312 	.area XABS    (ABS,XDATA)
                            313 ;--------------------------------------------------------
                            314 ; external initialized ram data
                            315 ;--------------------------------------------------------
                            316 	.area XISEG   (XDATA)
                            317 	.area HOME    (CODE)
                            318 	.area GSINIT0 (CODE)
                            319 	.area GSINIT1 (CODE)
                            320 	.area GSINIT2 (CODE)
                            321 	.area GSINIT3 (CODE)
                            322 	.area GSINIT4 (CODE)
                            323 	.area GSINIT5 (CODE)
                            324 	.area GSINIT  (CODE)
                            325 	.area GSFINAL (CODE)
                            326 	.area CSEG    (CODE)
                            327 ;--------------------------------------------------------
                            328 ; interrupt vector 
                            329 ;--------------------------------------------------------
                            330 	.area HOME    (CODE)
   0000                     331 __interrupt_vect:
   0000 02 00 08            332 	ljmp	__sdcc_gsinit_startup
                            333 ;--------------------------------------------------------
                            334 ; global & static initialisations
                            335 ;--------------------------------------------------------
                            336 	.area HOME    (CODE)
                            337 	.area GSINIT  (CODE)
                            338 	.area GSFINAL (CODE)
                            339 	.area GSINIT  (CODE)
                            340 	.globl __sdcc_gsinit_startup
                            341 	.globl __sdcc_program_startup
                            342 	.globl __start__stack
                            343 	.globl __mcs51_genXINIT
                            344 	.globl __mcs51_genXRAMCLEAR
                            345 	.globl __mcs51_genRAMCLEAR
                            346 	.area GSFINAL (CODE)
   0061 02 00 03            347 	ljmp	__sdcc_program_startup
                            348 ;--------------------------------------------------------
                            349 ; Home
                            350 ;--------------------------------------------------------
                            351 	.area HOME    (CODE)
                            352 	.area HOME    (CODE)
   0003                     353 __sdcc_program_startup:
   0003 12 00 90            354 	lcall	_main
                            355 ;	return from main will lock up
   0006 80 FE               356 	sjmp .
                            357 ;--------------------------------------------------------
                            358 ; code
                            359 ;--------------------------------------------------------
                            360 	.area CSEG    (CODE)
                            361 ;------------------------------------------------------------
                            362 ;Allocation info for local variables in function 'delay'
                            363 ;------------------------------------------------------------
                            364 ;i                         Allocated to registers r6 r7 
                            365 ;a                         Allocated to registers r4 r5 
                            366 ;b                         Allocated to registers r2 r3 
                            367 ;------------------------------------------------------------
                            368 ;	test2.c:16: void delay(int i)
                            369 ;	-----------------------------------------
                            370 ;	 function delay
                            371 ;	-----------------------------------------
   0064                     372 _delay:
                    0007    373 	ar7 = 0x07
                    0006    374 	ar6 = 0x06
                    0005    375 	ar5 = 0x05
                    0004    376 	ar4 = 0x04
                    0003    377 	ar3 = 0x03
                    0002    378 	ar2 = 0x02
                    0001    379 	ar1 = 0x01
                    0000    380 	ar0 = 0x00
   0064 AE 82               381 	mov	r6,dpl
   0066 AF 83               382 	mov	r7,dph
                            383 ;	test2.c:19: for(a=0;a<i;a++)
   0068 7C 00               384 	mov	r4,#0x00
   006A 7D 00               385 	mov	r5,#0x00
   006C                     386 00104$:
   006C C3                  387 	clr	c
   006D EC                  388 	mov	a,r4
   006E 9E                  389 	subb	a,r6
   006F ED                  390 	mov	a,r5
   0070 64 80               391 	xrl	a,#0x80
   0072 8F F0               392 	mov	b,r7
   0074 63 F0 80            393 	xrl	b,#0x80
   0077 95 F0               394 	subb	a,b
   0079 50 14               395 	jnc	00108$
                            396 ;	test2.c:21: for(b=0;b<120;b++);
   007B 7A 78               397 	mov	r2,#0x78
   007D 7B 00               398 	mov	r3,#0x00
   007F                     399 00103$:
   007F 1A                  400 	dec	r2
   0080 BA FF 01            401 	cjne	r2,#0xFF,00117$
   0083 1B                  402 	dec	r3
   0084                     403 00117$:
   0084 EA                  404 	mov	a,r2
   0085 4B                  405 	orl	a,r3
   0086 70 F7               406 	jnz	00103$
                            407 ;	test2.c:19: for(a=0;a<i;a++)
   0088 0C                  408 	inc	r4
   0089 BC 00 E0            409 	cjne	r4,#0x00,00104$
   008C 0D                  410 	inc	r5
   008D 80 DD               411 	sjmp	00104$
   008F                     412 00108$:
   008F 22                  413 	ret
                            414 ;------------------------------------------------------------
                            415 ;Allocation info for local variables in function 'main'
                            416 ;------------------------------------------------------------
                            417 ;i                         Allocated to registers r6 r7 
                            418 ;i2                        Allocated to registers r4 r5 
                            419 ;------------------------------------------------------------
                            420 ;	test2.c:32: void main()
                            421 ;	-----------------------------------------
                            422 ;	 function main
                            423 ;	-----------------------------------------
   0090                     424 _main:
                            425 ;	test2.c:34: int i=0;
   0090 7E 00               426 	mov	r6,#0x00
   0092 7F 00               427 	mov	r7,#0x00
                            428 ;	test2.c:35: int i2=0;
   0094 7C 00               429 	mov	r4,#0x00
   0096 7D 00               430 	mov	r5,#0x00
                            431 ;	test2.c:37: P2=0xFE;
   0098 75 A0 FE            432 	mov	_P2,#0xFE
                            433 ;	test2.c:39: set_bit(P2,0,1);
   009B 43 A0 01            434 	orl	_P2,#0x01
                            435 ;	test2.c:40: while(1)
   009E                     436 00109$:
                            437 ;	test2.c:42: P1=~display_seg6[i];
   009E EE                  438 	mov	a,r6
   009F 24 40               439 	add	a,#_display_seg6
   00A1 F5 82               440 	mov	dpl,a
   00A3 EF                  441 	mov	a,r7
   00A4 34 01               442 	addc	a,#(_display_seg6 >> 8)
   00A6 F5 83               443 	mov	dph,a
   00A8 E4                  444 	clr	a
   00A9 93                  445 	movc	a,@a+dptr
   00AA FB                  446 	mov	r3,a
   00AB F4                  447 	cpl	a
   00AC F5 90               448 	mov	_P1,a
                            449 ;	test2.c:43: P0=display_seg[i2];
   00AE EC                  450 	mov	a,r4
   00AF 24 0F               451 	add	a,#_display_seg
   00B1 F5 82               452 	mov	dpl,a
   00B3 ED                  453 	mov	a,r5
   00B4 34 01               454 	addc	a,#(_display_seg >> 8)
   00B6 F5 83               455 	mov	dph,a
   00B8 E4                  456 	clr	a
   00B9 93                  457 	movc	a,@a+dptr
   00BA F5 80               458 	mov	_P0,a
                            459 ;	test2.c:44: while(!get_bit(P2,0));
   00BC                     460 00101$:
   00BC E5 A0               461 	mov	a,_P2
   00BE 30 E0 FB            462 	jnb	acc.0,00101$
                            463 ;	test2.c:46: i++;
   00C1 0E                  464 	inc	r6
   00C2 BE 00 01            465 	cjne	r6,#0x00,00119$
   00C5 0F                  466 	inc	r7
   00C6                     467 00119$:
                            468 ;	test2.c:47: /*if(tmp==0)*/i2++;
   00C6 0C                  469 	inc	r4
   00C7 BC 00 01            470 	cjne	r4,#0x00,00120$
   00CA 0D                  471 	inc	r5
   00CB                     472 00120$:
                            473 ;	test2.c:48: if(display_seg6[i]==0x11)i=0;
   00CB EE                  474 	mov	a,r6
   00CC 24 40               475 	add	a,#_display_seg6
   00CE F5 82               476 	mov	dpl,a
   00D0 EF                  477 	mov	a,r7
   00D1 34 01               478 	addc	a,#(_display_seg6 >> 8)
   00D3 F5 83               479 	mov	dph,a
   00D5 E4                  480 	clr	a
   00D6 93                  481 	movc	a,@a+dptr
   00D7 FB                  482 	mov	r3,a
   00D8 BB 11 04            483 	cjne	r3,#0x11,00105$
   00DB 7E 00               484 	mov	r6,#0x00
   00DD 7F 00               485 	mov	r7,#0x00
   00DF                     486 00105$:
                            487 ;	test2.c:49: if(i2>9)i2=0;
   00DF C3                  488 	clr	c
   00E0 74 09               489 	mov	a,#0x09
   00E2 9C                  490 	subb	a,r4
   00E3 E4                  491 	clr	a
   00E4 64 80               492 	xrl	a,#0x80
   00E6 8D F0               493 	mov	b,r5
   00E8 63 F0 80            494 	xrl	b,#0x80
   00EB 95 F0               495 	subb	a,b
   00ED 50 04               496 	jnc	00107$
   00EF 7C 00               497 	mov	r4,#0x00
   00F1 7D 00               498 	mov	r5,#0x00
   00F3                     499 00107$:
                            500 ;	test2.c:52: delay(200);
   00F3 90 00 C8            501 	mov	dptr,#0x00C8
   00F6 C0 07               502 	push	ar7
   00F8 C0 06               503 	push	ar6
   00FA C0 05               504 	push	ar5
   00FC C0 04               505 	push	ar4
   00FE 12 00 64            506 	lcall	_delay
   0101 D0 04               507 	pop	ar4
   0103 D0 05               508 	pop	ar5
   0105 D0 06               509 	pop	ar6
   0107 D0 07               510 	pop	ar7
   0109 80 93               511 	sjmp	00109$
                            512 	.area CSEG    (CODE)
                            513 	.area CONST   (CODE)
   010F                     514 _display_seg:
   010F C0                  515 	.db #0xC0	; 192
   0110 F9                  516 	.db #0xF9	; 249
   0111 A4                  517 	.db #0xA4	; 164
   0112 B0                  518 	.db #0xB0	; 176
   0113 99                  519 	.db #0x99	; 153
   0114 92                  520 	.db #0x92	; 146
   0115 82                  521 	.db #0x82	; 130
   0116 F8                  522 	.db #0xF8	; 248
   0117 80                  523 	.db #0x80	; 128
   0118 90                  524 	.db #0x90	; 144
   0119 7F                  525 	.db #0x7F	; 127
   011A                     526 _display_seg2:
   011A FE                  527 	.db #0xFE	; 254
   011B FD                  528 	.db #0xFD	; 253
   011C FB                  529 	.db #0xFB	; 251
   011D F7                  530 	.db #0xF7	; 247
   011E EF                  531 	.db #0xEF	; 239
   011F DF                  532 	.db #0xDF	; 223
   0120                     533 _display_seg3:
   0120 01                  534 	.db #0x01	; 1
   0121 02                  535 	.db #0x02	; 2
   0122 04                  536 	.db #0x04	; 4
   0123 08                  537 	.db #0x08	; 8
   0124 10                  538 	.db #0x10	; 16
   0125 20                  539 	.db #0x20	; 32
   0126 40                  540 	.db #0x40	; 64
   0127 80                  541 	.db #0x80	; 128
   0128                     542 _display_seg4:
   0128 FF                  543 	.db #0xFF	; 255
   0129 90                  544 	.db #0x90	; 144
   012A 90                  545 	.db #0x90	; 144
   012B 90                  546 	.db #0x90	; 144
   012C 00                  547 	.db #0x00	; 0
   012D FF                  548 	.db #0xFF	; 255
   012E 01                  549 	.db #0x01	; 1
   012F 01                  550 	.db #0x01	; 1
   0130 FF                  551 	.db #0xFF	; 255
   0131 00                  552 	.db #0x00	; 0
   0132 18                  553 	.db #0x18	; 24
   0133 24                  554 	.db #0x24	; 36
   0134 42                  555 	.db #0x42	; 66	'B'
   0135 81                  556 	.db #0x81	; 129
   0136 00                  557 	.db #0x00	; 0
   0137 FF                  558 	.db #0xFF	; 255
   0138 04                  559 	.db #0x04	; 4
   0139 0A                  560 	.db #0x0A	; 10
   013A 11                  561 	.db #0x11	; 17
   013B 00                  562 	.db #0x00	; 0
   013C                     563 _display_seg5:
   013C 88                  564 	.db #0x88	; 136
   013D 44                  565 	.db #0x44	; 68	'D'
   013E 22                  566 	.db #0x22	; 34
   013F 11                  567 	.db #0x11	; 17
   0140                     568 _display_seg6:
   0140 00                  569 	.db #0x00	; 0
   0141 01                  570 	.db #0x01	; 1
   0142 02                  571 	.db #0x02	; 2
   0143 04                  572 	.db #0x04	; 4
   0144 08                  573 	.db #0x08	; 8
   0145 10                  574 	.db #0x10	; 16
   0146 20                  575 	.db #0x20	; 32
   0147 40                  576 	.db #0x40	; 64
   0148 80                  577 	.db #0x80	; 128
   0149 81                  578 	.db #0x81	; 129
   014A 82                  579 	.db #0x82	; 130
   014B 84                  580 	.db #0x84	; 132
   014C 88                  581 	.db #0x88	; 136
   014D 90                  582 	.db #0x90	; 144
   014E A0                  583 	.db #0xA0	; 160
   014F C0                  584 	.db #0xC0	; 192
   0150 C1                  585 	.db #0xC1	; 193
   0151 C2                  586 	.db #0xC2	; 194
   0152 C4                  587 	.db #0xC4	; 196
   0153 C8                  588 	.db #0xC8	; 200
   0154 D0                  589 	.db #0xD0	; 208
   0155 E0                  590 	.db #0xE0	; 224
   0156 E1                  591 	.db #0xE1	; 225
   0157 E2                  592 	.db #0xE2	; 226
   0158 E4                  593 	.db #0xE4	; 228
   0159 E8                  594 	.db #0xE8	; 232
   015A F0                  595 	.db #0xF0	; 240
   015B F1                  596 	.db #0xF1	; 241
   015C F2                  597 	.db #0xF2	; 242
   015D F4                  598 	.db #0xF4	; 244
   015E F8                  599 	.db #0xF8	; 248
   015F F9                  600 	.db #0xF9	; 249
   0160 FA                  601 	.db #0xFA	; 250
   0161 FC                  602 	.db #0xFC	; 252
   0162 FD                  603 	.db #0xFD	; 253
   0163 FE                  604 	.db #0xFE	; 254
   0164 FF                  605 	.db #0xFF	; 255
   0165 11                  606 	.db #0x11	; 17
                            607 	.area XINIT   (CODE)
                            608 	.area CABS    (ABS,CODE)
