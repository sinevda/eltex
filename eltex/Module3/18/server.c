#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAXPACKET 192
#define MAXCLIENTS 10

// Прототип функции, обслуживающей подключившихся пользователей
void dostuff(int sock);
int CheckRecv(int bytes_recv, char buff[]);

// Функция обработки ошибок
void error(const char *msg) {
    perror(msg);
    exit(1);
}

// Глобальная переменная – количество активных пользователей 
int nclients = 0;

// Макрос для печати количества активных пользователей 
void printusers() { 
    if (nclients) {
        printf("%d user(s) on-line\n", nclients);
    } else {
        printf("No User on line\n");
    }
}

int main(int argc, char *argv[])
{
    printf("TCP SERVER\n");
    
    int sockfd, newsockfd; // Дескрипторы сокетов
    int portno; // Номер порта
    socklen_t clilen; // Размер адреса клиента
    struct sockaddr_in serv_addr, cli_addr; // Структура сокета сервера и клиента
    
    // Ошибка в случае если мы не указали порт
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    
    // Шаг 1 - создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
     
    // Шаг 2 - связывание сокета с локальным адресом
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    
    // Вызываем bind для связывания
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    
    // Шаг 3 - ожидание подключений, размер очереди - 5
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    
    fd_set read_fds; // Множество дескрипторов для select
    int client_sockets[MAXCLIENTS] = {0}; // Массив для хранения клиентских сокетов

    while (1) {
        // Обнуляем множество дескрипторов
        FD_ZERO(&read_fds);
        FD_SET(sockfd, &read_fds);
        int max_sd = sockfd;

        // Добавляем клиентские сокеты в множество
        for (int i = 0; i < MAXCLIENTS; i++) {
            if (client_sockets[i] > 0) {
                FD_SET(client_sockets[i], &read_fds);
            }
            if (client_sockets[i] > max_sd) {
                max_sd = client_sockets[i];
            }
        }

        // Ожидание активности на сокетах
        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            error("ERROR in select");
        }

        // Проверяем, есть ли новое подключение
        if (FD_ISSET(sockfd, &read_fds)) {
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) 
                error("ERROR on accept");

            // Добавляем новый сокет в массив клиентских сокетов
            for (int i = 0; i < MAXCLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = newsockfd;
                    nclients++; // Увеличиваем счетчик подключившихся клиентов
                    printusers();
                    dostuff(client_sockets[i]);
                    break;
                }
            }
        }

        // Обрабатываем активные клиентские сокеты
        for (int i = 0; i < MAXCLIENTS; i++) {
            if (FD_ISSET(client_sockets[i], &read_fds)) {
                char buff[MAXPACKET];
                int bytes_recv = read(client_sockets[i], buff, sizeof(buff));
                
                if (bytes_recv <= 0) {
                    // Клиент отключился
                    close(client_sockets[i]);
                    client_sockets[i] = 0; // Сбрасываем сокет
                    nclients--; // Уменьшаем счетчик активных клиентов
                    printf("-disconnect\n");
                    printusers();
                }
                else if (CheckRecv(bytes_recv, buff))
                    dostuff(client_sockets[i]);
            }
        }
    }

    close(sockfd);
    return 0; // Мы никогда не доберемся сюда
}

void dostuff(int sock)
{
    while (1)
    {
        int bytes_recv; // Размер принятого сообщения
        int a, b; // Переменные для операций
        char buff[MAXPACKET];

        char str[] = "Выберите действие:\n1. Сложение\n2. Вычитание\n3. Умножение\n4. Деление\n5. Передача файлов\nquit для выхода\n";
        write(sock, str, sizeof(str));

        bytes_recv = read(sock, &buff[0], sizeof(buff));
        if (!CheckRecv(bytes_recv, buff)) return;

        switch (atoi(buff))
        {
        case 1:
        {
            char str1[] = "Сложение. Введите первое число:\r\n";
            char str2[] = "Введите второе число:\r\n";
            write(sock, str1, sizeof(str1));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            a = atoi(buff);
            write(sock, str2, sizeof(str2));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            b = atoi(buff);
            snprintf(buff, sizeof(buff), "Результат: %d\n", a + b);
            write(sock, buff, strlen(buff));
        }
            break;

        case 2:
        {
            char str1[] = "Вычитание. Введите первое число:\r\n";
            char str2[] = "Введите второе число:\r\n";
            write(sock, str1, sizeof(str1));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            a = atoi(buff);
            write(sock, str2, sizeof(str2));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            b = atoi(buff);
            snprintf(buff, sizeof(buff), "Результат: %d\n", a - b);
            write(sock, buff, strlen(buff));
        }
            break;

        case 3:
        {
            char str1[] = "Умножение. Введите первое число:\r\n";
            char str2[] = "Введите второе число:\r\n";
            write(sock, str1, sizeof(str1));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            a = atoi(buff);
            write(sock, str2, sizeof(str2));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            b = atoi(buff);
            snprintf(buff, sizeof(buff), "Результат: %d\n", a * b);
            write(sock, buff, strlen(buff));
        }
            break;

        case 4:
        {
            char str1[] = "Деление. Введите первое число:\r\n";
            char str2[] = "Введите второе число:\r\n";
            write(sock, str1, sizeof(str1));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            a = atoi(buff);
            write(sock, str2, sizeof(str2));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;
            b = atoi(buff);
            snprintf(buff, sizeof(buff), "Результат: %lf\n", (double)a / b);
            write(sock, buff, strlen(buff));
        }
            break;

        case 5:
        {
            char str1[] = "Введите путь к файлу\r\n";
            write(sock, str1, sizeof(str1));
            bytes_recv = read(sock, &buff[0], sizeof(buff));
            if (!CheckRecv(bytes_recv, buff)) return;

            char* ptr = strtok(buff, "\\");
            char filename[MAXPACKET];
            strcpy(filename, ptr);

            ptr = strtok(NULL, "\\");
            int sendcount = atoi(ptr);

            ptr = strtok(NULL, "\\");
            int filesize = atoi(ptr);

            mkdir("received", 0775);
            chdir("received");

            int file = open(filename, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);
            if (file < 0) {
                perror("ERROR opening file");
                return;
            }

            sprintf(buff, "ready");
            write(sock, buff, sizeof(buff));

            for (int i = 0; i < sendcount; ++i) {
                bytes_recv = read(sock, &buff[0], sizeof(buff));
                if (bytes_recv <= 0) {
                    close(file);
                    return; // Если произошла ошибка чтения, выходим
                }
                if (i != sendcount - 1) {
                    write(file, buff, sizeof(buff));
                } else {
                    write(file, buff, filesize);
                }
                filesize -= sizeof(buff);
            }

            close(file);
            chdir("..");

            char str2[] = "Успешная передача!\r\n";
            write(sock, str2, sizeof(str2));
        }
            break;

        default:
            break;
        }
    }
}

int CheckRecv(int bytes_recv, char buff[])
{
    if (bytes_recv < 0) {
        error("ERROR reading from socket");
    }
    if (!strcmp(&buff[0], "quit\n")) {
        //nclients--; // Уменьшаем счетчик активных клиентов
        printf("-disconnect\n");
        fflush(stdout);
        printusers();
        return 0;
    }
    return 1;
}
