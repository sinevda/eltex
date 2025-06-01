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

#define MAXPACKET 192

const char* getFileName(const char* path);
const char* remove_filename(const char *path);

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buff[MAXPACKET];
    printf("TCP CLIENT\n");
    
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    // Извлечение порта
    portno = atoi(argv[2]);
    
    // Шаг 1 - создание сокета
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) 
        error("ERROR opening socket");
    
    // Извлечение хоста
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    // Заполнение структуры serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    
    // Установка порта
    serv_addr.sin_port = htons(portno);
    
    // Шаг 2 - установка соединения    
    if (connect(my_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    // Шаг 3 - чтение и передача сообщений
    while (1) {
        // Получаем сообщение от сервера
        n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0);
        if (n <= 0) {
            printf("Connection closed by server.\n");
            break;
        }
        
        // Ставим завершающий ноль в конце строки
        buff[n] = 0;
        printf("S=>C: %s", buff);

        // Читаем пользовательский ввод с клавиатуры
        printf("S<=C: "); 
        fgets(&buff[0], sizeof(buff) - 1, stdin);

        // Передаем строку клиента серверу
        send(my_sock, &buff[0], strlen(&buff[0]), 0);

        // Проверка на "quit"
        if (!strcmp(&buff[0], "quit\n")) {
            printf("Exit...\n");
            close(my_sock);
            return 0;
        }

        if (atoi(buff) == 5)
        {
            n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0);
            if (n <= 0) {
                printf("Connection closed by server.\n");
                break;
            }        
            buff[n] = 0;
            printf("S=>C: %s", buff);

            char filename[MAXPACKET];
            printf("file: "); 
            fgets(&filename[0], sizeof(filename) - 1, stdin);
            filename[strcspn(filename, "\n")] = 0; // Удаляем символ новой строки

            int file = open(filename, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);
            if (file == -1)
                error("Open file error");

            struct stat _fileStatbuff;
            if (fstat(file, &_fileStatbuff) != 0) {
                error("File stat error");
            }

            int filesize = _fileStatbuff.st_size;
            if (filesize == 0) {
                printf("File is empty.\n");
                close(file);
                continue; // Пропускаем итерацию, если файл пустой
            }

            int sendcount = ceil((double)filesize / MAXPACKET);

            sprintf(buff, "%s\\%d\\%d\\\0", getFileName(filename), sendcount, filesize);
            send(my_sock, &buff[0], strlen(&buff[0]), 0);

            n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0);
            if (n <= 0) {
                printf("Connection closed by server.\n");
                close(file);
                break;
            }
            buff[n] = 0;
            printf("S=>C: %s", buff);

            // Отправка содержимого файла
            lseek(file, 0, SEEK_SET); // Устанавливаем указатель на начало файла
            for (int i = 0; i < sendcount; ++i) {
                int bytesRead = read(file, buff, MAXPACKET);
                if (bytesRead > 0) {
                    send(my_sock, buff, bytesRead, 0);
                } else {
                    break; // Если чтение не удалось, выходим из цикла
                }
            }

            n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0);
            if (n <= 0) {
                printf("Connection closed by server.\n");
                close(file);
                break;
            }
            buff[n] = 0;
            printf("S=>C: %s", buff);

            close(file); // Закрываем файл после завершения передачи
        }
    }
    
    close(my_sock);
    return 0;
}

const char* getFileName(const char* path) {
    const char* fileName = strrchr(path, '/'); // Находим последний символ '/'
    if (fileName) {
        return fileName + 1; // Возвращаем строку после '/'
    }
    return path; // Если '/' не найден, возвращаем весь путь
}

const char* remove_filename(const char *path) {
    static char temp[MAXPACKET];
    strcpy(temp, path);
    char *last_slash = strrchr(temp, '/');
    
    if (last_slash != NULL) {
        *last_slash = '\0';
    }
    return temp; // Возвращаем измененный путь
}
