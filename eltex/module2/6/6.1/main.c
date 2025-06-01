
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int main()
{

	setlocale(LC_ALL, "RUS");

	int choice;

	struct list* head = NULL;

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
			head = addQuery(head);
			break;
		}
		case 2:
			editQuery(head);
			break;
		case 3:
			head = deleteQuery(head);
			break;
		case 4:
			displayPhoneBook(head);
			break;
		case 5:
			return 0;
			break;
		default:
			getchar();
			printf("Некорректный выбор, попробуйте снова.\n");
		}
	}


	return 0;
}

