#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    char input[100];
    pthread_t thread;

    while(1)
    {
        printf(">> ");
        fgets(input, 100, stdin);
        input[strcspn(input, "\n")] = 0;

        if(strcmp(input, "exit") == 0)
            exit(EXIT_SUCCESS);

        
        char to[100];
        strcpy(to, input);
        pid_t pid;
        int status;
        switch (pid = fork())
        {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
            break;

        case 0:
        {
            int i = 0;
            char* args[10];
            char* curr = strtok((char*)input, " ");
            while(curr != NULL) 
            {
                args[i++] = curr;
                curr = strtok(NULL, " ");
            }
            args[i] = NULL;
            execvp(args[0], args);

            perror("Ошибка выполнения команды");
            exit(EXIT_FAILURE);
            break;
        }
        default:
        {
            int status;
            wait(&status);
            if(!WIFEXITED(status))
            {
                perror("Ошибка завершения дочернего процесса(функции)");
                exit(EXIT_FAILURE);
            }
        }
        
        }   
    }
}