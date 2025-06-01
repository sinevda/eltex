#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define MAXSTRING 50
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

typedef struct Node
{
    contact data;
    struct Node* left;
    struct Node* right;
} Node;

Node* addQuery(Node*);
Node* removeQuery(Node*);
Node* searchContact(Node*, contact);
int editQuery(Node* root);
int editContact(Node*, char*, ...);

int getHeight(Node* node);
int getBalance(Node* node);
Node* rightRotate(Node* node);
Node* leftRotate(Node* node);
Node* balanceTree(Node* node);
Node* addContact(Node* root, contact data);
Node* deleteContact(Node* root, contact data);
void displayContacts(Node* root);