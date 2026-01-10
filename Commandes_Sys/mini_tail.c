#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../mini_lib.h"

//#define IOBUFFER_SIZE 512

int main(int argc, char *argv[]) {
    if (argc < 3) {
        mini_perror("Usage : mini_tail <nombre_de_lignes> <nom_fichier>\n");
        mini_exit();
    }

    int n = atoi(argv[1]);
    MYFILE *f = mini_open(argv[2], 'r');
    if (!f) {
        mini_perror("Erreur ouverture fichier");
        mini_exit();
    }

    // Aller à la fin du fichier
    off_t taille = lseek(f->fd, 0, SEEK_END);
    if (taille == -1) {
        mini_perror("Erreur lseek");
        mini_exit();
    }

    char buffer[IOBUFFER_SIZE];
    off_t position = taille;
    int nb_lignes_trouvees = 0;
    off_t debut_lecture = 0;

    // Lire à rebours
    while (position > 0 && nb_lignes_trouvees <= n) {
        int bloc = (position >= IOBUFFER_SIZE) ? IOBUFFER_SIZE : position;
        position -= bloc;
        lseek(f->fd, position, SEEK_SET);
        int lus = read(f->fd, buffer, bloc);
        if (lus < 0) {
            mini_perror("Erreur lecture fichier");
            mini_exit();
        }

        // Parcourir le bloc à l'envers pour compter les lignes
        for (int i = lus - 1; i >= 0; i--) {
            if (buffer[i] == '\n') {
                nb_lignes_trouvees++;
                if (nb_lignes_trouvees > n) {
                    debut_lecture = position + i + 1; // position de la n-ième dernière ligne
                    break;
                }
            }
        }
    }

    // Aller à la position de début de lecture
    lseek(f->fd, debut_lecture, SEEK_SET);

    // Afficher le reste du fichier par blocs
    int lus;
    while ((lus = read(f->fd, buffer, IOBUFFER_SIZE)) > 0) {
        if (write(1, buffer, lus) < 0) {
            mini_perror("Erreur écriture sortie standard");
            mini_exit();
        }
    }

    mini_fclose(f);
    mini_exit();
}
