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
//#include<unp.h>
//#include"hw_addrs.h"
//#include"fields.h"
//#include<stdbool.h>


//checked
void addifiinfo(struct ifiinfo **ifi_head,int ifi_indx,struct sockaddr *ifi_ipaddr, char ifi_hwaddr[])
{

	struct ifiinfo *temp,*r,*temp1;
	r=NULL;
	temp=*ifi_head;
	temp1=*ifi_head;

	if(temp==NULL)
	{
		temp=(struct ifiinfo *)malloc(sizeof(struct ifiinfo));
		bzero(temp, sizeof(struct ifiinfo));
		memcpy(&(temp->hwaddr),ifi_hwaddr,6);
		//strncpy(temp->hwaddr,ifi_hwaddr,6);
		temp->indx=ifi_indx;
		temp->ipaddr=*ifi_ipaddr;
		temp->next=NULL;
		*ifi_head=temp;
	}

	else
	{
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
		r=(struct ifiinfo*)malloc(sizeof(struct ifiinfo));
		bzero(r,sizeof(struct ifiinfo));
		memcpy(&(r->hwaddr),ifi_hwaddr,6);
		//strcpy(r->hwaddr,ifi_hwaddr);
		r->indx=ifi_indx;
		r->ipaddr=*ifi_ipaddr;

		while(temp1!=NULL)
		{
			if(temp1->indx==r->indx)
			return;
			temp1=temp1->next;
		}
		temp->next=r;
		r->next=NULL;
	}
}

//checked
void disply(struct ifiinfo **disp_head)
{
	struct ifiinfo *temp,*r;
	r=NULL;
	temp=*disp_head;
	while(temp!=NULL)
	{
		printf("the structure consists of \n");
		printf("index= %d \n",(temp->indx));
		printf("hwaddr= %s \n",temp->hwaddr);
		printf("ipaddr= %s  \n",temp->ipaddr.sa_data);
		temp=temp->next;
	}
}

//checked
void display_port(struct ports **dp_phead)
{
	struct ports *temp,*r;
	r=NULL;
	temp=*dp_phead;
	while(temp!=NULL)
	{
		printf("the addport consists of \n");
		printf("pathname: %s\n",temp->pathname);
		printf("portno %d \n",temp->portno);
		printf("time %ld \n",temp->timestamp);
		printf("wellknown %d\n",temp->wellknown);
		temp=temp->next;
	}
}

//checked
void addport(struct ports **ports_head,char ports_pathname[104],int ports_portno,unsigned long ports_timestamp,int ports_wellknown)
{

	struct ports *temp,*r;
	r=NULL;
	temp=*ports_head;

	if(temp==NULL)
	{
		temp=(struct ports *)malloc(sizeof(struct ports));
		bzero(temp,sizeof(temp));
		memcpy(&(temp->pathname),ports_pathname,104);
		//strncpy(temp->pathname,ports_pathname,104);
		temp->portno=ports_portno;
		temp->timestamp=ports_timestamp;
		temp->wellknown=ports_wellknown;
		temp->next=NULL;
		*ports_head=temp;
	}


	else
	{
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
		r=(struct ports*)malloc(sizeof(struct ports));
		bzero(r,sizeof(struct ports));
		memcpy(&(r->pathname),ports_pathname,104);
		//strncpy(temp->pathname,ports_pathname,104);
		r->timestamp=ports_timestamp;
		r->portno=ports_portno;
		r->wellknown=ports_wellknown;
		temp->next=r;
		r->next=NULL;
	}
}


int checkport(struct ports **ckport_head,int ckport_portno)
{
	struct ports *temp,*r;
	temp=*ckport_head;

	while(temp!=NULL)
	{
		if(temp->portno==ckport_portno)
		{
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}

//checked
void display_rtable(struct routingtable **drt_phead)
{
	struct routingtable *temp,*r;
	r=NULL;
	temp=*drt_phead;
	while(temp!=NULL)
	{
		printf("the routingtable consists of \n");
		printf("nexthop: %s\n",temp->nexthop);
		printf("dest %ud \n",temp->dest.s_addr);
		printf("hopcount %d \n",temp->hopcnt);
		printf("index: %d \n",temp->ifiindex);
		printf("time : %ld \n",temp->timestamp);
		temp=temp->next;
	}
}

//checked
void addroute(struct routingtable **rtable_head,unsigned char rtable_nexthop[6],struct in_addr rtable_dest,int rtable_hopcnt,int rtable_ifiindex,unsigned long rtable_timestamp)
{
	struct routingtable *temp,*r;
	temp=*rtable_head;
	r=NULL;

	if(temp==NULL)
	{
		temp=(struct routingtable *)malloc(sizeof(struct routingtable));
		bzero(temp,sizeof(struct routingtable));
		temp->dest=rtable_dest;
		temp->hopcnt=rtable_hopcnt;
		temp->ifiindex=rtable_ifiindex;
		memcpy(&(temp->nexthop),rtable_nexthop,6);
		//strcpy(temp->nexthop,rtable_nexthop);
		temp->timestamp=rtable_timestamp;
		temp->next=NULL;
		*rtable_head=temp;
	}

	else
	{
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
		r=(struct routingtable *)malloc(sizeof(struct routingtable));
		bzero(r,sizeof(struct routingtable));
		r->dest=rtable_dest;
		r->hopcnt=rtable_hopcnt;
		r->ifiindex=rtable_ifiindex;
		memcpy(&(r->nexthop),rtable_nexthop,6);
		//strcpy(temp->nexthop,rtable_nexthop);
		r->timestamp=rtable_timestamp;
		r->next=NULL;
		temp->next=r;
	}
}

//checked
int checkroute(struct routingtable **chtable_head,struct in_addr chtable_dest)
{
	struct routingtable *temp,*r;
	temp=*chtable_head;

		while(temp!=NULL)
		{
			//printf("the value of incoming dest is %ld\n",chtable_dest);
			//printf("the value of inside dest is %s\n",inet_ntoa(temp->dest));
			//printf("the value of incoming dest is %s\n",inet_ntoa(chtable_dest));

			//if(strcmp((char *)inet_ntoa(temp->dest),(char *)inet_ntoa(chtable_dest))==0)
			if(memcmp(&(temp->dest),&chtable_dest,sizeof(struct in_addr))==0)
			{
				//printf("matching entries \n");
				//printf("the value of inside dest is %s\n",inet_ntoa(temp->dest));
				//printf("the value of incoming dest is %s\n",inet_ntoa(chtable_dest));
				return 1;
			}
				temp=temp->next;
		}
		return 0;
}

//checked
unsigned long gettime(struct routingtable **ti_head,struct in_addr ti_dest)
{
	struct routingtable *temp,*r;
		temp=*ti_head;
		unsigned long ts;

			while(temp!=NULL)
			{
				//if(strcmp((char *)inet_ntoa(temp->dest),(char *)inet_ntoa(ti_dest))==0)
				if(memcmp(&(temp->dest),&ti_dest,sizeof(struct in_addr))==0)
				{
					ts=temp->timestamp;
					return ts;
				}
				temp=temp->next;
			}
			return 0;
}

//checked
void deleteentry(struct routingtable **de_head,struct in_addr de_dest)
{
		struct routingtable *temp,*r,*q,*temp1;
			temp=*de_head;
			temp1=*de_head;
			r=NULL;
			q=NULL;

			while(temp!=NULL)
			{
				temp1=*de_head;
				//if(strcmp((char *)inet_ntoa(temp->dest),(char *)inet_ntoa(de_dest))==0)
				if(memcmp(&(temp->dest),&de_dest,sizeof(struct in_addr))==0)
				{
					if(temp==*de_head)
					{
						*de_head=temp->next;
						free(temp);
						return;
					}
					else
					{
						while(temp!=NULL)
						{
							if(temp1->next==temp)
								break;
							temp1=temp1->next;
						}

						r=temp->next;
						free(temp);
						temp1->next=r;
						return;
					}
				}
				temp=temp->next;
			}

}

//checked
void display_ptable(struct pendingdata **pdrt_phead)
{
	struct pendingdata *temp,*r;
	r=NULL;
	temp=*pdrt_phead;
	while(temp!=NULL)
	{
		printf("the pendingdata table consists of \n");
		printf("dest %u \n",temp->destaddr);
		printf("destport %d \n",temp->destport);
		printf("hopcount %d \n",temp->hopcnt);
		printf("msg: %s\n",temp->msg);
		printf("bytes %d\n",temp->bytes);
		printf("src addr : %u \n",temp->srcaddr);
		printf("src port : %d \n",temp->srcport);
		temp=temp->next;
	}
}

//checked
void insertdata(struct pendingdata **pdata_head,struct in_addr pdata_destaddr,int pdata_destport,int pdata_hopcnt,char pdata_msg[maxpayload-(2*sizeof(struct in_addr))-16],int pdata_bytes,struct in_addr pdata_srcaddr,int pdata_srcport)
{
	struct pendingdata *temp,*r;
		temp=*pdata_head;
		r=NULL;
		if(temp==NULL)
		{
			temp=(struct pendingdata *)malloc(sizeof(struct pendingdata));
			bzero(temp,sizeof(struct pendingdata));
			temp->srcaddr=pdata_srcaddr;
			temp->srcport=pdata_srcport;
			temp->destaddr=pdata_destaddr;
			temp->destport=pdata_destport;
			temp->hopcnt=pdata_hopcnt;
			temp->bytes=pdata_bytes;
			memcpy(&(temp->msg),pdata_msg,sizeof(maxpayload-(2*sizeof(struct in_addr))-16));
			//strcpy(temp->msg,pdata_msg);
			*pdata_head=temp;
			temp->next=NULL;

		}
			else
			{
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
				r=(struct pendingdata *)malloc(sizeof(struct pendingdata));
				bzero(r,sizeof(struct pendingdata));
				r->srcaddr=pdata_srcaddr;
				r->srcport=pdata_srcport;
				r->destaddr=pdata_destaddr;
				r->destport=pdata_destport;
				r->hopcnt=pdata_hopcnt;
				r->bytes=pdata_bytes;
				memcpy(&(r->msg),pdata_msg,sizeof(maxpayload-(2*sizeof(struct in_addr))-16));
				//strcpy(r->msg,pdata_msg);
				r->next=NULL;
				temp->next=r;
			}
}



/*
struct ifiinfo *getifiinfo(struct ifiinfo **gif_head)//doubtful
{
	struct ifiinfo *temp,*r,*q;
	temp=*gif_head;

	if(temp!=NULL)
	{
		r=temp;
		return r;
	}
	else
		return NULL;

}
*/


// ask what to do if dest addr not found ///check once again wrongly written
void getroute(struct routingtable **gtroute_head,struct in_addr gtroute_dest,char *gtroute_hopcnt,int *gtroute_ifiindex)
{
	struct routingtable *temp,*r;
	temp=*gtroute_head;

				while(temp!=NULL)
				{
					//if(strcmp((char *)inet_ntoa(temp->dest),(char *)inet_ntoa(gtroute_dest))==0)
					if(memcmp(&(temp->dest),&gtroute_dest,sizeof(struct in_addr))==0)
					{
						//*gtroute_hopcnt=temp->nexthop;//check if to do memcpy
						memcpy(gtroute_hopcnt,&(temp->nexthop),6)
						*gtroute_ifiindex=temp->ifiindex;
					}
					temp=temp->next;
				}
}

//checked
/*
void updatetimestamp(struct routingtable **updts_head,struct in_addr updts_dest)
{
	struct routingtable *temp,*r;
	temp=*updts_head;
	time_t now;
	now=time(NULL);

	while(temp!=NULL)
		{
			//if(strcmp((char *)inet_ntoa(temp->dest),(char *)inet_ntoa(updts_dest))==0)
			if(memcmp(&(temp->dest),&updts_dest,sizeof(struct in_addr))==0)
			{
				temp->timestamp=now;
			}
			temp=temp->next;
		}
}
*/

//checked
void getifihw(struct ifiinfo **gthwaddr_head,int gthwaddr_indx,unsigned char *gthwadddr_hwaddr)
{
	struct ifiinfo *temp,*r;
	temp=*gthwaddr_head;

	while(temp!=NULL)
	{
		if(temp->indx==gthwaddr_indx)
		{

//			printf("debug 1\n");
			memcpy(gthwadddr_hwaddr,&(temp->hwaddr),6);
			//strncpy(gthwadddr_hwaddr,temp->hwaddr,6);
//			printf("debug 2\n");
		}
		temp=temp->next;
	}
}




//checked
void display_rreqtable(struct rreqslist **dpsrreq_phead)
{
	struct rreqslist *temp,*r;
	r=NULL;
	temp=*dpsrreq_phead;
	while(temp!=NULL)
	{
		printf("the rreqslist table consists of \n");
		printf("src %u \n",temp->src);
		//printf("destport %d \n",temp->destport);
		printf("hopcount %d \n",temp->hopcnt);
		//printf("msg: %s\n",temp->msg);
		//printf("bytes %d\n",temp->bytes);
		//printf("src addr : %u \n",temp->srcaddr);
		printf("src brodcast id : %d \n",temp->broadcastid);
		temp=temp->next;
	}
}




//checked
void addproreq(struct rreqslist **adproreq_head,struct in_addr adproreq_src,int adproreq_broadcastid,int adproreq_hopcnt)
{
	struct rreqslist *temp,*r;
	temp=*adproreq_head;
	r=NULL;

	if(temp==NULL)
	{
		temp=(struct rreqslist *)malloc(sizeof(struct rreqslist));
		bzero(temp,sizeof(struct rreqslist));
		temp->src=adproreq_src;
		temp->hopcnt=adproreq_hopcnt;
		temp->broadcastid=adproreq_broadcastid;
		*adproreq_head=temp;
		temp->next=NULL;
	}

	else
	{
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}

		r=(struct rreqslist *)malloc(sizeof(struct rreqslist));
		bzero(r,sizeof(struct rreqslist));
		r->src=adproreq_src;
		r->hopcnt=adproreq_hopcnt;
		r->broadcastid=adproreq_broadcastid;
		r->next=NULL;
		temp->next=r;

	}
}

void assport(int *count1)//did not test
{
	*count1=*count1+1;
}


//checked //change to return value //changed
int getport(struct ports **gtport_head,char gtport_pathname[104])
{
	struct ports *temp,*q;
	temp=*gtport_head;
	q=NULL;

	while(temp!=NULL)
	{
		if(strcmp(temp->pathname,gtport_pathname)==0)
		{
			return temp->portno;
		}
		temp=temp->next;
	}
	return 0;
}


//checked
int isprocessed(struct rreqslist **ispro_head,struct in_addr ispro_src,int ispro_broadcastid)
{
	struct rreqslist *temp,*r;
	temp=*ispro_head;
	r=NULL;

	while(temp!=NULL)
	{
		//if(strcmp((char *)inet_ntoa(temp->src),(char *)inet_ntoa(ispro_src))==0 && temp->broadcastid==ispro_broadcastid)
		if(memcmp(&(temp->src),&ispro_src,sizeof(struct in_addr))==0 && temp->broadcastid==ispro_broadcastid)
		{
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}

//checked
int gethopcount(struct routingtable **gthop_head,struct in_addr gthop_dest)
{
	struct routingtable *temp,*r;
	temp=*gthop_head;
	r=NULL;

	while(temp!=NULL)

	{
				//if(strcmp((char *)inet_ntoa(temp->dest),(char *)inet_ntoa(gthop_dest))==0)
				if(memcmp(&(temp->dest),&gthop_dest,sizeof(struct in_addr))==0)
				{
					return temp->hopcnt;
				}
				temp=temp->next;
	}
	return 0;
}

//checked
int checkpath(struct ports **ck_head,char ck_pathname[104])
{
	struct ports *temp,*r;
	temp=*ck_head;
	r=NULL;

	while(temp!=NULL)
	{
		if(strcmp(temp->pathname,ck_pathname)==0)
			return 1;
		temp=temp->next;
	}
	return 0;
}

//checked
/*void reinitialize(struct ports **in_head,char in_pathname[104])
{
	struct ports *temp,*r;
	temp=*in_head;
	r=NULL;
	time_t t;
	t=time(NULL);

	while(temp!=NULL)
	{
		if(strcmp(temp->pathname,in_pathname)==0)
		{
			temp->timestamp=t;
		}
		temp=temp->next;
	}
}*/

//checked //check again change return valeu //changes done
char *getpath(struct ports **gtpath_head,int gtpath_portno)
{
	struct ports *temp,*r;
	temp=*gtpath_head;
	r=NULL;

	while(temp!=NULL)
	{
		if(temp->portno==gtpath_portno)
		{
			//memcpy(gtpath_pathname,&(temp->pathname),104);
			return temp->pathname;
			//strcpy(gtpath_pathname,temp->pathname);
		}
		temp=temp->next;
	}
	return NULL;
}

//checked
void deletestale(struct ports **dpath_head,unsigned long dpath_timestamp)
{
	struct ports *temp,*r,*temp1;
	temp=*dpath_head;

	r=NULL;
	unsigned long t;
	t=getcurrenttime();

	while(temp!=NULL)
	{
		temp1=*dpath_head;

		if(temp->wellknown==1)
		{
			temp=temp->next;
			continue;
		}
		else
		{
			if((t-temp->timestamp)>=dpath_timestamp)
			{
					if(temp==*dpath_head)
					{
						*dpath_head=temp->next;
						free(temp);
						temp=*dpath_head;
						//break;
						//continue;
					}
					else
					{
							while(temp!=NULL)
							{
								if(temp1->next==temp)
								break;
								temp1=temp1->next;
							}
							r=temp->next;
							free(temp);
							temp1->next=r;
							temp=*dpath_head;
					}

			}
			temp=temp->next;
			continue;
		}
	}
}


/*
int isrepprocessed(struct rrepslist **isproc_head,struct in_addr isproc_src,int isproc_broadcastid)
{
	struct rrepslist *temp,*r;
	temp=*isproc_head;
	r=NULL;

	while(temp!=NULL)
	{
		//if(strcmp((char *)inet_ntoa(temp->src),(char *)inet_ntoa(isproc_src))==0 && temp->broadcastid==isproc_broadcastid)
		if(memcmp(&(temp->src),&isproc_src,sizeof(struct in_addr))==0 && temp->broadcastid==isproc_broadcastid)
		{
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}
*/



/*int gethopcountrep(struct rrepslist **gthop1_head,struct in_addr gthop1_src,int gthop1_broadcastid)
{
	struct rrepslist *temp,*r;
	temp=*gthop1_head;
	r=NULL;

	while(temp!=NULL)

	{
		if(strcmp(inet_ntoa(temp->src),inet_ntoa(gthop1_src))==0 && temp->broadcastid==gthop1_broadcastid)
				{
					return temp->hopcnt;
				}
				temp=temp->next;
	}
			return 0;
}*/



//checked
int isdatapending(struct pendingdata **isdpd_head,struct in_addr isdpd_dest)//check about functionality
{
	struct pendingdata *temp,*r;
	temp=*isdpd_head;
	r=NULL;

	while(temp!=NULL)
	{
		if(memcmp(&(temp->destaddr),&isdpd_dest,sizeof(struct in_addr))==0)
			return 1;
		temp=temp->next;
	}
	return 0;
}

//checked
void getpendingdata(struct pendingdata **gtpd_head,struct in_addr gtpd_dest,struct pendingdata **gtpd_node)
{
	struct pendingdata *temp,*r;
	temp=*gtpd_head;
	r=NULL;
	while(temp!=NULL)
	{
		if(memcmp(&(temp->destaddr),&gtpd_dest,sizeof(struct in_addr))==0)
		{
			*gtpd_node=temp;
			return;
		}
		temp=temp->next;
	}
	*gtpd_node=NULL;
}


//checked
void deletependingdata(struct pendingdata **delpd_head,struct in_addr delpd_dest)
{
	struct pendingdata *temp,*r,*temp1;
	temp=*delpd_head;
	r=NULL;
	temp1=*delpd_head;

	while(temp!=NULL)
	{
		temp1=*delpd_head;

		if(memcmp(&(temp->destaddr),&delpd_dest,sizeof(struct in_addr))==0)
		{
			if(temp==*delpd_head)
			{
				*delpd_head=temp->next;
				free(temp);
				temp=*delpd_head;
				return;
			}
			else
			{
				while(temp!=NULL)
				{
					if(temp1->next==temp)
						break;
					temp1=temp1->next;
				}
				r=temp->next;
				free(temp);
				temp1->next=r;
				return;
			}
		}
		temp=temp->next;
	}
}




void display_preptable(struct pendingrrep **prrep_phead)
{
	struct pendingrrep *temp,*r;
	r=NULL;
	temp=*prrep_phead;
	while(temp!=NULL)
	{
		printf("the rreqslist table consists of \n");
		printf("src %u \n",temp->srcaddr);
		printf("dest %u \n",temp->dstaddr);
		printf("hopcount %d \n",temp->hopcnt);
		//printf("msg: %s\n",temp->msg);
		//printf("rrepsent %d\n",temp->rrepsent);
		//printf("src addr : %u \n",temp->srcaddr);
		printf("forceddiscovery is : %d \n",temp->forceddiscovery);
		temp=temp->next;
	}
}




//checked
void insertrrep(struct pendingrrep **inrrep_head,struct in_addr inrrep_src,struct in_addr inrrep_dest,int inrrep_hopcount,int inrrep_fdisc)
{
	struct pendingrrep *temp,*q,*r;
	temp=*inrrep_head;
	r=NULL;

	if(temp==NULL)
	{
		temp=(struct pendingrrep *)malloc(sizeof(struct pendingrrep));
		bzero(temp,sizeof(struct pendingrrep));
		temp->forceddiscovery=inrrep_fdisc;
		temp->hopcnt=inrrep_hopcount;
		//temp->rrepsent=inrrep_rrepsent;
		temp->dstaddr=inrrep_dest;
		temp->srcaddr=inrrep_src;
		//memcpy(&(temp->dstaddr),inrrep_dest,sizeof(struct in_addr));
		//memcpy(&(temp->srcaddr),inrrep_src,sizeof(struct in_addr));
		temp->next=NULL;
		*inrrep_head=temp;
	}
	else
	{
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
		r=(struct pendingrrep *)malloc(sizeof(struct pendingrrep));
		bzero(r,sizeof(struct pendingrrep));
		r->forceddiscovery=inrrep_fdisc;
		r->hopcnt=inrrep_hopcount;
		//r->rrepsent=inrrep_rrepsent;
		r->dstaddr=inrrep_dest;
		r->srcaddr=inrrep_src;
		//memcpy(&(r->dstaddr),inrrep_dest,sizeof(struct in_addr));
		//memcpy(&(r->srcaddr),inrrep_src,sizeof(struct in_addr));
		r->next=NULL;
		temp->next=r;
	}

}

//checked
int isrreppending(struct pendingrrep **isrpp_head,struct in_addr isrpp_dest)
{
	struct pendingrrep *temp,*r;
	temp=*isrpp_head;
	r=NULL;

	while(temp!=NULL)
	{
		if(memcmp(&(temp->dstaddr),&isrpp_dest,sizeof(struct in_addr))==0)
		{
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}

//checked
void getpendingrrep(struct pendingrrep **gtprrep_head,struct in_addr gtprrep_dest,struct pendingrrep **gtprrep_node)
{
	struct pendingrrep *temp,*r;
	temp=*gtprrep_head;
	r=NULL;
	int minhopcount=65530;

	while(temp!=NULL)
	{
		if(memcmp(&(temp->dstaddr),&gtprrep_dest,sizeof(struct in_addr))==0)
		{
			if(minhopcount>temp->hopcnt)
			{
				minhopcount=temp->hopcnt;

				//break;
			}
			else
				temp=temp->next;
		}
		temp=temp->next;
	}

	printf("minhopcont is %d\n",minhopcount);
	temp=*gtprrep_head;

	while(temp!=NULL)
	{
		if(minhopcount==temp->hopcnt)
		{
			*gtprrep_node=temp;
			return;
		}
		temp=temp->next;
	}
	*gtprrep_node=NULL;
}


//checked
void deletependingrrep(struct pendingrrep **delprep_head,struct in_addr delprep_dest)
{
	struct pendingrrep *temp,*r,*temp1;
	temp=*delprep_head;
	temp1=*delprep_head;
	r=NULL;
	while(temp!=NULL)
	{
		temp1=*delprep_head;
		if(memcmp(&(temp->dstaddr),&delprep_dest,sizeof(struct in_addr))==0)
		{
			if(temp==*delprep_head)
			{
				*delprep_head=temp->next;
				free(temp);
				temp=*delprep_head;
			}
			else
			{
				while(temp!=NULL)
				{
					if(temp1->next==temp)
					{
						break;
					}
					temp1=temp1->next;
				}
				r=temp->next;
				free(temp);
				temp1->next=r;
				temp=*delprep_head;
			}
		}
		temp=temp->next;
	}
}


unsigned long getcurrenttime()
{
	struct timeval ti;
	gettimeofday(&ti,NULL);
	return(((ti.tv_sec*1000)+(ti.tv_usec/1000)));
}



//void insertrrep()//insert in the same order
//isrreppending(head,destaddr)//return 1 if there or 0
//getpending rrep(head,destaddr,node)//if dest addr
//satisfies then
//deletependingrrep(head,destaddr)//if match delete all nodes


