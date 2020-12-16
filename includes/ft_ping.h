/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matheme <matheme@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 13:07:26 by matheme           #+#    #+#             */
/*   Updated: 2020/12/11 14:11:45 by matheme          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# ifndef FT_PING_H
# define FT_PING_H

// stdio.h allow the use of printf
# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>

// netdb.h allow the use of getaddrinfo
#include <netdb.h>

#include <errno.h>

# define SUCCESS 0
# define FAILURE 1

# endif