                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
                              4 ; This file was generated Sun May 13 18:20:02 2012
                              5 ;--------------------------------------------------------
                              6 	.module test1
                              7 	.optsdcc -mmcs51 --model-large
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _c
                             13 	.globl _b
                             14 	.globl _main
                             15 	.globl _display
                             16 	.globl _delay
                             17 	.globl _CY
                             18 	.globl _AC
                             19 	.globl _F0
                             20 	.globl _RS1
                             21 	.globl _RS0
                             22 	.globl _OV
                             23 	.globl _F1
                             24 	.globl _P
                             25 	.globl _PS
                             26 	.globl _PT1
                             27 	.globl _PX1
                             28 	.globl _PT0
                             29 	.globl _PX0
                             30 	.globl _RD
                             31 	.globl _WR
                             32 	.globl _T1
                             33 	.globl _T0
                             34 	.globl _INT1
                             35 	.globl _INT0
                             36 	.globl _TXD
                             37 	.globl _RXD
                             38 	.globl _P3_7
                             39 	.globl _P3_6
                             40 	.globl _P3_5
                             41 	.globl _P3_4
                             42 	.globl _P3_3
                             43 	.globl _P3_2
                             44 	.globl _P3_1
                             45 	.globl _P3_0
                             46 	.globl _EA
                             47 	.globl _ES
                             48 	.globl _ET1
                             49 	.globl _EX1
                             50 	.globl _ET0
                             51 	.globl _EX0
                             52 	.globl _P2_7
                             53 	.globl _P2_6
                             54 	.globl _P2_5
                             55 	.globl _P2_4
                             56 	.globl _P2_3
                             57 	.globl _P2_2
                             58 	.globl _P2_1
                             59 	.globl _P2_0
                             60 	.globl _SM0
                             61 	.globl _SM1
                             62 	.globl _SM2
                             63 	.globl _REN
                             64 	.globl _TB8
                             65 	.globl _RB8
                             66 	.globl _TI
                             67 	.globl _RI
                             68 	.globl _P1_7
                             69 	.globl _P1_6
                             70 	.globl _P1_5
                             71 	.globl _P1_4
                             72 	.globl _P1_3
                             73 	.globl _P1_2
                             74 	.globl _P1_1
                             75 	.globl _P1_0
                             76 	.globl _TF1
                             77 	.globl _TR1
                             78 	.globl _TF0
                             79 	.globl _TR0
                             80 	.globl _IE1
                             81 	.globl _IT1
                             82 	.globl _IE0
                             83 	.globl _IT0
                             84 	.globl _P0_7
                             85 	.globl _P0_6
                             86 	.globl _P0_5
                             87 	.globl _P0_4
                             88 	.globl _P0_3
                             89 	.globl _P0_2
                             90 	.globl _P0_1
                             91 	.globl _P0_0
                             92 	.globl _B
                             93 	.globl _ACC
                             94 	.globl _PSW
                             95 	.globl _IP
                             96 	.globl _P3
                             97 	.globl _IE
                             98 	.globl _P2
                             99 	.globl _SBUF
                            100 	.globl _SCON
                            101 	.globl _P1
                            102 	.globl _TH1
                            103 	.globl _TH0
                            104 	.globl _TL1
                            105 	.globl _TL0
                            106 	.globl _TMOD
                            107 	.globl _TCON
                            108 	.globl _PCON
                            109 	.globl _DPH
                            110 	.globl _DPL
                            111 	.globl _SP
                            112 	.globl _P0
                            113 	.globl _count
                            114 	.globl _seg_position
                            115 	.globl _display_seg
                            116 	.globl _s
                            117 	.globl _time
                            118 	.globl _t
                            119 	.globl __c
                            120 	.globl _i
                            121 	.globl _t_l
                            122 	.globl _t_h
                            123 ;--------------------------------------------------------
                            124 ; special function registers
                            125 ;--------------------------------------------------------
                            126 	.area RSEG    (DATA)
                    0080    127 _P0	=	0x0080
                    0081    128 _SP	=	0x0081
                    0082    129 _DPL	=	0x0082
                    0083    130 _DPH	=	0x0083
                    0087    131 _PCON	=	0x0087
                    0088    132 _TCON	=	0x0088
                    0089    133 _TMOD	=	0x0089
                    008A    134 _TL0	=	0x008a
                    008B    135 _TL1	=	0x008b
                    008C    136 _TH0	=	0x008c
                    008D    137 _TH1	=	0x008d
                    0090    138 _P1	=	0x0090
                    0098    139 _SCON	=	0x0098
                    0099    140 _SBUF	=	0x0099
                    00A0    141 _P2	=	0x00a0
                    00A8    142 _IE	=	0x00a8
                    00B0    143 _P3	=	0x00b0
                    00B8    144 _IP	=	0x00b8
                    00D0    145 _PSW	=	0x00d0
                    00E0    146 _ACC	=	0x00e0
                    00F0    147 _B	=	0x00f0
                            148 ;--------------------------------------------------------
                            149 ; special function bits
                            150 ;--------------------------------------------------------
                            151 	.area RSEG    (DATA)
                    0080    152 _P0_0	=	0x0080
                    0081    153 _P0_1	=	0x0081
                    0082    154 _P0_2	=	0x0082
                    0083    155 _P0_3	=	0x0083
                    0084    156 _P0_4	=	0x0084
                    0085    157 _P0_5	=	0x0085
                    0086    158 _P0_6	=	0x0086
                    0087    159 _P0_7	=	0x0087
                    0088    160 _IT0	=	0x0088
                    0089    161 _IE0	=	0x0089
                    008A    162 _IT1	=	0x008a
                    008B    163 _IE1	=	0x008b
                    008C    164 _TR0	=	0x008c
                    008D    165 _TF0	=	0x008d
                    008E    166 _TR1	=	0x008e
                    008F    167 _TF1	=	0x008f
                    0090    168 _P1_0	=	0x0090
                    0091    169 _P1_1	=	0x0091
                    0092    170 _P1_2	=	0x0092
                    0093    171 _P1_3	=	0x0093
                    0094    172 _P1_4	=	0x0094
                    0095    173 _P1_5	=	0x0095
                    0096    174 _P1_6	=	0x0096
                    0097    175 _P1_7	=	0x0097
                    0098    176 _RI	=	0x0098
                    0099    177 _TI	=	0x0099
                    009A    178 _RB8	=	0x009a
                    009B    179 _TB8	=	0x009b
                    009C    180 _REN	=	0x009c
                    009D    181 _SM2	=	0x009d
                    009E    182 _SM1	=	0x009e
                    009F    183 _SM0	=	0x009f
                    00A0    184 _P2_0	=	0x00a0
                    00A1    185 _P2_1	=	0x00a1
                    00A2    186 _P2_2	=	0x00a2
                    00A3    187 _P2_3	=	0x00a3
                    00A4    188 _P2_4	=	0x00a4
                    00A5    189 _P2_5	=	0x00a5
                    00A6    190 _P2_6	=	0x00a6
                    00A7    191 _P2_7	=	0x00a7
                    00A8    192 _EX0	=	0x00a8
                    00A9    193 _ET0	=	0x00a9
                    00AA    194 _EX1	=	0x00aa
                    00AB    195 _ET1	=	0x00ab
                    00AC    196 _ES	=	0x00ac
                    00AF    197 _EA	=	0x00af
                    00B0    198 _P3_0	=	0x00b0
                    00B1    199 _P3_1	=	0x00b1
                    00B2    200 _P3_2	=	0x00b2
                    00B3    201 _P3_3	=	0x00b3
                    00B4    202 _P3_4	=	0x00b4
                    00B5    203 _P3_5	=	0x00b5
                    00B6    204 _P3_6	=	0x00b6
                    00B7    205 _P3_7	=	0x00b7
                    00B0    206 _RXD	=	0x00b0
                    00B1    207 _TXD	=	0x00b1
                    00B2    208 _INT0	=	0x00b2
                    00B3    209 _INT1	=	0x00b3
                    00B4    210 _T0	=	0x00b4
                    00B5    211 _T1	=	0x00b5
                    00B6    212 _WR	=	0x00b6
                    00B7    213 _RD	=	0x00b7
                    00B8    214 _PX0	=	0x00b8
                    00B9    215 _PT0	=	0x00b9
                    00BA    216 _PX1	=	0x00ba
                    00BB    217 _PT1	=	0x00bb
                    00BC    218 _PS	=	0x00bc
                    00D0    219 _P	=	0x00d0
                    00D1    220 _F1	=	0x00d1
                    00D2    221 _OV	=	0x00d2
                    00D3    222 _RS0	=	0x00d3
                    00D4    223 _RS1	=	0x00d4
                    00D5    224 _F0	=	0x00d5
                    00D6    225 _AC	=	0x00d6
                    00D7    226 _CY	=	0x00d7
                            227 ;--------------------------------------------------------
                            228 ; overlayable register banks
                            229 ;--------------------------------------------------------
                            230 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     231 	.ds 8
                            232 ;--------------------------------------------------------
                            233 ; overlayable bit register bank
                            234 ;--------------------------------------------------------
                            235 	.area BIT_BANK	(REL,OVR,DATA)
   0021                     236 bits:
   0021                     237 	.ds 1
                    8000    238 	b0 = bits[0]
                    8100    239 	b1 = bits[1]
                    8200    240 	b2 = bits[2]
                    8300    241 	b3 = bits[3]
                    8400    242 	b4 = bits[4]
                    8500    243 	b5 = bits[5]
                    8600    244 	b6 = bits[6]
                    8700    245 	b7 = bits[7]
                            246 ;--------------------------------------------------------
                            247 ; internal ram data
                            248 ;--------------------------------------------------------
                            249 	.area DSEG    (DATA)
   0008                     250 _c_sloc0_1_0:
   0008                     251 	.ds 4
                            252 ;--------------------------------------------------------
                            253 ; overlayable items in internal ram 
                            254 ;--------------------------------------------------------
                            255 	.area OSEG    (OVR,DATA)
                            256 ;--------------------------------------------------------
                            257 ; Stack segment in internal ram 
                            258 ;--------------------------------------------------------
                            259 	.area	SSEG	(DATA)
   0022                     260 __start__stack:
   0022                     261 	.ds	1
                            262 
                            263 ;--------------------------------------------------------
                            264 ; indirectly addressable internal ram data
                            265 ;--------------------------------------------------------
                            266 	.area ISEG    (DATA)
                            267 ;--------------------------------------------------------
                            268 ; absolute internal ram data
                            269 ;--------------------------------------------------------
                            270 	.area IABS    (ABS,DATA)
                            271 	.area IABS    (ABS,DATA)
                            272 ;--------------------------------------------------------
                            273 ; bit data
                            274 ;--------------------------------------------------------
                            275 	.area BSEG    (BIT)
                            276 ;--------------------------------------------------------
                            277 ; paged external ram data
                            278 ;--------------------------------------------------------
                            279 	.area PSEG    (PAG,XDATA)
                            280 ;--------------------------------------------------------
                            281 ; external ram data
                            282 ;--------------------------------------------------------
                            283 	.area XSEG    (XDATA)
   0000                     284 _delay_i_1_1:
   0000                     285 	.ds 2
   0002                     286 _t_h::
   0002                     287 	.ds 1
   0003                     288 _t_l::
   0003                     289 	.ds 1
   0004                     290 _display_i_1_1:
   0004                     291 	.ds 2
   0006                     292 _i::
   0006                     293 	.ds 2
   0008                     294 __c::
   0008                     295 	.ds 2
                            296 ;--------------------------------------------------------
                            297 ; absolute external ram data
                            298 ;--------------------------------------------------------
                            299 	.area XABS    (ABS,XDATA)
                            300 ;--------------------------------------------------------
                            301 ; external initialized ram data
                            302 ;--------------------------------------------------------
                            303 	.area XISEG   (XDATA)
   0042                     304 _t::
   0042                     305 	.ds 24
   005A                     306 _time::
   005A                     307 	.ds 66
   009C                     308 _s::
   009C                     309 	.ds 29
   00B9                     310 _display_seg::
   00B9                     311 	.ds 11
   00C4                     312 _seg_position::
   00C4                     313 	.ds 5
   00C9                     314 _count::
   00C9                     315 	.ds 2
                            316 	.area HOME    (CODE)
                            317 	.area GSINIT0 (CODE)
                            318 	.area GSINIT1 (CODE)
                            319 	.area GSINIT2 (CODE)
                            320 	.area GSINIT3 (CODE)
                            321 	.area GSINIT4 (CODE)
                            322 	.area GSINIT5 (CODE)
                            323 	.area GSINIT  (CODE)
                            324 	.area GSFINAL (CODE)
                            325 	.area CSEG    (CODE)
                            326 ;--------------------------------------------------------
                            327 ; interrupt vector 
                            328 ;--------------------------------------------------------
                            329 	.area HOME    (CODE)
   0000                     330 __interrupt_vect:
   0000 02 00 23            331 	ljmp	__sdcc_gsinit_startup
   0003 32                  332 	reti
   0004                     333 	.ds	7
   000B 02 01 F5            334 	ljmp	_b
   000E                     335 	.ds	5
   0013 32                  336 	reti
   0014                     337 	.ds	7
   001B 02 02 39            338 	ljmp	_c
                            339 ;--------------------------------------------------------
                            340 ; global & static initialisations
                            341 ;--------------------------------------------------------
                            342 	.area HOME    (CODE)
                            343 	.area GSINIT  (CODE)
                            344 	.area GSFINAL (CODE)
                            345 	.area GSINIT  (CODE)
                            346 	.globl __sdcc_gsinit_startup
                            347 	.globl __sdcc_program_startup
                            348 	.globl __start__stack
                            349 	.globl __mcs51_genXINIT
                            350 	.globl __mcs51_genXRAMCLEAR
                            351 	.globl __mcs51_genRAMCLEAR
                            352 	.area GSFINAL (CODE)
   007C 02 00 1E            353 	ljmp	__sdcc_program_startup
                            354 ;--------------------------------------------------------
                            355 ; Home
                            356 ;--------------------------------------------------------
                            357 	.area HOME    (CODE)
                            358 	.area HOME    (CODE)
   001E                     359 __sdcc_program_startup:
   001E 12 01 96            360 	lcall	_main
                            361 ;	return from main will lock up
   0021 80 FE               362 	sjmp .
                            363 ;--------------------------------------------------------
                            364 ; code
                            365 ;--------------------------------------------------------
                            366 	.area CSEG    (CODE)
                            367 ;------------------------------------------------------------
                            368 ;Allocation info for local variables in function 'delay'
                            369 ;------------------------------------------------------------
                            370 ;i                         Allocated with name '_delay_i_1_1'
                            371 ;a                         Allocated with name '_delay_a_1_1'
                            372 ;b                         Allocated with name '_delay_b_1_1'
                            373 ;------------------------------------------------------------
                            374 ;	test1.c:2: void delay(int i)
                            375 ;	-----------------------------------------
                            376 ;	 function delay
                            377 ;	-----------------------------------------
   007F                     378 _delay:
                    0002    379 	ar2 = 0x02
                    0003    380 	ar3 = 0x03
                    0004    381 	ar4 = 0x04
                    0005    382 	ar5 = 0x05
                    0006    383 	ar6 = 0x06
                    0007    384 	ar7 = 0x07
                    0000    385 	ar0 = 0x00
                    0001    386 	ar1 = 0x01
   007F AA 83               387 	mov	r2,dph
   0081 E5 82               388 	mov	a,dpl
   0083 90 00 00            389 	mov	dptr,#_delay_i_1_1
   0086 F0                  390 	movx	@dptr,a
   0087 A3                  391 	inc	dptr
   0088 EA                  392 	mov	a,r2
   0089 F0                  393 	movx	@dptr,a
                            394 ;	test1.c:5: for(a=0;a<i;a++)
   008A 90 00 00            395 	mov	dptr,#_delay_i_1_1
   008D E0                  396 	movx	a,@dptr
   008E FA                  397 	mov	r2,a
   008F A3                  398 	inc	dptr
   0090 E0                  399 	movx	a,@dptr
   0091 FB                  400 	mov	r3,a
   0092 7C 00               401 	mov	r4,#0x00
   0094 7D 00               402 	mov	r5,#0x00
   0096                     403 00104$:
   0096 C3                  404 	clr	c
   0097 EC                  405 	mov	a,r4
   0098 9A                  406 	subb	a,r2
   0099 ED                  407 	mov	a,r5
   009A 64 80               408 	xrl	a,#0x80
   009C 8B F0               409 	mov	b,r3
   009E 63 F0 80            410 	xrl	b,#0x80
   00A1 95 F0               411 	subb	a,b
   00A3 50 14               412 	jnc	00108$
                            413 ;	test1.c:7: for(b=0;b<120;b++);
   00A5 7E 78               414 	mov	r6,#0x78
   00A7 7F 00               415 	mov	r7,#0x00
   00A9                     416 00103$:
   00A9 1E                  417 	dec	r6
   00AA BE FF 01            418 	cjne	r6,#0xff,00117$
   00AD 1F                  419 	dec	r7
   00AE                     420 00117$:
   00AE EE                  421 	mov	a,r6
   00AF 4F                  422 	orl	a,r7
   00B0 70 F7               423 	jnz	00103$
                            424 ;	test1.c:5: for(a=0;a<i;a++)
   00B2 0C                  425 	inc	r4
   00B3 BC 00 E0            426 	cjne	r4,#0x00,00104$
   00B6 0D                  427 	inc	r5
   00B7 80 DD               428 	sjmp	00104$
   00B9                     429 00108$:
   00B9 22                  430 	ret
                            431 ;------------------------------------------------------------
                            432 ;Allocation info for local variables in function 'display'
                            433 ;------------------------------------------------------------
                            434 ;i                         Allocated with name '_display_i_1_1'
                            435 ;------------------------------------------------------------
                            436 ;	test1.c:36: void display(int i)
                            437 ;	-----------------------------------------
                            438 ;	 function display
                            439 ;	-----------------------------------------
   00BA                     440 _display:
   00BA AA 83               441 	mov	r2,dph
   00BC E5 82               442 	mov	a,dpl
   00BE 90 00 04            443 	mov	dptr,#_display_i_1_1
   00C1 F0                  444 	movx	@dptr,a
   00C2 A3                  445 	inc	dptr
   00C3 EA                  446 	mov	a,r2
   00C4 F0                  447 	movx	@dptr,a
                            448 ;	test1.c:46: P0=255;
   00C5 75 80 FF            449 	mov	_P0,#0xFF
                            450 ;	test1.c:47: P2 = seg_position[2];
   00C8 90 00 C6            451 	mov	dptr,#(_seg_position + 0x0002)
   00CB E0                  452 	movx	a,@dptr
   00CC F5 A0               453 	mov	_P2,a
                            454 ;	test1.c:48: P0=display_seg[i%1000/100];
   00CE 90 00 04            455 	mov	dptr,#_display_i_1_1
   00D1 E0                  456 	movx	a,@dptr
   00D2 FA                  457 	mov	r2,a
   00D3 A3                  458 	inc	dptr
   00D4 E0                  459 	movx	a,@dptr
   00D5 FB                  460 	mov	r3,a
   00D6 90 00 2F            461 	mov	dptr,#__modsint_PARM_2
   00D9 74 E8               462 	mov	a,#0xE8
   00DB F0                  463 	movx	@dptr,a
   00DC A3                  464 	inc	dptr
   00DD 74 03               465 	mov	a,#0x03
   00DF F0                  466 	movx	@dptr,a
   00E0 8A 82               467 	mov	dpl,r2
   00E2 8B 83               468 	mov	dph,r3
   00E4 C0 02               469 	push	ar2
   00E6 C0 03               470 	push	ar3
   00E8 12 07 60            471 	lcall	__modsint
   00EB AC 82               472 	mov	r4,dpl
   00ED AD 83               473 	mov	r5,dph
   00EF 90 00 38            474 	mov	dptr,#__divsint_PARM_2
   00F2 74 64               475 	mov	a,#0x64
   00F4 F0                  476 	movx	@dptr,a
   00F5 E4                  477 	clr	a
   00F6 A3                  478 	inc	dptr
   00F7 F0                  479 	movx	@dptr,a
   00F8 8C 82               480 	mov	dpl,r4
   00FA 8D 83               481 	mov	dph,r5
   00FC 12 08 7B            482 	lcall	__divsint
   00FF AC 82               483 	mov	r4,dpl
   0101 AD 83               484 	mov	r5,dph
   0103 EC                  485 	mov	a,r4
   0104 24 B9               486 	add	a,#_display_seg
   0106 F5 82               487 	mov	dpl,a
   0108 ED                  488 	mov	a,r5
   0109 34 00               489 	addc	a,#(_display_seg >> 8)
   010B F5 83               490 	mov	dph,a
   010D E0                  491 	movx	a,@dptr
   010E F5 80               492 	mov	_P0,a
                            493 ;	test1.c:49: delay(2);
   0110 90 00 02            494 	mov	dptr,#0x0002
   0113 12 00 7F            495 	lcall	_delay
   0116 D0 03               496 	pop	ar3
   0118 D0 02               497 	pop	ar2
                            498 ;	test1.c:50: P0=255;
   011A 75 80 FF            499 	mov	_P0,#0xFF
                            500 ;	test1.c:51: P2 = seg_position[1];
   011D 90 00 C5            501 	mov	dptr,#(_seg_position + 0x0001)
   0120 E0                  502 	movx	a,@dptr
   0121 F5 A0               503 	mov	_P2,a
                            504 ;	test1.c:52: P0=display_seg[i%100/10];
   0123 90 00 2F            505 	mov	dptr,#__modsint_PARM_2
   0126 74 64               506 	mov	a,#0x64
   0128 F0                  507 	movx	@dptr,a
   0129 E4                  508 	clr	a
   012A A3                  509 	inc	dptr
   012B F0                  510 	movx	@dptr,a
   012C 8A 82               511 	mov	dpl,r2
   012E 8B 83               512 	mov	dph,r3
   0130 C0 02               513 	push	ar2
   0132 C0 03               514 	push	ar3
   0134 12 07 60            515 	lcall	__modsint
   0137 AC 82               516 	mov	r4,dpl
   0139 AD 83               517 	mov	r5,dph
   013B 90 00 38            518 	mov	dptr,#__divsint_PARM_2
   013E 74 0A               519 	mov	a,#0x0A
   0140 F0                  520 	movx	@dptr,a
   0141 E4                  521 	clr	a
   0142 A3                  522 	inc	dptr
   0143 F0                  523 	movx	@dptr,a
   0144 8C 82               524 	mov	dpl,r4
   0146 8D 83               525 	mov	dph,r5
   0148 12 08 7B            526 	lcall	__divsint
   014B AC 82               527 	mov	r4,dpl
   014D AD 83               528 	mov	r5,dph
   014F EC                  529 	mov	a,r4
   0150 24 B9               530 	add	a,#_display_seg
   0152 F5 82               531 	mov	dpl,a
   0154 ED                  532 	mov	a,r5
   0155 34 00               533 	addc	a,#(_display_seg >> 8)
   0157 F5 83               534 	mov	dph,a
   0159 E0                  535 	movx	a,@dptr
   015A F5 80               536 	mov	_P0,a
                            537 ;	test1.c:53: delay(2);
   015C 90 00 02            538 	mov	dptr,#0x0002
   015F 12 00 7F            539 	lcall	_delay
   0162 D0 03               540 	pop	ar3
   0164 D0 02               541 	pop	ar2
                            542 ;	test1.c:54: P0=255;
   0166 75 80 FF            543 	mov	_P0,#0xFF
                            544 ;	test1.c:55: P2 = seg_position[0];
   0169 90 00 C4            545 	mov	dptr,#_seg_position
   016C E0                  546 	movx	a,@dptr
   016D F5 A0               547 	mov	_P2,a
                            548 ;	test1.c:56: P0=display_seg[i%10];
   016F 90 00 2F            549 	mov	dptr,#__modsint_PARM_2
   0172 74 0A               550 	mov	a,#0x0A
   0174 F0                  551 	movx	@dptr,a
   0175 E4                  552 	clr	a
   0176 A3                  553 	inc	dptr
   0177 F0                  554 	movx	@dptr,a
   0178 8A 82               555 	mov	dpl,r2
   017A 8B 83               556 	mov	dph,r3
   017C 12 07 60            557 	lcall	__modsint
   017F AA 82               558 	mov	r2,dpl
   0181 AB 83               559 	mov	r3,dph
   0183 EA                  560 	mov	a,r2
   0184 24 B9               561 	add	a,#_display_seg
   0186 F5 82               562 	mov	dpl,a
   0188 EB                  563 	mov	a,r3
   0189 34 00               564 	addc	a,#(_display_seg >> 8)
   018B F5 83               565 	mov	dph,a
   018D E0                  566 	movx	a,@dptr
   018E F5 80               567 	mov	_P0,a
                            568 ;	test1.c:57: delay(2);
   0190 90 00 02            569 	mov	dptr,#0x0002
   0193 02 00 7F            570 	ljmp	_delay
                            571 ;------------------------------------------------------------
                            572 ;Allocation info for local variables in function 'main'
                            573 ;------------------------------------------------------------
                            574 ;------------------------------------------------------------
                            575 ;	test1.c:63: main()
                            576 ;	-----------------------------------------
                            577 ;	 function main
                            578 ;	-----------------------------------------
   0196                     579 _main:
                            580 ;	test1.c:70: IE=0x8a;
   0196 75 A8 8A            581 	mov	_IE,#0x8A
                            582 ;	test1.c:71: TMOD=0x11;
   0199 75 89 11            583 	mov	_TMOD,#0x11
                            584 ;	test1.c:119: set_bit(P2,4,0);
   019C 53 A0 EF            585 	anl	_P2,#0xEF
                            586 ;	test1.c:126: i=0;
   019F 90 00 06            587 	mov	dptr,#_i
   01A2 E4                  588 	clr	a
   01A3 F0                  589 	movx	@dptr,a
   01A4 A3                  590 	inc	dptr
   01A5 F0                  591 	movx	@dptr,a
                            592 ;	test1.c:127: _c=0;
   01A6 90 00 08            593 	mov	dptr,#__c
   01A9 E4                  594 	clr	a
   01AA F0                  595 	movx	@dptr,a
   01AB A3                  596 	inc	dptr
   01AC F0                  597 	movx	@dptr,a
                            598 ;	test1.c:128: TH1=(65536-50000)/256;
   01AD 75 8D 3C            599 	mov	_TH1,#0x3C
                            600 ;	test1.c:129: TL1=(65536-50000)%256;
   01B0 75 8B B0            601 	mov	_TL1,#0xB0
                            602 ;	test1.c:130: TR1=1;
   01B3 D2 8E               603 	setb	_TR1
                            604 ;	test1.c:131: while(1)
   01B5                     605 00102$:
                            606 ;	test1.c:133: display(i);
   01B5 90 00 06            607 	mov	dptr,#_i
   01B8 E0                  608 	movx	a,@dptr
   01B9 FA                  609 	mov	r2,a
   01BA A3                  610 	inc	dptr
   01BB E0                  611 	movx	a,@dptr
   01BC FB                  612 	mov	r3,a
   01BD 8A 82               613 	mov	dpl,r2
   01BF 8B 83               614 	mov	dph,r3
   01C1 12 00 BA            615 	lcall	_display
                            616 ;	test1.c:134: i++;
   01C4 90 00 06            617 	mov	dptr,#_i
   01C7 E0                  618 	movx	a,@dptr
   01C8 FA                  619 	mov	r2,a
   01C9 A3                  620 	inc	dptr
   01CA E0                  621 	movx	a,@dptr
   01CB FB                  622 	mov	r3,a
   01CC 90 00 06            623 	mov	dptr,#_i
   01CF 74 01               624 	mov	a,#0x01
   01D1 2A                  625 	add	a,r2
   01D2 F0                  626 	movx	@dptr,a
   01D3 E4                  627 	clr	a
   01D4 3B                  628 	addc	a,r3
   01D5 A3                  629 	inc	dptr
   01D6 F0                  630 	movx	@dptr,a
                            631 ;	test1.c:135: set_bit(P2,4,!get_bit(P2,4));
   01D7 74 10               632 	mov	a,#0x10
   01D9 55 A0               633 	anl	a,_P2
   01DB FA                  634 	mov	r2,a
   01DC B4 01 00            635 	cjne	a,#0x01,00110$
   01DF                     636 00110$:
   01DF E4                  637 	clr	a
   01E0 33                  638 	rlc	a
   01E1 FA                  639 	mov	r2,a
   01E2 60 07               640 	jz	00106$
   01E4 74 10               641 	mov	a,#0x10
   01E6 45 A0               642 	orl	a,_P2
   01E8 FA                  643 	mov	r2,a
   01E9 80 06               644 	sjmp	00107$
   01EB                     645 00106$:
   01EB AB A0               646 	mov	r3,_P2
   01ED 74 EF               647 	mov	a,#0xEF
   01EF 5B                  648 	anl	a,r3
   01F0 FA                  649 	mov	r2,a
   01F1                     650 00107$:
   01F1 8A A0               651 	mov	_P2,r2
   01F3 80 C0               652 	sjmp	00102$
                            653 ;------------------------------------------------------------
                            654 ;Allocation info for local variables in function 'b'
                            655 ;------------------------------------------------------------
                            656 ;------------------------------------------------------------
                            657 ;	test1.c:160: void b(void) interrupt 1
                            658 ;	-----------------------------------------
                            659 ;	 function b
                            660 ;	-----------------------------------------
   01F5                     661 _b:
   01F5 C0 E0               662 	push	acc
   01F7 C0 82               663 	push	dpl
   01F9 C0 83               664 	push	dph
   01FB C0 02               665 	push	ar2
   01FD C0 03               666 	push	ar3
   01FF C0 D0               667 	push	psw
   0201 75 D0 00            668 	mov	psw,#0x00
                            669 ;	test1.c:163: set_bit(P2,4,!get_bit(P2,4));
   0204 74 10               670 	mov	a,#0x10
   0206 55 A0               671 	anl	a,_P2
   0208 FA                  672 	mov	r2,a
   0209 B4 01 00            673 	cjne	a,#0x01,00106$
   020C                     674 00106$:
   020C E4                  675 	clr	a
   020D 33                  676 	rlc	a
   020E FA                  677 	mov	r2,a
   020F 60 07               678 	jz	00103$
   0211 74 10               679 	mov	a,#0x10
   0213 45 A0               680 	orl	a,_P2
   0215 FA                  681 	mov	r2,a
   0216 80 06               682 	sjmp	00104$
   0218                     683 00103$:
   0218 AB A0               684 	mov	r3,_P2
   021A 74 EF               685 	mov	a,#0xEF
   021C 5B                  686 	anl	a,r3
   021D FA                  687 	mov	r2,a
   021E                     688 00104$:
   021E 8A A0               689 	mov	_P2,r2
                            690 ;	test1.c:164: TH0=t_h;
   0220 90 00 02            691 	mov	dptr,#_t_h
   0223 E0                  692 	movx	a,@dptr
   0224 F5 8C               693 	mov	_TH0,a
                            694 ;	test1.c:165: TL0=t_l;
   0226 90 00 03            695 	mov	dptr,#_t_l
   0229 E0                  696 	movx	a,@dptr
   022A F5 8A               697 	mov	_TL0,a
   022C D0 D0               698 	pop	psw
   022E D0 03               699 	pop	ar3
   0230 D0 02               700 	pop	ar2
   0232 D0 83               701 	pop	dph
   0234 D0 82               702 	pop	dpl
   0236 D0 E0               703 	pop	acc
   0238 32                  704 	reti
                            705 ;	eliminated unneeded push/pop b
                            706 ;------------------------------------------------------------
                            707 ;Allocation info for local variables in function 'c'
                            708 ;------------------------------------------------------------
                            709 ;sloc0                     Allocated with name '_c_sloc0_1_0'
                            710 ;------------------------------------------------------------
                            711 ;	test1.c:169: void c(void) interrupt 3
                            712 ;	-----------------------------------------
                            713 ;	 function c
                            714 ;	-----------------------------------------
   0239                     715 _c:
   0239 C0 21               716 	push	bits
   023B C0 E0               717 	push	acc
   023D C0 F0               718 	push	b
   023F C0 82               719 	push	dpl
   0241 C0 83               720 	push	dph
   0243 C0 02               721 	push	(0+2)
   0245 C0 03               722 	push	(0+3)
   0247 C0 04               723 	push	(0+4)
   0249 C0 05               724 	push	(0+5)
   024B C0 06               725 	push	(0+6)
   024D C0 07               726 	push	(0+7)
   024F C0 00               727 	push	(0+0)
   0251 C0 01               728 	push	(0+1)
   0253 C0 D0               729 	push	psw
   0255 75 D0 00            730 	mov	psw,#0x00
                            731 ;	test1.c:171: TH1=(65536-50000)/256;
   0258 75 8D 3C            732 	mov	_TH1,#0x3C
                            733 ;	test1.c:172: TL1=(65536-50000)%256;
   025B 75 8B B0            734 	mov	_TL1,#0xB0
                            735 ;	test1.c:173: _c+=50;
   025E 90 00 08            736 	mov	dptr,#__c
   0261 E0                  737 	movx	a,@dptr
   0262 FA                  738 	mov	r2,a
   0263 A3                  739 	inc	dptr
   0264 E0                  740 	movx	a,@dptr
   0265 FB                  741 	mov	r3,a
   0266 90 00 08            742 	mov	dptr,#__c
   0269 74 32               743 	mov	a,#0x32
   026B 2A                  744 	add	a,r2
   026C F0                  745 	movx	@dptr,a
   026D E4                  746 	clr	a
   026E 3B                  747 	addc	a,r3
   026F A3                  748 	inc	dptr
   0270 F0                  749 	movx	@dptr,a
                            750 ;	test1.c:175: if(_c<time[i])return;
   0271 90 00 06            751 	mov	dptr,#_i
   0274 E0                  752 	movx	a,@dptr
   0275 FA                  753 	mov	r2,a
   0276 A3                  754 	inc	dptr
   0277 E0                  755 	movx	a,@dptr
   0278 FB                  756 	mov	r3,a
   0279 8A 04               757 	mov	ar4,r2
   027B CC                  758 	xch	a,r4
   027C 25 E0               759 	add	a,acc
   027E CC                  760 	xch	a,r4
   027F 33                  761 	rlc	a
   0280 FD                  762 	mov	r5,a
   0281 EC                  763 	mov	a,r4
   0282 24 5A               764 	add	a,#_time
   0284 F5 82               765 	mov	dpl,a
   0286 ED                  766 	mov	a,r5
   0287 34 00               767 	addc	a,#(_time >> 8)
   0289 F5 83               768 	mov	dph,a
   028B E0                  769 	movx	a,@dptr
   028C FC                  770 	mov	r4,a
   028D A3                  771 	inc	dptr
   028E E0                  772 	movx	a,@dptr
   028F FD                  773 	mov	r5,a
   0290 90 00 08            774 	mov	dptr,#__c
   0293 E0                  775 	movx	a,@dptr
   0294 FE                  776 	mov	r6,a
   0295 A3                  777 	inc	dptr
   0296 E0                  778 	movx	a,@dptr
   0297 FF                  779 	mov	r7,a
   0298 C3                  780 	clr	c
   0299 EE                  781 	mov	a,r6
   029A 9C                  782 	subb	a,r4
   029B EF                  783 	mov	a,r7
   029C 9D                  784 	subb	a,r5
   029D 50 03               785 	jnc	00102$
   029F 02 03 70            786 	ljmp	00109$
   02A2                     787 00102$:
                            788 ;	test1.c:176: _c=0;
   02A2 90 00 08            789 	mov	dptr,#__c
   02A5 E4                  790 	clr	a
   02A6 F0                  791 	movx	@dptr,a
   02A7 A3                  792 	inc	dptr
   02A8 F0                  793 	movx	@dptr,a
                            794 ;	test1.c:177: if(s[i]==0)TR0=0;
   02A9 EA                  795 	mov	a,r2
   02AA 24 9C               796 	add	a,#_s
   02AC F5 82               797 	mov	dpl,a
   02AE EB                  798 	mov	a,r3
   02AF 34 00               799 	addc	a,#(_s >> 8)
   02B1 F5 83               800 	mov	dph,a
   02B3 E0                  801 	movx	a,@dptr
   02B4 FC                  802 	mov	r4,a
   02B5 70 02               803 	jnz	00104$
   02B7 C2 8C               804 	clr	_TR0
   02B9                     805 00104$:
                            806 ;	test1.c:178: t_h=(65536-(t[s[i]]))/256;
   02B9 C0 02               807 	push	ar2
   02BB C0 03               808 	push	ar3
   02BD EC                  809 	mov	a,r4
   02BE 2C                  810 	add	a,r4
   02BF 24 42               811 	add	a,#_t
   02C1 F5 82               812 	mov	dpl,a
   02C3 E4                  813 	clr	a
   02C4 34 00               814 	addc	a,#(_t >> 8)
   02C6 F5 83               815 	mov	dph,a
   02C8 E0                  816 	movx	a,@dptr
   02C9 FD                  817 	mov	r5,a
   02CA A3                  818 	inc	dptr
   02CB E0                  819 	movx	a,@dptr
   02CC FE                  820 	mov	r6,a
   02CD E4                  821 	clr	a
   02CE FF                  822 	mov	r7,a
   02CF F8                  823 	mov	r0,a
   02D0 C3                  824 	clr	c
   02D1 9D                  825 	subb	a,r5
   02D2 F5 08               826 	mov	_c_sloc0_1_0,a
   02D4 E4                  827 	clr	a
   02D5 9E                  828 	subb	a,r6
   02D6 F5 09               829 	mov	(_c_sloc0_1_0 + 1),a
   02D8 74 01               830 	mov	a,#0x01
   02DA 9F                  831 	subb	a,r7
   02DB F5 0A               832 	mov	(_c_sloc0_1_0 + 2),a
   02DD E4                  833 	clr	a
   02DE 98                  834 	subb	a,r0
   02DF F5 0B               835 	mov	(_c_sloc0_1_0 + 3),a
   02E1 90 00 1B            836 	mov	dptr,#__divslong_PARM_2
   02E4 E4                  837 	clr	a
   02E5 F0                  838 	movx	@dptr,a
   02E6 A3                  839 	inc	dptr
   02E7 74 01               840 	mov	a,#0x01
   02E9 F0                  841 	movx	@dptr,a
   02EA A3                  842 	inc	dptr
   02EB E4                  843 	clr	a
   02EC F0                  844 	movx	@dptr,a
   02ED A3                  845 	inc	dptr
   02EE F0                  846 	movx	@dptr,a
   02EF 85 08 82            847 	mov	dpl,_c_sloc0_1_0
   02F2 85 09 83            848 	mov	dph,(_c_sloc0_1_0 + 1)
   02F5 85 0A F0            849 	mov	b,(_c_sloc0_1_0 + 2)
   02F8 E5 0B               850 	mov	a,(_c_sloc0_1_0 + 3)
   02FA C0 04               851 	push	ar4
   02FC 12 05 8D            852 	lcall	__divslong
   02FF A9 82               853 	mov	r1,dpl
   0301 90 00 02            854 	mov	dptr,#_t_h
   0304 E9                  855 	mov	a,r1
   0305 F0                  856 	movx	@dptr,a
                            857 ;	test1.c:179: t_l=(65536-(t[s[i]]))%256;
   0306 90 00 0A            858 	mov	dptr,#__modslong_PARM_2
   0309 E4                  859 	clr	a
   030A F0                  860 	movx	@dptr,a
   030B A3                  861 	inc	dptr
   030C 74 01               862 	mov	a,#0x01
   030E F0                  863 	movx	@dptr,a
   030F A3                  864 	inc	dptr
   0310 E4                  865 	clr	a
   0311 F0                  866 	movx	@dptr,a
   0312 A3                  867 	inc	dptr
   0313 F0                  868 	movx	@dptr,a
   0314 85 08 82            869 	mov	dpl,_c_sloc0_1_0
   0317 85 09 83            870 	mov	dph,(_c_sloc0_1_0 + 1)
   031A 85 0A F0            871 	mov	b,(_c_sloc0_1_0 + 2)
   031D E5 0B               872 	mov	a,(_c_sloc0_1_0 + 3)
   031F C0 01               873 	push	ar1
   0321 12 03 8D            874 	lcall	__modslong
   0324 AA 82               875 	mov	r2,dpl
   0326 AD F0               876 	mov	r5,b
   0328 FE                  877 	mov	r6,a
   0329 D0 01               878 	pop	ar1
   032B D0 04               879 	pop	ar4
   032D 90 00 03            880 	mov	dptr,#_t_l
   0330 EA                  881 	mov	a,r2
   0331 F0                  882 	movx	@dptr,a
                            883 ;	test1.c:180: TH0=t_h;
   0332 89 8C               884 	mov	_TH0,r1
                            885 ;	test1.c:181: TL0=t_l;
   0334 8A 8A               886 	mov	_TL0,r2
                            887 ;	test1.c:182: if(s[i]!=0)TR0=1;
   0336 D0 03               888 	pop	ar3
   0338 D0 02               889 	pop	ar2
   033A EC                  890 	mov	a,r4
   033B 60 02               891 	jz	00106$
   033D D2 8C               892 	setb	_TR0
   033F                     893 00106$:
                            894 ;	test1.c:185: i+=1;
   033F 90 00 06            895 	mov	dptr,#_i
   0342 74 01               896 	mov	a,#0x01
   0344 2A                  897 	add	a,r2
   0345 F0                  898 	movx	@dptr,a
   0346 E4                  899 	clr	a
   0347 3B                  900 	addc	a,r3
   0348 A3                  901 	inc	dptr
   0349 F0                  902 	movx	@dptr,a
                            903 ;	test1.c:186: if(i>=count)i=0;
   034A 90 00 06            904 	mov	dptr,#_i
   034D E0                  905 	movx	a,@dptr
   034E FA                  906 	mov	r2,a
   034F A3                  907 	inc	dptr
   0350 E0                  908 	movx	a,@dptr
   0351 FB                  909 	mov	r3,a
   0352 90 00 C9            910 	mov	dptr,#_count
   0355 E0                  911 	movx	a,@dptr
   0356 FC                  912 	mov	r4,a
   0357 A3                  913 	inc	dptr
   0358 E0                  914 	movx	a,@dptr
   0359 FD                  915 	mov	r5,a
   035A C3                  916 	clr	c
   035B EA                  917 	mov	a,r2
   035C 9C                  918 	subb	a,r4
   035D EB                  919 	mov	a,r3
   035E 64 80               920 	xrl	a,#0x80
   0360 8D F0               921 	mov	b,r5
   0362 63 F0 80            922 	xrl	b,#0x80
   0365 95 F0               923 	subb	a,b
   0367 40 07               924 	jc	00109$
   0369 90 00 06            925 	mov	dptr,#_i
   036C E4                  926 	clr	a
   036D F0                  927 	movx	@dptr,a
   036E A3                  928 	inc	dptr
   036F F0                  929 	movx	@dptr,a
   0370                     930 00109$:
   0370 D0 D0               931 	pop	psw
   0372 D0 01               932 	pop	(0+1)
   0374 D0 00               933 	pop	(0+0)
   0376 D0 07               934 	pop	(0+7)
   0378 D0 06               935 	pop	(0+6)
   037A D0 05               936 	pop	(0+5)
   037C D0 04               937 	pop	(0+4)
   037E D0 03               938 	pop	(0+3)
   0380 D0 02               939 	pop	(0+2)
   0382 D0 83               940 	pop	dph
   0384 D0 82               941 	pop	dpl
   0386 D0 F0               942 	pop	b
   0388 D0 E0               943 	pop	acc
   038A D0 21               944 	pop	bits
   038C 32                  945 	reti
                            946 	.area CSEG    (CODE)
                            947 	.area CONST   (CODE)
                            948 	.area XINIT   (CODE)
   0991                     949 __xinit__t:
   0991 00 00               950 	.byte #0x00,#0x00
   0993 BC 03               951 	.byte #0xBC,#0x03
   0995 54 03               952 	.byte #0x54,#0x03
   0997 F7 02               953 	.byte #0xF7,#0x02
   0999 CC 02               954 	.byte #0xCC,#0x02
   099B 7E 02               955 	.byte #0x7E,#0x02
   099D 38 02               956 	.byte #0x38,#0x02
   099F FA 01               957 	.byte #0xFA,#0x01
   09A1 DE 01               958 	.byte #0xDE,#0x01
   09A3 AA 01               959 	.byte #0xAA,#0x01
   09A5 7B 01               960 	.byte #0x7B,#0x01
   09A7 00 00               961 	.byte #0x00,#0x00
   09A9                     962 __xinit__time:
   09A9 F4 01               963 	.byte #0xF4,#0x01
   09AB F4 01               964 	.byte #0xF4,#0x01
   09AD F4 01               965 	.byte #0xF4,#0x01
   09AF F4 01               966 	.byte #0xF4,#0x01
   09B1 F4 01               967 	.byte #0xF4,#0x01
   09B3 F4 01               968 	.byte #0xF4,#0x01
   09B5 F4 01               969 	.byte #0xF4,#0x01
   09B7 F4 01               970 	.byte #0xF4,#0x01
   09B9 F4 01               971 	.byte #0xF4,#0x01
   09BB F4 01               972 	.byte #0xF4,#0x01
   09BD F4 01               973 	.byte #0xF4,#0x01
   09BF F4 01               974 	.byte #0xF4,#0x01
   09C1 F4 01               975 	.byte #0xF4,#0x01
   09C3 F4 01               976 	.byte #0xF4,#0x01
   09C5 F4 01               977 	.byte #0xF4,#0x01
   09C7 F4 01               978 	.byte #0xF4,#0x01
   09C9 F4 01               979 	.byte #0xF4,#0x01
   09CB F4 01               980 	.byte #0xF4,#0x01
   09CD F4 01               981 	.byte #0xF4,#0x01
   09CF F4 01               982 	.byte #0xF4,#0x01
   09D1 F4 01               983 	.byte #0xF4,#0x01
   09D3 F4 01               984 	.byte #0xF4,#0x01
   09D5 F4 01               985 	.byte #0xF4,#0x01
   09D7 F4 01               986 	.byte #0xF4,#0x01
   09D9 F4 01               987 	.byte #0xF4,#0x01
   09DB F4 01               988 	.byte #0xF4,#0x01
   09DD F4 01               989 	.byte #0xF4,#0x01
   09DF F4 01               990 	.byte #0xF4,#0x01
   09E1 F4 01               991 	.byte #0xF4,#0x01
   09E3 F4 01               992 	.byte #0xF4,#0x01
   09E5 F4 01               993 	.byte #0xF4,#0x01
   09E7 F4 01               994 	.byte #0xF4,#0x01
   09E9 F4 01               995 	.byte #0xF4,#0x01
   09EB                     996 __xinit__s:
   09EB 01                  997 	.db #0x01
   09EC 01                  998 	.db #0x01
   09ED 02                  999 	.db #0x02
   09EE 01                 1000 	.db #0x01
   09EF 04                 1001 	.db #0x04
   09F0 03                 1002 	.db #0x03
   09F1 00                 1003 	.db #0x00
   09F2 01                 1004 	.db #0x01
   09F3 01                 1005 	.db #0x01
   09F4 02                 1006 	.db #0x02
   09F5 01                 1007 	.db #0x01
   09F6 05                 1008 	.db #0x05
   09F7 04                 1009 	.db #0x04
   09F8 00                 1010 	.db #0x00
   09F9 01                 1011 	.db #0x01
   09FA 01                 1012 	.db #0x01
   09FB 08                 1013 	.db #0x08
   09FC 06                 1014 	.db #0x06
   09FD 04                 1015 	.db #0x04
   09FE 03                 1016 	.db #0x03
   09FF 02                 1017 	.db #0x02
   0A00 00                 1018 	.db #0x00
   0A01 07                 1019 	.db #0x07
   0A02 07                 1020 	.db #0x07
   0A03 06                 1021 	.db #0x06
   0A04 04                 1022 	.db #0x04
   0A05 05                 1023 	.db #0x05
   0A06 04                 1024 	.db #0x04
   0A07 00                 1025 	.db #0x00
   0A08                    1026 __xinit__display_seg:
   0A08 C0                 1027 	.db #0xC0
   0A09 F9                 1028 	.db #0xF9
   0A0A A4                 1029 	.db #0xA4
   0A0B B0                 1030 	.db #0xB0
   0A0C 99                 1031 	.db #0x99
   0A0D 92                 1032 	.db #0x92
   0A0E 82                 1033 	.db #0x82
   0A0F F8                 1034 	.db #0xF8
   0A10 80                 1035 	.db #0x80
   0A11 90                 1036 	.db #0x90
   0A12 7F                 1037 	.db #0x7F
   0A13                    1038 __xinit__seg_position:
   0A13 06                 1039 	.db #0x06
   0A14 05                 1040 	.db #0x05
   0A15 03                 1041 	.db #0x03
   0A16 07                 1042 	.db #0x07
   0A17 00                 1043 	.db #0x00
   0A18                    1044 __xinit__count:
   0A18 1D 00              1045 	.byte #0x1D,#0x00
                           1046 	.area CABS    (ABS,CODE)
