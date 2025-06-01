#include <stdio.h>

int main(int argc, char* argv[])
{
    int res = atoi(argv[1]);
    for(int i = 2; i < argc; ++i)
        res *= atoi(argv[i]);
    
    printf("%d\n", res);
    return 0;
}