#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define             MSG_SIZE    100
#define             LOGIN_CODE  1
#define             LOGIN_SUCC  "login"
#define             MSG_CODE    2
#define             LOGOUT_CODE 3
#define             MSG_SRV     1
#define             MAX_USERS   10


struct              messagestr
{
    long            mtype;
    char            mtext[MSG_SIZE];
};


void                errorHandling(char* message);
int                 login();
int                 logout();
int                 sendMessages();

int                 user[MAX_USERS];
int                 counter = 0;
int                 msgid;
struct messagestr   message;

int main()
{
    key_t key = ftok(".queuef", 'a');
    if (key  == -1)
        errorHandling("Ошибка преобразования в key_t");

    msgid = msgget(key, S_IRWXU | S_IRWXG | IPC_CREAT);
    if (msgid == -1)
        errorHandling("Ошибка создания очереди сообщений");

    while (1)
    {
        if (msgrcv(msgid, &message, sizeof(message.mtext), MSG_SRV, 0) == -1)
            errorHandling("Ошибка получения сообщения");
        
        switch (message.mtext[0])
        {
        case LOGIN_CODE:
            if (login())
                errorHandling("Ошибка авторизации пользователя");
            break;
        
        case MSG_CODE:
        {
            if (sendMessages())
                errorHandling("Ошибка отправки сообщения");
            break;
        }

        case LOGOUT_CODE:
            logout();
            break;

        default:
            break;
        }
    }

    if (msgctl(msgid, IPC_RMID, NULL))
        errorHandling("Ошибка удаления очереди");
    
    exit(EXIT_SUCCESS);
}

void errorHandling(char* message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int login()
{
    if(counter < MAX_USERS)
    {
        user[counter] = atoi(&message.mtext[1]);
        message.mtype = user[counter];
        strcpy(message.mtext, LOGIN_SUCC);

        printf("Used logged in. Pid: %d\n", user[counter]);
        ++counter;
    }
    else
    {
        message.mtype = atoi(&message.mtext[1]);
        strcpy(message.mtext, "");
        printf("User login failed. Pid: %d\n", atoi(&message.mtext[1]));
    }

    if (msgsnd(msgid, &message, sizeof(message.mtext), 0))
        return 1;

    return 0;
}

int logout()
{
    int flag = 0;
    for(int i = 0; i < counter; ++i)
    {
        if (user[i] == atoi(&message.mtext[1]))
            flag = 1;

        if (flag && i + 1 != counter)
            user[i] = user[i + 1];
    }
    --counter;

    printf("User logged out. Pid: %d\n", atoi(&message.mtext[1]));
    fflush(stdout);
    return 0;
}

int sendMessages()
{
    char* s = strtok(&message.mtext[1], "\30");
    pid_t tmppid = atoi(s);

    s = strtok(NULL, "\30");
    printf("Новое сообщение пользователям: %s", s);
    sprintf(message.mtext, "%c%s", MSG_CODE, s);

    for(int i = 0; i < counter; ++i)
    {
        if (user[i] != tmppid)
        {
            message.mtype = user[i];
            if (msgsnd(msgid, &message, sizeof(message.mtext), 0))
                return 1;
        }
    }
    return 0;
}