#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define MAXPROCESSTOREAD 3

union semun
{
    int             val;
    struct          semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
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
    arg.val = MAXPROCESSTOREAD;
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

    struct sembuf lock_read =   {0, -1, 0};
    struct sembuf rel_read =    {0, 1, 0};
    struct sembuf lock_write =  {0, -MAXPROCESSTOREAD, 0};
    struct sembuf rel_write[] = {{0, 0, 0},
                                {0, MAXPROCESSTOREAD, 0}};

    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        perror("Ошибка разделения процесса");
        exit(EXIT_FAILURE);

    case 0:
        for(int i = 0; i < atoi(argv[1]); ++i)
        {
            if (semop(semid, &lock_read, 1) == -1) perror("semop:lock_read");

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
            
            if (semctl(semid, 0, GETVAL) >= MAXPROCESSTOREAD)
            {
                perror("Ошибка в работе семафора");
                exit(EXIT_FAILURE);
            }
            if (semop(semid, &rel_read, 1) == -1) perror("semop:rel_read");
            
            sleep(0);
        }
        exit(EXIT_SUCCESS);
    
    default:
        {
            sleep(0);

            int num;
            for(int i = 0; i < atoi(argv[1]); ++i)
            {
                if (semop(semid, &lock_write, 1) == -1) perror("semop:lock_write");

                FILE* file = fopen(FILENAME, "a");
                if(file == NULL)
                {
                    perror("Ошибка открытия файла");
                    exit(EXIT_FAILURE);
                }

                read(pipefd[0], &num, sizeof(num));
                fprintf(file, "%d\n", num);

                fclose(file);

                if (semop(semid, rel_write, 2) == -1) perror("semop:rel_write");

                sleep(0);
            }
            wait(NULL);

            semctl(semid, 0, IPC_RMID);

            exit(EXIT_SUCCESS);
        }
    }
}