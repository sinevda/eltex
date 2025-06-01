#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

#define PAGE_SIZE SHMLBA

volatile sig_atomic_t running = 1;

void sigfunc(int signum) {running = 0; return;}

int main()
{
    struct sigaction sa;

    sa.sa_handler = *sigfunc;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);

    key_t semkey = ftok("sem", 'a');
    if (semkey == -1)
    {
        perror("Ошибка получения ключа IPC для семафора");
        exit(EXIT_FAILURE);
    }

    key_t shmkey = ftok("shm", 'a');
    if (shmkey == -1)
    {
        perror("Ошибка получения ключа IPC для разделяемой памяти");
        exit(EXIT_FAILURE);
    }

    int semid = semget(semkey, 1, IPC_CREAT | 0660);
    if(semid == -1)
    {
        perror("Ошибка создания семафора");
        exit(EXIT_FAILURE);
    }
    
    if(semctl(semid, 0, SETVAL, 1) == -1)
    {
        perror("Ошибка задания здачения семафора");
        exit(EXIT_FAILURE);
    }

    struct sembuf lock =    {0, -1, 0};
    struct sembuf rel[] =  {{0, 0, 0},
                            {0, 1, 0}};

    int shmid = shmget(shmkey, PAGE_SIZE, IPC_CREAT | 0660);
    if(shmid == -1)
    {
        perror("Ошибка создания сегмента разделяемой памяти");
        exit(EXIT_FAILURE);
    }

    int* shm = (int*)shmat(shmid, NULL, 0);
    if(*shm == -1)
    {
        perror("Ошибка подключения разделяемой памяти");
        exit(EXIT_FAILURE);
    }

    int maxcount = PAGE_SIZE / sizeof(int) - 1;

    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    
    case 0:
    {
        while(1)
        {
            if(!running) exit(EXIT_SUCCESS);

            sleep(0);

            if (semop(semid, &lock, 1) == -1)
            {
                perror("0:semop:lock(1)");
                break;
            }

            int min = INT32_MAX , max = INT32_MIN;


            int count = shm[0];
            for(int i = 1; i < count; ++i)
            {
                int temp = shm[i];
                if (temp > max) max = temp;
                if (temp < min) min = temp;
            }

            shm[0] = max;
            shm[1] = min;

            if (semop(semid, rel, 2) == -1)
            {
                perror("0:semop:rel(2)");
                break;
            }

            if(!running) exit(EXIT_SUCCESS);

            sleep(1);
        }
    }

    default:
    {
        int packets = 0;

        while(1)
        {
            if(!running) break;

            if (semop(semid, &lock, 1) == -1)
            {
                perror("semop:lock(1)");
                break;
            }

            int count = rand() % maxcount;
            shm[0] = count;
            for(int i = 1; i <= count; ++i)
            {
                shm[i] = rand();
            }

            if (semop(semid, rel, 2) == -1)
            {
                perror("semop:rel(2)");
                break;
            }

            sleep(1);

            if(!running) break;

            if (semop(semid, &lock, 1) == -1)
            {
                perror("semop:lock(1)");
                break;
            }

            int max = shm[0];
            int min = shm[1];
            printf("count: %d, min: %d, max: %d\n", count, min, max);

            if (semop(semid, rel, 2) == -1)
            {
                perror("semop:rel(2)");
                break;
            }
            ++packets;


            if(!running) break;
        }
        printf(" Выход...\nОбработано наборов: %d\n", packets);
        break;
    }
    }

    if(shmdt(shm))
    {

    }
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 1, IPC_RMID);
    exit(EXIT_SUCCESS);
}