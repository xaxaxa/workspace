                              1 ;--------------------------------------------------------
                              2 ; File Created by SDCC : free open source ANSI-C Compiler
                              3 ; Version 2.9.0 #5416 (Feb  3 2010) (UNIX)
                              4 ; This file was generated Sun May 13 16:06:33 2012
                              5 ;--------------------------------------------------------
                              6 	.module asdf
                              7 	.optsdcc -mmcs51 --model-small
                              8 	
                              9 ;--------------------------------------------------------
                             10 ; Public variables in this module
                             11 ;--------------------------------------------------------
                             12 	.globl _main
                             13 	.globl _CY
                             14 	.globl _AC
                             15 	.globl _F0
                             16 	.globl _RS1
                             17 	.globl _RS0
                             18 	.globl _OV
                             19 	.globl _F1
                             20 	.globl _P
                             21 	.globl _PS
                             22 	.globl _PT1
                             23 	.globl _PX1
                             24 	.globl _PT0
                             25 	.globl _PX0
                             26 	.globl _RD
                             27 	.globl _WR
                             28 	.globl _T1
                             29 	.globl _T0
                             30 	.globl _INT1
                             31 	.globl _INT0
                             32 	.globl _TXD
                             33 	.globl _RXD
                             34 	.globl _P3_7
                             35 	.globl _P3_6
                             36 	.globl _P3_5
                             37 	.globl _P3_4
                             38 	.globl _P3_3
                             39 	.globl _P3_2
                             40 	.globl _P3_1
                             41 	.globl _P3_0
                             42 	.globl _EA
                             43 	.globl _ES
                             44 	.globl _ET1
                             45 	.globl _EX1
                             46 	.globl _ET0
                             47 	.globl _EX0
                             48 	.globl _P2_7
                             49 	.globl _P2_6
                             50 	.globl _P2_5
                             51 	.globl _P2_4
                             52 	.globl _P2_3
                             53 	.globl _P2_2
                             54 	.globl _P2_1
                             55 	.globl _P2_0
                             56 	.globl _SM0
                             57 	.globl _SM1
                             58 	.globl _SM2
                             59 	.globl _REN
                             60 	.globl _TB8
                             61 	.globl _RB8
                             62 	.globl _TI
                             63 	.globl _RI
                             64 	.globl _P1_7
                             65 	.globl _P1_6
                             66 	.globl _P1_5
                             67 	.globl _P1_4
                             68 	.globl _P1_3
                             69 	.globl _P1_2
                             70 	.globl _P1_1
                             71 	.globl _P1_0
                             72 	.globl _TF1
                             73 	.globl _TR1
                             74 	.globl _TF0
                             75 	.globl _TR0
                             76 	.globl _IE1
                             77 	.globl _IT1
                             78 	.globl _IE0
                             79 	.globl _IT0
                             80 	.globl _P0_7
                             81 	.globl _P0_6
                             82 	.globl _P0_5
                             83 	.globl _P0_4
                             84 	.globl _P0_3
                             85 	.globl _P0_2
                             86 	.globl _P0_1
                             87 	.globl _P0_0
                             88 	.globl _B
                             89 	.globl _ACC
                             90 	.globl _PSW
                             91 	.globl _IP
                             92 	.globl _P3
                             93 	.globl _IE
                             94 	.globl _P2
                             95 	.globl _SBUF
                             96 	.globl _SCON
                             97 	.globl _P1
                             98 	.globl _TH1
                             99 	.globl _TH0
                            100 	.globl _TL1
                            101 	.globl _TL0
                            102 	.globl _TMOD
                            103 	.globl _TCON
                            104 	.globl _PCON
                            105 	.globl _DPH
                            106 	.globl _DPL
                            107 	.globl _SP
                            108 	.globl _P0
                            109 ;--------------------------------------------------------
                            110 ; special function registers
                            111 ;--------------------------------------------------------
                            112 	.area RSEG    (DATA)
                    0080    113 _P0	=	0x0080
                    0081    114 _SP	=	0x0081
                    0082    115 _DPL	=	0x0082
                    0083    116 _DPH	=	0x0083
                    0087    117 _PCON	=	0x0087
                    0088    118 _TCON	=	0x0088
                    0089    119 _TMOD	=	0x0089
                    008A    120 _TL0	=	0x008a
                    008B    121 _TL1	=	0x008b
                    008C    122 _TH0	=	0x008c
                    008D    123 _TH1	=	0x008d
                    0090    124 _P1	=	0x0090
                    0098    125 _SCON	=	0x0098
                    0099    126 _SBUF	=	0x0099
                    00A0    127 _P2	=	0x00a0
                    00A8    128 _IE	=	0x00a8
                    00B0    129 _P3	=	0x00b0
                    00B8    130 _IP	=	0x00b8
                    00D0    131 _PSW	=	0x00d0
                    00E0    132 _ACC	=	0x00e0
                    00F0    133 _B	=	0x00f0
                            134 ;--------------------------------------------------------
                            135 ; special function bits
                            136 ;--------------------------------------------------------
                            137 	.area RSEG    (DATA)
                    0080    138 _P0_0	=	0x0080
                    0081    139 _P0_1	=	0x0081
                    0082    140 _P0_2	=	0x0082
                    0083    141 _P0_3	=	0x0083
                    0084    142 _P0_4	=	0x0084
                    0085    143 _P0_5	=	0x0085
                    0086    144 _P0_6	=	0x0086
                    0087    145 _P0_7	=	0x0087
                    0088    146 _IT0	=	0x0088
                    0089    147 _IE0	=	0x0089
                    008A    148 _IT1	=	0x008a
                    008B    149 _IE1	=	0x008b
                    008C    150 _TR0	=	0x008c
                    008D    151 _TF0	=	0x008d
                    008E    152 _TR1	=	0x008e
                    008F    153 _TF1	=	0x008f
                    0090    154 _P1_0	=	0x0090
                    0091    155 _P1_1	=	0x0091
                    0092    156 _P1_2	=	0x0092
                    0093    157 _P1_3	=	0x0093
                    0094    158 _P1_4	=	0x0094
                    0095    159 _P1_5	=	0x0095
                    0096    160 _P1_6	=	0x0096
                    0097    161 _P1_7	=	0x0097
                    0098    162 _RI	=	0x0098
                    0099    163 _TI	=	0x0099
                    009A    164 _RB8	=	0x009a
                    009B    165 _TB8	=	0x009b
                    009C    166 _REN	=	0x009c
                    009D    167 _SM2	=	0x009d
                    009E    168 _SM1	=	0x009e
                    009F    169 _SM0	=	0x009f
                    00A0    170 _P2_0	=	0x00a0
                    00A1    171 _P2_1	=	0x00a1
                    00A2    172 _P2_2	=	0x00a2
                    00A3    173 _P2_3	=	0x00a3
                    00A4    174 _P2_4	=	0x00a4
                    00A5    175 _P2_5	=	0x00a5
                    00A6    176 _P2_6	=	0x00a6
                    00A7    177 _P2_7	=	0x00a7
                    00A8    178 _EX0	=	0x00a8
                    00A9    179 _ET0	=	0x00a9
                    00AA    180 _EX1	=	0x00aa
                    00AB    181 _ET1	=	0x00ab
                    00AC    182 _ES	=	0x00ac
                    00AF    183 _EA	=	0x00af
                    00B0    184 _P3_0	=	0x00b0
                    00B1    185 _P3_1	=	0x00b1
                    00B2    186 _P3_2	=	0x00b2
                    00B3    187 _P3_3	=	0x00b3
                    00B4    188 _P3_4	=	0x00b4
                    00B5    189 _P3_5	=	0x00b5
                    00B6    190 _P3_6	=	0x00b6
                    00B7    191 _P3_7	=	0x00b7
                    00B0    192 _RXD	=	0x00b0
                    00B1    193 _TXD	=	0x00b1
                    00B2    194 _INT0	=	0x00b2
                    00B3    195 _INT1	=	0x00b3
                    00B4    196 _T0	=	0x00b4
                    00B5    197 _T1	=	0x00b5
                    00B6    198 _WR	=	0x00b6
                    00B7    199 _RD	=	0x00b7
                    00B8    200 _PX0	=	0x00b8
                    00B9    201 _PT0	=	0x00b9
                    00BA    202 _PX1	=	0x00ba
                    00BB    203 _PT1	=	0x00bb
                    00BC    204 _PS	=	0x00bc
                    00D0    205 _P	=	0x00d0
                    00D1    206 _F1	=	0x00d1
                    00D2    207 _OV	=	0x00d2
                    00D3    208 _RS0	=	0x00d3
                    00D4    209 _RS1	=	0x00d4
                    00D5    210 _F0	=	0x00d5
                    00D6    211 _AC	=	0x00d6
                    00D7    212 _CY	=	0x00d7
                            213 ;--------------------------------------------------------
                            214 ; overlayable register banks
                            215 ;--------------------------------------------------------
                            216 	.area REG_BANK_0	(REL,OVR,DATA)
   0000                     217 	.ds 8
                            218 ;--------------------------------------------------------
                            219 ; internal ram data
                            220 ;--------------------------------------------------------
                            221 	.area DSEG    (DATA)
                            222 ;--------------------------------------------------------
                            223 ; overlayable items in internal ram 
                            224 ;--------------------------------------------------------
                            225 	.area OSEG    (OVR,DATA)
                            226 ;--------------------------------------------------------
                            227 ; Stack segment in internal ram 
                            228 ;--------------------------------------------------------
                            229 	.area	SSEG	(DATA)
   0008                     230 __start__stack:
   0008                     231 	.ds	1
                            232 
                            233 ;--------------------------------------------------------
                            234 ; indirectly addressable internal ram data
                            235 ;--------------------------------------------------------
                            236 	.area ISEG    (DATA)
                            237 ;--------------------------------------------------------
                            238 ; absolute internal ram data
                            239 ;--------------------------------------------------------
                            240 	.area IABS    (ABS,DATA)
                            241 	.area IABS    (ABS,DATA)
                            242 ;--------------------------------------------------------
                            243 ; bit data
                            244 ;--------------------------------------------------------
                            245 	.area BSEG    (BIT)
                            246 ;--------------------------------------------------------
                            247 ; paged external ram data
                            248 ;--------------------------------------------------------
                            249 	.area PSEG    (PAG,XDATA)
                            250 ;--------------------------------------------------------
                            251 ; external ram data
                            252 ;--------------------------------------------------------
                            253 	.area XSEG    (XDATA)
                            254 ;--------------------------------------------------------
                            255 ; absolute external ram data
                            256 ;--------------------------------------------------------
                            257 	.area XABS    (ABS,XDATA)
                            258 ;--------------------------------------------------------
                            259 ; external initialized ram data
                            260 ;--------------------------------------------------------
                            261 	.area XISEG   (XDATA)
                            262 	.area HOME    (CODE)
                            263 	.area GSINIT0 (CODE)
                            264 	.area GSINIT1 (CODE)
                            265 	.area GSINIT2 (CODE)
                            266 	.area GSINIT3 (CODE)
                            267 	.area GSINIT4 (CODE)
                            268 	.area GSINIT5 (CODE)
                            269 	.area GSINIT  (CODE)
                            270 	.area GSFINAL (CODE)
                            271 	.area CSEG    (CODE)
                            272 ;--------------------------------------------------------
                            273 ; interrupt vector 
                            274 ;--------------------------------------------------------
                            275 	.area HOME    (CODE)
   0000                     276 __interrupt_vect:
   0000 02 00 08            277 	ljmp	__sdcc_gsinit_startup
                            278 ;--------------------------------------------------------
                            279 ; global & static initialisations
                            280 ;--------------------------------------------------------
                            281 	.area HOME    (CODE)
                            282 	.area GSINIT  (CODE)
                            283 	.area GSFINAL (CODE)
                            284 	.area GSINIT  (CODE)
                            285 	.globl __sdcc_gsinit_startup
                            286 	.globl __sdcc_program_startup
                            287 	.globl __start__stack
                            288 	.globl __mcs51_genXINIT
                            289 	.globl __mcs51_genXRAMCLEAR
                            290 	.globl __mcs51_genRAMCLEAR
                            291 	.area GSFINAL (CODE)
   0061 02 00 03            292 	ljmp	__sdcc_program_startup
                            293 ;--------------------------------------------------------
                            294 ; Home
                            295 ;--------------------------------------------------------
                            296 	.area HOME    (CODE)
                            297 	.area HOME    (CODE)
   0003                     298 __sdcc_program_startup:
   0003 12 00 64            299 	lcall	_main
                            300 ;	return from main will lock up
   0006 80 FE               301 	sjmp .
                            302 ;--------------------------------------------------------
                            303 ; code
                            304 ;--------------------------------------------------------
                            305 	.area CSEG    (CODE)
                            306 ;------------------------------------------------------------
                            307 ;Allocation info for local variables in function 'main'
                            308 ;------------------------------------------------------------
                            309 ;------------------------------------------------------------
                            310 ;	asdf.c:3: unsigned int main(void)
                            311 ;	-----------------------------------------
                            312 ;	 function main
                            313 ;	-----------------------------------------
   0064                     314 _main:
                    0002    315 	ar2 = 0x02
                    0003    316 	ar3 = 0x03
                    0004    317 	ar4 = 0x04
                    0005    318 	ar5 = 0x05
                    0006    319 	ar6 = 0x06
                    0007    320 	ar7 = 0x07
                    0000    321 	ar0 = 0x00
                    0001    322 	ar1 = 0x01
                            323 ;	asdf.c:5: P0=0xff;
   0064 75 80 FF            324 	mov	_P0,#0xFF
                            325 ;	asdf.c:6: P1=0xff;
   0067 75 90 FF            326 	mov	_P1,#0xFF
                            327 ;	asdf.c:7: P2=0xff;
   006A 75 A0 FF            328 	mov	_P2,#0xFF
                            329 ;	asdf.c:8: P3=0xff;
   006D 75 B0 FF            330 	mov	_P3,#0xFF
                            331 ;	asdf.c:9: while(1)
   0070                     332 00102$:
                            333 ;	asdf.c:11: P0=~P0;
   0070 E5 80               334 	mov	a,_P0
   0072 F4                  335 	cpl	a
   0073 F5 80               336 	mov	_P0,a
                            337 ;	asdf.c:12: P1=~P1;
   0075 E5 90               338 	mov	a,_P1
   0077 F4                  339 	cpl	a
   0078 F5 90               340 	mov	_P1,a
                            341 ;	asdf.c:13: P2=~P2;
   007A E5 A0               342 	mov	a,_P2
   007C F4                  343 	cpl	a
   007D F5 A0               344 	mov	_P2,a
                            345 ;	asdf.c:14: P3=~P3;
   007F E5 B0               346 	mov	a,_P3
   0081 F4                  347 	cpl	a
   0082 F5 B0               348 	mov	_P3,a
   0084 80 EA               349 	sjmp	00102$
                            350 	.area CSEG    (CODE)
                            351 	.area CONST   (CODE)
                            352 	.area XINIT   (CODE)
                            353 	.area CABS    (ABS,CODE)
