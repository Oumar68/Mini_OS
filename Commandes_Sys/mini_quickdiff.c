#include <stddef.h> // Pour size_t
#include "../mini_lib.h"

#define MAX_LINE 1024

// Fonction pour lire une ligne caractère par caractère
int mini_getline(char *buffer, MYFILE *file) {
    int i = 0;
    char c;
    while (i < MAX_LINE - 1) {
        if (mini_fread(&c, 1, 1, file) <= 0) break;
        buffer[i++] = c;
        if (c == '\n') break;
    }
    buffer[i] = '\0';
    return i; // Retourne le nombre de caractères lus
}

int compare_lignes(char *s1, char *s2) {
    int i = 0;
    while (s1[i] != '\0' && s1[i] == s2[i]) i++;
    return (unsigned char)s1[i] - (unsigned char)s2[i];
}

void quickdiff(char *path1, char *path2) {
    MYFILE *f1 = mini_open(path1, 'r');
    MYFILE *f2 = mini_open(path2, 'r');

    if (!f1 || !f2) {
        mini_perror("Erreur ouverture fichiers");
        return;
    }

    char line1[MAX_LINE];
    char line2[MAX_LINE];
    int num_ligne = 1;

    while (1) {
        // Lecture d'une ligne dans chaque fichier
        int r1 = mini_getline(line1, f1);
        int r2 = mini_getline(line2, f2);

        if (r1 == 0 && r2 == 0) break; // Fin des deux fichiers

        if (compare_lignes(line1, line2) != 0) {
            mini_printf("Différence ligne ");
            // On peut afficher le numéro de ligne ici si on veut
            mini_printf("\n< ");
            mini_printf(line1);
            mini_printf("--- \n> ");
            mini_printf(line2);
            mini_printf("\n");
        }
        num_ligne++;
    }

    mini_fclose(f1);
    mini_fclose(f2);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        mini_printf("Usage: mini_quickdiff <fichier1> <fichier2>\n");
        mini_exit();
    }
    quickdiff(argv[1], argv[2]);
    mini_exit();
}