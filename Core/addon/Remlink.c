#include	"remmenu.h"

char	*link_buff;		/* receiving buffer			*/
uint	link_source;	/* message source			*/
int		byteT=3;		/* bytes per tick			*/
int		RFa,RFn,RFm;
char	*RFb;
/*------------------------------------------------------*/
void	ModemLinkDecode(int n)
		{
		command	*cp,*ccp;
		lib		*l;
		uchar	c[LINK_LEN],*p=link_buff;
		int		i,j;

		n=BitUnstuff(link_buff,n);
		if(n & 1)
			n -= 5;
		else
			n -= 4;
		j = p[n];
		j = (j<<8) + p[n+1];
		j =	Crc(j,link_source);
		for(i=0; i < n; ++i) {
 			p[i] ^= j;
			j = Crc(j,userpass[i % 8]);
			}
		i = p[n+2];
		i = (i<<8) + p[n+3];
		if(i != j){
			LinkAddrText(link_source,c);
			strcat(c," ");
			strcat(c,E_ILL_PASS);
			warn(c);
			return;
			}

		p=c;
		switch(*link_buff) {
			case TARGT_REQ:	p=link_buff;
							get_int(&i,++p);
							l=FindLib(i);
							if(l)	{
									p=c;
									*p++ = TARGT_UPDT;
									RFAck(link_source,c,1+tx_lib(&p,l));
									}
							else	{
									sprintf(c,"%c%s",NACK_MESS,W_ITEM_N_FOUND);
									RFAck(link_source,c,strlen(c)+1);
									}
							break;

			case UNIT_REQ:	p=link_buff;
							get_int(&i,++p);
							p=c;
							*p++ = UNIT_UPDT;
							RFAck(link_source,c,1+tx_coord(&p,OBP));
							break;

			case ACK_REQ:   p=link_buff;
/* cp->n = 0?	*/			p = get_int(&i,++p);
							if(i)
								cp=SearchTo();
							else
								cp=GUN;
							if(cp) {
/* gun no.	1..8	*/			p = get_int(&i,p);
/* comm. status		*/			p = get_int(&j,p);
								cp->ntx=i-1;
								if(cp->txmode[i-1] != j) {
									cp->txmode[i-1]=j;
									UpdateAmm(cp,j);
									}
								*link_buff = ACK_UPDT;
								RFAck(link_source,link_buff,n);
								Ungetch(REFRESH,0);
							}
							break;

			case METEO_REQ:	*p++ = METEO_UPDT;
							j=sizeof(char);
							for(i=0; i<8; ++i)
								j+=put_int(&p,meteo[i][0]);
							RFAck(link_source,c,j);
							break;

			case RTCM_REQ:  if(RTCMSize) {
								*p++ = RTCM_UPDT;
								j=sizeof(char);
								j+=put_int(&p,RTCMSize);
								strncpy(p,RTCMLog,RTCMSize);
								j += RTCMSize;
								*p += RTCMSize;
								RFAck(link_source,c,j);
								}
            				else{
								sprintf(c,"%c%s",NACK_MESS,W_DGPS_OFF);
								RFAck(link_source,c,strlen(c)+1);
								}
							break;

			case COMM_REQ:	cp=rx_command(link_buff,link_source);
							if(cp)
							{
								if(cp->tg != cp->to)
								{
									if(cp->to == LinkAddr(OBP) && checktype(OBP,TYPE_BT))
										EraseCommand(NULL);
									ccp=SearchFrom();
									if(!ccp)
									{
										if(!CP)
											CP=cp;
										cp->next = CP->next;
										CP->next = cp;
									}
									else
									{
/* vrine se na mesto prejsnje	*/		cp->next = ccp->next;
/* chk se prepise, ce sta enaki,*/		cp->chk=ccp->chk;
/* de GXM ne razume tega kot	*/		*ccp=*cp;
/* novo sporocilo !				*/		memfree(cp);
										cp=ccp;
									}
									cp->rxmode=M_PEND;
									j = sizeof(char);
									*p++ = COMM_UPDT;
									j += put_int(&p,(cp->n));
									i=targets_xn(OBP);
									j += put_int(&p,i);
									i=M_PEND;
									j += put_int(&p,i);
									RFAck(link_source,c,j);
									mess(M_LIST_UPDAT);
                                    SetTask(FlipMessage,5,RTC_task);
									Ungetch(GXM_MESSAGE_ID,0);
								}
								else
								{
									if(!GUN)
										GUN = cp;
									else
									{
										cp->chk=GUN->chk;
										*GUN=*cp;
										memfree(cp);
									}
									GUN->next = GUN;
									GUN->rxmode=M_PEND;
									j = sizeof(char);
									*p++ = COMM_UPDT;
									j += put_int(&p,(GUN->n));
									i=targets_xn(OBP);
									j += put_int(&p,i);
									i=M_PEND;
									j += put_int(&p,i);
									RFAck(link_source,c,j);
									mess(M_GUN_UPDAT);
                                    SetTask(FlipMessage,5,RTC_task);
									Ungetch(GXM_GUN_ID,0);
								}
							}
							break;

			case ABORT_REQ:	cp=SearchFrom();
							if(cp) {
								cp->rxmode=M_ABORT;
								j = sizeof(char);
								*p++ = COMM_UPDT;
								j += put_int(&p,(cp->n));
								i=targets_xn(OBP);
								j += put_int(&p,i);
								i=M_ABORT;
								j += put_int(&p,i);
								RFAck(link_source,c,j);
								mess(M_COMM_ABORT);
								Ungetch(REFRESH,0);
							}
							break;

			case COMM_UPDT: p = link_buff;
/* cp->n = 0?	*/			p = get_int(&i,++p);
							if(i)
								cp=SearchTo();
							else
								cp=GUN;
							if(cp) {
/* gun no.	1..8	*/			p = get_int(&i,p);
/* comm. status		*/			p = get_int(&j,p);
								cp->ntx=i-1;
								if(cp->txmode[i-1] != j)
									cp->txmode[i-1]=j;
								Ungetch(REFRESH,0);
							}
							break;

			case ACK_UPDT:	p = link_buff;
/* cp->n ???	*/			p = get_int(&i,++p);
							if(i)
								cp=SearchFrom();
							else
								cp=GUN;
							if(cp) {
/* gun no.	1..8	*/			p = get_int(&i,p);
/* comm. status		*/			p = get_int(&j,p);
								if((i == targets_xn(OBP)) && (cp->rxmode != j)) {
									cp->rxmode = j;
									if(cp->to == LinkAddr(OBP))
										UpdateAmm(cp,j);
									if(!CheckTask(CountDown,RTC_task))
										Ungetch(REFRESH,0);
								}
							}
							break;

			case UNIT_UPDT:	Lr=scan_coord(Lr);
							break;

			case TARGT_UPDT:p=link_buff;
							++p;
							freelib(Lr);
							Lr=scan_lib(&p);
							break;

			case METEO_UPDT:p=link_buff;
							++p;
							for(i=0; i<8; ++i)
								p=get_int(&meteo[i][0],p);
                            if(METEO)
                            	METEO->from = link_source;
							mess(M_METEO_UPDT);
							break;

			case RTCM_UPDT:	p=link_buff;
							p=get_int(&i,++p);
/* load RTCM message 	*/	Putch(0x10,CHGPS);
/* to SVeeSix			*/	while(i--) {
								Putch(*p,CHGPS);
								if(*p++ == 0x10)
									Putch(0x10,CHGPS);
								}
							Putch(0x10,CHGPS);
							Putch(0x03,CHGPS);
							break;

			case NACK_MESS: p=link_buff;
							strcpy(c,++p);
							err(c);
							break;

			case ACK_MESS:	break;

			default:		sprintf(c,"undef. link %02X",link_buff[0]);
							err(c);
							break;
			}
		}
/*------------------------------------------------------------------*/
void	link_scan(void)
{
	int		n;
	if(!link_buff)
		link_buff=memalloc(LINK_LEN*sizeof(char));
	n=recblk(&link_source,link_buff);
	if(n)
		ModemLinkDecode(n);
}
/*------------------------------------------------------*/
void	ReadRf (void)
{
	static	char	*p;
	static	int		in,out,i,j,n;

	if(Kbhit(CHPC))
	{
		if(!p)
		{
			p=memalloc(LINK_LEN);
			n=in=out=0;
		}
		if(!wait_count)
			n=in=out=0;

		p[in++]=Getch(CHPC);
		wait_count=50;

		if(n)
		{
			if(in-out == 2*n+16)
			{
				if(j==LinkAddr(OBP))
				{
					link_source=i;
					for(i=0; i<n; ++i)
					{
						sscanf(&p[2*i+16+out],"%02X",&j);
						link_buff[i]=j;
					}
					ModemLinkDecode(n);
					in=out=n=0;
				}
			}
		return;
		}
		while(in != out)
		{
			if(p[out]=='@')
			{
				if(in-out < 16)
					return;
				else
					if(sscanf(&p[out],"@%04X,%04X,%04X:",&i,&j,&n)==3)
						return;
			}
			++out;
			n=0;
		}
		in=out=0;
	}
}
/*------------------------------------------------------------------*/
lib		*scan_lib(char **c) {

		char	i,j,*p=*c;
		lib 	*l;
		float	xx,yy,zz;

		l=makelib();
		l->ltype=addtype(p);
		p += (LIBTYPE_LEN)*sizeof(char);
		p=get_int(&l->n,p);
		l->txt=addtxt(l,p);
		p += LIBTEXT_LEN*sizeof(char);
		p = get_char(&i,p);

		while(i) {
			add_coord(l,OBP->xc->x,OBP->xc->y,OBP->xc->z);
			p = get_float(&xx,p);
			p = get_float(&yy,p);
			p = get_float(&zz,p);
			if(checktype(l,TYPE_BT)) {
				p = get_int(&l->xc->refp,p);
				p = get_int(&l->xc->muzzle,p);
				}
			if(checktype(l,TYPE_MT)) {
				p = get_char(&l->xc->t.hour,p);
				p = get_char(&l->xc->t.min,p);
				p = get_char(&l->xc->t.sec,p);
				}
			l->xc->x = xx;
			l->xc->y = yy;
			l->xc->z = zz;
			--i;
			}
		p = get_char(&i,p);
		if(checktype(l,TYPE_BT)) {
			p = get_int(&l->refd,p);
			p = get_int(&l->gun,p);
			p = get_int(&l->powder,p);
			for(j=0; j<MAXAMM; ++j)
				p = get_int(&l->ammpcs[j],p);
			}
		for(l->xc = l->x; i; l->xc = l->xc->next, --i);
		*c=p;
		return(l);
		}
/*------------------------------------------------------------------*/
lib		*scan_coord(lib *l) {

		char	i,j,*p=link_buff;
		float	xx,yy,zz;
		int		n;

		p = get_char(&i,++p);
		p = get_float(&xx,p);
		p = get_float(&yy,p);
		p = get_float(&zz,p);

		if(!l) {
			l=makelib();
			copylib(LL,l);
		}
		if(!l->ltype) {
			l->ltype=addtype(TYPE_TG);
			l->txt=l->ltype->typtxt;
			auto_ntyp(&l);
		}
		if(!l->x)
			l->x=l->xc=add_coord(l,xx,yy,zz);
		for(j=1,l->xc=l->x; j != i; ++j)
			if(l->xc->next)
				l->xc=l->xc->next;
			else
				add_coord(l,xx,yy,zz);

		l->xc->x = xx;
		l->xc->y = yy;
		l->xc->z = zz;

		if(checktype(l,TYPE_BT)) {
			p = get_int(&l->xc->refp,p);
			p = get_int(&l->xc->muzzle,p);
			p = get_int(&l->refd,p);
			p = get_int(&l->gun,p);
			p = get_int(&l->powder,p);
			for(i=0; i<MAXAMM; ++i) {
				p = get_int(&n,p);
				l->ammpcs[i] += n;
				}
			}
		if(checktype(l,TYPE_MT)) {
			p = get_char(&l->xc->t.hour,p);
			p = get_char(&l->xc->t.min,p);
			get_char(&l->xc->t.sec,p);
			}
		return(l);
		}
/*------------------------------------------------------------------*/
lib		*rx_lib(lib *l)
		{
		int	i;
		lib *ll=checklib(Ln,l->ltype,l->n);
		if(!ll) {
			if(!Ln)
				Ln=l->next=l;
			else {
				l->next=Ln->next;
				Ln->next=l;
				}
			return(l);
			}
		else {
/* Ko je objekt prenosa OBP, preveri ce ustreza stevilo koord.;	*/
/* ce je xn > n, vrne NULL in ne spremeni pozicije				*/
			if(ll == OBP) {
				i=targets_xn(OBP);
				if(i > targets_n(l->x)) {
					freelib(l);
					return(NULL);
					}
				l->xc = l->x;
				while(--i)
					l->xc = l->xc->next;
				}
			copylib(l,ll);
			freelib(l);
			return(ll);
			}
		}
/*------------------------------------------------------------------*/
int		tx_lib(char **s,lib *ll)
		{
		char 	k,*p=*s;
		coord	*l;
		float	xx,yy,zz;
		int		i,j=0;
/*----------------------------------------------------------*/
/*		type text 2 characters								*/
		strncpy(p,ll->ltype->t,LIBTYPE_LEN);
		j += LIBTYPE_LEN;
		p += LIBTYPE_LEN;
		/*		 type designation									*/
		j += put_int(&p,ll->n);
/*		 target text 12 characters							*/
		strncpy(p,ll->txt->t,LIBTEXT_LEN);
		j += LIBTEXT_LEN;
		p += LIBTEXT_LEN;
/*----------------------------------------------------------*/
/* stevilo koordinat										*/
		k = targets_n(ll->x);
		j += put_char(&p,k);
/*----------------------------------------------------------*/
/* coordinates, sizeof float								*/
		for(l=ll->x; l; l=l->next) {
			xx = l->x;
			yy = l->y;
			zz = l->z;

			j+=put_float(&p,xx);
			j+=put_float(&p,yy);
			j+=put_float(&p,zz);

			if(checktype(ll,TYPE_MT)) {
				j+=put_char(&p,l->t.hour);
				j+=put_char(&p,l->t.min);
				j+=put_char(&p,l->t.sec);
				}

			if(checktype(ll,TYPE_BT)) {
				j+=put_int(&p,l->refp);
				j+=put_int(&p,l->muzzle);
				}
			}
/*--------------------------------------------------------*/
/* aktivna koordinata									  */
		for(l=ll->x, k=0; l != ll->xc; l=l->next)
			++k;
		j+=put_char(&p,k);
/*--------------------------------------------------------*/
		if(checktype(ll,TYPE_BT)) {
			j+=put_int(&p,ll->refd);
			j+=put_int(&p,ll->gun);
			j+=put_int(&p,ll->powder);
			for(i=0; i<MAXAMM; ++i)
				j+=put_int(&p,ll->ammpcs[i]);
			}
		*s=p;
		return(j);
		}
/*----------------------------------------------------------*/
int		tx_coord(char **s,lib *ll)
		{
		char 	k,*p=*s;
		float	xx,yy,zz;
		int		i,j=0;
/*----------------------------------------------------------*/
/* coord. number     										*/
		k=targets_xn(ll);
		j += put_char(&p,k);
/*----------------------------------------------------------*/
/* coordinates, sizeof float								*/
		xx = ll->xc->x;
		yy = ll->xc->y;
		zz = ll->xc->z;

		j+=put_float(&p,xx);
		j+=put_float(&p,yy);
		j+=put_float(&p,zz);
/*--------------------------------------------------------*/
		if(checktype(ll,TYPE_BT)) {
			j+=put_int(&p,ll->xc->refp);
			j+=put_int(&p,ll->xc->muzzle);
			j+=put_int(&p,ll->refd);
			j+=put_int(&p,ll->gun);
			j+=put_int(&p,ll->powder);
			for(i=0; i<MAXAMM;++i)
				j+=put_int(&p,ll->ammpcs[i]);
			}
		if(checktype(ll,TYPE_MT)) {
			j+=put_char(&p,ll->xc->t.hour);
			j+=put_char(&p,ll->xc->t.min);
			j+=put_char(&p,ll->xc->t.sec);
			}
/*--------------------------------------------------------*/
		*s=p;
		return(j);
		}

/*--------------------------------------------------------*/
command	*SearchFrom(void)
		{
		command *c=CP;
		int		n;
		char	*p=link_buff;

		if(!c)
			return(NULL);
		get_int(&n,++p);
		do {
			if((c->from == link_source) && (c->n == n))
				return(c);
			c=c->next;
			}	while(c != CP);
		return(NULL);
		}
/*--------------------------------------------------------*/
command	*SearchTo(void)
		{
		int		n;
		command	*c=CP;
		char	*p=link_buff;

		if(!c)
			return(NULL);
		get_int(&n,++p);
		do {
			if((c->to == link_source) && (c->n == n))
				return(c);
			c=c->next;
			}	while(c != CP);
		return(NULL);
		}
/*--------------------------------------------------------*/
/* neodvisna oddaja RTCM, klice se v rtcm.c, ob sporocilu */
/* iz ref. postaje										  */
void	SendDGPS(int a)
		{
		char	c[LINK_LEN],*p;
		int		j;
		p=c;
		j=1;
		*p++ = RTCM_UPDT;
		j+=put_int(&p,RTCMSize);
		strncpy(p,RTCMLog,RTCMSize);
		j += RTCMSize;
		p += RTCMSize;
		SendBlk(a,c,j,CTRL_F_UNM);
		}
/*--------------------------------------------------------*/
int		RFAck(int a,char *p,int n)
		{
		RFa=a;
		RFn=n;
		RFm=CTRL_F_UNM;

		while(CheckTask(SendAck,RTC_task))
			watchdog();

		if(!RFb)
			RFb=memalloc(LINK_LEN);
		do	{
			--n;
			RFb[n]=p[n];
			}	while(n);

		SetTask(SendAck,(((RFn + 10) * 8 + abs(key_delay)) * (targets_xn(OBP)-1)) / 20 + 1,PIT_task);

/*		SetTask(SendAck,2+(((targets_xn(OBP)-1)*(key_del/8 + 10 + RFn)))/byteT,PIT_task);
*/		return(null);
		}
/*----------------------------------------------------------*/
int		SendBlk(int a,char *p,int n,int m)
{
	int		i,j;
	char	c[2*LINK_LEN];

	n = ++n & ~1;
	j=rand();
	p[n]=j >> 8;
	p[n+1]=j % 256;
	j = Crc(j,LinkAddr(OBP));
	for(i=0; i < n; ++i) {
		p[i] ^= j;
		j = Crc(j,userpass[i % 8]);
		}
	p[n+2]=j >> 8;
	p[n+3]=j % 256;
	n=BitStuff(p,n+4);
	p[n]=0;
	if(key_delay > 0)
		return(sendblk(a,p,n,m));
	else
	{
		sprintf(c,"@@@%04X,%04X,%04X:",LinkAddr(OBP),a,n);
		for(i=0; i<n; ++i)
		{
			j=p[i];
			sprintf(strchr(c,0),"%02X",j & 0xff);
		}
		wait(abs(key_delay)/20);
		puts_pc(c);
		return(null);
	}
}
/*----------------------------------------------------------*/
void	SendAck(void)
{
	if(CheckTask(SendAck,PIT_task))
	{
		DeleteTask(SendAck,PIT_task);
		SetTask(SendAck,0,RTC_task);
	}
	else
	{
		DeleteTask(SendAck,RTC_task);
		SendBlk(RFa,RFb,RFn,CTRL_F_UNM);
	}
}
/*----------------------------------------------------------*/
int		BitUnstuff(uchar *c, uint n)
		{
		uint	x,i,j,k,m,f;
		uchar	p[LINK_LEN];
		f=x=i=j=k=0;
		while(i<n) {
			x |= c[i++];
			for(m=0;m<8;++m) {
				if(!f && ((x & 0x0f00) == 0x0f00)) {
					x += x & 0x00ff;
					f=4;
					}
				else{
					if(f)
						--f;
					x += x;
					++j;
					}
				if(j == 8) {
					p[k++] = x>>8;
					j = 0;
					}
				}
			}
		for(i=0;i<k;++i)
			c[i]=p[i];
		return(k);
		}
/*----------------------------------------------------------*/
int		BitStuff(uchar *c, uint n)
		{
		uint	x,i,j,k,m;
		uchar	p[LINK_LEN];
		x=i=j=k=0;
		while(i<n) {
			x |= c[i++];
			for(m=0;m<8;++m) {
				if((x & 0x0f00) == 0x0f00) {
					x += x & 0xff00;
					++j;
					if(j == 8) {
						p[k++] = x>>8;
						j = 0;
						}
					}
				x += x;
				++j;
				if(j == 8) {
					p[k++] = x>>8;
					j = 0;
					}
				}
			}
		if(j % 8)
			p[k++] = x>>j;
		for(i=0;i<k;++i)
			c[i]=p[i];
		return(k);
		}

