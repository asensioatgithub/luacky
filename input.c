#include "c.h"

#define BUFSIZE 2048
static int bsize;   
static unsigned char buffer[BUFSIZE+1];

unsigned char *cp;	/* current input character */
FILE *file;		/* current input file name */
unsigned char *limit;	/* points to last character */
char *line;		/* current line */
int lineno;		/* line number of current line */

// void nextline(void) {
//     do{

//     }
// }



void fillbuf(void) {
    if(cp > limit)
        cp = buffer;
    else {
        int n = limit - cp;
        unsigned char *s = buffer;
        for(;cp <= limit; *s++ = *cp++)
            ;
        limit = --cp;
        cp = buffer;
    }
    bsize = fread(buffer, 1, BUFSIZE, file);
    if(bsize < 0){
        perror("read error");
        exit(EXIT_FAILURE);
    }
    limit = limit + bsize;
    *(limit+1) = '\0';
}

void input_init(int argc, char *argv[]) {
    static int inited;
    limit = cp = buffer; 
    if(inited) 
        return ;
    if((file = fopen(argv[1], "r"))== NULL){
        perror("file is not exit");
        exit(EXIT_FAILURE);
    }
    fillbuf();
    inited = 1;
}