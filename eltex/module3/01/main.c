#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int calculate(int i)
{
    return i * i;
}

void printsq(char* argv[], int start, int stop)
{
    for(int i = start; i < stop; ++i)
        printf("Side: %d, area: %d\n", atoi(argv[i]), calculate(atoi(argv[i])));
}

int main(int argc, char* argv[])
{
    if(argc < 2)
        exit(EXIT_FAILURE);

    int mid = (argc - 1) / 2 + 1;

    pid_t pid;
    switch (pid = fork())
    {
    case 0:
        printsq(argv, mid, argc);
        exit(EXIT_SUCCESS);
    
    default:
        printsq(argv, 1, mid);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}