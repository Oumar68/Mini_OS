#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc ,char **argv){
    if(argc != 2){
        mini_printf("il faut un argument <nom du repertoire>");
        mini_exit();
    }
    struct stat status;

    int fichier_stat = stat(argv[1],&status);
    if(fichier_stat < 0){
        mini_perror("Impossible de recuperer le status du fichier");
        mini_exit();
    }
    if(!S_ISDIR(status.st_mode)){
        mini_printf("le fichier n'est pas un repertoire");
        mini_exit();
    }
    if(rmdir(argv[1])< 0){
        mini_perror("Erreur desctruction du repertoire");
        mini_exit();
    }
    mini_exit();
}