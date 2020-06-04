
/*
	Date 	:	23-05-2019
	Authors	:	Viknesh, Ganesh, Nanda
	5G Testbed summer interns
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "PDCP_base.h"
#include <time.h>

int N = 1<<16; //N is max buff size - SN belongs to [0,N-1]

int main()
{
	clock_t start = clock(); // start of program

	PDU_data_DRB_1_t *A;
	A = (PDU_data_DRB_1_t*)malloc(N * sizeof(PDU_data_DRB_1_t)); //initialisation of temp. pointer for t-reordering purposes

	int l,i;
	
	for(i=0;i<N;i++)
	{
		(A + i)->DC_R_SN = 0; //initialisation for t-reordering purposes
	}

	int dbuf[N], discard_idx=0; //gotta be declared since cant declare STATIC array of variable size
	
	//alarm(1);
	// sleep(1);

	/*client-server connection establishment*/
	int clientSocket;
	uint8_t buffer[1024];

	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	//printf("hello");
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	//ob initilaise - has final obuf corres answer. this will be intialised and incremented as and when reqd.

	double loop = 0; //loop time variable. for checing purpose. not reqd for reorder function

	while(l = recv(clientSocket, buffer, 10, 0)) // receiving packets
	{
		printf("\nloop time %.6f\n", current_time(start) - loop);
		loop = current_time(start);
		
		PDU_data_DRB_1_t *p = PDU_data_DRB_1_t_ini();  //initialisation of temp. pointer to store data

		//storing information
		p->data = buffer;
		p->set = true;
		p->data_len = l;

		PDU_data_DRB_1_t_oph(p); //remove header

		reord_drb1(A,p,dbuf,&discard_idx,start); //reordering
	}

	printf("\nDiscarded SNs (discard_idx = %d)\n", discard_idx); // printing dbuf for cross checking. not needed finally.
	for (i = 0; i < discard_idx; ++i)
	{
		printf("%04X ", dbuf[i]);
	}

	close(clientSocket);
	return 0;
}