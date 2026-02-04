#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

#define SORTIE_SIZE 512

int main(int argc ,char *argv[]){
    if(argc < 3) {
        mini_perror("Usage : mini_head <nombre_de_lignes> <nom_fichier>\n");
        mini_exit();
    }

    struct stat etat;
    if(stat(argv[2], &etat) < 0){
        mini_printf("le fichier n'existe pas\n");
        mini_exit();
    }

    MYFILE *f  = mini_open(argv[2],'r');
    if(!f){
        mini_perror("Erreur ouverture fichier");
        mini_exit();
    }

    void *buffer = mini_calloc(sizeof(char), IOBUFFER_SIZE);
    if(!buffer){
        mini_perror("Erreur allocation mémoire");
        mini_exit();
    }

    char buffer_sortie[SORTIE_SIZE];
    int nb_lues;
    int nb_lignes = 0;
    int pos_sortie = 0;
    int max_lignes = atoi(argv[1]);

    while((nb_lues = mini_fread(buffer, 1, IOBUFFER_SIZE, f)) > 0){
        for(int i = 0; i < nb_lues; i++){
            buffer_sortie[pos_sortie++] = ((char*)buffer)[i];

            if(((char*)buffer)[i] == '\n'){
                nb_lignes++;
                if(nb_lignes >= max_lignes){
                    // écrire ce qu’il reste dans le tampon intermédiaire
                    if(pos_sortie > 0){
                        if(write(1, buffer_sortie, pos_sortie) < 0){
                            mini_perror("Erreur écriture sur la sortie standard");
                            mini_exit();
                        }
                    }
                    mini_free(buffer);
                    mini_fclose(f);
                    mini_exit();
                }
            }

            // si tampon intermédiaire plein
            if(pos_sortie >= SORTIE_SIZE){
                if(write(1, buffer_sortie, pos_sortie) < 0){
                    mini_perror("Erreur écriture sur la sortie standard");
                    mini_exit();
                }
                pos_sortie = 0;
            }
        }

        // écrire ce qu’il reste dans le tampon intermédiaire
        if(pos_sortie > 0){
            if(write(1, buffer_sortie, pos_sortie) < 0){
                mini_perror("Erreur écriture sur la sortie standard");
                mini_exit();
            }
            pos_sortie = 0;
        }
    }

    mini_free(buffer);
    mini_fclose(f);
    mini_exit();
}
