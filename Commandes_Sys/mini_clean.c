#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc , char *argv[]){
    if(argc != 2) {
        mini_perror("Usage : mini_touch <nom_fichier>\n");
        mini_exit();
    }
    else{
        struct stat buffer;
        int exist = stat(argv[1],&buffer);
        if(exist ==  0){
            MYFILE *f = mini_open(argv[1],'w');
            mini_fclose(f); MYFILE *f = mini_open(argv[2],'w');
            mini_fclose(f);
            mini_exit();
        }
        MYFILE *f = mini_open(argv[1],'a');
            if(f == NULL) {
                mini_perror("Erreur creation fichier");
                mini_exit();
            }
            mini_printf("Fichier cree \n");
            mini_fclose(f);
        }
    mini_exit();
}