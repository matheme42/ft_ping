/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 11:11:02 by maxence           #+#    #+#             */
/*   Updated: 2021/04/21 12:11:20 by maxence          ###   ########lyon.fr   */
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

void sendfirst(int sendbyte, struct sockaddr *dest, const char *domainname)
{
    char charip[15];
    char *ptr = (char*)&(((struct sockaddr_in *)dest)->sin_addr);
    inet_ntop(dest->sa_family, ptr, charip, sizeof(charip));
    dprintf(1, "PING %s (%s) %d(%d) bytes of data.\n", domainname , charip, ICMP_PAYLOAD_SIZE, PACKET_SIZE);
}


int send_packet(const int sockfd, struct sockaddr *dest, int seq, const char *domainname)
{
	char    packet[PACKET_SIZE];

    struct iphdr *ipheader = (struct iphdr*)packet;
    struct icmphdr *icmpheader = (struct icmphdr*)&packet[IP_HDR_SIZE];


    // IP HEADER
    ipheader->version = 4;
    ipheader->ihl = IP_HDR_SIZE / 4;
    ipheader->tos = 0;
    ipheader->tot_len = htons(PACKET_SIZE); // convert little endian to big endian
    ipheader->ttl = 64;
    ipheader->frag_off = htons(0);
    ipheader->protocol = IPPROTO_ICMP; // 1 for ICMP
    ipheader->saddr = INADDR_ANY;
    ipheader->daddr = (unsigned int)((struct sockaddr_in *)dest)->sin_addr.s_addr;
    ipheader->check = 0;

    // ICMP DATA
    memset(icmpheader, 42, PACKET_SIZE - IP_HDR_SIZE); // fill with random data
    gettimeofday((void*)&packet[IP_HDR_SIZE + ICMP_HDR_SIZE + 4], NULL);


    // ICMP HEADER
    icmpheader->type = ICMP_ECHO;
    icmpheader->code = 0;
    icmpheader->un.echo.id = getpid();
    icmpheader->un.echo.sequence = htons(seq);
    icmpheader->checksum = 0;
    icmpheader->checksum = checksum(icmpheader, ICMP_PAYLOAD_SIZE + ICMP_HDR_SIZE);

    int sendbyte = sendto(sockfd, packet, PACKET_SIZE, 0, dest, sizeof(*dest));
    g_stat()->packets_send += 1;
    if (sendbyte < 0) {
        perror("ft_ping: sendto");
        return (1);
    }
    if (seq == 1) {
        sendfirst(sendbyte, dest, domainname);
    }

    return (0);
}