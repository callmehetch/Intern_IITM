
/*
	Date 	:	23-05-2019
	Authors	:	Viknesh, Ganesh, Nanda
	5G Testbed summer interns
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

double current_time(clock_t start); 

typedef struct PDU_data_SRB
{
	uint16_t R_SN; //4-12
	uint8_t *data;
	int data_len; 
	uint32_t MAC;
	bool set; //if false - no header. else header is there.
}PDU_data_SRB_t;

PDU_data_SRB_t* PDU_data_SRB_t_ini(void);  //initialisation
void PDU_data_SRB_t_data(PDU_data_SRB_t *ob);  //generate data
void PDU_data_SRB_t_oph(PDU_data_SRB_t *ob);  //operation on header based on set variable
void PDU_data_SRB_t_ciph(PDU_data_SRB_t *ob); //ciphering
void PDU_data_SRB_t_int_proc(PDU_data_SRB_t *ob); //integrity protection
// for reorder
void reord_srb(PDU_data_SRB_t *A, PDU_data_SRB_t *p, int dbuf[], int *discard_idx, clock_t start);
int zero_srb(PDU_data_SRB_t *A, int m, int n);

typedef struct PDU_data_DRB_1
{
	uint16_t DC_R_SN; //1-3-12
	uint8_t *data;
	int data_len; 
	uint32_t MAC;
	bool set;
}PDU_data_DRB_1_t;

PDU_data_DRB_1_t* PDU_data_DRB_1_t_ini(void);  //initialisation
void PDU_data_DRB_1_t_data(PDU_data_DRB_1_t *ob);  //generate data
void PDU_data_DRB_1_t_oph(PDU_data_DRB_1_t *ob);  //operation on header based on set variable
void PDU_data_DRB_1_t_ciph(PDU_data_DRB_1_t *ob); //ciphering
void PDU_data_DRB_1_t_int_proc(PDU_data_DRB_1_t *ob); //integrity protection
// for reorder
void reord_drb1(PDU_data_DRB_1_t *A, PDU_data_DRB_1_t *p, int dbuf[], int *discard_idx, clock_t start);
int zero_drb1(PDU_data_DRB_1_t *A, int m, int n);

typedef struct PDU_data_DRB_2
{
	uint8_t DC_R_SN; //1-5-2
	uint16_t SN;
	uint8_t *data;
	int data_len; 
	uint32_t MAC;
	bool set;
}PDU_data_DRB_2_t;

PDU_data_DRB_2_t* PDU_data_DRB_2_t_ini(void);  //initialisation
void PDU_data_DRB_2_t_data(PDU_data_DRB_2_t *ob);  //generate data
void PDU_data_DRB_2_t_oph(PDU_data_DRB_2_t *ob);  //operation on header based on set variable
void PDU_data_DRB_2_t_ciph(PDU_data_DRB_2_t *ob); //ciphering
void PDU_data_DRB_2_t_int_proc(PDU_data_DRB_2_t *ob); //integrity protection
//for reorder
void reord_drb2(PDU_data_DRB_2_t *A, PDU_data_DRB_2_t *p, int dbuf[], int *discard_idx, clock_t start);
int zero_drb2(PDU_data_DRB_2_t *A, int m, int n);

typedef struct PDU_control_Status
{
	uint8_t DC_Type_R; //1-3-4
	uint32_t FMC;
	uint8_t *BitMap;
	int BitMap_len;
}PDU_control_status_t;

PDU_control_status_t* PDU_control_status_t_ini(void);  //initialisation

typedef struct PDU_control_ROHC
{
	uint8_t DC_Type_R; //1-3-4
	uint8_t *ROHCfb;
	int ROHCfb_len;
}PDU_control_ROHC_t;

PDU_control_ROHC_t* PDU_control_ROHC_t_ini(void);  //initialisation


/*-----------------------------------initialisation------------------------------------*/
PDU_data_SRB_t* PDU_data_SRB_t_ini(void)  
{
	PDU_data_SRB_t* p = malloc(sizeof(PDU_data_SRB_t));
	p->R_SN = 0;
	p->data_len = 8;
	p->set = false;
	return p;
}

PDU_data_DRB_1_t* PDU_data_DRB_1_t_ini(void)  
{
	PDU_data_DRB_1_t* p = malloc(sizeof(PDU_data_DRB_1_t));
	p->DC_R_SN = 1<<15;
	p->data_len = 8;
	p->set = false;
	return p;
}

PDU_data_DRB_2_t* PDU_data_DRB_2_t_ini(void)  
{
	PDU_data_DRB_2_t* p = malloc(sizeof(PDU_data_DRB_2_t));
	p->DC_R_SN = 1<<7;
	p->SN = 0;
	p->data_len = 8;
	p->set = false;
	return p;
}

PDU_control_status_t* PDU_control_status_t_ini(void)  
{
	PDU_control_status_t* p = malloc(sizeof(PDU_control_status_t));
	p->DC_Type_R = 0;
	return p;
}

PDU_control_ROHC_t* PDU_control_ROHC_t_ini(void)  
{
	PDU_control_ROHC_t* p = malloc(sizeof(PDU_control_ROHC_t));
	p->DC_Type_R = 1<<4;
	return p; 
}


/*-----------------------------------data generation------------------------------------*/
void PDU_data_SRB_t_data(PDU_data_SRB_t *ob)  
{
	srand(time(0));
	// ob->data_len = rand()%8 + 1;
	ob->data_len = 8;

	uint8_t* p;
	p = (uint8_t*)malloc(ob->data_len * sizeof(uint8_t));

	for (int i = 0; i < ob->data_len; ++i)
		*(p + i) = rand() & 0xFF; 

	ob->data = p; 
}

void PDU_data_DRB_1_t_data(PDU_data_DRB_1_t *ob)  
{
	srand(time(0));
	// ob->data_len = rand()%8 + 1;
	ob->data_len = 8;

	uint8_t* p;
	p = (uint8_t*)malloc(ob->data_len * sizeof(uint8_t));

	for (int i = 0; i < ob->data_len; ++i)
		*(p + i) = rand() & 0xFF; 

	ob->data = p; 
}

void PDU_data_DRB_2_t_data(PDU_data_DRB_2_t *ob)  
{
	srand(time(0));
	// ob->data_len = rand()%8 + 1;
	ob->data_len = 8;

	uint8_t* p;
	p = (uint8_t*)malloc(ob->data_len * sizeof(uint8_t));

	for (int i = 0; i < ob->data_len; ++i)
		*(p + i) = rand() & 0xFF; 

	ob->data = p; 
}

/*------------------------- operation on header based on set variable----------------------------*/
void PDU_data_SRB_t_oph(PDU_data_SRB_t *ob)  
{
	if (!ob->set)
	{
		// Add Header
		ob->data -= 2;
		ob->data_len += 2;
		ob->set = true;
		*(ob->data) = ob->R_SN >> 8;
		*(ob->data + 1) = ob->R_SN & 0xFF;
	}
	else
	{
		// Remove Header
		ob->R_SN = (*(ob->data) << 8) | *(ob->data + 1);
		ob->data += 2;
		ob->data_len -= 2;
		ob->set = false;
	}
}

void PDU_data_DRB_1_t_oph(PDU_data_DRB_1_t *ob)  
{
	if (!ob->set)
	{
		// Add Header
		ob->data -= 2;
		ob->data_len += 2;
		ob->set = true;
		*(ob->data) = ob->DC_R_SN >> 8;
		*(ob->data + 1) = ob->DC_R_SN & 0xFF;
	}
	else
	{
		// Remove Header
		ob->DC_R_SN = (*(ob->data) << 8) | *(ob->data + 1);
		ob->data += 2;
		ob->data_len -= 2;
		ob->set = false;
	}
}

void PDU_data_DRB_2_t_oph(PDU_data_DRB_2_t *ob)  
{
	if (!ob->set)
	{
		// Add Header
		ob->data -= 3;
		ob->data_len += 3;
		ob->set = true;
		*(ob->data) = ob->DC_R_SN;
		*(ob->data + 1) = ob->SN >> 8;
		*(ob->data + 2) = ob->SN & 0xFF;
		}
	else
	{
		// Remove Header
		ob->DC_R_SN = *(ob->data);
		ob->SN = (*(ob->data + 1) << 8) | *(ob->data + 2);
		ob->data += 3;
		ob->data_len -= 3;
		ob->set = false;
	}
}

/*--------------------------------------- t-reordering ----------------------------------------------------*/

double current_time(clock_t start) //returns the duration between current time and start
{
	double seconds;
	clock_t end = clock();
	seconds = (double)(end-start)/CLOCKS_PER_SEC;
	return seconds;
}

int zero_srb(PDU_data_SRB_t *A, int m, int n) // m<n; returns the first unreceived index between m and n
{
	for (int i = m; i < n; ++i)
	{
		if ((A + i)->R_SN == (1<<16) - 1)
		{
			return i;
		}
	}
	return -1;
}

void reord_srb(PDU_data_SRB_t *A, PDU_data_SRB_t *p, int dbuf[], int *discard_idx, clock_t start)
{ 
	static int rx_deliv=0,rx_reord=0,rx_next=0,ct=0; //variables used in t-reordering & n is no of PDU_SRB transferred
	//dbuf is also reqd for t-reorder bt cant be declared in static. so passed as argument in reord_srb function
	//discard_idx = rx_deliv = rx_reord = rx_next = ct = n = 0;
	
	int i;
	int N = 1<<16; //N is max buff size - SN belongs to [0,N-1]

	static double seconds,t_reordering=0.000064; //variables to store various time durations 

	int rx_recd = p->R_SN; //received = sequence no.
	// printf("%04X %04X %04X %04X %d\n", rx_recd, rx_next, rx_deliv, rx_reord, ct);
	
	if (rx_recd < rx_deliv || (A + rx_recd)->R_SN != (1<<16) - 1) //discard condition
	{
		dbuf[(*discard_idx)++] = rx_recd; //store discarded sequence no.
	}
	else
	{
		// transfer p to (A + rx_recd) since its not discarded
		(A + rx_recd)->R_SN = p->R_SN;
		(A + rx_recd)->data_len = p->data_len;
		(A + rx_recd)->set = p->set;
		(A + rx_recd)->MAC = p->MAC;
		
		(A + rx_recd)->data = malloc((A + rx_recd)->data_len * sizeof(uint8_t));

		for (int j = 0; j < (A + rx_recd)->data_len; ++j)
		{
			*((A + rx_recd)->data + j) = *(p->data +j);
		}

		// printf("Seq. no. = %04X\n", (A + rx_recd)->R_SN);
	}
	if (rx_recd >= rx_next)
	{
		rx_next = rx_recd + 1; //update rx_next based on rx_recd
	}
	// printf("%04X %04X %04X %04X %d\n", rx_recd, rx_next, rx_deliv, rx_reord, ct);
	if (rx_recd == rx_deliv)
	{
		// printf("Inside rx_recd=rx_deliv\n");
		for (i = rx_deliv; i < zero_srb(A,rx_deliv,N); i++)
		{
			// transfer (A + i) to ob since its getting transmitted 
			PDU_data_SRB_t *ob = PDU_data_SRB_t_ini(); //initialisation of temp. pointer to store data
			ob->R_SN = (A + i)->R_SN;
			ob->data = (A + i)->data;
			ob->data_len = (A + i)->data_len;
			ob->set = (A + i)->set;
			ob->MAC = (A + i)->MAC;
			// Integrity protection and ciphering functions to be called here
			printf("\nSN:%04X\n%d bytes data: \n", ob->R_SN, ob->data_len); 
			for(int j=0; j< ob->data_len; j++)
				printf("%02X ", *(ob->data + j));
			printf("\n------------------------------------------------\n");
			// Send to RLC
		}
		rx_deliv = i;// update rx_deliv
	}
	// printf("%04X %04X %04X %04X %d\n", rx_recd, rx_next, rx_deliv, rx_reord, ct);
	
	/*From here, t-reordering algorithm is implemented*/
	if ((ct == 1) && (rx_deliv >= rx_reord))
	{
		ct = 0;
	}
	if ((ct == 0) && (rx_deliv < rx_next))
	{
		rx_reord = rx_next;
		ct = 1;
		seconds = current_time(start);
	}
	// printf("%04X %04X %04X %04X %d\n", rx_recd, rx_next, rx_deliv, rx_reord, ct);
	if ((ct == 1) && ((current_time(start)-seconds) > t_reordering))
	{
		for (i = rx_deliv; i < rx_reord; i++)
		{
			if ((A + i)->R_SN != (1<<16) - 1)
			{
				// transfer (A + i) to ob since its getting transmitted 
				PDU_data_SRB_t *ob = PDU_data_SRB_t_ini(); //initialisation of temp. pointer to store data
				ob->R_SN = (A + i)->R_SN;
				ob->data = (A + i)->data;
				ob->data_len = (A + i)->data_len;
				ob->set = (A + i)->set;
				ob->MAC = (A + i)->MAC;
				// Integrity protection and ciphering functions to be called here
				printf("SN:%04X\n%d bytes data: \n", ob->R_SN, ob->data_len);
				for(int j=0; j< ob->data_len; j++)
					printf("%02X ", *(ob->data + j));
				printf("\n------------------------------------------------\n");
				// Send to RLC
			}
		}
		rx_deliv = zero_srb(A,rx_reord,N);
		for (i = rx_reord; i < rx_deliv; i++)
		{
			// transfer (A + i) to ob since its getting transmitted 
			PDU_data_SRB_t *ob = PDU_data_SRB_t_ini(); //initialisation of temp. pointer to store data
			ob->R_SN = (A + i)->R_SN;
			ob->data = (A + i)->data;
			ob->data_len = (A + i)->data_len;
			ob->set = (A + i)->set;
			ob->MAC = (A + i)->MAC;
			// Integrity protection and ciphering functions to be called here
			printf("SN:%04X\n%d bytes data: \n", ob->R_SN, ob->data_len);
			for(int j=0; j< ob->data_len; j++)
				printf("%02X ", *(ob->data + j));
			printf("\n------------------------------------------------\n");
			// Send to RLC
		}
		ct = 0;
	}
	if (ct == 0)
	{
		rx_reord = rx_next;
	}
}

int zero_drb1(PDU_data_DRB_1_t *A, int m, int n) // m<n; returns the first unreceived index between m and n
{
	for (int i = m; i < n; ++i)
	{
		if ((A + i)->DC_R_SN == 0)
		{
			// printf("i=%04X in zero_drb1() and %04X\n", i, (1<<16) - 1);
			return i;
		}
	}
	return -1;
}

void reord_drb1(PDU_data_DRB_1_t *A, PDU_data_DRB_1_t *p, int dbuf[], int *discard_idx, clock_t start)
{
	static int rx_deliv=0,rx_reord=0,rx_next=0,ct=0; //variables used in t-reordering & n is no of PDU_SRB transferred
	//dbuf is also reqd for t-reorder bt cant be declared in static. so passed as argument in reord_srb function
	//discard_idx = rx_deliv = rx_reord = rx_next = ct = n = 0;
	
	int i;
	int N = 1<<16; //N is max buff size - SN belongs to [0,N-1]

	static double seconds,t_reordering=0.000064; //variables to store various time durations 

	int rx_recd = p->DC_R_SN ^ 0x8000; //received = sequence no.
	// printf("%04X %04X %04X %04X %d\n", rx_recd, rx_next, rx_deliv, rx_reord, ct);
	
	if (rx_recd < rx_deliv || (A + rx_recd)->DC_R_SN != 0) //discard condition
	{
		dbuf[(*discard_idx)++] = rx_recd ^ 0x8000; //store discarded sequence no.
	}
	else
	{
		// transfer p to (A + rx_recd) since its not discarded
		(A + rx_recd)->DC_R_SN = p->DC_R_SN;
		// (A + rx_recd)->data = p->data; // error updating data
		(A + rx_recd)->data_len = p->data_len;
		(A + rx_recd)->set = p->set;
		(A + rx_recd)->MAC = p->MAC;
		
		(A + rx_recd)->data = malloc((A + rx_recd)->data_len * sizeof(uint8_t));

		for (int j = 0; j < (A + rx_recd)->data_len; ++j)
		{
			*((A + rx_recd)->data + j) = *(p->data +j);
		}

		// printf("Seq. No. = %04X\n", (A + rx_recd)->DC_R_SN);
	}
	if (rx_recd >= rx_next)
	{
		rx_next = rx_recd + 1; //update rx_next based on rx_recd
	}
	if (rx_recd == rx_deliv)
	{
		// printf("Entered rx_recd=rx_deliv\n");
		for (i = rx_deliv; i < zero_drb1(A,rx_deliv,N); i++)
		{
			// transfer (A + i) to ob since its getting transmitted 
			PDU_data_DRB_1_t *ob = PDU_data_DRB_1_t_ini(); //initialisation of temp. pointer to store data
			ob->DC_R_SN = (A + i)->DC_R_SN;
			ob->data = (A + i)->data;
			ob->data_len = (A + i)->data_len;
			ob->set = (A + i)->set;
			ob->MAC = (A + i)->MAC;
			// Integrity protection and ciphering functions to be called here
			printf("\nSN:%04X\n%d bytes data: \n", ob->DC_R_SN, ob->data_len); // check  first "\n" of this line - working?
			for(int j=0; j< ob->data_len; j++)
				printf("%02X ", *(ob->data + j));
			printf("\n------------------------------------------------\n");
			//send to RLC
		}
		rx_deliv = i;
	}
	
	/*From here, t-reordering algorithm is implemented*/
	if ((ct == 1) && (rx_deliv >= rx_reord))
	{
		ct = 0;
	}
	if ((ct == 0) && (rx_deliv < rx_next))
	{
		rx_reord = rx_next;
		ct = 1;
		seconds = current_time(start);
	}
	if ((ct == 1) && ((current_time(start)-seconds) > t_reordering))
	{
		for (i = rx_deliv; i < rx_reord; i++)
		{
			if ((A + i)->DC_R_SN != 0)
			{
				// transfer (A + i) to ob since its getting transmitted 
				PDU_data_DRB_1_t *ob = PDU_data_DRB_1_t_ini(); //initialisation of temp. pointer to store data
				ob->DC_R_SN = (A + i)->DC_R_SN;
				ob->data = (A + i)->data;
				ob->data_len = (A + i)->data_len;
				ob->set = (A + i)->set;
				ob->MAC = (A + i)->MAC;
				// Integrity protection and ciphering functions to be called here
				printf("SN:%04X\n%d bytes data: \n", ob->DC_R_SN, ob->data_len);
				for(int j=0; j< ob->data_len; j++)
					printf("%02X ", *(ob->data + j));
				printf("\n------------------------------------------------\n");
				//send to RLC
			}
		}
		rx_deliv = zero_drb1(A,rx_reord,N);
		for (i = rx_reord; i < rx_deliv; i++)
		{
			// transfer (A + i) to ob since its getting transmitted 
			PDU_data_DRB_1_t *ob = PDU_data_DRB_1_t_ini(); //initialisation of temp. pointer to store data
			ob->DC_R_SN = (A + i)->DC_R_SN;
			ob->data = (A + i)->data;
			ob->data_len = (A + i)->data_len;
			ob->set = (A + i)->set;
			ob->MAC = (A + i)->MAC;
			// Integrity protection and ciphering functions to be called here
			printf("SN:%04X\n%d bytes data: \n", ob->DC_R_SN, ob->data_len);
			for(int j=0; j< ob->data_len; j++)
				printf("%02X ", *(ob->data + j));
			printf("\n------------------------------------------------\n");
			//send to RLC
		}
		ct = 0;
	}
	if (ct == 0)
	{
		rx_reord = rx_next;
	}
}

int zero_drb2(PDU_data_DRB_2_t *A, int m, int n) // m<n;  returns the first unreceived index between m and n
{
	for (int i = m; i < n; ++i)
	{
		if ((A + i)->DC_R_SN == 0)
		{
			// printf("i=%04X in zero_drb2() and %04X\n", i, (1<<8) - 1);
			return i;
		}
	}
	return -1;
}

void reord_drb2(PDU_data_DRB_2_t *A, PDU_data_DRB_2_t *p, int dbuf[], int *discard_idx, clock_t start)
{
	static int rx_deliv=0,rx_reord=0,rx_next=0,ct=0; //variables used in t-reordering & n is no of PDU_SRB transferred
	//dbuf is also reqd for t-reorder bt cant be declared in static. so passed as argument in reord_srb function
	//discard_idx = rx_deliv = rx_reord = rx_next = ct = n = 0;
	
	int i;
	int N = 1<<18; //N is max buff size - SN belongs to [0,N-1]

	static double seconds,t_reordering=0.000064; //variables to store various time durations 

	int rx_recd = ((p->DC_R_SN<<16) | p->SN) ^ 0x800000; //received = sequence no.
	// printf("%04X %04X %04X %04X %d\n", rx_recd, rx_next, rx_deliv, rx_reord, ct);
	
	if (rx_recd < rx_deliv || (A + rx_recd)->DC_R_SN != 0) //discard condition
	{
		dbuf[(*discard_idx)++] = rx_recd ^ 0x800000; //store discarded sequence no.
	}
	else
	{
		// transfer p to (A + rx_recd) since its not discarded
		(A + rx_recd)->DC_R_SN = p->DC_R_SN;
		(A + rx_recd)->SN = p->SN;
		// (A + rx_recd)->data = p->data; // error updating data
		(A + rx_recd)->data_len = p->data_len;
		(A + rx_recd)->set = p->set;
		(A + rx_recd)->MAC = p->MAC;
		
		(A + rx_recd)->data = malloc((A + rx_recd)->data_len * sizeof(uint8_t));

		for (int j = 0; j < (A + rx_recd)->data_len; ++j)
		{
			*((A + rx_recd)->data + j) = *(p->data +j);
		}

		// printf("Seq. No. = %06X\n", ((A + rx_recd)->DC_R_SN))<<16 + (A + rx_recd)->SN);
	}
	if (rx_recd >= rx_next)
	{
		rx_next = rx_recd + 1; //update rx_next based on rx_recd
	}
	if (rx_recd == rx_deliv)
	{
		// printf("Entered rx_recd=rx_deliv\n");
		for (i = rx_deliv; i < zero_drb2(A,rx_deliv,N); i++)
		{
			// transfer (A + i) to ob since its getting transmitted 
			PDU_data_DRB_2_t *ob = PDU_data_DRB_2_t_ini(); //initialisation of temp. pointer to store data
			ob->DC_R_SN = (A + i)->DC_R_SN;
			ob->SN = (A + i)->SN;
			ob->data = (A + i)->data;
			ob->data_len = (A + i)->data_len;
			ob->set = (A + i)->set;
			ob->MAC = (A + i)->MAC;
			// Integrity protection and ciphering functions to be called here
			printf("\nSN:%06X\n%d bytes data: \n", (ob->DC_R_SN<<16) + ob->SN, ob->data_len); // check  first "\n" of this line - working?
			for(int j=0; j< ob->data_len; j++)
				printf("%02X ", *(ob->data + j));
			printf("\n------------------------------------------------\n");
			//send to RLC
		}
		rx_deliv = i;
	}
	
	/*From here, t-reordering algorithm is implemented*/
	if ((ct == 1) && (rx_deliv >= rx_reord))
	{
		ct = 0;
	}
	if ((ct == 0) && (rx_deliv < rx_next))
	{
		rx_reord = rx_next;
		ct = 1;
		seconds = current_time(start);
	}
	if ((ct == 1) && ((current_time(start)-seconds) > t_reordering))
	{
		for (i = rx_deliv; i < rx_reord; i++)
		{
			if ((A + i)->DC_R_SN != 0)
			{
				// transfer (A + i) to ob since its getting transmitted 
				PDU_data_DRB_2_t *ob = PDU_data_DRB_2_t_ini(); //initialisation of temp. pointer to store data
				ob->DC_R_SN = (A + i)->DC_R_SN;
				ob->SN = (A + i)->SN;
				ob->data = (A + i)->data;
				ob->data_len = (A + i)->data_len;
				ob->set = (A + i)->set;
				ob->MAC = (A + i)->MAC;
				// Integrity protection and ciphering functions to be called here
				printf("\nSN:%06X\n%d bytes data: \n", (ob->DC_R_SN<<16) + ob->SN, ob->data_len);
				for(int j=0; j< ob->data_len; j++)
					printf("%02X ", *(ob->data + j));
				printf("\n------------------------------------------------\n");
				//send to RLC
			}
		}
		rx_deliv = zero_drb2(A,rx_reord,N);
		for (i = rx_reord; i < rx_deliv; i++)
		{
			// transfer (A + i) to ob since its getting transmitted 
			PDU_data_DRB_2_t *ob = PDU_data_DRB_2_t_ini(); //initialisation of temp. pointer to store data
			ob->DC_R_SN = (A + i)->DC_R_SN;
			ob->SN = (A + i)->SN;
			ob->data = (A + i)->data;
			ob->data_len = (A + i)->data_len;
			ob->set = (A + i)->set;
			ob->MAC = (A + i)->MAC;
			// Integrity protection and ciphering functions to be called here
			printf("\nSN:%06X\n%d bytes data: \n", (ob->DC_R_SN<<16) + ob->SN, ob->data_len);
			for(int j=0; j< ob->data_len; j++)
				printf("%02X ", *(ob->data + j));
			printf("\n------------------------------------------------\n");
			//send to RLC
		}
		ct = 0;
	}
	if (ct == 0)
	{
		rx_reord = rx_next;
	}
}
