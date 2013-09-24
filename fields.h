#include<stdio.h>
#include <sys/socket.h>
#define maxpayload 1500
#define protocol 5486 //last 4 digits of rollno,its hexadecimal is 0X156E

struct ifiinfo
{
	int indx;
	struct sockaddr ipaddr;
	unsigned char hwaddr[6];
	struct ifiinfo *next;
};

struct msg
{
	int sockfd;
char dstaddr[16];
int dstport;
char msg[400];
int flag;

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
struct rreq *next;
};


 struct rrep
{
	struct in_addr srcaddr;
	struct in_addr dstaddr;
	//int broadcastid;
	int hopcnt;
	//int rrepsent;
	int forceddiscovery;
	struct rrep *next;
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
 };


 struct odrmsg
{
	int type;
	struct rreq rq;
	struct rrep rp;
	struct payload pl;
	struct odrmsg *next;
};

struct ethernetheader
{
unsigned char srcaddr[6];
unsigned char destaddr[6];
int protocol1;
};




struct pendingrrep
{
	struct in_addr srcaddr;
	struct in_addr dstaddr;
	int hopcnt;
	//int rrepsent;
	int forceddiscovery;
	struct pendingrrep *next;
};



 struct pendingdata
{
	struct in_addr srcaddr;
	int srcport;
	struct in_addr destaddr;
	int destport;
	int hopcnt;
	int bytes;
	char msg[maxpayload-(2*sizeof(struct in_addr))-16];
	struct pendingdata *next;
};


 struct ports
{
	int portno;
	char pathname[104];
	unsigned long timestamp;
	int wellknown;
	struct ports *next;
};



struct routingtable
{

	unsigned char nexthop[6];   //neighbour
	struct in_addr dest;
	int hopcnt;
	int ifiindex;
	time_t timestamp;
	struct routingtable *next;
};


 struct rreqslist
{
	struct in_addr src;
	int hopcnt;
	int broadcastid;
	struct rreqslist *next;
};

struct rrepslist
{
	struct in_addr src;
	int hopcnt;
	//int broadcastid;
	struct rrepslist *next;
};



/*
void addifiinfo(struct ifiinfo **,int,struct sockaddr *, char []);
void disply(struct ifiinfo **);
void display_port(struct ports **);
void addport(struct ports **,char[104],int,unsigned long ,int);
int checkport(struct ports **ckport_head,int ckport_portno)
void display_rtable(struct routingtable **);
void addroute(struct routingtable **,unsigned char [],struct in_addr rtable_dest,int rtable_hopcnt,int rtable_ifiindex,unsigned long rtable_timestamp)
int checkroute(struct routingtable **chtable_head,struct in_addr chtable_dest)
unsigned long gettime(struct routingtable **ti_head,struct in_addr ti_dest)
void deleteentry(struct routingtable **de_head,struct in_addr de_dest)
void display_ptable(struct pendingdata **pdrt_phead)
void insertdata(struct pendingdata **pdata_head,struct in_addr pdata_destaddr,int pdata_destport,int pdata_hopcnt,char pdata_msg[maxpayload-(2*sizeof(struct in_addr))-16],int pdata_bytes,struct in_addr pdata_srcaddr,int pdata_srcport)
void getroute(struct routingtable **gtroute_head,struct in_addr gtroute_dest,int *gtroute_hopcnt,int *gtroute_ifiindex)
void getifihw(struct ifiinfo **gthwaddr_head,int gthwaddr_indx,unsigned char *gthwadddr_hwaddr)
void display_rreqtable(struct rreqslist **dpsrreq_phead)
void addproreq(struct rreqslist **adproreq_head,struct in_addr adproreq_src,int adproreq_broadcastid,int adproreq_hopcnt)
void assport(int *count1)
void getport(struct ports **gtport_head,char gtport_pathname[104],int *gtport_portno)
int isprocessed(struct rreqslist **ispro_head,struct in_addr ispro_src,int ispro_broadcastid)
int gethopcount(struct routingtable **gthop_head,struct in_addr gthop_dest)
int checkpath(struct ports **ck_head,char ck_pathname[104])
void getpath(struct ports **gtpath_head,int gtpath_portno,char *gtpath_pathname)
void deletestale(struct ports **dpath_head,unsigned long dpath_timestamp)
int isdatapending(struct pendingdata **isdpd_head,struct in_addr isdpd_dest)
void getpendingdata(struct pendingdata **gtpd_head,struct in_addr gtpd_dest,struct pendingdata **gtpd_node)
void deletependingdata(struct pendingdata **delpd_head,struct in_addr delpd_dest)
void display_preptable(struct pendingrrep **prrep_phead)
void insertrrep(struct pendingrrep **inrrep_head,struct in_addr inrrep_src,struct in_addr inrrep_dest,int inrrep_hopcount,int inrrep_fdisc)
int isrreppending(struct pendingrrep **isrpp_head,struct in_addr isrpp_dest)
void getpendingrrep(struct pendingrrep **gtprrep_head,struct in_addr gtprrep_dest,struct pendingrrep **gtprrep_node)
void deletependingrrep(struct pendingrrep **delprep_head,struct in_addr delprep_dest)
unsigned long getcurrenttime()*/
