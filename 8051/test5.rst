                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
                              4 ; This file was generated Sun May 13 21:12:21 2012
                              5 ;--------------------------------------------------------
                              6 	.module test5
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _seg_position
                             13 	.globl _display_seg
                             14 	.globl _main
                             15 	.globl _func
                             16 	.globl _display
                             17 	.globl _get_digits
                             18 	.globl _get_digit
                             19 	.globl _pow
                             20 	.globl _delay
                             21 	.globl _CY
                             22 	.globl _AC
                             23 	.globl _F0
                             24 	.globl _RS1
                             25 	.globl _RS0
                             26 	.globl _OV
                             27 	.globl _F1
                             28 	.globl _P
                             29 	.globl _PS
                             30 	.globl _PT1
                             31 	.globl _PX1
                             32 	.globl _PT0
                             33 	.globl _PX0
                             34 	.globl _RD
                             35 	.globl _WR
                             36 	.globl _T1
                             37 	.globl _T0
                             38 	.globl _INT1
                             39 	.globl _INT0
                             40 	.globl _TXD
                             41 	.globl _RXD
                             42 	.globl _P3_7
                             43 	.globl _P3_6
                             44 	.globl _P3_5
                             45 	.globl _P3_4
                             46 	.globl _P3_3
                             47 	.globl _P3_2
                             48 	.globl _P3_1
                             49 	.globl _P3_0
                             50 	.globl _EA
                             51 	.globl _ES
                             52 	.globl _ET1
                             53 	.globl _EX1
                             54 	.globl _ET0
                             55 	.globl _EX0
                             56 	.globl _P2_7
                             57 	.globl _P2_6
                             58 	.globl _P2_5
                             59 	.globl _P2_4
                             60 	.globl _P2_3
                             61 	.globl _P2_2
                             62 	.globl _P2_1
                             63 	.globl _P2_0
                             64 	.globl _SM0
                             65 	.globl _SM1
                             66 	.globl _SM2
                             67 	.globl _REN
                             68 	.globl _TB8
                             69 	.globl _RB8
                             70 	.globl _TI
                             71 	.globl _RI
                             72 	.globl _P1_7
                             73 	.globl _P1_6
                             74 	.globl _P1_5
                             75 	.globl _P1_4
                             76 	.globl _P1_3
                             77 	.globl _P1_2
                             78 	.globl _P1_1
                             79 	.globl _P1_0
                             80 	.globl _TF1
                             81 	.globl _TR1
                             82 	.globl _TF0
                             83 	.globl _TR0
                             84 	.globl _IE1
                             85 	.globl _IT1
                             86 	.globl _IE0
                             87 	.globl _IT0
                             88 	.globl _P0_7
                             89 	.globl _P0_6
                             90 	.globl _P0_5
                             91 	.globl _P0_4
                             92 	.globl _P0_3
                             93 	.globl _P0_2
                             94 	.globl _P0_1
                             95 	.globl _P0_0
                             96 	.globl _B
                             97 	.globl _ACC
                             98 	.globl _PSW
                             99 	.globl _IP
                            100 	.globl _P3
                            101 	.globl _IE
                            102 	.globl _P2
                            103 	.globl _SBUF
                            104 	.globl _SCON
                            105 	.globl _P1
                            106 	.globl _TH1
                            107 	.globl _TH0
                            108 	.globl _TL1
                            109 	.globl _TL0
                            110 	.globl _TMOD
                            111 	.globl _TCON
                            112 	.globl _PCON
                            113 	.globl _DPH
                            114 	.globl _DPL
                            115 	.globl _SP
                            116 	.globl _P0
                            117 	.globl _get_digit_PARM_2
                            118 	.globl _pow_PARM_2
                            119 ;--------------------------------------------------------
                            120 ; special function registers
                            121 ;--------------------------------------------------------
                            122 	.area RSEG    (DATA)
                    0080    123 _P0	=	0x0080
                    0081    124 _SP	=	0x0081
                    0082    125 _DPL	=	0x0082
                    0083    126 _DPH	=	0x0083
                    0087    127 _PCON	=	0x0087
                    0088    128 _TCON	=	0x0088
                    0089    129 _TMOD	=	0x0089
                    008A    130 _TL0	=	0x008a
                    008B    131 _TL1	=	0x008b
                    008C    132 _TH0	=	0x008c
                    008D    133 _TH1	=	0x008d
                    0090    134 _P1	=	0x0090
                    0098    135 _SCON	=	0x0098
                    0099    136 _SBUF	=	0x0099
                    00A0    137 _P2	=	0x00a0
                    00A8    138 _IE	=	0x00a8
                    00B0    139 _P3	=	0x00b0
                    00B8    140 _IP	=	0x00b8
                    00D0    141 _PSW	=	0x00d0
                    00E0    142 _ACC	=	0x00e0
                    00F0    143 _B	=	0x00f0
                            144 ;--------------------------------------------------------
                            145 ; special function bits
                            146 ;--------------------------------------------------------
                            147 	.area RSEG    (DATA)
                    0080    148 _P0_0	=	0x0080
                    0081    149 _P0_1	=	0x0081
                    0082    150 _P0_2	=	0x0082
                    0083    151 _P0_3	=	0x0083
                    0084    152 _P0_4	=	0x0084
                    0085    153 _P0_5	=	0x0085
                    0086    154 _P0_6	=	0x0086
                    0087    155 _P0_7	=	0x0087
                    0088    156 _IT0	=	0x0088
                    0089    157 _IE0	=	0x0089
                    008A    158 _IT1	=	0x008a
                    008B    159 _IE1	=	0x008b
                    008C    160 _TR0	=	0x008c
                    008D    161 _TF0	=	0x008d
                    008E    162 _TR1	=	0x008e
                    008F    163 _TF1	=	0x008f
                    0090    164 _P1_0	=	0x0090
                    0091    165 _P1_1	=	0x0091
                    0092    166 _P1_2	=	0x0092
                    0093    167 _P1_3	=	0x0093
                    0094    168 _P1_4	=	0x0094
                    0095    169 _P1_5	=	0x0095
                    0096    170 _P1_6	=	0x0096
                    0097    171 _P1_7	=	0x0097
                    0098    172 _RI	=	0x0098
                    0099    173 _TI	=	0x0099
                    009A    174 _RB8	=	0x009a
                    009B    175 _TB8	=	0x009b
                    009C    176 _REN	=	0x009c
                    009D    177 _SM2	=	0x009d
                    009E    178 _SM1	=	0x009e
                    009F    179 _SM0	=	0x009f
                    00A0    180 _P2_0	=	0x00a0
                    00A1    181 _P2_1	=	0x00a1
                    00A2    182 _P2_2	=	0x00a2
                    00A3    183 _P2_3	=	0x00a3
                    00A4    184 _P2_4	=	0x00a4
                    00A5    185 _P2_5	=	0x00a5
                    00A6    186 _P2_6	=	0x00a6
                    00A7    187 _P2_7	=	0x00a7
                    00A8    188 _EX0	=	0x00a8
                    00A9    189 _ET0	=	0x00a9
                    00AA    190 _EX1	=	0x00aa
                    00AB    191 _ET1	=	0x00ab
                    00AC    192 _ES	=	0x00ac
                    00AF    193 _EA	=	0x00af
                    00B0    194 _P3_0	=	0x00b0
                    00B1    195 _P3_1	=	0x00b1
                    00B2    196 _P3_2	=	0x00b2
                    00B3    197 _P3_3	=	0x00b3
                    00B4    198 _P3_4	=	0x00b4
                    00B5    199 _P3_5	=	0x00b5
                    00B6    200 _P3_6	=	0x00b6
                    00B7    201 _P3_7	=	0x00b7
                    00B0    202 _RXD	=	0x00b0
                    00B1    203 _TXD	=	0x00b1
                    00B2    204 _INT0	=	0x00b2
                    00B3    205 _INT1	=	0x00b3
                    00B4    206 _T0	=	0x00b4
                    00B5    207 _T1	=	0x00b5
                    00B6    208 _WR	=	0x00b6
                    00B7    209 _RD	=	0x00b7
                    00B8    210 _PX0	=	0x00b8
                    00B9    211 _PT0	=	0x00b9
                    00BA    212 _PX1	=	0x00ba
                    00BB    213 _PT1	=	0x00bb
                    00BC    214 _PS	=	0x00bc
                    00D0    215 _P	=	0x00d0
                    00D1    216 _F1	=	0x00d1
                    00D2    217 _OV	=	0x00d2
                    00D3    218 _RS0	=	0x00d3
                    00D4    219 _RS1	=	0x00d4
                    00D5    220 _F0	=	0x00d5
                    00D6    221 _AC	=	0x00d6
                    00D7    222 _CY	=	0x00d7
                            223 ;--------------------------------------------------------
                            224 ; overlayable register banks
                            225 ;--------------------------------------------------------
                            226 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     227 	.ds 8
                            228 ;--------------------------------------------------------
                            229 ; internal ram data
                            230 ;--------------------------------------------------------
                            231 	.area DSEG    (DATA)
   0008                     232 _pow_PARM_2:
   0008                     233 	.ds 4
   000C                     234 _pow_a_1_1:
   000C                     235 	.ds 4
   0010                     236 _get_digit_PARM_2:
   0010                     237 	.ds 1
                            238 ;--------------------------------------------------------
                            239 ; overlayable items in internal ram 
                            240 ;--------------------------------------------------------
                            241 	.area	OSEG    (OVR,DATA)
                            242 ;--------------------------------------------------------
                            243 ; Stack segment in internal ram 
                            244 ;--------------------------------------------------------
                            245 	.area	SSEG	(DATA)
   0015                     246 __start__stack:
   0015                     247 	.ds	1
                            248 
                            249 ;--------------------------------------------------------
                            250 ; indirectly addressable internal ram data
                            251 ;--------------------------------------------------------
                            252 	.area ISEG    (DATA)
                            253 ;--------------------------------------------------------
                            254 ; absolute internal ram data
                            255 ;--------------------------------------------------------
                            256 	.area IABS    (ABS,DATA)
                            257 	.area IABS    (ABS,DATA)
                            258 ;--------------------------------------------------------
                            259 ; bit data
                            260 ;--------------------------------------------------------
                            261 	.area BSEG    (BIT)
                            262 ;--------------------------------------------------------
                            263 ; paged external ram data
                            264 ;--------------------------------------------------------
                            265 	.area PSEG    (PAG,XDATA)
                            266 ;--------------------------------------------------------
                            267 ; external ram data
                            268 ;--------------------------------------------------------
                            269 	.area XSEG    (XDATA)
                            270 ;--------------------------------------------------------
                            271 ; absolute external ram data
                            272 ;--------------------------------------------------------
                            273 	.area XABS    (ABS,XDATA)
                            274 ;--------------------------------------------------------
                            275 ; external initialized ram data
                            276 ;--------------------------------------------------------
                            277 	.area XISEG   (XDATA)
                            278 	.area HOME    (CODE)
                            279 	.area GSINIT0 (CODE)
                            280 	.area GSINIT1 (CODE)
                            281 	.area GSINIT2 (CODE)
                            282 	.area GSINIT3 (CODE)
                            283 	.area GSINIT4 (CODE)
                            284 	.area GSINIT5 (CODE)
                            285 	.area GSINIT  (CODE)
                            286 	.area GSFINAL (CODE)
                            287 	.area CSEG    (CODE)
                            288 ;--------------------------------------------------------
                            289 ; interrupt vector 
                            290 ;--------------------------------------------------------
                            291 	.area HOME    (CODE)
   0000                     292 __interrupt_vect:
   0000 02 00 08            293 	ljmp	__sdcc_gsinit_startup
                            294 ;--------------------------------------------------------
                            295 ; global & static initialisations
                            296 ;--------------------------------------------------------
                            297 	.area HOME    (CODE)
                            298 	.area GSINIT  (CODE)
                            299 	.area GSFINAL (CODE)
                            300 	.area GSINIT  (CODE)
                            301 	.globl __sdcc_gsinit_startup
                            302 	.globl __sdcc_program_startup
                            303 	.globl __start__stack
                            304 	.globl __mcs51_genXINIT
                            305 	.globl __mcs51_genXRAMCLEAR
                            306 	.globl __mcs51_genRAMCLEAR
                            307 	.area GSFINAL (CODE)
   0061 02 00 03            308 	ljmp	__sdcc_program_startup
                            309 ;--------------------------------------------------------
                            310 ; Home
                            311 ;--------------------------------------------------------
                            312 	.area HOME    (CODE)
                            313 	.area HOME    (CODE)
   0003                     314 __sdcc_program_startup:
   0003 12 02 CB            315 	lcall	_main
                            316 ;	return from main will lock up
   0006 80 FE               317 	sjmp .
                            318 ;--------------------------------------------------------
                            319 ; code
                            320 ;--------------------------------------------------------
                            321 	.area CSEG    (CODE)
                            322 ;------------------------------------------------------------
                            323 ;Allocation info for local variables in function 'delay'
                            324 ;------------------------------------------------------------
                            325 ;i                         Allocated to registers r2 r3 
                            326 ;a                         Allocated to registers r4 r5 
                            327 ;b                         Allocated to registers r6 r7 
                            328 ;------------------------------------------------------------
                            329 ;	test5.c:7: void delay(int i)
                            330 ;	-----------------------------------------
                            331 ;	 function delay
                            332 ;	-----------------------------------------
   0064                     333 _delay:
                    0002    334 	ar2 = 0x02
                    0003    335 	ar3 = 0x03
                    0004    336 	ar4 = 0x04
                    0005    337 	ar5 = 0x05
                    0006    338 	ar6 = 0x06
                    0007    339 	ar7 = 0x07
                    0000    340 	ar0 = 0x00
                    0001    341 	ar1 = 0x01
   0064 AA 82               342 	mov	r2,dpl
   0066 AB 83               343 	mov	r3,dph
                            344 ;	test5.c:10: for(a=0;a<i;a++)
   0068 7C 00               345 	mov	r4,#0x00
   006A 7D 00               346 	mov	r5,#0x00
   006C                     347 00104$:
   006C C3                  348 	clr	c
   006D EC                  349 	mov	a,r4
   006E 9A                  350 	subb	a,r2
   006F ED                  351 	mov	a,r5
   0070 64 80               352 	xrl	a,#0x80
   0072 8B F0               353 	mov	b,r3
   0074 63 F0 80            354 	xrl	b,#0x80
   0077 95 F0               355 	subb	a,b
   0079 50 14               356 	jnc	00108$
                            357 ;	test5.c:12: for(b=0;b<120;b++);
   007B 7E 78               358 	mov	r6,#0x78
   007D 7F 00               359 	mov	r7,#0x00
   007F                     360 00103$:
   007F 1E                  361 	dec	r6
   0080 BE FF 01            362 	cjne	r6,#0xff,00117$
   0083 1F                  363 	dec	r7
   0084                     364 00117$:
   0084 EE                  365 	mov	a,r6
   0085 4F                  366 	orl	a,r7
   0086 70 F7               367 	jnz	00103$
                            368 ;	test5.c:10: for(a=0;a<i;a++)
   0088 0C                  369 	inc	r4
   0089 BC 00 E0            370 	cjne	r4,#0x00,00104$
   008C 0D                  371 	inc	r5
   008D 80 DD               372 	sjmp	00104$
   008F                     373 00108$:
   008F 22                  374 	ret
                            375 ;------------------------------------------------------------
                            376 ;Allocation info for local variables in function 'pow'
                            377 ;------------------------------------------------------------
                            378 ;b                         Allocated with name '_pow_PARM_2'
                            379 ;a                         Allocated with name '_pow_a_1_1'
                            380 ;tmp                       Allocated to registers r6 r7 r0 r1 
                            381 ;------------------------------------------------------------
                            382 ;	test5.c:22: unsigned long int pow(unsigned long int a, unsigned long int b)
                            383 ;	-----------------------------------------
                            384 ;	 function pow
                            385 ;	-----------------------------------------
   0090                     386 _pow:
   0090 85 82 0C            387 	mov	_pow_a_1_1,dpl
   0093 85 83 0D            388 	mov	(_pow_a_1_1 + 1),dph
   0096 85 F0 0E            389 	mov	(_pow_a_1_1 + 2),b
   0099 F5 0F               390 	mov	(_pow_a_1_1 + 3),a
                            391 ;	test5.c:24: unsigned long int tmp=1;
   009B 7E 01               392 	mov	r6,#0x01
   009D 7F 00               393 	mov	r7,#0x00
   009F 78 00               394 	mov	r0,#0x00
   00A1 79 00               395 	mov	r1,#0x00
                            396 ;	test5.c:25: while(b>0)
   00A3 AA 08               397 	mov	r2,_pow_PARM_2
   00A5 AB 09               398 	mov	r3,(_pow_PARM_2 + 1)
   00A7 AC 0A               399 	mov	r4,(_pow_PARM_2 + 2)
   00A9 AD 0B               400 	mov	r5,(_pow_PARM_2 + 3)
   00AB                     401 00101$:
   00AB EA                  402 	mov	a,r2
   00AC 4B                  403 	orl	a,r3
   00AD 4C                  404 	orl	a,r4
   00AE 4D                  405 	orl	a,r5
   00AF 60 3C               406 	jz	00103$
                            407 ;	test5.c:27: tmp=tmp*a;
   00B1 85 0C 11            408 	mov	__mullong_PARM_2,_pow_a_1_1
   00B4 85 0D 12            409 	mov	(__mullong_PARM_2 + 1),(_pow_a_1_1 + 1)
   00B7 85 0E 13            410 	mov	(__mullong_PARM_2 + 2),(_pow_a_1_1 + 2)
   00BA 85 0F 14            411 	mov	(__mullong_PARM_2 + 3),(_pow_a_1_1 + 3)
   00BD 8E 82               412 	mov	dpl,r6
   00BF 8F 83               413 	mov	dph,r7
   00C1 88 F0               414 	mov	b,r0
   00C3 E9                  415 	mov	a,r1
   00C4 C0 02               416 	push	ar2
   00C6 C0 03               417 	push	ar3
   00C8 C0 04               418 	push	ar4
   00CA C0 05               419 	push	ar5
   00CC 12 03 D1            420 	lcall	__mullong
   00CF AE 82               421 	mov	r6,dpl
   00D1 AF 83               422 	mov	r7,dph
   00D3 A8 F0               423 	mov	r0,b
   00D5 F9                  424 	mov	r1,a
   00D6 D0 05               425 	pop	ar5
   00D8 D0 04               426 	pop	ar4
   00DA D0 03               427 	pop	ar3
   00DC D0 02               428 	pop	ar2
                            429 ;	test5.c:28: b--;
   00DE 1A                  430 	dec	r2
   00DF BA FF C9            431 	cjne	r2,#0xff,00101$
   00E2 1B                  432 	dec	r3
   00E3 BB FF C5            433 	cjne	r3,#0xff,00101$
   00E6 1C                  434 	dec	r4
   00E7 BC FF C1            435 	cjne	r4,#0xff,00101$
   00EA 1D                  436 	dec	r5
   00EB 80 BE               437 	sjmp	00101$
   00ED                     438 00103$:
                            439 ;	test5.c:30: return tmp;
   00ED 8E 82               440 	mov	dpl,r6
   00EF 8F 83               441 	mov	dph,r7
   00F1 88 F0               442 	mov	b,r0
   00F3 E9                  443 	mov	a,r1
   00F4 22                  444 	ret
                            445 ;------------------------------------------------------------
                            446 ;Allocation info for local variables in function 'get_digit'
                            447 ;------------------------------------------------------------
                            448 ;d                         Allocated with name '_get_digit_PARM_2'
                            449 ;i                         Allocated to registers r2 r3 r4 r5 
                            450 ;------------------------------------------------------------
                            451 ;	test5.c:32: unsigned char get_digit(unsigned long int i, unsigned char d)
                            452 ;	-----------------------------------------
                            453 ;	 function get_digit
                            454 ;	-----------------------------------------
   00F5                     455 _get_digit:
   00F5 AA 82               456 	mov	r2,dpl
   00F7 AB 83               457 	mov	r3,dph
   00F9 AC F0               458 	mov	r4,b
   00FB FD                  459 	mov	r5,a
                            460 ;	test5.c:35: return i%(pow(10,d)*10)/pow(10,d);
   00FC AE 10               461 	mov	r6,_get_digit_PARM_2
   00FE 7F 00               462 	mov	r7,#0x00
   0100 78 00               463 	mov	r0,#0x00
   0102 79 00               464 	mov	r1,#0x00
   0104 8E 08               465 	mov	_pow_PARM_2,r6
   0106 8F 09               466 	mov	(_pow_PARM_2 + 1),r7
   0108 88 0A               467 	mov	(_pow_PARM_2 + 2),r0
   010A 89 0B               468 	mov	(_pow_PARM_2 + 3),r1
   010C 90 00 0A            469 	mov	dptr,#(0x0A&0x00ff)
   010F E4                  470 	clr	a
   0110 F5 F0               471 	mov	b,a
   0112 C0 02               472 	push	ar2
   0114 C0 03               473 	push	ar3
   0116 C0 04               474 	push	ar4
   0118 C0 05               475 	push	ar5
   011A C0 06               476 	push	ar6
   011C C0 07               477 	push	ar7
   011E C0 00               478 	push	ar0
   0120 C0 01               479 	push	ar1
   0122 12 00 90            480 	lcall	_pow
   0125 85 82 11            481 	mov	__mullong_PARM_2,dpl
   0128 85 83 12            482 	mov	(__mullong_PARM_2 + 1),dph
   012B 85 F0 13            483 	mov	(__mullong_PARM_2 + 2),b
   012E F5 14               484 	mov	(__mullong_PARM_2 + 3),a
   0130 90 00 0A            485 	mov	dptr,#(0x0A&0x00ff)
   0133 E4                  486 	clr	a
   0134 F5 F0               487 	mov	b,a
   0136 12 03 D1            488 	lcall	__mullong
   0139 85 82 11            489 	mov	__modulong_PARM_2,dpl
   013C 85 83 12            490 	mov	(__modulong_PARM_2 + 1),dph
   013F 85 F0 13            491 	mov	(__modulong_PARM_2 + 2),b
   0142 F5 14               492 	mov	(__modulong_PARM_2 + 3),a
   0144 D0 01               493 	pop	ar1
   0146 D0 00               494 	pop	ar0
   0148 D0 07               495 	pop	ar7
   014A D0 06               496 	pop	ar6
   014C D0 05               497 	pop	ar5
   014E D0 04               498 	pop	ar4
   0150 D0 03               499 	pop	ar3
   0152 D0 02               500 	pop	ar2
   0154 8A 82               501 	mov	dpl,r2
   0156 8B 83               502 	mov	dph,r3
   0158 8C F0               503 	mov	b,r4
   015A ED                  504 	mov	a,r5
   015B C0 06               505 	push	ar6
   015D C0 07               506 	push	ar7
   015F C0 00               507 	push	ar0
   0161 C0 01               508 	push	ar1
   0163 12 02 E9            509 	lcall	__modulong
   0166 AA 82               510 	mov	r2,dpl
   0168 AB 83               511 	mov	r3,dph
   016A AC F0               512 	mov	r4,b
   016C FD                  513 	mov	r5,a
   016D D0 01               514 	pop	ar1
   016F D0 00               515 	pop	ar0
   0171 D0 07               516 	pop	ar7
   0173 D0 06               517 	pop	ar6
   0175 8E 08               518 	mov	_pow_PARM_2,r6
   0177 8F 09               519 	mov	(_pow_PARM_2 + 1),r7
   0179 88 0A               520 	mov	(_pow_PARM_2 + 2),r0
   017B 89 0B               521 	mov	(_pow_PARM_2 + 3),r1
   017D 90 00 0A            522 	mov	dptr,#(0x0A&0x00ff)
   0180 E4                  523 	clr	a
   0181 F5 F0               524 	mov	b,a
   0183 C0 02               525 	push	ar2
   0185 C0 03               526 	push	ar3
   0187 C0 04               527 	push	ar4
   0189 C0 05               528 	push	ar5
   018B 12 00 90            529 	lcall	_pow
   018E 85 82 11            530 	mov	__divulong_PARM_2,dpl
   0191 85 83 12            531 	mov	(__divulong_PARM_2 + 1),dph
   0194 85 F0 13            532 	mov	(__divulong_PARM_2 + 2),b
   0197 F5 14               533 	mov	(__divulong_PARM_2 + 3),a
   0199 D0 05               534 	pop	ar5
   019B D0 04               535 	pop	ar4
   019D D0 03               536 	pop	ar3
   019F D0 02               537 	pop	ar2
   01A1 8A 82               538 	mov	dpl,r2
   01A3 8B 83               539 	mov	dph,r3
   01A5 8C F0               540 	mov	b,r4
   01A7 ED                  541 	mov	a,r5
   01A8 02 03 6C            542 	ljmp	__divulong
                            543 ;------------------------------------------------------------
                            544 ;Allocation info for local variables in function 'get_digits'
                            545 ;------------------------------------------------------------
                            546 ;i                         Allocated to registers r2 r3 r4 r5 
                            547 ;tmp                       Allocated to registers r6 
                            548 ;------------------------------------------------------------
                            549 ;	test5.c:37: unsigned char get_digits(unsigned long int i)
                            550 ;	-----------------------------------------
                            551 ;	 function get_digits
                            552 ;	-----------------------------------------
   01AB                     553 _get_digits:
   01AB AA 82               554 	mov	r2,dpl
   01AD AB 83               555 	mov	r3,dph
   01AF AC F0               556 	mov	r4,b
   01B1 FD                  557 	mov	r5,a
                            558 ;	test5.c:40: while(i>0)
   01B2 7E 00               559 	mov	r6,#0x00
   01B4                     560 00101$:
   01B4 EA                  561 	mov	a,r2
   01B5 4B                  562 	orl	a,r3
   01B6 4C                  563 	orl	a,r4
   01B7 4D                  564 	orl	a,r5
   01B8 60 22               565 	jz	00103$
                            566 ;	test5.c:42: tmp++;
   01BA 0E                  567 	inc	r6
                            568 ;	test5.c:43: i=i/10;
   01BB 75 11 0A            569 	mov	__divulong_PARM_2,#0x0A
   01BE E4                  570 	clr	a
   01BF F5 12               571 	mov	(__divulong_PARM_2 + 1),a
   01C1 F5 13               572 	mov	(__divulong_PARM_2 + 2),a
   01C3 F5 14               573 	mov	(__divulong_PARM_2 + 3),a
   01C5 8A 82               574 	mov	dpl,r2
   01C7 8B 83               575 	mov	dph,r3
   01C9 8C F0               576 	mov	b,r4
   01CB ED                  577 	mov	a,r5
   01CC C0 06               578 	push	ar6
   01CE 12 03 6C            579 	lcall	__divulong
   01D1 AA 82               580 	mov	r2,dpl
   01D3 AB 83               581 	mov	r3,dph
   01D5 AC F0               582 	mov	r4,b
   01D7 FD                  583 	mov	r5,a
   01D8 D0 06               584 	pop	ar6
   01DA 80 D8               585 	sjmp	00101$
   01DC                     586 00103$:
                            587 ;	test5.c:45: return tmp;
   01DC 8E 82               588 	mov	dpl,r6
   01DE 22                  589 	ret
                            590 ;------------------------------------------------------------
                            591 ;Allocation info for local variables in function 'display'
                            592 ;------------------------------------------------------------
                            593 ;i                         Allocated to registers r2 r3 r4 r5 
                            594 ;d0                        Allocated to registers 
                            595 ;d1                        Allocated to registers 
                            596 ;d2                        Allocated to registers 
                            597 ;d3                        Allocated to registers r6 
                            598 ;------------------------------------------------------------
                            599 ;	test5.c:47: void display(unsigned long int i)
                            600 ;	-----------------------------------------
                            601 ;	 function display
                            602 ;	-----------------------------------------
   01DF                     603 _display:
   01DF AA 82               604 	mov	r2,dpl
   01E1 AB 83               605 	mov	r3,dph
   01E3 AC F0               606 	mov	r4,b
   01E5 FD                  607 	mov	r5,a
                            608 ;	test5.c:53: unsigned char d3=0xFF;
   01E6 7E FF               609 	mov	r6,#0xFF
                            610 ;	test5.c:88: P0=255;
   01E8 75 80 FF            611 	mov	_P0,#0xFF
                            612 ;	test5.c:89: P2 = seg_position[3];
   01EB 90 04 51            613 	mov	dptr,#(_seg_position + 0x0003)
   01EE E4                  614 	clr	a
   01EF 93                  615 	movc	a,@a+dptr
   01F0 F5 A0               616 	mov	_P2,a
                            617 ;	test5.c:90: if(i>=10000)
   01F2 C3                  618 	clr	c
   01F3 EA                  619 	mov	a,r2
   01F4 94 10               620 	subb	a,#0x10
   01F6 EB                  621 	mov	a,r3
   01F7 94 27               622 	subb	a,#0x27
   01F9 EC                  623 	mov	a,r4
   01FA 94 00               624 	subb	a,#0x00
   01FC ED                  625 	mov	a,r5
   01FD 94 00               626 	subb	a,#0x00
   01FF 40 02               627 	jc	00102$
                            628 ;	test5.c:92: d3=0x7F;
   0201 7E 7F               629 	mov	r6,#0x7F
   0203                     630 00102$:
                            631 ;	test5.c:94: P0=display_seg[get_digit(i,3)] & d3;
   0203 75 10 03            632 	mov	_get_digit_PARM_2,#0x03
   0206 8A 82               633 	mov	dpl,r2
   0208 8B 83               634 	mov	dph,r3
   020A 8C F0               635 	mov	b,r4
   020C ED                  636 	mov	a,r5
   020D C0 02               637 	push	ar2
   020F C0 03               638 	push	ar3
   0211 C0 04               639 	push	ar4
   0213 C0 05               640 	push	ar5
   0215 C0 06               641 	push	ar6
   0217 12 00 F5            642 	lcall	_get_digit
   021A AF 82               643 	mov	r7,dpl
   021C D0 06               644 	pop	ar6
   021E EF                  645 	mov	a,r7
   021F 90 04 43            646 	mov	dptr,#_display_seg
   0222 93                  647 	movc	a,@a+dptr
   0223 FF                  648 	mov	r7,a
   0224 EE                  649 	mov	a,r6
   0225 5F                  650 	anl	a,r7
   0226 F5 80               651 	mov	_P0,a
                            652 ;	test5.c:95: delay(2);
   0228 90 00 02            653 	mov	dptr,#0x0002
   022B 12 00 64            654 	lcall	_delay
   022E D0 05               655 	pop	ar5
   0230 D0 04               656 	pop	ar4
   0232 D0 03               657 	pop	ar3
   0234 D0 02               658 	pop	ar2
                            659 ;	test5.c:97: P0=255;
   0236 75 80 FF            660 	mov	_P0,#0xFF
                            661 ;	test5.c:98: P2 = seg_position[2];
   0239 90 04 50            662 	mov	dptr,#(_seg_position + 0x0002)
   023C E4                  663 	clr	a
   023D 93                  664 	movc	a,@a+dptr
   023E F5 A0               665 	mov	_P2,a
                            666 ;	test5.c:99: P0=display_seg[get_digit(i,2)] & d2;
   0240 75 10 02            667 	mov	_get_digit_PARM_2,#0x02
   0243 8A 82               668 	mov	dpl,r2
   0245 8B 83               669 	mov	dph,r3
   0247 8C F0               670 	mov	b,r4
   0249 ED                  671 	mov	a,r5
   024A C0 02               672 	push	ar2
   024C C0 03               673 	push	ar3
   024E C0 04               674 	push	ar4
   0250 C0 05               675 	push	ar5
   0252 12 00 F5            676 	lcall	_get_digit
   0255 E5 82               677 	mov	a,dpl
   0257 90 04 43            678 	mov	dptr,#_display_seg
   025A 93                  679 	movc	a,@a+dptr
   025B F5 80               680 	mov	_P0,a
                            681 ;	test5.c:100: delay(2);
   025D 90 00 02            682 	mov	dptr,#0x0002
   0260 12 00 64            683 	lcall	_delay
   0263 D0 05               684 	pop	ar5
   0265 D0 04               685 	pop	ar4
   0267 D0 03               686 	pop	ar3
   0269 D0 02               687 	pop	ar2
                            688 ;	test5.c:101: P0=255;
   026B 75 80 FF            689 	mov	_P0,#0xFF
                            690 ;	test5.c:102: P2 = seg_position[1];
   026E 90 04 4F            691 	mov	dptr,#(_seg_position + 0x0001)
   0271 E4                  692 	clr	a
   0272 93                  693 	movc	a,@a+dptr
   0273 F5 A0               694 	mov	_P2,a
                            695 ;	test5.c:103: P0=display_seg[get_digit(i,1)] & d1;
   0275 75 10 01            696 	mov	_get_digit_PARM_2,#0x01
   0278 8A 82               697 	mov	dpl,r2
   027A 8B 83               698 	mov	dph,r3
   027C 8C F0               699 	mov	b,r4
   027E ED                  700 	mov	a,r5
   027F C0 02               701 	push	ar2
   0281 C0 03               702 	push	ar3
   0283 C0 04               703 	push	ar4
   0285 C0 05               704 	push	ar5
   0287 12 00 F5            705 	lcall	_get_digit
   028A E5 82               706 	mov	a,dpl
   028C 90 04 43            707 	mov	dptr,#_display_seg
   028F 93                  708 	movc	a,@a+dptr
   0290 F5 80               709 	mov	_P0,a
                            710 ;	test5.c:104: delay(2);
   0292 90 00 02            711 	mov	dptr,#0x0002
   0295 12 00 64            712 	lcall	_delay
   0298 D0 05               713 	pop	ar5
   029A D0 04               714 	pop	ar4
   029C D0 03               715 	pop	ar3
   029E D0 02               716 	pop	ar2
                            717 ;	test5.c:105: P0=255;
   02A0 75 80 FF            718 	mov	_P0,#0xFF
                            719 ;	test5.c:106: P2 = seg_position[0];
   02A3 90 04 4E            720 	mov	dptr,#_seg_position
   02A6 E4                  721 	clr	a
   02A7 93                  722 	movc	a,@a+dptr
   02A8 F5 A0               723 	mov	_P2,a
                            724 ;	test5.c:107: P0=display_seg[get_digit(i,0)] & d0;
   02AA 75 10 00            725 	mov	_get_digit_PARM_2,#0x00
   02AD 8A 82               726 	mov	dpl,r2
   02AF 8B 83               727 	mov	dph,r3
   02B1 8C F0               728 	mov	b,r4
   02B3 ED                  729 	mov	a,r5
   02B4 12 00 F5            730 	lcall	_get_digit
   02B7 E5 82               731 	mov	a,dpl
   02B9 90 04 43            732 	mov	dptr,#_display_seg
   02BC 93                  733 	movc	a,@a+dptr
   02BD F5 80               734 	mov	_P0,a
                            735 ;	test5.c:108: delay(2);
   02BF 90 00 02            736 	mov	dptr,#0x0002
   02C2 02 00 64            737 	ljmp	_delay
                            738 ;------------------------------------------------------------
                            739 ;Allocation info for local variables in function 'func'
                            740 ;------------------------------------------------------------
                            741 ;------------------------------------------------------------
                            742 ;	test5.c:112: void func()
                            743 ;	-----------------------------------------
                            744 ;	 function func
                            745 ;	-----------------------------------------
   02C5                     746 _func:
                            747 ;	test5.c:114: P2=~P2;
   02C5 E5 A0               748 	mov	a,_P2
   02C7 F4                  749 	cpl	a
   02C8 F5 A0               750 	mov	_P2,a
   02CA 22                  751 	ret
                            752 ;------------------------------------------------------------
                            753 ;Allocation info for local variables in function 'main'
                            754 ;------------------------------------------------------------
                            755 ;f                         Allocated to registers 
                            756 ;------------------------------------------------------------
                            757 ;	test5.c:117: void main()
                            758 ;	-----------------------------------------
                            759 ;	 function main
                            760 ;	-----------------------------------------
   02CB                     761 _main:
                            762 ;	test5.c:121: while(1)
   02CB                     763 00102$:
                            764 ;	test5.c:123: display(get_digits(1234567));
   02CB 90 D6 87            765 	mov	dptr,#0xD687
   02CE 75 F0 12            766 	mov	b,#0x12
   02D1 E4                  767 	clr	a
   02D2 12 01 AB            768 	lcall	_get_digits
   02D5 AA 82               769 	mov	r2,dpl
   02D7 7B 00               770 	mov	r3,#0x00
   02D9 7C 00               771 	mov	r4,#0x00
   02DB 7D 00               772 	mov	r5,#0x00
   02DD 8A 82               773 	mov	dpl,r2
   02DF 8B 83               774 	mov	dph,r3
   02E1 8C F0               775 	mov	b,r4
   02E3 ED                  776 	mov	a,r5
   02E4 12 01 DF            777 	lcall	_display
   02E7 80 E2               778 	sjmp	00102$
                            779 	.area CSEG    (CODE)
                            780 	.area CONST   (CODE)
   0443                     781 _display_seg:
   0443 C0                  782 	.db #0xC0
   0444 F9                  783 	.db #0xF9
   0445 A4                  784 	.db #0xA4
   0446 B0                  785 	.db #0xB0
   0447 99                  786 	.db #0x99
   0448 92                  787 	.db #0x92
   0449 82                  788 	.db #0x82
   044A F8                  789 	.db #0xF8
   044B 80                  790 	.db #0x80
   044C 90                  791 	.db #0x90
   044D 7F                  792 	.db #0x7F
   044E                     793 _seg_position:
   044E FE                  794 	.db #0xFE
   044F FD                  795 	.db #0xFD
   0450 FB                  796 	.db #0xFB
   0451 F7                  797 	.db #0xF7
   0452 F0                  798 	.db #0xF0
                            799 	.area XINIT   (CODE)
                            800 	.area CABS    (ABS,CODE)
