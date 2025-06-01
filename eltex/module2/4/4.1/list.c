#include "list.h"

struct list* addContact(contact _newContact, struct list* head)
{
	struct list* new;
	new = malloc(sizeof(struct list));
	new->value = _newContact;
	new->next = NULL;
	new->prev = NULL;
	
	printf("%s", new->value.Surname);
	if(NULL == head)
	{
		head = new;
		return head;
	}

	struct list* curr = head;
	
	for(curr = head; curr != NULL && strcmp(_newContact.Surname, curr->value.Surname) > 0; curr = curr->next);
	
	if(curr == head)
	{
		new->next = curr;
		curr->prev = new;
		head = new;
		printf("Контакт добавлен!\n");
		return head;
	}
	else if(curr == NULL)
	{
		for(curr = head; curr->next != NULL; curr = curr->next);
		curr->next = new;
		new->prev = curr;
		printf("Контакт добавлен!\n");
		return head;
	}
	else
	{
		new->next = curr;
		new->prev = curr->prev;
		curr->prev->next = new;
		curr->prev = new;
		printf("Контакт добавлен!\n");
		return head;
	}
	return head;
}


int editContact(struct list* curr, int person, char* format, ...)
{
	va_list f;

	va_start(f, format);

	for (char* c = format; *c; ++c)
	{
		if (*c != '%') continue;
		switch (*++c)
		{
		case 'n':
			strcpy(curr->value.Name, va_arg(f, char*));
			break;
			
		case 's':
			strcpy(curr->value.Surname, va_arg(f, char*));
			break;

		case 'o':
			strcpy(curr->value.Patronymic, va_arg(f, char*));
			break;

		case 'p':
			strcpy(curr->value.JobPlace, va_arg(f, char*));
			break;

		case 't':
			strcpy(curr->value.JobTitle, va_arg(f, char*));
			break;

		case 'w':
			strcpy(curr->value.Networks.FB, va_arg(f, char*));
			strcpy(curr->value.Networks.VK, va_arg(f, char*));
			strcpy(curr->value.Networks.OK, va_arg(f, char*));
			strcpy(curr->value.Networks.inst, va_arg(f, char*));
			strcpy(curr->value.Networks.TG, va_arg(f, char*));
			break;

		case 'h':
		{
			char ph[MAXPHONES][MAXSTRING];
			memcpy(ph, va_arg(f, char**), sizeof(char) * MAXSTRING * MAXPHONES);

			for (int i = 0; i < MAXPHONES; ++i)
			{
				strcpy(curr->value.PhonesNumber[i], ph[i]);
			}
			break;
		}

		case 'e':
		{
			char em[MAXPHONES][MAXSTRING];
			memcpy(em, va_arg(f, char**), sizeof(char) * MAXSTRING * MAXEMAILS);

			for (int i = 0; i < MAXPHONES; ++i)
			{
				strcpy(curr->value.EMail[i], em[i]);
			}
			break;
		}

		}
	}
	va_end(f);

	return 0;
}


struct list*  deleteContact(int person, struct list* head)
{
	struct list* curr;
	
	int i = 0;
	for (curr = head; i < person; curr = curr->next) ++i;

	if (curr->prev != NULL)
		curr->prev->next = curr->next;
	else
		head = curr->next;

	if (curr->next != NULL)
		curr->next->prev = curr->prev;

	free(curr);
	return head;

	printf("Контакт удален.\n");
	
	return head;
}


struct list* addQuery(struct list* head)
{
	contact newContact;
	printf("Введите фамилию: ");
	scanf("%s", newContact.Surname);
	printf("Введите имя: ");
	scanf("%s", newContact.Name);

	printf("Введите отчество: ");
	getchar();
	fgets(newContact.Patronymic, MAXSTRING, stdin);
	newContact.Patronymic[strcspn(newContact.Patronymic, "\n")] = '\0';

	printf("Введите место работы: ");
	fgets(newContact.JobPlace, MAXSTRING, stdin);
	newContact.JobPlace[strcspn(newContact.JobPlace, "\n")] = '\0';

	printf("Введите должность: ");
	fgets(newContact.JobTitle, MAXSTRING, stdin);
	newContact.JobTitle[strcspn(newContact.JobTitle, "\n")] = '\0';

	printf("Количество номеров телефона(не более %d): ", MAXPHONES);
	int n;
	scanf("%d", &n);
	getchar();
	for (int i = 0; i < n && i < MAXPHONES; ++i)
	{
		printf("Введите номер телефона: ");
		fgets(newContact.PhonesNumber[i], MAXSTRING, stdin);
		newContact.PhonesNumber[i][strcspn(newContact.PhonesNumber[i], "\n")] = '\0';
	}
	for (int i = n; i < MAXPHONES; ++i) strcpy(newContact.PhonesNumber[i], "\0");


	printf("Количество электронных почт(не более %d): ", MAXEMAILS);
	scanf("%d", &n);
	getchar();
	for (int i = 0; i < n && i < MAXPHONES; ++i)
	{
		printf("Введите электронную почту: ");
		fgets(newContact.EMail[i], MAXSTRING, stdin);
		newContact.EMail[i][strcspn(newContact.EMail[i], "\n")] = '\0';
	}
	for (int i = n; i < MAXPHONES; ++i) strcpy(newContact.EMail[i], "\0");

	printf("Аккаунт FB: ");
	fgets(newContact.Networks.FB, MAXSTRING, stdin);
	newContact.Networks.FB[strcspn(newContact.Networks.FB, "\n")] = '\0';
	printf("Аккаунт VK: ");
	fgets(newContact.Networks.VK, MAXSTRING, stdin);
	newContact.Networks.VK[strcspn(newContact.Networks.VK, "\n")] = '\0';
	printf("Аккаунт OK: ");
	fgets(newContact.Networks.OK, MAXSTRING, stdin);
	newContact.Networks.OK[strcspn(newContact.Networks.OK, "\n")] = '\0';
	printf("Аккаунт inst: ");
	fgets(newContact.Networks.inst, MAXSTRING, stdin);
	newContact.Networks.inst[strcspn(newContact.Networks.inst, "\n")] = '\0';
	printf("Аккаунт TG: ");
	fgets(newContact.Networks.TG, MAXSTRING, stdin);
	newContact.Networks.TG[strcspn(newContact.Networks.TG, "\n")] = '\0';

	return addContact(newContact, head);
}

struct list* editQuery(struct list* head)
{
	displayNames(head);

	printf("Выберите контакт для изменения: ");
	int person = 0;
	scanf("%d", &person);

	struct list* curr = head;
	for(int i = 1; i < person; ++i) curr = curr->next;

	int state = -1;

	printf("Что хотите изменить?\n\
1. Имя\n\
2. Фамилию\n\
3. Отчество\n\
4. Место работы\n\
5. Должность\n\
6. Социальные сети\n\
7. Номер(-а) телефона(-ов)\n\
8. Электронная(-ые) почта(-ы)\n\
Выбор: ");
	scanf("%d", &state);

	switch (state)
	{
	case 1:														//Name - %n
	{
		char name[MAXSTRING];
		printf("Текущее имя: %s\nВведите новое имя: ", curr->value.Name); getchar();
		if (scanf("%s", &name) == 0)
		{
			printf("Имя не введено");
		}
		editContact(curr, person, "%n", name);

		break;
	}

	case 2:														//Surname - %s
	{
		char surname[MAXSTRING];
		printf("Текущая фамилия: %s\nВведите новую фамилию: ", curr->value.Surname); getchar();
		if (scanf("%s", &surname) == 0)
		{
			printf("Имя не введено");
		}
		editContact(curr, person, "%s", surname);

		break;
	}

	case 3:														//Patronymic - %o
	{
		char patronymic[MAXSTRING];
		printf("Текущее отчество: %s\nВведите новое отчество: ", curr->value.Patronymic); getchar();
		scanf("%s", &patronymic);
		editContact(curr, person, "%o", patronymic);
		break;
	}

	case 4:														//Job place - %p
	{
		char jobPlace[MAXSTRING];
		printf("Текущее место работы: %s\nВведите новое место работы: ", curr->value.JobPlace); getchar();
		scanf("%s", &jobPlace);
		editContact(curr, person, "%p", jobPlace);
		break;
	}

	case 5:														//Job title - %t
	{
		char jobTitle[MAXSTRING];
		printf("Текущее место работы: %s\nВведите новое место работы: ", curr->value.JobTitle); getchar();
		scanf("%s", &jobTitle);
		editContact(curr, person, "%t", jobTitle);
		break;
	}

	case 6:														//Social networks - %w
	{
		char FB[MAXSTRING], VK[MAXSTRING], OK[MAXSTRING], inst[MAXSTRING], TG[MAXSTRING];
		getchar();
		printf("Аккаунт FB: ");
		fgets(FB, MAXSTRING, stdin);
		FB[strcspn(FB, "\n")] = '\0';
		printf("Аккаунт VK: ");
		fgets(VK, MAXSTRING, stdin);
		VK[strcspn(VK, "\n")] = '\0';
		printf("Аккаунт OK: ");
		fgets(OK, MAXSTRING, stdin);
		OK[strcspn(OK, "\n")] = '\0';
		printf("Аккаунт inst: ");
		fgets(inst, MAXSTRING, stdin);
		inst[strcspn(inst, "\n")] = '\0';
		printf("Аккаунт TG: ");
		fgets(TG, MAXSTRING, stdin);
		TG[strcspn(TG, "\n")] = '\0';

		editContact(curr, person, "%w", FB, VK, OK, inst, TG);

		break;
	}

	case 7:														//Phones - %h
	{
		char ph[MAXPHONES][MAXSTRING];

		printf("Новое количество номеров телефона(не более %d): ", MAXPHONES);
		int n;
		scanf("%d", &n);
		for (int j = 0; j < n && j < MAXPHONES; ++j)
		{
			printf("Введите номер телефона №%d: ", j + 1); getchar();
			scanf("%s", &ph[j]);
		}
		for (int j = n; j < MAXPHONES; ++j) strcpy(ph[j], "\0");

		editContact(curr, person, "%h", ph);
		break;
	}

	case 8:														//Emails - %e
	{
		char em[MAXEMAILS][MAXSTRING];
		printf("Новое количество электронных почт(не более %d): ", MAXEMAILS);
		int n;
		scanf("%d", &n);
		for (int j = 0; j < n && j < MAXEMAILS; ++j)
		{
			printf("Введите электронную почту №%d: ", j + 1);
			scanf("%s", &em[j]);
		}
		for (int j = n; j < MAXEMAILS; ++j) strcpy(em[j], "\0");

		editContact(curr, person, "%e", em);
		break;
	}

	default:
		break;
	}

}

struct list*  deleteQuery(struct list* head)
{
	displayNames(head);
	printf("Выберите контакт для изменения: ");
	int person = 0;
	scanf("%d", &person);

	return deleteContact(person, head);
}


void displayPhoneBook(struct list* head)
{
	printf("\n\nСписок контактов:\n");

	int i = 0;
	for(struct list* curr = head; curr != NULL; curr = curr->next)
	{
		++i;
		printf("Контакт №%d:\n", i);
		printf("Имя: %s %s %s\n", curr->value.Surname, curr->value.Name, curr->value.Patronymic);
		if (strcmp(curr->value.JobPlace, "")) printf("Место работы: %s\n", curr->value.JobPlace);
		if (strcmp(curr->value.JobTitle, "")) printf("Должность: %s\n", curr->value.JobTitle);
		printf("Номера телефонов:\n");
		for (int j = 0; j < MAXPHONES; ++j)
		{
			if (!strcmp(curr->value.PhonesNumber[j], "\0"))
			{
				j = MAXPHONES;
				break;
			}
			printf("№%d. %s\n", j + 1, curr->value.PhonesNumber[j]);
		}

		printf("Эл. почты:\n");
		for (int j = 0; j < MAXEMAILS; ++j)
		{
			if (!strcmp(curr->value.EMail[j], "\0"))
			{
				j = MAXEMAILS;
				break;
			}
			printf("№%d. %s\n", j + 1, curr->value.EMail[j]);
		}
		if (strcmp(curr->value.JobPlace, "")) printf("Место работы: %s\n", curr->value.JobPlace);
		if (strcmp(curr->value.JobTitle, "")) printf("Должность: %s\n", curr->value.JobTitle);
		
		if (strcmp(curr->value.Networks.FB, "")) printf("Аккаунт FB: %s\n", curr->value.Networks.FB);
		if (strcmp(curr->value.Networks.VK, "")) printf("Аккаунт VK: %s\n", curr->value.Networks.VK);
		if (strcmp(curr->value.Networks.OK, "")) printf("Аккаунт OK:  %s\n", curr->value.Networks.OK);
		if (strcmp(curr->value.Networks.inst, "")) printf("Аккаунт inst:  %s\n", curr->value.Networks.inst);
		if (strcmp(curr->value.Networks.TG, "")) printf("Аккаунт TG:  %s\n", curr->value.Networks.TG);
		printf("\n");
	}

	return;
}

void displayNames(struct list* head)
{
	printf("\n\nСписок контактов:\n");

	int i = 1;
	for(struct list* curr = head; curr != NULL; curr = curr->next)
	{
		printf("№%d %s %s %s\n", i++, curr->value.Surname, curr->value.Name, curr->value.Patronymic);
	}

	return;
}