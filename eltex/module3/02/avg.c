#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Нет аргументов для вычисления среднего.\n");
        return 1;
    }

    double sum = 0;
    for (int i = 1; i < argc; i++) {
        sum += atof(argv[i]);
    }
    double average = sum / (argc - 1);
    printf("Среднее значение: %.2f\n", average);
    return 0;
}
