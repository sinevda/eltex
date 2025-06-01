#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define QUEUE_NAME "/my_queue"
#define MSG_SIZE 100
#define EXIT_MSG "exit"

int main() {
    mqd_t ds;
    char message[MSG_SIZE];

    struct mq_attr queue_attr;
    queue_attr.mq_maxmsg = 1;
    queue_attr.mq_msgsize = MSG_SIZE;
    
    ds = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG, &queue_attr);
    if (ds == (mqd_t)-1)
    {
        perror("Ошибка создания очереди сообщений");
        exit(EXIT_FAILURE);
    }

    unsigned prio = 1;

    while (1)
    {
        printf("Введите сообщение: ");
        fgets(message, MSG_SIZE, stdin);

        if (strcmp(message, EXIT_MSG "\n") == 0 || strcmp(message, "\n") == 0)
            prio = MQ_PRIO_MAX -1;

        if (mq_send(ds, message, strlen(message) + 1, prio) == -1)
        {
            perror("Ошибка отправки сообщения");
            exit(EXIT_FAILURE);
        }     
        
        if(prio == MQ_PRIO_MAX - 1)
        {
            printf("Завершение обмена.");
            break;
        }

        if (mq_receive(ds, message, MSG_SIZE, &prio) == -1)
        {
            perror("Ошибка получения сообщения");
            exit(EXIT_FAILURE);
        }        
        
        if(prio == MQ_PRIO_MAX - 1)
        {
            printf("Завершение обменa\n");
            exit(EXIT_SUCCESS);
        }
        printf("Получено сообщение: %s", message);
    }

    if (mq_close(ds) == -1)
        perror("Ошибка закрытия очереди");
    if (mq_unlink(QUEUE_NAME) == -1)
        perror("Ошибка удаления очереди");
    
    exit(EXIT_SUCCESS);
}
