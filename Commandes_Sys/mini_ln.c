#include <unistd.h>
#include <stdio.h>
#include "../mini_lib.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        mini_printf("il faut 2 argument <nom fichier source> <nom fichier destination");
        mini_exit();
    }

    // Appel système symlink
    if (symlink(argv[1], argv[2]) == -1) {
        mini_perror("Erreur lors de la création du lien");
        mini_exit();
    }

    mini_exit();
}