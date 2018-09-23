#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>


// A function to receive message from client and write back to client
void doStuff(int sock)
{
    int n;
    char read_buffer[256];
    char write_buffer[256];
    char welcome_msg[256] = "Welcome to chat room";

    int isJustConnected = 1;

    bzero(read_buffer, 256);
    bzero(write_buffer, 256);

    n = read(sock, read_buffer, 256);
    
    // Check read() function
    if(n < 0)
    {
        printf("Error reading from socket!!\n");
        return;
    }

    printf("Client says->> %s\n", read_buffer);

    if(isJustConnected == 1)
    {
        n = write(sock, welcome_msg, strlen(welcome_msg));
    }
        
}


int main(int argc, char *argv[])
{

 int sockfd;     // sockfd = socket file descriptor, holds server's socket
 int newsockfd;  // it will be used in accept() to establis connection between server and client
 int portno;     // port number is supplied by command line, here port number is server's port number
 struct sockaddr_in server_addr, client_addr;   
 /* 
  sockaddr_in - is a protocol address structure

  struct sockaddr_in
  {
      unsigned short sin_family;        // set to AF_INET
      unsigned short sin_port;          // port no. to bind to
      struct in_addr sin_addr;          // according to KD IP address
      char sin_zero[8]l                 // DON'T KNOW THE PURPOSE
  };
 
 */

 // Checking whether user provided a port number
 if(sizeof(argv) < 2)
 {
     printf("NO PORT PROVIDED...\n");
     exit(2);
 }

 sockfd = socket(AF_INET, SOCK_STREAM, 0);

 if(sockfd == -1)
 {
    printf("SOCKET CREATION FAILED!!\n");
    exit(1);
 }
 printf("SOCKET CREATED SUCCESSFULLY!!%d\n", sockfd);


/*
    #include <strings.h>
    void bzero(void *s, size_t n);
    
    The bzero() function shall place n zero-valued bytes in the area pointed to by s.

 */
 bzero((char*)&server_addr, sizeof(server_addr));
 
 /*
 The C library function int atoi(const char *str)
 converts the string argument str to an integer (type int).
 */
 
 portno = (int)atoi(argv[1]);    // Receive's the port no. passed by the user
 server_addr.sin_family = AF_INET;  // IPV4
 server_addr.sin_addr.s_addr = INADDR_ANY;      // SERVER'S IP ADDRESS
 /*
 The htons function converts a unsigned short integer
 from host to TCP/IP network byte order (which is big-endian).
 */
 server_addr.sin_port = htons(portno); // convert the port no. to network byte order

 // checking the binding is successful or not
 if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
     printf("ERROR IN BINDING PROCESS!!\n");
     exit(1);
 }
 printf("Binding successful...\n");
 
 // CREATING LISTENING QUEUE
 /*
  int listen(inf sockfd, int Max_Connection)
  First argument is socket and second is maximum connection
  On success returns 0
  On failure returns -1
 */
 listen(sockfd, 2);
 printf("Waiting for client....\n");

 // accepting client
 /*
  int accept(int socket, struct sockaddr *clientAddress, int *clientAddressLength)
  On success returns new socket descriptor
  On failure returns -1
 */
 newsockfd = (int)accept(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr));
 if(newsockfd == -1)
 {
     printf("CONNECTION IS NOT ESTABLISHED!!\n");
     exit(1);
 }
 doStuff(newsockfd);
 close(sockfd);

return 0;
}
