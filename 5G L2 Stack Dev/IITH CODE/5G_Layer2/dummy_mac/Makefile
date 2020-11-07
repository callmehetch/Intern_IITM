LDIR=./lib
TXDIR=./tx
RXDIR=./rx

CC=gcc
CFLAGS=-I$(LDIR) -lpthread -lrt

all: txapp txmac rxmac rxapp

txapp: $(TXDIR)/txApp.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c 
	$(CC) -o $(TXDIR)/txapp $(TXDIR)/txApp.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c $(CFLAGS)

txmac: $(TXDIR)/txMac.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c
	$(CC) -o $(TXDIR)/txmac $(TXDIR)/txMac.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c $(CFLAGS)

rxmac: $(RXDIR)/rxMac.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c
	$(CC) -o $(RXDIR)/rxmac $(RXDIR)/rxMac.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c $(CFLAGS)

rxapp: $(RXDIR)/rxApp.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c
	$(CC) -o $(RXDIR)/rxapp $(RXDIR)/rxApp.c $(LDIR)/ringBuffer.c $(LDIR)/macHdrs.c $(CFLAGS)





