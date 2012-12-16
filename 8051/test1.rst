                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 3.1.0 #7066 (Feb 26 2012) (Linux)
                              4 ; This file was generated Sat Dec  1 15:06:17 2012
                              5 ;--------------------------------------------------------
                              6 	.module test1
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _seg_position
                             13 	.globl _display_seg
                             14 	.globl _s
                             15 	.globl _time
                             16 	.globl _t
                             17 	.globl _c
                             18 	.globl _b
                             19 	.globl _main
                             20 	.globl _display
                             21 	.globl _delay
                             22 	.globl _TF2
                             23 	.globl _EXF2
                             24 	.globl _RCLK
                             25 	.globl _TCLK
                             26 	.globl _EXEN2
                             27 	.globl _TR2
                             28 	.globl _C_T2
                             29 	.globl _CP_RL2
                             30 	.globl _T2CON_7
                             31 	.globl _T2CON_6
                             32 	.globl _T2CON_5
                             33 	.globl _T2CON_4
                             34 	.globl _T2CON_3
                             35 	.globl _T2CON_2
                             36 	.globl _T2CON_1
                             37 	.globl _T2CON_0
                             38 	.globl _PT2
                             39 	.globl _ET2
                             40 	.globl _CY
                             41 	.globl _AC
                             42 	.globl _F0
                             43 	.globl _RS1
                             44 	.globl _RS0
                             45 	.globl _OV
                             46 	.globl _F1
                             47 	.globl _P
                             48 	.globl _PS
                             49 	.globl _PT1
                             50 	.globl _PX1
                             51 	.globl _PT0
                             52 	.globl _PX0
                             53 	.globl _RD
                             54 	.globl _WR
                             55 	.globl _T1
                             56 	.globl _T0
                             57 	.globl _INT1
                             58 	.globl _INT0
                             59 	.globl _TXD
                             60 	.globl _RXD
                             61 	.globl _P3_7
                             62 	.globl _P3_6
                             63 	.globl _P3_5
                             64 	.globl _P3_4
                             65 	.globl _P3_3
                             66 	.globl _P3_2
                             67 	.globl _P3_1
                             68 	.globl _P3_0
                             69 	.globl _EA
                             70 	.globl _ES
                             71 	.globl _ET1
                             72 	.globl _EX1
                             73 	.globl _ET0
                             74 	.globl _EX0
                             75 	.globl _P2_7
                             76 	.globl _P2_6
                             77 	.globl _P2_5
                             78 	.globl _P2_4
                             79 	.globl _P2_3
                             80 	.globl _P2_2
                             81 	.globl _P2_1
                             82 	.globl _P2_0
                             83 	.globl _SM0
                             84 	.globl _SM1
                             85 	.globl _SM2
                             86 	.globl _REN
                             87 	.globl _TB8
                             88 	.globl _RB8
                             89 	.globl _TI
                             90 	.globl _RI
                             91 	.globl _P1_7
                             92 	.globl _P1_6
                             93 	.globl _P1_5
                             94 	.globl _P1_4
                             95 	.globl _P1_3
                             96 	.globl _P1_2
                             97 	.globl _P1_1
                             98 	.globl _P1_0
                             99 	.globl _TF1
                            100 	.globl _TR1
                            101 	.globl _TF0
                            102 	.globl _TR0
                            103 	.globl _IE1
                            104 	.globl _IT1
                            105 	.globl _IE0
                            106 	.globl _IT0
                            107 	.globl _P0_7
                            108 	.globl _P0_6
                            109 	.globl _P0_5
                            110 	.globl _P0_4
                            111 	.globl _P0_3
                            112 	.globl _P0_2
                            113 	.globl _P0_1
                            114 	.globl _P0_0
                            115 	.globl _TH2
                            116 	.globl _TL2
                            117 	.globl _RCAP2H
                            118 	.globl _RCAP2L
                            119 	.globl _T2CON
                            120 	.globl _B
                            121 	.globl _ACC
                            122 	.globl _PSW
                            123 	.globl _IP
                            124 	.globl _P3
                            125 	.globl _IE
                            126 	.globl _P2
                            127 	.globl _SBUF
                            128 	.globl _SCON
                            129 	.globl _P1
                            130 	.globl _TH1
                            131 	.globl _TH0
                            132 	.globl _TL1
                            133 	.globl _TL0
                            134 	.globl _TMOD
                            135 	.globl _TCON
                            136 	.globl _PCON
                            137 	.globl _DPH
                            138 	.globl _DPL
                            139 	.globl _SP
                            140 	.globl _P0
                            141 	.globl __c
                            142 	.globl _i
                            143 	.globl _count
                            144 	.globl _t_l
                            145 	.globl _t_h
                            146 ;--------------------------------------------------------
                            147 ; special function registers
                            148 ;--------------------------------------------------------
                            149 	.area RSEG    (ABS,DATA)
   0000                     150 	.org 0x0000
                    0080    151 _P0	=	0x0080
                    0081    152 _SP	=	0x0081
                    0082    153 _DPL	=	0x0082
                    0083    154 _DPH	=	0x0083
                    0087    155 _PCON	=	0x0087
                    0088    156 _TCON	=	0x0088
                    0089    157 _TMOD	=	0x0089
                    008A    158 _TL0	=	0x008a
                    008B    159 _TL1	=	0x008b
                    008C    160 _TH0	=	0x008c
                    008D    161 _TH1	=	0x008d
                    0090    162 _P1	=	0x0090
                    0098    163 _SCON	=	0x0098
                    0099    164 _SBUF	=	0x0099
                    00A0    165 _P2	=	0x00a0
                    00A8    166 _IE	=	0x00a8
                    00B0    167 _P3	=	0x00b0
                    00B8    168 _IP	=	0x00b8
                    00D0    169 _PSW	=	0x00d0
                    00E0    170 _ACC	=	0x00e0
                    00F0    171 _B	=	0x00f0
                    00C8    172 _T2CON	=	0x00c8
                    00CA    173 _RCAP2L	=	0x00ca
                    00CB    174 _RCAP2H	=	0x00cb
                    00CC    175 _TL2	=	0x00cc
                    00CD    176 _TH2	=	0x00cd
                            177 ;--------------------------------------------------------
                            178 ; special function bits
                            179 ;--------------------------------------------------------
                            180 	.area RSEG    (ABS,DATA)
   0000                     181 	.org 0x0000
                    0080    182 _P0_0	=	0x0080
                    0081    183 _P0_1	=	0x0081
                    0082    184 _P0_2	=	0x0082
                    0083    185 _P0_3	=	0x0083
                    0084    186 _P0_4	=	0x0084
                    0085    187 _P0_5	=	0x0085
                    0086    188 _P0_6	=	0x0086
                    0087    189 _P0_7	=	0x0087
                    0088    190 _IT0	=	0x0088
                    0089    191 _IE0	=	0x0089
                    008A    192 _IT1	=	0x008a
                    008B    193 _IE1	=	0x008b
                    008C    194 _TR0	=	0x008c
                    008D    195 _TF0	=	0x008d
                    008E    196 _TR1	=	0x008e
                    008F    197 _TF1	=	0x008f
                    0090    198 _P1_0	=	0x0090
                    0091    199 _P1_1	=	0x0091
                    0092    200 _P1_2	=	0x0092
                    0093    201 _P1_3	=	0x0093
                    0094    202 _P1_4	=	0x0094
                    0095    203 _P1_5	=	0x0095
                    0096    204 _P1_6	=	0x0096
                    0097    205 _P1_7	=	0x0097
                    0098    206 _RI	=	0x0098
                    0099    207 _TI	=	0x0099
                    009A    208 _RB8	=	0x009a
                    009B    209 _TB8	=	0x009b
                    009C    210 _REN	=	0x009c
                    009D    211 _SM2	=	0x009d
                    009E    212 _SM1	=	0x009e
                    009F    213 _SM0	=	0x009f
                    00A0    214 _P2_0	=	0x00a0
                    00A1    215 _P2_1	=	0x00a1
                    00A2    216 _P2_2	=	0x00a2
                    00A3    217 _P2_3	=	0x00a3
                    00A4    218 _P2_4	=	0x00a4
                    00A5    219 _P2_5	=	0x00a5
                    00A6    220 _P2_6	=	0x00a6
                    00A7    221 _P2_7	=	0x00a7
                    00A8    222 _EX0	=	0x00a8
                    00A9    223 _ET0	=	0x00a9
                    00AA    224 _EX1	=	0x00aa
                    00AB    225 _ET1	=	0x00ab
                    00AC    226 _ES	=	0x00ac
                    00AF    227 _EA	=	0x00af
                    00B0    228 _P3_0	=	0x00b0
                    00B1    229 _P3_1	=	0x00b1
                    00B2    230 _P3_2	=	0x00b2
                    00B3    231 _P3_3	=	0x00b3
                    00B4    232 _P3_4	=	0x00b4
                    00B5    233 _P3_5	=	0x00b5
                    00B6    234 _P3_6	=	0x00b6
                    00B7    235 _P3_7	=	0x00b7
                    00B0    236 _RXD	=	0x00b0
                    00B1    237 _TXD	=	0x00b1
                    00B2    238 _INT0	=	0x00b2
                    00B3    239 _INT1	=	0x00b3
                    00B4    240 _T0	=	0x00b4
                    00B5    241 _T1	=	0x00b5
                    00B6    242 _WR	=	0x00b6
                    00B7    243 _RD	=	0x00b7
                    00B8    244 _PX0	=	0x00b8
                    00B9    245 _PT0	=	0x00b9
                    00BA    246 _PX1	=	0x00ba
                    00BB    247 _PT1	=	0x00bb
                    00BC    248 _PS	=	0x00bc
                    00D0    249 _P	=	0x00d0
                    00D1    250 _F1	=	0x00d1
                    00D2    251 _OV	=	0x00d2
                    00D3    252 _RS0	=	0x00d3
                    00D4    253 _RS1	=	0x00d4
                    00D5    254 _F0	=	0x00d5
                    00D6    255 _AC	=	0x00d6
                    00D7    256 _CY	=	0x00d7
                    00AD    257 _ET2	=	0x00ad
                    00BD    258 _PT2	=	0x00bd
                    00C8    259 _T2CON_0	=	0x00c8
                    00C9    260 _T2CON_1	=	0x00c9
                    00CA    261 _T2CON_2	=	0x00ca
                    00CB    262 _T2CON_3	=	0x00cb
                    00CC    263 _T2CON_4	=	0x00cc
                    00CD    264 _T2CON_5	=	0x00cd
                    00CE    265 _T2CON_6	=	0x00ce
                    00CF    266 _T2CON_7	=	0x00cf
                    00C8    267 _CP_RL2	=	0x00c8
                    00C9    268 _C_T2	=	0x00c9
                    00CA    269 _TR2	=	0x00ca
                    00CB    270 _EXEN2	=	0x00cb
                    00CC    271 _TCLK	=	0x00cc
                    00CD    272 _RCLK	=	0x00cd
                    00CE    273 _EXF2	=	0x00ce
                    00CF    274 _TF2	=	0x00cf
                            275 ;--------------------------------------------------------
                            276 ; overlayable register banks
                            277 ;--------------------------------------------------------
                            278 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     279 	.ds 8
                            280 ;--------------------------------------------------------
                            281 ; overlayable bit register bank
                            282 ;--------------------------------------------------------
                            283 	.area BIT_BANK	(REL,OVR,DATA)
   0020                     284 bits:
   0020                     285 	.ds 1
                    8000    286 	b0 = bits[0]
                    8100    287 	b1 = bits[1]
                    8200    288 	b2 = bits[2]
                    8300    289 	b3 = bits[3]
                    8400    290 	b4 = bits[4]
                    8500    291 	b5 = bits[5]
                    8600    292 	b6 = bits[6]
                    8700    293 	b7 = bits[7]
                            294 ;--------------------------------------------------------
                            295 ; internal ram data
                            296 ;--------------------------------------------------------
                            297 	.area DSEG    (DATA)
   0008                     298 _t_h::
   0008                     299 	.ds 1
   0009                     300 _t_l::
   0009                     301 	.ds 1
   000A                     302 _count::
   000A                     303 	.ds 2
   000C                     304 _i::
   000C                     305 	.ds 2
   000E                     306 __c::
   000E                     307 	.ds 2
   0010                     308 _c_sloc0_1_0:
   0010                     309 	.ds 4
                            310 ;--------------------------------------------------------
                            311 ; overlayable items in internal ram 
                            312 ;--------------------------------------------------------
                            313 	.area	OSEG    (OVR,DATA)
                            314 ;--------------------------------------------------------
                            315 ; Stack segment in internal ram 
                            316 ;--------------------------------------------------------
                            317 	.area	SSEG	(DATA)
   0021                     318 __start__stack:
   0021                     319 	.ds	1
                            320 
                            321 ;--------------------------------------------------------
                            322 ; indirectly addressable internal ram data
                            323 ;--------------------------------------------------------
                            324 	.area ISEG    (DATA)
                            325 ;--------------------------------------------------------
                            326 ; absolute internal ram data
                            327 ;--------------------------------------------------------
                            328 	.area IABS    (ABS,DATA)
                            329 	.area IABS    (ABS,DATA)
                            330 ;--------------------------------------------------------
                            331 ; bit data
                            332 ;--------------------------------------------------------
                            333 	.area BSEG    (BIT)
                            334 ;--------------------------------------------------------
                            335 ; paged external ram data
                            336 ;--------------------------------------------------------
                            337 	.area PSEG    (PAG,XDATA)
                            338 ;--------------------------------------------------------
                            339 ; external ram data
                            340 ;--------------------------------------------------------
                            341 	.area XSEG    (XDATA)
                            342 ;--------------------------------------------------------
                            343 ; absolute external ram data
                            344 ;--------------------------------------------------------
                            345 	.area XABS    (ABS,XDATA)
                            346 ;--------------------------------------------------------
                            347 ; external initialized ram data
                            348 ;--------------------------------------------------------
                            349 	.area XISEG   (XDATA)
                            350 	.area HOME    (CODE)
                            351 	.area GSINIT0 (CODE)
                            352 	.area GSINIT1 (CODE)
                            353 	.area GSINIT2 (CODE)
                            354 	.area GSINIT3 (CODE)
                            355 	.area GSINIT4 (CODE)
                            356 	.area GSINIT5 (CODE)
                            357 	.area GSINIT  (CODE)
                            358 	.area GSFINAL (CODE)
                            359 	.area CSEG    (CODE)
                            360 ;--------------------------------------------------------
                            361 ; interrupt vector 
                            362 ;--------------------------------------------------------
                            363 	.area HOME    (CODE)
   0000                     364 __interrupt_vect:
   0000 02 00 23            365 	ljmp	__sdcc_gsinit_startup
   0003 32                  366 	reti
   0004                     367 	.ds	7
   000B 02 01 C1            368 	ljmp	_b
   000E                     369 	.ds	5
   0013 32                  370 	reti
   0014                     371 	.ds	7
   001B 02 01 F7            372 	ljmp	_c
                            373 ;--------------------------------------------------------
                            374 ; global & static initialisations
                            375 ;--------------------------------------------------------
                            376 	.area HOME    (CODE)
                            377 	.area GSINIT  (CODE)
                            378 	.area GSFINAL (CODE)
                            379 	.area GSINIT  (CODE)
                            380 	.globl __sdcc_gsinit_startup
                            381 	.globl __sdcc_program_startup
                            382 	.globl __start__stack
                            383 	.globl __mcs51_genXINIT
                            384 	.globl __mcs51_genXRAMCLEAR
                            385 	.globl __mcs51_genRAMCLEAR
                            386 ;	test1.c:31: int count=29;//33;
   007C 75 0A 1D            387 	mov	_count,#0x1D
   007F 75 0B 00            388 	mov	(_count + 1),#0x00
                            389 	.area GSFINAL (CODE)
   0082 02 00 1E            390 	ljmp	__sdcc_program_startup
                            391 ;--------------------------------------------------------
                            392 ; Home
                            393 ;--------------------------------------------------------
                            394 	.area HOME    (CODE)
                            395 	.area HOME    (CODE)
   001E                     396 __sdcc_program_startup:
   001E 12 01 74            397 	lcall	_main
                            398 ;	return from main will lock up
   0021 80 FE               399 	sjmp .
                            400 ;--------------------------------------------------------
                            401 ; code
                            402 ;--------------------------------------------------------
                            403 	.area CSEG    (CODE)
                            404 ;------------------------------------------------------------
                            405 ;Allocation info for local variables in function 'delay'
                            406 ;------------------------------------------------------------
                            407 ;i                         Allocated to registers r6 r7 
                            408 ;a                         Allocated to registers r4 r5 
                            409 ;b                         Allocated to registers r2 r3 
                            410 ;------------------------------------------------------------
                            411 ;	test1.c:2: void delay(int i)
                            412 ;	-----------------------------------------
                            413 ;	 function delay
                            414 ;	-----------------------------------------
   0085                     415 _delay:
                    0007    416 	ar7 = 0x07
                    0006    417 	ar6 = 0x06
                    0005    418 	ar5 = 0x05
                    0004    419 	ar4 = 0x04
                    0003    420 	ar3 = 0x03
                    0002    421 	ar2 = 0x02
                    0001    422 	ar1 = 0x01
                    0000    423 	ar0 = 0x00
   0085 AE 82               424 	mov	r6,dpl
   0087 AF 83               425 	mov	r7,dph
                            426 ;	test1.c:5: for(a=0;a<i;a++)
   0089 7C 00               427 	mov	r4,#0x00
   008B 7D 00               428 	mov	r5,#0x00
   008D                     429 00104$:
   008D C3                  430 	clr	c
   008E EC                  431 	mov	a,r4
   008F 9E                  432 	subb	a,r6
   0090 ED                  433 	mov	a,r5
   0091 64 80               434 	xrl	a,#0x80
   0093 8F F0               435 	mov	b,r7
   0095 63 F0 80            436 	xrl	b,#0x80
   0098 95 F0               437 	subb	a,b
   009A 50 14               438 	jnc	00108$
                            439 ;	test1.c:7: for(b=0;b<120;b++);
   009C 7A 78               440 	mov	r2,#0x78
   009E 7B 00               441 	mov	r3,#0x00
   00A0                     442 00103$:
   00A0 1A                  443 	dec	r2
   00A1 BA FF 01            444 	cjne	r2,#0xFF,00117$
   00A4 1B                  445 	dec	r3
   00A5                     446 00117$:
   00A5 EA                  447 	mov	a,r2
   00A6 4B                  448 	orl	a,r3
   00A7 70 F7               449 	jnz	00103$
                            450 ;	test1.c:5: for(a=0;a<i;a++)
   00A9 0C                  451 	inc	r4
   00AA BC 00 E0            452 	cjne	r4,#0x00,00104$
   00AD 0D                  453 	inc	r5
   00AE 80 DD               454 	sjmp	00104$
   00B0                     455 00108$:
   00B0 22                  456 	ret
                            457 ;------------------------------------------------------------
                            458 ;Allocation info for local variables in function 'display'
                            459 ;------------------------------------------------------------
                            460 ;i                         Allocated to registers r6 r7 
                            461 ;------------------------------------------------------------
                            462 ;	test1.c:36: void display(int i)
                            463 ;	-----------------------------------------
                            464 ;	 function display
                            465 ;	-----------------------------------------
   00B1                     466 _display:
   00B1 AE 82               467 	mov	r6,dpl
   00B3 AF 83               468 	mov	r7,dph
                            469 ;	test1.c:46: P0=255;
   00B5 75 80 FF            470 	mov	_P0,#0xFF
                            471 ;	test1.c:47: P2 = seg_position[2];
   00B8 90 06 04            472 	mov	dptr,#(_seg_position + 0x0002)
   00BB E4                  473 	clr	a
   00BC 93                  474 	movc	a,@a+dptr
   00BD F5 A0               475 	mov	_P2,a
                            476 ;	test1.c:48: P0=display_seg[i%1000/100];
   00BF 75 14 E8            477 	mov	__modsint_PARM_2,#0xE8
   00C2 75 15 03            478 	mov	(__modsint_PARM_2 + 1),#0x03
   00C5 8E 82               479 	mov	dpl,r6
   00C7 8F 83               480 	mov	dph,r7
   00C9 C0 07               481 	push	ar7
   00CB C0 06               482 	push	ar6
   00CD 12 05 0E            483 	lcall	__modsint
   00D0 75 14 64            484 	mov	__divsint_PARM_2,#0x64
   00D3 75 15 00            485 	mov	(__divsint_PARM_2 + 1),#0x00
   00D6 12 05 44            486 	lcall	__divsint
   00D9 AC 82               487 	mov	r4,dpl
   00DB AD 83               488 	mov	r5,dph
   00DD D0 06               489 	pop	ar6
   00DF D0 07               490 	pop	ar7
   00E1 EC                  491 	mov	a,r4
   00E2 24 F7               492 	add	a,#_display_seg
   00E4 F5 82               493 	mov	dpl,a
   00E6 ED                  494 	mov	a,r5
   00E7 34 05               495 	addc	a,#(_display_seg >> 8)
   00E9 F5 83               496 	mov	dph,a
   00EB E4                  497 	clr	a
   00EC 93                  498 	movc	a,@a+dptr
   00ED F5 80               499 	mov	_P0,a
                            500 ;	test1.c:49: delay(2);
   00EF 90 00 02            501 	mov	dptr,#0x0002
   00F2 C0 07               502 	push	ar7
   00F4 C0 06               503 	push	ar6
   00F6 12 00 85            504 	lcall	_delay
   00F9 D0 06               505 	pop	ar6
   00FB D0 07               506 	pop	ar7
                            507 ;	test1.c:50: P0=255;
   00FD 75 80 FF            508 	mov	_P0,#0xFF
                            509 ;	test1.c:51: P2 = seg_position[1];
   0100 90 06 03            510 	mov	dptr,#(_seg_position + 0x0001)
   0103 E4                  511 	clr	a
   0104 93                  512 	movc	a,@a+dptr
   0105 F5 A0               513 	mov	_P2,a
                            514 ;	test1.c:52: P0=display_seg[i%100/10];
   0107 75 14 64            515 	mov	__modsint_PARM_2,#0x64
   010A 75 15 00            516 	mov	(__modsint_PARM_2 + 1),#0x00
   010D 8E 82               517 	mov	dpl,r6
   010F 8F 83               518 	mov	dph,r7
   0111 C0 07               519 	push	ar7
   0113 C0 06               520 	push	ar6
   0115 12 05 0E            521 	lcall	__modsint
   0118 75 14 0A            522 	mov	__divsint_PARM_2,#0x0A
   011B 75 15 00            523 	mov	(__divsint_PARM_2 + 1),#0x00
   011E 12 05 44            524 	lcall	__divsint
   0121 AC 82               525 	mov	r4,dpl
   0123 AD 83               526 	mov	r5,dph
   0125 D0 06               527 	pop	ar6
   0127 D0 07               528 	pop	ar7
   0129 EC                  529 	mov	a,r4
   012A 24 F7               530 	add	a,#_display_seg
   012C F5 82               531 	mov	dpl,a
   012E ED                  532 	mov	a,r5
   012F 34 05               533 	addc	a,#(_display_seg >> 8)
   0131 F5 83               534 	mov	dph,a
   0133 E4                  535 	clr	a
   0134 93                  536 	movc	a,@a+dptr
   0135 F5 80               537 	mov	_P0,a
                            538 ;	test1.c:53: delay(2);
   0137 90 00 02            539 	mov	dptr,#0x0002
   013A C0 07               540 	push	ar7
   013C C0 06               541 	push	ar6
   013E 12 00 85            542 	lcall	_delay
   0141 D0 06               543 	pop	ar6
   0143 D0 07               544 	pop	ar7
                            545 ;	test1.c:54: P0=255;
   0145 75 80 FF            546 	mov	_P0,#0xFF
                            547 ;	test1.c:55: P2 = seg_position[0];
   0148 90 06 02            548 	mov	dptr,#_seg_position
   014B E4                  549 	clr	a
   014C 93                  550 	movc	a,@a+dptr
   014D F5 A0               551 	mov	_P2,a
                            552 ;	test1.c:56: P0=display_seg[i%10];
   014F 75 14 0A            553 	mov	__modsint_PARM_2,#0x0A
   0152 75 15 00            554 	mov	(__modsint_PARM_2 + 1),#0x00
   0155 8E 82               555 	mov	dpl,r6
   0157 8F 83               556 	mov	dph,r7
   0159 12 05 0E            557 	lcall	__modsint
   015C AE 82               558 	mov	r6,dpl
   015E AF 83               559 	mov	r7,dph
   0160 EE                  560 	mov	a,r6
   0161 24 F7               561 	add	a,#_display_seg
   0163 F5 82               562 	mov	dpl,a
   0165 EF                  563 	mov	a,r7
   0166 34 05               564 	addc	a,#(_display_seg >> 8)
   0168 F5 83               565 	mov	dph,a
   016A E4                  566 	clr	a
   016B 93                  567 	movc	a,@a+dptr
   016C F5 80               568 	mov	_P0,a
                            569 ;	test1.c:57: delay(2);
   016E 90 00 02            570 	mov	dptr,#0x0002
   0171 02 00 85            571 	ljmp	_delay
                            572 ;------------------------------------------------------------
                            573 ;Allocation info for local variables in function 'main'
                            574 ;------------------------------------------------------------
                            575 ;	test1.c:63: main()
                            576 ;	-----------------------------------------
                            577 ;	 function main
                            578 ;	-----------------------------------------
   0174                     579 _main:
                            580 ;	test1.c:70: IE=0x8a;
   0174 75 A8 8A            581 	mov	_IE,#0x8A
                            582 ;	test1.c:71: TMOD=0x11;
   0177 75 89 11            583 	mov	_TMOD,#0x11
                            584 ;	test1.c:119: set_bit(P2,4,0);
   017A AF A0               585 	mov	r7,_P2
   017C 53 07 EF            586 	anl	ar7,#0xEF
   017F 8F A0               587 	mov	_P2,r7
                            588 ;	test1.c:126: i=0;
                            589 ;	test1.c:127: _c=0;
   0181 E4                  590 	clr	a
   0182 F5 0C               591 	mov	_i,a
   0184 F5 0D               592 	mov	(_i + 1),a
   0186 F5 0E               593 	mov	__c,a
   0188 F5 0F               594 	mov	(__c + 1),a
                            595 ;	test1.c:128: TH1=(65536-50000)/256;
   018A 75 8D 3C            596 	mov	_TH1,#0x3C
                            597 ;	test1.c:129: TL1=(65536-50000)%256;
   018D 75 8B B0            598 	mov	_TL1,#0xB0
                            599 ;	test1.c:130: TR1=1;
   0190 D2 8E               600 	setb	_TR1
                            601 ;	test1.c:131: while(1)
   0192                     602 00102$:
                            603 ;	test1.c:133: display(i);
   0192 85 0C 82            604 	mov	dpl,_i
   0195 85 0D 83            605 	mov	dph,(_i + 1)
   0198 12 00 B1            606 	lcall	_display
                            607 ;	test1.c:134: i++;
   019B 05 0C               608 	inc	_i
   019D E4                  609 	clr	a
   019E B5 0C 02            610 	cjne	a,_i,00110$
   01A1 05 0D               611 	inc	(_i + 1)
   01A3                     612 00110$:
                            613 ;	test1.c:135: set_bit(P2,4,!get_bit(P2,4));
   01A3 74 10               614 	mov	a,#0x10
   01A5 55 A0               615 	anl	a,_P2
   01A7 FF                  616 	mov	r7,a
   01A8 B4 01 00            617 	cjne	a,#0x01,00111$
   01AB                     618 00111$:
   01AB E4                  619 	clr	a
   01AC 33                  620 	rlc	a
   01AD FF                  621 	mov	r7,a
   01AE 60 07               622 	jz	00106$
   01B0 74 10               623 	mov	a,#0x10
   01B2 45 A0               624 	orl	a,_P2
   01B4 FF                  625 	mov	r7,a
   01B5 80 06               626 	sjmp	00107$
   01B7                     627 00106$:
   01B7 AE A0               628 	mov	r6,_P2
   01B9 74 EF               629 	mov	a,#0xEF
   01BB 5E                  630 	anl	a,r6
   01BC FF                  631 	mov	r7,a
   01BD                     632 00107$:
   01BD 8F A0               633 	mov	_P2,r7
   01BF 80 D1               634 	sjmp	00102$
                            635 ;------------------------------------------------------------
                            636 ;Allocation info for local variables in function 'b'
                            637 ;------------------------------------------------------------
                            638 ;	test1.c:160: void b(void) interrupt 1
                            639 ;	-----------------------------------------
                            640 ;	 function b
                            641 ;	-----------------------------------------
   01C1                     642 _b:
   01C1 C0 E0               643 	push	acc
   01C3 C0 07               644 	push	ar7
   01C5 C0 06               645 	push	ar6
   01C7 C0 D0               646 	push	psw
   01C9 75 D0 00            647 	mov	psw,#0x00
                            648 ;	test1.c:163: set_bit(P2,4,!get_bit(P2,4));
   01CC 74 10               649 	mov	a,#0x10
   01CE 55 A0               650 	anl	a,_P2
   01D0 FF                  651 	mov	r7,a
   01D1 B4 01 00            652 	cjne	a,#0x01,00106$
   01D4                     653 00106$:
   01D4 E4                  654 	clr	a
   01D5 33                  655 	rlc	a
   01D6 FF                  656 	mov	r7,a
   01D7 60 07               657 	jz	00103$
   01D9 74 10               658 	mov	a,#0x10
   01DB 45 A0               659 	orl	a,_P2
   01DD FF                  660 	mov	r7,a
   01DE 80 06               661 	sjmp	00104$
   01E0                     662 00103$:
   01E0 AE A0               663 	mov	r6,_P2
   01E2 74 EF               664 	mov	a,#0xEF
   01E4 5E                  665 	anl	a,r6
   01E5 FF                  666 	mov	r7,a
   01E6                     667 00104$:
   01E6 8F A0               668 	mov	_P2,r7
                            669 ;	test1.c:164: TH0=t_h;
   01E8 85 08 8C            670 	mov	_TH0,_t_h
                            671 ;	test1.c:165: TL0=t_l;
   01EB 85 09 8A            672 	mov	_TL0,_t_l
   01EE D0 D0               673 	pop	psw
   01F0 D0 06               674 	pop	ar6
   01F2 D0 07               675 	pop	ar7
   01F4 D0 E0               676 	pop	acc
   01F6 32                  677 	reti
                            678 ;	eliminated unneeded push/pop dpl
                            679 ;	eliminated unneeded push/pop dph
                            680 ;	eliminated unneeded push/pop b
                            681 ;------------------------------------------------------------
                            682 ;Allocation info for local variables in function 'c'
                            683 ;------------------------------------------------------------
                            684 ;sloc0                     Allocated with name '_c_sloc0_1_0'
                            685 ;------------------------------------------------------------
                            686 ;	test1.c:169: void c(void) interrupt 3
                            687 ;	-----------------------------------------
                            688 ;	 function c
                            689 ;	-----------------------------------------
   01F7                     690 _c:
   01F7 C0 20               691 	push	bits
   01F9 C0 E0               692 	push	acc
   01FB C0 F0               693 	push	b
   01FD C0 82               694 	push	dpl
   01FF C0 83               695 	push	dph
   0201 C0 07               696 	push	(0+7)
   0203 C0 06               697 	push	(0+6)
   0205 C0 05               698 	push	(0+5)
   0207 C0 04               699 	push	(0+4)
   0209 C0 03               700 	push	(0+3)
   020B C0 02               701 	push	(0+2)
   020D C0 01               702 	push	(0+1)
   020F C0 00               703 	push	(0+0)
   0211 C0 D0               704 	push	psw
   0213 75 D0 00            705 	mov	psw,#0x00
                            706 ;	test1.c:171: TH1=(65536-50000)/256;
   0216 75 8D 3C            707 	mov	_TH1,#0x3C
                            708 ;	test1.c:172: TL1=(65536-50000)%256;
   0219 75 8B B0            709 	mov	_TL1,#0xB0
                            710 ;	test1.c:173: _c+=50;
   021C 74 32               711 	mov	a,#0x32
   021E 25 0E               712 	add	a,__c
   0220 F5 0E               713 	mov	__c,a
   0222 E4                  714 	clr	a
   0223 35 0F               715 	addc	a,(__c + 1)
   0225 F5 0F               716 	mov	(__c + 1),a
                            717 ;	test1.c:175: if(_c<time[i])return;
   0227 E5 0C               718 	mov	a,_i
   0229 25 0C               719 	add	a,_i
   022B FE                  720 	mov	r6,a
   022C E5 0D               721 	mov	a,(_i + 1)
   022E 33                  722 	rlc	a
   022F FF                  723 	mov	r7,a
   0230 EE                  724 	mov	a,r6
   0231 24 98               725 	add	a,#_time
   0233 F5 82               726 	mov	dpl,a
   0235 EF                  727 	mov	a,r7
   0236 34 05               728 	addc	a,#(_time >> 8)
   0238 F5 83               729 	mov	dph,a
   023A E4                  730 	clr	a
   023B 93                  731 	movc	a,@a+dptr
   023C FE                  732 	mov	r6,a
   023D A3                  733 	inc	dptr
   023E E4                  734 	clr	a
   023F 93                  735 	movc	a,@a+dptr
   0240 FF                  736 	mov	r7,a
   0241 AC 0E               737 	mov	r4,__c
   0243 AD 0F               738 	mov	r5,(__c + 1)
   0245 C3                  739 	clr	c
   0246 EC                  740 	mov	a,r4
   0247 9E                  741 	subb	a,r6
   0248 ED                  742 	mov	a,r5
   0249 9F                  743 	subb	a,r7
   024A 50 03               744 	jnc	00102$
   024C 02 02 F2            745 	ljmp	00109$
   024F                     746 00102$:
                            747 ;	test1.c:176: _c=0;
   024F E4                  748 	clr	a
   0250 F5 0E               749 	mov	__c,a
   0252 F5 0F               750 	mov	(__c + 1),a
                            751 ;	test1.c:177: if(s[i]==0)TR0=0;
   0254 E5 0C               752 	mov	a,_i
   0256 24 DA               753 	add	a,#_s
   0258 F5 82               754 	mov	dpl,a
   025A E5 0D               755 	mov	a,(_i + 1)
   025C 34 05               756 	addc	a,#(_s >> 8)
   025E F5 83               757 	mov	dph,a
   0260 E4                  758 	clr	a
   0261 93                  759 	movc	a,@a+dptr
   0262 FF                  760 	mov	r7,a
   0263 70 02               761 	jnz	00104$
   0265 C2 8C               762 	clr	_TR0
   0267                     763 00104$:
                            764 ;	test1.c:178: t_h=(65536-(t[s[i]]))/256;
   0267 EF                  765 	mov	a,r7
   0268 2F                  766 	add	a,r7
   0269 FD                  767 	mov	r5,a
   026A 90 05 80            768 	mov	dptr,#_t
   026D 93                  769 	movc	a,@a+dptr
   026E CD                  770 	xch	a,r5
   026F A3                  771 	inc	dptr
   0270 93                  772 	movc	a,@a+dptr
   0271 FE                  773 	mov	r6,a
   0272 E4                  774 	clr	a
   0273 FC                  775 	mov	r4,a
   0274 FB                  776 	mov	r3,a
   0275 C3                  777 	clr	c
   0276 9D                  778 	subb	a,r5
   0277 F5 10               779 	mov	_c_sloc0_1_0,a
   0279 E4                  780 	clr	a
   027A 9E                  781 	subb	a,r6
   027B F5 11               782 	mov	(_c_sloc0_1_0 + 1),a
   027D 74 01               783 	mov	a,#0x01
   027F 9C                  784 	subb	a,r4
   0280 F5 12               785 	mov	(_c_sloc0_1_0 + 2),a
   0282 E4                  786 	clr	a
   0283 9B                  787 	subb	a,r3
   0284 F5 13               788 	mov	(_c_sloc0_1_0 + 3),a
   0286 E4                  789 	clr	a
   0287 F5 14               790 	mov	__divslong_PARM_2,a
   0289 75 15 01            791 	mov	(__divslong_PARM_2 + 1),#0x01
   028C F5 16               792 	mov	(__divslong_PARM_2 + 2),a
   028E F5 17               793 	mov	(__divslong_PARM_2 + 3),a
   0290 85 10 82            794 	mov	dpl,_c_sloc0_1_0
   0293 85 11 83            795 	mov	dph,(_c_sloc0_1_0 + 1)
   0296 85 12 F0            796 	mov	b,(_c_sloc0_1_0 + 2)
   0299 E5 13               797 	mov	a,(_c_sloc0_1_0 + 3)
   029B C0 07               798 	push	ar7
   029D 12 04 BC            799 	lcall	__divslong
   02A0 A8 82               800 	mov	r0,dpl
   02A2 88 08               801 	mov	_t_h,r0
                            802 ;	test1.c:179: t_l=(65536-(t[s[i]]))%256;
   02A4 E4                  803 	clr	a
   02A5 F5 14               804 	mov	__modslong_PARM_2,a
   02A7 75 15 01            805 	mov	(__modslong_PARM_2 + 1),#0x01
   02AA F5 16               806 	mov	(__modslong_PARM_2 + 2),a
   02AC F5 17               807 	mov	(__modslong_PARM_2 + 3),a
   02AE 85 10 82            808 	mov	dpl,_c_sloc0_1_0
   02B1 85 11 83            809 	mov	dph,(_c_sloc0_1_0 + 1)
   02B4 85 12 F0            810 	mov	b,(_c_sloc0_1_0 + 2)
   02B7 E5 13               811 	mov	a,(_c_sloc0_1_0 + 3)
   02B9 12 04 6D            812 	lcall	__modslong
   02BC AB 82               813 	mov	r3,dpl
   02BE AC 83               814 	mov	r4,dph
   02C0 AD F0               815 	mov	r5,b
   02C2 FE                  816 	mov	r6,a
   02C3 D0 07               817 	pop	ar7
   02C5 8B 09               818 	mov	_t_l,r3
                            819 ;	test1.c:180: TH0=t_h;
   02C7 85 08 8C            820 	mov	_TH0,_t_h
                            821 ;	test1.c:181: TL0=t_l;
   02CA 85 09 8A            822 	mov	_TL0,_t_l
                            823 ;	test1.c:182: if(s[i]!=0)TR0=1;
   02CD EF                  824 	mov	a,r7
   02CE 60 02               825 	jz	00106$
   02D0 D2 8C               826 	setb	_TR0
   02D2                     827 00106$:
                            828 ;	test1.c:185: i+=1;
   02D2 05 0C               829 	inc	_i
   02D4 E4                  830 	clr	a
   02D5 B5 0C 02            831 	cjne	a,_i,00118$
   02D8 05 0D               832 	inc	(_i + 1)
   02DA                     833 00118$:
                            834 ;	test1.c:186: if(i>=count)i=0;
   02DA C3                  835 	clr	c
   02DB E5 0C               836 	mov	a,_i
   02DD 95 0A               837 	subb	a,_count
   02DF E5 0D               838 	mov	a,(_i + 1)
   02E1 64 80               839 	xrl	a,#0x80
   02E3 85 0B F0            840 	mov	b,(_count + 1)
   02E6 63 F0 80            841 	xrl	b,#0x80
   02E9 95 F0               842 	subb	a,b
   02EB 40 05               843 	jc	00109$
   02ED E4                  844 	clr	a
   02EE F5 0C               845 	mov	_i,a
   02F0 F5 0D               846 	mov	(_i + 1),a
   02F2                     847 00109$:
   02F2 D0 D0               848 	pop	psw
   02F4 D0 00               849 	pop	(0+0)
   02F6 D0 01               850 	pop	(0+1)
   02F8 D0 02               851 	pop	(0+2)
   02FA D0 03               852 	pop	(0+3)
   02FC D0 04               853 	pop	(0+4)
   02FE D0 05               854 	pop	(0+5)
   0300 D0 06               855 	pop	(0+6)
   0302 D0 07               856 	pop	(0+7)
   0304 D0 83               857 	pop	dph
   0306 D0 82               858 	pop	dpl
   0308 D0 F0               859 	pop	b
   030A D0 E0               860 	pop	acc
   030C D0 20               861 	pop	bits
   030E 32                  862 	reti
                            863 	.area CSEG    (CODE)
                            864 	.area CONST   (CODE)
   0580                     865 _t:
   0580 00 00               866 	.byte #0x00,#0x00	; 0
   0582 BC 03               867 	.byte #0xBC,#0x03	; 956
   0584 54 03               868 	.byte #0x54,#0x03	; 852
   0586 F7 02               869 	.byte #0xF7,#0x02	; 759
   0588 CC 02               870 	.byte #0xCC,#0x02	; 716
   058A 7E 02               871 	.byte #0x7E,#0x02	; 638
   058C 38 02               872 	.byte #0x38,#0x02	; 568
   058E FA 01               873 	.byte #0xFA,#0x01	; 506
   0590 DE 01               874 	.byte #0xDE,#0x01	; 478
   0592 AA 01               875 	.byte #0xAA,#0x01	; 426
   0594 7B 01               876 	.byte #0x7B,#0x01	; 379
   0596 00 00               877 	.byte #0x00,#0x00	; 0
   0598                     878 _time:
   0598 F4 01               879 	.byte #0xF4,#0x01	; 500
   059A F4 01               880 	.byte #0xF4,#0x01	; 500
   059C F4 01               881 	.byte #0xF4,#0x01	; 500
   059E F4 01               882 	.byte #0xF4,#0x01	; 500
   05A0 F4 01               883 	.byte #0xF4,#0x01	; 500
   05A2 F4 01               884 	.byte #0xF4,#0x01	; 500
   05A4 F4 01               885 	.byte #0xF4,#0x01	; 500
   05A6 F4 01               886 	.byte #0xF4,#0x01	; 500
   05A8 F4 01               887 	.byte #0xF4,#0x01	; 500
   05AA F4 01               888 	.byte #0xF4,#0x01	; 500
   05AC F4 01               889 	.byte #0xF4,#0x01	; 500
   05AE F4 01               890 	.byte #0xF4,#0x01	; 500
   05B0 F4 01               891 	.byte #0xF4,#0x01	; 500
   05B2 F4 01               892 	.byte #0xF4,#0x01	; 500
   05B4 F4 01               893 	.byte #0xF4,#0x01	; 500
   05B6 F4 01               894 	.byte #0xF4,#0x01	; 500
   05B8 F4 01               895 	.byte #0xF4,#0x01	; 500
   05BA F4 01               896 	.byte #0xF4,#0x01	; 500
   05BC F4 01               897 	.byte #0xF4,#0x01	; 500
   05BE F4 01               898 	.byte #0xF4,#0x01	; 500
   05C0 F4 01               899 	.byte #0xF4,#0x01	; 500
   05C2 F4 01               900 	.byte #0xF4,#0x01	; 500
   05C4 F4 01               901 	.byte #0xF4,#0x01	; 500
   05C6 F4 01               902 	.byte #0xF4,#0x01	; 500
   05C8 F4 01               903 	.byte #0xF4,#0x01	; 500
   05CA F4 01               904 	.byte #0xF4,#0x01	; 500
   05CC F4 01               905 	.byte #0xF4,#0x01	; 500
   05CE F4 01               906 	.byte #0xF4,#0x01	; 500
   05D0 F4 01               907 	.byte #0xF4,#0x01	; 500
   05D2 F4 01               908 	.byte #0xF4,#0x01	; 500
   05D4 F4 01               909 	.byte #0xF4,#0x01	; 500
   05D6 F4 01               910 	.byte #0xF4,#0x01	; 500
   05D8 F4 01               911 	.byte #0xF4,#0x01	; 500
   05DA                     912 _s:
   05DA 01                  913 	.db #0x01	; 1
   05DB 01                  914 	.db #0x01	; 1
   05DC 02                  915 	.db #0x02	; 2
   05DD 01                  916 	.db #0x01	; 1
   05DE 04                  917 	.db #0x04	; 4
   05DF 03                  918 	.db #0x03	; 3
   05E0 00                  919 	.db #0x00	; 0
   05E1 01                  920 	.db #0x01	; 1
   05E2 01                  921 	.db #0x01	; 1
   05E3 02                  922 	.db #0x02	; 2
   05E4 01                  923 	.db #0x01	; 1
   05E5 05                  924 	.db #0x05	; 5
   05E6 04                  925 	.db #0x04	; 4
   05E7 00                  926 	.db #0x00	; 0
   05E8 01                  927 	.db #0x01	; 1
   05E9 01                  928 	.db #0x01	; 1
   05EA 08                  929 	.db #0x08	; 8
   05EB 06                  930 	.db #0x06	; 6
   05EC 04                  931 	.db #0x04	; 4
   05ED 03                  932 	.db #0x03	; 3
   05EE 02                  933 	.db #0x02	; 2
   05EF 00                  934 	.db #0x00	; 0
   05F0 07                  935 	.db #0x07	; 7
   05F1 07                  936 	.db #0x07	; 7
   05F2 06                  937 	.db #0x06	; 6
   05F3 04                  938 	.db #0x04	; 4
   05F4 05                  939 	.db #0x05	; 5
   05F5 04                  940 	.db #0x04	; 4
   05F6 00                  941 	.db #0x00	; 0
   05F7                     942 _display_seg:
   05F7 C0                  943 	.db #0xC0	; 192
   05F8 F9                  944 	.db #0xF9	; 249
   05F9 A4                  945 	.db #0xA4	; 164
   05FA B0                  946 	.db #0xB0	; 176
   05FB 99                  947 	.db #0x99	; 153
   05FC 92                  948 	.db #0x92	; 146
   05FD 82                  949 	.db #0x82	; 130
   05FE F8                  950 	.db #0xF8	; 248
   05FF 80                  951 	.db #0x80	; 128
   0600 90                  952 	.db #0x90	; 144
   0601 7F                  953 	.db #0x7F	; 127
   0602                     954 _seg_position:
   0602 06                  955 	.db #0x06	; 6
   0603 05                  956 	.db #0x05	; 5
   0604 03                  957 	.db #0x03	; 3
   0605 07                  958 	.db #0x07	; 7
   0606 00                  959 	.db #0x00	; 0
                            960 	.area XINIT   (CODE)
                            961 	.area CABS    (ABS,CODE)
