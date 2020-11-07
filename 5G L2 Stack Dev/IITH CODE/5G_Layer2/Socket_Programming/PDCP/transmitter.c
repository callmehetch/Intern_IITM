/* Author: Jaswanthi Mandalapu
   Date  : 29-04-2019
   File  : PDCP transmitter (over UDP socket)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>

#define BUF_SIZE (ETH_FRAME_LEN)

//fixing the broadcast address
static char broadcast_addr[ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };


