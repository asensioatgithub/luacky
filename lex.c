// #include "lex.h"
#include "c.h"



enum RESERVED {
  /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED, TK_BREAK,
  TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FALSE, TK_FOR, TK_FUNCTION,
  TK_GOTO, TK_IF, TK_IN, TK_LOCAL, TK_NIL, TK_NOT, TK_OR, TK_REPEAT,
  TK_RETURN, TK_THEN, TK_TRUE, TK_UNTIL, TK_WHILE,
  /* other terminal symbols */
  TK_IDIV, TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE,
  TK_SHL, TK_SHR,
  TK_DBCOLON, TK_EOS,
  TK_FLT, TK_INT, TK_NAME, TK_STRING
};

/* ORDER RESERVED */
const char *const luaX_tokens [] = {
    "and", "break", "do", "else", "elseif",
    "end", "false", "for", "function", "goto", "if",
    "in", "local", "nil", "not", "or", "repeat",
    "return", "then", "true", "until", "while",
    "//", "..", "...", "==", ">=", "<=", "~=",
    "<<", ">>", "::", "<eof>",
    "<number>", "<integer>", "<name>", "<string>"
};




enum { BLANK=01,  NEWLINE=02, LETTER=04,
       DIGIT=010, HEX=020,    OTHER=040 };

static unsigned char map[256] = { /* 000 nul */	0,
				   /* 001 soh */	0,
				   /* 002 stx */	0,
				   /* 003 etx */	0,
				   /* 004 eot */	0,
				   /* 005 enq */	0,
				   /* 006 ack */	0,
				   /* 007 bel */	0,
				   /* 010 bs  */	0,
				   /* 011 ht  */	BLANK,
				   /* 012 nl  */	NEWLINE,
				   /* 013 vt  */	BLANK,
				   /* 014 ff  */	BLANK,
				   /* 015 cr  */	0,
				   /* 016 so  */	0,
				   /* 017 si  */	0,
				   /* 020 dle */	0,
				   /* 021 dc1 */	0,
				   /* 022 dc2 */	0,
				   /* 023 dc3 */	0,
				   /* 024 dc4 */	0,
				   /* 025 nak */	0,
				   /* 026 syn */	0,
				   /* 027 etb */	0,
				   /* 030 can */	0,
				   /* 031 em  */	0,
				   /* 032 sub */	0,
				   /* 033 esc */	0,
				   /* 034 fs  */	0,
				   /* 035 gs  */	0,
				   /* 036 rs  */	0,
				   /* 037 us  */	0,
				   /* 040 sp  */	BLANK,
				   /* 041 !   */	OTHER,
				   /* 042 "   */	OTHER,
				   /* 043 #   */	OTHER,
				   /* 044 $   */	0,
				   /* 045 %   */	OTHER,
				   /* 046 &   */	OTHER,
				   /* 047 '   */	OTHER,
				   /* 050 (   */	OTHER,
				   /* 051 )   */	OTHER,
				   /* 052 *   */	OTHER,
				   /* 053 +   */	OTHER,
				   /* 054 ,   */	OTHER,
				   /* 055 -   */	OTHER,
				   /* 056 .   */	OTHER,
				   /* 057 /   */	OTHER,
				   /* 060 0   */	DIGIT,
				   /* 061 1   */	DIGIT,
				   /* 062 2   */	DIGIT,
				   /* 063 3   */	DIGIT,
				   /* 064 4   */	DIGIT,
				   /* 065 5   */	DIGIT,
				   /* 066 6   */	DIGIT,
				   /* 067 7   */	DIGIT,
				   /* 070 8   */	DIGIT,
				   /* 071 9   */	DIGIT,
				   /* 072 :   */	OTHER,
				   /* 073 ;   */	OTHER,
				   /* 074 <   */	OTHER,
				   /* 075 =   */	OTHER,
				   /* 076 >   */	OTHER,
				   /* 077 ?   */	OTHER,
				   /* 100 @   */	0,
				   /* 101 A   */	LETTER|HEX,
				   /* 102 B   */	LETTER|HEX,
				   /* 103 C   */	LETTER|HEX,
				   /* 104 D   */	LETTER|HEX,
				   /* 105 E   */	LETTER|HEX,
				   /* 106 F   */	LETTER|HEX,
				   /* 107 G   */	LETTER,
				   /* 110 H   */	LETTER,
				   /* 111 I   */	LETTER,
				   /* 112 J   */	LETTER,
				   /* 113 K   */	LETTER,
				   /* 114 L   */	LETTER,
				   /* 115 M   */	LETTER,
				   /* 116 N   */	LETTER,
				   /* 117 O   */	LETTER,
				   /* 120 P   */	LETTER,
				   /* 121 Q   */	LETTER,
				   /* 122 R   */	LETTER,
				   /* 123 S   */	LETTER,
				   /* 124 T   */	LETTER,
				   /* 125 U   */	LETTER,
				   /* 126 V   */	LETTER,
				   /* 127 W   */	LETTER,
				   /* 130 X   */	LETTER,
				   /* 131 Y   */	LETTER,
				   /* 132 Z   */	LETTER,
				   /* 133 [   */	OTHER,
				   /* 134 \   */	OTHER,
				   /* 135 ]   */	OTHER,
				   /* 136 ^   */	OTHER,
				   /* 137 _   */	LETTER,
				   /* 140 `   */	0,
				   /* 141 a   */	LETTER|HEX,
				   /* 142 b   */	LETTER|HEX,
				   /* 143 c   */	LETTER|HEX,
				   /* 144 d   */	LETTER|HEX,
				   /* 145 e   */	LETTER|HEX,
				   /* 146 f   */	LETTER|HEX,
				   /* 147 g   */	LETTER,
				   /* 150 h   */	LETTER,
				   /* 151 i   */	LETTER,
				   /* 152 j   */	LETTER,
				   /* 153 k   */	LETTER,
				   /* 154 l   */	LETTER,
				   /* 155 m   */	LETTER,
				   /* 156 n   */	LETTER,
				   /* 157 o   */	LETTER,
				   /* 160 p   */	LETTER,
				   /* 161 q   */	LETTER,
				   /* 162 r   */	LETTER,
				   /* 163 s   */	LETTER,
				   /* 164 t   */	LETTER,
				   /* 165 u   */	LETTER,
				   /* 166 v   */	LETTER,
				   /* 167 w   */	LETTER,
				   /* 170 x   */	LETTER,
				   /* 171 y   */	LETTER,
				   /* 172 z   */	LETTER,
				   /* 173 {   */	OTHER,
				   /* 174 |   */	OTHER,
				   /* 175 }   */	OTHER,
				   /* 176 ~   */	OTHER, };




int gettok(void) {
    register unsigned char *rcp = cp;
    // if (limit - rcp < MAXTOKEN) {
	// 	cp = rcp;
	// 	fillbuf();
	// 	rcp = cp;
	// }
    for(;;){
        while (map[*rcp]&BLANK)
		    rcp++;
        cp = rcp + 1;
        switch (*rcp++){
            /*运算符*/
            case '=': return *rcp == '=' ? cp+=1, TK_EQ : '=';
            case '/': return *rcp == '/' ? cp++, TK_IDIV: '/';
            case '~': return *rcp == '=' ? cp++, TK_NE  : '~';
            case '<':
			    if (*rcp == '=') return cp++, TK_LE;
			    if (*rcp == '<') return cp++, TK_SHL;
			    return '<';
		    case '>':
			    if (*rcp == '=') return cp++, TK_GE;
			    if (*rcp == '>') return cp++, TK_SHR;
			    return '>';

            /*保留关键字*/
            case 'a':
                if (rcp[0] == 'n'
                &&  rcp[1] == 'd'
                &&  !(map[rcp[2]] & (DIGIT|LETTER))) {
                    cp = rcp + 2;
                    return TK_AND;
                }
                goto id;
            case 'b':
                if (rcp[0] == 'r'
                &&  rcp[1] == 'e'
                &&  rcp[2] == 'a'
                &&  rcp[3] == 'k'
                &&  !(map[rcp[4]] & (DIGIT|LETTER))) {
                    cp = rcp + 4;
                    return TK_BREAK;
                }
                goto id;
            case 'd':
                if (rcp[0] == 'o'
                && !(map[rcp[1]] & (DIGIT|LETTER))) {
                    cp = rcp + 1;
                    return TK_DO;
                }
                goto id;
            case 'e':
                if (rcp[0] == 'n'
                &&  rcp[1] == 'd'
                && !(map[rcp[2]] & (DIGIT|LETTER))) {
                    cp = rcp + 2;
                    return TK_END;
                }
                if (rcp[0] == 'l'
                &&  rcp[1] == 's'
                &&  rcp[2] == 'e'
                && !(map[rcp[3]] & (DIGIT|LETTER))) {
                    cp = rcp + 3;
                    return TK_ELSE;
                }
                if (rcp[0] == 'l'
                &&  rcp[1] == 's'
                &&  rcp[2] == 'e'
                &&  rcp[3] == 'i'
                &&  rcp[4] == 'f'
                && !(map[rcp[5]] & (DIGIT|LETTER))) {
                    cp = rcp + 5;
                    return TK_ELSEIF;
                }
                goto id;
            case 'f':
                if (rcp[0] == 'o'
                &&  rcp[1] == 'r'
                && !(map[rcp[2]] & (DIGIT|LETTER))) {
                    cp = rcp + 2;
                    return TK_FOR;
                }
                if (rcp[0] == 'a'
                &&  rcp[1] == 'l'
                &&  rcp[2] == 's'
                &&  rcp[3] == 'e'
                && !(map[rcp[4]] & (DIGIT|LETTER))) {
                    cp = rcp + 4;
                    return TK_FALSE;
                }
                if (rcp[0] == 'u'
                &&  rcp[1] == 'n'
                &&  rcp[2] == 'c'
                &&  rcp[3] == 't'
                &&  rcp[4] == 'i'
                &&  rcp[5] == 'o'
                &&  rcp[6] == 'n'
                && !(map[rcp[7]] & (DIGIT|LETTER))) {
                    cp = rcp + 7;
                    return TK_FUNCTION;
                }
                goto id; 
            case 'i':
                if (rcp[0] == 'n'
                && !(map[rcp[1]] & (DIGIT|LETTER))) {
                    cp = rcp + 1;
                    return TK_IN;
                }
                if (rcp[0] == 'f'
                && !(map[rcp[1]] & (DIGIT|LETTER))) {
                    cp = rcp + 1;
                    return TK_IF;
                }
                goto id;
            case 'l':
                if (rcp[0] == 'o'
                &&  rcp[1] == 'c'
                &&  rcp[2] == 'a'
                &&  rcp[3] == 'l'
                &&  !(map[rcp[4]] & (DIGIT|LETTER))) {
                    cp = rcp + 4;
                    return TK_LOCAL;
                }
                goto id;
            case 'n':
                if (rcp[0] == 'i'
                &&  rcp[1] == 'l'
                &&  !(map[rcp[2]] & (DIGIT|LETTER))) {
                    cp = rcp + 2;
                    return TK_NIL;
                }
                if (rcp[0] == 'o'
                &&  rcp[1] == 't'
                &&  !(map[rcp[2]] & (DIGIT|LETTER))) {
                    cp = rcp + 2;
                    return TK_NOT;
                }
                goto id;
            case 'o':
                if (rcp[0] == 'r'
                &&  !(map[rcp[1]] & (DIGIT|LETTER))) {
                    cp = rcp + 1;
                    return TK_OR;
                }
                goto id;
            case 'r':
                if (rcp[0] == 'p'
                &&  rcp[1] == 'e'
                &&  rcp[2] == 'a'
                &&  rcp[3] == 't'
                &&  !(map[rcp[4]] & (DIGIT|LETTER))) {
                    cp = rcp + 4;
                    return TK_REPEAT;
                }
                if (rcp[0] == 't'
                &&  rcp[1] == 'u'
                &&  rcp[2] == 'r'
                &&  rcp[3] == 'n'
                &&  !(map[rcp[4]] & (DIGIT|LETTER))) {
                    cp = rcp + 4;
                    return TK_RETURN;
                }
                goto id;
            case 't':
                if (rcp[0] == 'h'
                &&  rcp[1] == 'e'
                &&  rcp[2] == 'n'
                &&  !(map[rcp[3]] & (DIGIT|LETTER))) {
                    cp = rcp + 3;
                    return TK_THEN;
                }
                if (rcp[0] == 'r'
                &&  rcp[1] == 'u'
                &&  rcp[2] == 'r'
                &&  !(map[rcp[3]] & (DIGIT|LETTER))) {
                    cp = rcp + 3;
                    return TK_TRUE;
                }
                goto id;
            case 'u':
                if (rcp[0] == 'n'
                &&  rcp[1] == 't'
                &&  rcp[2] == 'i'
                &&  rcp[3] == 'l'
                &&  !(map[rcp[4]] & (DIGIT|LETTER))) {
                    cp = rcp + 4;
                    return TK_UNTIL;
                }
                goto id;
            case 'w':
                if (rcp[0] == 'h'
                &&  rcp[1] == 'i'
                &&  rcp[2] == 'l'
                &&  rcp[3] == 'e'
                &&  !(map[rcp[4]] & (DIGIT|LETTER))) {
                    cp = rcp + 4;
                    return TK_WHILE;
                }
                goto id;

            
            /*标志符*/
            case '_':
            case 'c': case 'g': case 'h': case 'j': case 'k': case 'm':
		    case 'p': case 'q': case 's': case 'v': case 'x': case 'y': 
            case 'z':
		    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		    case 'G': case 'H': case 'I': case 'J': case 'K': case 'M': 
            case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': 
            case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': 
            case 'Z':
            id:
                while(map[*rcp]&(DIGIT|LETTER))
                    rcp++;
                cp = rcp;
                return TK_NAME;

            /*整形or浮点数*/
            case '0': case '1': case '2': case '3': case '4':
		    case '5': case '6': case '7': case '8': case '9': 
            /*注释*/
            case '-':
                if(*rcp == '-'){ //单行注释
                    for(rcp++; map[*rcp]&NEWLINE == 0; rcp++)
                        ;
                    cp = rcp;
                    continue;
                }
                return '-'; //减号
                
        }
    } 
}



