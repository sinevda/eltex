#include "BTree.h"

// Функция для поиска контакта в дереве
Node* searchContact(Node* node, contact data)
{
    if (node == NULL)
        return NULL;
    
    if (strcmp(data.Surname, node->data.Surname) < 0)
        return searchContact(node->left, data);
    else if (strcmp(data.Surname, node->data.Surname) > 0)
        return searchContact(node->right, data);
    else
    {
        if (strcmp(data.Name, node->data.Name) < 0)
            return searchContact(node->left, data);
        else if (strcmp(data.Name, node->data.Name) > 0)
            return searchContact(node->right, data);
        else
            return node;
    }
}

int editQuery(Node* root)
{
    contact toEdit;
    char sn[MAXSTRING];
	printf("Введите фамилию контакта для изменения: ");
    scanf("%s", toEdit.Surname);
    
    char n[MAXSTRING];
	printf("Введите имя контакта для изменения: ");
    scanf("%s", toEdit.Name);

    Node* curr = searchContact(root, toEdit);
    if(curr == NULL)
    {
        printf("Контакт не найден.");
        return -1;
    }

    int state;
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
		printf("Текущее имя: %s\nВведите новое имя: ", curr->data.Name); getchar();
		if (scanf("%s", &name) == 0)
		{
			printf("Имя не введено");
		}
		editContact(curr, "%n", name);

		break;
	}

	case 2:														//Surname - %s
	{
		char surname[MAXSTRING];
		printf("Текущая фамилия: %s\nВведите новую фамилию: ", curr->data.Surname); getchar();
		if (scanf("%s", &surname) == 0)
		{
			printf("Имя не введено");
		}
		editContact(curr, "%s", surname);

		break;
	}

	case 3:														//Patronymic - %o
	{
		char patronymic[MAXSTRING];
		printf("Текущее отчество: %s\nВведите новое отчество: ", curr->data.Patronymic); getchar();
		scanf("%s", &patronymic);
		editContact(curr, "%o", patronymic);
		break;
	}

	case 4:														//Job place - %p
	{
		char jobPlace[MAXSTRING];
		printf("Текущее место работы: %s\nВведите новое место работы: ", curr->data.JobPlace); getchar();
		scanf("%s", &jobPlace);
		editContact(curr, "%p", jobPlace);
		break;
	}

	case 5:														//Job title - %t
	{
		char jobTitle[MAXSTRING];
		printf("Текущее место работы: %s\nВведите новое место работы: ", curr->data.JobTitle); getchar();
		scanf("%s", &jobTitle);
		editContact(curr, "%t", jobTitle);
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

		editContact(curr, "%w", FB, VK, OK, inst, TG);

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

		editContact(curr, "%h", ph);
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

		editContact(curr, "%e", em);
		break;
	}

	default:
		break;
	}

}

int editContact(Node* curr, char* format, ...)
{
	va_list f;

	va_start(f, format);

	for (char* c = format; *c; ++c)
	{
		if (*c != '%') continue;
		switch (*++c)
		{
		case 'n':
			strcpy(curr->data.Name, va_arg(f, char*));
			break;
			
		case 's':
			strcpy(curr->data.Surname, va_arg(f, char*));
			break;

		case 'o':
			strcpy(curr->data.Patronymic, va_arg(f, char*));
			break;

		case 'p':
			strcpy(curr->data.JobPlace, va_arg(f, char*));
			break;

		case 't':
			strcpy(curr->data.JobTitle, va_arg(f, char*));
			break;

		case 'w':
			strcpy(curr->data.Networks.FB, va_arg(f, char*));
			strcpy(curr->data.Networks.VK, va_arg(f, char*));
			strcpy(curr->data.Networks.OK, va_arg(f, char*));
			strcpy(curr->data.Networks.inst, va_arg(f, char*));
			strcpy(curr->data.Networks.TG, va_arg(f, char*));
			break;

		case 'h':
		{
			char ph[MAXPHONES][MAXSTRING];
			memcpy(ph, va_arg(f, char**), sizeof(char) * MAXSTRING * MAXPHONES);

			for (int i = 0; i < MAXPHONES; ++i)
			{
				strcpy(curr->data.PhonesNumber[i], ph[i]);
			}
			break;
		}

		case 'e':
		{
			char em[MAXPHONES][MAXSTRING];
			memcpy(em, va_arg(f, char**), sizeof(char) * MAXSTRING * MAXEMAILS);

			for (int i = 0; i < MAXPHONES; ++i)
			{
				strcpy(curr->data.EMail[i], em[i]);
			}
			break;
		}

		}
	}
	va_end(f);

	return 0;
}


Node* addQuery(Node* root)
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
    for (int i = 0; i < n && i < MAXEMAILS; ++i)
    {
        printf("Введите электронную почту: ");
        fgets(newContact.EMail[i], MAXSTRING, stdin);
        newContact.EMail[i][strcspn(newContact.EMail[i], "\n")] = '\0';
    }
    for (int i = n; i < MAXEMAILS; ++i) strcpy(newContact.EMail[i], "\0");

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

    return addContact(root, newContact);
}

Node* removeQuery(Node* root)
{
    contact contactToDelete;
    printf("Введите фамилию контакта для удаления: ");
    scanf("%s", contactToDelete.Surname);
    printf("Введите имя контакта для удаления: ");
    scanf("%s", contactToDelete.Name);

    return deleteContact(root, contactToDelete);
            
}

int getHeight(Node* node)
{
    if (node == NULL) {
        return 0;
    }

    int rh = getHeight(node->right);
    int lh = getHeight(node->left);

    return 1 + (rh > lh ? rh : lh);
}

int getBalance(Node* node)
{
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

Node* rightRotate(Node* node)
{
    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}

Node* leftRotate(Node* node) 
{
    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

Node* balanceTree(Node* node)
{
    int balance = getBalance(node);
    if (balance > 1) {
        if (getBalance(node->left) >= 0) {
            return rightRotate(node);
        } else {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }
    if (balance < -1) {
        if (getBalance(node->right) <= 0) {
            return leftRotate(node);
        } else {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }
    return node;
}

Node* createNode(contact data)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* addContact(Node* root, contact data)
{
    if (root == NULL)
        return createNode(data);

    if (strcmp(data.Surname, root->data.Surname) < 0)
        root->left = addContact(root->left, data);
    else
        root->right = addContact(root->right, data);

    return root;
}

Node* deleteContact(Node* root, contact data)
{
    if (root == NULL)
    {
        return root;
    }

    if (strcmp(data.Surname, root->data.Surname) < 0)
    {
        root->left = deleteContact(root->left, data);
    }
    else if (strcmp(data.Surname, root->data.Surname) > 0)
    {
        root->right = deleteContact(root->right, data);
    }
    else
    {
        if (strcmp(data.Name, root->data.Name) < 0)
        {
            root->left = deleteContact(root->left, data);
        }
        else if (strcmp(data.Name, root->data.Name) > 0)
        {
            root->right = deleteContact(root->right, data);
        }
        else
        {
            if (root->left == NULL && root->right == NULL) 
            {
                free(root);
                return NULL;
            }
            else if (root->left == NULL)
            {
                Node* temp = root;
                root = root->right;
                free(temp);
            }
            else if (root->right == NULL)
            {
                Node* temp = root;
                root = root->left;
                free(temp);
            }
            else
            {
                Node* temp = root->right;
                while (temp->left != NULL)
                {
                    temp = temp->left;
                }
                root->data = temp->data;
                root->right = deleteContact(root->right, temp->data);
            }
        }
    }

    return root;
}

void displayContacts(Node* root)
{
    if (root == NULL)
    {
        return;
    }

    displayContacts(root->left);

    printf("\nКонтакт:\n");
    printf("Имя: %s %s %s\n", root->data.Surname, root->data.Name, root->data.Patronymic);
    if (strcmp(root->data.JobPlace, "")) printf("Место работы: %s\n", root->data.JobPlace);
    if (strcmp(root->data.JobTitle, "")) printf("Должность: %s\n", root->data.JobTitle);
    printf("Номера телефонов:\n");
    for (int j = 0; j < MAXPHONES; ++j)
    {
        if (!strcmp(root->data.PhonesNumber[j], "\0"))
        {
            j = MAXPHONES;
            break;
        }
        printf("№%d. %s\n", j, root->data.PhonesNumber[j]);
    }

    printf("Эл. почты:\n");
    for (int j = 0; j < MAXEMAILS; ++j)
    {
        if (!strcmp(root->data.EMail[j], "\0"))
        {
            j = MAXEMAILS;
            break;
        }
        printf("№%d. %s\n", j, root->data.EMail[j]);
    }

    if (strcmp(root->data.Networks.FB, "")) printf("Аккаунт FB: %s\n", root->data.Networks.FB);
    if (strcmp(root->data.Networks.VK, "")) printf("Аккаунт VK: %s\n", root->data.Networks.VK);
    if (strcmp(root->data.Networks.OK, "")) printf("Аккаунт OK:  %s\n", root->data.Networks.OK);
    if (strcmp(root->data.Networks.inst, "")) printf("Аккаунт inst:  %s\n", root->data.Networks.inst);
    if (strcmp(root->data.Networks.TG, "")) printf("Аккаунт TG:  %s\n", root->data.Networks.TG);
    printf("\n");

    displayContacts(root->right);
}