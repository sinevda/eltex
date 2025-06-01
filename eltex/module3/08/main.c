#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

union semun {
int val; /* значение для SETVAL */
struct semid_ds *buf; /* буферы для IPC_STAT, IPC_SET */
unsigned short *array; /* массивы для GETALL, SETALL */
/* часть, особенная для Linux: */
struct seminfo *__buf; /* буфер для IPC_INFO */
};

int semid;
static const char FILENAME[] = "numbers";

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        perror("Неверное количество аргументов запуска");
        exit(EXIT_FAILURE);
    }

    key_t key = ftok("/etc/fstab", getpid());
    if (key  == -1)
    {
        perror("Ошибка преобразования в key_t");
        exit(EXIT_FAILURE);
    }

    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("Ошибка создания семафора");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("Ошибка инициализации семафора");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    struct sembuf lock_res =    {0, -1, 0};
    struct sembuf rel_res[] = { {0, 0, 0},
                                {0, 1, 0}};

    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        perror("Ошибка разделения процесса");
        exit(EXIT_FAILURE);

    case 0:
        for(int i = 0; i < atoi(argv[1]); ++i)
        {
            if (semop(semid, &lock_res, 1) == -1){ perror("semop:lock_res"); }

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
            semop(semid, rel_res, 2);
            sleep(0);
        }
        exit(EXIT_SUCCESS);
    
    default:
        {
            sleep(0);

            int num;
            for(int i = 0; i < atoi(argv[1]); ++i)
            {
                if (semop(semid, &lock_res, 1) == -1){ perror("semop:lock_res"); }

                FILE* file = fopen(FILENAME, "a");
                if(file == NULL)
                {
                    perror("Ошибка открытия файла");
                    exit(EXIT_FAILURE);
                }

                read(pipefd[0], &num, sizeof(num));
                fprintf(file, "%d\n", num);

                fclose(file);

                semop(semid, rel_res, 2);
                sleep(0);
            }
            wait(NULL);

            semctl(semid, 0, IPC_RMID);

            exit(EXIT_SUCCESS);
        }
    }
}