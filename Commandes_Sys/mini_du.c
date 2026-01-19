#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "../mini_lib.h"

#define MAX_INT_STR 32

// Convertit un entier en chaîne (Version propre)
void int_to_str(long n, char *buf) {
    char tmp[MAX_INT_STR];
    int i = 0, j = 0;
    if (n == 0) { buf[j++] = '0'; buf[j] = '\0'; return; }
    if (n < 0) { buf[j++] = '-'; n = -n; }
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    for (int k = i - 1; k >= 0; k--) buf[j++] = tmp[k];
    buf[j] = '\0';
}

// Affiche une chaîne
void print_str(const char *s) {
    if (!s) return;
    int len = 0;
    while (s[len] != '\0') len++;
    write(1, s, len);
}

int main(int argc, char **argv) {
    char base_path[256];
    for (int i = 0; i < 256; i++) base_path[i] = '\0';

    // Gestion de l'argument ou du dossier courant
    if (argc < 2) {
        base_path[0] = '.';
    } else {
        int i = 0;
        while (argv[1][i] != '\0' && i < 254) {
            base_path[i] = argv[1][i];
            i++;
        }
    }

    DIR *d = opendir(base_path);
    if (!d) {
        mini_perror("Erreur opendir");
        return 1;
    }

    struct dirent *entree;
    char chemin_complet[1024];
    size_t taille = 0;

    while ((entree = readdir(d)) != NULL) {
        // Ignorer les fichiers cachés . et ..
        if (entree->d_name[0] == '.') continue;

        // Reset du buffer de chemin
        for (int i = 0; i < 1024; i++) chemin_complet[i] = '\0';

        // Construction manuelle sécurisée du chemin (base_path + / + d_name)
        int p = 0;
        while (base_path[p] != '\0') {
            chemin_complet[p] = base_path[p];
            p++;
        }
        if (p > 0 && chemin_complet[p-1] != '/') {
            chemin_complet[p++] = '/';
        }
        int f = 0;
        while (entree->d_name[f] != '\0') {
            chemin_complet[p++] = entree->d_name[f++];
        }
        struct stat info;
        if (stat(chemin_complet, &info) == -1) return 1;
        taille += info.st_size;

    }
    char *buffer = (char*) mini_calloc(1,MAX_INT_STR);
    int_to_str(taille, buffer);
    mini_printf("Taille totale des fichiers dans le repertoire : ");
    //print_str(buffer);
    mini_printf(buffer);
    mini_printf(" octets\n");
    mini_free(buffer);

    closedir(d);
    return 0;
}