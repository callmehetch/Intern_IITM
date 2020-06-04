
/*
	Date 	:	23-05-2019
	Authors	:	Viknesh, Ganesh
	5G Testbed summer interns
*/

#include <stdio.h>
#include <stdlib.h>
#include "new.h"

#define N 1<<18 //N is max buff size - assuming SN belongs to [0,N]

int zero(int A[N], int m, int n) // m<n
{
	for (int i = m; i < n; ++i)
	{
		if (A[i] == 0)
		{
			return i;
		}
	}
	return -1;
}

int main()
{

	int i,obuf[N],dbuf[N],A[N],t_reord=4;
	//rbuf - ibuf - input buffer, obuf - output, dbuf-discard, rbuf-read, tbuf - temp
		
	for(i=0;i<N;i++)
	{
		A[i] = 0;
	}

	int ibuf[] = {0,1,3,5,6,8,9,11,4,10,7,12,14,13,15};

	int in_idx,out_idx,discard_idx,rx_deliv,rx_reord,rx_next,rx_recd,ct;
	in_idx = out_idx = discard_idx = rx_deliv = rx_reord = rx_next = ct = 0;
	//assuming 0 is starting SN of pkts to be received.

	for (in_idx = 0; in_idx < sizeof(ibuf)/sizeof(int); in_idx++) ///<N or <=N ?
	{
		rx_recd = ibuf[in_idx];
		
		if (rx_recd < rx_deliv || A[rx_recd] == 1)
		{
			dbuf[discard_idx++] = rx_recd;
		}
		else
		{
			A[rx_recd] = 1;
		}
		if (rx_recd >= rx_next)
		{
			rx_next = rx_recd + 1;
		}
		if (rx_recd == rx_deliv)
		{
			for (i = rx_deliv; i < zero(A,rx_deliv,N); i++)
			{
				obuf[out_idx++] = i;
			}
			rx_deliv = i;
		}
		if ((ct != 0) && (rx_deliv >= rx_reord))
		{
			ct = 0;
		}
		if (ct != 0)
		{
			ct++;
		}
		if ((ct == 0) && (rx_deliv < rx_next))
		{
			rx_reord = rx_next;
			ct = 1;
		}
		if (ct == t_reord)
		{
			for (i = rx_deliv; i < rx_reord; i++)
			{
				if (A[i] == 0)
				{
					A[i] = -1;
				}
				else
				{
					obuf[out_idx++] = i;
				}
			}
			rx_deliv = zero(A,rx_reord,N);
			for (i = rx_reord; i < rx_deliv; i++)
			{
				obuf[out_idx++] = i;
			}
			ct = 0;
		}
		if (ct == 0)
		{
			rx_reord = rx_next;
		}
	
		printf("\nrx_recd = %d \t rx_next = %d \t rx_deliv = %d \t rx_reord = %d \t ct = %d\n", rx_recd, rx_next, rx_deliv, rx_reord, ct);

		printf("output buffer : ");
		for (i = 0; i < out_idx; i++)
		{
			printf("%d ", obuf[i]);
		}
		printf("\n");
		printf("discard buffer : ");
		for (i = 0; i < discard_idx; i++)
		{
			printf("%d ", dbuf[i]);
		}
		printf("\n");

	}

	printf("\nA\n");
	for (i = 0; i < 100; i++)
	{
		printf("%d ", A[i]);
	}

	printf("\n");

}









