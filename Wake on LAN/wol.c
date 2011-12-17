//
//  wol.c
//  Wake on LAN
//
//  Created by Markus Kwaśnicki on 17.12.11.
//  Copyright (c) 2011 Kwasniccy. All rights reserved.
//

/* From: http://www.osxentwicklerforum.de/index.php?page=Thread&postID=103923 */

#include "wol.h"

int Wake_on_LAN(char *ip_broadcast,char *wake_mac)
{
	int i,sockfd,an=1;
	char *x;
	char mac[102];
	char macpart[2];
	char test[103];
    
    //	ip_broadcast = "192.168.1.255";
    //  wake_mac = "00:1d:7d:a6:44:da";
    printf("%s\n%s\n", ip_broadcast, wake_mac);
	
	struct sockaddr_in serverAddress;
	
	if ( (sockfd = socket( AF_INET, SOCK_DGRAM,17)) < 0 ) {
		perror( "socket" );
		return 1;
	}
	
	setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&an,sizeof(an));
	
	bzero( &serverAddress, sizeof(serverAddress) );
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( 9 );
	
	inet_pton( AF_INET, ip_broadcast, &serverAddress.sin_addr );
	
	for (i=0;i<6;i++) mac[i]=255;
	for (i=1;i<17;i++) {
		macpart[0]=wake_mac[0];
		macpart[1]=wake_mac[1];
		mac[6*i]=strtol(macpart,&x,16);
		macpart[0]=wake_mac[3];
		macpart[1]=wake_mac[4];
		mac[6*i+1]=strtol(macpart,&x,16);
		macpart[0]=wake_mac[6];
		macpart[1]=wake_mac[7];
		mac[6*i+2]=strtol(macpart,&x,16);
		macpart[0]=wake_mac[9];
		macpart[1]=wake_mac[10];
		mac[6*i+3]=strtol(macpart,&x,16);
		macpart[0]=wake_mac[12];
		macpart[1]=wake_mac[13];
		mac[6*i+4]=strtol(macpart,&x,16);
		macpart[0]=wake_mac[15];
		macpart[1]=wake_mac[16];
		mac[6*i+5]=strtol(macpart,&x,16);
	}
	for (i=0;i<103;i++) test[i]=mac[i];
	test[102]=0;
	
	sendto(sockfd,&mac,102,0,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
	close(sockfd);
	
	return 0;
}
