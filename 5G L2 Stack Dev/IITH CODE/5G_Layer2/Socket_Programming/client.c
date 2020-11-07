/* Author: Jaswanthi Mandalapu
   Date  : 16-04-2019
   File  : Socket Programming Client Function
   Organisation : IITM
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//Creating socket for communicating with server
short SocketCreate(void)
{
    short hsocket;
    printf("\n\rCreating a Socket");
    hsocket = socket(AF_INET, SOCK_STREAM, 0); //AF_INET indicates the communication is through ipv4 protocol ans SOCK_STREAM indicates that the communication is double sided byte communication
    return hsocket;
}

//Connecting with the server
int SocketConnect(int hsocket)
{
    int iRetval = -1;
    int serverport = 90190;
    struct sockaddr_in remote = {0};

    remote.sin_addr.s_addr = inet_addr("192.168.1.21");
    remote.sin_family      = AF_INET;
    remote.sin_port        = htons(serverport);

    iRetval = connect(hsocket, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
    return iRetval;
}

//Send the data to the server and set the time out of 20 seconds
int SocketSend(int hsocket, char* rqst, short lenrqst)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;       //timeout after 20sec
    tv.tv_usec = 0;

    if(setsockopt(hsocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv)) <0)
    {
        printf("\n\r Time Out");
        return -1;
    }

    shortRetval = send(hsocket, rqst, lenrqst, 0);
    return shortRetval;

}

//Receive data from the server
int SocketReceive(int hsocket, char* Rsp, short Rvcsize)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;         //20 sec timeout
    tv.tv_usec = 0;

    if(setsockopt(hsocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        printf("\n\r Time Out");
        return -1;
    }
    
    shortRetval = recv(hsocket, Rsp, Rvcsize, 0);
    printf("\n\rResponse %s\n",Rsp);
    return shortRetval;
}

//main driver function

int main(int argc, char *argv[])
{
    int hsocket, read_size;
    struct sockaddr_in server;
    char SendToServer[100] = {0};
    char server_reply[200] = {0};

    //creating socket
    hsocket =  SocketCreate();
    if(hsocket == -1)
    {
        printf("\n\r Could not create socket ");
        return 1;
    }

    printf("\n\r Socket Created Successfully");

    //Connect to the remote server
    if(SocketConnect(hsocket) < 0)
    {
        perror("\n\r Connection Failed");
        return 1;
    }

    printf("\n\r Created Socket SUccessfully");

    printf("\n\r Enter the message to be sent:");
    gets(SendToServer);

    //Send data to the server
    SocketSend(hsocket, SendToServer, strlen(SendToServer));

    //Receive the data from server
    read_size = SocketReceive(hsocket, server_reply, 200);

    printf("\n\rServer Response : %s \n\n",server_reply);

    close(hsocket);
    shutdown(hsocket,0);
    shutdown(hsocket,1);
    shutdown(hsocket,2);

    return 0;

}
    













 


