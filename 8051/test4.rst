                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
                              4 ; This file was generated Sun May 13 19:13:08 2012
                              5 ;--------------------------------------------------------
                              6 	.module test4
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _a
                             13 	.globl _i1
                             14 	.globl _main
                             15 	.globl _CY
                             16 	.globl _AC
                             17 	.globl _F0
                             18 	.globl _RS1
                             19 	.globl _RS0
                             20 	.globl _OV
                             21 	.globl _F1
                             22 	.globl _P
                             23 	.globl _PS
                             24 	.globl _PT1
                             25 	.globl _PX1
                             26 	.globl _PT0
                             27 	.globl _PX0
                             28 	.globl _RD
                             29 	.globl _WR
                             30 	.globl _T1
                             31 	.globl _T0
                             32 	.globl _INT1
                             33 	.globl _INT0
                             34 	.globl _TXD
                             35 	.globl _RXD
                             36 	.globl _P3_7
                             37 	.globl _P3_6
                             38 	.globl _P3_5
                             39 	.globl _P3_4
                             40 	.globl _P3_3
                             41 	.globl _P3_2
                             42 	.globl _P3_1
                             43 	.globl _P3_0
                             44 	.globl _EA
                             45 	.globl _ES
                             46 	.globl _ET1
                             47 	.globl _EX1
                             48 	.globl _ET0
                             49 	.globl _EX0
                             50 	.globl _P2_7
                             51 	.globl _P2_6
                             52 	.globl _P2_5
                             53 	.globl _P2_4
                             54 	.globl _P2_3
                             55 	.globl _P2_2
                             56 	.globl _P2_1
                             57 	.globl _P2_0
                             58 	.globl _SM0
                             59 	.globl _SM1
                             60 	.globl _SM2
                             61 	.globl _REN
                             62 	.globl _TB8
                             63 	.globl _RB8
                             64 	.globl _TI
                             65 	.globl _RI
                             66 	.globl _P1_7
                             67 	.globl _P1_6
                             68 	.globl _P1_5
                             69 	.globl _P1_4
                             70 	.globl _P1_3
                             71 	.globl _P1_2
                             72 	.globl _P1_1
                             73 	.globl _P1_0
                             74 	.globl _TF1
                             75 	.globl _TR1
                             76 	.globl _TF0
                             77 	.globl _TR0
                             78 	.globl _IE1
                             79 	.globl _IT1
                             80 	.globl _IE0
                             81 	.globl _IT0
                             82 	.globl _P0_7
                             83 	.globl _P0_6
                             84 	.globl _P0_5
                             85 	.globl _P0_4
                             86 	.globl _P0_3
                             87 	.globl _P0_2
                             88 	.globl _P0_1
                             89 	.globl _P0_0
                             90 	.globl _B
                             91 	.globl _ACC
                             92 	.globl _PSW
                             93 	.globl _IP
                             94 	.globl _P3
                             95 	.globl _IE
                             96 	.globl _P2
                             97 	.globl _SBUF
                             98 	.globl _SCON
                             99 	.globl _P1
                            100 	.globl _TH1
                            101 	.globl _TH0
                            102 	.globl _TL1
                            103 	.globl _TL0
                            104 	.globl _TMOD
                            105 	.globl _TCON
                            106 	.globl _PCON
                            107 	.globl _DPH
                            108 	.globl _DPL
                            109 	.globl _SP
                            110 	.globl _P0
                            111 	.globl _i
                            112 ;--------------------------------------------------------
                            113 ; special function registers
                            114 ;--------------------------------------------------------
                            115 	.area RSEG    (DATA)
                    0080    116 _P0	=	0x0080
                    0081    117 _SP	=	0x0081
                    0082    118 _DPL	=	0x0082
                    0083    119 _DPH	=	0x0083
                    0087    120 _PCON	=	0x0087
                    0088    121 _TCON	=	0x0088
                    0089    122 _TMOD	=	0x0089
                    008A    123 _TL0	=	0x008a
                    008B    124 _TL1	=	0x008b
                    008C    125 _TH0	=	0x008c
                    008D    126 _TH1	=	0x008d
                    0090    127 _P1	=	0x0090
                    0098    128 _SCON	=	0x0098
                    0099    129 _SBUF	=	0x0099
                    00A0    130 _P2	=	0x00a0
                    00A8    131 _IE	=	0x00a8
                    00B0    132 _P3	=	0x00b0
                    00B8    133 _IP	=	0x00b8
                    00D0    134 _PSW	=	0x00d0
                    00E0    135 _ACC	=	0x00e0
                    00F0    136 _B	=	0x00f0
                            137 ;--------------------------------------------------------
                            138 ; special function bits
                            139 ;--------------------------------------------------------
                            140 	.area RSEG    (DATA)
                    0080    141 _P0_0	=	0x0080
                    0081    142 _P0_1	=	0x0081
                    0082    143 _P0_2	=	0x0082
                    0083    144 _P0_3	=	0x0083
                    0084    145 _P0_4	=	0x0084
                    0085    146 _P0_5	=	0x0085
                    0086    147 _P0_6	=	0x0086
                    0087    148 _P0_7	=	0x0087
                    0088    149 _IT0	=	0x0088
                    0089    150 _IE0	=	0x0089
                    008A    151 _IT1	=	0x008a
                    008B    152 _IE1	=	0x008b
                    008C    153 _TR0	=	0x008c
                    008D    154 _TF0	=	0x008d
                    008E    155 _TR1	=	0x008e
                    008F    156 _TF1	=	0x008f
                    0090    157 _P1_0	=	0x0090
                    0091    158 _P1_1	=	0x0091
                    0092    159 _P1_2	=	0x0092
                    0093    160 _P1_3	=	0x0093
                    0094    161 _P1_4	=	0x0094
                    0095    162 _P1_5	=	0x0095
                    0096    163 _P1_6	=	0x0096
                    0097    164 _P1_7	=	0x0097
                    0098    165 _RI	=	0x0098
                    0099    166 _TI	=	0x0099
                    009A    167 _RB8	=	0x009a
                    009B    168 _TB8	=	0x009b
                    009C    169 _REN	=	0x009c
                    009D    170 _SM2	=	0x009d
                    009E    171 _SM1	=	0x009e
                    009F    172 _SM0	=	0x009f
                    00A0    173 _P2_0	=	0x00a0
                    00A1    174 _P2_1	=	0x00a1
                    00A2    175 _P2_2	=	0x00a2
                    00A3    176 _P2_3	=	0x00a3
                    00A4    177 _P2_4	=	0x00a4
                    00A5    178 _P2_5	=	0x00a5
                    00A6    179 _P2_6	=	0x00a6
                    00A7    180 _P2_7	=	0x00a7
                    00A8    181 _EX0	=	0x00a8
                    00A9    182 _ET0	=	0x00a9
                    00AA    183 _EX1	=	0x00aa
                    00AB    184 _ET1	=	0x00ab
                    00AC    185 _ES	=	0x00ac
                    00AF    186 _EA	=	0x00af
                    00B0    187 _P3_0	=	0x00b0
                    00B1    188 _P3_1	=	0x00b1
                    00B2    189 _P3_2	=	0x00b2
                    00B3    190 _P3_3	=	0x00b3
                    00B4    191 _P3_4	=	0x00b4
                    00B5    192 _P3_5	=	0x00b5
                    00B6    193 _P3_6	=	0x00b6
                    00B7    194 _P3_7	=	0x00b7
                    00B0    195 _RXD	=	0x00b0
                    00B1    196 _TXD	=	0x00b1
                    00B2    197 _INT0	=	0x00b2
                    00B3    198 _INT1	=	0x00b3
                    00B4    199 _T0	=	0x00b4
                    00B5    200 _T1	=	0x00b5
                    00B6    201 _WR	=	0x00b6
                    00B7    202 _RD	=	0x00b7
                    00B8    203 _PX0	=	0x00b8
                    00B9    204 _PT0	=	0x00b9
                    00BA    205 _PX1	=	0x00ba
                    00BB    206 _PT1	=	0x00bb
                    00BC    207 _PS	=	0x00bc
                    00D0    208 _P	=	0x00d0
                    00D1    209 _F1	=	0x00d1
                    00D2    210 _OV	=	0x00d2
                    00D3    211 _RS0	=	0x00d3
                    00D4    212 _RS1	=	0x00d4
                    00D5    213 _F0	=	0x00d5
                    00D6    214 _AC	=	0x00d6
                    00D7    215 _CY	=	0x00d7
                            216 ;--------------------------------------------------------
                            217 ; overlayable register banks
                            218 ;--------------------------------------------------------
                            219 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     220 	.ds 8
                            221 ;--------------------------------------------------------
                            222 ; internal ram data
                            223 ;--------------------------------------------------------
                            224 	.area DSEG    (DATA)
   0008                     225 _i::
   0008                     226 	.ds 1
                            227 ;--------------------------------------------------------
                            228 ; overlayable items in internal ram 
                            229 ;--------------------------------------------------------
                            230 	.area	OSEG    (OVR,DATA)
                            231 ;--------------------------------------------------------
                            232 ; Stack segment in internal ram 
                            233 ;--------------------------------------------------------
                            234 	.area	SSEG	(DATA)
   0009                     235 __start__stack:
   0009                     236 	.ds	1
                            237 
                            238 ;--------------------------------------------------------
                            239 ; indirectly addressable internal ram data
                            240 ;--------------------------------------------------------
                            241 	.area ISEG    (DATA)
                            242 ;--------------------------------------------------------
                            243 ; absolute internal ram data
                            244 ;--------------------------------------------------------
                            245 	.area IABS    (ABS,DATA)
                            246 	.area IABS    (ABS,DATA)
                            247 ;--------------------------------------------------------
                            248 ; bit data
                            249 ;--------------------------------------------------------
                            250 	.area BSEG    (BIT)
                            251 ;--------------------------------------------------------
                            252 ; paged external ram data
                            253 ;--------------------------------------------------------
                            254 	.area PSEG    (PAG,XDATA)
                            255 ;--------------------------------------------------------
                            256 ; external ram data
                            257 ;--------------------------------------------------------
                            258 	.area XSEG    (XDATA)
                            259 ;--------------------------------------------------------
                            260 ; absolute external ram data
                            261 ;--------------------------------------------------------
                            262 	.area XABS    (ABS,XDATA)
                            263 ;--------------------------------------------------------
                            264 ; external initialized ram data
                            265 ;--------------------------------------------------------
                            266 	.area XISEG   (XDATA)
                            267 	.area HOME    (CODE)
                            268 	.area GSINIT0 (CODE)
                            269 	.area GSINIT1 (CODE)
                            270 	.area GSINIT2 (CODE)
                            271 	.area GSINIT3 (CODE)
                            272 	.area GSINIT4 (CODE)
                            273 	.area GSINIT5 (CODE)
                            274 	.area GSINIT  (CODE)
                            275 	.area GSFINAL (CODE)
                            276 	.area CSEG    (CODE)
                            277 ;--------------------------------------------------------
                            278 ; interrupt vector 
                            279 ;--------------------------------------------------------
                            280 	.area HOME    (CODE)
   0000                     281 __interrupt_vect:
   0000 02 00 13            282 	ljmp	__sdcc_gsinit_startup
   0003 32                  283 	reti
   0004                     284 	.ds	7
   000B 02 00 B4            285 	ljmp	_i1
                            286 ;--------------------------------------------------------
                            287 ; global & static initialisations
                            288 ;--------------------------------------------------------
                            289 	.area HOME    (CODE)
                            290 	.area GSINIT  (CODE)
                            291 	.area GSFINAL (CODE)
                            292 	.area GSINIT  (CODE)
                            293 	.globl __sdcc_gsinit_startup
                            294 	.globl __sdcc_program_startup
                            295 	.globl __start__stack
                            296 	.globl __mcs51_genXINIT
                            297 	.globl __mcs51_genXRAMCLEAR
                            298 	.globl __mcs51_genRAMCLEAR
                            299 ;	test4.c:3: unsigned char i=0;
   006C 75 08 00            300 	mov	_i,#0x00
                            301 	.area GSFINAL (CODE)
   006F 02 00 0E            302 	ljmp	__sdcc_program_startup
                            303 ;--------------------------------------------------------
                            304 ; Home
                            305 ;--------------------------------------------------------
                            306 	.area HOME    (CODE)
                            307 	.area HOME    (CODE)
   000E                     308 __sdcc_program_startup:
   000E 12 00 72            309 	lcall	_main
                            310 ;	return from main will lock up
   0011 80 FE               311 	sjmp .
                            312 ;--------------------------------------------------------
                            313 ; code
                            314 ;--------------------------------------------------------
                            315 	.area CSEG    (CODE)
                            316 ;------------------------------------------------------------
                            317 ;Allocation info for local variables in function 'main'
                            318 ;------------------------------------------------------------
                            319 ;x                         Allocated to registers r4 r5 
                            320 ;b                         Allocated to registers r2 r3 
                            321 ;------------------------------------------------------------
                            322 ;	test4.c:9: main()
                            323 ;	-----------------------------------------
                            324 ;	 function main
                            325 ;	-----------------------------------------
   0072                     326 _main:
                    0002    327 	ar2 = 0x02
                    0003    328 	ar3 = 0x03
                    0004    329 	ar4 = 0x04
                    0005    330 	ar5 = 0x05
                    0006    331 	ar6 = 0x06
                    0007    332 	ar7 = 0x07
                    0000    333 	ar0 = 0x00
                    0001    334 	ar1 = 0x01
                            335 ;	test4.c:12: int b=0;
   0072 7A 00               336 	mov	r2,#0x00
   0074 7B 00               337 	mov	r3,#0x00
                            338 ;	test4.c:13: IE=0x8a;
   0076 75 A8 8A            339 	mov	_IE,#0x8A
                            340 ;	test4.c:14: TMOD=0x11;
   0079 75 89 11            341 	mov	_TMOD,#0x11
                            342 ;	test4.c:15: TH0=(65536-50000)/256;
   007C 75 8C 3C            343 	mov	_TH0,#0x3C
                            344 ;	test4.c:16: TL0=(65536-50000)%256;
   007F 75 8A B0            345 	mov	_TL0,#0xB0
                            346 ;	test4.c:17: TR0=1;
   0082 D2 8C               347 	setb	_TR0
                            348 ;	test4.c:18: while(1)
   0084                     349 00102$:
                            350 ;	test4.c:21: b=b?0:1;
   0084 EA                  351 	mov	a,r2
   0085 4B                  352 	orl	a,r3
   0086 60 04               353 	jz	00109$
   0088 7C 00               354 	mov	r4,#0x00
   008A 80 02               355 	sjmp	00110$
   008C                     356 00109$:
   008C 7C 01               357 	mov	r4,#0x01
   008E                     358 00110$:
   008E 8C 02               359 	mov	ar2,r4
   0090 7B 00               360 	mov	r3,#0x00
                            361 ;	test4.c:27: set_bit(P2,4,b);
   0092 EA                  362 	mov	a,r2
   0093 4B                  363 	orl	a,r3
   0094 60 07               364 	jz	00111$
   0096 74 10               365 	mov	a,#0x10
   0098 45 A0               366 	orl	a,_P2
   009A FC                  367 	mov	r4,a
   009B 80 06               368 	sjmp	00112$
   009D                     369 00111$:
   009D AD A0               370 	mov	r5,_P2
   009F 74 EF               371 	mov	a,#0xEF
   00A1 5D                  372 	anl	a,r5
   00A2 FC                  373 	mov	r4,a
   00A3                     374 00112$:
   00A3 8C A0               375 	mov	_P2,r4
                            376 ;	test4.c:28: for(x=0;x<100;x++);
   00A5 7C 64               377 	mov	r4,#0x64
   00A7 7D 00               378 	mov	r5,#0x00
   00A9                     379 00106$:
   00A9 1C                  380 	dec	r4
   00AA BC FF 01            381 	cjne	r4,#0xff,00121$
   00AD 1D                  382 	dec	r5
   00AE                     383 00121$:
   00AE EC                  384 	mov	a,r4
   00AF 4D                  385 	orl	a,r5
   00B0 70 F7               386 	jnz	00106$
   00B2 80 D0               387 	sjmp	00102$
                            388 ;------------------------------------------------------------
                            389 ;Allocation info for local variables in function 'i1'
                            390 ;------------------------------------------------------------
                            391 ;------------------------------------------------------------
                            392 ;	test4.c:31: void i1(void) interrupt 1
                            393 ;	-----------------------------------------
                            394 ;	 function i1
                            395 ;	-----------------------------------------
   00B4                     396 _i1:
   00B4 C0 E0               397 	push	acc
   00B6 C0 82               398 	push	dpl
   00B8 C0 83               399 	push	dph
   00BA C0 D0               400 	push	psw
   00BC 75 D0 00            401 	mov	psw,#0x00
                            402 ;	test4.c:33: TH0=(65536-50000)/256;
   00BF 75 8C 3C            403 	mov	_TH0,#0x3C
                            404 ;	test4.c:34: TL0=(65536-50000)%256;
   00C2 75 8A B0            405 	mov	_TL0,#0xB0
                            406 ;	test4.c:35: P1=a[i];
   00C5 E5 08               407 	mov	a,_i
   00C7 90 00 E5            408 	mov	dptr,#_a
   00CA 93                  409 	movc	a,@a+dptr
   00CB F5 90               410 	mov	_P1,a
                            411 ;	test4.c:38: i++;
   00CD 05 08               412 	inc	_i
                            413 ;	test4.c:39: if(i>7)i=0;
   00CF E5 08               414 	mov	a,_i
   00D1 24 F8               415 	add	a,#0xff - 0x07
   00D3 50 03               416 	jnc	00103$
   00D5 75 08 00            417 	mov	_i,#0x00
   00D8                     418 00103$:
   00D8 D0 D0               419 	pop	psw
   00DA D0 83               420 	pop	dph
   00DC D0 82               421 	pop	dpl
   00DE D0 E0               422 	pop	acc
   00E0 32                  423 	reti
                            424 ;	eliminated unneeded push/pop b
                            425 	.area CSEG    (CODE)
                            426 	.area CONST   (CODE)
   00E5                     427 _a:
   00E5 09                  428 	.db #0x09
   00E6 08                  429 	.db #0x08
   00E7 0C                  430 	.db #0x0C
   00E8 04                  431 	.db #0x04
   00E9 06                  432 	.db #0x06
   00EA 02                  433 	.db #0x02
   00EB 03                  434 	.db #0x03
   00EC 01                  435 	.db #0x01
                            436 	.area XINIT   (CODE)
                            437 	.area CABS    (ABS,CODE)
