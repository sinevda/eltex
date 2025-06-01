#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        perror("Неверное количество аргументов запуска");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        perror("Ошибка разделения процесса");
        exit(EXIT_FAILURE);

    case 0:
        if (!close(pipefd[0]))
        {
            perror("Ошибка закрытия канала");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < atoi(argv[1]); ++i)
        {
            int num = rand();
            write(pipefd[1], &num, sizeof(num));
        }
        
        if (!close(pipefd[1]))
        {
            perror("Ошибка закрытия канала");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    
    default:
        {
            FILE* file = fopen("numbers", "w");
            if(file == NULL)
            {
                perror("Ошибка открытия файла");
                exit(EXIT_FAILURE);
            }
            if (!close(pipefd[1]))
            {
                perror("Ошибка закрытия канала");
                exit(EXIT_FAILURE);
            }

            int num;
            for(int i = 0; i < atoi(argv[1]); ++i)
            {
                read(pipefd[0], &num, sizeof(num));
                fprintf(file, "%d\n", num);
            }

            if (!close(pipefd[0]))
            {
                perror("Ошибка закрытия канала");
                exit(EXIT_FAILURE);
            }

            int status;
            wait(&status);
            if(!WIFEXITED(status))
            {
                perror("Ошибка завершения дочернего процесса");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
    }
}