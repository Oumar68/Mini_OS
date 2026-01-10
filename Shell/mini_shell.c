#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "mini_lib.h"

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

void execute_command(char *line) {
    // Allocation du tableau d'arguments (ex: max 10 arguments)
    char **args = (char **)mini_calloc(11, sizeof(char *));
    
    // On découpe la ligne en arguments
    int num_args = mini_split(line, args);

    if (num_args == 0) {
        mini_free(args);
        return;
    }
     // Préparation du chemin complet pour le binaire (args[0])
        char *full_path = (char *)mini_calloc(512, sizeof(char));
        mini_strcat(full_path, "/home/ewann/Documents/INSA STI 3A/Prog_systeme/TP1/Commandes_Sys/");
        mini_strcat(full_path, args[0]);

        // On remplace le premier argument par le chemin complet
        args[0] = full_path;
    // Gestion des commandes internes 
        // Cas du mini_env 
        if (mini_strcmp(args[0], "mini_env") == 0) {
            if(execve(full_path, args , NULL ) == -1){
                mini_perror("Erreur mini_shell");
                mini_free(full_path);
                mini_free(args);
                return;
            }
        }
        // Cas du mini_getenv
        else if (mini_strcmp(args[0], "mini_getenv") == 0) {
            if(execve(full_path, args , NULL ) == -1){
                mini_perror("Erreur mini_shell");
                mini_free(full_path);
                mini_free(args);
                return;
            }
        }
        // Cas du mini_cd
        else if (mini_strcmp(args[0], "mini_cd") == 0) {
            if(execve(full_path, args , NULL ) == -1){
                mini_perror("Erreur mini_shell");
                mini_free(full_path);
                mini_free(args);
                return;
            }
        }
        // Cas du mini_export 
        else if (mini_strcmp(args[0], "mini_export") == 0) {
            if(execve(full_path, args , NULL ) == -1){
                mini_perror("Erreur mini_shell");
                mini_free(full_path);
                mini_free(args);
                return;
            }
        }

    pid_t pid = fork();
    if (pid == 0) {
       

        if (execve(full_path, args, NULL) == -1) {
            mini_perror("Erreur mini_shell");
            _exit(1);
        }
        else{
            _exit(0);
        }
    } else {
        int status;
        wait(&status);
    }

   
    mini_free(args);
}
int main() {
    char *buffer = (char *)mini_calloc(1024, sizeof(char));

    while (1) {
        // Affichage du prompt
        write(STDOUT_FILENO, "ewann@mini_shell> ", 18);
        // Lecture de l'entrée standard
        int n = read(STDIN_FILENO, buffer, 1024);
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