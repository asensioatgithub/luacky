#include "c.h"

int main(char argc, char* argv[]){
    input_init(argc, argv);
    int t;
    for(;*cp != '\0';){
        t = gettok();
        // if(t==75)
        //     prinf("%c\n", t);
        // else
        //     printf("%s\n",luaX_tokens[t-FIRST_RESERVED]);
        printf("%d ",t);
    }
  
}
