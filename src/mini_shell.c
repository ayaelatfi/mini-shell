#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

int main() {
    char cmd[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    char *token;
    int status;

    while (1) {
        // Affichage du prompt
        printf("mysh> ");
        fflush(stdout);

        // Lecture de la ligne de commande
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            break;  // EOF ou erreur
        }

        // Retirer le saut de ligne
        cmd[strcspn(cmd, "\n")] = '\0';

        // Quitter si l'utilisateur tape "exit"
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        // Découper la commande en arguments
        int i = 0;
        token = strtok(cmd, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // Terminer la liste d’arguments

        // Créer un processus enfant
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            // Code du processus enfant
            execvp(args[0], args);
            perror("execvp");  // Si execvp échoue
            exit(EXIT_FAILURE);
        } else {
            // Code du processus parent
            waitpid(pid, &status, 0);
        }
    }

    printf("Bye!\n");
    return 0;
}

