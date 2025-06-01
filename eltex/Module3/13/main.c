#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>

#define MAXMEM sizeof(int) * 10000

volatile sig_atomic_t running = 1;

void sigfunc(int signum) {running = 0; return;}

int main()
{
    struct sigaction sa;

    sa.sa_handler = *sigfunc;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);

    sem_unlink("semaphore");
    shm_unlink("sharedmem");

    sem_t* sem = sem_open("semaphore", O_CREAT, S_IRWXU | S_IRWXG, 1);
    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    int shmid = shm_open("sharedmem", O_CREAT, S_IRWXU | S_IRWXG);
    int* shm = mmap(NULL, MAXMEM, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, shmid, 0);
    if(shm == MAP_FAILED)
    {
        perror("Ошибка подключения сегмента памяти");
        exit(EXIT_FAILURE);
    }

    int maxcount = MAXMEM / sizeof(int) - 1;

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

            if (sem_wait(sem)) perror("sem_wait");

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

            int val;
            sem_getvalue(sem, &val);
            if(val >= 1)
            {
                perror("Ошибка в работе семафора");
                exit(EXIT_FAILURE);
            }
            if (sem_post(sem)) perror("sem_post");

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

            if (sem_wait(sem)) perror("sem_wait");

            int count = rand() % maxcount;
            shm[0] = count;
            for(int i = 1; i <= count; ++i)
            {
                shm[i] = rand();
            }

            int val;
            sem_getvalue(sem, &val);
            if(val >= 1)
            {
                perror("Ошибка в работе семафора");
                exit(EXIT_FAILURE);
            }
            if (sem_post(sem)) perror("sem_post");

            sleep(1);

            if(!running) break;

            if (sem_wait(sem)) perror("sem_wait");

            int max = shm[0];
            int min = shm[1];
            printf("count: %d, min: %d, max: %d\n", count, min, max);

            sem_getvalue(sem, &val);
            if(val >= 1)
            {
                perror("Ошибка в работе семафора");
                exit(EXIT_FAILURE);
            }
            if (sem_post(sem)) perror("sem_post");
            ++packets;


            if(!running) break;
        }
        printf(" Выход...\nОбработано наборов: %d\n", packets);
        break;
    }
    }

    munmap(shm, MAXMEM);
    shm_unlink("sharedmem");
    
    sem_close(sem);
    sem_unlink("semaphore");

    return 0;

}
