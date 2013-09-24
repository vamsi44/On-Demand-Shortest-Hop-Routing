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
#include<string.h>
#include<sys/un.h>
#include<netdb.h>
//#include"unp.h"
#include"hw_addrs.h"

	struct params{
	int sockfd;
	char ip[16];
	int port_number;
	char mesg[400];
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
	char buf1[400],buf2[400];
	char buf[400];//send message server
	int f=0;//flag for rediscovery



	char *message1;
	char *addr1;
	int *port1;

	struct hostent *he,*he1,*he2;
	struct in_addr a;
	char servsunpath[200];

	char buff1[400],buff2[400],buff3[400];

	int n;//for recv from


	fd_set rset;
	struct timeval tv;
	int maxfd=0;
	int retval;
	int count=0;
	char vmname[20];
	int filedesc;
	//struct hwa_info	*hwa, *hwahead;
	//struct sockaddr	*sa,*sa1;
	char   *ptr;
	int    i, prflag;

	//tmp=(char *)malloc(sizeof(char));
	//buf=(char *)malloc(sizeof(char));
	//servsunpath=(char *)malloc(sizeof(char));

	printf("\n");

	gethostname(vmname,sizeof(vmname));
	sockfd=socket(AF_LOCAL,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		printf("socket creation error \n");
		exit(-1);
	}

	bzero(&cliaddr,sizeof(cliaddr));
	//unlink(cliaddr.sun_path);
	cliaddr.sun_family=AF_LOCAL;
	//bzero(buf,sizeof(buf));
	bzero(servsunpath,sizeof(servsunpath));

	//strcpy(tmp,"/tmp/tempfileXXXXXX");
	//printf("debug \n");
	//int =mkstemp(tmp);
	strcpy(cliaddr.sun_path,"/tmp/tempfileXXXXXX");
	filedesc=mkstemp(cliaddr.sun_path);
	if(filedesc==-1)
	printf("File not created by mkstemp \n");
	unlink(cliaddr.sun_path);

	if((bind(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr)))<0)
	{
		perror("Binding client: \n");
		exit(-1);
	}


	printf("Domain Socket Creation and Bind completed \n");
	printf("Starting connection : \n");

	while(1)
	{
	printf("Choose a VM to connect to (vm1,vm2 ....vm10) \n");
	gets(vm);
	//scanf("%s",vm);
    he = gethostbyname(vm);
    if (he)
    {
        printf("name: %s\n", he->h_name);
        /*while (*he->h_aliases)
            printf("alias: %s\n", *he->h_aliases++);*/
        while (*he->h_addr_list)
        {
            bcopy(*he->h_addr_list++, (char *) &a, sizeof(a));
            printf("address: %s\n",(char *)inet_ntoa(a));
        }
    }

    else
    {
    	printf("there are no hosts with that name \n");
    	printf("give the hostname correctly \n");
    	continue;
    }//remember to exit



	/*bzero(&servaddr,sizeof(servaddr));
	servaddr.sun_family=AF_LOCAL;
	//printf("debug 1\n");
	strcpy(servsunpath,"/tmp/serv");
	//printf("debug 1\n");
	strncpy(servaddr.sun_path,servsunpath,sizeof((servaddr.sun_path)-1));
	*///servaddr.sun_family=AF_LOCAL;

    bzero(buf,sizeof(buf));
    strcpy(buf,"Requesting time");
    printf("buf is %s ",buf);
    printf("The client at node %s sending a request to the server at %s \n",vmname,vm);//complete this statement

    strcpy(buff1,(char *)inet_ntoa(a));

    msg_send(sockfd,buff1,5413,buf,f);

    	message1=(char *)malloc(400);
    	addr1=(char *)malloc(400);
    	port1=(int *)malloc(sizeof(int));

	FD_SET(sockfd, &rset);
	FD_ZERO(&rset);
again:
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	maxfd=sockfd+1;

	retval = select(maxfd, &rset, NULL, NULL, &tv);


   if (retval == -1)
       perror("select()");
   else
   {
	   if(FD_ISSET(sockfd,&rset))
   	   {
		   bzero(buf1,sizeof(buf1));
		   msg_recv(sockfd,message1,addr1,port1);
		   he1=gethostbyaddr(addr1,strlen(addr1),AF_INET);
		   if(he1!=NULL)
		   {
		   strcpy(buf1,he1->h_name);
		   printf("Client at node %s : received from %s \n",vmname,buf1); //complete this statement
	   	   printf("timestamp: %s\n",message1);
		   }
		   else
			   printf("gethostbyaddr returned null \n");
   	   }

	   if(retval==0)
	   {
		   count++;
		   if(count<=1)
		   {
		   printf("Client at node %s : timeout on response from %s \n",vmname,vm);	//complete this statement
		   printf("Retransmitting the message as there is a time out \n");
		   f=1;
		   strcpy(buff2,(char *)inet_ntoa(a));
		   msg_send(sockfd,buff2,5413,buf,f);
		   goto again;
		   }
	   }
   }

 }//for infinite while loop
//	unlink(tmp);
} // main loop


void msg_send(int sfd,char *ipaddr,int portno,char msg[400],int flags)
{

//	printf("this is in the msg_send function \n");
	struct params *p;
	struct sockaddr_un servaddr;

	char buff1[400],buff2[400],buff3[400];

	char servsunpath[200];

	p=(struct params *)malloc(sizeof(struct params));

   // p->mesg=(char *)malloc(strlen(msg)+1);
  //  p->ip=(char *)malloc(strlen(ipaddr)+1);

    p->sockfd=sfd;
	printf("%s is the message \n",msg);

	strcpy(p->mesg,msg);
	strcpy(p->ip,ipaddr);
	//p->mesg=msg;
	//p->ip=ipaddr;

	//memcpy(p->mesg,msg,strlen(msg)+1);
	p->port_number=portno;
	p->flag_rediscover=flags;

	//memcpy(p->ip,ipaddr,strlen(ipaddr)+1);

	printf("the values inserted in p are : %s \n",p->ip);
	printf("the values inserted in p are : %d %d \n",p->port_number,p->flag_rediscover);
	printf("the values inserted in p are : %s \n",p->mesg);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sun_family=AF_LOCAL;
	strcpy(servaddr.sun_path,"/tmp/odr");
	//strncpy(servaddr.sun_path,servsunpath,sizeof((servaddr.sun_path)-1));
	//sendto(p.sockfd,p.mesg,strlen(p.mesg),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
	if(sendto(p->sockfd,p,sizeof(struct params),0,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
		printf("sendto failed in client \n");
	else
	printf("this is after sendto \n");
}



void msg_recv(int sockfd,char *mesg1,char *ipaddr1,int *portno1)
{

	char buff1[400],buff2[400],buff3[400];
	char *recvbuf;
	int n;
	ssize_t len;
	char *recvstream;
	struct params1 *ptr1;

	struct sockaddr_un servaddr;
	char *servsunpath;

	/*

	servaddr.sun_family=AF_LOCAL;
	strcpy(servsunpath,"/tmp/odr");
	strncpy(servaddr.sun_path,servsunpath,sizeof((servaddr.sun_path)-1));

*/

	servsunpath=(char *)malloc(sizeof(char));
	p1=(struct params1 *)malloc(sizeof(struct params1));

	//len=sizeof(servaddr);
	n=recvfrom(sockfd,(void *)p1,sizeof(struct params1),0,NULL,NULL);

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
    	printf("Server Didn't send anything\n");
    }
}





/*for (hwahead = hwa = Get_hw_addrs(); hwa != NULL; hwa = hwa->hwa_next) {

	printf("%s :%s", (char *)hwa->if_name, (char *)((hwa->ip_alias) == IP_ALIAS) ? " (alias)\n" : "\n");

	if ( (sa = hwa->ip_addr) != NULL)
		printf("         IP addr = %s\n",(char *) Sock_ntop_host(sa, sizeof(*sa)));

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
