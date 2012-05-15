                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
                              4 ; This file was generated Sun May 13 19:22:03 2012
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
                            118 	.area RSEG    (DATA)
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
                            143 	.area RSEG    (DATA)
                    0080    144 _P0_0	=	0x0080
                    0081    145 _P0_1	=	0x0081
                    0082    146 _P0_2	=	0x0082
                    0083    147 _P0_3	=	0x0083
                    0084    148 _P0_4	=	0x0084
                    0085    149 _P0_5	=	0x0085
                    0086    150 _P0_6	=	0x0086
                    0087    151 _P0_7	=	0x0087
                    0088    152 _IT0	=	0x0088
                    0089    153 _IE0	=	0x0089
                    008A    154 _IT1	=	0x008a
                    008B    155 _IE1	=	0x008b
                    008C    156 _TR0	=	0x008c
                    008D    157 _TF0	=	0x008d
                    008E    158 _TR1	=	0x008e
                    008F    159 _TF1	=	0x008f
                    0090    160 _P1_0	=	0x0090
                    0091    161 _P1_1	=	0x0091
                    0092    162 _P1_2	=	0x0092
                    0093    163 _P1_3	=	0x0093
                    0094    164 _P1_4	=	0x0094
                    0095    165 _P1_5	=	0x0095
                    0096    166 _P1_6	=	0x0096
                    0097    167 _P1_7	=	0x0097
                    0098    168 _RI	=	0x0098
                    0099    169 _TI	=	0x0099
                    009A    170 _RB8	=	0x009a
                    009B    171 _TB8	=	0x009b
                    009C    172 _REN	=	0x009c
                    009D    173 _SM2	=	0x009d
                    009E    174 _SM1	=	0x009e
                    009F    175 _SM0	=	0x009f
                    00A0    176 _P2_0	=	0x00a0
                    00A1    177 _P2_1	=	0x00a1
                    00A2    178 _P2_2	=	0x00a2
                    00A3    179 _P2_3	=	0x00a3
                    00A4    180 _P2_4	=	0x00a4
                    00A5    181 _P2_5	=	0x00a5
                    00A6    182 _P2_6	=	0x00a6
                    00A7    183 _P2_7	=	0x00a7
                    00A8    184 _EX0	=	0x00a8
                    00A9    185 _ET0	=	0x00a9
                    00AA    186 _EX1	=	0x00aa
                    00AB    187 _ET1	=	0x00ab
                    00AC    188 _ES	=	0x00ac
                    00AF    189 _EA	=	0x00af
                    00B0    190 _P3_0	=	0x00b0
                    00B1    191 _P3_1	=	0x00b1
                    00B2    192 _P3_2	=	0x00b2
                    00B3    193 _P3_3	=	0x00b3
                    00B4    194 _P3_4	=	0x00b4
                    00B5    195 _P3_5	=	0x00b5
                    00B6    196 _P3_6	=	0x00b6
                    00B7    197 _P3_7	=	0x00b7
                    00B0    198 _RXD	=	0x00b0
                    00B1    199 _TXD	=	0x00b1
                    00B2    200 _INT0	=	0x00b2
                    00B3    201 _INT1	=	0x00b3
                    00B4    202 _T0	=	0x00b4
                    00B5    203 _T1	=	0x00b5
                    00B6    204 _WR	=	0x00b6
                    00B7    205 _RD	=	0x00b7
                    00B8    206 _PX0	=	0x00b8
                    00B9    207 _PT0	=	0x00b9
                    00BA    208 _PX1	=	0x00ba
                    00BB    209 _PT1	=	0x00bb
                    00BC    210 _PS	=	0x00bc
                    00D0    211 _P	=	0x00d0
                    00D1    212 _F1	=	0x00d1
                    00D2    213 _OV	=	0x00d2
                    00D3    214 _RS0	=	0x00d3
                    00D4    215 _RS1	=	0x00d4
                    00D5    216 _F0	=	0x00d5
                    00D6    217 _AC	=	0x00d6
                    00D7    218 _CY	=	0x00d7
                            219 ;--------------------------------------------------------
                            220 ; overlayable register banks
                            221 ;--------------------------------------------------------
                            222 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     223 	.ds 8
                            224 ;--------------------------------------------------------
                            225 ; internal ram data
                            226 ;--------------------------------------------------------
                            227 	.area DSEG    (DATA)
   0008                     228 _display_i_1_1:
   0008                     229 	.ds 4
   000C                     230 _display_d1_1_1:
   000C                     231 	.ds 1
   000D                     232 _display_d2_1_1:
   000D                     233 	.ds 1
   000E                     234 _display_d3_1_1:
   000E                     235 	.ds 1
   000F                     236 _display_sloc0_1_0:
   000F                     237 	.ds 4
   0013                     238 _i::
   0013                     239 	.ds 4
                            240 ;--------------------------------------------------------
                            241 ; overlayable items in internal ram 
                            242 ;--------------------------------------------------------
                            243 	.area	OSEG    (OVR,DATA)
                            244 ;--------------------------------------------------------
                            245 ; Stack segment in internal ram 
                            246 ;--------------------------------------------------------
                            247 	.area	SSEG	(DATA)
   001B                     248 __start__stack:
   001B                     249 	.ds	1
                            250 
                            251 ;--------------------------------------------------------
                            252 ; indirectly addressable internal ram data
                            253 ;--------------------------------------------------------
                            254 	.area ISEG    (DATA)
                            255 ;--------------------------------------------------------
                            256 ; absolute internal ram data
                            257 ;--------------------------------------------------------
                            258 	.area IABS    (ABS,DATA)
                            259 	.area IABS    (ABS,DATA)
                            260 ;--------------------------------------------------------
                            261 ; bit data
                            262 ;--------------------------------------------------------
                            263 	.area BSEG    (BIT)
                            264 ;--------------------------------------------------------
                            265 ; paged external ram data
                            266 ;--------------------------------------------------------
                            267 	.area PSEG    (PAG,XDATA)
                            268 ;--------------------------------------------------------
                            269 ; external ram data
                            270 ;--------------------------------------------------------
                            271 	.area XSEG    (XDATA)
                            272 ;--------------------------------------------------------
                            273 ; absolute external ram data
                            274 ;--------------------------------------------------------
                            275 	.area XABS    (ABS,XDATA)
                            276 ;--------------------------------------------------------
                            277 ; external initialized ram data
                            278 ;--------------------------------------------------------
                            279 	.area XISEG   (XDATA)
                            280 	.area HOME    (CODE)
                            281 	.area GSINIT0 (CODE)
                            282 	.area GSINIT1 (CODE)
                            283 	.area GSINIT2 (CODE)
                            284 	.area GSINIT3 (CODE)
                            285 	.area GSINIT4 (CODE)
                            286 	.area GSINIT5 (CODE)
                            287 	.area GSINIT  (CODE)
                            288 	.area GSFINAL (CODE)
                            289 	.area CSEG    (CODE)
                            290 ;--------------------------------------------------------
                            291 ; interrupt vector 
                            292 ;--------------------------------------------------------
                            293 	.area HOME    (CODE)
   0000                     294 __interrupt_vect:
   0000 02 00 13            295 	ljmp	__sdcc_gsinit_startup
   0003 32                  296 	reti
   0004                     297 	.ds	7
   000B 02 05 94            298 	ljmp	_timer
                            299 ;--------------------------------------------------------
                            300 ; global & static initialisations
                            301 ;--------------------------------------------------------
                            302 	.area HOME    (CODE)
                            303 	.area GSINIT  (CODE)
                            304 	.area GSFINAL (CODE)
                            305 	.area GSINIT  (CODE)
                            306 	.globl __sdcc_gsinit_startup
                            307 	.globl __sdcc_program_startup
                            308 	.globl __start__stack
                            309 	.globl __mcs51_genXINIT
                            310 	.globl __mcs51_genXRAMCLEAR
                            311 	.globl __mcs51_genRAMCLEAR
                            312 ;	test3.c:84: unsigned long int i=0;
   006C E4                  313 	clr	a
   006D F5 13               314 	mov	_i,a
   006F F5 14               315 	mov	(_i + 1),a
   0071 F5 15               316 	mov	(_i + 2),a
   0073 F5 16               317 	mov	(_i + 3),a
                            318 	.area GSFINAL (CODE)
   0075 02 00 0E            319 	ljmp	__sdcc_program_startup
                            320 ;--------------------------------------------------------
                            321 ; Home
                            322 ;--------------------------------------------------------
                            323 	.area HOME    (CODE)
                            324 	.area HOME    (CODE)
   000E                     325 __sdcc_program_startup:
   000E 12 05 76            326 	lcall	_main
                            327 ;	return from main will lock up
   0011 80 FE               328 	sjmp .
                            329 ;--------------------------------------------------------
                            330 ; code
                            331 ;--------------------------------------------------------
                            332 	.area CSEG    (CODE)
                            333 ;------------------------------------------------------------
                            334 ;Allocation info for local variables in function 'delay'
                            335 ;------------------------------------------------------------
                            336 ;i                         Allocated to registers r2 r3 
                            337 ;a                         Allocated to registers r4 r5 
                            338 ;b                         Allocated to registers r6 r7 
                            339 ;------------------------------------------------------------
                            340 ;	test3.c:6: void delay(int i)
                            341 ;	-----------------------------------------
                            342 ;	 function delay
                            343 ;	-----------------------------------------
   0078                     344 _delay:
                    0002    345 	ar2 = 0x02
                    0003    346 	ar3 = 0x03
                    0004    347 	ar4 = 0x04
                    0005    348 	ar5 = 0x05
                    0006    349 	ar6 = 0x06
                    0007    350 	ar7 = 0x07
                    0000    351 	ar0 = 0x00
                    0001    352 	ar1 = 0x01
   0078 AA 82               353 	mov	r2,dpl
   007A AB 83               354 	mov	r3,dph
                            355 ;	test3.c:9: for(a=0;a<i;a++)
   007C 7C 00               356 	mov	r4,#0x00
   007E 7D 00               357 	mov	r5,#0x00
   0080                     358 00104$:
   0080 C3                  359 	clr	c
   0081 EC                  360 	mov	a,r4
   0082 9A                  361 	subb	a,r2
   0083 ED                  362 	mov	a,r5
   0084 64 80               363 	xrl	a,#0x80
   0086 8B F0               364 	mov	b,r3
   0088 63 F0 80            365 	xrl	b,#0x80
   008B 95 F0               366 	subb	a,b
   008D 50 14               367 	jnc	00108$
                            368 ;	test3.c:11: for(b=0;b<120;b++);
   008F 7E 78               369 	mov	r6,#0x78
   0091 7F 00               370 	mov	r7,#0x00
   0093                     371 00103$:
   0093 1E                  372 	dec	r6
   0094 BE FF 01            373 	cjne	r6,#0xff,00117$
   0097 1F                  374 	dec	r7
   0098                     375 00117$:
   0098 EE                  376 	mov	a,r6
   0099 4F                  377 	orl	a,r7
   009A 70 F7               378 	jnz	00103$
                            379 ;	test3.c:9: for(a=0;a<i;a++)
   009C 0C                  380 	inc	r4
   009D BC 00 E0            381 	cjne	r4,#0x00,00104$
   00A0 0D                  382 	inc	r5
   00A1 80 DD               383 	sjmp	00104$
   00A3                     384 00108$:
   00A3 22                  385 	ret
                            386 ;------------------------------------------------------------
                            387 ;Allocation info for local variables in function 'display'
                            388 ;------------------------------------------------------------
                            389 ;i                         Allocated with name '_display_i_1_1'
                            390 ;d0                        Allocated to registers 
                            391 ;d1                        Allocated with name '_display_d1_1_1'
                            392 ;d2                        Allocated with name '_display_d2_1_1'
                            393 ;d3                        Allocated with name '_display_d3_1_1'
                            394 ;sloc0                     Allocated with name '_display_sloc0_1_0'
                            395 ;------------------------------------------------------------
                            396 ;	test3.c:21: void display(unsigned long int i)
                            397 ;	-----------------------------------------
                            398 ;	 function display
                            399 ;	-----------------------------------------
   00A4                     400 _display:
   00A4 85 82 08            401 	mov	_display_i_1_1,dpl
   00A7 85 83 09            402 	mov	(_display_i_1_1 + 1),dph
   00AA 85 F0 0A            403 	mov	(_display_i_1_1 + 2),b
   00AD F5 0B               404 	mov	(_display_i_1_1 + 3),a
                            405 ;	test3.c:25: unsigned char d1=0xFF;
   00AF 75 0C FF            406 	mov	_display_d1_1_1,#0xFF
                            407 ;	test3.c:26: unsigned char d2=0x7F;
   00B2 75 0D 7F            408 	mov	_display_d2_1_1,#0x7F
                            409 ;	test3.c:27: unsigned char d3=0xFF;
   00B5 75 0E FF            410 	mov	_display_d3_1_1,#0xFF
                            411 ;	test3.c:29: if(i/600>=60*100)
   00B8 75 17 58            412 	mov	__divulong_PARM_2,#0x58
   00BB 75 18 02            413 	mov	(__divulong_PARM_2 + 1),#0x02
   00BE 75 19 00            414 	mov	(__divulong_PARM_2 + 2),#0x00
   00C1 75 1A 00            415 	mov	(__divulong_PARM_2 + 3),#0x00
   00C4 85 08 82            416 	mov	dpl,_display_i_1_1
   00C7 85 09 83            417 	mov	dph,(_display_i_1_1 + 1)
   00CA 85 0A F0            418 	mov	b,(_display_i_1_1 + 2)
   00CD E5 0B               419 	mov	a,(_display_i_1_1 + 3)
   00CF 12 06 3B            420 	lcall	__divulong
   00D2 A9 82               421 	mov	r1,dpl
   00D4 A8 83               422 	mov	r0,dph
   00D6 AF F0               423 	mov	r7,b
   00D8 FE                  424 	mov	r6,a
   00D9 C3                  425 	clr	c
   00DA E9                  426 	mov	a,r1
   00DB 94 70               427 	subb	a,#0x70
   00DD E8                  428 	mov	a,r0
   00DE 94 17               429 	subb	a,#0x17
   00E0 EF                  430 	mov	a,r7
   00E1 94 00               431 	subb	a,#0x00
   00E3 EE                  432 	mov	a,r6
   00E4 94 00               433 	subb	a,#0x00
   00E6 50 03               434 	jnc	00118$
   00E8 02 01 A5            435 	ljmp	00110$
   00EB                     436 00118$:
                            437 ;	test3.c:31: i=i/60/60/100*1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/6000*10000;
   00EB 75 17 00            438 	mov	__divulong_PARM_2,#0x00
   00EE 75 18 97            439 	mov	(__divulong_PARM_2 + 1),#0x97
   00F1 75 19 FF            440 	mov	(__divulong_PARM_2 + 2),#0xFF
   00F4 75 1A FF            441 	mov	(__divulong_PARM_2 + 3),#0xFF
   00F7 85 08 82            442 	mov	dpl,_display_i_1_1
   00FA 85 09 83            443 	mov	dph,(_display_i_1_1 + 1)
   00FD 85 0A F0            444 	mov	b,(_display_i_1_1 + 2)
   0100 E5 0B               445 	mov	a,(_display_i_1_1 + 3)
   0102 12 06 3B            446 	lcall	__divulong
   0105 85 82 17            447 	mov	__mullong_PARM_2,dpl
   0108 85 83 18            448 	mov	(__mullong_PARM_2 + 1),dph
   010B 85 F0 19            449 	mov	(__mullong_PARM_2 + 2),b
   010E F5 1A               450 	mov	(__mullong_PARM_2 + 3),a
   0110 90 42 40            451 	mov	dptr,#0x4240
   0113 75 F0 0F            452 	mov	b,#0x0F
   0116 E4                  453 	clr	a
   0117 12 06 A0            454 	lcall	__mullong
   011A 85 82 0F            455 	mov	_display_sloc0_1_0,dpl
   011D 85 83 10            456 	mov	(_display_sloc0_1_0 + 1),dph
   0120 85 F0 11            457 	mov	(_display_sloc0_1_0 + 2),b
   0123 F5 12               458 	mov	(_display_sloc0_1_0 + 3),a
   0125 75 17 40            459 	mov	__modulong_PARM_2,#0x40
   0128 75 18 7E            460 	mov	(__modulong_PARM_2 + 1),#0x7E
   012B 75 19 05            461 	mov	(__modulong_PARM_2 + 2),#0x05
   012E 75 1A 00            462 	mov	(__modulong_PARM_2 + 3),#0x00
   0131 85 08 82            463 	mov	dpl,_display_i_1_1
   0134 85 09 83            464 	mov	dph,(_display_i_1_1 + 1)
   0137 85 0A F0            465 	mov	b,(_display_i_1_1 + 2)
   013A E5 0B               466 	mov	a,(_display_i_1_1 + 3)
   013C 12 05 B8            467 	lcall	__modulong
   013F 75 17 70            468 	mov	__divulong_PARM_2,#0x70
   0142 75 18 17            469 	mov	(__divulong_PARM_2 + 1),#0x17
   0145 75 19 00            470 	mov	(__divulong_PARM_2 + 2),#0x00
   0148 75 1A 00            471 	mov	(__divulong_PARM_2 + 3),#0x00
   014B 12 06 3B            472 	lcall	__divulong
   014E 85 82 17            473 	mov	__mullong_PARM_2,dpl
   0151 85 83 18            474 	mov	(__mullong_PARM_2 + 1),dph
   0154 85 F0 19            475 	mov	(__mullong_PARM_2 + 2),b
   0157 F5 1A               476 	mov	(__mullong_PARM_2 + 3),a
   0159 90 27 10            477 	mov	dptr,#0x2710
   015C E4                  478 	clr	a
   015D F5 F0               479 	mov	b,a
   015F 12 06 A0            480 	lcall	__mullong
   0162 AE 82               481 	mov	r6,dpl
   0164 AF 83               482 	mov	r7,dph
   0166 A8 F0               483 	mov	r0,b
   0168 F9                  484 	mov	r1,a
   0169 EE                  485 	mov	a,r6
   016A 25 0F               486 	add	a,_display_sloc0_1_0
   016C F5 08               487 	mov	_display_i_1_1,a
   016E EF                  488 	mov	a,r7
   016F 35 10               489 	addc	a,(_display_sloc0_1_0 + 1)
   0171 F5 09               490 	mov	(_display_i_1_1 + 1),a
   0173 E8                  491 	mov	a,r0
   0174 35 11               492 	addc	a,(_display_sloc0_1_0 + 2)
   0176 F5 0A               493 	mov	(_display_i_1_1 + 2),a
   0178 E9                  494 	mov	a,r1
   0179 35 12               495 	addc	a,(_display_sloc0_1_0 + 3)
   017B F5 0B               496 	mov	(_display_i_1_1 + 3),a
                            497 ;	test3.c:32: i/=10000;
   017D 75 17 10            498 	mov	__divulong_PARM_2,#0x10
   0180 75 18 27            499 	mov	(__divulong_PARM_2 + 1),#0x27
   0183 75 19 00            500 	mov	(__divulong_PARM_2 + 2),#0x00
   0186 75 1A 00            501 	mov	(__divulong_PARM_2 + 3),#0x00
   0189 85 08 82            502 	mov	dpl,_display_i_1_1
   018C 85 09 83            503 	mov	dph,(_display_i_1_1 + 1)
   018F 85 0A F0            504 	mov	b,(_display_i_1_1 + 2)
   0192 E5 0B               505 	mov	a,(_display_i_1_1 + 3)
   0194 12 06 3B            506 	lcall	__divulong
   0197 85 82 08            507 	mov	_display_i_1_1,dpl
   019A 85 83 09            508 	mov	(_display_i_1_1 + 1),dph
   019D 85 F0 0A            509 	mov	(_display_i_1_1 + 2),b
   01A0 F5 0B               510 	mov	(_display_i_1_1 + 3),a
   01A2 02 04 48            511 	ljmp	00111$
   01A5                     512 00110$:
                            513 ;	test3.c:37: else if(i/60>=60*100)
   01A5 75 17 3C            514 	mov	__divulong_PARM_2,#0x3C
   01A8 E4                  515 	clr	a
   01A9 F5 18               516 	mov	(__divulong_PARM_2 + 1),a
   01AB F5 19               517 	mov	(__divulong_PARM_2 + 2),a
   01AD F5 1A               518 	mov	(__divulong_PARM_2 + 3),a
   01AF 85 08 82            519 	mov	dpl,_display_i_1_1
   01B2 85 09 83            520 	mov	dph,(_display_i_1_1 + 1)
   01B5 85 0A F0            521 	mov	b,(_display_i_1_1 + 2)
   01B8 E5 0B               522 	mov	a,(_display_i_1_1 + 3)
   01BA 12 06 3B            523 	lcall	__divulong
   01BD AE 82               524 	mov	r6,dpl
   01BF AF 83               525 	mov	r7,dph
   01C1 A8 F0               526 	mov	r0,b
   01C3 F9                  527 	mov	r1,a
   01C4 C3                  528 	clr	c
   01C5 EE                  529 	mov	a,r6
   01C6 94 70               530 	subb	a,#0x70
   01C8 EF                  531 	mov	a,r7
   01C9 94 17               532 	subb	a,#0x17
   01CB E8                  533 	mov	a,r0
   01CC 94 00               534 	subb	a,#0x00
   01CE E9                  535 	mov	a,r1
   01CF 94 00               536 	subb	a,#0x00
   01D1 50 03               537 	jnc	00119$
   01D3 02 02 DE            538 	ljmp	00107$
   01D6                     539 00119$:
                            540 ;	test3.c:39: i=i/60/60/100*1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/6000*10000 + i%6000;
   01D6 75 17 00            541 	mov	__divulong_PARM_2,#0x00
   01D9 75 18 97            542 	mov	(__divulong_PARM_2 + 1),#0x97
   01DC 75 19 FF            543 	mov	(__divulong_PARM_2 + 2),#0xFF
   01DF 75 1A FF            544 	mov	(__divulong_PARM_2 + 3),#0xFF
   01E2 85 08 82            545 	mov	dpl,_display_i_1_1
   01E5 85 09 83            546 	mov	dph,(_display_i_1_1 + 1)
   01E8 85 0A F0            547 	mov	b,(_display_i_1_1 + 2)
   01EB E5 0B               548 	mov	a,(_display_i_1_1 + 3)
   01ED 12 06 3B            549 	lcall	__divulong
   01F0 85 82 17            550 	mov	__mullong_PARM_2,dpl
   01F3 85 83 18            551 	mov	(__mullong_PARM_2 + 1),dph
   01F6 85 F0 19            552 	mov	(__mullong_PARM_2 + 2),b
   01F9 F5 1A               553 	mov	(__mullong_PARM_2 + 3),a
   01FB 90 42 40            554 	mov	dptr,#0x4240
   01FE 75 F0 0F            555 	mov	b,#0x0F
   0201 E4                  556 	clr	a
   0202 12 06 A0            557 	lcall	__mullong
   0205 AE 82               558 	mov	r6,dpl
   0207 AF 83               559 	mov	r7,dph
   0209 A8 F0               560 	mov	r0,b
   020B F9                  561 	mov	r1,a
   020C 75 17 40            562 	mov	__modulong_PARM_2,#0x40
   020F 75 18 7E            563 	mov	(__modulong_PARM_2 + 1),#0x7E
   0212 75 19 05            564 	mov	(__modulong_PARM_2 + 2),#0x05
   0215 75 1A 00            565 	mov	(__modulong_PARM_2 + 3),#0x00
   0218 85 08 82            566 	mov	dpl,_display_i_1_1
   021B 85 09 83            567 	mov	dph,(_display_i_1_1 + 1)
   021E 85 0A F0            568 	mov	b,(_display_i_1_1 + 2)
   0221 E5 0B               569 	mov	a,(_display_i_1_1 + 3)
   0223 C0 06               570 	push	ar6
   0225 C0 07               571 	push	ar7
   0227 C0 00               572 	push	ar0
   0229 C0 01               573 	push	ar1
   022B 12 05 B8            574 	lcall	__modulong
   022E 75 17 70            575 	mov	__divulong_PARM_2,#0x70
   0231 75 18 17            576 	mov	(__divulong_PARM_2 + 1),#0x17
   0234 75 19 00            577 	mov	(__divulong_PARM_2 + 2),#0x00
   0237 75 1A 00            578 	mov	(__divulong_PARM_2 + 3),#0x00
   023A 12 06 3B            579 	lcall	__divulong
   023D 85 82 17            580 	mov	__mullong_PARM_2,dpl
   0240 85 83 18            581 	mov	(__mullong_PARM_2 + 1),dph
   0243 85 F0 19            582 	mov	(__mullong_PARM_2 + 2),b
   0246 F5 1A               583 	mov	(__mullong_PARM_2 + 3),a
   0248 90 27 10            584 	mov	dptr,#0x2710
   024B E4                  585 	clr	a
   024C F5 F0               586 	mov	b,a
   024E 12 06 A0            587 	lcall	__mullong
   0251 AA 82               588 	mov	r2,dpl
   0253 AB 83               589 	mov	r3,dph
   0255 AC F0               590 	mov	r4,b
   0257 FD                  591 	mov	r5,a
   0258 D0 01               592 	pop	ar1
   025A D0 00               593 	pop	ar0
   025C D0 07               594 	pop	ar7
   025E D0 06               595 	pop	ar6
   0260 EA                  596 	mov	a,r2
   0261 2E                  597 	add	a,r6
   0262 FE                  598 	mov	r6,a
   0263 EB                  599 	mov	a,r3
   0264 3F                  600 	addc	a,r7
   0265 FF                  601 	mov	r7,a
   0266 EC                  602 	mov	a,r4
   0267 38                  603 	addc	a,r0
   0268 F8                  604 	mov	r0,a
   0269 ED                  605 	mov	a,r5
   026A 39                  606 	addc	a,r1
   026B F9                  607 	mov	r1,a
   026C 75 17 70            608 	mov	__modulong_PARM_2,#0x70
   026F 75 18 17            609 	mov	(__modulong_PARM_2 + 1),#0x17
   0272 75 19 00            610 	mov	(__modulong_PARM_2 + 2),#0x00
   0275 75 1A 00            611 	mov	(__modulong_PARM_2 + 3),#0x00
   0278 85 08 82            612 	mov	dpl,_display_i_1_1
   027B 85 09 83            613 	mov	dph,(_display_i_1_1 + 1)
   027E 85 0A F0            614 	mov	b,(_display_i_1_1 + 2)
   0281 E5 0B               615 	mov	a,(_display_i_1_1 + 3)
   0283 C0 06               616 	push	ar6
   0285 C0 07               617 	push	ar7
   0287 C0 00               618 	push	ar0
   0289 C0 01               619 	push	ar1
   028B 12 05 B8            620 	lcall	__modulong
   028E AA 82               621 	mov	r2,dpl
   0290 AB 83               622 	mov	r3,dph
   0292 AC F0               623 	mov	r4,b
   0294 FD                  624 	mov	r5,a
   0295 D0 01               625 	pop	ar1
   0297 D0 00               626 	pop	ar0
   0299 D0 07               627 	pop	ar7
   029B D0 06               628 	pop	ar6
   029D EA                  629 	mov	a,r2
   029E 2E                  630 	add	a,r6
   029F F5 08               631 	mov	_display_i_1_1,a
   02A1 EB                  632 	mov	a,r3
   02A2 3F                  633 	addc	a,r7
   02A3 F5 09               634 	mov	(_display_i_1_1 + 1),a
   02A5 EC                  635 	mov	a,r4
   02A6 38                  636 	addc	a,r0
   02A7 F5 0A               637 	mov	(_display_i_1_1 + 2),a
   02A9 ED                  638 	mov	a,r5
   02AA 39                  639 	addc	a,r1
   02AB F5 0B               640 	mov	(_display_i_1_1 + 3),a
                            641 ;	test3.c:40: i/=1000;
   02AD 75 17 E8            642 	mov	__divulong_PARM_2,#0xE8
   02B0 75 18 03            643 	mov	(__divulong_PARM_2 + 1),#0x03
   02B3 75 19 00            644 	mov	(__divulong_PARM_2 + 2),#0x00
   02B6 75 1A 00            645 	mov	(__divulong_PARM_2 + 3),#0x00
   02B9 85 08 82            646 	mov	dpl,_display_i_1_1
   02BC 85 09 83            647 	mov	dph,(_display_i_1_1 + 1)
   02BF 85 0A F0            648 	mov	b,(_display_i_1_1 + 2)
   02C2 E5 0B               649 	mov	a,(_display_i_1_1 + 3)
   02C4 12 06 3B            650 	lcall	__divulong
   02C7 85 82 08            651 	mov	_display_i_1_1,dpl
   02CA 85 83 09            652 	mov	(_display_i_1_1 + 1),dph
   02CD 85 F0 0A            653 	mov	(_display_i_1_1 + 2),b
   02D0 F5 0B               654 	mov	(_display_i_1_1 + 3),a
                            655 ;	test3.c:41: d2=0xFF;
   02D2 75 0D FF            656 	mov	_display_d2_1_1,#0xFF
                            657 ;	test3.c:42: d1=0x7F;
   02D5 75 0C 7F            658 	mov	_display_d1_1_1,#0x7F
                            659 ;	test3.c:43: d3=0x7F;
   02D8 75 0E 7F            660 	mov	_display_d3_1_1,#0x7F
   02DB 02 04 48            661 	ljmp	00111$
   02DE                     662 00107$:
                            663 ;	test3.c:45: else if(i>=60000)
   02DE C3                  664 	clr	c
   02DF E5 08               665 	mov	a,_display_i_1_1
   02E1 94 60               666 	subb	a,#0x60
   02E3 E5 09               667 	mov	a,(_display_i_1_1 + 1)
   02E5 94 EA               668 	subb	a,#0xEA
   02E7 E5 0A               669 	mov	a,(_display_i_1_1 + 2)
   02E9 94 00               670 	subb	a,#0x00
   02EB E5 0B               671 	mov	a,(_display_i_1_1 + 3)
   02ED 94 00               672 	subb	a,#0x00
   02EF 50 03               673 	jnc	00120$
   02F1 02 03 90            674 	ljmp	00104$
   02F4                     675 00120$:
                            676 ;	test3.c:47: i=i/6000*10000+i%6000;
   02F4 75 17 70            677 	mov	__divulong_PARM_2,#0x70
   02F7 75 18 17            678 	mov	(__divulong_PARM_2 + 1),#0x17
   02FA 75 19 00            679 	mov	(__divulong_PARM_2 + 2),#0x00
   02FD 75 1A 00            680 	mov	(__divulong_PARM_2 + 3),#0x00
   0300 85 08 82            681 	mov	dpl,_display_i_1_1
   0303 85 09 83            682 	mov	dph,(_display_i_1_1 + 1)
   0306 85 0A F0            683 	mov	b,(_display_i_1_1 + 2)
   0309 E5 0B               684 	mov	a,(_display_i_1_1 + 3)
   030B 12 06 3B            685 	lcall	__divulong
   030E 85 82 17            686 	mov	__mullong_PARM_2,dpl
   0311 85 83 18            687 	mov	(__mullong_PARM_2 + 1),dph
   0314 85 F0 19            688 	mov	(__mullong_PARM_2 + 2),b
   0317 F5 1A               689 	mov	(__mullong_PARM_2 + 3),a
   0319 90 27 10            690 	mov	dptr,#0x2710
   031C E4                  691 	clr	a
   031D F5 F0               692 	mov	b,a
   031F 12 06 A0            693 	lcall	__mullong
   0322 AA 82               694 	mov	r2,dpl
   0324 AB 83               695 	mov	r3,dph
   0326 AC F0               696 	mov	r4,b
   0328 FD                  697 	mov	r5,a
   0329 75 17 70            698 	mov	__modulong_PARM_2,#0x70
   032C 75 18 17            699 	mov	(__modulong_PARM_2 + 1),#0x17
   032F 75 19 00            700 	mov	(__modulong_PARM_2 + 2),#0x00
   0332 75 1A 00            701 	mov	(__modulong_PARM_2 + 3),#0x00
   0335 85 08 82            702 	mov	dpl,_display_i_1_1
   0338 85 09 83            703 	mov	dph,(_display_i_1_1 + 1)
   033B 85 0A F0            704 	mov	b,(_display_i_1_1 + 2)
   033E E5 0B               705 	mov	a,(_display_i_1_1 + 3)
   0340 C0 02               706 	push	ar2
   0342 C0 03               707 	push	ar3
   0344 C0 04               708 	push	ar4
   0346 C0 05               709 	push	ar5
   0348 12 05 B8            710 	lcall	__modulong
   034B AE 82               711 	mov	r6,dpl
   034D AF 83               712 	mov	r7,dph
   034F A8 F0               713 	mov	r0,b
   0351 F9                  714 	mov	r1,a
   0352 D0 05               715 	pop	ar5
   0354 D0 04               716 	pop	ar4
   0356 D0 03               717 	pop	ar3
   0358 D0 02               718 	pop	ar2
   035A EE                  719 	mov	a,r6
   035B 2A                  720 	add	a,r2
   035C F5 08               721 	mov	_display_i_1_1,a
   035E EF                  722 	mov	a,r7
   035F 3B                  723 	addc	a,r3
   0360 F5 09               724 	mov	(_display_i_1_1 + 1),a
   0362 E8                  725 	mov	a,r0
   0363 3C                  726 	addc	a,r4
   0364 F5 0A               727 	mov	(_display_i_1_1 + 2),a
   0366 E9                  728 	mov	a,r1
   0367 3D                  729 	addc	a,r5
   0368 F5 0B               730 	mov	(_display_i_1_1 + 3),a
                            731 ;	test3.c:48: i/=100;
   036A 75 17 64            732 	mov	__divulong_PARM_2,#0x64
   036D E4                  733 	clr	a
   036E F5 18               734 	mov	(__divulong_PARM_2 + 1),a
   0370 F5 19               735 	mov	(__divulong_PARM_2 + 2),a
   0372 F5 1A               736 	mov	(__divulong_PARM_2 + 3),a
   0374 85 08 82            737 	mov	dpl,_display_i_1_1
   0377 85 09 83            738 	mov	dph,(_display_i_1_1 + 1)
   037A 85 0A F0            739 	mov	b,(_display_i_1_1 + 2)
   037D E5 0B               740 	mov	a,(_display_i_1_1 + 3)
   037F 12 06 3B            741 	lcall	__divulong
   0382 85 82 08            742 	mov	_display_i_1_1,dpl
   0385 85 83 09            743 	mov	(_display_i_1_1 + 1),dph
   0388 85 F0 0A            744 	mov	(_display_i_1_1 + 2),b
   038B F5 0B               745 	mov	(_display_i_1_1 + 3),a
   038D 02 04 48            746 	ljmp	00111$
   0390                     747 00104$:
                            748 ;	test3.c:53: else if(i>=6000)
   0390 C3                  749 	clr	c
   0391 E5 08               750 	mov	a,_display_i_1_1
   0393 94 70               751 	subb	a,#0x70
   0395 E5 09               752 	mov	a,(_display_i_1_1 + 1)
   0397 94 17               753 	subb	a,#0x17
   0399 E5 0A               754 	mov	a,(_display_i_1_1 + 2)
   039B 94 00               755 	subb	a,#0x00
   039D E5 0B               756 	mov	a,(_display_i_1_1 + 3)
   039F 94 00               757 	subb	a,#0x00
   03A1 50 03               758 	jnc	00121$
   03A3 02 04 48            759 	ljmp	00111$
   03A6                     760 00121$:
                            761 ;	test3.c:56: i=i/6000*10000+i%6000;
   03A6 75 17 70            762 	mov	__divulong_PARM_2,#0x70
   03A9 75 18 17            763 	mov	(__divulong_PARM_2 + 1),#0x17
   03AC 75 19 00            764 	mov	(__divulong_PARM_2 + 2),#0x00
   03AF 75 1A 00            765 	mov	(__divulong_PARM_2 + 3),#0x00
   03B2 85 08 82            766 	mov	dpl,_display_i_1_1
   03B5 85 09 83            767 	mov	dph,(_display_i_1_1 + 1)
   03B8 85 0A F0            768 	mov	b,(_display_i_1_1 + 2)
   03BB E5 0B               769 	mov	a,(_display_i_1_1 + 3)
   03BD 12 06 3B            770 	lcall	__divulong
   03C0 85 82 17            771 	mov	__mullong_PARM_2,dpl
   03C3 85 83 18            772 	mov	(__mullong_PARM_2 + 1),dph
   03C6 85 F0 19            773 	mov	(__mullong_PARM_2 + 2),b
   03C9 F5 1A               774 	mov	(__mullong_PARM_2 + 3),a
   03CB 90 27 10            775 	mov	dptr,#0x2710
   03CE E4                  776 	clr	a
   03CF F5 F0               777 	mov	b,a
   03D1 12 06 A0            778 	lcall	__mullong
   03D4 AA 82               779 	mov	r2,dpl
   03D6 AB 83               780 	mov	r3,dph
   03D8 AC F0               781 	mov	r4,b
   03DA FD                  782 	mov	r5,a
   03DB 75 17 70            783 	mov	__modulong_PARM_2,#0x70
   03DE 75 18 17            784 	mov	(__modulong_PARM_2 + 1),#0x17
   03E1 75 19 00            785 	mov	(__modulong_PARM_2 + 2),#0x00
   03E4 75 1A 00            786 	mov	(__modulong_PARM_2 + 3),#0x00
   03E7 85 08 82            787 	mov	dpl,_display_i_1_1
   03EA 85 09 83            788 	mov	dph,(_display_i_1_1 + 1)
   03ED 85 0A F0            789 	mov	b,(_display_i_1_1 + 2)
   03F0 E5 0B               790 	mov	a,(_display_i_1_1 + 3)
   03F2 C0 02               791 	push	ar2
   03F4 C0 03               792 	push	ar3
   03F6 C0 04               793 	push	ar4
   03F8 C0 05               794 	push	ar5
   03FA 12 05 B8            795 	lcall	__modulong
   03FD AE 82               796 	mov	r6,dpl
   03FF AF 83               797 	mov	r7,dph
   0401 A8 F0               798 	mov	r0,b
   0403 F9                  799 	mov	r1,a
   0404 D0 05               800 	pop	ar5
   0406 D0 04               801 	pop	ar4
   0408 D0 03               802 	pop	ar3
   040A D0 02               803 	pop	ar2
   040C EE                  804 	mov	a,r6
   040D 2A                  805 	add	a,r2
   040E F5 08               806 	mov	_display_i_1_1,a
   0410 EF                  807 	mov	a,r7
   0411 3B                  808 	addc	a,r3
   0412 F5 09               809 	mov	(_display_i_1_1 + 1),a
   0414 E8                  810 	mov	a,r0
   0415 3C                  811 	addc	a,r4
   0416 F5 0A               812 	mov	(_display_i_1_1 + 2),a
   0418 E9                  813 	mov	a,r1
   0419 3D                  814 	addc	a,r5
   041A F5 0B               815 	mov	(_display_i_1_1 + 3),a
                            816 ;	test3.c:57: i/=10;
   041C 75 17 0A            817 	mov	__divulong_PARM_2,#0x0A
   041F E4                  818 	clr	a
   0420 F5 18               819 	mov	(__divulong_PARM_2 + 1),a
   0422 F5 19               820 	mov	(__divulong_PARM_2 + 2),a
   0424 F5 1A               821 	mov	(__divulong_PARM_2 + 3),a
   0426 85 08 82            822 	mov	dpl,_display_i_1_1
   0429 85 09 83            823 	mov	dph,(_display_i_1_1 + 1)
   042C 85 0A F0            824 	mov	b,(_display_i_1_1 + 2)
   042F E5 0B               825 	mov	a,(_display_i_1_1 + 3)
   0431 12 06 3B            826 	lcall	__divulong
   0434 85 82 08            827 	mov	_display_i_1_1,dpl
   0437 85 83 09            828 	mov	(_display_i_1_1 + 1),dph
   043A 85 F0 0A            829 	mov	(_display_i_1_1 + 2),b
   043D F5 0B               830 	mov	(_display_i_1_1 + 3),a
                            831 ;	test3.c:58: d2=0xFF;
   043F 75 0D FF            832 	mov	_display_d2_1_1,#0xFF
                            833 ;	test3.c:59: d1=0x7F;
   0442 75 0C 7F            834 	mov	_display_d1_1_1,#0x7F
                            835 ;	test3.c:60: d3=0x7F;
   0445 75 0E 7F            836 	mov	_display_d3_1_1,#0x7F
   0448                     837 00111$:
                            838 ;	test3.c:62: P0=255;
   0448 75 80 FF            839 	mov	_P0,#0xFF
                            840 ;	test3.c:64: P2 = seg_position[3];
   044B 90 07 20            841 	mov	dptr,#(_seg_position + 0x0003)
   044E E4                  842 	clr	a
   044F 93                  843 	movc	a,@a+dptr
   0450 F5 A0               844 	mov	_P2,a
                            845 ;	test3.c:65: P0=display_seg[i/1000] & d3;
   0452 75 17 E8            846 	mov	__divulong_PARM_2,#0xE8
   0455 75 18 03            847 	mov	(__divulong_PARM_2 + 1),#0x03
   0458 75 19 00            848 	mov	(__divulong_PARM_2 + 2),#0x00
   045B 75 1A 00            849 	mov	(__divulong_PARM_2 + 3),#0x00
   045E 85 08 82            850 	mov	dpl,_display_i_1_1
   0461 85 09 83            851 	mov	dph,(_display_i_1_1 + 1)
   0464 85 0A F0            852 	mov	b,(_display_i_1_1 + 2)
   0467 E5 0B               853 	mov	a,(_display_i_1_1 + 3)
   0469 12 06 3B            854 	lcall	__divulong
   046C AA 82               855 	mov	r2,dpl
   046E AB 83               856 	mov	r3,dph
   0470 EA                  857 	mov	a,r2
   0471 24 12               858 	add	a,#_display_seg
   0473 F5 82               859 	mov	dpl,a
   0475 EB                  860 	mov	a,r3
   0476 34 07               861 	addc	a,#(_display_seg >> 8)
   0478 F5 83               862 	mov	dph,a
   047A E4                  863 	clr	a
   047B 93                  864 	movc	a,@a+dptr
   047C FA                  865 	mov	r2,a
   047D E5 0E               866 	mov	a,_display_d3_1_1
   047F 5A                  867 	anl	a,r2
   0480 F5 80               868 	mov	_P0,a
                            869 ;	test3.c:66: delay(2);
   0482 90 00 02            870 	mov	dptr,#0x0002
   0485 12 00 78            871 	lcall	_delay
                            872 ;	test3.c:68: P0=255;
   0488 75 80 FF            873 	mov	_P0,#0xFF
                            874 ;	test3.c:69: P2 = seg_position[2];
   048B 90 07 1F            875 	mov	dptr,#(_seg_position + 0x0002)
   048E E4                  876 	clr	a
   048F 93                  877 	movc	a,@a+dptr
   0490 F5 A0               878 	mov	_P2,a
                            879 ;	test3.c:70: P0=display_seg[i%1000/100] & d2;
   0492 75 17 E8            880 	mov	__modulong_PARM_2,#0xE8
   0495 75 18 03            881 	mov	(__modulong_PARM_2 + 1),#0x03
   0498 75 19 00            882 	mov	(__modulong_PARM_2 + 2),#0x00
   049B 75 1A 00            883 	mov	(__modulong_PARM_2 + 3),#0x00
   049E 85 08 82            884 	mov	dpl,_display_i_1_1
   04A1 85 09 83            885 	mov	dph,(_display_i_1_1 + 1)
   04A4 85 0A F0            886 	mov	b,(_display_i_1_1 + 2)
   04A7 E5 0B               887 	mov	a,(_display_i_1_1 + 3)
   04A9 12 05 B8            888 	lcall	__modulong
   04AC AA 82               889 	mov	r2,dpl
   04AE AB 83               890 	mov	r3,dph
   04B0 AC F0               891 	mov	r4,b
   04B2 FD                  892 	mov	r5,a
   04B3 75 17 64            893 	mov	__divulong_PARM_2,#0x64
   04B6 E4                  894 	clr	a
   04B7 F5 18               895 	mov	(__divulong_PARM_2 + 1),a
   04B9 F5 19               896 	mov	(__divulong_PARM_2 + 2),a
   04BB F5 1A               897 	mov	(__divulong_PARM_2 + 3),a
   04BD 8A 82               898 	mov	dpl,r2
   04BF 8B 83               899 	mov	dph,r3
   04C1 8C F0               900 	mov	b,r4
   04C3 ED                  901 	mov	a,r5
   04C4 12 06 3B            902 	lcall	__divulong
   04C7 AA 82               903 	mov	r2,dpl
   04C9 AB 83               904 	mov	r3,dph
   04CB EA                  905 	mov	a,r2
   04CC 24 12               906 	add	a,#_display_seg
   04CE F5 82               907 	mov	dpl,a
   04D0 EB                  908 	mov	a,r3
   04D1 34 07               909 	addc	a,#(_display_seg >> 8)
   04D3 F5 83               910 	mov	dph,a
   04D5 E4                  911 	clr	a
   04D6 93                  912 	movc	a,@a+dptr
   04D7 FA                  913 	mov	r2,a
   04D8 E5 0D               914 	mov	a,_display_d2_1_1
   04DA 5A                  915 	anl	a,r2
   04DB F5 80               916 	mov	_P0,a
                            917 ;	test3.c:71: delay(2);
   04DD 90 00 02            918 	mov	dptr,#0x0002
   04E0 12 00 78            919 	lcall	_delay
                            920 ;	test3.c:72: P0=255;
   04E3 75 80 FF            921 	mov	_P0,#0xFF
                            922 ;	test3.c:73: P2 = seg_position[1];
   04E6 90 07 1E            923 	mov	dptr,#(_seg_position + 0x0001)
   04E9 E4                  924 	clr	a
   04EA 93                  925 	movc	a,@a+dptr
   04EB F5 A0               926 	mov	_P2,a
                            927 ;	test3.c:74: P0=display_seg[i%100/10] & d1;
   04ED 75 17 64            928 	mov	__modulong_PARM_2,#0x64
   04F0 E4                  929 	clr	a
   04F1 F5 18               930 	mov	(__modulong_PARM_2 + 1),a
   04F3 F5 19               931 	mov	(__modulong_PARM_2 + 2),a
   04F5 F5 1A               932 	mov	(__modulong_PARM_2 + 3),a
   04F7 85 08 82            933 	mov	dpl,_display_i_1_1
   04FA 85 09 83            934 	mov	dph,(_display_i_1_1 + 1)
   04FD 85 0A F0            935 	mov	b,(_display_i_1_1 + 2)
   0500 E5 0B               936 	mov	a,(_display_i_1_1 + 3)
   0502 12 05 B8            937 	lcall	__modulong
   0505 AA 82               938 	mov	r2,dpl
   0507 AB 83               939 	mov	r3,dph
   0509 AC F0               940 	mov	r4,b
   050B FD                  941 	mov	r5,a
   050C 75 17 0A            942 	mov	__divulong_PARM_2,#0x0A
   050F E4                  943 	clr	a
   0510 F5 18               944 	mov	(__divulong_PARM_2 + 1),a
   0512 F5 19               945 	mov	(__divulong_PARM_2 + 2),a
   0514 F5 1A               946 	mov	(__divulong_PARM_2 + 3),a
   0516 8A 82               947 	mov	dpl,r2
   0518 8B 83               948 	mov	dph,r3
   051A 8C F0               949 	mov	b,r4
   051C ED                  950 	mov	a,r5
   051D 12 06 3B            951 	lcall	__divulong
   0520 AA 82               952 	mov	r2,dpl
   0522 AB 83               953 	mov	r3,dph
   0524 EA                  954 	mov	a,r2
   0525 24 12               955 	add	a,#_display_seg
   0527 F5 82               956 	mov	dpl,a
   0529 EB                  957 	mov	a,r3
   052A 34 07               958 	addc	a,#(_display_seg >> 8)
   052C F5 83               959 	mov	dph,a
   052E E4                  960 	clr	a
   052F 93                  961 	movc	a,@a+dptr
   0530 FA                  962 	mov	r2,a
   0531 E5 0C               963 	mov	a,_display_d1_1_1
   0533 5A                  964 	anl	a,r2
   0534 F5 80               965 	mov	_P0,a
                            966 ;	test3.c:75: delay(2);
   0536 90 00 02            967 	mov	dptr,#0x0002
   0539 12 00 78            968 	lcall	_delay
                            969 ;	test3.c:76: P0=255;
   053C 75 80 FF            970 	mov	_P0,#0xFF
                            971 ;	test3.c:77: P2 = seg_position[0];
   053F 90 07 1D            972 	mov	dptr,#_seg_position
   0542 E4                  973 	clr	a
   0543 93                  974 	movc	a,@a+dptr
   0544 F5 A0               975 	mov	_P2,a
                            976 ;	test3.c:78: P0=display_seg[i%10] & d0;
   0546 75 17 0A            977 	mov	__modulong_PARM_2,#0x0A
   0549 E4                  978 	clr	a
   054A F5 18               979 	mov	(__modulong_PARM_2 + 1),a
   054C F5 19               980 	mov	(__modulong_PARM_2 + 2),a
   054E F5 1A               981 	mov	(__modulong_PARM_2 + 3),a
   0550 85 08 82            982 	mov	dpl,_display_i_1_1
   0553 85 09 83            983 	mov	dph,(_display_i_1_1 + 1)
   0556 85 0A F0            984 	mov	b,(_display_i_1_1 + 2)
   0559 E5 0B               985 	mov	a,(_display_i_1_1 + 3)
   055B 12 05 B8            986 	lcall	__modulong
   055E AA 82               987 	mov	r2,dpl
   0560 AB 83               988 	mov	r3,dph
   0562 EA                  989 	mov	a,r2
   0563 24 12               990 	add	a,#_display_seg
   0565 F5 82               991 	mov	dpl,a
   0567 EB                  992 	mov	a,r3
   0568 34 07               993 	addc	a,#(_display_seg >> 8)
   056A F5 83               994 	mov	dph,a
   056C E4                  995 	clr	a
   056D 93                  996 	movc	a,@a+dptr
   056E F5 80               997 	mov	_P0,a
                            998 ;	test3.c:79: delay(2);
   0570 90 00 02            999 	mov	dptr,#0x0002
   0573 02 00 78           1000 	ljmp	_delay
                           1001 ;------------------------------------------------------------
                           1002 ;Allocation info for local variables in function 'main'
                           1003 ;------------------------------------------------------------
                           1004 ;------------------------------------------------------------
                           1005 ;	test3.c:85: void main()
                           1006 ;	-----------------------------------------
                           1007 ;	 function main
                           1008 ;	-----------------------------------------
   0576                    1009 _main:
                           1010 ;	test3.c:88: IE=0x8a;
   0576 75 A8 8A           1011 	mov	_IE,#0x8A
                           1012 ;	test3.c:89: TMOD=0x11;
   0579 75 89 11           1013 	mov	_TMOD,#0x11
                           1014 ;	test3.c:90: TH0=(65536-10000)/256;
   057C 75 8C D8           1015 	mov	_TH0,#0xD8
                           1016 ;	test3.c:91: TL0=(65536-10000)%256;
   057F 75 8A F0           1017 	mov	_TL0,#0xF0
                           1018 ;	test3.c:92: TR0=1;
   0582 D2 8C              1019 	setb	_TR0
                           1020 ;	test3.c:93: while(1)
   0584                    1021 00102$:
                           1022 ;	test3.c:97: display(i);
   0584 85 13 82           1023 	mov	dpl,_i
   0587 85 14 83           1024 	mov	dph,(_i + 1)
   058A 85 15 F0           1025 	mov	b,(_i + 2)
   058D E5 16              1026 	mov	a,(_i + 3)
   058F 12 00 A4           1027 	lcall	_display
   0592 80 F0              1028 	sjmp	00102$
                           1029 ;------------------------------------------------------------
                           1030 ;Allocation info for local variables in function 'timer'
                           1031 ;------------------------------------------------------------
                           1032 ;------------------------------------------------------------
                           1033 ;	test3.c:102: void timer() interrupt 1
                           1034 ;	-----------------------------------------
                           1035 ;	 function timer
                           1036 ;	-----------------------------------------
   0594                    1037 _timer:
   0594 C0 E0              1038 	push	acc
   0596 C0 D0              1039 	push	psw
   0598 75 D0 00           1040 	mov	psw,#0x00
                           1041 ;	test3.c:104: TH0=(65536-10000)/256;
   059B 75 8C D8           1042 	mov	_TH0,#0xD8
                           1043 ;	test3.c:105: TL0=(65536-10000)%256;
   059E 75 8A F0           1044 	mov	_TL0,#0xF0
                           1045 ;	test3.c:106: i++;
   05A1 05 13              1046 	inc	_i
   05A3 E4                 1047 	clr	a
   05A4 B5 13 0C           1048 	cjne	a,_i,00103$
   05A7 05 14              1049 	inc	(_i + 1)
   05A9 B5 14 07           1050 	cjne	a,(_i + 1),00103$
   05AC 05 15              1051 	inc	(_i + 2)
   05AE B5 15 02           1052 	cjne	a,(_i + 2),00103$
   05B1 05 16              1053 	inc	(_i + 3)
   05B3                    1054 00103$:
   05B3 D0 D0              1055 	pop	psw
   05B5 D0 E0              1056 	pop	acc
   05B7 32                 1057 	reti
                           1058 ;	eliminated unneeded push/pop dpl
                           1059 ;	eliminated unneeded push/pop dph
                           1060 ;	eliminated unneeded push/pop b
                           1061 	.area CSEG    (CODE)
                           1062 	.area CONST   (CODE)
   0712                    1063 _display_seg:
   0712 C0                 1064 	.db #0xC0
   0713 F9                 1065 	.db #0xF9
   0714 A4                 1066 	.db #0xA4
   0715 B0                 1067 	.db #0xB0
   0716 99                 1068 	.db #0x99
   0717 92                 1069 	.db #0x92
   0718 82                 1070 	.db #0x82
   0719 F8                 1071 	.db #0xF8
   071A 80                 1072 	.db #0x80
   071B 90                 1073 	.db #0x90
   071C 7F                 1074 	.db #0x7F
   071D                    1075 _seg_position:
   071D FE                 1076 	.db #0xFE
   071E FD                 1077 	.db #0xFD
   071F FB                 1078 	.db #0xFB
   0720 F7                 1079 	.db #0xF7
   0721 F0                 1080 	.db #0xF0
                           1081 	.area XINIT   (CODE)
                           1082 	.area CABS    (ABS,CODE)
