#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    if (argc!=3)
    {
        printf("using: %s ip port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct in_addr sourceip, destip, needip;
    ushort sourceport, destport, needport;

    inet_aton(argv[1], &needip);;
    needport = (ushort)atoi(argv[2]);

    printf("Searching UPD packets for: ip: %s, port: %d\n", inet_ntoa(needip), needport);
    fflush(stdout);

    int sockfd;
    char buffer[BUFFER_SIZE];

    // Создание RAW сокета
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int n = recv(sockfd, (char *)buffer, BUFFER_SIZE, 0);

        u_char* ipheader = buffer;
        u_char* udpheader = buffer + 20;

        destip.s_addr = *(uint*)(ipheader + 16);
        destport = *(ushort*)(udpheader + 2);
        
        if(destip.s_addr == needip.s_addr && destport == htons(needport))
        {
            sourceip.s_addr = *(uint*)(ipheader + 12);

            sourceport = *(ushort*)(udpheader);

            printf("Source ip: %s, Destionation ip: %s\n", inet_ntoa(sourceip), inet_ntoa(destip));
            printf("Source port: %d, destionation port: %d\n", htons(sourceport), htons(destport));
            
            printf("Dump:\n");  
            for(int i = 0; i < n; ++i) printf("%02X ", (u_char)buffer[i]);
            printf("\n\n");

            fflush(stdout);
        }
    }

    close(sockfd);
    return 0;
}
