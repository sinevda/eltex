
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

typedef struct list* 	(*listlist_function)(struct list*);
typedef void 			(*voidvoid_function)(void);
typedef void 			(*voidlist_function)(struct list*);


int main()
{

	void* handle;
	handle = dlopen("./phonelib.so", RTLD_LAZY);
	if(!handle)	return -1;

	listlist_function addQuery = dlsym(handle, "addQuery");
	listlist_function deleteQuery = dlsym(handle, "deleteQuery");
	voidlist_function editQuery = dlsym(handle, "editQuery");
	voidlist_function displayPhoneBook = dlsym(handle, "displayPhoneBook");

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

	dlclose(handle);

	return 0;
}

