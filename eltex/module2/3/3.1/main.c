#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

int LetToBin(char*);
int NumToBin(char*);
void DisplayPerm(int);
int FileStat(char*);
void editPerm(int);

int main()
{
    char input[10];
    int binary;
    int choice;

    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Ввести права доступа\n");
        printf("2. Узнать права доступа файла\n");
        printf("3. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Введите права доступа: ");
                scanf("%s", input);

                if (isdigit(input[0])) binary = NumToBin(input);

                else if (strlen(input) == 9 && strspn(input, "rwx-") == 9) binary = LetToBin(input);
                else
                {
                    printf("Некорректный ввод.\n");
                    continue;
                }

                DisplayPerm(binary);

                editPerm(binary);

                break;
            }

            case 2: {
                char filename[256];
                printf("Введите имя файла: ");
                scanf("%s", filename);

                if (binary = FileStat(filename) != -1) DisplayPerm(binary);
                
                break;
            }

            case 3:
                return 0;

            default:
                printf("Некорректный выбор.\n");
                break;
        }
    }

    return 0;
}

void editPerm(int binary)
{
    getchar();
    printf("Желаете изменить право доступа? (Y/n): ");
    char c;
    scanf("%c", &c);

    if(!(c == 'y' || c == 'Y')) return;

    while(1)
    {
        

    char perm[10];
    printf("Введите новые права доступа или как хотите изменить: ");
    scanf("%s", perm);
    
    int newbinary = 0;

    char* a = strchr(perm, '+');
    char * b = strchr(perm, '-');

    if (strspn(perm, "rwx-") == 9) newbinary = LetToBin(perm);
    else if (strspn(perm, "01") == 9) newbinary = (perm, NULL, 2);
    else if(strspn(perm, "01234567"))newbinary = NumToBin(perm);
    else if(strchr(perm, '-') != NULL || strchr(perm, '+') != NULL)
    {
        int a = strchr(perm, '+') == NULL ? 0 : 1;

        char* p = strtok(perm, "+-");
        int groupmask = 0000;
        for(int i = 0; i < strlen(p); ++i)
        {
            if (p[i] == 'u') groupmask |= 0700;
            if (p[i] == 'g') groupmask |= 0070;
            if (p[i] == 'o') groupmask |= 0007;
            if (p[i] == 'a') groupmask |= 0777;
        }
        p = strtok(NULL, "+-");

        int permmask = 0000;
        for(int i = 0; i < strlen(p); ++i)
        {
            if (p[i] == 'r') permmask |= 0444;
            if (p[i] == 'w') permmask |= 0222;
            if (p[i] == 'x') permmask |= 0111;
        }

        int mask = groupmask & permmask;

        if(a)
        {
            newbinary = binary | mask;            
        }
        else
        {
            newbinary = binary & ~mask;
        }
    }

    printf("Новые права доступа:\n");
    DisplayPerm(newbinary);

    printf("\nПродолжить изменение прав? (Y/n): ");
    scanf("%c", &c);
    if(!(c == 'y' || c == 'Y')) return;
    }
    return;
}

int LetToBin(char *permissions) 
{
    int bin = 0;
    for (int i = 0; i < 9; i++)
    {
        bin <<= 1;
        bin += (permissions[i] == 'r' || permissions[i] == 'w' || permissions[i] == 'x');

    }

    return bin;
}

int NumToBin(char *permissions)
{
    return strtol(permissions, NULL, 8);
}

void DisplayPerm(int binary)
{
    char letter[] = "---------\0";
    char binstr[] = "000000000\0";

    int bintmp = binary;


    for(int i = 0; i < 3; ++i)
    {
        if (bintmp & 0001) binstr[9 - i * 3 - 1] = '1';
        if (bintmp & 0002) binstr[9 - i * 3 - 2] = '1';
        if (bintmp & 0004) binstr[9 - i * 3 - 3] = '1';
        
        if (bintmp & 0001) letter[9 - i * 3 - 1] = 'x';
        if (bintmp & 0002) letter[9 - i * 3 - 2] = 'w';
        if (bintmp & 0004) letter[9 - i * 3 - 3] = 'r';
        bintmp >>= 3;
    }

    printf("Бинарное представление: %s\n", binstr);
    printf("Цифровое обозначение: %o\n", 356);
    printf("Буквенное обозначение: %s\n", letter);

    return;
}

int FileStat(char *filename)
{
    struct stat fileStat;
    if (stat(filename, &fileStat) < 0) {
        perror("Ошибка при получении информации о файле\n");
        return -1;
    }

    return fileStat.st_mode;
}
