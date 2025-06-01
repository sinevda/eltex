#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#include "pb.h"

contact PhoneBook[BOOKVOLUME];
int volume = 0;

int main()
{
	setlocale(LC_ALL, "ru");
    loadContacts();

	int choice;

	while (1)
	{
		printf("\n-----МЕНЮ-----\n");
		printf("1. Добавить контакт\n");
		printf("2. Редактировать контакт\n");
		printf("3. Удалить контакт\n");
		printf("4. Показать все контакты\n");
		printf("5. Выход\n");
		printf("Выбор: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
		{
			addQuery();
			break;
		}
		case 2:
			editQuery();
			break;
		case 3:
			deleteQuery();
			break;
		case 4:
			displayPhoneBook();
			break;
		case 5:
            saveContacts();
			return 0;
			break;
		default:
			printf("Некорректный выбор, попробуйте снова.\n");
		}
	}

	return 0;
}