#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../mini_lib.h"

int main(int argc , char *argv[]){
    if(argc != 3){
        mini_perror("Usage : mini_mv <source> <destination>\n");
        mini_exit();
    }
    int status = rename(argv[1], argv[2]);
    if(status == -1){
        mini_perror("Erreur de déplacement");
        mini_exit();
    }
    mini_exit();
}