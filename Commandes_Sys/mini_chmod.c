#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"
//Conversion d'une chaine octale en mode_t pour eviter l'utilisation de fonctions glibc 
mode_t octal_to_mode(const char *str) {
    mode_t mode = 0;
    while (*str) {
        char c = *str++;
        if (c < '0' || c > '7') break; // non valide
        mode = (mode << 3) | (c - '0'); // multiplication par 8 + valeur du chiffre
    }
    return mode;
}
int main(int argc , char *argv[]){
    if(argc != 3) {
        mini_perror("Usage : mini_chmod <mode> <nom_fichier>\n");
        mini_exit();
    }
    else{
        struct stat buffer;
        int exist = stat(argv[2],&buffer);
        if(exist ==  0){
            mode_t mode = octal_to_mode(argv[1]);
            if(chmod(argv[2],mode)== -1){
                mini_perror("Erreur chmod");
                mini_exit();
            }
        }
        else{
            mini_perror("Fichier n'existe pas"); 
            mini_exit();
        }
    }
    mini_exit();
}