/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:16:14 by maxence           #+#    #+#             */
/*   Updated: 2021/01/22 17:37:58 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int raw_socket() {

    static int sockfd = 0;

    if (sockfd == 0)
    {
        // create a raw socket to send a ICMP message
        if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        {
            sockfd = E_SOCKET;
            return (E_SOCKET);
        }

        if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, (int[1]){1}, sizeof(int)) == -1)
        {
            sockfd = E_SOCKET;
		    return (E_SOCKET);
        }
    }
    return (sockfd);
}