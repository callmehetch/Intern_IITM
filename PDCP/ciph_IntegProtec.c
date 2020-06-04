
/*
	Date 	:	23-05-2019
	Authors	:	Viknesh, Ganesh, Nanda
	5G Testbed summer interns
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*Snow 3G Ciphering outline*/

/* LFSR */

uint32_t LFSR_S0 = 0x00;
uint32_t LFSR_S1 = 0x00;
uint32_t LFSR_S2 = 0x00;
uint32_t LFSR_S3 = 0x00;
uint32_t LFSR_S4 = 0x00;
uint32_t LFSR_S5 = 0x00;
uint32_t LFSR_S6 = 0x00;
uint32_t LFSR_S7 = 0x00;
uint32_t LFSR_S8 = 0x00;
uint32_t LFSR_S9 = 0x00;
uint32_t LFSR_S10 = 0x00;
uint32_t LFSR_S11 = 0x00;
uint32_t LFSR_S12 = 0x00;
uint32_t LFSR_S13 = 0x00;
uint32_t LFSR_S14 = 0x00;
uint32_t LFSR_S15 = 0x00;

/* FSM */

uint32_t FSM_R1 = 0x00;
uint32_t FSM_R2 = 0x00;
uint32_t FSM_R3 = 0x00;

/* Rijndael S-box SR */

uint8_t SR[256] = {
0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16
};

/* S-box SQ */

uint8_t SQ[256] = {
0x25,0x24,0x73,0x67,0xD7,0xAE,0x5C,0x30,0xA4,0xEE,0x6E,0xCB,0x7D,0xB5,0x82,0xDB,
0xE4,0x8E,0x48,0x49,0x4F,0x5D,0x6A,0x78,0x70,0x88,0xE8,0x5F,0x5E,0x84,0x65,0xE2,
0xD8,0xE9,0xCC,0xED,0x40,0x2F,0x11,0x28,0x57,0xD2,0xAC,0xE3,0x4A,0x15,0x1B,0xB9,
0xB2,0x80,0x85,0xA6,0x2E,0x02,0x47,0x29,0x07,0x4B,0x0E,0xC1,0x51,0xAA,0x89,0xD4,
0xCA,0x01,0x46,0xB3,0xEF,0xDD,0x44,0x7B,0xC2,0x7F,0xBE,0xC3,0x9F,0x20,0x4C,0x64,
0x83,0xA2,0x68,0x42,0x13,0xB4,0x41,0xCD,0xBA,0xC6,0xBB,0x6D,0x4D,0x71,0x21,0xF4,
0x8D,0xB0,0xE5,0x93,0xFE,0x8F,0xE6,0xCF,0x43,0x45,0x31,0x22,0x37,0x36,0x96,0xFA,
0xBC,0x0F,0x08,0x52,0x1D,0x55,0x1A,0xC5,0x4E,0x23,0x69,0x7A,0x92,0xFF,0x5B,0x5A,
0xEB,0x9A,0x1C,0xA9,0xD1,0x7E,0x0D,0xFC,0x50,0x8A,0xB6,0x62,0xF5,0x0A,0xF8,0xDC,
0x03,0x3C,0x0C,0x39,0xF1,0xB8,0xF3,0x3D,0xF2,0xD5,0x97,0x66,0x81,0x32,0xA0,0x00,
0x06,0xCE,0xF6,0xEA,0xB7,0x17,0xF7,0x8C,0x79,0xD6,0xA7,0xBF,0x8B,0x3F,0x1F,0x53,
0x63,0x75,0x35,0x2C,0x60,0xFD,0x27,0xD3,0x94,0xA5,0x7C,0xA1,0x05,0x58,0x2D,0xBD,
0xD9,0xC7,0xAF,0x6B,0x54,0x0B,0xE0,0x38,0x04,0xC8,0x9D,0xE7,0x14,0xB1,0x87,0x9C,
0xDF,0x6F,0xF9,0xDA,0x2A,0xC4,0x59,0x16,0x74,0x91,0xAB,0x26,0x61,0x76,0x34,0x2B,
0xAD,0x99,0xFB,0x72,0xEC,0x33,0x12,0xDE,0x98,0x3B,0xC0,0x9B,0x3E,0x18,0x10,0x3A,
0x56,0xE1,0x77,0xC9,0x1E,0x9E,0x95,0xA3,0x90,0x19,0xA8,0x6C,0x09,0xD0,0xF0,0x86
};

uint8_t MULx(uint8_t V, uint8_t c)
{
	if ((V>>7) == 1)
	{
		return ((V<<1) ^ c);
	}
	else
		return (V<<1);
}

uint8_t MULxPOW(uint8_t V, int i, uint8_t c)
{
	if (i == 0)
	{
		return V;
	}
	else
		return MULx(MULxPOW(V, i-1, c), c);
}

uint32_t MULa(uint8_t c)
{
	return ((MULxPOW(c, 23, 169) << 24) | (MULxPOW(c, 245, 169) << 16) | (MULxPOW(c, 48, 169) << 8) | MULxPOW(c, 239, 169));
}

uint32_t DIVa(uint8_t c)
{
	return ((MULxPOW(c, 16, 169) << 24) | (MULxPOW(c, 39, 169) << 16) | (MULxPOW(c, 6, 169) << 8) | MULxPOW(c, 64, 169));
}

uint32_t S1(uint32_t w)
{
	uint8_t w0 = w>>24;
	uint8_t w1 = ((w>>16) & 0xFF);
	uint8_t w2 = ((w>>8) & 0xFF);
	uint8_t w3 = (w & 0xFF);

	uint8_t r0 = SR[w1] ^ SR[w2] ^ SR[w3] ^ MULx(SR[w3], 27) ^ MULx(SR[w0], 27);
	uint8_t r1 = SR[w2] ^ SR[w3] ^ SR[w0] ^ MULx(SR[w0], 27) ^ MULx(SR[w1], 27);
	uint8_t r2 = SR[w3] ^ SR[w0] ^ SR[w1] ^ MULx(SR[w1], 27) ^ MULx(SR[w1], 27);
	uint8_t r3 = SR[w0] ^ SR[w1] ^ SR[w2] ^ MULx(SR[w2], 27) ^ MULx(SR[w3], 27);

	return ((r0<<24) | (r1<<16) | (r2<<8) | r3);
}

uint32_t S2(uint32_t w)
{
	uint8_t w0 = w>>24;
	uint8_t w1 = ((w>>16) & 0xFF);
	uint8_t w2 = ((w>>8) & 0xFF);
	uint8_t w3 = (w & 0xFF);

	uint8_t r0 = SQ[w1] ^ SQ[w2] ^ SQ[w3] ^ MULx(SQ[w3], 105) ^ MULx(SQ[w0], 105);
	uint8_t r1 = SQ[w2] ^ SQ[w3] ^ SQ[w0] ^ MULx(SQ[w0], 105) ^ MULx(SQ[w1], 105);
	uint8_t r2 = SQ[w3] ^ SQ[w0] ^ SQ[w1] ^ MULx(SQ[w1], 105) ^ MULx(SQ[w1], 105);
	uint8_t r3 = SQ[w0] ^ SQ[w1] ^ SQ[w2] ^ MULx(SQ[w2], 105) ^ MULx(SQ[w3], 105);

	return ((r0<<24) | (r1<<16) | (r2<<8) | r3);
}

void ClockLFSRInitializationMode(uint32_t F)
{
	uint32_t v = (LFSR_S0 << 8) ^ MULa(LFSR_S0 >> 24) ^ LFSR_S2 ^ (LFSR_S11 >> 8) ^ DIVa(LFSR_S11 && 0xFF) ^ F;  

	LFSR_S0 = LFSR_S1;
	LFSR_S1 = LFSR_S2;
	LFSR_S2 = LFSR_S3;
	LFSR_S3 = LFSR_S4;
	LFSR_S4 = LFSR_S5;
	LFSR_S5 = LFSR_S6;
	LFSR_S6 = LFSR_S7;
	LFSR_S7 = LFSR_S8;
	LFSR_S8 = LFSR_S9;
	LFSR_S9 = LFSR_S10;
	LFSR_S10 = LFSR_S11;
	LFSR_S11 = LFSR_S12;
	LFSR_S12 = LFSR_S13;
	LFSR_S13 = LFSR_S14;
	LFSR_S14 = LFSR_S15;
	LFSR_S15 = v;
}

void ClockLFSRKeyStreamMode(void)
{
	uint32_t v = (LFSR_S0 << 8) ^ MULa(LFSR_S0 >> 24) ^ LFSR_S2 ^ (LFSR_S11 >> 8) ^ DIVa(LFSR_S11 && 0xFF);  
	
	LFSR_S0 = LFSR_S1;
	LFSR_S1 = LFSR_S2;
	LFSR_S2 = LFSR_S3;
	LFSR_S3 = LFSR_S4;
	LFSR_S4 = LFSR_S5;
	LFSR_S5 = LFSR_S6;
	LFSR_S6 = LFSR_S7;
	LFSR_S7 = LFSR_S8;
	LFSR_S8 = LFSR_S9;
	LFSR_S9 = LFSR_S10;
	LFSR_S10 = LFSR_S11;
	LFSR_S11 = LFSR_S12;
	LFSR_S12 = LFSR_S13;
	LFSR_S13 = LFSR_S14;
	LFSR_S14 = LFSR_S15;
	LFSR_S15 = v;
}

uint32_t clockFSM(void)
{
	uint32_t F = ( ( LFSR_S15 + FSM_R1 ) & 0xFFFFFFFF ) ^ FSM_R2 ;
	uint32_t r = ( FSM_R2 + ( FSM_R3 ^ LFSR_S5 ) ) & 0xFFFFFFFF ;
	FSM_R3 = S2(FSM_R2);
	FSM_R2 = S1(FSM_R1);
	FSM_R1 = r;
	return F;
}

void Initialize(uint32_t k[4], uint32_t IV[4])
{
	LFSR_S0 = k[0] ^ 0xFFFFFFFF;
	LFSR_S1 = k[1] ^ 0xFFFFFFFF;
	LFSR_S2 = k[2] ^ 0xFFFFFFFF;
	LFSR_S3 = k[3] ^ 0xFFFFFFFF;
	LFSR_S4 = k[0];
	LFSR_S5 = k[1];
	LFSR_S6 = k[2];
	LFSR_S7 = k[3];
	LFSR_S8 = k[0] ^ 0xFFFFFFFF;
	LFSR_S9 = k[1] ^ 0xFFFFFFFF ^ IV[3];
	LFSR_S10 = k[2] ^ 0xFFFFFFFF ^ IV[2];
	LFSR_S11 = k[3] ^ 0xFFFFFFFF;
	LFSR_S12 = k[0] ^ IV[1];
	LFSR_S13 = k[1];
	LFSR_S14 = k[2];
	LFSR_S15 = k[3] ^ IV[0];

	FSM_R1 = 0;
	FSM_R2 = 0;
	FSM_R3 = 0;

	for(int i=0; i<32; i++)
	{
		ClockLFSRInitializationMode(clockFSM());
	}
}

void GenerateKeystream(uint32_t n, uint32_t *ks)
{
	ClockFSM();
	ClockLFSRKeyStreamMode();
	for (uint32_t t=0; t<n; t++)
	{
		ks[t] = ClockFSM() ^ LFSR_S0;
		ClockLFSRKeyStreamMode();
	}
}

void EEA(uint8_t *key, uint32_t count, uint32_t bearer, uint32_t dir, uint8_t *data, uint32_t length)
{
	uint32_t K[4],IV[4];
	int lastbits, n = ( length + 31 ) >> 5;
	((length & 7) == 0)? lastbits = 0 : lastbits = (8 - (length & 7));
	uint32_t *KS;
	
	/*Initialisation*/
	/* Load the confidentiality key for SNOW 3G initialization as in section
	3.4. */
	for (int i=0; i<4; i++)
		K[3-i] = (key[i<<2] << 24) ^ (key[(i<<2) + 1] << 16) ^ (key[(i<<2) + 2] << 8) ^ (key[(i<<2) + 3]);
	
	/* Prepare the initialization vector (IV) for SNOW 3G initialization as in
	section 3.4. */
	IV[3] = count;
	IV[2] = (bearer << 27) | ((dir & 0x1) << 26);
	IV[1] = IV[3];
	IV[0] = IV[2];
	
	/* Run SNOW 3G algorithm to generate sequence of key stream bits KS*/
	Initialize(K,IV);
	KS = (uint32_t *)malloc(n<<2);
	GenerateKeystream(n, (uint32_t*)KS);
	
	/* Exclusive-OR the input data with keystream to generate the output bit
	stream */
	for (int i=0; i<n; i++)
	{
		data[i<<2] ^= (KS[i] >> 24) ;
		data[(i<<2) + 1] ^= (KS[i] >> 16) & 0xff;
		data[(i<<2) + 2] ^= (KS[i] >> 8) & 0xff;
		data[(i<<2) + 3] ^= KS[i] & 0xff;
	}
	
	free(KS);
	
	/* zero last bits of data in case its length is not byte-aligned 
	   this is an addition to the C reference code, which did not handle it */
	if (lastbits)
		data[length>>3] &= (0xff<<lastbits) & 0xff; //256 - (1<<lastbits) = (0xff<<lastbits) & 0xff; 
}

int main(int argc, char const *argv[])
{
	printf("%0X\n", MULx(255,153));
	printf("%0X\n", MULxPOW(255,5,0));
	printf("%0X\n", MULa(1));
	printf("%0X\n", DIVa(1));
	printf("%0X\n", S1(0));
	printf("%0X\n", S2(0));
	printf("%0X\n", clockFSM());
	return 0;
}