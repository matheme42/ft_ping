/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxence <maxence@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 14:16:14 by maxence           #+#    #+#             */
/*   Updated: 2021/01/21 15:07:52 by maxence          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int raw_socket() {

    static int sockfd = 0;
    int ttl = 65;

    if (sockfd == 0)
    {
        // create a raw socket to send a ICMP message
        if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
            return (E_SOCKET);

        setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    }
    return (sockfd);
}