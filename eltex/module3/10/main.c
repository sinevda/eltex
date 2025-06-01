#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

int semid;
static const char FILENAME[] = "numbers";

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        perror("Неверное количество аргументов запуска");
        exit(EXIT_FAILURE);
    }

    sem_unlink("semaphore");
    sem_t* sem = sem_open("semaphore", O_CREAT, S_IRWXU | S_IRWXG, 1);
    if (sem == SEM_FAILED)
    {
        perror("sem_open");
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
        for(int i = 0; i < atoi(argv[1]); ++i)
        {
            if (sem_wait(sem)) perror("sem_wait");
            
            FILE* file = fopen(FILENAME, "r");
            if (file == NULL)
            {
                perror("Ошибка открытия файла");
                exit(EXIT_FAILURE);
            }

            printf("\n--------------------\n");
            int n;
            while (fscanf(file, "%d", &n) != EOF )
                printf("Прочитано дочерним процессом: %d\n", n);

            fclose(file);
            printf("--------------------\n");

            int num = rand();
            write(pipefd[1], &num, sizeof(num));
            
            int val;
            sem_getvalue(sem, &val);
            if(val >= 1)
            {
                perror("Ошибка в работе семафора");
                exit(EXIT_FAILURE);
            }
            if (sem_post(sem)) perror("sem_post");
            
            sleep(0);
        }
        exit(EXIT_SUCCESS);
    
    default:
        {
            sleep(0);
            int num;
            
            if (sem_wait(sem)) perror("sem_wait");

            FILE* file = fopen(FILENAME, "a");
            if(file == NULL)
            {
                perror("Ошибка открытия файла");
                exit(EXIT_FAILURE);
            }

            read(pipefd[0], &num, sizeof(num));
            fprintf(file, "%d\n", num);

            fclose(file);

  
            if (sem_post(sem)) perror("sem_post");

            sleep(0);
        }
        wait(NULL);

        sem_close(sem);
        sem_unlink("semaphore");

        exit(EXIT_SUCCESS);
    }
}