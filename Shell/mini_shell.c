#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdint.h>
#include "mini_lib.h"

extern char **environ;
// Fonction pour découper une ligne en arguments
int mini_split(char *line, char **args) {
    int count = 0;
    int i = 0;
    int in_word = 0;

    while (line[i] != '\0') {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
            line[i] = '\0'; // On coupe la chaîne
            in_word = 0;
        } else {
            if (in_word == 0) {
                args[count] = &line[i]; // On mémorise le début du mot
                count++;
                in_word = 1;
            }
        }
        i++;
    }
    args[count] = NULL; // Toujours terminer par NULL pour execve
    return count;
}

char* mini_getenv(char *name) {
    int i = 0;
    int name_len = mini_strlen(name);

    while (environ[i] != NULL) {
        // On compare 'name' avec le début de la ligne environ[i]
        int j = 0;
        while (environ[i][j] == name[j] && j < name_len) {
            j++;
        }

        // Si on a parcouru tout 'name' et qu'on tombe sur '=' dans environ[i]
        if (j == name_len && environ[i][j] == '=') {
            // On renvoie le pointeur juste après le '='
            return &environ[i][j + 1];
        }
        i++;
    }
    return NULL; 
}

// --- FONCTIONS INTERNES (BUILT-INS) ---

// 1. mini_env : Affiche tout l'environnement
void builtin_env() {
    int i = 0;
    char **envp = environ;
    while (envp[i] != NULL) {
        if ((uintptr_t)environ[i] < 4096) break;
        mini_printf(envp[i]);
        mini_printf("\n");
        i++;
    }
}

// 2. mini_getenv : Affiche la valeur d'une variable spécifique
void builtin_getenv(char **args) {
    if (args[1] == NULL) {
        write(2, "Usage: mini_getenv <VAR>\n", 25);
        return;
    }
    char *val = mini_getenv(args[1]); // Ta fonction que nous avons écrite avant
    if (val) {
        mini_printf(val);
        mini_printf("\n");
    } else {
        mini_printf("Variable non trouvée\n");
    }
}

// 3. mini_cd : Change le répertoire de travail
void builtin_cd(char **args) {
    if (args[1] == NULL) {
        write(2, "Usage: mini_cd <PATH>\n", 22);
        return;
    }
    if (chdir(args[1]) == -1) {
        mini_perror("mini_cd");
    }
}

// 4. mini_export : Ajoute ou modifie une variable d'environnement
void builtin_export(char **args) {
    if (args[1] == NULL) {
        write(2, "Usage: mini_export VAR=VAL\n", 27);
        return;
    }
    
    // On compte l'actuel
    int count = 0;
    while (environ[count] != NULL) count++;

    // On alloue un nouveau tableau (count + 2 pour le nouveau et le NULL)
    char **new_env = (char **)mini_calloc(count + 2, sizeof(char *));
    
    for (int i = 0; i < count; i++) {
        new_env[i] = environ[i];
    }
    // 4. Créer une copie persistante de la chaîne VAR=VAL
    int len = mini_strlen(args[1]);
    char *copie_persistante = (char *)mini_calloc(len + 1, sizeof(char));
    mini_strcpy(args[1],copie_persistante, len + 1);

    // 5. Ajouter la copie au tableau
    new_env[count] = copie_persistante;
    new_env[count + 1] = NULL;

    // 6. Basculer l'environnement
    environ = new_env; 
}

// --- LOGIQUE D'EXECUTION ---

void execute_command(char *line) {
    int arriere_plan = 0;
    char **args = (char **)mini_calloc(11, sizeof(char *));
    //char *args[11];
    int num_args = mini_split(line, args);

    if (num_args == 0) {
        mini_free(args);
        return;
    }


     //--- 1 Routage vers les fonctions internes ---
    if (mini_strcmp(args[0], "mini_env") == 0) {
        builtin_env();
        return;
    } 
    else if (mini_strcmp(args[0], "mini_getenv") == 0) {
        builtin_getenv(args);
        
        return;
    } 
    else if (mini_strcmp(args[0], "mini_cd") == 0) {
        builtin_cd(args);
        return;
    } 
    else if (mini_strcmp(args[0], "mini_export") == 0) {
        builtin_export(args);
        return;
    } 
    // --- 2. Détection du & ---
    // On vérifie si le dernier argument est "&"
    if (num_args > 0 && mini_strcmp(args[num_args - 1], "&") == 0) {
        arriere_plan = 1;
        args[num_args - 1] = NULL; // On l'enlève pour ne pas le passer à execve
    }

    // --- 3. Exécution (Interne ou Externe) ---
    pid_t pid = fork();
    if (pid == -1) {
        mini_perror("fork");
    } else if (pid == 0) {
        // Dans le fils
        char path[512] = "/home/ewann/Documents/INSA STI 3A/Prog_systeme/TP1/Commandes_Sys/";
        mini_strcat(path, args[0]);
        
        if (execve(path, args, NULL) == -1) {
            mini_perror("Commande introuvable");
            _exit(1);
        }
    } else {
        // Dans le parent
        if (arriere_plan) {
            // On n'attend pas ! On affiche juste une info
            mini_printf("[Processus lancé en arrière-plan]\n");
        } else {
            // Mode normal : on attend la fin du fils
            waitpid(pid, NULL, 0); 
        }
    }

    mini_free(args);
}

int main() {
    char *buffer = (char *)mini_calloc(1024, sizeof(char));
    //char buffer[1024];
    while (1) {
        // Affichage du prompt
        write(STDOUT_FILENO, "ewann@mini_shell> ", 18);
        // Lecture de l'entrée standard
        int n = read(STDIN_FILENO, buffer, 1023);
        if (n <= 0) break;
        buffer[n] = '\0';
        if (mini_strcmp(buffer, "exit\n") == 0) {
            break;
        }
        execute_command(buffer);
        mini_memset(buffer, 0, 1024);
    }
    
    mini_free(buffer);
    mini_exit(); // Ton propre exit
    return 0;
}