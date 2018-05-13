#include<netdb.h>
#include<errno.h>
#include<syslog.h>
#include<sys/socket.h>
#include"apue.h"
#include"fcntl.h"

#define BUFLEN 128 
#define QLEN 10
#define REST 18

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen);
void set_cloexec(int);
int
initserver(int type, const struct sockaddr *addr, socklen_t alen,
  int qlen)
{
	int fd, err;
	int reuse = 1;

	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return(-1);
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse,
	  sizeof(int)) < 0) {
		err = errno;
		goto errout;
	}
	if (bind(fd, addr, alen) < 0) {
		err = errno;
		goto errout;
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
		if (listen(fd, qlen) < 0) {
			err = errno;
			goto errout;
		}
	}
	return(fd);

errout:
	close(fd);
	errno = err;
	return(-1);
}
#if 0
int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
	printf("aaa\n");
	int fd;
	int err = 0;

	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return (-1);
	if (bind(fd, addr, alen) < 0);
		printf("a111\n");
		goto errout;
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
	{
		if(listen(fd, qlen) < 0)
			printf("a222\n");
		goto errout;
	}
	return (fd);
	printf("bbb\n");
errout:
	printf("cc\n");
	err = errno;
	close(fd);
	errno = err;
	return (-1);

}
#endif
void serve(int sockfd)
{
	int clfd;
	FILE *fp;
	char buf[BUFLEN] = "123YIZHENKAIZUISHUAI!";
	char buf2[BUFLEN] = "hahaha";
	char bufRecv[BUFLEN] = "";
	int n = 0;
	printf("eeee\n");
	set_cloexec(sockfd);
	for(;;)
	{
		if((clfd = accept(sockfd,NULL,NULL)) < 0)
		{
			syslog(LOG_ERR,"ruptimed : accept error: %s",strerror(errno));
			exit(1);
		}
		set_cloexec(clfd);
		#if 0
		if((fp = popen("/usr/bin/uptime","r") == NULL))
		{
			sprintf(buf,"error:%s\n",strerror(errno));
			send(clfd,buf,strlen(buf),0);
		}
		else
		{
			while (fgets(buf,BUFLEN,fp) != NULL)
			send(clfd,buf,strlen(buf),0);
			pclose(fp);
			
		}
		#endif
		send(clfd,buf,strlen(buf),0);
		n = recv(clfd, bufRecv, REST, 0);
		printf("n = %d\n",n);
		if(n != REST)
		{
			printf("error !!\n");
			return ;
		}
		printf("bufRecv = %s\n",bufRecv);
		send(clfd,buf2,strlen(buf2),0);
		close(clfd);
		printf("ffff\n");
	}
	
}
void set_cloexec(int sockfd)
{
	int val;
	val = fcntl(sockfd,F_GETFD);
	val |= FD_CLOEXEC;
	fcntl(sockfd,F_SETFD,val);
}
int main(int argc, char* argv[])
{
	int sockfd, err, n;
	char *host;
	struct sockaddr_in sin;
	struct sockaddr *ai_addr;
	char addrBuf[INET_ADDRSTRLEN] = "192.168.1.19";

	if(inet_pton(AF_INET,addrBuf,&sin.sin_addr) == 0)
	{
		printf("error\n");
		return;
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(39003);
	ai_addr = (struct sockaddr *)&sin;
	if((sockfd = initserver(SOCK_STREAM,ai_addr,INET_ADDRSTRLEN,QLEN)) >= 0)
	{
		printf("ddd\n");
		serve(sockfd);
		exit(0);
	}
	exit(1);
}
