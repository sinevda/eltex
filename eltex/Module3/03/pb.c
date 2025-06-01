#include "pb.h"

int addQuery()
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

	addContact(newContact);

	return 0;
}

int addContact(contact _newContact)
{
	if (volume >= BOOKVOLUME)
	{
		printf("Телефонная книга заполнена.\n");
		return -1;
	}

	PhoneBook[volume++] = _newContact;
	printf("Контакт добавлен!\n");
	return 0;
}


int editQuery()
{
	displayNames();

	printf("Выберите контакт для изменения: ");
	int person = 0;
	scanf("%d", &person);
	
	if (person > volume && person < 0)
	{
		printf("Такого контакта нет");
		return -1;
	}
	else --person;

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
		printf("Текущее имя: %s\nВведите новое имя: ", PhoneBook[person].Name); getchar();
		if (scanf("%s", &name) == 0)
		{
			printf("Имя не введено");
		}
		editContact(person, "%n", name);

		break;
	}

	case 2:														//Surname - %s
	{
		char surname[MAXSTRING];
		printf("Текущая фамилия: %s\nВведите новую фамилию: ", PhoneBook[person].Surname); getchar();
		if (scanf("%s", &surname) == 0)
		{
			printf("Имя не введено");
		}
		editContact(person, "%s", surname);

		break;
	}

	case 3:														//Patronymic - %o
	{
		char patronymic[MAXSTRING];
		printf("Текущее отчество: %s\nВведите новое отчество: ", PhoneBook[person].Patronymic); getchar();
		scanf("%s", &patronymic);
		editContact(person, "%o", patronymic);
		break;
	}

	case 4:														//Job place - %p
	{
		char jobPlace[MAXSTRING];
		printf("Текущее место работы: %s\nВведите новое место работы: ", PhoneBook[person].JobPlace); getchar();
		scanf("%s", &jobPlace);
		editContact(person, "%p", jobPlace);
		break;
	}

	case 5:														//Job title - %t
	{
		char jobTitle[MAXSTRING];
		printf("Текущее место работы: %s\nВведите новое место работы: ", PhoneBook[person].JobTitle); getchar();
		scanf("%s", &jobTitle);
		editContact(person, "%t", jobTitle);
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

		editContact(person, "%w", FB, VK, OK, inst, TG);

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

		editContact(person, "%h", ph);
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

		editContact(person, "%e", em);
		break;
	}

	default:
		break;
	}

}

int editContact(int person, char* format, ...)
{
	va_list f;

	va_start(f, format);

	for (char* c = format; *c; ++c)
	{
		if (*c != '%') continue;
		switch (*++c)
		{
		case 'n':
			strcpy(PhoneBook[person].Name, va_arg(f, char*));
			break;
			
		case 's':
			strcpy(PhoneBook[person].Surname, va_arg(f, char*));
			break;

		case 'o':
			strcpy(PhoneBook[person].Patronymic, va_arg(f, char*));
			break;

		case 'p':
			strcpy(PhoneBook[person].JobPlace, va_arg(f, char*));
			break;

		case 't':
			strcpy(PhoneBook[person].JobTitle, va_arg(f, char*));
			break;

		case 'w':
			strcpy(PhoneBook[person].Networks.FB, va_arg(f, char*));
			strcpy(PhoneBook[person].Networks.VK, va_arg(f, char*));
			strcpy(PhoneBook[person].Networks.OK, va_arg(f, char*));
			strcpy(PhoneBook[person].Networks.inst, va_arg(f, char*));
			strcpy(PhoneBook[person].Networks.TG, va_arg(f, char*));
			break;

		case 'h':
		{
			char ph[MAXPHONES][MAXSTRING];
			memcpy(ph, va_arg(f, char**), sizeof(char) * MAXSTRING * MAXPHONES);

			for (int i = 0; i < MAXPHONES; ++i)
			{
				strcpy(PhoneBook[person].PhonesNumber[i], ph[i]);
			}
			break;
		}

		case 'e':
		{
			char em[MAXPHONES][MAXSTRING];
			memcpy(em, va_arg(f, char**), sizeof(char) * MAXSTRING * MAXEMAILS);

			for (int i = 0; i < MAXPHONES; ++i)
			{
				strcpy(PhoneBook[person].EMail[i], em[i]);
			}
			break;
		}

		}
	}
	va_end(f);

	return 0;
}


int deleteQuery()
{
	displayNames();
	printf("Выберите контакт для изменения: ");
	int person = 0;
	scanf("%d", &person);

	deleteContact(person);
	return 0;
}

int deleteContact(int person)
{
	for (int j = --person; j < volume - 1; j++) {
		PhoneBook[j] = PhoneBook[j + 1];
	}
	volume--;
	printf("Контакт удален.\n");
	
	return 0;
}


int displayPhoneBook()
{
	printf("\n\nСписок контактов:\n");

	for (int i = 0; i < volume; i++) {
		printf("Контакт №%d:\n", i + 1);
		printf("Имя: %s %s %s\n", PhoneBook[i].Surname, PhoneBook[i].Name, PhoneBook[i].Patronymic);
		if (strcmp(PhoneBook[i].JobPlace, "")) printf("Место работы: %s\n", PhoneBook[i].JobPlace);
		if (strcmp(PhoneBook[i].JobTitle, "")) printf("Должность: %s\n", PhoneBook[i].JobTitle);
		printf("Номера телефонов:\n");
		for (int j = 0; j < MAXPHONES; ++j)
		{
			if (!strcmp(PhoneBook[i].PhonesNumber[j], "\0"))
			{
				j = MAXPHONES;
				break;
			}
			printf("№%d. %s\n", j, PhoneBook[i].PhonesNumber[j]);
		}

		printf("Эл. почты:\n");
		for (int j = 0; j < MAXEMAILS; ++j)
		{
			if (!strcmp(PhoneBook[i].EMail[j], "\0"))
			{
				j = MAXEMAILS;
				break;
			}
			printf("№%d. %s\n", j, PhoneBook[i].EMail[j]);
		}

		if (strcmp(PhoneBook[i].Networks.FB, "")) printf("Аккаунт FB: %s\n", PhoneBook[i].Networks.FB);
		if (strcmp(PhoneBook[i].Networks.VK, "")) printf("Аккаунт VK: %s\n", PhoneBook[i].Networks.VK);
		if (strcmp(PhoneBook[i].Networks.OK, "")) printf("Аккаунт OK:  %s\n", PhoneBook[i].Networks.OK);
		if (strcmp(PhoneBook[i].Networks.inst, "")) printf("Аккаунт inst:  %s\n", PhoneBook[i].Networks.inst);
		if (strcmp(PhoneBook[i].Networks.TG, "")) printf("Аккаунт TG:  %s\n", PhoneBook[i].Networks.TG);
		printf("\n");
	}
	return 0;

}

int displayNames()
{
	printf("\n\nСписок контактов:\n");

	for (int i = 0; i < volume; i++) {
		printf("№%d: ", i + 1);
		printf("%s %s %s\n", PhoneBook[i].Surname, PhoneBook[i].Name, PhoneBook[i].Patronymic);
	}

	return 0;
}

int saveContacts()
{
    int file = open(FILENAME, O_RDWR);
    if (file == -1) {
        perror("Ошибка при открытии файла для записи.\n");
		exit(EXIT_FAILURE);
    }

	write(file, &volume, sizeof(int));
    for(int i = 0; i < volume; ++i)
	    write(file, &PhoneBook[i], sizeof(contact));

    close(file);
    return 0;
}

int loadContacts()
{
    int file = open(FILENAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);
    if (file == -1) {
		perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

	read(file, &volume, sizeof(int));
    for(int i = 0; i < volume; ++i)
	    read(file, &PhoneBook[i], sizeof(contact));

    close(file);
    return 0;
}