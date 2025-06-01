#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#define MAXSTRING 50
#define BOOKVOLUME 15
#define MAXPHONES 3
#define MAXEMAILS 3
static const char FILENAME[] = "contacts";

typedef struct
{
	char VK[MAXSTRING];
	char TG[MAXSTRING];
	char inst[MAXSTRING];
	char OK[MAXSTRING];
	char FB[MAXSTRING];
} SocNet;

typedef struct
{
	char Name[MAXSTRING];
	char Surname[MAXSTRING];
	char Patronymic[MAXSTRING];

	char PhonesNumber[MAXPHONES][MAXSTRING];
	char EMail[MAXEMAILS][MAXSTRING];
	char JobPlace[MAXSTRING];
	char JobTitle[MAXSTRING];

	SocNet Networks;
} contact;

extern contact PhoneBook[];
extern int volume ;

int addQuery();
int editQuery();
int deleteQuery();

/*Formating:
%n - name, %s - surname, %o - patronymic,
%p - job place, %t - job title,
%w - social networks(FB, VK, OK, inst, TG),
%h - phones phones...,
%e - emails emails...*/
int editContact(int, char*, ...);

int addContact(contact);
int deleteContact(int);
int displayPhoneBook();
int displayNames();

int loadContacts();
int saveContacts();
