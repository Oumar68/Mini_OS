#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include "mini_lib.h"

int main(int argc , char *argv[]){
    if(argc != 3) {
        mini_perror("Usage : mini_cp <source> <destination>\n");
        mini_exit();
    }
    else{
        MYFILE *src = mini_open(argv[1],'r');
        MYFILE *dest = mini_open(argv[2],'w');
        if(!src || !dest){
            mini_perror("Erreur ouverture fichiers");
            mini_exit();
            }
            void *buffer = mini_calloc(sizeof(char),IOBUFFER_SIZE);
            if(!buffer){
                mini_perror("Erreur d'alloctaion mémoire");
                mini_exit();
            }
            int nblues;
            while((nblues = mini_fread(buffer,sizeof(char),IOBUFFER_SIZE,src)) > 0){
                int nbecrit = mini_fwrite(buffer,sizeof(char),nblues,dest);
                if(nbecrit < 0){
                    mini_perror("Erreur ecriture fichier");
                    mini_exit();
                }
            }
            mini_free(buffer);
            mini_fclose(dest);
            mini_fclose(src);
        }
}
