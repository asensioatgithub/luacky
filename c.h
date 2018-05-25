#include <stdio.h>
#include <stdlib.h>

#define MAXTOKEN 32
#define FIRST_RESERVED	257

/*input.c*/
extern unsigned char *cp;	/* current input character */
extern FILE *file;		/* current input file name */
extern char *firstfile;	/* first input file */
extern unsigned char *limit;	/* points to last character + 1 */
extern char *line;		/* current line */
extern int lineno;		/* line number of current line */

extern void input_init(int argc, char *argv[]);
extern void fillbuf();

/*lex.c*/
extern int gettok();
extern const char *const luaX_tokens[];