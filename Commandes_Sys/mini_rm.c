#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc ,char **argv){
    if(argc != 2){
        mini_printf("il faut un argument <nom du fihier>");
        mini_exit();
    }
    struct stat status;

    int fichier_stat = stat(argv[1],&status);
    if(fichier_stat < 0){
        mini_perror("Impossible de recuperer le status du fichier");
        mini_exit();
    }
    if(!S_ISREG(status.st_mode)){
        mini_printf("le fichier n'est pas régulier");
        mini_exit();
    }
    if(unlink(argv[1])< 0){
        mini_perror("Erreur desctruction de fichier");
        mini_exit();
    }
    mini_exit();
}