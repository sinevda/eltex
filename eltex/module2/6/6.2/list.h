#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAXSTRING 50
#define BOOKVOLUME 15
#define MAXPHONES 3
#define MAXEMAILS 3

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

struct list
{
	contact value;

	struct list* next;
	struct list* prev;	
};

struct list* addQuery(struct list*);
void editQuery(struct list*);
struct list* deleteQuery(struct list*);


void displayPhoneBook(struct list* head);
void displayNames(struct list* head);

/*Formating:
%n - name, %s - surname, %o - patronymic,
%p - job place, %t - job title,
%w - social networks(FB, VK, OK, inst, TG),
%h - phones phones...,
%e - emails emails...*/
int editContact(struct list*, int, char*, ...);

struct list* addContact(contact, struct list*);
struct list* deleteContact(int, struct list*);