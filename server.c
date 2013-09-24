#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<sys/types.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/select.h>
//#include "unpifiplus.h"
#include<pthread.h>
#include<math.h>
#include <string.h>
#include <sys/un.h>
#include<netdb.h>
//#include"unp.h"
#include"hw_addrs.h"

	struct params{
	int sockfd;
	char *ip;
	int port_number;
	char *mesg;
	int flag_rediscover;
	};

	struct params1
		{
		int sfd1;
		char *mesg1;
		char *ipaddr1;
		int *portno1; // remember to check if it pointer or normal
		}*p1;


void msg_send(int,char *,int,char *,int);
void msg_recv(int,char *,char *,int *);

int main(int argc, char **argv)
{

	int sockfd;
	struct sockaddr_un cliaddr,servaddr;
	char tmp[100];
	char vm[100];

	char buf[100],buf1[100];//send message server
	int f=0;//flag for rediscovery

	struct hostent *he,*he1;
	struct in_addr a;
	char *servsunpath;

	time_t ticks;

	char buff1[400],buff2[400],buff3[400];

	int n;//for recv from

	fd_set rset;
	struct timeval tv;
	int maxfd=0;
	int retval;
	int count=0;

	char *message1,*addr1;
	int *portno1;

	struct hwa_info	*hwa, *hwahead;
	struct sockaddr	*sa,*sa1;
	char   *ptr;
	char vmname[20];
	int    i, prflag;

	printf("\n");

	gethostname(vmname,sizeof(vmname));

	sockfd=socket(AF_LOCAL,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		printf("socket creation error \n");
		exit(-1);
	}

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sun_family=AF_LOCAL;
	strcpy(tmp,"/tmp/serv");
	strcpy(servaddr.sun_path,tmp);
	unlink(tmp);
	//if((bind(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr)))<0)
	if((bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)))<0)
	{
		perror("Binding server: \n");
		exit(-1);
	}

	message1=(char *)malloc(400);
	addr1=(char *)malloc(400);
	portno1=(int *)malloc(sizeof(int));


	printf("Domain Socket Creation and Bind completed \n");
//	printf("Starting connection : \n");

	while(1)
	{
		bzero(buf,sizeof(buf));
		bzero(buff3,sizeof(buff3));
		bzero(buf1,sizeof(buf1));
		msg_recv(sockfd,message1,addr1,portno1);
		printf("Server received a message from the client: %s \n",message1);

		   he1=gethostbyaddr(addr1,strlen(addr1),AF_INET);
		   if(he1!=NULL)
		   {
			strcpy(buf1,he1->h_name);
			printf("Server at node %s responding to request from %s \n", vmname,buf1);//complete the statement
			ticks=time(NULL);
			snprintf(buf,sizeof(buf),"%.24s\r\n",ctime(&ticks));
			printf("value sending %s \n ",buf);
			strcpy(buff3,(char *)inet_ntoa(a));
			msg_send(sockfd,buff3,*portno1,buf,f);
		   }//	printf("this is in the msg_send function \n");

		   else
			   printf("gethostbyaddr returned null \n");
	}


} // main loop


void msg_send(int sfd,char *ipaddr,int portno,char *msg,int flags)
{

	struct params *p;
	struct sockaddr_un servaddr;

	char buff1[400],buff2[400],buff3[400];

	char servsunpath[200];
    p=(struct params *)malloc(sizeof(struct params));

	p->sockfd=sfd;
	memcpy(p->ip,ipaddr,strlen(ipaddr)+1);
	p->port_number=portno;
	p->flag_rediscover=flags;
	memcpy(p->mesg,msg,strlen(msg)+1);

	//char *sendstream;
	//sendstream=(char *)p;

	bzero(servsunpath,sizeof(servsunpath));
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sun_family=AF_LOCAL;
	strcpy(servsunpath,"/tmp/odr");
	strncpy(servaddr.sun_path,servsunpath,sizeof((servaddr.sun_path)-1));

	//sendto(p.sockfd,p.mesg,strlen(p.mesg),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
	sendto(p->sockfd,p,sizeof(struct params),0,(struct sockaddr *)&servaddr,sizeof(servaddr));

}



void msg_recv(int sockfd,char *mesg1,char *ipaddr1,int *portno1)
{

	char buff1[400],buff2[400],buff3[400];
	char *recvbuf;
	int n;
	char *recvstream;
	struct params1 *ptr1;

	struct sockaddr_un servaddr;
	char servsunpath[200];

	p1=(struct params1 *)malloc(sizeof(struct params1));
	n=recvfrom(sockfd,p1,sizeof(struct params1),0,NULL,NULL);

	if(n<0)
    	printf("RecvFrom error in client \n");
	else if(n>0)
    {
		memcpy(mesg1,p1->mesg1,strlen(p1->mesg1)+1);
		memcpy(ipaddr1,p1->ipaddr1,strlen(p1->ipaddr1)+1);
		portno1=p1->portno1;
		//memcpy(portno1,p1->portno1,sizeof(p1.portno1));
    	//sscanf(buff1,"%d %s %d %s",p1.sfd1,p1.mesg1,p1.portno1,p1.ipaddr1);
    }
    else
    {
    	printf("Server Didn't send anything \n");
    }
}

/*
	for (hwahead = hwa = Get_hw_addrs(); hwa != NULL; hwa = hwa->hwa_next) {

		printf("%s :%s", hwa->if_name, ((hwa->ip_alias) == IP_ALIAS) ? " (alias)\n" : "\n");

		if ( (sa = hwa->ip_addr) != NULL)
			printf("         IP addr = %s\n", Sock_ntop_host(sa, sizeof(*sa)));

		if(hwa->ip_addr!=NULL && strcmp("eth0",hwa->if_name)==0)
		{
			sa1=hwa->ip_addr;
		}

		prflag = 0;
		i = 0;
		do {
			if (hwa->if_haddr[i] != '\0') {
				prflag = 1;
				break;
			}
		} while (++i < IF_HADDR);

		if (prflag) {
			printf("         HW addr = ");
			ptr = hwa->if_haddr;
			i = IF_HADDR;
			do {
				printf("%.2x%s", *ptr++ & 0xff, (i == 1) ? " " : ":");
			} while (--i > 0);
		}

		printf("\n interface index = %d\n\n", hwa->if_index);
	}
*/
