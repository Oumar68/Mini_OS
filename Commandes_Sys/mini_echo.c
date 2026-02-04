#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc , char *argv[]){
    if(argc < 2) {
        mini_perror("Usage : mini_echo <chaine>\n");
        mini_exit();
    }
    void *buffer = mini_calloc(sizeof(char),IOBUFFER_SIZE);
    int position_index = 0;
    for(int i = 1 ; i < argc ; i++){
        int taille_arg = 0;
        while(argv[i][taille_arg] != '\0'){
            taille_arg++;
        }
        mini_strcpy(argv[i],buffer + position_index,IOBUFFER_SIZE-position_index);
        position_index += taille_arg;
        if( i != argc -1){
            mini_strcpy(" ",buffer + position_index,IOBUFFER_SIZE-position_index);
            position_index += 1;
        }

    }
    mini_printf(buffer);
    mini_free(buffer);
    mini_exit();
}