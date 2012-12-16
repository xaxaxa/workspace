                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 3.1.0 #7066 (Feb 26 2012) (Linux)
                              4 ; This file was generated Sat Dec  1 16:13:19 2012
                              5 ;--------------------------------------------------------
                              6 	.module asdf
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _seg_position
                             13 	.globl _display_seg
                             14 	.globl _main
                             15 	.globl _asd
                             16 	.globl _display
                             17 	.globl _delay
                             18 	.globl _CY
                             19 	.globl _AC
                             20 	.globl _F0
                             21 	.globl _RS1
                             22 	.globl _RS0
                             23 	.globl _OV
                             24 	.globl _F1
                             25 	.globl _P
                             26 	.globl _PS
                             27 	.globl _PT1
                             28 	.globl _PX1
                             29 	.globl _PT0
                             30 	.globl _PX0
                             31 	.globl _RD
                             32 	.globl _WR
                             33 	.globl _T1
                             34 	.globl _T0
                             35 	.globl _INT1
                             36 	.globl _INT0
                             37 	.globl _TXD
                             38 	.globl _RXD
                             39 	.globl _P3_7
                             40 	.globl _P3_6
                             41 	.globl _P3_5
                             42 	.globl _P3_4
                             43 	.globl _P3_3
                             44 	.globl _P3_2
                             45 	.globl _P3_1
                             46 	.globl _P3_0
                             47 	.globl _EA
                             48 	.globl _ES
                             49 	.globl _ET1
                             50 	.globl _EX1
                             51 	.globl _ET0
                             52 	.globl _EX0
                             53 	.globl _P2_7
                             54 	.globl _P2_6
                             55 	.globl _P2_5
                             56 	.globl _P2_4
                             57 	.globl _P2_3
                             58 	.globl _P2_2
                             59 	.globl _P2_1
                             60 	.globl _P2_0
                             61 	.globl _SM0
                             62 	.globl _SM1
                             63 	.globl _SM2
                             64 	.globl _REN
                             65 	.globl _TB8
                             66 	.globl _RB8
                             67 	.globl _TI
                             68 	.globl _RI
                             69 	.globl _P1_7
                             70 	.globl _P1_6
                             71 	.globl _P1_5
                             72 	.globl _P1_4
                             73 	.globl _P1_3
                             74 	.globl _P1_2
                             75 	.globl _P1_1
                             76 	.globl _P1_0
                             77 	.globl _TF1
                             78 	.globl _TR1
                             79 	.globl _TF0
                             80 	.globl _TR0
                             81 	.globl _IE1
                             82 	.globl _IT1
                             83 	.globl _IE0
                             84 	.globl _IT0
                             85 	.globl _P0_7
                             86 	.globl _P0_6
                             87 	.globl _P0_5
                             88 	.globl _P0_4
                             89 	.globl _P0_3
                             90 	.globl _P0_2
                             91 	.globl _P0_1
                             92 	.globl _P0_0
                             93 	.globl _B
                             94 	.globl _ACC
                             95 	.globl _PSW
                             96 	.globl _IP
                             97 	.globl _P3
                             98 	.globl _IE
                             99 	.globl _P2
                            100 	.globl _SBUF
                            101 	.globl _SCON
                            102 	.globl _P1
                            103 	.globl _TH1
                            104 	.globl _TH0
                            105 	.globl _TL1
                            106 	.globl _TL0
                            107 	.globl _TMOD
                            108 	.globl _TCON
                            109 	.globl _PCON
                            110 	.globl _DPH
                            111 	.globl _DPL
                            112 	.globl _SP
                            113 	.globl _P0
                            114 ;--------------------------------------------------------
                            115 ; special function registers
                            116 ;--------------------------------------------------------
                            117 	.area RSEG    (ABS,DATA)
   0000                     118 	.org 0x0000
                    0080    119 _P0	=	0x0080
                    0081    120 _SP	=	0x0081
                    0082    121 _DPL	=	0x0082
                    0083    122 _DPH	=	0x0083
                    0087    123 _PCON	=	0x0087
                    0088    124 _TCON	=	0x0088
                    0089    125 _TMOD	=	0x0089
                    008A    126 _TL0	=	0x008a
                    008B    127 _TL1	=	0x008b
                    008C    128 _TH0	=	0x008c
                    008D    129 _TH1	=	0x008d
                    0090    130 _P1	=	0x0090
                    0098    131 _SCON	=	0x0098
                    0099    132 _SBUF	=	0x0099
                    00A0    133 _P2	=	0x00a0
                    00A8    134 _IE	=	0x00a8
                    00B0    135 _P3	=	0x00b0
                    00B8    136 _IP	=	0x00b8
                    00D0    137 _PSW	=	0x00d0
                    00E0    138 _ACC	=	0x00e0
                    00F0    139 _B	=	0x00f0
                            140 ;--------------------------------------------------------
                            141 ; special function bits
                            142 ;--------------------------------------------------------
                            143 	.area RSEG    (ABS,DATA)
   0000                     144 	.org 0x0000
                    0080    145 _P0_0	=	0x0080
                    0081    146 _P0_1	=	0x0081
                    0082    147 _P0_2	=	0x0082
                    0083    148 _P0_3	=	0x0083
                    0084    149 _P0_4	=	0x0084
                    0085    150 _P0_5	=	0x0085
                    0086    151 _P0_6	=	0x0086
                    0087    152 _P0_7	=	0x0087
                    0088    153 _IT0	=	0x0088
                    0089    154 _IE0	=	0x0089
                    008A    155 _IT1	=	0x008a
                    008B    156 _IE1	=	0x008b
                    008C    157 _TR0	=	0x008c
                    008D    158 _TF0	=	0x008d
                    008E    159 _TR1	=	0x008e
                    008F    160 _TF1	=	0x008f
                    0090    161 _P1_0	=	0x0090
                    0091    162 _P1_1	=	0x0091
                    0092    163 _P1_2	=	0x0092
                    0093    164 _P1_3	=	0x0093
                    0094    165 _P1_4	=	0x0094
                    0095    166 _P1_5	=	0x0095
                    0096    167 _P1_6	=	0x0096
                    0097    168 _P1_7	=	0x0097
                    0098    169 _RI	=	0x0098
                    0099    170 _TI	=	0x0099
                    009A    171 _RB8	=	0x009a
                    009B    172 _TB8	=	0x009b
                    009C    173 _REN	=	0x009c
                    009D    174 _SM2	=	0x009d
                    009E    175 _SM1	=	0x009e
                    009F    176 _SM0	=	0x009f
                    00A0    177 _P2_0	=	0x00a0
                    00A1    178 _P2_1	=	0x00a1
                    00A2    179 _P2_2	=	0x00a2
                    00A3    180 _P2_3	=	0x00a3
                    00A4    181 _P2_4	=	0x00a4
                    00A5    182 _P2_5	=	0x00a5
                    00A6    183 _P2_6	=	0x00a6
                    00A7    184 _P2_7	=	0x00a7
                    00A8    185 _EX0	=	0x00a8
                    00A9    186 _ET0	=	0x00a9
                    00AA    187 _EX1	=	0x00aa
                    00AB    188 _ET1	=	0x00ab
                    00AC    189 _ES	=	0x00ac
                    00AF    190 _EA	=	0x00af
                    00B0    191 _P3_0	=	0x00b0
                    00B1    192 _P3_1	=	0x00b1
                    00B2    193 _P3_2	=	0x00b2
                    00B3    194 _P3_3	=	0x00b3
                    00B4    195 _P3_4	=	0x00b4
                    00B5    196 _P3_5	=	0x00b5
                    00B6    197 _P3_6	=	0x00b6
                    00B7    198 _P3_7	=	0x00b7
                    00B0    199 _RXD	=	0x00b0
                    00B1    200 _TXD	=	0x00b1
                    00B2    201 _INT0	=	0x00b2
                    00B3    202 _INT1	=	0x00b3
                    00B4    203 _T0	=	0x00b4
                    00B5    204 _T1	=	0x00b5
                    00B6    205 _WR	=	0x00b6
                    00B7    206 _RD	=	0x00b7
                    00B8    207 _PX0	=	0x00b8
                    00B9    208 _PT0	=	0x00b9
                    00BA    209 _PX1	=	0x00ba
                    00BB    210 _PT1	=	0x00bb
                    00BC    211 _PS	=	0x00bc
                    00D0    212 _P	=	0x00d0
                    00D1    213 _F1	=	0x00d1
                    00D2    214 _OV	=	0x00d2
                    00D3    215 _RS0	=	0x00d3
                    00D4    216 _RS1	=	0x00d4
                    00D5    217 _F0	=	0x00d5
                    00D6    218 _AC	=	0x00d6
                    00D7    219 _CY	=	0x00d7
                            220 ;--------------------------------------------------------
                            221 ; overlayable register banks
                            222 ;--------------------------------------------------------
                            223 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     224 	.ds 8
                            225 ;--------------------------------------------------------
                            226 ; internal ram data
                            227 ;--------------------------------------------------------
                            228 	.area DSEG    (DATA)
   0008                     229 _main_i_1_1:
   0008                     230 	.ds 2
   000A                     231 _main_a_1_1:
   000A                     232 	.ds 2
   000C                     233 _main_b_1_1:
   000C                     234 	.ds 3
                            235 ;--------------------------------------------------------
                            236 ; overlayable items in internal ram 
                            237 ;--------------------------------------------------------
                            238 	.area	OSEG    (OVR,DATA)
                            239 	.area	OSEG    (OVR,DATA)
                            240 ;--------------------------------------------------------
                            241 ; Stack segment in internal ram 
                            242 ;--------------------------------------------------------
                            243 	.area	SSEG	(DATA)
   0013                     244 __start__stack:
   0013                     245 	.ds	1
                            246 
                            247 ;--------------------------------------------------------
                            248 ; indirectly addressable internal ram data
                            249 ;--------------------------------------------------------
                            250 	.area ISEG    (DATA)
                            251 ;--------------------------------------------------------
                            252 ; absolute internal ram data
                            253 ;--------------------------------------------------------
                            254 	.area IABS    (ABS,DATA)
                            255 	.area IABS    (ABS,DATA)
                            256 ;--------------------------------------------------------
                            257 ; bit data
                            258 ;--------------------------------------------------------
                            259 	.area BSEG    (BIT)
                            260 ;--------------------------------------------------------
                            261 ; paged external ram data
                            262 ;--------------------------------------------------------
                            263 	.area PSEG    (PAG,XDATA)
                            264 ;--------------------------------------------------------
                            265 ; external ram data
                            266 ;--------------------------------------------------------
                            267 	.area XSEG    (XDATA)
                            268 ;--------------------------------------------------------
                            269 ; absolute external ram data
                            270 ;--------------------------------------------------------
                            271 	.area XABS    (ABS,XDATA)
                            272 ;--------------------------------------------------------
                            273 ; external initialized ram data
                            274 ;--------------------------------------------------------
                            275 	.area XISEG   (XDATA)
                            276 	.area HOME    (CODE)
                            277 	.area GSINIT0 (CODE)
                            278 	.area GSINIT1 (CODE)
                            279 	.area GSINIT2 (CODE)
                            280 	.area GSINIT3 (CODE)
                            281 	.area GSINIT4 (CODE)
                            282 	.area GSINIT5 (CODE)
                            283 	.area GSINIT  (CODE)
                            284 	.area GSFINAL (CODE)
                            285 	.area CSEG    (CODE)
                            286 ;--------------------------------------------------------
                            287 ; interrupt vector 
                            288 ;--------------------------------------------------------
                            289 	.area HOME    (CODE)
   0000                     290 __interrupt_vect:
   0000 02 00 0A            291 	ljmp	__sdcc_gsinit_startup
                            292 ;--------------------------------------------------------
                            293 ; global & static initialisations
                            294 ;--------------------------------------------------------
                            295 	.area HOME    (CODE)
                            296 	.area GSINIT  (CODE)
                            297 	.area GSFINAL (CODE)
                            298 	.area GSINIT  (CODE)
                            299 	.globl __sdcc_gsinit_startup
                            300 	.globl __sdcc_program_startup
                            301 	.globl __start__stack
                            302 	.globl __mcs51_genXINIT
                            303 	.globl __mcs51_genXRAMCLEAR
                            304 	.globl __mcs51_genRAMCLEAR
                            305 	.area GSFINAL (CODE)
   0063 02 00 03            306 	ljmp	__sdcc_program_startup
                            307 ;--------------------------------------------------------
                            308 ; Home
                            309 ;--------------------------------------------------------
                            310 	.area HOME    (CODE)
                            311 	.area HOME    (CODE)
   0003                     312 __sdcc_program_startup:
   0003 12 02 48            313 	lcall	_main
                            314 ;	return from main will lock up
   0006 80 FE               315 	sjmp .
                            316 ;--------------------------------------------------------
                            317 ; code
                            318 ;--------------------------------------------------------
                            319 	.area CSEG    (CODE)
                            320 ;------------------------------------------------------------
                            321 ;Allocation info for local variables in function 'delay'
                            322 ;------------------------------------------------------------
                            323 ;i                         Allocated to registers r6 r7 
                            324 ;a                         Allocated to registers r4 r5 
                            325 ;b                         Allocated to registers r2 r3 
                            326 ;------------------------------------------------------------
                            327 ;	asdf.c:5: void delay(int i)
                            328 ;	-----------------------------------------
                            329 ;	 function delay
                            330 ;	-----------------------------------------
   0066                     331 _delay:
                    0007    332 	ar7 = 0x07
                    0006    333 	ar6 = 0x06
                    0005    334 	ar5 = 0x05
                    0004    335 	ar4 = 0x04
                    0003    336 	ar3 = 0x03
                    0002    337 	ar2 = 0x02
                    0001    338 	ar1 = 0x01
                    0000    339 	ar0 = 0x00
   0066 AE 82               340 	mov	r6,dpl
   0068 AF 83               341 	mov	r7,dph
                            342 ;	asdf.c:8: for(a=0;a<i;a++)
   006A 7C 00               343 	mov	r4,#0x00
   006C 7D 00               344 	mov	r5,#0x00
   006E                     345 00104$:
   006E C3                  346 	clr	c
   006F EC                  347 	mov	a,r4
   0070 9E                  348 	subb	a,r6
   0071 ED                  349 	mov	a,r5
   0072 64 80               350 	xrl	a,#0x80
   0074 8F F0               351 	mov	b,r7
   0076 63 F0 80            352 	xrl	b,#0x80
   0079 95 F0               353 	subb	a,b
   007B 50 14               354 	jnc	00108$
                            355 ;	asdf.c:10: for(b=0;b<120;b++);
   007D 7A 78               356 	mov	r2,#0x78
   007F 7B 00               357 	mov	r3,#0x00
   0081                     358 00103$:
   0081 1A                  359 	dec	r2
   0082 BA FF 01            360 	cjne	r2,#0xFF,00117$
   0085 1B                  361 	dec	r3
   0086                     362 00117$:
   0086 EA                  363 	mov	a,r2
   0087 4B                  364 	orl	a,r3
   0088 70 F7               365 	jnz	00103$
                            366 ;	asdf.c:8: for(a=0;a<i;a++)
   008A 0C                  367 	inc	r4
   008B BC 00 E0            368 	cjne	r4,#0x00,00104$
   008E 0D                  369 	inc	r5
   008F 80 DD               370 	sjmp	00104$
   0091                     371 00108$:
   0091 22                  372 	ret
                            373 ;------------------------------------------------------------
                            374 ;Allocation info for local variables in function 'display'
                            375 ;------------------------------------------------------------
                            376 ;i                         Allocated to registers r4 r5 r6 r7 
                            377 ;------------------------------------------------------------
                            378 ;	asdf.c:20: void display(unsigned long int i)
                            379 ;	-----------------------------------------
                            380 ;	 function display
                            381 ;	-----------------------------------------
   0092                     382 _display:
   0092 AC 82               383 	mov	r4,dpl
   0094 AD 83               384 	mov	r5,dph
   0096 AE F0               385 	mov	r6,b
   0098 FF                  386 	mov	r7,a
                            387 ;	asdf.c:30: P0=255;
   0099 75 80 FF            388 	mov	_P0,#0xFF
                            389 ;	asdf.c:31: P2 = seg_position[3];
   009C 90 04 36            390 	mov	dptr,#(_seg_position + 0x0003)
   009F E4                  391 	clr	a
   00A0 93                  392 	movc	a,@a+dptr
   00A1 F5 A0               393 	mov	_P2,a
                            394 ;	asdf.c:32: P0=display_seg[(i%(unsigned long)10000)/(unsigned long)1000];
   00A3 75 0F 10            395 	mov	__modulong_PARM_2,#0x10
   00A6 75 10 27            396 	mov	(__modulong_PARM_2 + 1),#0x27
   00A9 E4                  397 	clr	a
   00AA F5 11               398 	mov	(__modulong_PARM_2 + 2),a
   00AC F5 12               399 	mov	(__modulong_PARM_2 + 3),a
   00AE 8C 82               400 	mov	dpl,r4
   00B0 8D 83               401 	mov	dph,r5
   00B2 8E F0               402 	mov	b,r6
   00B4 EF                  403 	mov	a,r7
   00B5 C0 07               404 	push	ar7
   00B7 C0 06               405 	push	ar6
   00B9 C0 05               406 	push	ar5
   00BB C0 04               407 	push	ar4
   00BD 12 03 07            408 	lcall	__modulong
   00C0 A8 82               409 	mov	r0,dpl
   00C2 A9 83               410 	mov	r1,dph
   00C4 AA F0               411 	mov	r2,b
   00C6 FB                  412 	mov	r3,a
   00C7 75 0F E8            413 	mov	__divulong_PARM_2,#0xE8
   00CA 75 10 03            414 	mov	(__divulong_PARM_2 + 1),#0x03
   00CD E4                  415 	clr	a
   00CE F5 11               416 	mov	(__divulong_PARM_2 + 2),a
   00D0 F5 12               417 	mov	(__divulong_PARM_2 + 3),a
   00D2 88 82               418 	mov	dpl,r0
   00D4 89 83               419 	mov	dph,r1
   00D6 8A F0               420 	mov	b,r2
   00D8 EB                  421 	mov	a,r3
   00D9 12 03 8A            422 	lcall	__divulong
   00DC A8 82               423 	mov	r0,dpl
   00DE A9 83               424 	mov	r1,dph
   00E0 D0 04               425 	pop	ar4
   00E2 D0 05               426 	pop	ar5
   00E4 D0 06               427 	pop	ar6
   00E6 D0 07               428 	pop	ar7
   00E8 E8                  429 	mov	a,r0
   00E9 24 28               430 	add	a,#_display_seg
   00EB F5 82               431 	mov	dpl,a
   00ED E9                  432 	mov	a,r1
   00EE 34 04               433 	addc	a,#(_display_seg >> 8)
   00F0 F5 83               434 	mov	dph,a
   00F2 E4                  435 	clr	a
   00F3 93                  436 	movc	a,@a+dptr
   00F4 F5 80               437 	mov	_P0,a
                            438 ;	asdf.c:33: delay(2);
   00F6 90 00 02            439 	mov	dptr,#0x0002
   00F9 C0 07               440 	push	ar7
   00FB C0 06               441 	push	ar6
   00FD C0 05               442 	push	ar5
   00FF C0 04               443 	push	ar4
   0101 12 00 66            444 	lcall	_delay
   0104 D0 04               445 	pop	ar4
   0106 D0 05               446 	pop	ar5
   0108 D0 06               447 	pop	ar6
   010A D0 07               448 	pop	ar7
                            449 ;	asdf.c:34: P0=255;
   010C 75 80 FF            450 	mov	_P0,#0xFF
                            451 ;	asdf.c:35: P2 = seg_position[2];
   010F 90 04 35            452 	mov	dptr,#(_seg_position + 0x0002)
   0112 E4                  453 	clr	a
   0113 93                  454 	movc	a,@a+dptr
   0114 F5 A0               455 	mov	_P2,a
                            456 ;	asdf.c:36: P0=display_seg[(i%(unsigned long)1000)/(unsigned long)100];
   0116 75 0F E8            457 	mov	__modulong_PARM_2,#0xE8
   0119 75 10 03            458 	mov	(__modulong_PARM_2 + 1),#0x03
   011C E4                  459 	clr	a
   011D F5 11               460 	mov	(__modulong_PARM_2 + 2),a
   011F F5 12               461 	mov	(__modulong_PARM_2 + 3),a
   0121 8C 82               462 	mov	dpl,r4
   0123 8D 83               463 	mov	dph,r5
   0125 8E F0               464 	mov	b,r6
   0127 EF                  465 	mov	a,r7
   0128 C0 07               466 	push	ar7
   012A C0 06               467 	push	ar6
   012C C0 05               468 	push	ar5
   012E C0 04               469 	push	ar4
   0130 12 03 07            470 	lcall	__modulong
   0133 A8 82               471 	mov	r0,dpl
   0135 A9 83               472 	mov	r1,dph
   0137 AA F0               473 	mov	r2,b
   0139 FB                  474 	mov	r3,a
   013A 75 0F 64            475 	mov	__divulong_PARM_2,#0x64
   013D E4                  476 	clr	a
   013E F5 10               477 	mov	(__divulong_PARM_2 + 1),a
   0140 F5 11               478 	mov	(__divulong_PARM_2 + 2),a
   0142 F5 12               479 	mov	(__divulong_PARM_2 + 3),a
   0144 88 82               480 	mov	dpl,r0
   0146 89 83               481 	mov	dph,r1
   0148 8A F0               482 	mov	b,r2
   014A EB                  483 	mov	a,r3
   014B 12 03 8A            484 	lcall	__divulong
   014E A8 82               485 	mov	r0,dpl
   0150 A9 83               486 	mov	r1,dph
   0152 D0 04               487 	pop	ar4
   0154 D0 05               488 	pop	ar5
   0156 D0 06               489 	pop	ar6
   0158 D0 07               490 	pop	ar7
   015A E8                  491 	mov	a,r0
   015B 24 28               492 	add	a,#_display_seg
   015D F5 82               493 	mov	dpl,a
   015F E9                  494 	mov	a,r1
   0160 34 04               495 	addc	a,#(_display_seg >> 8)
   0162 F5 83               496 	mov	dph,a
   0164 E4                  497 	clr	a
   0165 93                  498 	movc	a,@a+dptr
   0166 F5 80               499 	mov	_P0,a
                            500 ;	asdf.c:37: delay(2);
   0168 90 00 02            501 	mov	dptr,#0x0002
   016B C0 07               502 	push	ar7
   016D C0 06               503 	push	ar6
   016F C0 05               504 	push	ar5
   0171 C0 04               505 	push	ar4
   0173 12 00 66            506 	lcall	_delay
   0176 D0 04               507 	pop	ar4
   0178 D0 05               508 	pop	ar5
   017A D0 06               509 	pop	ar6
   017C D0 07               510 	pop	ar7
                            511 ;	asdf.c:38: P0=255;
   017E 75 80 FF            512 	mov	_P0,#0xFF
                            513 ;	asdf.c:39: P2 = seg_position[1];
   0181 90 04 34            514 	mov	dptr,#(_seg_position + 0x0001)
   0184 E4                  515 	clr	a
   0185 93                  516 	movc	a,@a+dptr
   0186 F5 A0               517 	mov	_P2,a
                            518 ;	asdf.c:40: P0=display_seg[(i%(unsigned long)100)/10];
   0188 75 0F 64            519 	mov	__modulong_PARM_2,#0x64
   018B E4                  520 	clr	a
   018C F5 10               521 	mov	(__modulong_PARM_2 + 1),a
   018E F5 11               522 	mov	(__modulong_PARM_2 + 2),a
   0190 F5 12               523 	mov	(__modulong_PARM_2 + 3),a
   0192 8C 82               524 	mov	dpl,r4
   0194 8D 83               525 	mov	dph,r5
   0196 8E F0               526 	mov	b,r6
   0198 EF                  527 	mov	a,r7
   0199 C0 07               528 	push	ar7
   019B C0 06               529 	push	ar6
   019D C0 05               530 	push	ar5
   019F C0 04               531 	push	ar4
   01A1 12 03 07            532 	lcall	__modulong
   01A4 A8 82               533 	mov	r0,dpl
   01A6 A9 83               534 	mov	r1,dph
   01A8 AA F0               535 	mov	r2,b
   01AA FB                  536 	mov	r3,a
   01AB 75 0F 0A            537 	mov	__divulong_PARM_2,#0x0A
   01AE E4                  538 	clr	a
   01AF F5 10               539 	mov	(__divulong_PARM_2 + 1),a
   01B1 F5 11               540 	mov	(__divulong_PARM_2 + 2),a
   01B3 F5 12               541 	mov	(__divulong_PARM_2 + 3),a
   01B5 88 82               542 	mov	dpl,r0
   01B7 89 83               543 	mov	dph,r1
   01B9 8A F0               544 	mov	b,r2
   01BB EB                  545 	mov	a,r3
   01BC 12 03 8A            546 	lcall	__divulong
   01BF A8 82               547 	mov	r0,dpl
   01C1 A9 83               548 	mov	r1,dph
   01C3 D0 04               549 	pop	ar4
   01C5 D0 05               550 	pop	ar5
   01C7 D0 06               551 	pop	ar6
   01C9 D0 07               552 	pop	ar7
   01CB E8                  553 	mov	a,r0
   01CC 24 28               554 	add	a,#_display_seg
   01CE F5 82               555 	mov	dpl,a
   01D0 E9                  556 	mov	a,r1
   01D1 34 04               557 	addc	a,#(_display_seg >> 8)
   01D3 F5 83               558 	mov	dph,a
   01D5 E4                  559 	clr	a
   01D6 93                  560 	movc	a,@a+dptr
   01D7 F5 80               561 	mov	_P0,a
                            562 ;	asdf.c:41: delay(2);
   01D9 90 00 02            563 	mov	dptr,#0x0002
   01DC C0 07               564 	push	ar7
   01DE C0 06               565 	push	ar6
   01E0 C0 05               566 	push	ar5
   01E2 C0 04               567 	push	ar4
   01E4 12 00 66            568 	lcall	_delay
   01E7 D0 04               569 	pop	ar4
   01E9 D0 05               570 	pop	ar5
   01EB D0 06               571 	pop	ar6
   01ED D0 07               572 	pop	ar7
                            573 ;	asdf.c:42: P0=255;
   01EF 75 80 FF            574 	mov	_P0,#0xFF
                            575 ;	asdf.c:43: P2 = seg_position[0];
   01F2 90 04 33            576 	mov	dptr,#_seg_position
   01F5 E4                  577 	clr	a
   01F6 93                  578 	movc	a,@a+dptr
   01F7 F5 A0               579 	mov	_P2,a
                            580 ;	asdf.c:44: P0=display_seg[i%10];
   01F9 75 0F 0A            581 	mov	__modulong_PARM_2,#0x0A
   01FC E4                  582 	clr	a
   01FD F5 10               583 	mov	(__modulong_PARM_2 + 1),a
   01FF F5 11               584 	mov	(__modulong_PARM_2 + 2),a
   0201 F5 12               585 	mov	(__modulong_PARM_2 + 3),a
   0203 8C 82               586 	mov	dpl,r4
   0205 8D 83               587 	mov	dph,r5
   0207 8E F0               588 	mov	b,r6
   0209 EF                  589 	mov	a,r7
   020A 12 03 07            590 	lcall	__modulong
   020D AC 82               591 	mov	r4,dpl
   020F AD 83               592 	mov	r5,dph
   0211 EC                  593 	mov	a,r4
   0212 24 28               594 	add	a,#_display_seg
   0214 F5 82               595 	mov	dpl,a
   0216 ED                  596 	mov	a,r5
   0217 34 04               597 	addc	a,#(_display_seg >> 8)
   0219 F5 83               598 	mov	dph,a
   021B E4                  599 	clr	a
   021C 93                  600 	movc	a,@a+dptr
   021D F5 80               601 	mov	_P0,a
                            602 ;	asdf.c:45: delay(2);
   021F 90 00 02            603 	mov	dptr,#0x0002
   0222 02 00 66            604 	ljmp	_delay
                            605 ;------------------------------------------------------------
                            606 ;Allocation info for local variables in function 'asd'
                            607 ;------------------------------------------------------------
                            608 ;i                         Allocated to registers r6 r7 
                            609 ;------------------------------------------------------------
                            610 ;	asdf.c:50: void asd() {
                            611 ;	-----------------------------------------
                            612 ;	 function asd
                            613 ;	-----------------------------------------
   0225                     614 _asd:
                            615 ;	asdf.c:52: while(1) {
   0225                     616 00102$:
                            617 ;	asdf.c:53: P0=~P0;
   0225 E5 80               618 	mov	a,_P0
   0227 F4                  619 	cpl	a
   0228 F5 80               620 	mov	_P0,a
                            621 ;	asdf.c:54: P1=~P1;
   022A E5 90               622 	mov	a,_P1
   022C F4                  623 	cpl	a
   022D F5 90               624 	mov	_P1,a
                            625 ;	asdf.c:55: P2=~P2;
   022F E5 A0               626 	mov	a,_P2
   0231 F4                  627 	cpl	a
   0232 F5 A0               628 	mov	_P2,a
                            629 ;	asdf.c:56: P3=~P3;
   0234 E5 B0               630 	mov	a,_P3
   0236 F4                  631 	cpl	a
   0237 F5 B0               632 	mov	_P3,a
                            633 ;	asdf.c:57: for(i=0;i<10;i++) {}
   0239 7E 0A               634 	mov	r6,#0x0A
   023B 7F 00               635 	mov	r7,#0x00
   023D                     636 00106$:
   023D 1E                  637 	dec	r6
   023E BE FF 01            638 	cjne	r6,#0xFF,00113$
   0241 1F                  639 	dec	r7
   0242                     640 00113$:
   0242 EE                  641 	mov	a,r6
   0243 4F                  642 	orl	a,r7
   0244 70 F7               643 	jnz	00106$
   0246 80 DD               644 	sjmp	00102$
                            645 ;------------------------------------------------------------
                            646 ;Allocation info for local variables in function 'main'
                            647 ;------------------------------------------------------------
                            648 ;i                         Allocated with name '_main_i_1_1'
                            649 ;a                         Allocated with name '_main_a_1_1'
                            650 ;b                         Allocated with name '_main_b_1_1'
                            651 ;------------------------------------------------------------
                            652 ;	asdf.c:60: unsigned int main(void)
                            653 ;	-----------------------------------------
                            654 ;	 function main
                            655 ;	-----------------------------------------
   0248                     656 _main:
                            657 ;	asdf.c:65: P0=0xff;
   0248 75 80 FF            658 	mov	_P0,#0xFF
                            659 ;	asdf.c:66: P1=0xff;
   024B 75 90 FF            660 	mov	_P1,#0xFF
                            661 ;	asdf.c:67: P2=0xff;
   024E 75 A0 FF            662 	mov	_P2,#0xFF
                            663 ;	asdf.c:68: P3=0xff;
   0251 75 B0 FF            664 	mov	_P3,#0xFF
                            665 ;	asdf.c:69: a=&asd;
   0254 75 0A 25            666 	mov	_main_a_1_1,#_asd
   0257 75 0B 02            667 	mov	(_main_a_1_1 + 1),#(_asd >> 8)
                            668 ;	asdf.c:70: b=(int*)a;
   025A AB 0A               669 	mov	r3,_main_a_1_1
   025C AC 0B               670 	mov	r4,(_main_a_1_1 + 1)
   025E 7D 80               671 	mov	r5,#0x80
                            672 ;	asdf.c:71: *b=5432;
   0260 8B 82               673 	mov	dpl,r3
   0262 8C 83               674 	mov	dph,r4
   0264 8D F0               675 	mov	b,r5
   0266 74 38               676 	mov	a,#0x38
   0268 12 03 EF            677 	lcall	__gptrput
   026B A3                  678 	inc	dptr
   026C 74 15               679 	mov	a,#0x15
   026E 12 03 EF            680 	lcall	__gptrput
                            681 ;	asdf.c:72: for(i=0;i<100;i++)
   0271 75 08 64            682 	mov	_main_i_1_1,#0x64
   0274 75 09 00            683 	mov	(_main_i_1_1 + 1),#0x00
   0277                     684 00106$:
                            685 ;	asdf.c:73: display(*b);
   0277 8B 82               686 	mov	dpl,r3
   0279 8C 83               687 	mov	dph,r4
   027B 8D F0               688 	mov	b,r5
   027D 12 04 08            689 	lcall	__gptrget
   0280 F8                  690 	mov	r0,a
   0281 A3                  691 	inc	dptr
   0282 12 04 08            692 	lcall	__gptrget
   0285 FA                  693 	mov	r2,a
   0286 33                  694 	rlc	a
   0287 95 E0               695 	subb	a,acc
   0289 FE                  696 	mov	r6,a
   028A 88 82               697 	mov	dpl,r0
   028C 8A 83               698 	mov	dph,r2
   028E 8E F0               699 	mov	b,r6
   0290 C0 05               700 	push	ar5
   0292 C0 04               701 	push	ar4
   0294 C0 03               702 	push	ar3
   0296 12 00 92            703 	lcall	_display
   0299 D0 03               704 	pop	ar3
   029B D0 04               705 	pop	ar4
   029D D0 05               706 	pop	ar5
   029F 15 08               707 	dec	_main_i_1_1
   02A1 74 FF               708 	mov	a,#0xFF
   02A3 B5 08 02            709 	cjne	a,_main_i_1_1,00119$
   02A6 15 09               710 	dec	(_main_i_1_1 + 1)
   02A8                     711 00119$:
                            712 ;	asdf.c:72: for(i=0;i<100;i++)
   02A8 E5 08               713 	mov	a,_main_i_1_1
   02AA 45 09               714 	orl	a,(_main_i_1_1 + 1)
   02AC 70 C9               715 	jnz	00106$
                            716 ;	asdf.c:74: *b=1234;
   02AE 8B 82               717 	mov	dpl,r3
   02B0 8C 83               718 	mov	dph,r4
   02B2 8D F0               719 	mov	b,r5
   02B4 74 D2               720 	mov	a,#0xD2
   02B6 12 03 EF            721 	lcall	__gptrput
   02B9 A3                  722 	inc	dptr
   02BA 74 04               723 	mov	a,#0x04
   02BC 12 03 EF            724 	lcall	__gptrput
                            725 ;	asdf.c:75: for(i=0;i<100;i++)
   02BF 75 08 64            726 	mov	_main_i_1_1,#0x64
   02C2 75 09 00            727 	mov	(_main_i_1_1 + 1),#0x00
   02C5                     728 00109$:
                            729 ;	asdf.c:76: display(*b);
   02C5 8B 82               730 	mov	dpl,r3
   02C7 8C 83               731 	mov	dph,r4
   02C9 8D F0               732 	mov	b,r5
   02CB 12 04 08            733 	lcall	__gptrget
   02CE F9                  734 	mov	r1,a
   02CF A3                  735 	inc	dptr
   02D0 12 04 08            736 	lcall	__gptrget
   02D3 F8                  737 	mov	r0,a
   02D4 33                  738 	rlc	a
   02D5 95 E0               739 	subb	a,acc
   02D7 FA                  740 	mov	r2,a
   02D8 89 82               741 	mov	dpl,r1
   02DA 88 83               742 	mov	dph,r0
   02DC 8A F0               743 	mov	b,r2
   02DE C0 05               744 	push	ar5
   02E0 C0 04               745 	push	ar4
   02E2 C0 03               746 	push	ar3
   02E4 12 00 92            747 	lcall	_display
   02E7 D0 03               748 	pop	ar3
   02E9 D0 04               749 	pop	ar4
   02EB D0 05               750 	pop	ar5
   02ED 15 08               751 	dec	_main_i_1_1
   02EF 74 FF               752 	mov	a,#0xFF
   02F1 B5 08 02            753 	cjne	a,_main_i_1_1,00121$
   02F4 15 09               754 	dec	(_main_i_1_1 + 1)
   02F6                     755 00121$:
                            756 ;	asdf.c:75: for(i=0;i<100;i++)
   02F6 E5 08               757 	mov	a,_main_i_1_1
   02F8 45 09               758 	orl	a,(_main_i_1_1 + 1)
   02FA 70 C9               759 	jnz	00109$
                            760 ;	asdf.c:77: a();
   02FC 85 0A 82            761 	mov	dpl,_main_a_1_1
   02FF 85 0B 83            762 	mov	dph,(_main_a_1_1 + 1)
   0302 12 00 08            763 	lcall	__sdcc_call_dptr
                            764 ;	asdf.c:78: while(1)
   0305                     765 00102$:
   0305 80 FE               766 	sjmp	00102$
                            767 	.area CSEG    (CODE)
                            768 	.area CONST   (CODE)
   0428                     769 _display_seg:
   0428 C0                  770 	.db #0xC0	; 192
   0429 F9                  771 	.db #0xF9	; 249
   042A A4                  772 	.db #0xA4	; 164
   042B B0                  773 	.db #0xB0	; 176
   042C 99                  774 	.db #0x99	; 153
   042D 92                  775 	.db #0x92	; 146
   042E 82                  776 	.db #0x82	; 130
   042F F8                  777 	.db #0xF8	; 248
   0430 80                  778 	.db #0x80	; 128
   0431 90                  779 	.db #0x90	; 144
   0432 7F                  780 	.db #0x7F	; 127
   0433                     781 _seg_position:
   0433 FE                  782 	.db #0xFE	; 254
   0434 FD                  783 	.db #0xFD	; 253
   0435 FB                  784 	.db #0xFB	; 251
   0436 F7                  785 	.db #0xF7	; 247
   0437 F0                  786 	.db #0xF0	; 240
                            787 	.area XINIT   (CODE)
                            788 	.area CABS    (ABS,CODE)
