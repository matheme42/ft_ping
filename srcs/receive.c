/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 12:08:48 by maxence           #+#    #+#             */
/*   Updated: 2021/05/28 14:31:12 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"


static char	*icmp_responses[] =
{
	[ICMP_DEST_UNREACH]		= "Destination Unreachable",
	[ICMP_SOURCE_QUENCH]	= "Source Quench",
	[ICMP_REDIRECT]			= "Redirect (change route)",
	[ICMP_ECHO]				= "Echo Request",
	[ICMP_TIME_EXCEEDED]	= "Time to live exceeded",
	[ICMP_PARAMETERPROB]	= "Parameter Problem",
	[ICMP_TIMESTAMP]		= "Timestamp Request",
	[ICMP_TIMESTAMPREPLY]	= "Timestamp Reply",
	[ICMP_INFO_REQUEST]		= "Information Request",
	[ICMP_INFO_REPLY]		= "Information Reply",
	[ICMP_ADDRESS]			= "Address Mask Request",
	[ICMP_ADDRESSREPLY]		= "Address Mask Reply"
};

suseconds_t	get_time(void)
{
	struct timeval	curr_time;

	if (gettimeofday(&curr_time, NULL) == -1)
	{
		dprintf(1, "failed getting time of day\n");
		return (0);
	}
	return (curr_time.tv_sec * 1000000 + curr_time.tv_usec);
}

//   - calculates Round Trip Time in ms

static suseconds_t	get_rtt(struct timeval *send_time)
{
	return (get_time() - send_time->tv_sec * 1000000 - send_time->tv_usec);
}

void receive_packet(const int sockfd, struct sockaddr *destaddr);

static void	update_rtt_stats(suseconds_t rtt, uint16_t seq)
{
	// if first packet init all
	if (seq == 1)
	{
		g_stat()->rtt_min = rtt;
		g_stat()->rtt_max = rtt;
		g_stat()->rtt_total = rtt;
		g_stat()->rtt_sq_total = rtt * rtt;
	}
	else
	{
		g_stat()->rtt_min = rtt < g_stat()->rtt_min ? rtt : g_stat()->rtt_min;
		g_stat()->rtt_max = rtt > g_stat()->rtt_max ? rtt : g_stat()->rtt_max;
		g_stat()->rtt_total += rtt;
		g_stat()->rtt_sq_total += rtt * rtt;
	}
}

void read_packet(void *packet, const int sockfd, struct sockaddr *destaddr) {
 	struct iphdr	*ip = packet;
 	struct icmphdr	*icmp = packet + IP_HDR_SIZE;
    char            *error_str;
 	char			*sender = inet_ntoa((struct in_addr){.s_addr = ip->saddr});
 	u_int16_t		recvd_seq = ntohs(icmp->un.echo.sequence);
 	suseconds_t		rtt;

    if (icmp->type != ICMP_ECHOREPLY)
	{
		if (icmp->type == ICMP_ECHO)
		{
			receive_packet(sockfd, destaddr);
			return ;
		}
		if (icmp->type < sizeof(icmp_responses))
			error_str = icmp_responses[icmp->type];
		else
			error_str = NULL;
		dprintf(1, "From %s icmp_seq=%hu %s\n", sender, recvd_seq, error_str);
		g_stat()->packets_error += 1;
	}
    else
    {
    	rtt = get_rtt(packet + IP_HDR_SIZE + ICMP_HDR_SIZE + 4);
    	dprintf(1, "%hu bytes from %s icmp_seq=%hu ttl=%hhu time=%ld.%02ld ms\n", \
           (uint16_t)(ntohs(ip->tot_len) - IP_HDR_SIZE), \
           sender, recvd_seq, ip->ttl, rtt / 1000l, rtt % 1000l);
		update_rtt_stats(rtt, recvd_seq);
		g_stat()->packets_recvd += 1;
    }
}

void receive_packet(const int sockfd, struct sockaddr *destaddr)
{

    char            packet[PACKET_SIZE];
    ssize_t         recvbytes;
	char			buffer[512];

	ft_bzero(packet, PACKET_SIZE);
	ft_bzero(buffer, 512);

	struct iovec	io =
	{
		.iov_base = packet,
		.iov_len = PACKET_SIZE
	};
	struct msghdr	msg =
	{
		.msg_name = destaddr,
		.msg_namelen = sizeof(destaddr),
		.msg_iov = &io,
		.msg_iovlen = 1,
		.msg_control = buffer,
		.msg_controllen = sizeof(buffer),
		.msg_flags = 0
	};

    recvbytes = recvmsg(sockfd, &msg, 0);
    if (recvbytes < -1) {
		return ;
		// an error occurd. Example wrong checksum
    }
	read_packet(packet, sockfd, destaddr);
}