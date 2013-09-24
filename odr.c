#include "unp.h"

//#include <sys/socket.h>
//#include <sys/un.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
//#include<netdb.h>
#include <stdlib.h>
//#include <time.h>
//#include <stdio.h>
//#include <string.h>
//#include "fields.h"

#include "functions.h"
#include "hw_addrs.h"

//#define ETH_FRAME_LEN 1518
#define USID_PROTO	0x8885
#define timeservice 5413
#define odrservice 7627
#define odr_path "/tmp/odr"
#define time_path "/tmp/time"
#define timetolive 10000
//#define protocol 5486 //last 4 digits of rollno,its hexadecimal is 0X156E

extern struct hwa_info * Get_hw_addrs();
struct odrmsg *odr1, *odr2, *odr3, *odr4;
//struct rreq *rq;
//struct rrep *rp;
//struct payload *pl;
int tempport = 20000;
/*struct rreq *req,*req1;
 struct rrep *rep;
 struct payload *pyl;*/
struct hostent *hptr, *hptr1, *hptr2;
struct hwa_info *hwahead, *hwa;
struct ports *porthead;
struct msg *rcvmsg, *rcvmsg1;
struct routingtable *rhead;
struct pendingdata *pdata, *pdata1;
struct pendingrrep *prep, *prep1;
struct rreqslist *proreq;
struct rrepslist *prorep;

/*struct odrmsg
 {
 int type;
 struct rreq *rq;
 struct rrep *rp;
 struct payload *pl;
 };



 struct rreq
 {
 //int type;
 struct in_addr srcaddr;
 struct in_addr dstaddr;
 int broadcastid;
 int hopcnt;
 int rrepsent;
 int forceddiscovery;

 };


 struct rrep
 {
 struct in_addr srcaddr;
 struct in_addr dstaddr;
 //int broadcastid;
 int hopcnt;
 //int rrepsent;
 int forceddiscovery;
 //	struct rrep *next;
 };

 struct payload
 {
 //int type;
 struct in_addr srcaddr;
 int srcport;
 struct in_addr destaddr;
 int destport;
 int hopcnt;
 int bytes;
 char msg[maxpayload-(2*sizeof(struct in_addr))-16];
 struct payload *next;
 };*/

/*
 typedef struct
 {
 struct in_addr srcaddr;
 int srcport;
 struct in_addr destaddr;
 int destport;
 int hopcnt;
 int bytes;
 char msg[maxpayload-(2*sizeof(struct in_addr))-16];
 struct pendingdata *next;
 }pendingdata;
 */

/*void fillrreq()
 {
 ifi=Get_hw_addrs()

 }*/

int main(int argc, char** argv) {

	int s, t, i, send_result, ispath, j, pono, length, staleness, sock_raw,
			unxsock, usdport, ispro, ispro1 = 0, hopflag = 0, hopflag1,
			hopflag2, rreppending, indx2;
	int route, route1, route2, isport, len, maxfd, nready, stale, stale1,
			stale2, stale3, st, rcvflag, discovery, datapending, bid, bid1,
			bid2, indx, portno, indx1, interface_index, tablehopcnt;
	unsigned long currenttime, tabletime, currenttime1, tabletime1,
			currenttime2, tabletime2, port_time, currenttime3, tabletime3, tt,
			ct, td;
	unsigned long timedif, timedif1, timedif2, timedif3, hton, ntoh1, ntoh2,
			ntoh3, ntoh4;
	socklen_t lenx, lens2;
	struct sockaddr_ll socket_address, socket_address1, socket_address2,
			socket_address3, socket_address4;
	struct sockaddr_in destad;
	struct sockaddr *cannonicaladdr;
	struct sockaddr_un odr, app, app1;
	struct ifiinfo *ifihead, *ifi;
	struct in_addr srcaddrs, myaddr;
	char bufad[100];
	char hostname[255], hostname1[255], hostname2[255], host[32], src[16],
			rcvstr[400], src1[16];
	unsigned char *neighbour_addr;
	unsigned char broadcast_mac[6], srchw[6], srchw1[6], srchw2[6], srchw3[6],
			srchw4[6], nexthp[6], nexthp1[6], nexthp2[6];
	char buff[1000], *ipaddr, *paths;
	fd_set rset;
	/*struct odrmsg *odr;
	 struct rreq *req;
	 struct rrep *rep;
	 struct payload *pyl;*/
	void *buffer, *sendbuf, *buff1, *buff2, *buffer1, *buff3, *buff4, *buff5,
			*mess;
	//struct hwa_info	*hwa, *hwahead;
	unsigned char *etherhead, *etherhead1;
	unsigned char *data;

	struct ethhdr *eh, *eh1;

	char *deststr, *msgstr;
	struct timeval t_val;

	unsigned char src_mac[6];
	unsigned char dest_mac[6];

	eh = (struct ethhdr *) etherhead;
	eh1 = (struct ethhdr *) etherhead1;
	send_result = 0;
	ispath = 0;
	length = 0;
	ispro = 0;
	ispro1 = 0;
	hopflag = 0;
	hopflag1 = 0;
	hopflag2 = 0;
	rreppending = 0;
	discovery = 0;
	datapending = 0;
	stale = 0;
	stale1 = 0;
	stale2 = 0;
	stale3 = 0;
	st = 0;
	bid = 1;
	bid1 = 1;
	bid2 = 1;
	route = 0;
	route1 = 0;
	route2 = 0;
	broadcast_mac[0] = 0xff;
	broadcast_mac[1] = 0xff;
	broadcast_mac[2] = 0xff;
	broadcast_mac[3] = 0xff;
	broadcast_mac[4] = 0xff;
	broadcast_mac[5] = 0xff;
	odr1 = NULL;
	odr1 = (struct odrmsg *) malloc(sizeof(struct odrmsg));
	/*odr1->rq=(struct rreq *)malloc(sizeof(struct rreq));
	 odr1->rp=(struct rrep *)malloc(sizeof(struct rrep));
	 odr1->pl=(struct payload *)malloc(sizeof(struct payload));
	 */
	odr2 = NULL;
	odr2 = (struct odrmsg *) malloc(sizeof(struct odrmsg));
	/*odr2->rq=(struct rreq *)malloc(sizeof(struct rreq));
	 odr2->rp=(struct rrep *)malloc(sizeof(struct rrep));
	 odr2->pl=(struct payload *)malloc(sizeof(struct payload));*/

	odr3 = NULL;
	odr3 = (struct odrmsg *) malloc(sizeof(struct odrmsg));
	/*odr3->rq=(struct rreq *)malloc(sizeof(struct rreq));
	 odr3->rp=(struct rrep *)malloc(sizeof(struct rrep));
	 odr3->pl=(struct payload *)malloc(sizeof(struct payload));*/

	odr4 = NULL;
	odr4 = (struct odrmsg *) malloc(sizeof(struct odrmsg));
	/*odr4->rq=(struct rreq *)malloc(sizeof(struct rreq));
	 odr4->rp=(struct rrep *)malloc(sizeof(struct rrep));
	 odr4->pl=(struct payload *)malloc(sizeof(struct payload));*/

	porthead = NULL;
	/*rq=NULL;
	 rq=(struct rreq *)malloc(sizeof(struct rreq));
	 rp=NULL;
	 rp=(struct rrep *)malloc(sizeof(struct rrep));
	 pl=NULL;
	 pl=(struct payload *)malloc(sizeof(struct payload));*/
	//porthead=(struct ports *)malloc(sizeof(struct ports));
	rcvmsg = NULL;
	rcvmsg1 = NULL;
	rcvmsg = (struct msg *) malloc(sizeof(struct msg));
	rcvmsg1 = (struct msg *) malloc(sizeof(struct msg));
	rhead = NULL;
	//rhead=(struct routingtable *)malloc(sizeof(struct routingtable));
	pdata = NULL;
	//pdata=(struct pendingdata *)malloc(sizeof(struct pendingdata));
	pdata1 = NULL;
	pdata1 = (struct pendingdata *) malloc(sizeof(struct pendingdata));
	ifihead = NULL;
	ifi = NULL;
	//ifihead=(struct ifiinfo *)malloc(sizeof(struct ifiinfo));
	ifi = (struct ifiinfo *) malloc(sizeof(struct ifiinfo));
	proreq = NULL;
	//proreq=(struct rreqslist *)malloc(sizeof(struct rreqslist));
	prorep = NULL;
	//prorep=(struct rrepslist *)malloc(sizeof(struct rrepslist));
	prep = NULL;
	//prep=(struct pendingrrep *)malloc(sizeof(struct pendingrrep));
	prep1 = NULL;
	prep1 = (struct pendingrrep *) malloc(sizeof(struct pendingrrep));
	if (argc != 2) {
		printf("You must provide a valid staleness parameter\n");
		exit(-1);
	}

	staleness = atoi(argv[1]);
	staleness = staleness * 1000;
	//hwa=(struct hwa_info *)malloc(sizeof(struct hwa_info));
	//hwa->ip_addr=(struct sockaddr *)malloc(sizeof(struct sockaddr));
	for (hwahead = hwa = Get_hw_addrs(); hwa != NULL ; hwa = hwa->hwa_next) {
		if (strstr(hwa->if_name, "lo") == (hwa->if_name))
			continue;
		if ((strstr(hwa->if_name, "eth0") == (hwa->if_name))
				&& ((hwa->ip_alias) != IP_ALIAS)) {
			cannonicaladdr = (struct sockaddr *) malloc(
					sizeof(struct sockaddr));
			//printf("cannonical add is %s ",sock_ntop_host((SA *)cannonicaladdr,sizeof(cannonicaladdr)));
			memset(cannonicaladdr, 0, sizeof(cannonicaladdr));
			printf("cannonical add is %s \n",
					sock_ntop_host((SA *) cannonicaladdr,
							sizeof(cannonicaladdr)));

			//cannonicaladdr=hwa->ip_addr;
			memcpy(&cannonicaladdr, &(hwa->ip_addr), sizeof(hwa->ip_addr));
			printf("cannonical add is %s \n",
					sock_ntop_host((SA *) cannonicaladdr,
							sizeof(cannonicaladdr)));
			continue;
		}
		if ((hwa->ip_alias) == IP_ALIAS)
			continue;
		addifiinfo(&ifihead, hwa->if_index, hwa->ip_addr, hwa->if_haddr);
		disply(&ifihead);

	}
	memset(hostname, 0, sizeof(hostname));
	/*debugging*/
	gethostname(host, sizeof host);
	printf("host name = '%s'\n", host);
	//debugging
	if ((hptr = gethostbyaddr(
			&(((struct sockaddr_in *) cannonicaladdr)->sin_addr),
			sizeof(struct in_addr), AF_INET)) == NULL )  //check this function
	{
		printf("The vm cannot be found from the IP address %s\n",
				Sock_ntop_host((SA *) cannonicaladdr, sizeof(cannonicaladdr)));
		exit(-1);
	}
	sprintf(hostname, "%s", hptr->h_name);
	srcaddrs = ((struct sockaddr_in *) cannonicaladdr)->sin_addr;
	myaddr = ((struct sockaddr_in *) cannonicaladdr)->sin_addr;
	printf("The source address is %s\n",
			inet_ntop(AF_INET, &(srcaddrs), src, 16));

	printf("The host name is %s and the cannonical ip address is %s\n",
			hostname,
			sock_ntop_host((SA *) cannonicaladdr, sizeof(cannonicaladdr)));
	t = sock_raw = socket(PF_PACKET, SOCK_RAW, htons(USID_PROTO));
	if (t == -1) {
		printf("socket() error\n");
		exit(-1);
	}

	unxsock = socket(AF_LOCAL, SOCK_DGRAM, 0);
	unlink(odr_path);
	memset(&odr, 0, sizeof(odr));
	odr.sun_family = AF_LOCAL;

	strcpy(odr.sun_path, odr_path);
	bind(unxsock, (struct sockaddr *) &odr, SUN_LEN(&odr));
	len = sizeof(odr);
	if (getsockname(unxsock, (struct sockaddr *) &odr, &len) < 0) {
		return -1;
	}
	printf("Unix domain socket bound to filename %s\n", odr.sun_path);

	addport(&porthead, time_path, timeservice, 0, 1); //0 is the timestamp,1 is the flag to see if it is permanent entry
	addport(&porthead, odr_path, odrservice, 0, 1);
	display_port(&porthead);

	buffer = (void *) malloc(ETH_FRAME_LEN);
	buffer1 = (void *) malloc(ETH_FRAME_LEN);
	buff1 = (void*) malloc(ETH_FRAME_LEN);
	buff2 = (void*) malloc(ETH_FRAME_LEN);
	buff3 = (void*) malloc(ETH_FRAME_LEN);
	buff4 = (void*) malloc(ETH_FRAME_LEN);
	buff5 = (void*) malloc(ETH_FRAME_LEN);
	printf("before for\n");
	for (;;) {
		printf("inside for loop\n");

		FD_ZERO(&rset);
		FD_SET(sock_raw, &rset);
		FD_SET(unxsock, &rset);
		/*t_val.tv_sec=5;
		 t_val.tv_usec=0;*/
		maxfd = ((sock_raw > unxsock) ? (sock_raw) : (unxsock));
		printf("maxfd is %d\n", maxfd);
		if ((nready = (select(maxfd + 1, &rset, NULL, NULL, NULL ))) < 0) {
			printf("inside select les than 0\n");
			if (errno == EINTR) {
				printf("select returned EINTR\n");
				continue;
			} else {
				printf("select failed");
				sleep(5);
				// exit(-1);
			}
		}
		printf("maxfd is %d\n", maxfd);
		if (FD_ISSET(unxsock,&rset)) {
			printf("inside unxsock set\n");
			//bzero(buff,sizeof(buff));
			memset(buff, 0, sizeof(buff));
			printf("debug1 \n");
			memset(&app, 0, sizeof(struct sockaddr_un));
			printf("debug2 \n");
			/*if(recvfrom(unxsock,buff,MAXLINE,0,(struct sockaddr *)&app,&sizeof(app))<0)
			 {
			 printf("Did not receive anything from the application process\n");
			 continue;
			 }*/

			//rcvmsg->dstaddr=(char *)malloc(400);
			//rcvmsg->msg=(char *)malloc(400);
			lenx = sizeof(app);
			printf("Received message from application\n");

			if (recvfrom(unxsock, rcvmsg, MAXLINE, 0, (struct sockaddr *) &app,
					&lenx) < 0) {
				printf(
						"Did not receive anything from the application process\n");
				continue;
			}

			printf("after recv from\n");
			/*memset(&rcvmsg,0,sizeof(rcvmsg));
			 if(sscanf(buff,"%s %d %s %d\n",rcvmsg->dstaddr,&rcvmsg->dstport,rcvmsg->msg,&rcvmsg->flag)!=4)
			 {
			 printf("Error reading from the unix domain socket\n");
			 continue;
			 }*/
			/*memcpy(ipaddr,rcvmsg->dstaddr,sizeof(rcvmsg->dstaddr));
			 //memcpy(&portno,rcvmsg->dstport,sizeof(rcvmsg->dstport));
			 portno=rcvmsg->dstport;
			 memcpy(mess,rcvmsg->msg,sizeof(rcvmsg->msg));
			 memcpy(&rcvflag,&(rcvmsg->flag),sizeof(rcvmsg->flag));*/
			//	memcpy(deststr,rcvmsg->dstaddr,sizeof(rcvmsg->dstaddr));
			//printf("after strcpy\n");
			//memcpy(msgstr,rcvmsg->msg,sizeof(rcvmsg->msg));
			printf("the destination address is %s \n", (rcvmsg->dstaddr));
			if (inet_pton(AF_INET, (rcvmsg->dstaddr), &(destad.sin_addr))
					<= 0) {
				printf("Invalid address\n");
			} else {
				printf("the destination address is %s \n",
						(inet_ntop(AF_INET, &(destad.sin_addr), bufad, 16)));
			}
			strcpy(rcvstr, rcvmsg->msg);
			printf("the string recev is %s\n", rcvstr);
			route = checkroute(&rhead, destad.sin_addr);
			if (route == 1) {
				tabletime = gettime(&rhead, destad.sin_addr);
			}
			/*memset(&pyl,0,sizeof(pyl));
			 pyl->type=2;
			 memcpy(&(pyl->destaddr),&destad.sin_addr,sizeof(pyl->destaddr));
			 pyl->destport=rcvmsg->dstport;
			 pyl->hopcnt=0;
			 memcpy(pyl->msg,rcvmsg->msg,500);
			 pyl->bytes=strlen(rcvmsg->msg);
			 memcpy(&(pyl->srcaddr,&srcaddr,sizeof(pyl->srcaddr)));
			 pyl->srcport=updateport(&porthead,app.sun_path);*/

			/*memcpy(&(pdata->destaddr),&((destad.sin_addr)),sizeof(destad.sin_addr));
			 pdata->destport=(rcvmsg->dstport);    //check this
			 pdata->hopcnt=0;
			 memcpy((pdata->msg),(rcvmsg->msg),strlen(rcvmsg->msg)+1);
			 pdata->bytes=strlen(rcvmsg->msg)+1;
			 memcpy(&(pdata->srcaddr),&(srcaddr),sizeof(pdata->srcaddr));*/
			deletestale(&porthead, timetolive);
			display_port(&porthead);
			ispath = checkpath(&porthead, app.sun_path);

			if (ispath == 0) {
				assport(&tempport);
				port_time = getcurrenttime();
				printf("The timestamp is %lu\n",port_time);
				addport(&porthead, app.sun_path, tempport, port_time, 0);
				display_port(&porthead);
			}
			/*else if(ispath==1)
			 {
			 reintialize(&porthead,app.sun_path);
			 }*/
			pono = getport(&porthead, app.sun_path);   //check this
			currenttime = getcurrenttime();
			timedif = currenttime - tabletime;
			if (timedif >= staleness)
				stale = 1;
			else
				stale = 0;
			if (rcvmsg->flag == 1)
				discovery = 1;
			if ((route == 0) || (discovery == 1) || (stale == 1)) {
				if ((discovery == 1) || (stale == 1)) {
					deleteentry(&rhead, destad.sin_addr);
				}
				insertdata(&pdata, destad.sin_addr, rcvmsg->dstport, 0, rcvstr,
						strlen(rcvmsg->msg) + 1, srcaddrs, pono);
				printf("payload added to the list\n");
				display_ptable(&pdata);

				//insertdata(&pdata,pdata->destaddr,pdata->destport,pdata->hopcnt,pdata->msg,pdata->bytes,pdata->srcaddr,pdata->srcport);
				memset(buffer, 0, ETH_FRAME_LEN);
				printf("before for loop for getifi\n");
				if ((hptr1 = gethostbyaddr(&srcaddrs, sizeof(struct in_addr),
						AF_INET)) == NULL )  //check this function
				{
					printf("Th vm cannot be found from the IP address %s\n",
							inet_ntop(AF_INET, &(srcaddrs), src1, 16));
					exit(-1);
				}
				printf("after gethostbyaddr\n");
				sprintf(hostname1, "%s", hptr1->h_name);
				printf("after sprintf\n");
				bzero(src, sizeof(src));
				inet_ntop(AF_INET, &(srcaddrs), src, 16);
				printf("after inet-ntop\n");
				printf(
						"The host name is %s and the src cannonical ip address is %s\n",
						hostname1, src);
				if ((hptr2 = gethostbyaddr(&(destad.sin_addr),
						sizeof(struct in_addr), AF_INET)) == NULL ) //check this function
				{
					printf("Th vm cannot be found from the IP address %s\n",
							inet_ntop(AF_INET, &(destad.sin_addr), bufad, 16));
					exit(-1);
				}
				sprintf(hostname2, "%s", hptr2->h_name);
				bzero(bufad, sizeof(bufad));
				printf(
						"The host name is %s and the dest cannonical ip address is %s\n",
						hostname2,
						inet_ntop(AF_INET, &(destad.sin_addr), bufad, 16));
				//bid++;
				for (ifi = ifihead; ifi != NULL ; ifi = ifi->next) {
					printf("insisde for loop for getifi, %d\n", ifi->indx);
					etherhead = buffer;
					eh = (struct ethhdr *) etherhead;
					memset(&socket_address, 0, sizeof(struct sockaddr_ll));
					socket_address.sll_family = PF_PACKET;
					socket_address.sll_ifindex = ifi->indx;
					socket_address.sll_hatype = ARPHRD_ETHER;
					socket_address.sll_pkttype = PACKET_BROADCAST;
					socket_address.sll_halen = ETH_ALEN;
					socket_address.sll_addr[0] = broadcast_mac[0];
					socket_address.sll_addr[1] = broadcast_mac[1];
					socket_address.sll_addr[2] = broadcast_mac[2];
					socket_address.sll_addr[3] = broadcast_mac[3];
					socket_address.sll_addr[4] = broadcast_mac[4];
					socket_address.sll_addr[5] = broadcast_mac[5];
					printf("insisde for loop for getifi 2\n");
					memcpy((void*) buffer, (void*) broadcast_mac, ETH_ALEN);
					printf("here1\n");
					memcpy((void*) (buffer + ETH_ALEN), (void*) ifi->hwaddr,
							ETH_ALEN);
					printf("here\n");
					eh->h_proto = htons(USID_PROTO);
					odr1 = buffer + 14;
					odr1->type = htonl(0);

					printf("before assigning source addr\n");
					printf("The size is %ld\n", sizeof(srcaddrs.s_addr));
					printf("The size is %ld \n",
							sizeof(odr1->rq.srcaddr.s_addr));
					printf("one is clear  %u\n", htonl(srcaddrs.s_addr));
					printf("both clear %u\n", odr1->rq.srcaddr.s_addr);
					odr1->rq.srcaddr.s_addr = htonl(srcaddrs.s_addr);
					printf("after assigning source add\n");
					printf("before assigning dest address\n");
					odr1->rq.dstaddr.s_addr = htonl(destad.sin_addr.s_addr);
					printf("after assigning dest address\n");
					odr1->rq.broadcastid = htonl(bid);
					//bid++;
					odr1->rq.hopcnt = htonl(0);
					odr1->rq.rrepsent = htonl(0);
					if (discovery == 1)
						odr1->rq.forceddiscovery = htonl(1);
					else
						odr1->rq.forceddiscovery = htonl(0);
					printf("after assigning values to rreq\n");
					/*req=buffer+14;
					 req->type=0;
					 req->srcaddr=srcaddr;
					 req->dstaddr=destad.sin_addr;
					 req->broadcastid=bid;

					 bid++;
					 req->hopcnt=0;
					 req->rrepsent=0;
					 if(discovery==1)
					 req->forceddiscovery=1;
					 else
					 req->forceddiscovery=0;*/
					/*if((hptr1=gethostbyaddr(&srcaddrs,sizeof(struct in_addr),AF_INET))==NULL)  //check this function
					 {
					 printf("Th vm cannot be found from the IP address %s\n",inet_ntop(AF_INET,&(srcaddrs),src1,16));
					 exit(-1);
					 }
					 printf("after gethostbyaddr\n");
					 sprintf(hostname1,"%s",hptr1->h_name);
					 printf("after sprintf\n");
					 bzero(src,sizeof(src));
					 inet_ntop(AF_INET,&(srcaddrs),src,16);
					 printf("after inet-ntop\n");
					 printf("The host name is %s and the src cannonical ip address is %s\n",hostname1,src);
					 if((hptr2=gethostbyaddr(&(destad.sin_addr),sizeof(struct in_addr),AF_INET))==NULL)  //check this function
					 {
					 printf("Th vm cannot be found from the IP address %s\n",inet_ntop(AF_INET,&(destad.sin_addr),bufad,16));
					 exit(-1);
					 }
					 sprintf(hostname2,"%s",hptr2->h_name);
					 bzero(bufad,sizeof(bufad));
					 printf("The host name is %s and the dest cannonical ip address is %s\n",hostname2,inet_ntop(AF_INET,&(destad.sin_addr),bufad,16));*/

					send_result = sendto(sock_raw, buffer, ETH_FRAME_LEN, 0,
							(struct sockaddr*) &socket_address,
							sizeof(socket_address));
					if (send_result == -1) {
						printf("sendto() error\n");
						exit(-1);
					}
					printf(
							"ODR at node vm %s: sending frame header src vm %s dest :%s, ODR message type %d src vm%s dest vm %s\n",
							hostname, ifi->hwaddr, broadcast_mac, odr1->type,
							hostname1, hostname2);

					free(buffer);
				}//close for
					addproreq(&proreq, NTOHLSV(odr1->rq.srcaddr),
							ntohl(odr1->rq.broadcastid),
							ntohl(odr1->rq.hopcnt));
					display_rreqtable(&proreq);
				  bid++;
			}

			else {

				getroute(&rhead, destad.sin_addr, nexthp, &indx);
				printf("routing entry");
				display_rtable(&rhead);
				//updatetimestamp(&rhead,destad.sin_addr);
				//buildheader(nexthp,destad.sin_addr,htons(USID_PROTO));
				memset(buffer, 0, ETH_FRAME_LEN);
				etherhead = buffer;
				eh = (struct ethhdr *) etherhead;
				memset(&socket_address, 0, sizeof(struct sockaddr_ll));
				socket_address.sll_family = PF_PACKET;
				socket_address.sll_ifindex = indx;
				socket_address.sll_hatype = ARPHRD_ETHER;
				socket_address.sll_pkttype = PACKET_OTHERHOST;
				socket_address.sll_halen = ETH_ALEN;
				socket_address.sll_addr[0] = nexthp[0];
				socket_address.sll_addr[1] = nexthp[1];
				socket_address.sll_addr[2] = nexthp[2];
				socket_address.sll_addr[3] = nexthp[3];
				socket_address.sll_addr[4] = nexthp[4];
				socket_address.sll_addr[5] = nexthp[5];
				memcpy((void*) buffer, (void*) nexthp, ETH_ALEN);
				getifihw(&ifihead, indx, srchw);    //check this function
				memcpy((void*) (buffer + ETH_ALEN), (void*) srchw, ETH_ALEN);
				eh->h_proto = htons(USID_PROTO);
				odr1 = buffer + 14;
				odr1->type = htonl(2);
				odr1->pl.destport = htonl(rcvmsg->dstport);   //check this
				odr1->pl.hopcnt = htonl(0);
				memcpy(odr1->pl.msg, rcvmsg->msg, strlen(rcvmsg->msg) + 1);
				odr1->pl.bytes = htonl(strlen(rcvmsg->msg) + 1);
				hton = htonl(srcaddrs.s_addr);
				memcpy(&(odr1->pl.srcaddr.s_addr), &(hton), sizeof(long));
				odr1->pl.srcport = htonl(getport(&porthead, app.sun_path));
				/*pyl=buffer+14;
				 memset(&pyl,0,sizeof(pyl));
				 pyl->type=2;
				 memcpy(&(pyl->destaddr),&destad.sin_addr,sizeof(pyl->destaddr));
				 pyl->destport=rcvmsg->dstport;
				 pyl->hopcnt=0;
				 memcpy(pyl->msg,rcvmsg->msg,500);
				 pyl->bytes=strlen(rcvmsg->msg);
				 memcpy(&(pyl->srcaddr,&srcaddr,sizeof(pyl->srcaddr)));
				 pyl->srcport=updateport(&porthead,app.sun_path);*/
				send_result = sendto(sock_raw, buffer, ETH_FRAME_LEN, 0,
						(struct sockaddr*) &socket_address,
						sizeof(socket_address));
				if (send_result == -1) {
					printf("sendto() error\n");
					exit(-1);
				}
				printf("payload sent as route found\n");
				free(buffer);

			}  //close else

		}  //close isset unxsock

		else if (FD_ISSET(sock_raw,&rset)) {
			printf("odr received packet\n");
			memset(buff1, 0, sizeof(buff1));
			memset(&socket_address1, 0, sizeof(struct sockaddr_ll));
			lens2 = sizeof(socket_address1);
			length = recvfrom(sock_raw, buff1, ETH_FRAME_LEN, 0,
					(struct sockaddr*) &socket_address1, &lens2);
			if (length == -1) {
				printf("recvfrom() error\n");
				continue;
				//exit(-1);
			}
			odr2 = buff1 + 14;

			//req1=buff1+14;
			interface_index = socket_address1.sll_ifindex;
			neighbour_addr = socket_address1.sll_addr;
			if (ntohl(odr2->type) == 0)

			{
				printf("RREQ received from interface %d,neighbour: [",
						interface_index);
				for (i = 0; i < 6; i++)
					printf("%x ", neighbour_addr[i]);

				printf("]\n Protocol: %x:\n",
						ntohs(socket_address1.sll_protocol));
				route1 = checkroute(&rhead, NTOHLSV(odr2->rq.dstaddr));
				printf("routing table\n");
				display_rtable(&rhead);
				if (route1 == 1) {
					tabletime1 = gettime(&rhead, NTOHLSV(odr2->rq.dstaddr));
				}
				currenttime1 = getcurrenttime();
				timedif1 = currenttime1 - tabletime1;
				if (timedif1 >= staleness)
					stale1 = 1;
				else
					stale1 = 0;

				if ((ntohl(odr2->rq.dstaddr.s_addr) == myaddr.s_addr)
						|| ((route1 == 1) && (stale1 == 0)
								&& (ntohl(odr2->rq.forceddiscovery) == 0))) {
					//if not dest
					//keep track of rreqs processed and check if this rreeq was not processed or if rreq is processed
					//but more efficient and discovery bit is not set, then
					//generate rrep to the neighbour and store forward routes if rrep sent bit
					//is not set and update timestamp in routing table
					//and check the number of hops
					//and the interface through whch it came and update table(i.e noof hops and timestamp)
					//if less or equal to no. of hops or from different interface,continue to flood by setting rrep sent
					//bit in rreq if more efficient route or previouly unknown source or dicovery bit is set.
					// if  destination
					//generate rrep if rreq not processed previosly or processed previously but
					//lower or equal hop count or discovery bit set and rrep sent bit is not set and store reverse routes,flood rreq by
					//setting rrep sent flag

					if (ntohl(odr2->rq.dstaddr.s_addr) == myaddr.s_addr) {

						ispro1 = isprocessed(&proreq, NTOHLSV(odr2->rq.srcaddr),
								ntohl(odr2->rq.broadcastid));
						//ispro1=ispresent(&proreq,odr2->rq->srcaddr);
						if (ispro1 == 1) {
							//tablehopcnt=gethopcount(&proreq,odr2->rq->srcaddr,odr2->rq->broadcastid);
							tablehopcnt = gethopcount(&rhead,
									NTOHLSV(odr2->rq.srcaddr));
							odr2->rq.hopcnt = htonl(ntohl(odr2->rq.hopcnt) + 1);
							if (tablehopcnt <= ntohl(odr2->rq.hopcnt))
								hopflag1 = 1;
							else
								hopflag1 = 0;
							tt = gettime(&rhead, NTOHLSV(odr2->rq.srcaddr));
							ct = getcurrenttime();
							td = ct - tt;
							if (td >= staleness)
								st = 1;
							else
								st = 0;
						}

						if ((ispro1 == 0) || (hopflag1 == 1)
								|| (ntohl(odr2->rq.forceddiscovery) == 1))//&&(odr2->rq->rrepsent==0))
								{
							if ((ispro1 == 0)
									&& (ntohl(odr2->rq.forceddiscovery) == 0))
								addroute(&rhead, neighbour_addr,
										NTOHLSV(odr2->rq.srcaddr),
										ntohl(odr2->rq.hopcnt), interface_index,
										getcurrenttime());
							display_rtable(&rhead);
							if ((hopflag1 == 1)
									|| (ntohl(odr2->rq.forceddiscovery) == 1)
									|| (st == 1))
									//if(hopflag1==1)
									{
								if (ispro1 == 1)
									deleteentry(&rhead,
											NTOHLSV(odr2->rq.srcaddr));
								addroute(&rhead, neighbour_addr,
										NTOHLSV(odr2->rq.srcaddr),
										ntohl(odr2->rq.hopcnt), interface_index,
										getcurrenttime());
								display_rtable(&rhead);
							}
							if (ntohl(odr2->rq.rrepsent) == 0) {
								memset(buff2, 0, ETH_FRAME_LEN);
								etherhead = buff2;
								eh = (struct ethhdr *) etherhead;
								memset(&socket_address3, 0,
										sizeof(struct sockaddr_ll));
								socket_address3.sll_family = PF_PACKET;
								socket_address3.sll_ifindex = interface_index;
								socket_address3.sll_hatype = ARPHRD_ETHER;
								socket_address3.sll_pkttype = PACKET_OTHERHOST;
								socket_address3.sll_halen = ETH_ALEN;
								socket_address3.sll_addr[0] = neighbour_addr[0];
								socket_address3.sll_addr[1] = neighbour_addr[1];
								socket_address3.sll_addr[2] = neighbour_addr[2];
								socket_address3.sll_addr[3] = neighbour_addr[3];
								socket_address3.sll_addr[4] = neighbour_addr[4];
								socket_address3.sll_addr[5] = neighbour_addr[5];
								//getifihw(&ifihead,indx,srchw1);    //check this function
								getifihw(&ifihead, interface_index, srchw1);
								memcpy((void*) buff2, (void*) neighbour_addr,
										ETH_ALEN);
								memcpy((void*) (buff2 + ETH_ALEN),
										(void*) srchw1, ETH_ALEN);
								eh->h_proto = htons(USID_PROTO);
								odr3 = buff2 + 14;
								odr3->type = htonl(1);
								odr3->rp.srcaddr.s_addr = htonl(myaddr.s_addr);
								odr3->rp.dstaddr = odr2->rq.srcaddr;
								//odr3->rp->broadcastid=odr2->rq->broadcastid;
								/*odr3->rp->broadcastid=bid2;
								 bid2++;*/
								odr3->rp.hopcnt = htonl(0);
								odr3->rp.forceddiscovery =
										odr2->rq.forceddiscovery;
								//odr3->rp->rrepsent=htonl(1);

								send_result = sendto(sock_raw, buff2,
										ETH_FRAME_LEN, 0,
										(struct sockaddr*) &socket_address3,
										sizeof(socket_address3));
								if (send_result == -1) {
									printf("sendto() error\n");
									exit(-1);
								}

								free(buff2);
							}
							addproreq(&proreq, NTOHLSV(odr2->rq.srcaddr),
									ntohl(odr2->rq.broadcastid),
									ntohl(odr2->rq.hopcnt));
							display_rreqtable(&proreq);
							if (((ispro1 == 0) || (hopflag1 == 1))
									&& (ntohl(odr2->rq.forceddiscovery) == 0)) {
								memset(buffer1, 0, ETH_FRAME_LEN);
								for (ifi = ifihead; ifi != NULL ;
										ifi = ifi->next) {

									if (ifi->indx == interface_index)
										continue;
									etherhead1 = buffer1;
									eh1 = (struct ethhdr *) etherhead1;
									memset(&socket_address2, 0,
											sizeof(struct sockaddr_ll));
									socket_address2.sll_family = PF_PACKET;
									socket_address2.sll_ifindex = ifi->indx;
									socket_address2.sll_hatype = ARPHRD_ETHER;
									socket_address2.sll_pkttype =
											PACKET_BROADCAST;
									socket_address2.sll_halen = ETH_ALEN;
									socket_address2.sll_addr[0] =
											broadcast_mac[0];
									socket_address2.sll_addr[1] =
											broadcast_mac[1];
									socket_address2.sll_addr[2] =
											broadcast_mac[2];
									socket_address2.sll_addr[3] =
											broadcast_mac[3];
									socket_address2.sll_addr[4] =
											broadcast_mac[4];
									socket_address2.sll_addr[5] =
											broadcast_mac[5];

									memcpy((void*) buffer1,
											(void*) broadcast_mac, ETH_ALEN);
									memcpy((void*) (buffer1 + ETH_ALEN),
											(void*) ifi->hwaddr, ETH_ALEN);
									eh1->h_proto = htons(USID_PROTO);
									odr4 = buffer1 + 14;
									odr4->type = htonl(0);

									odr4->rq.srcaddr = odr2->rq.srcaddr;
									odr4->rq.dstaddr = odr2->rq.dstaddr;
									odr4->rq.broadcastid = odr2->rq.broadcastid;
									odr4->rq.hopcnt = (odr2->rq.hopcnt);

									odr4->rq.rrepsent = htonl(1);
									/*if(discovery==1)
									 odr4->rq->forceddiscovery=1;
									 else*/
									odr4->rq.forceddiscovery =
											odr2->rq.forceddiscovery;

									send_result = sendto(sock_raw, buffer,
											ETH_FRAME_LEN, 0,
											(struct sockaddr*) &socket_address2,
											sizeof(socket_address1));
									if (send_result == -1) {
										printf("sendto() error\n");
										exit(-1);
									}

									free(buffer1);
								}   //close for

							}
						} else
							free(buff1);   //discard the packet
					} else {
						ispro1 = isprocessed(&proreq, NTOHLSV(odr2->rq.srcaddr),
								ntohl(odr2->rq.broadcastid));
						if (ispro1 == 1) {
							//tablehopcnt=gethopcount(&proreq,odr2->rq->srcaddr,odr2->rq->broadcastid);
							tablehopcnt = gethopcount(&rhead,
									NTOHLSV(odr2->rq.srcaddr));
							odr2->rq.hopcnt = htonl(ntohl(odr2->rq.hopcnt) + 1);
							if (tablehopcnt <= ntohl(odr2->rq.hopcnt))
								hopflag1 = 1;
							else
								hopflag1 = 0;
							tt = gettime(&rhead, NTOHLSV(odr2->rq.srcaddr));
							ct = getcurrenttime();
							td = ct - tt;
							if (td >= staleness)
								st = 1;
							else
								st = 0;
						}

						if (((ispro1 == 0) || (hopflag1 == 1))
								&& (ntohl(odr2->rq.forceddiscovery) == 0)) //&&(odr2->rq->rrepsent==0))
								{
							if ((ispro1 == 0)
									&& (ntohl(odr2->rq.forceddiscovery) == 0))
								addroute(&rhead, neighbour_addr,
										NTOHLSV(odr2->rq.srcaddr),
										ntohl(odr2->rq.hopcnt), interface_index,
										getcurrenttime());
							display_rtable(&rhead);
							if ((hopflag1 == 1) || (st == 1)) {
								deleteentry(&rhead, NTOHLSV(odr2->rq.srcaddr));
								addroute(&rhead, neighbour_addr,
										NTOHLSV(odr2->rq.srcaddr),
										ntohl(odr2->rq.hopcnt), interface_index,
										getcurrenttime());
								display_rtable(&rhead);
							}
							if (ntohl(odr2->rq.rrepsent) == 0) {
								memset(buff2, 0, ETH_FRAME_LEN);
								etherhead = buff2;
								eh = (struct ethhdr *) etherhead;
								memset(&socket_address3, 0,
										sizeof(struct sockaddr_ll));
								socket_address3.sll_family = PF_PACKET;
								socket_address3.sll_ifindex = interface_index;
								socket_address3.sll_hatype = ARPHRD_ETHER;
								socket_address3.sll_pkttype = PACKET_OTHERHOST;
								socket_address3.sll_halen = ETH_ALEN;
								socket_address3.sll_addr[0] = neighbour_addr[0];
								socket_address3.sll_addr[1] = neighbour_addr[1];
								socket_address3.sll_addr[2] = neighbour_addr[2];
								socket_address3.sll_addr[3] = neighbour_addr[3];
								socket_address3.sll_addr[4] = neighbour_addr[4];
								socket_address3.sll_addr[5] = neighbour_addr[5];
								getifihw(&ifihead, interface_index, srchw1); //check this function
								memcpy((void*) buff2, (void*) neighbour_addr,
										ETH_ALEN);
								memcpy((void*) (buff2 + ETH_ALEN),
										(void*) srchw1, ETH_ALEN);
								eh->h_proto = htons(USID_PROTO);
								odr3 = buff2 + 14;
								odr3->type = htonl(1);
								odr3->rp.srcaddr.s_addr = htonl(myaddr.s_addr);
								odr3->rp.dstaddr = odr2->rq.srcaddr;
								//odr3->rp->broadcastid=odr2->rq->broadcastid;
								/*odr3->rp->broadcastid=bid1;
								 bid1++;*/
								odr3->rp.hopcnt = htonl(0);
								odr3->rp.forceddiscovery =
										odr2->rq.forceddiscovery;
								//odr3->rp->rrepsent=htonl(1);

								send_result = sendto(sock_raw, buff2,
										ETH_FRAME_LEN, 0,
										(struct sockaddr*) &socket_address3,
										sizeof(socket_address3));
								if (send_result == -1) {
									printf("sendto() error\n");
									exit(-1);
								}

								free(buff2);
							}
							addproreq(&proreq, NTOHLSV(odr2->rq.srcaddr),
									ntohl(odr2->rq.broadcastid),
									ntohl(odr2->rq.hopcnt));
							display_rreqtable(&proreq);
							memset(buffer1, 0, ETH_FRAME_LEN);
							for (ifi = ifihead; ifi != NULL ; ifi = ifi->next) {

								if (ifi->indx == interface_index)
									continue;
								etherhead1 = buffer1;
								eh1 = (struct ethhdr *) etherhead1;
								memset(&socket_address2, 0,
										sizeof(struct sockaddr_ll));
								socket_address2.sll_family = PF_PACKET;
								socket_address2.sll_ifindex = ifi->indx;
								socket_address2.sll_hatype = ARPHRD_ETHER;
								socket_address2.sll_pkttype = PACKET_BROADCAST;
								socket_address2.sll_halen = ETH_ALEN;
								socket_address2.sll_addr[0] = broadcast_mac[0];
								socket_address2.sll_addr[1] = broadcast_mac[1];
								socket_address2.sll_addr[2] = broadcast_mac[2];
								socket_address2.sll_addr[3] = broadcast_mac[3];
								socket_address2.sll_addr[4] = broadcast_mac[4];
								socket_address2.sll_addr[5] = broadcast_mac[5];

								memcpy((void*) buffer1, (void*) broadcast_mac,
										ETH_ALEN);
								memcpy((void*) (buffer1 + ETH_ALEN),
										(void*) ifi->hwaddr, ETH_ALEN);
								eh1->h_proto = htons(USID_PROTO);
								odr4 = buffer1 + 14;
								odr4->type = htonl(0);
								odr4->rq.srcaddr = odr2->rq.srcaddr;
								odr4->rq.dstaddr = odr2->rq.dstaddr;
								odr4->rq.broadcastid = odr2->rq.broadcastid;
								odr4->rq.hopcnt = (odr2->rq.hopcnt);
								odr4->rq.rrepsent = htonl(1);
								/*if(discovery==1)
								 odr4->rq->forceddiscovery=1;
								 else*/
								odr4->rq.forceddiscovery =
										odr2->rq.forceddiscovery;
								send_result = sendto(sock_raw, buffer,
										ETH_FRAME_LEN, 0,
										(struct sockaddr*) &socket_address2,
										sizeof(socket_address1));
								if (send_result == -1) {
									printf("sendto() error\n");
									exit(-1);
								}

								free(buffer1);
							}   //close for
						} else
							free(buff1);
					}   //close else

				}

				else {
					memset(buff2, 0, ETH_FRAME_LEN);
					if ((route1 == 0) || (stale1 == 1)
							|| (ntohl(odr2->rq.forceddiscovery) == 1)) {
						ispro = isprocessed(&proreq, NTOHLSV(odr2->rq.srcaddr),
								ntohl(odr2->rq.broadcastid));
						if (ispro == 1) {
							//tablehopcnt=gethopcount(&proreq,odr2->rq->srcaddr,odr2->rq->broadcastid);
							tablehopcnt = gethopcount(&rhead,
									NTOHLSV(odr2->rq.srcaddr));
							odr2->rq.hopcnt = htonl(ntohl(odr2->rq.hopcnt) + 1);
							//ntohl(odr2->rq->hopcnt++);
							if (tablehopcnt <= ntohl(odr2->rq.hopcnt))

								hopflag = 1;
							else
								hopflag = 0;
							tt = gettime(&rhead, NTOHLSV(odr2->rq.srcaddr));
							ct = getcurrenttime();
							td = ct - tt;
							if (td >= staleness)
								st = 1;
							else
								st = 0;
						}
						if ((ispro == 0) || (hopflag == 1)) {
							if ((ispro == 0)
									&& (ntohl(odr2->rq.forceddiscovery) == 0))
								addroute(&rhead, neighbour_addr,
										NTOHLSV(odr2->rq.srcaddr),
										ntohl(odr2->rq.hopcnt), interface_index,
										getcurrenttime());
							display_rtable(&rhead);

							if ((hopflag == 1)
									|| (ntohl(odr2->rq.forceddiscovery) == 1)
									|| (st == 1))
									//if(hopflag==1)
									{
								if (ispro == 1)
									deleteentry(&rhead,
											NTOHLSV(odr2->rq.srcaddr));
								addroute(&rhead, neighbour_addr,
										NTOHLSV(odr2->rq.srcaddr),
										ntohl(odr2->rq.hopcnt), interface_index,
										getcurrenttime());
								display_rtable(&rhead);
							}
							if ((stale1 == 1)
									|| (ntohl(odr2->rq.forceddiscovery) == 1)) //should it also be forceddiscovery==1
									{
								deleteentry(&rhead, NTOHLSV(odr2->rq.dstaddr));
							}

							//If it has not seen that rrreq before broadcast on all interfaces except the one on which it came from and store(update) reverse path
							//and add the processed rreq
							memset(buffer, 0, ETH_FRAME_LEN);

							for (ifi = ifihead; ifi != NULL ; ifi = ifi->next) {

								if (ifi->indx == interface_index)
									continue;
								etherhead = buffer;
								eh = (struct ethhdr *) etherhead;
								memset(&socket_address3, 0,
										sizeof(struct sockaddr_ll));
								socket_address3.sll_family = PF_PACKET;
								socket_address3.sll_ifindex = ifi->indx;
								socket_address3.sll_hatype = ARPHRD_ETHER;
								socket_address3.sll_pkttype = PACKET_BROADCAST;
								socket_address3.sll_halen = ETH_ALEN;
								socket_address3.sll_addr[0] = broadcast_mac[0];
								socket_address3.sll_addr[1] = broadcast_mac[1];
								socket_address3.sll_addr[2] = broadcast_mac[2];
								socket_address3.sll_addr[3] = broadcast_mac[3];
								socket_address3.sll_addr[4] = broadcast_mac[4];
								socket_address3.sll_addr[5] = broadcast_mac[5];

								memcpy((void*) buffer, (void*) broadcast_mac,
										ETH_ALEN);
								memcpy((void*) (buffer + ETH_ALEN),
										(void*) ifi->hwaddr, ETH_ALEN);
								eh->h_proto = htons(USID_PROTO);
								odr3 = buffer + 14;
								odr3->type = htonl(0);
								//odr3->rq->srcaddr=odr2->rq->srcaddr;
								odr3->rq.srcaddr.s_addr = htonl(myaddr.s_addr);
								odr3->rq.dstaddr = odr2->rq.dstaddr;
								//odr3->rq->broadcastid=odr2->rq->broadcastid;
								odr3->rq.broadcastid = htonl(bid);
								//bid++;
								//odr3->rq->hopcnt=(odr2->rq->hopcnt);
								odr3->rq.hopcnt = htonl(0);
								/*odr3->rq->rrepsent=0;
								 if(discovery==1)
								 odr3->rq->forceddiscovery=1;
								 else*/
								odr3->rq.forceddiscovery = htonl(0);
								send_result = sendto(sock_raw, buffer,
										ETH_FRAME_LEN, 0,
										(struct sockaddr*) &socket_address3,
										sizeof(socket_address3));
								if (send_result == -1) {
									printf("sendto() error\n");
									exit(-1);
								}

								free(buffer);
								addproreq(&proreq, NTOHLSV(odr2->rq.srcaddr),
										ntohl(odr2->rq.broadcastid),
										ntohl(odr2->rq.hopcnt));
								display_rreqtable(&proreq);
							}	bid++;									//close for
						} else {
							//discard the packet
							free(buff1);
						}
					}
				}

			}  //close if type=0
			else if (ntohl(odr2->type) == 1) {
				printf("RREP received from interface %d,neighbour: [",
						interface_index);
				for (i = 0; i < 6; i++)
					printf("%x ", neighbour_addr[i]);

				printf("]\n Protocol: %x:\n",
						ntohs(socket_address1.sll_protocol));
				route2 = checkroute(&rhead, NTOHLSV(odr2->rp.dstaddr));
				if (route2 == 1) {
					tabletime2 = gettime(&rhead, NTOHLSV(odr2->rp.dstaddr));
				}
				currenttime2 = getcurrenttime();
				timedif2 = currenttime2 - tabletime2;
				if (timedif2 >= staleness)
					stale2 = 1;
				else
					stale2 = 0;

				if ((ntohl(odr2->rp.dstaddr.s_addr) == myaddr.s_addr)
						|| ((route2 == 1) && (stale2 == 0)
								&& (ntohl(odr2->rp.forceddiscovery) == 0))) {
					if (ntohl(odr2->rp.dstaddr.s_addr) == myaddr.s_addr) {

						/*ispro1=isrepprocessed(&prorep,odr2->rp->srcaddr,odr2->rp->broadcastid);
						 if(ispro1==1)
						 {*/
						//tablehopcnt=gethopcountrep(&prorep,odr2->rp->srcaddr,odr2->rp->broadcastid);
						tablehopcnt = gethopcount(&rhead,
								NTOHLSV(odr2->rp.srcaddr));
						if (tablehopcnt != 0) {
							tt = gettime(&rhead, NTOHLSV(odr2->rp.srcaddr));
							ct = getcurrenttime();
							td = ct - tt;
							if (td >= staleness)
								st = 1;
							else
								st = 0;
						}
						odr2->rp.hopcnt = htonl(ntohl(odr2->rp.hopcnt) + 1);
						if ((tablehopcnt <= ntohl(odr2->rp.hopcnt))
								|| (ntohl(odr2->rp.forceddiscovery) == 1)
								|| (st == 1)) {
							hopflag2 = 1;
							deleteentry(&rhead, NTOHLSV(odr2->rp.srcaddr));
							addroute(&rhead, neighbour_addr,
									NTOHLSV(odr2->rp.srcaddr),
									ntohl(odr2->rp.hopcnt), interface_index,
									getcurrenttime());
							display_rtable(&rhead);
						} else
							hopflag2 = 0;
						//}
						//if(ispro1==0)||(hopflag2==1)||(odr2->rp->forceddiscovery==1))

						/*if(((ispro1==0)||(hopflag1==1)||(odr2->rq->forceddiscovery==1))&&(odr2->rq->rrepsent==0))
						 {
						 if((ispro1==0)&&(odr->rq->forceddiscovery==0))
						 addroute(&rhead,neighbour_addr,odr2->rq->srcaddr,odr2->rq->hopcnt,interface_index,time(NULL));
						 if((hopflag1==1)||(odr2->rq->forceddiscovery==1))
						 //if(hopflag1==1)
						 {
						 if(ispro1==1)
						 deleteentry(&rhead,odr2->rq->srcaddr);
						 addroute(&rhead,neighbour_addr,odr2->rq->srcaddr,odr2->rq->hopcnt,interface_index,time(NULL));
						 }*/

						pending: datapending = isdatapending(&pdata,
								NTOHLSV(odr2->rp.srcaddr));
						rreppending = isrreppending(&prep,
								NTOHLSV(odr2->rp.srcaddr));
						if ((datapending == 1) || (rreppending == 1)) {
							//if(ispro1==0)&&(odr->rp->forceddiscovery==0))
							//addroute(&rhead,neighbour_addr,odr2->rp->srcaddr,odr2->rp->hopcnt,interface_index,time(NULL));
							/*if((hopflag2==1)||(odr2->rp->forceddiscovery==1))
							 {
							 if(ispro1==1)
							 deleteentry(&rhead,odr2->rp->srcaddr);
							 addroute(&rhead,neighbour_addr,odr2->rp->srcaddr,odr2->rp->hopcnt,interface_index,time(NULL));
							 }*/
							if (datapending == 1) {
								memset(buff3, 0, ETH_FRAME_LEN);
								etherhead = buff3;
								eh = (struct ethhdr *) etherhead;
								memset(&socket_address3, 0,
										sizeof(struct sockaddr_ll));
								socket_address3.sll_family = PF_PACKET;
								socket_address3.sll_ifindex = interface_index;
								socket_address3.sll_hatype = ARPHRD_ETHER;
								socket_address3.sll_pkttype = PACKET_OTHERHOST;
								socket_address3.sll_halen = ETH_ALEN;
								socket_address3.sll_addr[0] = neighbour_addr[0];
								socket_address3.sll_addr[1] = neighbour_addr[1];
								socket_address3.sll_addr[2] = neighbour_addr[2];
								socket_address3.sll_addr[3] = neighbour_addr[3];
								socket_address3.sll_addr[4] = neighbour_addr[4];
								socket_address3.sll_addr[5] = neighbour_addr[5];
								getifihw(&ifihead, interface_index, srchw2); //check this function
								memcpy((void*) buff3, (void*) neighbour_addr,
										ETH_ALEN);
								memcpy((void*) (buff3 + ETH_ALEN),
										(void*) srchw2, ETH_ALEN);
								eh->h_proto = htons(USID_PROTO);
								getpendingdata(&pdata, odr2->rp.srcaddr,
										&pdata1);
								display_ptable(&pdata);

								memset(&odr3, 0, sizeof(odr3));
								printf("After memset\n");

								odr3 = buff3 + 14;
								odr3->type = htonl(2);
								odr3->pl.srcaddr.s_addr = htonl(
										pdata1->srcaddr.s_addr);
								odr3->pl.destaddr.s_addr = htonl(
										pdata1->destaddr.s_addr);
								odr3->pl.srcport = htonl(pdata1->srcport);
								odr3->pl.destport = htonl(pdata1->destport);
								odr3->pl.hopcnt = htonl(pdata1->hopcnt);
								odr3->pl.bytes = htonl(pdata1->bytes);
								//odr3->pl->msg=pdata1->msg;
								printf("before msg copy\n");
								memcpy((odr3->pl.msg), (pdata1->msg),
										strlen(pdata1->msg) + 1);  //check once
								/*odr3->rp->broadcastid=bid2;
								 bid2++;
								 odr3->rp->hopcnt=0;
								 odr3->rp->forceddiscovery=odr2->rq->forceddiscovery;
								 odr3->rp->rrepsent=1;*/

								send_result = sendto(sock_raw, buff3,
										ETH_FRAME_LEN, 0,
										(struct sockaddr*) &socket_address3,
										sizeof(socket_address3));
								if (send_result == -1) {
									printf("sendto() error\n");
									exit(-1);
								}
								deletependingdata(&pdata,
										NTOHLSV(odr2->rp.srcaddr));
								display_ptable(&pdata);

								free(buff3);
								goto pending;
							}
							if (rreppending == 1) {
								memset(buff3, 0, ETH_FRAME_LEN);
								etherhead = buff3;
								eh = (struct ethhdr *) etherhead;
								memset(&socket_address3, 0,
										sizeof(struct sockaddr_ll));
								socket_address3.sll_family = PF_PACKET;
								socket_address3.sll_ifindex = interface_index;
								socket_address3.sll_hatype = ARPHRD_ETHER;
								socket_address3.sll_pkttype = PACKET_OTHERHOST;
								socket_address3.sll_halen = ETH_ALEN;
								socket_address3.sll_addr[0] = neighbour_addr[0];
								socket_address3.sll_addr[1] = neighbour_addr[1];
								socket_address3.sll_addr[2] = neighbour_addr[2];
								socket_address3.sll_addr[3] = neighbour_addr[3];
								socket_address3.sll_addr[4] = neighbour_addr[4];
								socket_address3.sll_addr[5] = neighbour_addr[5];
								getifihw(&ifihead, interface_index, srchw2); //check this function
								memcpy((void*) buff3, (void*) neighbour_addr,
										ETH_ALEN);
								memcpy((void*) (buff3 + ETH_ALEN),
										(void*) srchw2, ETH_ALEN);
								eh->h_proto = htons(USID_PROTO);
								getpendingrrep(&prep, odr2->rp.srcaddr, &prep1);
								display_preptable(&prep);
								memset(&odr3, 0, sizeof(odr3));

								odr3 = buff3 + 14;
								odr3->type = htonl(1);
								odr3->rp.srcaddr.s_addr = htonl(
										prep1->srcaddr.s_addr);
								odr3->rp.dstaddr.s_addr = htonl(
										prep1->dstaddr.s_addr);
								//odr3->rp->broadcastid=odr2->rq->broadcastid;
								/*odr3->rp->broadcastid=bid2;
								 bid2++;*/
								odr3->rp.hopcnt = htonl(prep1->hopcnt);
								odr3->rp.forceddiscovery = htonl(
										prep1->forceddiscovery);
								//odr3->rp->rrepsent=htonl(prep1->rrepsent);
								/*odr3->rp->broadcastid=bid2;
								 bid2++;
								 odr3->rp->hopcnt=0;
								 odr3->rp->forceddiscovery=odr2->rq->forceddiscovery;
								 odr3->rp->rrepsent=1;*/

								send_result = sendto(sock_raw, buff3,
										ETH_FRAME_LEN, 0,
										(struct sockaddr*) &socket_address3,
										sizeof(socket_address3));
								if (send_result == -1) {
									printf("sendto() error\n");
									exit(-1);
								}
								deletependingrrep(&prep,
										NTOHLSV(odr2->rp.srcaddr));
								display_preptable(&prep);

								free(buff3);
							}
							//addprorep(&proreq,odr2->rp->srcaddr,odr2->rp->broadcastid,odr2->rp->hopcnt);
						} else
							free(buff1);

					} else {
						/*	ispro1=isrepprocessed(&prorep,odr2->rp->srcaddr,odr2->rp->broadcastid);
						 if(ispro1==1)
						 {
						 tablehopcnt=gethopcountrep(&prorep,odr2->rp->srcaddr,odr2->rp->broadcastid);
						 if(tablehocount<=odr2->rp->hopcnt)
						 hopflag2=1;
						 else
						 hopflag2=0;
						 }*/
						odr2->rp.hopcnt = htonl(ntohl(odr2->rp.hopcnt) + 1);
						tablehopcnt = gethopcount(&rhead,
								NTOHLSV(odr2->rp.srcaddr));
						if (tablehopcnt != 0) {
							tt = gettime(&rhead, NTOHLSV(odr2->rp.srcaddr));
							ct = getcurrenttime();
							td = ct - tt;
							if (td >= staleness)
								st = 1;
							else
								st = 0;
						}

						if ((tablehopcnt <= ntohl(odr2->rp.hopcnt))
								|| (st == 1)) {
							hopflag2 = 1;
							deleteentry(&rhead, NTOHLSV(odr2->rp.srcaddr));
							addroute(&rhead, neighbour_addr,
									NTOHLSV(odr2->rp.srcaddr),
									ntohl(odr2->rp.hopcnt), interface_index,
									getcurrenttime());
							display_rtable(&rhead);
						} else
							hopflag2 = 0;

						/*if((ispro1==0)||(hopflag2==1)||(odr2->rp->forceddiscovery==1))
						 {
						 if(ispro1==0)//&&(odr->rp->forceddiscovery==0))
						 addroute(&rhead,neighbour_addr,odr2->rp->srcaddr,odr2->rp->hopcnt,interface_index,time(NULL));
						 if((hopflag2==1)||(odr2->rp->forceddiscovery==1))
						 {
						 if(ispro1==1)
						 deleteentry(&rhead,odr2->rp->srcaddr);
						 addroute(&rhead,neighbour_addr,odr2->rp->srcaddr,odr2->rp->hopcnt,interface_index,time(NULL));
						 }*/
						if (hopflag2 == 1) {
							getroute(&rhead, NTOHLSV(odr2->rp.dstaddr), nexthp1,
									&indx1);
							memset(buff2, 0, ETH_FRAME_LEN);
							etherhead = buff2;
							eh = (struct ethhdr *) etherhead;
							memset(&socket_address3, 0,
									sizeof(struct sockaddr_ll));
							socket_address3.sll_family = PF_PACKET;
							socket_address3.sll_ifindex = indx1;
							socket_address3.sll_hatype = ARPHRD_ETHER;
							socket_address3.sll_pkttype = PACKET_OTHERHOST;
							socket_address3.sll_halen = ETH_ALEN;
							socket_address3.sll_addr[0] = nexthp1[0];
							socket_address3.sll_addr[1] = nexthp1[1];
							socket_address3.sll_addr[2] = nexthp1[2];
							socket_address3.sll_addr[3] = nexthp1[3];
							socket_address3.sll_addr[4] = nexthp1[4];
							socket_address3.sll_addr[5] = nexthp1[5];
							//getifihw(&ifihead,interface_index,srchw1);    //check this function
							memcpy((void*) buff2, (void*) nexthp1, ETH_ALEN);
							getifihw(&ifihead, indx1, srchw4);
							memcpy((void*) (buff2 + ETH_ALEN), (void*) srchw4,
									ETH_ALEN);
							eh->h_proto = htons(USID_PROTO);
							odr3 = buff2 + 14;
							odr3->type = htonl(1);
							odr3->rp.srcaddr = odr2->rp.srcaddr;
							odr3->rp.dstaddr = odr2->rp.dstaddr;
							//odr3->rp->broadcastid=bid1;
							//bid1++;
							odr3->rp.hopcnt = odr2->rp.hopcnt;
							odr3->rp.forceddiscovery = odr2->rp.forceddiscovery;
							//odr3->rp->rrepsent=odr2->rp->rrepsent;

							send_result = sendto(sock_raw, buff2, ETH_FRAME_LEN,
									0, (struct sockaddr*) &socket_address3,
									sizeof(socket_address3));
							if (send_result == -1) {
								printf("sendto() error\n");
								exit(-1);
							}

							free(buff2);
							//addproreq(&proreq,odr2->rq->srcaddr,odr2->rq->broadcastid,odr2->rq->hopcnt);
						} else
							free(buff1);
					}
				} else {
					odr2->rp.hopcnt = htonl(ntohl(odr2->rp.hopcnt) + 1);
					tablehopcnt = gethopcount(&rhead,
							NTOHLSV(odr2->rp.srcaddr));
					if (tablehopcnt != 0) {
						tt = gettime(&rhead, NTOHLSV(odr2->rp.srcaddr));
						ct = getcurrenttime();
						td = ct - tt;
						if (td >= staleness)
							st = 1;
						else
							st = 0;
					}

					if ((tablehopcnt <= ntohl(odr2->rp.hopcnt))
							|| (ntohl(odr2->rp.forceddiscovery) == 1)
							|| (st == 1)) {
						hopflag2 = 1;
						deleteentry(&rhead, NTOHLSV(odr2->rp.srcaddr));
						addroute(&rhead, neighbour_addr,
								NTOHLSV(odr2->rp.srcaddr),
								ntohl(odr2->rp.hopcnt), interface_index,
								getcurrenttime());
						display_rtable(&rhead);
					} else
						hopflag2 = 0;
					if ((stale2 == 1) || (ntohl(odr2->rp.forceddiscovery) == 1)) //should it also be forceddiscovery==1
							{
						deleteentry(&rhead, NTOHLSV(odr2->rp.dstaddr));
					}
					if (hopflag2 == 1) {

						ntoh1 = ntohl(odr2->rp.dstaddr.s_addr);
						ntoh2 = ntohl(odr2->rp.srcaddr.s_addr);
						/*memcpy(&(prep->dstaddr.s_addr),&(ntohl(odr2->rp->dstaddr.s_addr)),sizeof(long));
						 prep->hopcnt=ntohl(odr2->rp->hopcnt);
						 //prep->rrepsent=ntohl(odr2->rp->rrepsent);
						 prep->forceddiscovery=ntohl(odr2->rp->forceddiscovery);
						 memcpy(&(prep->srcaddr.s_addr),&(ntohl(odr2->rp->srcaddr.s_addr)),sizeof(odr2->rp->srcaddr.s_addr)); */ //check this
						//insertrrep(&prep,NTOHLSV(prep->srcaddr),NTOHLSV(prep->dstaddr),ntohl(prep->hopcnt),ntohl(prep->forceddiscovery));
						insertrrep(&prep, NTOHLSV(odr2->rp.srcaddr),
								NTOHLSV(odr2->rp.dstaddr),
								ntohl(odr2->rp.hopcnt),
								ntohl(odr2->rp.forceddiscovery));
						display_preptable(&prep);
						memset(buff4, 0, ETH_FRAME_LEN);
						for (ifi = ifihead; ifi != NULL ; ifi = ifi->next) {

							if (ifi->indx == interface_index)
								continue;
							etherhead = buff4;
							eh = (struct ethhdr *) etherhead;
							memset(&socket_address3, 0,
									sizeof(struct sockaddr_ll));
							socket_address3.sll_family = PF_PACKET;
							socket_address3.sll_ifindex = ifi->indx;
							socket_address3.sll_hatype = ARPHRD_ETHER;
							socket_address3.sll_pkttype = PACKET_BROADCAST;
							socket_address3.sll_halen = ETH_ALEN;
							socket_address3.sll_addr[0] = broadcast_mac[0];
							socket_address3.sll_addr[1] = broadcast_mac[1];
							socket_address3.sll_addr[2] = broadcast_mac[2];
							socket_address3.sll_addr[3] = broadcast_mac[3];
							socket_address3.sll_addr[4] = broadcast_mac[4];
							socket_address3.sll_addr[5] = broadcast_mac[5];

							memcpy((void*) buff4, (void*) broadcast_mac,
									ETH_ALEN);
							memcpy((void*) (buff4 + ETH_ALEN),
									(void*) ifi->hwaddr, ETH_ALEN);
							eh->h_proto = htons(USID_PROTO);
							odr3 = buff4 + 14;
							odr3->type = htonl(0);
							//odr3->rq->srcaddr=odr2->rq->srcaddr;
							odr3->rq.srcaddr.s_addr = htonl(myaddr.s_addr);
							odr3->rq.dstaddr = odr2->rp.dstaddr;
							//odr3->rq->broadcastid=odr2->rq->broadcastid;
							odr3->rq.broadcastid = htonl(bid);
							//bid++;
							odr3->rq.hopcnt = odr2->rp.hopcnt;
							odr3->rq.rrepsent = htonl(0);
							/*if(discovery==1)
							 odr3->rq->forceddiscovery=1;
							 else*/
							odr3->rq.forceddiscovery = htonl(0);
							send_result = sendto(sock_raw, buff4, ETH_FRAME_LEN,
									0, (struct sockaddr*) &socket_address3,
									sizeof(socket_address3));
							if (send_result == -1) {
								printf("sendto() error\n");
								exit(-1);
							}

							free(buff4);
							//addproreq(&proreq,odr2->rq->srcaddr,odr2->rq->broadcastid,odr2->rq->hopcnt);
						}		bid++;					//close for
					} else
						free(buff1);
				}
			}							//close odr type 1
			else if (ntohl(odr2->type) == 2) {
				printf("Payload received from interface %d,neighbour: [",
						interface_index);
				for (i = 0; i < 6; i++)
					printf("%x ", neighbour_addr[i]);

				printf("]\n Protocol: %x:\n",
						ntohs(socket_address1.sll_protocol));
				route2 = checkroute(&rhead, NTOHLSV(odr2->pl.destaddr));
				if (route2 == 1) {
					tabletime3 = gettime(&rhead, NTOHLSV(odr2->pl.destaddr));
				}
				currenttime3 = getcurrenttime();
				timedif3 = currenttime3 - tabletime3;
				if (timedif3 >= staleness)
					stale3 = 1;
				else
					stale3 = 0;
				if ((ntohl(odr2->pl.destaddr.s_addr) == myaddr.s_addr)
						|| ((route2 == 1) && (stale2 == 0)))//&&(odr2->rp->forceddiscovery==0)))
						{
					if (ntohl(odr2->pl.destaddr.s_addr) == myaddr.s_addr) {

						/*ispro1=isrepprocessed(&prorep,odr2->rp->srcaddr,odr2->rp->broadcastid);
						 if(ispro1==1)
						 {*/
						//tablehopcnt=gethopcountrep(&prorep,odr2->rp->srcaddr,odr2->rp->broadcastid);
						odr2->pl.hopcnt = htonl(ntohl(odr2->pl.hopcnt) + 1);
						tablehopcnt = gethopcount(&rhead,
								NTOHLSV(odr2->pl.srcaddr));
						if (tablehopcnt != 0) {
							tt = gettime(&rhead, NTOHLSV(odr2->pl.srcaddr));
							ct = getcurrenttime();
							td = ct - tt;
							if (td >= staleness)
								st = 1;
							else
								st = 0;
						}

						if ((tablehopcnt <= ntohl(odr2->pl.hopcnt))
								|| (st == 1))//||(odr2->rp->forceddiscovery==1))
								{
							hopflag2 = 1;
							deleteentry(&rhead, NTOHLSV(odr2->pl.srcaddr));
							addroute(&rhead, neighbour_addr,
									NTOHLSV(odr2->pl.srcaddr),
									ntohl(odr2->pl.hopcnt), interface_index,
									getcurrenttime());
							display_rtable(&rhead);
						} else
							hopflag2 = 0;
						memset(&app1, 0, sizeof(struct sockaddr_un));
						app1.sun_family = AF_LOCAL;
						memset(buff5, 0, sizeof(buff5));
						memset(rcvmsg1, 0, sizeof(rcvmsg1));
						inet_ntop(AF_INET, &(odr2->pl.srcaddr),
								rcvmsg1->dstaddr, sizeof(rcvmsg1->dstaddr));

						deletestale(&porthead, timetolive);
						display_port(&porthead);
						isport = checkport(&porthead, odr2->pl.srcport);

						if (isport == 0) {
							printf("The client is dead\n");
						} else if (isport == 1) {
							paths = getpath(&porthead, odr2->pl.srcport);
							memcpy(&(app.sun_path), paths, sizeof(paths));
							//reintialize(&porthead,app.sun_path);
						}
						memcpy(rcvmsg1->msg, (odr2->pl.msg),
								strlen(odr2->pl.msg) + 1);
						//memcpy(rcvmsg1->dstaddr,ntohl(odr2->pl->srcaddr),sizeof(long));
						rcvmsg1->dstport = ntohl(odr2->pl.srcport);
						/*if(sprintf(buff,"%s%d%s%d\n",rcvmsg->dstaddr,ntohl(odr2->pl->srcport),odr2->pl->msg)!=3)

						 {
						 printf("Error printing to the unix domain socket\n");
						 continue;
						 }*/
						sendto(unxsock, rcvmsg1, MAXLINE, 0,
								(struct sockaddr *) &app1, sizeof(app1));
					} else {
						odr2->pl.hopcnt = htonl(ntohl(odr2->pl.hopcnt) + 1);
						tablehopcnt = gethopcount(&rhead,
								NTOHLSV(odr2->pl.srcaddr));
						if (tablehopcnt != 0) {
							tt = gettime(&rhead, NTOHLSV(odr2->pl.srcaddr));
							ct = getcurrenttime();
							td = ct - tt;
							if (td >= staleness)
								st = 1;
							else
								st = 0;
						}

						if (tablehopcnt <= ntohl(odr2->pl.hopcnt)) {
							//hopflag2=1;
							deleteentry(&rhead, NTOHLSV(odr2->pl.srcaddr));
							addroute(&rhead, neighbour_addr,
									NTOHLSV(odr2->pl.srcaddr),
									ntohl(odr2->pl.hopcnt), interface_index,
									getcurrenttime());
							display_rtable(&rhead);
						}
						/*else
						 hopflag2=0;*/
						getroute(&rhead, NTOHLSV(odr2->pl.destaddr), nexthp2,
								&indx2);
						memset(buff5, 0, ETH_FRAME_LEN);
						etherhead = buff5;
						eh = (struct ethhdr *) etherhead;
						memset(&socket_address3, 0, sizeof(struct sockaddr_ll));
						socket_address3.sll_family = PF_PACKET;
						socket_address3.sll_ifindex = indx2;
						socket_address3.sll_hatype = ARPHRD_ETHER;
						socket_address3.sll_pkttype = PACKET_OTHERHOST;
						socket_address3.sll_halen = ETH_ALEN;
						socket_address3.sll_addr[0] = nexthp2[0];
						socket_address3.sll_addr[1] = nexthp2[1];
						socket_address3.sll_addr[2] = nexthp2[2];
						socket_address3.sll_addr[3] = nexthp2[3];
						socket_address3.sll_addr[4] = nexthp2[4];
						socket_address3.sll_addr[5] = nexthp2[5];
						//getifihw(&ifihead,interface_index,srchw1);    //check this function
						memcpy((void*) buff5, (void*) nexthp2, ETH_ALEN);
						getifihw(&ifihead, indx2, srchw4);
						memcpy((void*) (buff5 + ETH_ALEN), (void*) srchw4,
								ETH_ALEN);
						eh->h_proto = htons(USID_PROTO);
						odr3 = buff5 + 14;
						odr3->type = htonl(2);
						odr3->pl.srcaddr = odr2->pl.srcaddr;
						odr3->pl.destaddr = odr2->pl.destaddr;
						//odr3->rp->broadcastid=bid1;
						//bid1++;
						odr3->pl.hopcnt = odr2->pl.hopcnt;
						odr3->pl.srcport = odr2->pl.srcport;
						odr3->pl.destport = odr2->pl.destport;
						odr3->pl.bytes = odr2->pl.bytes;
						//odr3->pl->msg=odr2->pl->msg;
						memcpy((odr3->pl.msg), (odr2->pl.msg),
								strlen(odr2->pl.msg) + 1);  //check this

						send_result = sendto(sock_raw, buff5, ETH_FRAME_LEN, 0,
								(struct sockaddr*) &socket_address3,
								sizeof(socket_address3));
						if (send_result == -1) {
							printf("sendto() error\n");
							exit(-1);
						}

						free(buff5);

					}

				} else {
					odr2->pl.hopcnt = htonl(ntohl(odr2->pl.hopcnt) + 1);
					tablehopcnt = gethopcount(&rhead,
							NTOHLSV(odr2->pl.srcaddr));
					if (tablehopcnt != 0) {
						tt = gettime(&rhead, NTOHLSV(odr2->pl.srcaddr));
						ct = getcurrenttime();
						td = ct - tt;
						if (td >= staleness)
							st = 1;
						else
							st = 0;
					}

					if (tablehopcnt <= ntohl(odr2->pl.hopcnt)) //||(ntohl(odr2->rp->forceddiscovery)==1))
							{
						hopflag2 = 1;
						deleteentry(&rhead, NTOHLSV(odr2->pl.srcaddr));
						addroute(&rhead, neighbour_addr,
								NTOHLSV(odr2->pl.srcaddr),
								ntohl(odr2->pl.hopcnt), interface_index,
								getcurrenttime());
						display_rtable(&rhead);
					} else
						hopflag2 = 0;
					if (stale2 == 1) //||(ntohl(odr->rp->forceddiscovery)==1))  //should it also be forceddiscovery==1
							{
						deleteentry(&rhead, NTOHLSV(odr2->pl.destaddr));
					}
					//if(hopflag2==1)
					ntoh3 = ntohl(odr2->pl.destaddr.s_addr);
					ntoh4 = ntohl(odr2->pl.srcaddr.s_addr);
					/*memcpy(&(pdata->destaddr.s_addr),&(ntohl(odr2->pl->destaddr.s_addr)),sizeof(long));
					 pdata->hopcnt=ntohl(odr2->pl->hopcnt);
					 pdata->srcport=ntohl(odr2->pl->srcport);
					 pdata->destport=ntohl(odr2->pl->destport);
					 memcpy(&(pdata->srcaddr.s_addr),&(ntohl(odr2->pl->srcaddr.s_addr)),sizeof(long));
					 pdata->bytes=ntohl(odr2->pl->bytes);
					 memcpy((pdata->msg),(odr2->pl->msg),strlen(odr2->pl->msg)+1);*/
					insertdata(&pdata, NTOHLSV(odr2->pl.destaddr),
							ntohl(odr2->pl.destport), ntohl(odr2->pl.hopcnt),
							odr2->pl.msg, ntohl(odr2->pl.bytes),
							NTOHLSV(odr2->pl.srcaddr), ntohl(odr2->pl.srcport));
					//insertdata(&pdata,pdata->destaddr,pdata->destport,pdata->hopcnt,pdata->msg,pdata->bytes,pdata->srcaddr,pdata->srcport);
					display_ptable(&pdata);
					memset(buff4, 0, ETH_FRAME_LEN);
					for (ifi = ifihead; ifi != NULL ; ifi = ifi->next) {

						if (ifi->indx == interface_index)
							continue;
						etherhead = buff4;
						eh = (struct ethhdr *) etherhead;
						memset(&socket_address3, 0, sizeof(struct sockaddr_ll));
						socket_address3.sll_family = PF_PACKET;
						socket_address3.sll_ifindex = ifi->indx;
						socket_address3.sll_hatype = ARPHRD_ETHER;
						socket_address3.sll_pkttype = PACKET_BROADCAST;
						socket_address3.sll_halen = ETH_ALEN;
						socket_address3.sll_addr[0] = broadcast_mac[0];
						socket_address3.sll_addr[1] = broadcast_mac[1];
						socket_address3.sll_addr[2] = broadcast_mac[2];
						socket_address3.sll_addr[3] = broadcast_mac[3];
						socket_address3.sll_addr[4] = broadcast_mac[4];
						socket_address3.sll_addr[5] = broadcast_mac[5];

						memcpy((void*) buff4, (void*) broadcast_mac, ETH_ALEN);
						memcpy((void*) (buff4 + ETH_ALEN), (void*) ifi->hwaddr,
								ETH_ALEN);
						eh->h_proto = htons(USID_PROTO);
						odr3 = buff4 + 14;
						odr3->type = htonl(0);
						//odr3->rq->srcaddr=odr2->rq->srcaddr;
						odr3->rq.srcaddr.s_addr = htonl(myaddr.s_addr);
						odr3->rq.dstaddr = odr2->pl.destaddr;
						//odr3->rq->broadcastid=odr2->rq->broadcastid;
						odr3->rq.broadcastid = htonl(bid);
						//bid++;
						odr3->rq.hopcnt = odr2->pl.hopcnt;
						odr3->rq.rrepsent = htonl(0);
						/*if(discovery==1)
						 odr3->rq->forceddiscovery=1;
						 else*/
						odr3->rq.forceddiscovery = htonl(0);
						send_result = sendto(sock_raw, buff4, ETH_FRAME_LEN, 0,
								(struct sockaddr*) &socket_address3,
								sizeof(socket_address3));
						if (send_result == -1) {
							printf("sendto() error\n");
							exit(-1);
						}

						free(buff4);
						//addproreq(&proreq,odr2->rq->srcaddr,odr2->rq->broadcastid,odr2->rq->hopcnt);
					}bid++; //close for
				}
			} //close type=2

		} //close isset sock_raw
	}   //close for
}   //close main

/*//etherhead = buffer;

 //data = buffer + 14;


 odr1=buffer+14;
 if(odr1->type==0)
 req=buffer+18;
 else if (odr1->type==1)
 rep=buffer+18;
 else if (odr1->type==2)
 pyl=buffer+18;

 eh = (struct ethhdr *) etherhead;


 //src_mac[6]={0x00,0x50,0x56,0x00,0x80,0x01};
 src_mac[0] = 0x00;
 src_mac[1] = 0x50;
 src_mac[2] = 0x56;
 src_mac[3] = 0x00;
 src_mac[4] = 0x80;
 src_mac[5] = 0x01;

 //dest_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
 dest_mac[0] = 0xff;
 dest_mac[1] = 0xff;
 dest_mac[2] = 0xff;
 dest_mac[3] = 0xff;
 dest_mac[4] = 0xff;
 dest_mac[5] = 0xff;

 s = socket(PF_PACKET, SOCK_RAW, htons(USID_PROTO));
 if (s == -1) {
 printf("socket() error\n");
 exit(-1);
 }

 target address
 //struct sockaddr_ll socket_address;
 buffer for ethernet frame
 //void* buffer = (void*)malloc(ETH_FRAME_LEN);
 pointer to ethenet header
 //unsigned char* etherhead = buffer;
 userdata in ethernet frame
 //unsigned char* data = buffer + 14;
 //struct ethhdr *eh=(struct ethhdr*)etherhead;
 //int send_result = 0;
 our MAC address
 //00:50:56:00:80:01
 //unsigned char src_mac[6] =  {0x00,0x50,0x56,0x00,0x80,0x01};
 other host MAC address
 //unsigned char dest_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
 //unsigned char dest_mac[6] = {0x08,0x9e,0x01,0x15,0x4e,0x86};
 prepare sockaddr_ll

 RAW communication
 socket_address.sll_family = PF_PACKET;
 we don't use a protocoll above ethernet layer
 ->just use anything here
 //socket_address.sll_protocol = htons(USID_PROTO);
 index of the network device
 see full code later how to retrieve it
 socket_address.sll_ifindex = 2;

 ARP hardware identifier is ethernet
 .sll_hatype = ARPHRD_ETHER;

 target is another host
 .sll_pkttype = PACKET_OTHERHOST;

 address length
 .sll_halen = ETH_ALEN;
 MAC - begin
 .sll_addr[0] = dest_mac[0];
 socket_address.sll_addr[1] = dest_mac[1];
 socket_address.sll_addr[2] = dest_mac[2];
 socket_address.sll_addr[3] = dest_mac[3];
 socket_address.sll_addr[4] = dest_mac[4];
 socket_address.sll_addr[5] = dest_mac[5];
 MAC - end
 socket_address.sll_addr[6] = 0x00;not used
 socket_address.sll_addr[7] = 0x00;not used

 set the frame header
 memcpy((void*) buffer, (void*) dest_mac, ETH_ALEN);
 memcpy((void*) (buffer + ETH_ALEN), (void*) src_mac, ETH_ALEN);
 void fillrreq()
 eh->h_proto = htons(USID_PROTO);

 fill the frame with some data

 for (j = 0; j < 1500; j++) {
 data[j] = (unsigned char) ('a');
 }

 send the packet
 send_result = sendto(s, buffer, ETH_FRAME_LEN, 0,
 (struct sockaddr*) &socket_address, sizeof());
 if (send_result == -1) {
 printf("sendto() error\n");
 exit(-1);
 }

 printf("message sent\n");

 }*/

/*
 * void disply(struct ifiinfo **disp_head)
 * void display_port(struct ports **dp_phead)
 * void display_rtable(struct routingtable **drt_phead)
 void display_ptable(struct pendingdata **pdrt_phead)
 void display_rreqtable(struct rreqslist **dpsrreq_phead)
 void display_preptable(struct pendingrrep **prrep_phead)

 */
