/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 11:11:02 by maxence           #+#    #+#             */
/*   Updated: 2021/01/21 14:55:23 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

// Calculating the Check Sum 
static unsigned short checksum(void *b, int len) 
{
    unsigned short  *buf = b;
    unsigned int    sum = 0;
    unsigned short  result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

static void fill_ping_packet(struct ping_pkt *pckt, int seq)
{
	int i;

	//filling packet 
	bzero(pckt, sizeof(*pckt)); 
		
	pckt->hdr.type = ICMP_ECHO; 
	pckt->hdr.un.echo.id = getpid(); 
		
	for ( i = 0; i < sizeof(pckt->msg)-1; i++) 
		pckt->msg[i] = i +'0'; 
		
	pckt->msg[i] = '\0'; 
	pckt->hdr.un.echo.sequence = seq;
	pckt->hdr.checksum = checksum(pckt, sizeof(*pckt));
}


struct timeval *send_packet(int sockfd, struct sockaddr* clientaddr, const char *hostname, int seq)
{
    int             databyte;
    struct ping_pkt pckt;
	char            clientname[15];
	char            *ptr;
    struct timeval	*sendtime;

    ptr = ((void*)(&clientaddr->sa_data)) + 2;


    // prepare the packet
    fill_ping_packet(&pckt, seq);
    if (!(sendtime = malloc(sizeof(*sendtime))))
        return (NULL);

    // send the packet
	if ((databyte = sendto(sockfd, &pckt, sizeof(pckt), 0, clientaddr, sizeof(*clientaddr))) <= 0)
		return (NULL);

    // get the time of the day
    gettimeofday(sendtime, NULL);

    // print the first message
    if (seq == 1)
    {
	    inet_ntop (clientaddr->sa_family, ptr, clientname, 15);
	    dprintf(1, "PING %s (%s): %d data bytes\n", hostname, clientname, databyte);
    }

    return (sendtime);
}