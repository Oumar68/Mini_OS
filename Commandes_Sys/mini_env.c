#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

extern char **environ ;
int main(){
    char **envp = environ;
    int i = 0 ;
    while (envp[i] != NULL){
        mini_printf(envp[i]);
        i++;
    }
    mini_exit();
}