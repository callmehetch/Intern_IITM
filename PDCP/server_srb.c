
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
// #include "rohc_initial.h"

int main()
{
	/*client-server connection establishment*/
	int welcomeSocket, newSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	if(listen(welcomeSocket,5)==0)
	printf("Listening\n");
	else
	printf("Error\n");
	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

	// char *FAKE_PAYLOAD = "hello, ROHC world!";
	// ROHC(FAKE_PAYLOAD);

	// Perform ROHC
	// Integrity protection and ciphering functions to be called here

	srand(time(0));
	int n = 15; //no. of PDU_SRB, sample

	uint16_t ibuf[] = {0,1,3,5,6,8,9,11,4,10,7,12,14,13,15}; //sample sequence no.s

	for (int i = 0; i < n; i++) //Should be replaced with receive function, receive from SDAP
	{
		PDU_data_SRB_t *ob = PDU_data_SRB_t_ini(); // initialisation of pointer for transmission purposes
		// ob->R_SN += (1<<12) - 1 - i;
		ob->R_SN |= ibuf[i]; // assign header - seq no. and R-bits
		printf("-----srb-----\n");

		PDU_data_SRB_t_data(ob); //data generation, should be replaced with received data
		
		printf("%d bytes data: ", ob->data_len);
		for(int i=0; i< ob->data_len; i++)
			printf("%02X ", *(ob->data + i));

		printf("\n");
		PDU_data_SRB_t_oph(ob); //add header
		printf("\nHEADER ADDED!\n");
		printf("%d bytes data: ", ob->data_len);
		
		uint8_t temp[1024]; // stores data to be sent

		for(int j=0; j< ob->data_len; j++)
		{
			printf("%02X ", *(ob->data + j));
			temp[j] = *(ob->data + j);
			//printf("\nhi..%02X\n", temp[j]);
		}

		//int z;
		//*(ob->data+j)=z;
		//strcpy(buffer,(ob->data+j));
		printf("\nSent %ld bytes successfully!\n", send(newSocket,temp,ob->data_len,0)); // sending packets
		printf("-----------------------------------------\n");

		//strcpy(buffer,"Hello World\n");
	   	//send(newSocket,buffer,13,0);
		// //Header Removed.
		// PDU_data_SRB_t_oph(ob);
		// printf("%d bytes data: ", ob->data_len);
		// for(int i=0; i< ob->data_len; i++)
		// 	printf("%02X ", *(ob->data + i));
		// printf("\n");
	}

	close(newSocket);
	close(welcomeSocket);
	return 0;
}
