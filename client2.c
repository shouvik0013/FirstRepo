#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    ssize_t bytes_written;

    char read_buffer[256], write_buffer[256];
    if(sizeof(argv) < 3)
    {
        printf("Provid port no....");
        exit(1);
    }
    portno = (int) atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        printf("SOCKET CREATION FAILED!!\n");
        exit(1);
    }
    printf("SOCKET CREATED...\n");
    server = gethostbyname(argv[1]);

    if(server==NULL)
    {
        printf("NO HOST FOUNDED!!\n");
        exit(1);
    }
    printf("SERVER FOUNDED!!\n");
    bzero((char*)&serv_addr, sizeof(serv_addr));

    /*
    #include <strings.h>
    void bcopy(const void *src, void *dest, size_t n);
    The bcopy() function copies n bytes from src to dest. The result is
    correct, even when both areas overlap.
    */
    bcopy((char*)server->h_addr_list, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    /*
    Generally connect() is used by client program to establish
    a connection to a remote machine(server)
    On successful returns 0
    On failure returns -1
    */
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("ERROR IN CONNECTION!!\n");
        exit(1);
    }

    bzero(write_buffer, 256);
    bzero(read_buffer, 256);
    printf("Type:: ");

    fgets(write_buffer, 255, stdin);
    //printf("%s",write_buffer);
    bytes_written = write(sockfd, write_buffer, strlen(write_buffer));
    printf("%d",bytes_written);
    bzero(read_buffer, 255);
    /*
    if(n < 0)
    {
        printf("ERROR writing to socket!!\n");
        exit(1);
    } */
    
    n = read(sockfd, read_buffer, 256);
    /*
    if(n < 0)
    {
        printf("ERROR READING FROM SOCKET!!\n");
        exit(1);
    }
    */
    printf("Server says: %s\n", read_buffer);

    return 0;
}

