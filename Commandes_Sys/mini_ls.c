#include <sys/stat.h>
#include <unistd.h>
#include "../mini_lib.h"

#define LTEMPS 32
#define MAX_INT_STR 32

// Convertit un entier en chaîne
void int_to_str(long n, char *buf) {
    char tmp[MAX_INT_STR];
    int i = 0;
    int negatif = 0;

    if (n < 0) {
        negatif = 1;
        n = -n;
    }

    if (n == 0) tmp[i++] = '0';
    else {
        while (n > 0) {
            tmp[i++] = '0' + (n % 10);
            n /= 10;
        }
    }

    int j = 0;
    if (negatif) buf[j++] = '-';

    for (int k = i - 1; k >= 0; k--) buf[j++] = tmp[k];
    buf[j] = '\0';
}

// Affiche une chaîne immédiatement
void print_str(const char *s) {
    int len = 0;
    while (s[len] != '\0') len++;
    if (write(1, s, len) < 0) {
        mini_perror("Erreur écriture sortie standard");
        mini_exit();
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_str("Usage: mini_ls -l <fichier>\n");
        mini_exit();
    }

    struct stat info;
    if (stat(argv[1], &info) == -1) {
        mini_perror("Erreur stat fichier");
        mini_exit();
    }

    char buf[MAX_INT_STR];

    int_to_str(info.st_nlink, buf);
    print_str("Nb liens: "); print_str(buf); print_str("\n");

    int_to_str(info.st_uid, buf);
    print_str("uid: "); print_str(buf); print_str("\n");

    int_to_str(info.st_gid, buf);
    print_str("gid: "); print_str(buf); print_str("\n");

    int_to_str(info.st_size, buf);
    print_str("taille fichier: "); print_str(buf); print_str("\n");

    int_to_str(info.st_mtime, buf);
    print_str("Date dernière modification (timestamp): "); print_str(buf); print_str("\n");

    print_str("Droits utilisateur: ");
    print_str((info.st_mode & S_IRUSR) ? "r" : "-");
    print_str((info.st_mode & S_IWUSR) ? "w" : "-");
    print_str((info.st_mode & S_IXUSR) ? "x" : "-");
    print_str("\n");

    print_str("Droits groupe: ");
    print_str((info.st_mode & S_IRGRP) ? "r" : "-");
    print_str((info.st_mode & S_IWGRP) ? "w" : "-");
    print_str((info.st_mode & S_IXGRP) ? "x" : "-");
    print_str("\n");

    print_str("Droits autres: ");
    print_str((info.st_mode & S_IROTH) ? "r" : "-");
    print_str((info.st_mode & S_IWOTH) ? "w" : "-");
    print_str((info.st_mode & S_IXOTH) ? "x" : "-");
    print_str("\n");

    mini_exit();
}
