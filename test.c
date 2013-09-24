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
#include"fields.h"
#include"functions.c"
//#include<stdbool.h>

int main(int argc, char **argv)
{



	//TEST FOR ALL PENDINGRREP FUNCTIONS

/*
	struct pendingrrep *h,*e;
	h=NULL;
	struct in_addr src,src1,src2,dest,dest1,dest2;
	int hp,hp1,hp2;
	int rs,rs1,rs2;
	int fd,fd1,fd2;

	dest.s_addr=11111;
	dest1.s_addr=22222;
	dest2.s_addr=33333;
	src.s_addr=44444;
	src1.s_addr=55555;
	src2.s_addr=66666;
	hp=10;
	hp1=20;
	hp2=30;


	insertrrep(&h,src,dest,hp-8,0,1);
	insertrrep(&h,src1,dest1,hp+1,0,2);
	insertrrep(&h,src2,dest1,hp-1,0,3);

	display_preptable(&h);
	int s=isrreppending(&h,dest2);
	printf("this is the value of pending rep %d\n",s);
	getpendingrrep(&h,dest1,&e);
	printf("the valued of the node is %u \n",e->srcaddr);

	deletependingrrep(&h,dest2);
	printf("***************************\n");
	display_preptable(&h);
*/


//TEST FOR RREQS LIST AND ASSOCIATED FUNCTIONS

/*

	struct rreqslist *h;
	h=NULL;

	struct in_addr src,src1,src2;
	int bid,bid1,bid2;
	int hp,hp1,hp2;


	src.s_addr=11111;
	src1.s_addr=22222;
	src2.s_addr=33333;
	bid=1;
	bid1=2;
	bid2=3;
	hp=10;
	hp1=20;
	hp2=30;
	addproreq(&h,src,bid,hp);
	addproreq(&h,src1,bid1,hp1);
	addproreq(&h,src2,bid2,hp2);

	display_rreqtable(&h);

int isproces=isprocessed(&h,src1,bid);

printf("returened value for isproces %d\n",isproces);

*/







	//Test for pending data included functions

	/*
	struct pendingdata *h,*e;
	h=NULL;
	struct in_addr dest,dest1,dest2,src,src1,src2;
	int destport,destport1,destport2,srcport,srcport1,srcport2;
	int hp,hp1,hp2;
	char m[maxpayload-(2*sizeof(struct in_addr))-16],s[maxpayload-(2*sizeof(struct in_addr))-16],g[maxpayload-(2*sizeof(struct in_addr))-16];
	int bts,bts1,bts2;
	dest.s_addr=11111;
	dest1.s_addr=22222;
	dest2.s_addr=33333;
	src.s_addr=44444;
	src1.s_addr=55555;
	src2.s_addr=66666;
	destport=11;
	destport1=22;
	destport2=33;
	srcport=44;
	srcport1=55;
	srcport2=66;
	hp=1;
	hp1=2;
	hp2=3;
	strcpy(m,"aaaaaaaaaa");
	strcpy(s,"bbbbbbbbbb");
	strcpy(g,"cccccccccc");
	bts=91;
	bts1=92;
	bts2=93;


	insertdata(&h,dest,destport,hp,m,bts,src,srcport);

	insertdata(&h,dest1,destport1,hp1,s,bts1,src1,srcport1);

	insertdata(&h,dest2,destport2,hp2,g,bts2,src2,srcport2);

	display_ptable(&h);

	int ispend=isdatapending(&h,dest2);
	printf("the value of ispending is %d \n",ispend);

	//e=NULL;

	getpendingdata(&h,dest2,&e);
	printf("blah blah \n");
	printf("value %d \n",e->destport);
	deletependingdata(&h,dest2);
	printf("**************************\n");
	display_ptable(&h);
*/


//TEST FOR ROUTINGTABLE ENTRIES CHECKED ALL FUNCTIONS OF ROUTING TABLE HEAD
/*
	struct routingtable *h;
	h=NULL;
	unsigned char s[6],r[6],e[6];
	strcpy(s,"vamsi");
	strcpy(r,"sreek");
	strcpy(e,"athin");
	struct in_addr dest,dest1,dest2,dest3;
	dest.s_addr=11111;
	dest1.s_addr=22222;
	dest2.s_addr=33333;
	dest3.s_addr=44444;
	int hop=10,*h2,*h3;
	int inds=2,*i2,*i3;
	time_t tim=time(NULL);

	addroute(&h,s,dest,hop,inds,tim);
	addroute(&h,r,dest1,5,3,tim+1);
	addroute(&h,e,dest2,6,4,tim+2);
	display_rtable(&h);
	int x=0;
	x=checkroute(&h,dest1);
	printf("checkroute value %d \n",x);
	time_t pq;
	pq=gettime(&h,dest3);
	printf("time returned is %ld \n",pq);
	deleteentry(&h,dest);
	printf("****************************\n");
	display_rtable(&h);
	getroute(&h,dest2,&h2,&i2);
	printf("the values of hopcount %d and index %d are \n",h2,i2);
	updatetimestamp(&h,dest2);
	printf("****************************\n");
	display_rtable(&h);
	int ls=gethopcount(&h,dest);
	printf("gethopcount returned %d \n ",ls);

*/




	//	TEST for contained ports structure functions
/*
	struct ports *h;
	h=NULL;

	char s[104],x[104],o[104];

	strcpy(s,"/temp/odr");
	strcpy(x,"/temp/odr/temp");
	//strcpy(o,"/s/s/s");
	int p=2222;
	int *l;
	int d;
	time_t t=time(NULL);
	int w=1;

	addport(&h,s,p,t-10,1);
	addport(&h,s,p+2,t,1);
	addport(&h,x,p+1,t-9,0);

	display_port(&h);
	getport(&h,x,&l);
	printf("the value of recieved l is %d\n ",l);
	d=checkpath(&h,s);
	printf("the value id d is %d",d);
	//reinitialize(&h,s);
	display_port(&h);
	getpath(&h,2222,o);
	printf("the pathname returned is %s \n",o);
	deletestale(&h,4);
	printf("*************************************");
	display_port(&h);


*/





	//TEST FOR IFIINFO AND GETIFIHW
	/*struct ifiinfo *h;
	struct sockaddr addr;

	strncpy(addr.sa_data,"random",14);
	addr.sa_family=AF_INET;
	char a[6]="secot";
	char s[6],r[6];
	h=NULL;

	addifiinfo(&h,1,&addr,a);
	addifiinfo(&h,4,&addr,a);
	disply(&h);
	getifihw(&h,4,s);
	getifihw(&h,1,r);
	printf("sssss %s %s \n",s,r);
	printf("blah, blah \n");
*/


}

