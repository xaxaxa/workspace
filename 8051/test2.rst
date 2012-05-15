                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
                              4 ; This file was generated Sun May 13 18:25:48 2012
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
                            142 	.area RSEG    (DATA)
                    0080    143 _P0	=	0x0080
                    0081    144 _SP	=	0x0081
                    0082    145 _DPL	=	0x0082
                    0083    146 _DPH	=	0x0083
                    0087    147 _PCON	=	0x0087
                    0088    148 _TCON	=	0x0088
                    0089    149 _TMOD	=	0x0089
                    008A    150 _TL0	=	0x008a
                    008B    151 _TL1	=	0x008b
                    008C    152 _TH0	=	0x008c
                    008D    153 _TH1	=	0x008d
                    0090    154 _P1	=	0x0090
                    0098    155 _SCON	=	0x0098
                    0099    156 _SBUF	=	0x0099
                    00A0    157 _P2	=	0x00a0
                    00A8    158 _IE	=	0x00a8
                    00B0    159 _P3	=	0x00b0
                    00B8    160 _IP	=	0x00b8
                    00D0    161 _PSW	=	0x00d0
                    00E0    162 _ACC	=	0x00e0
                    00F0    163 _B	=	0x00f0
                    00C8    164 _T2CON	=	0x00c8
                    00CA    165 _RCAP2L	=	0x00ca
                    00CB    166 _RCAP2H	=	0x00cb
                    00CC    167 _TL2	=	0x00cc
                    00CD    168 _TH2	=	0x00cd
                            169 ;--------------------------------------------------------
                            170 ; special function bits
                            171 ;--------------------------------------------------------
                            172 	.area RSEG    (DATA)
                    0080    173 _P0_0	=	0x0080
                    0081    174 _P0_1	=	0x0081
                    0082    175 _P0_2	=	0x0082
                    0083    176 _P0_3	=	0x0083
                    0084    177 _P0_4	=	0x0084
                    0085    178 _P0_5	=	0x0085
                    0086    179 _P0_6	=	0x0086
                    0087    180 _P0_7	=	0x0087
                    0088    181 _IT0	=	0x0088
                    0089    182 _IE0	=	0x0089
                    008A    183 _IT1	=	0x008a
                    008B    184 _IE1	=	0x008b
                    008C    185 _TR0	=	0x008c
                    008D    186 _TF0	=	0x008d
                    008E    187 _TR1	=	0x008e
                    008F    188 _TF1	=	0x008f
                    0090    189 _P1_0	=	0x0090
                    0091    190 _P1_1	=	0x0091
                    0092    191 _P1_2	=	0x0092
                    0093    192 _P1_3	=	0x0093
                    0094    193 _P1_4	=	0x0094
                    0095    194 _P1_5	=	0x0095
                    0096    195 _P1_6	=	0x0096
                    0097    196 _P1_7	=	0x0097
                    0098    197 _RI	=	0x0098
                    0099    198 _TI	=	0x0099
                    009A    199 _RB8	=	0x009a
                    009B    200 _TB8	=	0x009b
                    009C    201 _REN	=	0x009c
                    009D    202 _SM2	=	0x009d
                    009E    203 _SM1	=	0x009e
                    009F    204 _SM0	=	0x009f
                    00A0    205 _P2_0	=	0x00a0
                    00A1    206 _P2_1	=	0x00a1
                    00A2    207 _P2_2	=	0x00a2
                    00A3    208 _P2_3	=	0x00a3
                    00A4    209 _P2_4	=	0x00a4
                    00A5    210 _P2_5	=	0x00a5
                    00A6    211 _P2_6	=	0x00a6
                    00A7    212 _P2_7	=	0x00a7
                    00A8    213 _EX0	=	0x00a8
                    00A9    214 _ET0	=	0x00a9
                    00AA    215 _EX1	=	0x00aa
                    00AB    216 _ET1	=	0x00ab
                    00AC    217 _ES	=	0x00ac
                    00AF    218 _EA	=	0x00af
                    00B0    219 _P3_0	=	0x00b0
                    00B1    220 _P3_1	=	0x00b1
                    00B2    221 _P3_2	=	0x00b2
                    00B3    222 _P3_3	=	0x00b3
                    00B4    223 _P3_4	=	0x00b4
                    00B5    224 _P3_5	=	0x00b5
                    00B6    225 _P3_6	=	0x00b6
                    00B7    226 _P3_7	=	0x00b7
                    00B0    227 _RXD	=	0x00b0
                    00B1    228 _TXD	=	0x00b1
                    00B2    229 _INT0	=	0x00b2
                    00B3    230 _INT1	=	0x00b3
                    00B4    231 _T0	=	0x00b4
                    00B5    232 _T1	=	0x00b5
                    00B6    233 _WR	=	0x00b6
                    00B7    234 _RD	=	0x00b7
                    00B8    235 _PX0	=	0x00b8
                    00B9    236 _PT0	=	0x00b9
                    00BA    237 _PX1	=	0x00ba
                    00BB    238 _PT1	=	0x00bb
                    00BC    239 _PS	=	0x00bc
                    00D0    240 _P	=	0x00d0
                    00D1    241 _F1	=	0x00d1
                    00D2    242 _OV	=	0x00d2
                    00D3    243 _RS0	=	0x00d3
                    00D4    244 _RS1	=	0x00d4
                    00D5    245 _F0	=	0x00d5
                    00D6    246 _AC	=	0x00d6
                    00D7    247 _CY	=	0x00d7
                    00AD    248 _ET2	=	0x00ad
                    00BD    249 _PT2	=	0x00bd
                    00C8    250 _T2CON_0	=	0x00c8
                    00C9    251 _T2CON_1	=	0x00c9
                    00CA    252 _T2CON_2	=	0x00ca
                    00CB    253 _T2CON_3	=	0x00cb
                    00CC    254 _T2CON_4	=	0x00cc
                    00CD    255 _T2CON_5	=	0x00cd
                    00CE    256 _T2CON_6	=	0x00ce
                    00CF    257 _T2CON_7	=	0x00cf
                    00C8    258 _CP_RL2	=	0x00c8
                    00C9    259 _C_T2	=	0x00c9
                    00CA    260 _TR2	=	0x00ca
                    00CB    261 _EXEN2	=	0x00cb
                    00CC    262 _TCLK	=	0x00cc
                    00CD    263 _RCLK	=	0x00cd
                    00CE    264 _EXF2	=	0x00ce
                    00CF    265 _TF2	=	0x00cf
                            266 ;--------------------------------------------------------
                            267 ; overlayable register banks
                            268 ;--------------------------------------------------------
                            269 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     270 	.ds 8
                            271 ;--------------------------------------------------------
                            272 ; internal ram data
                            273 ;--------------------------------------------------------
                            274 	.area DSEG    (DATA)
                            275 ;--------------------------------------------------------
                            276 ; overlayable items in internal ram 
                            277 ;--------------------------------------------------------
                            278 	.area	OSEG    (OVR,DATA)
                            279 ;--------------------------------------------------------
                            280 ; Stack segment in internal ram 
                            281 ;--------------------------------------------------------
                            282 	.area	SSEG	(DATA)
   0008                     283 __start__stack:
   0008                     284 	.ds	1
                            285 
                            286 ;--------------------------------------------------------
                            287 ; indirectly addressable internal ram data
                            288 ;--------------------------------------------------------
                            289 	.area ISEG    (DATA)
                            290 ;--------------------------------------------------------
                            291 ; absolute internal ram data
                            292 ;--------------------------------------------------------
                            293 	.area IABS    (ABS,DATA)
                            294 	.area IABS    (ABS,DATA)
                            295 ;--------------------------------------------------------
                            296 ; bit data
                            297 ;--------------------------------------------------------
                            298 	.area BSEG    (BIT)
                            299 ;--------------------------------------------------------
                            300 ; paged external ram data
                            301 ;--------------------------------------------------------
                            302 	.area PSEG    (PAG,XDATA)
                            303 ;--------------------------------------------------------
                            304 ; external ram data
                            305 ;--------------------------------------------------------
                            306 	.area XSEG    (XDATA)
                            307 ;--------------------------------------------------------
                            308 ; absolute external ram data
                            309 ;--------------------------------------------------------
                            310 	.area XABS    (ABS,XDATA)
                            311 ;--------------------------------------------------------
                            312 ; external initialized ram data
                            313 ;--------------------------------------------------------
                            314 	.area XISEG   (XDATA)
                            315 	.area HOME    (CODE)
                            316 	.area GSINIT0 (CODE)
                            317 	.area GSINIT1 (CODE)
                            318 	.area GSINIT2 (CODE)
                            319 	.area GSINIT3 (CODE)
                            320 	.area GSINIT4 (CODE)
                            321 	.area GSINIT5 (CODE)
                            322 	.area GSINIT  (CODE)
                            323 	.area GSFINAL (CODE)
                            324 	.area CSEG    (CODE)
                            325 ;--------------------------------------------------------
                            326 ; interrupt vector 
                            327 ;--------------------------------------------------------
                            328 	.area HOME    (CODE)
   0000                     329 __interrupt_vect:
   0000 02 00 08            330 	ljmp	__sdcc_gsinit_startup
                            331 ;--------------------------------------------------------
                            332 ; global & static initialisations
                            333 ;--------------------------------------------------------
                            334 	.area HOME    (CODE)
                            335 	.area GSINIT  (CODE)
                            336 	.area GSFINAL (CODE)
                            337 	.area GSINIT  (CODE)
                            338 	.globl __sdcc_gsinit_startup
                            339 	.globl __sdcc_program_startup
                            340 	.globl __start__stack
                            341 	.globl __mcs51_genXINIT
                            342 	.globl __mcs51_genXRAMCLEAR
                            343 	.globl __mcs51_genRAMCLEAR
                            344 	.area GSFINAL (CODE)
   0061 02 00 03            345 	ljmp	__sdcc_program_startup
                            346 ;--------------------------------------------------------
                            347 ; Home
                            348 ;--------------------------------------------------------
                            349 	.area HOME    (CODE)
                            350 	.area HOME    (CODE)
   0003                     351 __sdcc_program_startup:
   0003 12 00 90            352 	lcall	_main
                            353 ;	return from main will lock up
   0006 80 FE               354 	sjmp .
                            355 ;--------------------------------------------------------
                            356 ; code
                            357 ;--------------------------------------------------------
                            358 	.area CSEG    (CODE)
                            359 ;------------------------------------------------------------
                            360 ;Allocation info for local variables in function 'delay'
                            361 ;------------------------------------------------------------
                            362 ;i                         Allocated to registers r2 r3 
                            363 ;a                         Allocated to registers r4 r5 
                            364 ;b                         Allocated to registers r6 r7 
                            365 ;------------------------------------------------------------
                            366 ;	test2.c:16: void delay(int i)
                            367 ;	-----------------------------------------
                            368 ;	 function delay
                            369 ;	-----------------------------------------
   0064                     370 _delay:
                    0002    371 	ar2 = 0x02
                    0003    372 	ar3 = 0x03
                    0004    373 	ar4 = 0x04
                    0005    374 	ar5 = 0x05
                    0006    375 	ar6 = 0x06
                    0007    376 	ar7 = 0x07
                    0000    377 	ar0 = 0x00
                    0001    378 	ar1 = 0x01
   0064 AA 82               379 	mov	r2,dpl
   0066 AB 83               380 	mov	r3,dph
                            381 ;	test2.c:19: for(a=0;a<i;a++)
   0068 7C 00               382 	mov	r4,#0x00
   006A 7D 00               383 	mov	r5,#0x00
   006C                     384 00104$:
   006C C3                  385 	clr	c
   006D EC                  386 	mov	a,r4
   006E 9A                  387 	subb	a,r2
   006F ED                  388 	mov	a,r5
   0070 64 80               389 	xrl	a,#0x80
   0072 8B F0               390 	mov	b,r3
   0074 63 F0 80            391 	xrl	b,#0x80
   0077 95 F0               392 	subb	a,b
   0079 50 14               393 	jnc	00108$
                            394 ;	test2.c:21: for(b=0;b<120;b++);
   007B 7E 78               395 	mov	r6,#0x78
   007D 7F 00               396 	mov	r7,#0x00
   007F                     397 00103$:
   007F 1E                  398 	dec	r6
   0080 BE FF 01            399 	cjne	r6,#0xff,00117$
   0083 1F                  400 	dec	r7
   0084                     401 00117$:
   0084 EE                  402 	mov	a,r6
   0085 4F                  403 	orl	a,r7
   0086 70 F7               404 	jnz	00103$
                            405 ;	test2.c:19: for(a=0;a<i;a++)
   0088 0C                  406 	inc	r4
   0089 BC 00 E0            407 	cjne	r4,#0x00,00104$
   008C 0D                  408 	inc	r5
   008D 80 DD               409 	sjmp	00104$
   008F                     410 00108$:
   008F 22                  411 	ret
                            412 ;------------------------------------------------------------
                            413 ;Allocation info for local variables in function 'main'
                            414 ;------------------------------------------------------------
                            415 ;i                         Allocated to registers r2 r3 
                            416 ;i2                        Allocated to registers r4 r5 
                            417 ;------------------------------------------------------------
                            418 ;	test2.c:32: void main()
                            419 ;	-----------------------------------------
                            420 ;	 function main
                            421 ;	-----------------------------------------
   0090                     422 _main:
                            423 ;	test2.c:34: int i=0;
   0090 7A 00               424 	mov	r2,#0x00
   0092 7B 00               425 	mov	r3,#0x00
                            426 ;	test2.c:35: int i2=0;
   0094 7C 00               427 	mov	r4,#0x00
   0096 7D 00               428 	mov	r5,#0x00
                            429 ;	test2.c:37: P2=0xFE;
   0098 75 A0 FE            430 	mov	_P2,#0xFE
                            431 ;	test2.c:39: set_bit(P2,0,1);
   009B 43 A0 01            432 	orl	_P2,#0x01
                            433 ;	test2.c:40: while(1)
   009E                     434 00109$:
                            435 ;	test2.c:42: P1=~display_seg6[i];
   009E EA                  436 	mov	a,r2
   009F 24 3F               437 	add	a,#_display_seg6
   00A1 F5 82               438 	mov	dpl,a
   00A3 EB                  439 	mov	a,r3
   00A4 34 01               440 	addc	a,#(_display_seg6 >> 8)
   00A6 F5 83               441 	mov	dph,a
   00A8 E4                  442 	clr	a
   00A9 93                  443 	movc	a,@a+dptr
   00AA FE                  444 	mov	r6,a
   00AB F4                  445 	cpl	a
   00AC F5 90               446 	mov	_P1,a
                            447 ;	test2.c:43: P0=display_seg[i2];
   00AE EC                  448 	mov	a,r4
   00AF 24 0E               449 	add	a,#_display_seg
   00B1 F5 82               450 	mov	dpl,a
   00B3 ED                  451 	mov	a,r5
   00B4 34 01               452 	addc	a,#(_display_seg >> 8)
   00B6 F5 83               453 	mov	dph,a
   00B8 E4                  454 	clr	a
   00B9 93                  455 	movc	a,@a+dptr
   00BA F5 80               456 	mov	_P0,a
                            457 ;	test2.c:44: while(!get_bit(P2,0));
   00BC                     458 00101$:
   00BC E5 A0               459 	mov	a,_P2
   00BE 30 E0 FB            460 	jnb	acc.0,00101$
                            461 ;	test2.c:46: i++;
   00C1 0A                  462 	inc	r2
   00C2 BA 00 01            463 	cjne	r2,#0x00,00119$
   00C5 0B                  464 	inc	r3
   00C6                     465 00119$:
                            466 ;	test2.c:47: /*if(tmp==0)*/i2++;
   00C6 0C                  467 	inc	r4
   00C7 BC 00 01            468 	cjne	r4,#0x00,00120$
   00CA 0D                  469 	inc	r5
   00CB                     470 00120$:
                            471 ;	test2.c:48: if(display_seg6[i]==0x11)i=0;
   00CB EA                  472 	mov	a,r2
   00CC 24 3F               473 	add	a,#_display_seg6
   00CE F5 82               474 	mov	dpl,a
   00D0 EB                  475 	mov	a,r3
   00D1 34 01               476 	addc	a,#(_display_seg6 >> 8)
   00D3 F5 83               477 	mov	dph,a
   00D5 E4                  478 	clr	a
   00D6 93                  479 	movc	a,@a+dptr
   00D7 FE                  480 	mov	r6,a
   00D8 BE 11 04            481 	cjne	r6,#0x11,00105$
   00DB 7A 00               482 	mov	r2,#0x00
   00DD 7B 00               483 	mov	r3,#0x00
   00DF                     484 00105$:
                            485 ;	test2.c:49: if(i2>9)i2=0;
   00DF C3                  486 	clr	c
   00E0 74 09               487 	mov	a,#0x09
   00E2 9C                  488 	subb	a,r4
   00E3 74 80               489 	mov	a,#(0x00 ^ 0x80)
   00E5 8D F0               490 	mov	b,r5
   00E7 63 F0 80            491 	xrl	b,#0x80
   00EA 95 F0               492 	subb	a,b
   00EC 50 04               493 	jnc	00107$
   00EE 7C 00               494 	mov	r4,#0x00
   00F0 7D 00               495 	mov	r5,#0x00
   00F2                     496 00107$:
                            497 ;	test2.c:52: delay(200);
   00F2 90 00 C8            498 	mov	dptr,#0x00C8
   00F5 C0 02               499 	push	ar2
   00F7 C0 03               500 	push	ar3
   00F9 C0 04               501 	push	ar4
   00FB C0 05               502 	push	ar5
   00FD 12 00 64            503 	lcall	_delay
   0100 D0 05               504 	pop	ar5
   0102 D0 04               505 	pop	ar4
   0104 D0 03               506 	pop	ar3
   0106 D0 02               507 	pop	ar2
   0108 80 94               508 	sjmp	00109$
                            509 	.area CSEG    (CODE)
                            510 	.area CONST   (CODE)
   010E                     511 _display_seg:
   010E C0                  512 	.db #0xC0
   010F F9                  513 	.db #0xF9
   0110 A4                  514 	.db #0xA4
   0111 B0                  515 	.db #0xB0
   0112 99                  516 	.db #0x99
   0113 92                  517 	.db #0x92
   0114 82                  518 	.db #0x82
   0115 F8                  519 	.db #0xF8
   0116 80                  520 	.db #0x80
   0117 90                  521 	.db #0x90
   0118 7F                  522 	.db #0x7F
   0119                     523 _display_seg2:
   0119 FE                  524 	.db #0xFE
   011A FD                  525 	.db #0xFD
   011B FB                  526 	.db #0xFB
   011C F7                  527 	.db #0xF7
   011D EF                  528 	.db #0xEF
   011E DF                  529 	.db #0xDF
   011F                     530 _display_seg3:
   011F 01                  531 	.db #0x01
   0120 02                  532 	.db #0x02
   0121 04                  533 	.db #0x04
   0122 08                  534 	.db #0x08
   0123 10                  535 	.db #0x10
   0124 20                  536 	.db #0x20
   0125 40                  537 	.db #0x40
   0126 80                  538 	.db #0x80
   0127                     539 _display_seg4:
   0127 FF                  540 	.db #0xFF
   0128 90                  541 	.db #0x90
   0129 90                  542 	.db #0x90
   012A 90                  543 	.db #0x90
   012B 00                  544 	.db #0x00
   012C FF                  545 	.db #0xFF
   012D 01                  546 	.db #0x01
   012E 01                  547 	.db #0x01
   012F FF                  548 	.db #0xFF
   0130 00                  549 	.db #0x00
   0131 18                  550 	.db #0x18
   0132 24                  551 	.db #0x24
   0133 42                  552 	.db #0x42
   0134 81                  553 	.db #0x81
   0135 00                  554 	.db #0x00
   0136 FF                  555 	.db #0xFF
   0137 04                  556 	.db #0x04
   0138 0A                  557 	.db #0x0A
   0139 11                  558 	.db #0x11
   013A 00                  559 	.db #0x00
   013B                     560 _display_seg5:
   013B 88                  561 	.db #0x88
   013C 44                  562 	.db #0x44
   013D 22                  563 	.db #0x22
   013E 11                  564 	.db #0x11
   013F                     565 _display_seg6:
   013F 00                  566 	.db #0x00
   0140 01                  567 	.db #0x01
   0141 02                  568 	.db #0x02
   0142 04                  569 	.db #0x04
   0143 08                  570 	.db #0x08
   0144 10                  571 	.db #0x10
   0145 20                  572 	.db #0x20
   0146 40                  573 	.db #0x40
   0147 80                  574 	.db #0x80
   0148 81                  575 	.db #0x81
   0149 82                  576 	.db #0x82
   014A 84                  577 	.db #0x84
   014B 88                  578 	.db #0x88
   014C 90                  579 	.db #0x90
   014D A0                  580 	.db #0xA0
   014E C0                  581 	.db #0xC0
   014F C1                  582 	.db #0xC1
   0150 C2                  583 	.db #0xC2
   0151 C4                  584 	.db #0xC4
   0152 C8                  585 	.db #0xC8
   0153 D0                  586 	.db #0xD0
   0154 E0                  587 	.db #0xE0
   0155 E1                  588 	.db #0xE1
   0156 E2                  589 	.db #0xE2
   0157 E4                  590 	.db #0xE4
   0158 E8                  591 	.db #0xE8
   0159 F0                  592 	.db #0xF0
   015A F1                  593 	.db #0xF1
   015B F2                  594 	.db #0xF2
   015C F4                  595 	.db #0xF4
   015D F8                  596 	.db #0xF8
   015E F9                  597 	.db #0xF9
   015F FA                  598 	.db #0xFA
   0160 FC                  599 	.db #0xFC
   0161 FD                  600 	.db #0xFD
   0162 FE                  601 	.db #0xFE
   0163 FF                  602 	.db #0xFF
   0164 11                  603 	.db #0x11
                            604 	.area XINIT   (CODE)
                            605 	.area CABS    (ABS,CODE)
