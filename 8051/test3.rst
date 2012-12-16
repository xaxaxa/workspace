                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 3.1.0 #7066 (Feb 26 2012) (Linux)
                              4 ; This file was generated Sat Dec  1 15:18:20 2012
                              5 ;--------------------------------------------------------
                              6 	.module test3
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _seg_position
                             13 	.globl _display_seg
                             14 	.globl _timer
                             15 	.globl _main
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
                            114 	.globl _i
                            115 ;--------------------------------------------------------
                            116 ; special function registers
                            117 ;--------------------------------------------------------
                            118 	.area RSEG    (ABS,DATA)
   0000                     119 	.org 0x0000
                    0080    120 _P0	=	0x0080
                    0081    121 _SP	=	0x0081
                    0082    122 _DPL	=	0x0082
                    0083    123 _DPH	=	0x0083
                    0087    124 _PCON	=	0x0087
                    0088    125 _TCON	=	0x0088
                    0089    126 _TMOD	=	0x0089
                    008A    127 _TL0	=	0x008a
                    008B    128 _TL1	=	0x008b
                    008C    129 _TH0	=	0x008c
                    008D    130 _TH1	=	0x008d
                    0090    131 _P1	=	0x0090
                    0098    132 _SCON	=	0x0098
                    0099    133 _SBUF	=	0x0099
                    00A0    134 _P2	=	0x00a0
                    00A8    135 _IE	=	0x00a8
                    00B0    136 _P3	=	0x00b0
                    00B8    137 _IP	=	0x00b8
                    00D0    138 _PSW	=	0x00d0
                    00E0    139 _ACC	=	0x00e0
                    00F0    140 _B	=	0x00f0
                            141 ;--------------------------------------------------------
                            142 ; special function bits
                            143 ;--------------------------------------------------------
                            144 	.area RSEG    (ABS,DATA)
   0000                     145 	.org 0x0000
                    0080    146 _P0_0	=	0x0080
                    0081    147 _P0_1	=	0x0081
                    0082    148 _P0_2	=	0x0082
                    0083    149 _P0_3	=	0x0083
                    0084    150 _P0_4	=	0x0084
                    0085    151 _P0_5	=	0x0085
                    0086    152 _P0_6	=	0x0086
                    0087    153 _P0_7	=	0x0087
                    0088    154 _IT0	=	0x0088
                    0089    155 _IE0	=	0x0089
                    008A    156 _IT1	=	0x008a
                    008B    157 _IE1	=	0x008b
                    008C    158 _TR0	=	0x008c
                    008D    159 _TF0	=	0x008d
                    008E    160 _TR1	=	0x008e
                    008F    161 _TF1	=	0x008f
                    0090    162 _P1_0	=	0x0090
                    0091    163 _P1_1	=	0x0091
                    0092    164 _P1_2	=	0x0092
                    0093    165 _P1_3	=	0x0093
                    0094    166 _P1_4	=	0x0094
                    0095    167 _P1_5	=	0x0095
                    0096    168 _P1_6	=	0x0096
                    0097    169 _P1_7	=	0x0097
                    0098    170 _RI	=	0x0098
                    0099    171 _TI	=	0x0099
                    009A    172 _RB8	=	0x009a
                    009B    173 _TB8	=	0x009b
                    009C    174 _REN	=	0x009c
                    009D    175 _SM2	=	0x009d
                    009E    176 _SM1	=	0x009e
                    009F    177 _SM0	=	0x009f
                    00A0    178 _P2_0	=	0x00a0
                    00A1    179 _P2_1	=	0x00a1
                    00A2    180 _P2_2	=	0x00a2
                    00A3    181 _P2_3	=	0x00a3
                    00A4    182 _P2_4	=	0x00a4
                    00A5    183 _P2_5	=	0x00a5
                    00A6    184 _P2_6	=	0x00a6
                    00A7    185 _P2_7	=	0x00a7
                    00A8    186 _EX0	=	0x00a8
                    00A9    187 _ET0	=	0x00a9
                    00AA    188 _EX1	=	0x00aa
                    00AB    189 _ET1	=	0x00ab
                    00AC    190 _ES	=	0x00ac
                    00AF    191 _EA	=	0x00af
                    00B0    192 _P3_0	=	0x00b0
                    00B1    193 _P3_1	=	0x00b1
                    00B2    194 _P3_2	=	0x00b2
                    00B3    195 _P3_3	=	0x00b3
                    00B4    196 _P3_4	=	0x00b4
                    00B5    197 _P3_5	=	0x00b5
                    00B6    198 _P3_6	=	0x00b6
                    00B7    199 _P3_7	=	0x00b7
                    00B0    200 _RXD	=	0x00b0
                    00B1    201 _TXD	=	0x00b1
                    00B2    202 _INT0	=	0x00b2
                    00B3    203 _INT1	=	0x00b3
                    00B4    204 _T0	=	0x00b4
                    00B5    205 _T1	=	0x00b5
                    00B6    206 _WR	=	0x00b6
                    00B7    207 _RD	=	0x00b7
                    00B8    208 _PX0	=	0x00b8
                    00B9    209 _PT0	=	0x00b9
                    00BA    210 _PX1	=	0x00ba
                    00BB    211 _PT1	=	0x00bb
                    00BC    212 _PS	=	0x00bc
                    00D0    213 _P	=	0x00d0
                    00D1    214 _F1	=	0x00d1
                    00D2    215 _OV	=	0x00d2
                    00D3    216 _RS0	=	0x00d3
                    00D4    217 _RS1	=	0x00d4
                    00D5    218 _F0	=	0x00d5
                    00D6    219 _AC	=	0x00d6
                    00D7    220 _CY	=	0x00d7
                            221 ;--------------------------------------------------------
                            222 ; overlayable register banks
                            223 ;--------------------------------------------------------
                            224 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     225 	.ds 8
                            226 ;--------------------------------------------------------
                            227 ; internal ram data
                            228 ;--------------------------------------------------------
                            229 	.area DSEG    (DATA)
   0008                     230 _display_i_1_1:
   0008                     231 	.ds 4
   000C                     232 _display_d1_1_1:
   000C                     233 	.ds 1
   000D                     234 _display_d2_1_1:
   000D                     235 	.ds 1
   000E                     236 _display_d3_1_1:
   000E                     237 	.ds 1
   000F                     238 _display_sloc0_1_0:
   000F                     239 	.ds 4
   0013                     240 _i::
   0013                     241 	.ds 4
                            242 ;--------------------------------------------------------
                            243 ; overlayable items in internal ram 
                            244 ;--------------------------------------------------------
                            245 	.area	OSEG    (OVR,DATA)
                            246 ;--------------------------------------------------------
                            247 ; Stack segment in internal ram 
                            248 ;--------------------------------------------------------
                            249 	.area	SSEG	(DATA)
   001B                     250 __start__stack:
   001B                     251 	.ds	1
                            252 
                            253 ;--------------------------------------------------------
                            254 ; indirectly addressable internal ram data
                            255 ;--------------------------------------------------------
                            256 	.area ISEG    (DATA)
                            257 ;--------------------------------------------------------
                            258 ; absolute internal ram data
                            259 ;--------------------------------------------------------
                            260 	.area IABS    (ABS,DATA)
                            261 	.area IABS    (ABS,DATA)
                            262 ;--------------------------------------------------------
                            263 ; bit data
                            264 ;--------------------------------------------------------
                            265 	.area BSEG    (BIT)
                            266 ;--------------------------------------------------------
                            267 ; paged external ram data
                            268 ;--------------------------------------------------------
                            269 	.area PSEG    (PAG,XDATA)
                            270 ;--------------------------------------------------------
                            271 ; external ram data
                            272 ;--------------------------------------------------------
                            273 	.area XSEG    (XDATA)
                            274 ;--------------------------------------------------------
                            275 ; absolute external ram data
                            276 ;--------------------------------------------------------
                            277 	.area XABS    (ABS,XDATA)
                            278 ;--------------------------------------------------------
                            279 ; external initialized ram data
                            280 ;--------------------------------------------------------
                            281 	.area XISEG   (XDATA)
                            282 	.area HOME    (CODE)
                            283 	.area GSINIT0 (CODE)
                            284 	.area GSINIT1 (CODE)
                            285 	.area GSINIT2 (CODE)
                            286 	.area GSINIT3 (CODE)
                            287 	.area GSINIT4 (CODE)
                            288 	.area GSINIT5 (CODE)
                            289 	.area GSINIT  (CODE)
                            290 	.area GSFINAL (CODE)
                            291 	.area CSEG    (CODE)
                            292 ;--------------------------------------------------------
                            293 ; interrupt vector 
                            294 ;--------------------------------------------------------
                            295 	.area HOME    (CODE)
   0000                     296 __interrupt_vect:
   0000 02 00 13            297 	ljmp	__sdcc_gsinit_startup
   0003 32                  298 	reti
   0004                     299 	.ds	7
   000B 02 05 9F            300 	ljmp	_timer
                            301 ;--------------------------------------------------------
                            302 ; global & static initialisations
                            303 ;--------------------------------------------------------
                            304 	.area HOME    (CODE)
                            305 	.area GSINIT  (CODE)
                            306 	.area GSFINAL (CODE)
                            307 	.area GSINIT  (CODE)
                            308 	.globl __sdcc_gsinit_startup
                            309 	.globl __sdcc_program_startup
                            310 	.globl __start__stack
                            311 	.globl __mcs51_genXINIT
                            312 	.globl __mcs51_genXRAMCLEAR
                            313 	.globl __mcs51_genRAMCLEAR
                            314 ;	test3.c:84: unsigned long int i=0;
   006C E4                  315 	clr	a
   006D F5 13               316 	mov	_i,a
   006F F5 14               317 	mov	(_i + 1),a
   0071 F5 15               318 	mov	(_i + 2),a
   0073 F5 16               319 	mov	(_i + 3),a
                            320 	.area GSFINAL (CODE)
   0075 02 00 0E            321 	ljmp	__sdcc_program_startup
                            322 ;--------------------------------------------------------
                            323 ; Home
                            324 ;--------------------------------------------------------
                            325 	.area HOME    (CODE)
                            326 	.area HOME    (CODE)
   000E                     327 __sdcc_program_startup:
   000E 12 05 81            328 	lcall	_main
                            329 ;	return from main will lock up
   0011 80 FE               330 	sjmp .
                            331 ;--------------------------------------------------------
                            332 ; code
                            333 ;--------------------------------------------------------
                            334 	.area CSEG    (CODE)
                            335 ;------------------------------------------------------------
                            336 ;Allocation info for local variables in function 'delay'
                            337 ;------------------------------------------------------------
                            338 ;i                         Allocated to registers r6 r7 
                            339 ;a                         Allocated to registers r4 r5 
                            340 ;b                         Allocated to registers r2 r3 
                            341 ;------------------------------------------------------------
                            342 ;	test3.c:6: void delay(int i)
                            343 ;	-----------------------------------------
                            344 ;	 function delay
                            345 ;	-----------------------------------------
   0078                     346 _delay:
                    0007    347 	ar7 = 0x07
                    0006    348 	ar6 = 0x06
                    0005    349 	ar5 = 0x05
                    0004    350 	ar4 = 0x04
                    0003    351 	ar3 = 0x03
                    0002    352 	ar2 = 0x02
                    0001    353 	ar1 = 0x01
                    0000    354 	ar0 = 0x00
   0078 AE 82               355 	mov	r6,dpl
   007A AF 83               356 	mov	r7,dph
                            357 ;	test3.c:9: for(a=0;a<i;a++)
   007C 7C 00               358 	mov	r4,#0x00
   007E 7D 00               359 	mov	r5,#0x00
   0080                     360 00104$:
   0080 C3                  361 	clr	c
   0081 EC                  362 	mov	a,r4
   0082 9E                  363 	subb	a,r6
   0083 ED                  364 	mov	a,r5
   0084 64 80               365 	xrl	a,#0x80
   0086 8F F0               366 	mov	b,r7
   0088 63 F0 80            367 	xrl	b,#0x80
   008B 95 F0               368 	subb	a,b
   008D 50 14               369 	jnc	00108$
                            370 ;	test3.c:11: for(b=0;b<120;b++);
   008F 7A 78               371 	mov	r2,#0x78
   0091 7B 00               372 	mov	r3,#0x00
   0093                     373 00103$:
   0093 1A                  374 	dec	r2
   0094 BA FF 01            375 	cjne	r2,#0xFF,00117$
   0097 1B                  376 	dec	r3
   0098                     377 00117$:
   0098 EA                  378 	mov	a,r2
   0099 4B                  379 	orl	a,r3
   009A 70 F7               380 	jnz	00103$
                            381 ;	test3.c:9: for(a=0;a<i;a++)
   009C 0C                  382 	inc	r4
   009D BC 00 E0            383 	cjne	r4,#0x00,00104$
   00A0 0D                  384 	inc	r5
   00A1 80 DD               385 	sjmp	00104$
   00A3                     386 00108$:
   00A3 22                  387 	ret
                            388 ;------------------------------------------------------------
                            389 ;Allocation info for local variables in function 'display'
                            390 ;------------------------------------------------------------
                            391 ;i                         Allocated with name '_display_i_1_1'
                            392 ;d0                        Allocated to registers 
                            393 ;d1                        Allocated with name '_display_d1_1_1'
                            394 ;d2                        Allocated with name '_display_d2_1_1'
                            395 ;d3                        Allocated with name '_display_d3_1_1'
                            396 ;sloc0                     Allocated with name '_display_sloc0_1_0'
                            397 ;------------------------------------------------------------
                            398 ;	test3.c:21: void display(unsigned long int i)
                            399 ;	-----------------------------------------
                            400 ;	 function display
                            401 ;	-----------------------------------------
   00A4                     402 _display:
   00A4 85 82 08            403 	mov	_display_i_1_1,dpl
   00A7 85 83 09            404 	mov	(_display_i_1_1 + 1),dph
   00AA 85 F0 0A            405 	mov	(_display_i_1_1 + 2),b
   00AD F5 0B               406 	mov	(_display_i_1_1 + 3),a
                            407 ;	test3.c:25: unsigned char d1=0xFF;
   00AF 75 0C FF            408 	mov	_display_d1_1_1,#0xFF
                            409 ;	test3.c:26: unsigned char d2=0x7F;
   00B2 75 0D 7F            410 	mov	_display_d2_1_1,#0x7F
                            411 ;	test3.c:27: unsigned char d3=0xFF;
   00B5 75 0E FF            412 	mov	_display_d3_1_1,#0xFF
                            413 ;	test3.c:29: if(i/600>=60*100)
   00B8 75 17 58            414 	mov	__divulong_PARM_2,#0x58
   00BB 75 18 02            415 	mov	(__divulong_PARM_2 + 1),#0x02
   00BE E4                  416 	clr	a
   00BF F5 19               417 	mov	(__divulong_PARM_2 + 2),a
   00C1 F5 1A               418 	mov	(__divulong_PARM_2 + 3),a
   00C3 85 08 82            419 	mov	dpl,_display_i_1_1
   00C6 85 09 83            420 	mov	dph,(_display_i_1_1 + 1)
   00C9 85 0A F0            421 	mov	b,(_display_i_1_1 + 2)
   00CC E5 0B               422 	mov	a,(_display_i_1_1 + 3)
   00CE 12 06 43            423 	lcall	__divulong
   00D1 A8 82               424 	mov	r0,dpl
   00D3 A9 83               425 	mov	r1,dph
   00D5 AA F0               426 	mov	r2,b
   00D7 FB                  427 	mov	r3,a
   00D8 C3                  428 	clr	c
   00D9 E8                  429 	mov	a,r0
   00DA 94 70               430 	subb	a,#0x70
   00DC E9                  431 	mov	a,r1
   00DD 94 17               432 	subb	a,#0x17
   00DF EA                  433 	mov	a,r2
   00E0 94 00               434 	subb	a,#0x00
   00E2 EB                  435 	mov	a,r3
   00E3 94 00               436 	subb	a,#0x00
   00E5 50 03               437 	jnc	00118$
   00E7 02 01 B0            438 	ljmp	00110$
   00EA                     439 00118$:
                            440 ;	test3.c:31: i=i/60/60/(unsigned long int)100*(unsigned long int)1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/(unsigned long int)6000*(unsigned long int)10000;
   00EA 75 17 00            441 	mov	__divulong_PARM_2,#0x00
   00ED 75 18 97            442 	mov	(__divulong_PARM_2 + 1),#0x97
   00F0 75 19 49            443 	mov	(__divulong_PARM_2 + 2),#0x49
   00F3 75 1A 01            444 	mov	(__divulong_PARM_2 + 3),#0x01
   00F6 85 08 82            445 	mov	dpl,_display_i_1_1
   00F9 85 09 83            446 	mov	dph,(_display_i_1_1 + 1)
   00FC 85 0A F0            447 	mov	b,(_display_i_1_1 + 2)
   00FF E5 0B               448 	mov	a,(_display_i_1_1 + 3)
   0101 12 06 43            449 	lcall	__divulong
   0104 85 82 17            450 	mov	__mullong_PARM_2,dpl
   0107 85 83 18            451 	mov	(__mullong_PARM_2 + 1),dph
   010A 85 F0 19            452 	mov	(__mullong_PARM_2 + 2),b
   010D F5 1A               453 	mov	(__mullong_PARM_2 + 3),a
   010F 90 42 40            454 	mov	dptr,#0x4240
   0112 75 F0 0F            455 	mov	b,#0x0F
   0115 E4                  456 	clr	a
   0116 12 06 A8            457 	lcall	__mullong
   0119 85 82 0F            458 	mov	_display_sloc0_1_0,dpl
   011C 85 83 10            459 	mov	(_display_sloc0_1_0 + 1),dph
   011F 85 F0 11            460 	mov	(_display_sloc0_1_0 + 2),b
   0122 F5 12               461 	mov	(_display_sloc0_1_0 + 3),a
   0124 75 17 40            462 	mov	__modulong_PARM_2,#0x40
   0127 75 18 7E            463 	mov	(__modulong_PARM_2 + 1),#0x7E
   012A 75 19 05            464 	mov	(__modulong_PARM_2 + 2),#0x05
   012D 75 1A 00            465 	mov	(__modulong_PARM_2 + 3),#0x00
   0130 85 08 82            466 	mov	dpl,_display_i_1_1
   0133 85 09 83            467 	mov	dph,(_display_i_1_1 + 1)
   0136 85 0A F0            468 	mov	b,(_display_i_1_1 + 2)
   0139 E5 0B               469 	mov	a,(_display_i_1_1 + 3)
   013B 12 05 C0            470 	lcall	__modulong
   013E A8 82               471 	mov	r0,dpl
   0140 A9 83               472 	mov	r1,dph
   0142 AA F0               473 	mov	r2,b
   0144 FB                  474 	mov	r3,a
   0145 75 17 70            475 	mov	__divulong_PARM_2,#0x70
   0148 75 18 17            476 	mov	(__divulong_PARM_2 + 1),#0x17
   014B E4                  477 	clr	a
   014C F5 19               478 	mov	(__divulong_PARM_2 + 2),a
   014E F5 1A               479 	mov	(__divulong_PARM_2 + 3),a
   0150 88 82               480 	mov	dpl,r0
   0152 89 83               481 	mov	dph,r1
   0154 8A F0               482 	mov	b,r2
   0156 EB                  483 	mov	a,r3
   0157 12 06 43            484 	lcall	__divulong
   015A 85 82 17            485 	mov	__mullong_PARM_2,dpl
   015D 85 83 18            486 	mov	(__mullong_PARM_2 + 1),dph
   0160 85 F0 19            487 	mov	(__mullong_PARM_2 + 2),b
   0163 F5 1A               488 	mov	(__mullong_PARM_2 + 3),a
   0165 90 27 10            489 	mov	dptr,#0x2710
   0168 E4                  490 	clr	a
   0169 F5 F0               491 	mov	b,a
   016B 12 06 A8            492 	lcall	__mullong
   016E A8 82               493 	mov	r0,dpl
   0170 A9 83               494 	mov	r1,dph
   0172 AA F0               495 	mov	r2,b
   0174 FB                  496 	mov	r3,a
   0175 E8                  497 	mov	a,r0
   0176 25 0F               498 	add	a,_display_sloc0_1_0
   0178 F5 08               499 	mov	_display_i_1_1,a
   017A E9                  500 	mov	a,r1
   017B 35 10               501 	addc	a,(_display_sloc0_1_0 + 1)
   017D F5 09               502 	mov	(_display_i_1_1 + 1),a
   017F EA                  503 	mov	a,r2
   0180 35 11               504 	addc	a,(_display_sloc0_1_0 + 2)
   0182 F5 0A               505 	mov	(_display_i_1_1 + 2),a
   0184 EB                  506 	mov	a,r3
   0185 35 12               507 	addc	a,(_display_sloc0_1_0 + 3)
   0187 F5 0B               508 	mov	(_display_i_1_1 + 3),a
                            509 ;	test3.c:32: i/=(unsigned long int)10000;
   0189 75 17 10            510 	mov	__divulong_PARM_2,#0x10
   018C 75 18 27            511 	mov	(__divulong_PARM_2 + 1),#0x27
   018F E4                  512 	clr	a
   0190 F5 19               513 	mov	(__divulong_PARM_2 + 2),a
   0192 F5 1A               514 	mov	(__divulong_PARM_2 + 3),a
   0194 85 08 82            515 	mov	dpl,_display_i_1_1
   0197 85 09 83            516 	mov	dph,(_display_i_1_1 + 1)
   019A 85 0A F0            517 	mov	b,(_display_i_1_1 + 2)
   019D E5 0B               518 	mov	a,(_display_i_1_1 + 3)
   019F 12 06 43            519 	lcall	__divulong
   01A2 85 82 08            520 	mov	_display_i_1_1,dpl
   01A5 85 83 09            521 	mov	(_display_i_1_1 + 1),dph
   01A8 85 F0 0A            522 	mov	(_display_i_1_1 + 2),b
   01AB F5 0B               523 	mov	(_display_i_1_1 + 3),a
   01AD 02 04 5A            524 	ljmp	00111$
   01B0                     525 00110$:
                            526 ;	test3.c:37: else if(i/60>=60*100)
   01B0 75 17 3C            527 	mov	__divulong_PARM_2,#0x3C
   01B3 E4                  528 	clr	a
   01B4 F5 18               529 	mov	(__divulong_PARM_2 + 1),a
   01B6 F5 19               530 	mov	(__divulong_PARM_2 + 2),a
   01B8 F5 1A               531 	mov	(__divulong_PARM_2 + 3),a
   01BA 85 08 82            532 	mov	dpl,_display_i_1_1
   01BD 85 09 83            533 	mov	dph,(_display_i_1_1 + 1)
   01C0 85 0A F0            534 	mov	b,(_display_i_1_1 + 2)
   01C3 E5 0B               535 	mov	a,(_display_i_1_1 + 3)
   01C5 12 06 43            536 	lcall	__divulong
   01C8 A8 82               537 	mov	r0,dpl
   01CA A9 83               538 	mov	r1,dph
   01CC AA F0               539 	mov	r2,b
   01CE FB                  540 	mov	r3,a
   01CF C3                  541 	clr	c
   01D0 E8                  542 	mov	a,r0
   01D1 94 70               543 	subb	a,#0x70
   01D3 E9                  544 	mov	a,r1
   01D4 94 17               545 	subb	a,#0x17
   01D6 EA                  546 	mov	a,r2
   01D7 94 00               547 	subb	a,#0x00
   01D9 EB                  548 	mov	a,r3
   01DA 94 00               549 	subb	a,#0x00
   01DC 50 03               550 	jnc	00119$
   01DE 02 02 F4            551 	ljmp	00107$
   01E1                     552 00119$:
                            553 ;	test3.c:39: i=i/60/60/(unsigned long int)100*(unsigned long int)1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/(unsigned long int)6000*(unsigned long int)10000 + i%(unsigned long int)6000;
   01E1 75 17 00            554 	mov	__divulong_PARM_2,#0x00
   01E4 75 18 97            555 	mov	(__divulong_PARM_2 + 1),#0x97
   01E7 75 19 49            556 	mov	(__divulong_PARM_2 + 2),#0x49
   01EA 75 1A 01            557 	mov	(__divulong_PARM_2 + 3),#0x01
   01ED 85 08 82            558 	mov	dpl,_display_i_1_1
   01F0 85 09 83            559 	mov	dph,(_display_i_1_1 + 1)
   01F3 85 0A F0            560 	mov	b,(_display_i_1_1 + 2)
   01F6 E5 0B               561 	mov	a,(_display_i_1_1 + 3)
   01F8 12 06 43            562 	lcall	__divulong
   01FB 85 82 17            563 	mov	__mullong_PARM_2,dpl
   01FE 85 83 18            564 	mov	(__mullong_PARM_2 + 1),dph
   0201 85 F0 19            565 	mov	(__mullong_PARM_2 + 2),b
   0204 F5 1A               566 	mov	(__mullong_PARM_2 + 3),a
   0206 90 42 40            567 	mov	dptr,#0x4240
   0209 75 F0 0F            568 	mov	b,#0x0F
   020C E4                  569 	clr	a
   020D 12 06 A8            570 	lcall	__mullong
   0210 A8 82               571 	mov	r0,dpl
   0212 A9 83               572 	mov	r1,dph
   0214 AA F0               573 	mov	r2,b
   0216 FB                  574 	mov	r3,a
   0217 75 17 40            575 	mov	__modulong_PARM_2,#0x40
   021A 75 18 7E            576 	mov	(__modulong_PARM_2 + 1),#0x7E
   021D 75 19 05            577 	mov	(__modulong_PARM_2 + 2),#0x05
   0220 75 1A 00            578 	mov	(__modulong_PARM_2 + 3),#0x00
   0223 85 08 82            579 	mov	dpl,_display_i_1_1
   0226 85 09 83            580 	mov	dph,(_display_i_1_1 + 1)
   0229 85 0A F0            581 	mov	b,(_display_i_1_1 + 2)
   022C E5 0B               582 	mov	a,(_display_i_1_1 + 3)
   022E C0 03               583 	push	ar3
   0230 C0 02               584 	push	ar2
   0232 C0 01               585 	push	ar1
   0234 C0 00               586 	push	ar0
   0236 12 05 C0            587 	lcall	__modulong
   0239 AC 82               588 	mov	r4,dpl
   023B AD 83               589 	mov	r5,dph
   023D AE F0               590 	mov	r6,b
   023F FF                  591 	mov	r7,a
   0240 75 17 70            592 	mov	__divulong_PARM_2,#0x70
   0243 75 18 17            593 	mov	(__divulong_PARM_2 + 1),#0x17
   0246 E4                  594 	clr	a
   0247 F5 19               595 	mov	(__divulong_PARM_2 + 2),a
   0249 F5 1A               596 	mov	(__divulong_PARM_2 + 3),a
   024B 8C 82               597 	mov	dpl,r4
   024D 8D 83               598 	mov	dph,r5
   024F 8E F0               599 	mov	b,r6
   0251 EF                  600 	mov	a,r7
   0252 12 06 43            601 	lcall	__divulong
   0255 85 82 17            602 	mov	__mullong_PARM_2,dpl
   0258 85 83 18            603 	mov	(__mullong_PARM_2 + 1),dph
   025B 85 F0 19            604 	mov	(__mullong_PARM_2 + 2),b
   025E F5 1A               605 	mov	(__mullong_PARM_2 + 3),a
   0260 90 27 10            606 	mov	dptr,#0x2710
   0263 E4                  607 	clr	a
   0264 F5 F0               608 	mov	b,a
   0266 12 06 A8            609 	lcall	__mullong
   0269 AC 82               610 	mov	r4,dpl
   026B AD 83               611 	mov	r5,dph
   026D AE F0               612 	mov	r6,b
   026F FF                  613 	mov	r7,a
   0270 D0 00               614 	pop	ar0
   0272 D0 01               615 	pop	ar1
   0274 D0 02               616 	pop	ar2
   0276 D0 03               617 	pop	ar3
   0278 EC                  618 	mov	a,r4
   0279 28                  619 	add	a,r0
   027A F8                  620 	mov	r0,a
   027B ED                  621 	mov	a,r5
   027C 39                  622 	addc	a,r1
   027D F9                  623 	mov	r1,a
   027E EE                  624 	mov	a,r6
   027F 3A                  625 	addc	a,r2
   0280 FA                  626 	mov	r2,a
   0281 EF                  627 	mov	a,r7
   0282 3B                  628 	addc	a,r3
   0283 FB                  629 	mov	r3,a
   0284 75 17 70            630 	mov	__modulong_PARM_2,#0x70
   0287 75 18 17            631 	mov	(__modulong_PARM_2 + 1),#0x17
   028A E4                  632 	clr	a
   028B F5 19               633 	mov	(__modulong_PARM_2 + 2),a
   028D F5 1A               634 	mov	(__modulong_PARM_2 + 3),a
   028F 85 08 82            635 	mov	dpl,_display_i_1_1
   0292 85 09 83            636 	mov	dph,(_display_i_1_1 + 1)
   0295 85 0A F0            637 	mov	b,(_display_i_1_1 + 2)
   0298 E5 0B               638 	mov	a,(_display_i_1_1 + 3)
   029A C0 03               639 	push	ar3
   029C C0 02               640 	push	ar2
   029E C0 01               641 	push	ar1
   02A0 C0 00               642 	push	ar0
   02A2 12 05 C0            643 	lcall	__modulong
   02A5 AC 82               644 	mov	r4,dpl
   02A7 AD 83               645 	mov	r5,dph
   02A9 AE F0               646 	mov	r6,b
   02AB FF                  647 	mov	r7,a
   02AC D0 00               648 	pop	ar0
   02AE D0 01               649 	pop	ar1
   02B0 D0 02               650 	pop	ar2
   02B2 D0 03               651 	pop	ar3
   02B4 EC                  652 	mov	a,r4
   02B5 28                  653 	add	a,r0
   02B6 F5 08               654 	mov	_display_i_1_1,a
   02B8 ED                  655 	mov	a,r5
   02B9 39                  656 	addc	a,r1
   02BA F5 09               657 	mov	(_display_i_1_1 + 1),a
   02BC EE                  658 	mov	a,r6
   02BD 3A                  659 	addc	a,r2
   02BE F5 0A               660 	mov	(_display_i_1_1 + 2),a
   02C0 EF                  661 	mov	a,r7
   02C1 3B                  662 	addc	a,r3
   02C2 F5 0B               663 	mov	(_display_i_1_1 + 3),a
                            664 ;	test3.c:40: i/=1000;
   02C4 75 17 E8            665 	mov	__divulong_PARM_2,#0xE8
   02C7 75 18 03            666 	mov	(__divulong_PARM_2 + 1),#0x03
   02CA E4                  667 	clr	a
   02CB F5 19               668 	mov	(__divulong_PARM_2 + 2),a
   02CD F5 1A               669 	mov	(__divulong_PARM_2 + 3),a
   02CF 85 08 82            670 	mov	dpl,_display_i_1_1
   02D2 85 09 83            671 	mov	dph,(_display_i_1_1 + 1)
   02D5 85 0A F0            672 	mov	b,(_display_i_1_1 + 2)
   02D8 E5 0B               673 	mov	a,(_display_i_1_1 + 3)
   02DA 12 06 43            674 	lcall	__divulong
   02DD 85 82 08            675 	mov	_display_i_1_1,dpl
   02E0 85 83 09            676 	mov	(_display_i_1_1 + 1),dph
   02E3 85 F0 0A            677 	mov	(_display_i_1_1 + 2),b
   02E6 F5 0B               678 	mov	(_display_i_1_1 + 3),a
                            679 ;	test3.c:41: d2=0xFF;
   02E8 75 0D FF            680 	mov	_display_d2_1_1,#0xFF
                            681 ;	test3.c:42: d1=0x7F;
   02EB 75 0C 7F            682 	mov	_display_d1_1_1,#0x7F
                            683 ;	test3.c:43: d3=0x7F;
   02EE 75 0E 7F            684 	mov	_display_d3_1_1,#0x7F
   02F1 02 04 5A            685 	ljmp	00111$
   02F4                     686 00107$:
                            687 ;	test3.c:45: else if(i>=60000)
   02F4 C3                  688 	clr	c
   02F5 E5 08               689 	mov	a,_display_i_1_1
   02F7 94 60               690 	subb	a,#0x60
   02F9 E5 09               691 	mov	a,(_display_i_1_1 + 1)
   02FB 94 EA               692 	subb	a,#0xEA
   02FD E5 0A               693 	mov	a,(_display_i_1_1 + 2)
   02FF 94 00               694 	subb	a,#0x00
   0301 E5 0B               695 	mov	a,(_display_i_1_1 + 3)
   0303 94 00               696 	subb	a,#0x00
   0305 50 03               697 	jnc	00120$
   0307 02 03 A4            698 	ljmp	00104$
   030A                     699 00120$:
                            700 ;	test3.c:47: i=i/6000*10000+i%6000;
   030A 75 17 70            701 	mov	__divulong_PARM_2,#0x70
   030D 75 18 17            702 	mov	(__divulong_PARM_2 + 1),#0x17
   0310 E4                  703 	clr	a
   0311 F5 19               704 	mov	(__divulong_PARM_2 + 2),a
   0313 F5 1A               705 	mov	(__divulong_PARM_2 + 3),a
   0315 85 08 82            706 	mov	dpl,_display_i_1_1
   0318 85 09 83            707 	mov	dph,(_display_i_1_1 + 1)
   031B 85 0A F0            708 	mov	b,(_display_i_1_1 + 2)
   031E E5 0B               709 	mov	a,(_display_i_1_1 + 3)
   0320 12 06 43            710 	lcall	__divulong
   0323 85 82 17            711 	mov	__mullong_PARM_2,dpl
   0326 85 83 18            712 	mov	(__mullong_PARM_2 + 1),dph
   0329 85 F0 19            713 	mov	(__mullong_PARM_2 + 2),b
   032C F5 1A               714 	mov	(__mullong_PARM_2 + 3),a
   032E 90 27 10            715 	mov	dptr,#0x2710
   0331 E4                  716 	clr	a
   0332 F5 F0               717 	mov	b,a
   0334 12 06 A8            718 	lcall	__mullong
   0337 AC 82               719 	mov	r4,dpl
   0339 AD 83               720 	mov	r5,dph
   033B AE F0               721 	mov	r6,b
   033D FF                  722 	mov	r7,a
   033E 75 17 70            723 	mov	__modulong_PARM_2,#0x70
   0341 75 18 17            724 	mov	(__modulong_PARM_2 + 1),#0x17
   0344 E4                  725 	clr	a
   0345 F5 19               726 	mov	(__modulong_PARM_2 + 2),a
   0347 F5 1A               727 	mov	(__modulong_PARM_2 + 3),a
   0349 85 08 82            728 	mov	dpl,_display_i_1_1
   034C 85 09 83            729 	mov	dph,(_display_i_1_1 + 1)
   034F 85 0A F0            730 	mov	b,(_display_i_1_1 + 2)
   0352 E5 0B               731 	mov	a,(_display_i_1_1 + 3)
   0354 C0 07               732 	push	ar7
   0356 C0 06               733 	push	ar6
   0358 C0 05               734 	push	ar5
   035A C0 04               735 	push	ar4
   035C 12 05 C0            736 	lcall	__modulong
   035F A8 82               737 	mov	r0,dpl
   0361 A9 83               738 	mov	r1,dph
   0363 AA F0               739 	mov	r2,b
   0365 FB                  740 	mov	r3,a
   0366 D0 04               741 	pop	ar4
   0368 D0 05               742 	pop	ar5
   036A D0 06               743 	pop	ar6
   036C D0 07               744 	pop	ar7
   036E E8                  745 	mov	a,r0
   036F 2C                  746 	add	a,r4
   0370 F5 08               747 	mov	_display_i_1_1,a
   0372 E9                  748 	mov	a,r1
   0373 3D                  749 	addc	a,r5
   0374 F5 09               750 	mov	(_display_i_1_1 + 1),a
   0376 EA                  751 	mov	a,r2
   0377 3E                  752 	addc	a,r6
   0378 F5 0A               753 	mov	(_display_i_1_1 + 2),a
   037A EB                  754 	mov	a,r3
   037B 3F                  755 	addc	a,r7
   037C F5 0B               756 	mov	(_display_i_1_1 + 3),a
                            757 ;	test3.c:48: i/=100;
   037E 75 17 64            758 	mov	__divulong_PARM_2,#0x64
   0381 E4                  759 	clr	a
   0382 F5 18               760 	mov	(__divulong_PARM_2 + 1),a
   0384 F5 19               761 	mov	(__divulong_PARM_2 + 2),a
   0386 F5 1A               762 	mov	(__divulong_PARM_2 + 3),a
   0388 85 08 82            763 	mov	dpl,_display_i_1_1
   038B 85 09 83            764 	mov	dph,(_display_i_1_1 + 1)
   038E 85 0A F0            765 	mov	b,(_display_i_1_1 + 2)
   0391 E5 0B               766 	mov	a,(_display_i_1_1 + 3)
   0393 12 06 43            767 	lcall	__divulong
   0396 85 82 08            768 	mov	_display_i_1_1,dpl
   0399 85 83 09            769 	mov	(_display_i_1_1 + 1),dph
   039C 85 F0 0A            770 	mov	(_display_i_1_1 + 2),b
   039F F5 0B               771 	mov	(_display_i_1_1 + 3),a
   03A1 02 04 5A            772 	ljmp	00111$
   03A4                     773 00104$:
                            774 ;	test3.c:53: else if(i>=6000)
   03A4 C3                  775 	clr	c
   03A5 E5 08               776 	mov	a,_display_i_1_1
   03A7 94 70               777 	subb	a,#0x70
   03A9 E5 09               778 	mov	a,(_display_i_1_1 + 1)
   03AB 94 17               779 	subb	a,#0x17
   03AD E5 0A               780 	mov	a,(_display_i_1_1 + 2)
   03AF 94 00               781 	subb	a,#0x00
   03B1 E5 0B               782 	mov	a,(_display_i_1_1 + 3)
   03B3 94 00               783 	subb	a,#0x00
   03B5 50 03               784 	jnc	00121$
   03B7 02 04 5A            785 	ljmp	00111$
   03BA                     786 00121$:
                            787 ;	test3.c:56: i=i/6000*10000+i%6000;
   03BA 75 17 70            788 	mov	__divulong_PARM_2,#0x70
   03BD 75 18 17            789 	mov	(__divulong_PARM_2 + 1),#0x17
   03C0 E4                  790 	clr	a
   03C1 F5 19               791 	mov	(__divulong_PARM_2 + 2),a
   03C3 F5 1A               792 	mov	(__divulong_PARM_2 + 3),a
   03C5 85 08 82            793 	mov	dpl,_display_i_1_1
   03C8 85 09 83            794 	mov	dph,(_display_i_1_1 + 1)
   03CB 85 0A F0            795 	mov	b,(_display_i_1_1 + 2)
   03CE E5 0B               796 	mov	a,(_display_i_1_1 + 3)
   03D0 12 06 43            797 	lcall	__divulong
   03D3 85 82 17            798 	mov	__mullong_PARM_2,dpl
   03D6 85 83 18            799 	mov	(__mullong_PARM_2 + 1),dph
   03D9 85 F0 19            800 	mov	(__mullong_PARM_2 + 2),b
   03DC F5 1A               801 	mov	(__mullong_PARM_2 + 3),a
   03DE 90 27 10            802 	mov	dptr,#0x2710
   03E1 E4                  803 	clr	a
   03E2 F5 F0               804 	mov	b,a
   03E4 12 06 A8            805 	lcall	__mullong
   03E7 AC 82               806 	mov	r4,dpl
   03E9 AD 83               807 	mov	r5,dph
   03EB AE F0               808 	mov	r6,b
   03ED FF                  809 	mov	r7,a
   03EE 75 17 70            810 	mov	__modulong_PARM_2,#0x70
   03F1 75 18 17            811 	mov	(__modulong_PARM_2 + 1),#0x17
   03F4 E4                  812 	clr	a
   03F5 F5 19               813 	mov	(__modulong_PARM_2 + 2),a
   03F7 F5 1A               814 	mov	(__modulong_PARM_2 + 3),a
   03F9 85 08 82            815 	mov	dpl,_display_i_1_1
   03FC 85 09 83            816 	mov	dph,(_display_i_1_1 + 1)
   03FF 85 0A F0            817 	mov	b,(_display_i_1_1 + 2)
   0402 E5 0B               818 	mov	a,(_display_i_1_1 + 3)
   0404 C0 07               819 	push	ar7
   0406 C0 06               820 	push	ar6
   0408 C0 05               821 	push	ar5
   040A C0 04               822 	push	ar4
   040C 12 05 C0            823 	lcall	__modulong
   040F A8 82               824 	mov	r0,dpl
   0411 A9 83               825 	mov	r1,dph
   0413 AA F0               826 	mov	r2,b
   0415 FB                  827 	mov	r3,a
   0416 D0 04               828 	pop	ar4
   0418 D0 05               829 	pop	ar5
   041A D0 06               830 	pop	ar6
   041C D0 07               831 	pop	ar7
   041E E8                  832 	mov	a,r0
   041F 2C                  833 	add	a,r4
   0420 F5 08               834 	mov	_display_i_1_1,a
   0422 E9                  835 	mov	a,r1
   0423 3D                  836 	addc	a,r5
   0424 F5 09               837 	mov	(_display_i_1_1 + 1),a
   0426 EA                  838 	mov	a,r2
   0427 3E                  839 	addc	a,r6
   0428 F5 0A               840 	mov	(_display_i_1_1 + 2),a
   042A EB                  841 	mov	a,r3
   042B 3F                  842 	addc	a,r7
   042C F5 0B               843 	mov	(_display_i_1_1 + 3),a
                            844 ;	test3.c:57: i/=10;
   042E 75 17 0A            845 	mov	__divulong_PARM_2,#0x0A
   0431 E4                  846 	clr	a
   0432 F5 18               847 	mov	(__divulong_PARM_2 + 1),a
   0434 F5 19               848 	mov	(__divulong_PARM_2 + 2),a
   0436 F5 1A               849 	mov	(__divulong_PARM_2 + 3),a
   0438 85 08 82            850 	mov	dpl,_display_i_1_1
   043B 85 09 83            851 	mov	dph,(_display_i_1_1 + 1)
   043E 85 0A F0            852 	mov	b,(_display_i_1_1 + 2)
   0441 E5 0B               853 	mov	a,(_display_i_1_1 + 3)
   0443 12 06 43            854 	lcall	__divulong
   0446 85 82 08            855 	mov	_display_i_1_1,dpl
   0449 85 83 09            856 	mov	(_display_i_1_1 + 1),dph
   044C 85 F0 0A            857 	mov	(_display_i_1_1 + 2),b
   044F F5 0B               858 	mov	(_display_i_1_1 + 3),a
                            859 ;	test3.c:58: d2=0xFF;
   0451 75 0D FF            860 	mov	_display_d2_1_1,#0xFF
                            861 ;	test3.c:59: d1=0x7F;
   0454 75 0C 7F            862 	mov	_display_d1_1_1,#0x7F
                            863 ;	test3.c:60: d3=0x7F;
   0457 75 0E 7F            864 	mov	_display_d3_1_1,#0x7F
   045A                     865 00111$:
                            866 ;	test3.c:62: P0=255;
   045A 75 80 FF            867 	mov	_P0,#0xFF
                            868 ;	test3.c:64: P2 = seg_position[3];
   045D 90 07 28            869 	mov	dptr,#(_seg_position + 0x0003)
   0460 E4                  870 	clr	a
   0461 93                  871 	movc	a,@a+dptr
   0462 F5 A0               872 	mov	_P2,a
                            873 ;	test3.c:65: P0=display_seg[i/1000] & d3;
   0464 75 17 E8            874 	mov	__divulong_PARM_2,#0xE8
   0467 75 18 03            875 	mov	(__divulong_PARM_2 + 1),#0x03
   046A E4                  876 	clr	a
   046B F5 19               877 	mov	(__divulong_PARM_2 + 2),a
   046D F5 1A               878 	mov	(__divulong_PARM_2 + 3),a
   046F 85 08 82            879 	mov	dpl,_display_i_1_1
   0472 85 09 83            880 	mov	dph,(_display_i_1_1 + 1)
   0475 85 0A F0            881 	mov	b,(_display_i_1_1 + 2)
   0478 E5 0B               882 	mov	a,(_display_i_1_1 + 3)
   047A 12 06 43            883 	lcall	__divulong
   047D AC 82               884 	mov	r4,dpl
   047F AD 83               885 	mov	r5,dph
   0481 EC                  886 	mov	a,r4
   0482 24 1A               887 	add	a,#_display_seg
   0484 F5 82               888 	mov	dpl,a
   0486 ED                  889 	mov	a,r5
   0487 34 07               890 	addc	a,#(_display_seg >> 8)
   0489 F5 83               891 	mov	dph,a
   048B E4                  892 	clr	a
   048C 93                  893 	movc	a,@a+dptr
   048D FF                  894 	mov	r7,a
   048E E5 0E               895 	mov	a,_display_d3_1_1
   0490 5F                  896 	anl	a,r7
   0491 F5 80               897 	mov	_P0,a
                            898 ;	test3.c:66: delay(3);
   0493 90 00 03            899 	mov	dptr,#0x0003
   0496 12 00 78            900 	lcall	_delay
                            901 ;	test3.c:68: P0=255;
   0499 75 80 FF            902 	mov	_P0,#0xFF
                            903 ;	test3.c:69: P2 = seg_position[2];
   049C 90 07 27            904 	mov	dptr,#(_seg_position + 0x0002)
   049F E4                  905 	clr	a
   04A0 93                  906 	movc	a,@a+dptr
   04A1 F5 A0               907 	mov	_P2,a
                            908 ;	test3.c:70: P0=display_seg[i%1000/100] & d2;
   04A3 75 17 E8            909 	mov	__modulong_PARM_2,#0xE8
   04A6 75 18 03            910 	mov	(__modulong_PARM_2 + 1),#0x03
   04A9 E4                  911 	clr	a
   04AA F5 19               912 	mov	(__modulong_PARM_2 + 2),a
   04AC F5 1A               913 	mov	(__modulong_PARM_2 + 3),a
   04AE 85 08 82            914 	mov	dpl,_display_i_1_1
   04B1 85 09 83            915 	mov	dph,(_display_i_1_1 + 1)
   04B4 85 0A F0            916 	mov	b,(_display_i_1_1 + 2)
   04B7 E5 0B               917 	mov	a,(_display_i_1_1 + 3)
   04B9 12 05 C0            918 	lcall	__modulong
   04BC AC 82               919 	mov	r4,dpl
   04BE AD 83               920 	mov	r5,dph
   04C0 AE F0               921 	mov	r6,b
   04C2 FF                  922 	mov	r7,a
   04C3 75 17 64            923 	mov	__divulong_PARM_2,#0x64
   04C6 E4                  924 	clr	a
   04C7 F5 18               925 	mov	(__divulong_PARM_2 + 1),a
   04C9 F5 19               926 	mov	(__divulong_PARM_2 + 2),a
   04CB F5 1A               927 	mov	(__divulong_PARM_2 + 3),a
   04CD 8C 82               928 	mov	dpl,r4
   04CF 8D 83               929 	mov	dph,r5
   04D1 8E F0               930 	mov	b,r6
   04D3 EF                  931 	mov	a,r7
   04D4 12 06 43            932 	lcall	__divulong
   04D7 AC 82               933 	mov	r4,dpl
   04D9 AD 83               934 	mov	r5,dph
   04DB EC                  935 	mov	a,r4
   04DC 24 1A               936 	add	a,#_display_seg
   04DE F5 82               937 	mov	dpl,a
   04E0 ED                  938 	mov	a,r5
   04E1 34 07               939 	addc	a,#(_display_seg >> 8)
   04E3 F5 83               940 	mov	dph,a
   04E5 E4                  941 	clr	a
   04E6 93                  942 	movc	a,@a+dptr
   04E7 FF                  943 	mov	r7,a
   04E8 E5 0D               944 	mov	a,_display_d2_1_1
   04EA 5F                  945 	anl	a,r7
   04EB F5 80               946 	mov	_P0,a
                            947 ;	test3.c:71: delay(3);
   04ED 90 00 03            948 	mov	dptr,#0x0003
   04F0 12 00 78            949 	lcall	_delay
                            950 ;	test3.c:72: P0=255;
   04F3 75 80 FF            951 	mov	_P0,#0xFF
                            952 ;	test3.c:73: P2 = seg_position[1];
   04F6 90 07 26            953 	mov	dptr,#(_seg_position + 0x0001)
   04F9 E4                  954 	clr	a
   04FA 93                  955 	movc	a,@a+dptr
   04FB F5 A0               956 	mov	_P2,a
                            957 ;	test3.c:74: P0=display_seg[i%100/10] & d1;
   04FD 75 17 64            958 	mov	__modulong_PARM_2,#0x64
   0500 E4                  959 	clr	a
   0501 F5 18               960 	mov	(__modulong_PARM_2 + 1),a
   0503 F5 19               961 	mov	(__modulong_PARM_2 + 2),a
   0505 F5 1A               962 	mov	(__modulong_PARM_2 + 3),a
   0507 85 08 82            963 	mov	dpl,_display_i_1_1
   050A 85 09 83            964 	mov	dph,(_display_i_1_1 + 1)
   050D 85 0A F0            965 	mov	b,(_display_i_1_1 + 2)
   0510 E5 0B               966 	mov	a,(_display_i_1_1 + 3)
   0512 12 05 C0            967 	lcall	__modulong
   0515 AC 82               968 	mov	r4,dpl
   0517 AD 83               969 	mov	r5,dph
   0519 AE F0               970 	mov	r6,b
   051B FF                  971 	mov	r7,a
   051C 75 17 0A            972 	mov	__divulong_PARM_2,#0x0A
   051F E4                  973 	clr	a
   0520 F5 18               974 	mov	(__divulong_PARM_2 + 1),a
   0522 F5 19               975 	mov	(__divulong_PARM_2 + 2),a
   0524 F5 1A               976 	mov	(__divulong_PARM_2 + 3),a
   0526 8C 82               977 	mov	dpl,r4
   0528 8D 83               978 	mov	dph,r5
   052A 8E F0               979 	mov	b,r6
   052C EF                  980 	mov	a,r7
   052D 12 06 43            981 	lcall	__divulong
   0530 AC 82               982 	mov	r4,dpl
   0532 AD 83               983 	mov	r5,dph
   0534 EC                  984 	mov	a,r4
   0535 24 1A               985 	add	a,#_display_seg
   0537 F5 82               986 	mov	dpl,a
   0539 ED                  987 	mov	a,r5
   053A 34 07               988 	addc	a,#(_display_seg >> 8)
   053C F5 83               989 	mov	dph,a
   053E E4                  990 	clr	a
   053F 93                  991 	movc	a,@a+dptr
   0540 FF                  992 	mov	r7,a
   0541 E5 0C               993 	mov	a,_display_d1_1_1
   0543 5F                  994 	anl	a,r7
   0544 F5 80               995 	mov	_P0,a
                            996 ;	test3.c:75: delay(3);
   0546 90 00 03            997 	mov	dptr,#0x0003
   0549 12 00 78            998 	lcall	_delay
                            999 ;	test3.c:76: P0=255;
   054C 75 80 FF           1000 	mov	_P0,#0xFF
                           1001 ;	test3.c:77: P2 = seg_position[0];
   054F 90 07 25           1002 	mov	dptr,#_seg_position
   0552 E4                 1003 	clr	a
   0553 93                 1004 	movc	a,@a+dptr
   0554 F5 A0              1005 	mov	_P2,a
                           1006 ;	test3.c:78: P0=display_seg[i%10] & d0;
   0556 75 17 0A           1007 	mov	__modulong_PARM_2,#0x0A
   0559 E4                 1008 	clr	a
   055A F5 18              1009 	mov	(__modulong_PARM_2 + 1),a
   055C F5 19              1010 	mov	(__modulong_PARM_2 + 2),a
   055E F5 1A              1011 	mov	(__modulong_PARM_2 + 3),a
   0560 85 08 82           1012 	mov	dpl,_display_i_1_1
   0563 85 09 83           1013 	mov	dph,(_display_i_1_1 + 1)
   0566 85 0A F0           1014 	mov	b,(_display_i_1_1 + 2)
   0569 E5 0B              1015 	mov	a,(_display_i_1_1 + 3)
   056B 12 05 C0           1016 	lcall	__modulong
   056E AC 82              1017 	mov	r4,dpl
   0570 AD 83              1018 	mov	r5,dph
   0572 EC                 1019 	mov	a,r4
   0573 24 1A              1020 	add	a,#_display_seg
   0575 F5 82              1021 	mov	dpl,a
   0577 ED                 1022 	mov	a,r5
   0578 34 07              1023 	addc	a,#(_display_seg >> 8)
   057A F5 83              1024 	mov	dph,a
   057C E4                 1025 	clr	a
   057D 93                 1026 	movc	a,@a+dptr
   057E F5 80              1027 	mov	_P0,a
   0580 22                 1028 	ret
                           1029 ;------------------------------------------------------------
                           1030 ;Allocation info for local variables in function 'main'
                           1031 ;------------------------------------------------------------
                           1032 ;	test3.c:85: void main()
                           1033 ;	-----------------------------------------
                           1034 ;	 function main
                           1035 ;	-----------------------------------------
   0581                    1036 _main:
                           1037 ;	test3.c:88: IE=0x8a;
   0581 75 A8 8A           1038 	mov	_IE,#0x8A
                           1039 ;	test3.c:89: TMOD=0x11;
   0584 75 89 11           1040 	mov	_TMOD,#0x11
                           1041 ;	test3.c:90: TH0=(65536-10000)/256;
   0587 75 8C D8           1042 	mov	_TH0,#0xD8
                           1043 ;	test3.c:91: TL0=(65536-10000)%256;
   058A 75 8A F0           1044 	mov	_TL0,#0xF0
                           1045 ;	test3.c:92: TR0=1;
   058D D2 8C              1046 	setb	_TR0
                           1047 ;	test3.c:93: while(1)
   058F                    1048 00102$:
                           1049 ;	test3.c:97: display(i);
   058F 85 13 82           1050 	mov	dpl,_i
   0592 85 14 83           1051 	mov	dph,(_i + 1)
   0595 85 15 F0           1052 	mov	b,(_i + 2)
   0598 E5 16              1053 	mov	a,(_i + 3)
   059A 12 00 A4           1054 	lcall	_display
   059D 80 F0              1055 	sjmp	00102$
                           1056 ;------------------------------------------------------------
                           1057 ;Allocation info for local variables in function 'timer'
                           1058 ;------------------------------------------------------------
                           1059 ;	test3.c:102: void timer() interrupt 1
                           1060 ;	-----------------------------------------
                           1061 ;	 function timer
                           1062 ;	-----------------------------------------
   059F                    1063 _timer:
   059F C0 E0              1064 	push	acc
   05A1 C0 D0              1065 	push	psw
                           1066 ;	test3.c:104: TH0=(65536-10000)/256;
   05A3 75 8C D8           1067 	mov	_TH0,#0xD8
                           1068 ;	test3.c:105: TL0=(65536-10000)%256;
   05A6 75 8A F0           1069 	mov	_TL0,#0xF0
                           1070 ;	test3.c:106: i++;
   05A9 05 13              1071 	inc	_i
   05AB E4                 1072 	clr	a
   05AC B5 13 0C           1073 	cjne	a,_i,00103$
   05AF 05 14              1074 	inc	(_i + 1)
   05B1 B5 14 07           1075 	cjne	a,(_i + 1),00103$
   05B4 05 15              1076 	inc	(_i + 2)
   05B6 B5 15 02           1077 	cjne	a,(_i + 2),00103$
   05B9 05 16              1078 	inc	(_i + 3)
   05BB                    1079 00103$:
   05BB D0 D0              1080 	pop	psw
   05BD D0 E0              1081 	pop	acc
   05BF 32                 1082 	reti
                           1083 ;	eliminated unneeded mov psw,# (no regs used in bank)
                           1084 ;	eliminated unneeded push/pop dpl
                           1085 ;	eliminated unneeded push/pop dph
                           1086 ;	eliminated unneeded push/pop b
                           1087 	.area CSEG    (CODE)
                           1088 	.area CONST   (CODE)
   071A                    1089 _display_seg:
   071A C0                 1090 	.db #0xC0	; 192
   071B F9                 1091 	.db #0xF9	; 249
   071C A4                 1092 	.db #0xA4	; 164
   071D B0                 1093 	.db #0xB0	; 176
   071E 99                 1094 	.db #0x99	; 153
   071F 92                 1095 	.db #0x92	; 146
   0720 82                 1096 	.db #0x82	; 130
   0721 F8                 1097 	.db #0xF8	; 248
   0722 80                 1098 	.db #0x80	; 128
   0723 90                 1099 	.db #0x90	; 144
   0724 7F                 1100 	.db #0x7F	; 127
   0725                    1101 _seg_position:
   0725 FE                 1102 	.db #0xFE	; 254
   0726 FD                 1103 	.db #0xFD	; 253
   0727 FB                 1104 	.db #0xFB	; 251
   0728 F7                 1105 	.db #0xF7	; 247
   0729 F0                 1106 	.db #0xF0	; 240
                           1107 	.area XINIT   (CODE)
                           1108 	.area CABS    (ABS,CODE)
