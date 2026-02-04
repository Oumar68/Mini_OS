#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc , char *argv[]){
    if(argc != 2){
        mini_perror("Usage : mini_mkdir <nom_dossier>\n");
        mini_exit();
    }
    int status = mkdir(argv[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(status == -1){
        mini_perror("Erreur creation dossier");
        mini_exit();
    }
    mini_exit();
}