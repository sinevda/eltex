#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t fileAcc = 1;
static const char FILENAME[] = "numbers";

void sig1(int);
void sig2(int);

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

    if (signal(SIGUSR1, sig1) == SIG_ERR)
    {
        perror("Ошибка настройки сигнала");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGUSR2, sig2) == SIG_ERR)
    {
        perror("Ошибка настройки сигнала");
        exit(EXIT_FAILURE);        
    }

    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        perror("Ошибка разделения процесса");
        exit(EXIT_FAILURE);

    case 0:
        if(close(pipefd[0]) == -1) perror("Ошибка закрытия канала");
        for(int i = 0; i < atoi(argv[1]); ++i)
        {
            while (fileAcc) sleep(0);

            FILE* file = fopen(FILENAME, "r");
            if (file == NULL)
            {
                perror("Ошибка открытия файла");
                exit(EXIT_FAILURE);
            }

            printf("\n--------------------\n");
            int n;
            while (fscanf(file, "%d", &n) != EOF || fileAcc)
                printf("Прочитано дочерним процессом: %d\n", n);

            fclose(file);
            printf("--------------------\n");

            int num = rand();
            write(pipefd[1], &num, sizeof(num));
            sleep(0);
        }
        if(close(pipefd[1]) == -1) perror("Ошибка закрытия канала");
        exit(EXIT_SUCCESS);
    
    default:
        {
            sleep(0);

            if(close(pipefd[1]) == -1) perror("Ошибка закрытия канала");

            int num;
            for(int i = 0; i < atoi(argv[1]); ++i)
            {
                if (kill(pid, SIGUSR1))
                {
                    perror("Ошибка отправки сигнала");
                    exit(EXIT_FAILURE);
                }

                FILE* file = fopen(FILENAME, "a");
                if(file == NULL)
                {
                    perror("Ошибка открытия файла");
                    exit(EXIT_FAILURE);
                }

                read(pipefd[0], &num, sizeof(num));
                fprintf(file, "%d\n", num);

                fclose(file);

                if (kill(pid, SIGUSR2))
                {
                    perror("Ошибка отправки сигнала");
                    exit(EXIT_FAILURE);
                }
                sleep(0);
            }
            if(close(pipefd[0]) == -1) perror("Ошибка закрытия канала");
            
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

void sig1(int signum)
{
    fileAcc = 0;
    return;
}

void sig2(int signum)
{
    fileAcc = 1;
    return;
}