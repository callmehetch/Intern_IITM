/* Author: Jaswanthi Mandalapu
  Date  : 16-04-2019
  File  : Socket Programming Client Function
  Organisation : IITM
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

short SocketCreate(void)
{
    short hsocket;
   // printf("\n\r Create the socket");

    hsocket = socket(AF_INET, SOCK_STREAM, 0);
    return hsocket;
}

int BindCreatedSocket(int hsocket)
{
    int iRetval = -1;
    int clientport  = 90190;
    struct sockaddr_in remote = {0};

    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = htons(clientport);
    iRetval = bind(hsocket, (struct sockaddr *)&remote, sizeof(remote));

    return iRetval;

}

int main(int argc, char *argv[])
{
    int socket_desc, sock, clientlen, read_size;
    struct sockaddr_in server, client;
    char client_message[200] = {0};
    char message[100] = {0};
    const char *pMessage = "hello Google.com";

    //creating socket
    socket_desc = SocketCreate();
    if(socket_desc == -1)
    {
        printf("\n\r Could not create socket");
        return 1;
    }

    printf("\n\r Socket Created");


    //Bind
    if(BindCreatedSocket(socket_desc) < 0)
    {
        //print the error message
        perror("bind failed");
        return 1;
    }

    printf("\n\r bind done");

    //listen
    listen(socket_desc, 3);

    //accept incoming connection
    while(1)
    {

        printf("\n\rWaiting for incoming connections");
        clientlen = sizeof(struct sockaddr_in);
        
        //accept connection from an incoming client
        sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&clientlen);
        if(sock <0)
        {
            perror("accept failed");
            return 1;
        }

        printf("\n\rConnection accepted");

        memset(client_message, '\0', sizeof(client_message));
        memset(message, '\0', sizeof(message));
        if(recv(sock, client_message, 200,0)<0)
        {
            printf("recv failed");
            break;
        }

        printf("\n\rClient Reply : %s\n", client_message);
        
        if(strcmp(pMessage, client_message)==0)
        {
            strcpy(message, "Hi there...!");
        }

        else
        {
            strcpy(message, "Invalid message");
        }

        //send some data
        if(send(sock, message, strlen(message), 0) < 0)
        {
            printf("send failed");
            return 1;
        }

        close(sock);
        sleep(1);
        }
    return 0;
}









