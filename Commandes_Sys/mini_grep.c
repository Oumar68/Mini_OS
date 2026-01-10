#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../mini_lib.h"

#define SORTIE_SIZE 4096 



int main(int argc, char *argv[]) {
    if (argc != 3) {
        mini_printf("Usage: mini_grep <mot> <fichier>\n");
        mini_exit();
    }

    MYFILE *f = mini_open(argv[2], 'r');
    if (!f) {
        mini_perror("Erreur ouverture");
        mini_exit();
    }

    char *buffer_lecture = mini_calloc(sizeof(char), IOBUFFER_SIZE);
    char *ligne_temp = mini_calloc(sizeof(char), 2048); // Pour reconstruire la ligne
    char *buffer_sortie = mini_calloc(sizeof(char), SORTIE_SIZE);
    
    int nb_lues;
    int pos_ligne = 0;
    int pos_sortie = 0;

    while ((nb_lues = mini_fread(buffer_lecture, 1, IOBUFFER_SIZE, f)) > 0) {
        for (int i = 0; i < nb_lues; i++) {
            char c = buffer_lecture[i];
            ligne_temp[pos_ligne++] = c;

            // Si on arrive à la fin d'une ligne ou si la ligne est trop longue
            if (c == '\n' || pos_ligne >= 2047) {
                ligne_temp[pos_ligne] = '\0'; 

                if (find_word(ligne_temp, argv[1])) {
                    // Si la ligne ne rentre pas dans le tampon de sortie, on vide d'abord
                    if (pos_sortie + pos_ligne >= SORTIE_SIZE) {
                        write(1, buffer_sortie, pos_sortie);
                        pos_sortie = 0;
                    }
                    
                    // On copie la ligne dans le tampon de sortie
                    for(int k = 0; k < pos_ligne; k++) {
                        buffer_sortie[pos_sortie++] = ligne_temp[k];
                    }
                }
                pos_ligne = 0;
            }
        }
    }

    // Vider le tampon de sortie à la fin s'il contient quelque chose
    if (pos_sortie > 0) {
        write(1, buffer_sortie, pos_sortie);
    }

    // Nettoyage final 
    mini_free(buffer_lecture);
    mini_free(ligne_temp);
    mini_free(buffer_sortie);
    mini_fclose(f);
    
    return 0;
}