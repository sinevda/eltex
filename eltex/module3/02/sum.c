#include <stdio.h>

int main(int argc, char* argv[])
{
    int res = 0;
    for(int i = 1; i < argc; ++i)
        res += atoi(argv[i]);
    
    printf("%d\n", res);
    return 0;
}