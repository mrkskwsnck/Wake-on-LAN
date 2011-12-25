//
//  wol.c
//  Wake on LAN
//
//  Created by Markus Kwaśnicki on 17.12.11.
//  Copyright (c) 2011 Kwasniccy. All rights reserved.
//

/* 
 * How to found from: 
 * • http://www.jibble.org/wake-on-lan/
 * • http://www.osxentwicklerforum.de/index.php?page=Thread&postID=103923 
 * 
 * Additional notes:
 * • The „Magic Packet“ needs to be sent as UDP packet to the networks broadcast address on port 7 or 9.
 * • Errors should be evaluated via „errno“!
 */

#include "wol.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int wakeOnLan(const char *ipBroadcast, const char *mac2wake)
{
    /* Create magic packet */
	unsigned char magicPacket[102];
	unsigned char macAddress[6];
    
    /* The first 6 bytes of the magic packet are filled with 0xFF. This is probably the source broadcast MAC address? */
    for (int i = 0; i < 6; i++) {
        magicPacket[i] = 0xFF;
    }
    
    /* The next 6 bytes are the MAC address of the target computer. */
    char *iterator = (char *)mac2wake;  // Reading from const char only, storing iterators position
    for (int i = 0; i < 6; i++) {
        long macPart = strtol(iterator, &iterator, 16);
        iterator++;
        macAddress[i] = macPart;
    }
    
    /* Each subsequent set of 6 bytes is also filled with the MAC address of the target computer, until the packet is full at 102 octets. */
    for (int i = 6; i < 102; i++) {
        for (int j = 0; j < 6; j++) {
            magicPacket[i++] = macAddress[j];
        }
        i--;
    }
    // --

	/* Sending UDP magic packet */
	int socketDescriptor;
	if ((socketDescriptor = socket(AF_INET, SOCK_DGRAM, 17)) < 0) {
		perror("socket");
		return 1;
	}
	int socketOptionValue = 1;
	setsockopt(socketDescriptor, SOL_SOCKET, SO_BROADCAST, &socketOptionValue, sizeof(socketOptionValue));
	
	struct sockaddr_in socketAddress;
	bzero(&socketAddress, sizeof(socketAddress));
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(9);
	
	inet_pton(AF_INET, ipBroadcast, &socketAddress.sin_addr);
	
	sendto(socketDescriptor, &magicPacket, 102, 0, (struct sockaddr *)&socketAddress, sizeof(socketAddress));
	close(socketDescriptor);
    // --
	
	return 0;
}

