#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>

#define MAXADDRLEN	256
#define BUFLEN		128



#define MAXSLEEP 128

int
connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
	int nsec;

	/*
	 * Try to connect with exponential backoff.
	 */
	for (nsec = 1; nsec <= MAXSLEEP; nsec <<= 1) {
		if (connect(sockfd, addr, alen) == 0) {
			/*
			 * Connection accepted.
			 */
			return(0);
		}

		/*
		 * Delay before trying again.
		 */
		if (nsec <= MAXSLEEP/2)
			sleep(nsec);
	}
	return(-1);
}

#define REST 18
void
print_uptime(int sockfd)
{
	int		n;
	char	buf[BUFLEN];
	char    head[3];
	memset(buf,0,BUFLEN);
	n = recv(sockfd, head, sizeof(head), 0);
	printf("n = %d\n",n);
	if(n != sizeof(head))
		{
			printf("error !!\n");
			return ;
		}
	printf("head[0] = %c\n",head[0]);
	printf("head[1] = %c\n",head[1]);
	printf("head[2] = %c\n",head[2]);
	if(head[2] == '3')
		{
			n = recv(sockfd, buf, REST, 0);
			printf("n = %d\n",n);
			if(n != REST)
				{
					printf("error !!\n");
					return ;
				}
			printf("buf = %s\n",buf);
			
		}
		else
		{
			printf("error !!\n");
			return ;
		}
		send(sockfd,buf,strlen(buf),0);
		n = recv(sockfd, buf, REST, 0);
			printf("n = %d\n",n);
			printf("buf = %s\n",buf);
	#if 0
	while ((n = recv(sockfd, buf, 3, 0)) > 0)
	{
		printf("yeyeyyeyeyeh~~~~~~~~~ n %d \n",n);
		write(STDOUT_FILENO, buf, n);
		printf(" %c%c%c%c%c%c%c%c \n",buf[n-8],buf[n-7],buf[n-6],buf[n-5],buf[n-4],buf[n-3],buf[n-2],buf[n-1]);
		printf("%d \n",buf[n-1]);
		printf("%c \n",buf[n]);
		printf("%x \n",buf[n]);
	}
	#endif	
	if (n < 0)
		err_sys("recv error");
}

int
main(int argc, char *argv[])
{
	struct addrinfo	*ailist, *aip;
	struct addrinfo	hint;
	int				sockfd, err;

	struct sockaddr_in sin;
	struct sockaddr   *ai_addr;
	char        *addr;
	char  abuf[INET_ADDRSTRLEN] = "192.168.1.19";
	char  bbuf[INET_ADDRSTRLEN];

	if (argc != 2)
		err_quit("usage: ruptime hostname");
	hint.ai_flags = 0;
	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	printf("aaa11a\n");
	if(inet_pton(AF_INET,abuf,&sin.sin_addr) == 0)
	{
		printf("error\n");
		return;
	}
	printf("sin.sin_addr.s_addr = %d \n",sin.sin_addr.s_addr);
	if(( inet_ntop(AF_INET,(void*)&sin.sin_addr,bbuf,INET_ADDRSTRLEN)) == NULL)
	{
		printf("error2\n");
		return;
	}
	printf("address = %s \n",bbuf?bbuf:"unkown");
	
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(39003);
	int i;
	i = 123;
	printf("sin.sin_family = %d sin.sin_port =%d \n",sin.sin_family,sin.sin_port);
	ai_addr = (struct sockaddr *)&sin;
	#if 1
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			err = errno;
		if (connect_retry(sockfd, ai_addr, INET_ADDRSTRLEN) < 0) {
			err = errno;
		} else {
			
			print_uptime(sockfd);
			exit(0);
		}
	#endif
	#if 0
	if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
		err_quit("getaddrinfo error: %s", gai_strerror(err));
	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			err = errno;
		if (connect_retry(sockfd, (struct sockaddr *)&sin, INET_ADDRSTRLEN) < 0) {
			err = errno;
		} else {
			print_uptime(sockfd);
			exit(0);
		}
	}
	#endif
	fprintf(stderr, "can't connect to %s: %s\n", argv[1],
	  strerror(err));
	exit(1);
}
