#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "BTree.h"



int main()
{
    setlocale(LC_ALL, "ru");

    Node* root = NULL;

    int choice;

    while (1)
    {
        printf("\n-----МЕНЮ-----\n");
        printf("1. Добавить контакт\n");
        printf("2. Удалить контакт\n");
        printf("3. Изменить контакт\n");
        printf("4. Показать все контакты\n");
        printf("5. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:        
            root = addQuery(root);
            break;
        
        case 2:
            root = removeQuery(root);
            break;

        case 3:
            editQuery(root);
            break;

        case 4:
            displayContacts(root);
            break;
        
        case 5:
            return 0;

        default:
            printf("Некорректный выбор, попробуйте снова.\n");
            getchar();
            break;
        }
    }

    return 0;
}
