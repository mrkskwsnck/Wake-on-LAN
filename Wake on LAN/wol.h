//
//  wol.h
//  Wake on LAN
//
//  Created by Markus Kwaśnicki on 17.12.11.
//  Copyright (c) 2011 Kwasniccy. All rights reserved.
//

#ifndef Wake_on_LAN_wol_h
#define Wake_on_LAN_wol_h

/* From: http://www.osxentwicklerforum.de/index.php?page=Thread&postID=103923 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int Wake_on_LAN(char *ip_broadcast,char *wake_mac);

#endif
