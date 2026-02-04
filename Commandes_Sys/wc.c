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
    MYFILE *f  = mini_open(argv[1],'r');
    if(!f){
        mini_perror("Erreur ouverture fichier");
        mini_exit();
    }

    void *buffer = mini_calloc(sizeof(char), IOBUFFER_SIZE);
    if(!buffer){
        mini_perror("Erreur allocation mémoire");
        mini_exit();
    }
    int nb_lues;
    int nb_mots = 0;
    int dans_un_mot = 0; // Flag : 0 = espace, 1 = lettre

    while ((nb_lues = mini_fread(buffer, 1, IOBUFFER_SIZE, f)) > 0) {
        for (int i = 0; i < nb_lues; i++) {
            char c = ((char*)buffer)[i];

            if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                dans_un_mot = 0;
            } else {
                // Si on n'était pas dans un mot et qu'on trouve un caractère normal
                if (dans_un_mot == 0) {
                    nb_mots++;
                    dans_un_mot = 1;
                }
            }
        }
    }
    char *mots = mini_itoa(nb_mots,mini_calloc(sizeof(char),12)); 
    mini_printf(mots);
    mini_free(buffer);
    mini_free(mots);
    mini_fclose(f);
    mini_exit();
}