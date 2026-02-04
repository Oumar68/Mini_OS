#include <unistd.h>
#include "../mini_lib.h"

extern char **environ;

int main(int argc, char **argv) {
    if (argc != 2) {
        write(2, "Usage: mini_export VAR=VAL\n", 27);
        mini_exit();
    }

    // 1. Compter le nombre de variables actuelles
    int count = 0;
    while (environ[count] != NULL) {
        count++;
    }

    // 2. Allouer un nouveau tableau de pointeurs
    char **new_env = (char **)mini_calloc(count + 2, sizeof(char *));
    if (!new_env) {
        mini_perror("Erreur allocation");
        mini_exit();
    }

    // 3. Copier les anciens pointeurs dans le nouveau tableau
    for (int i = 0; i < count; i++) {
        new_env[i] = environ[i];
    }

    // 4. Ajouter la nouvelle variable (provenant de argv[1])
    char *new_var = (char *)mini_calloc(mini_strlen(argv[1]) + 1, sizeof(char));
    mini_strcpy(new_var, argv[1],mini_strlen(argv[1])+1); 
    
    new_env[count] = new_var;
    // 5. Terminer par NULL
    new_env[count + 1] = NULL;

    environ = new_env;
    
    mini_exit();
   
}