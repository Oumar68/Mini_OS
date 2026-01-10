#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc ,char *argv[]){
    if(argc != 2) {
        mini_printf("Usage : mini_cat <nom_fichier>\n");
        mini_exit();
    }
    struct stat etat;
        int exist = stat(argv[1],&etat);
        if(exist < 0){
            mini_perror("le fichier n'existe pas\n");
            mini_exit();
        }
        MYFILE *f  = mini_open(argv[1],'r');
        if(!f){
                mini_perror("Erreur ouverture fichier");
                mini_exit();
        }
        void *buffer = mini_calloc(sizeof(char),IOBUFFER_SIZE);
        if(!buffer ){
            mini_perror("Erreur allocation  mémoire");
            mini_exit();
        }
        int nb_lues;
        while((nb_lues = mini_fread(buffer,sizeof(char),IOBUFFER_SIZE,f)) > 0){
            int nb_ecrit = write(1,buffer,nb_lues);
            if(nb_ecrit < 0){
                mini_perror("Erreur ecriture sur la sortie standart");
                mini_exit();
            }
        }
    mini_free(buffer);
    mini_fclose(f);
    mini_exit();
}