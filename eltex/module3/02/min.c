#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Нет аргументов для сравнения.\n");
        return 1;
    }

    int min = atoi(argv[1]);
    for (int i = 2; i < argc; i++) {
        int num = atoi(argv[i]);
        if (num < min) {
            min = num;
        }
    }
    printf("Наименьшее значение: %d\n", min);
    return 0;
}
