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

// Affiche les détails d'un fichier (ls -l style)
void afficher_details(const char *nom, const char *chemin_complet) {
    struct stat info;
    if (stat(chemin_complet, &info) == -1) return;

    char buf[MAX_INT_STR];

    // 1. Permissions et Type
    print_str((S_ISDIR(info.st_mode)) ? "d" : "-");
    print_str((info.st_mode & S_IRUSR) ? "r" : "-");
    print_str((info.st_mode & S_IWUSR) ? "w" : "-");
    print_str((info.st_mode & S_IXUSR) ? "x" : "-");
    print_str((info.st_mode & S_IRGRP) ? "r" : "-");
    print_str((info.st_mode & S_IWGRP) ? "w" : "-");
    print_str((info.st_mode & S_IXGRP) ? "x" : "-");
    print_str((info.st_mode & S_IROTH) ? "r" : "-");
    print_str((info.st_mode & S_IWOTH) ? "w" : "-");
    print_str((info.st_mode & S_IXOTH) ? "x" : "-");
    print_str(" ");

    // 2. Nb liens
    int_to_str(info.st_nlink, buf);
    print_str(buf); print_str(" ");

    // 3. Propriétaire et Groupe
    struct passwd *pw = getpwuid(info.st_uid);
    struct group *gr = getgrgid(info.st_gid);
    print_str(pw ? pw->pw_name : "unknown"); print_str(" ");
    print_str(gr ? gr->gr_name : "unknown"); print_str(" ");

    // 4. Taille
    int_to_str(info.st_size, buf);
    print_str(buf); print_str(" ");

    // 5. Date (Formatée)
    char *date = ctime(&info.st_mtime);
    if (date) {
        date[16] = '\0'; // On coupe l'année et les secondes
        print_str(date + 4); // On saute le jour de la semaine
        print_str(" ");
    }

    // 6. Nom du fichier
    print_str(nom);
    print_str("\n");
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

        // On affiche les détails avec le nom simple et le chemin complet
        afficher_details(entree->d_name, chemin_complet);
    }

    closedir(d);
    return 0;
}