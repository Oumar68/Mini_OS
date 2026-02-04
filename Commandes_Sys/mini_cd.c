#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc , char *argv[]){
    if(argc != 2) {
        mini_perror("Usage : mini_cd <nom_dossier>\n");
        mini_exit();
    }
    else{
        if (chdir(args[1]) == -1) { 
            mini_perror("mini_cd");
        }
    }
    mini_exit();
}