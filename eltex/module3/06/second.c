#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define MSG_SIZE 100
#define MSG_TYPE 1
#define EXIT_MSG_TYPE 2

struct messagestr
{
    long mtype;
    char mtext[MSG_SIZE];
};

int main() {
    key_t key = ftok("queuef", 'a');
    if (key  == -1)
    {
        perror("Ошибка преобразования в key_t");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(key, S_IRWXU | S_IRWXG /*| S_IRWXO*/ | IPC_CREAT) ;
    if(msgid == -1)
    {
        perror("Ошибка создания очереди сообщений");
        exit(EXIT_FAILURE);
    }

    struct messagestr message;
    while (1)
    {
        if (msgrcv(msgid, &message, sizeof(message.mtext), 0, 0) == -1)
        {
            perror("Ошибка получения сообщения");
            exit(EXIT_FAILURE); 
        }
        
        if (message.mtype == EXIT_MSG_TYPE)
        {
            printf("Завершение обмена.\n");
            break;
        }

        printf("Получено сообщение: %s", message.mtext);
        printf("Введите сообщение: ");
        fgets(message.mtext, MSG_SIZE, stdin);
        message.mtype = MSG_TYPE;

        if (strcmp(message.mtext, "exit\n") == 0 || 
            strcmp(message.mtext, "\n")     == 0)
        {
            message.mtype = EXIT_MSG_TYPE;
            
            if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
            {
                perror("Ошибка отправки сообщения");
                exit(EXIT_FAILURE);
            }

            break;
        }

        if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
        {
            perror("Ошибка отправки сообщения");
            exit(EXIT_FAILURE);
        }
    }

    msgctl(msgid, IPC_RMID, NULL);
    
    exit(EXIT_SUCCESS);
}
