#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../mini_lib.h"

extern char **environ ;

char* mini_getenv(char *name) {
    int i = 0;
    int name_len = mini_strlen(name);

    while (environ[i] != NULL) {
        // On compare 'name' avec le début de la ligne environ[i]
        int j = 0;
        while (environ[i][j] == name[j] && j < name_len) {
            j++;
        }

        // Si on a parcouru tout 'name' et qu'on tombe sur '=' dans environ[i]
        if (j == name_len && environ[i][j] == '=') {
            // On renvoie le pointeur juste après le '='
            return &environ[i][j + 1];
        }
        i++;
    }
    return NULL; 
}


int main(int argc ,char **argv){
    if(argc != 2) {
        mini_perror("Usage : mini_getenv <nom_variable_environnement>\n");
        mini_exit();
    }
    char *var = mini_getenv(argv[1]);
    mini_printf(var);
    mini_exit();
}