/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 12:08:48 by maxence           #+#    #+#             */
/*   Updated: 2021/05/29 00:14:41 by maxence          ###   ########lyon.fr   */
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

static void read_packet(void *packet, const int sockfd, struct sockaddr *destaddr) {
 	struct iphdr	*ip = packet;
 	struct icmphdr	*icmp = packet + IP_HDR_SIZE;
    char            *error_str;
 	char			*sender = inet_ntoa((struct in_addr){.s_addr = ip->saddr});
 	u_int16_t		recvd_seq = ntohs(icmp->un.echo.sequence);
 	suseconds_t		rtt;

	opt['D'] ? dprintf(1, "[%ld.%ld] ", get_time() / 1000000, get_time() % 1000000) : 0;
    if (icmp->type != ICMP_ECHOREPLY)
	{
		if (icmp->type == ICMP_ECHO)
		{
			receive_packet(sockfd, destaddr);
			return ;
		}
		g_stat()->packets_error += 1;
		if (opt['q'])
			return ;
		if (icmp->type < sizeof(icmp_responses))
			error_str = icmp_responses[icmp->type];
		else
			error_str = NULL;
		//	type=11 (Time to live exceeded) code=0
		if (opt['v'])
			dprintf(1, "From %s, icmp_seq=%hu type=%d (%s) code=%d", sender, recvd_seq, icmp->type, error_str, icmp->code);
		else
			dprintf(1, "From %s icmp_seq=%hu %s", sender, recvd_seq, error_str);
	}
    else
    {
    	rtt = get_rtt(packet + IP_HDR_SIZE + ICMP_HDR_SIZE + 4);
		update_rtt_stats(rtt, recvd_seq);
		g_stat()->packets_recvd += 1;
		if (opt['q'])
			return ;
    	dprintf(1, "%hu bytes from %s icmp_seq=%hu ttl=%hhu time=%ld.%02ld ms", \
           (uint16_t)(ntohs(ip->tot_len) - IP_HDR_SIZE), \
           sender, recvd_seq, ip->ttl, rtt / 1000l, rtt % 1000l);
    }
	opt['a'] ? system("echo \"\a\"") : dprintf(1, "\n");
}

static void prepare_msghdr(struct msghdr *msg, struct iovec *iov, char (*content)[84])
{
	ft_bzero(msg, sizeof(*msg));
	ft_bzero(iov, sizeof(*iov));
	ft_bzero(content, PACKET_SIZE);
	iov->iov_base = content;
	iov->iov_len = PACKET_SIZE;
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
}

void receive_packet(const int sockfd, struct sockaddr *destaddr)
{
	struct msghdr		msg;
	struct iovec		io;
	char				packet[PACKET_SIZE];

	prepare_msghdr(&msg, &io, &packet);
    if (recvmsg(sockfd, &msg, 0) < 0) {

		return ;
		// an error occurd. Example wrong checksum
    }
	read_packet(&packet, sockfd, destaddr);
}