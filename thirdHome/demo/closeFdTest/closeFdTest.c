#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fd = -1;
	char buf[100];
	while(fgets(buf,100,stdin) != NULL)
	{
		printf("%s\n",buf);
		if(buf[strnlen(buf)-1] == '\n')
	       {
		    buf[strnlen(buf)-1] = '\0';
	     	printf("%s\n",buf);
			if(strcmp(buf,"closefd2") == 0)
			{
				printf("closefd0\n");
				close(2);
				fd = open("/home/demo/fdTest",O_RDWR|O_CREAT);
				printf("openfd = %d\n",fd);
			}
			if(strcmp(buf,"openfd") == 0)
			{
				fd = open("/home/demo/fdTest",O_RDWR);
				printf("openfd = %d\n",fd);
			
			}
			}
		sleep(1);
	}

	return 0;
}
