#include	"remmenu.h"
/*------------------------------------------------------*/
/*	Procesiranje ukazov iz kom. linije					*/
/*------------------------------------------------------*/
int			CheckScr;
char		*LinkBuff, *Linkp;
char		*AuxBuff, *Auxp;
/*------------------------------------------------------*/
void	AuxDef (void)
{
	char c;
	while(Kbhit(CHAUX))
	{
		if(!AuxBuff)
			Auxp=AuxBuff=memalloc(LINK_LEN);
		switch(c=Getch(CHAUX))
		{
		case 0x0A:
		case 0x0D:
			puts_aux("\r\n");
			*Auxp=null;
			Auxp=AuxBuff;
			if(!strcmp("?v",Auxp))
				SysMessage(CHAUX);
			puts_aux(">");
			break;
		default:
			if(Auxp != &AuxBuff[LINK_LEN-1])
				*Auxp++=c;
			else
			{
				*Auxp=c;
				Putch('\b',CHAUX);
			}
			Putch(c,CHAUX);
			break;
		}
	}
}
/*------------------------------------------------------*/
void	AuxRTCM (void)
{
	char c;
	while(Kbhit(CHAUX))
	{
		if(!AuxBuff)
			Auxp=AuxBuff=memalloc(LINK_LEN);
		switch(c=Getch(CHAUX))
		{
		case 0x0A:
		case 0x0D:
			*Auxp=null;
			Auxp=AuxBuff;
			if(RTCMDec(Auxp))
				if(dGPS)
				{
					SendDGPS(0);
					SendDGPS(LinkAddr(OBP));
				}
			break;
		default:
			if(Auxp != &AuxBuff[LINK_LEN-1])
				*Auxp++=c;
			else
				*Auxp=c;
			break;
		}
	}
}
/*------------------------------------------------------*/
void	ReadPc (void)
{
	char c;
	while(Kbhit(CHPC))
	{
		if(!LinkBuff)
			Linkp=LinkBuff=memalloc(LINK_LEN);
		switch(c=Getch(CHPC))
		{
		case 0x0A:
		case 0x0D:
			puts_pc("\r\n");
			*Linkp=null;
			Linkp=LinkBuff;
			PcLinkDecode(Linkp);
			puts_pc(">");
			break;
		case 0x7F:
			if(Linkp != LinkBuff)
			{
				--Linkp;
				if(!LPP)
					puts_pc("\b \b");
			}
			break;
		default:
			if(Linkp != &LinkBuff[LINK_LEN-1])
				*Linkp++=c;
			else
			{
				*Linkp=c;
				if(!LPP)
					Putch('\b',CHPC);
			}
			if(!LPP)
				Putch(c,CHPC);
			break;
		}
	}
}
/*------------------------------------------------------*/
int	PcLinkDecode (char *c)
{
/*
	long		i;
	rtime	t1,t2;
	char	cc[128];
	double	a,b,aa;
*/
	switch(*c)
	{
	case '=':
		LoadFromPc(++c);
		break;
	case ':':
		puts_rld(++c);
		return(eof);
	case '*':
		puts_C100(++c);
		puts_C100("\r");
		return(eof);
	case '.':
		GpsComm(++c);
		return(eof);
	case '@':
		WriteSophie(++c);
		return(eof);
	case '-':
		DeleteFromPc(++c);
		break;
	case '?':
		ListToPc(++c);
		break;
	case '#':
		SetAlarm();
		break;
#ifndef	WIN32
	case '!':
		sector_erase(SYSPAR,0);
		SavePar(SYSPAR,0);
		sector_erase(FONTS,0);
		fonts=FONTS;
		WriteGunData(Guns,&fonts);
		fonts=SaveZone(SaveFonts(fonts));
		break;
#endif
/* testiranje hitrosti
	case 'r':
		sscanf(++c,"%ld,%lf",&i,&a);
		ReadTime(&t1);
		watchdog();
		PIT_disable();
		while(i--)
			b=sqrt(a);
		PIT_enable();
		wait(5);
		ReadTime(&t2);
		sprintf(cc,">>> %lf\r\n",dtime(&t2,&t1));
		puts_pc(cc);
		beep(100);
		break;
	case 'm':
		sscanf(++c,"%ld,%lf,&lf",&i,&a,&b);
		ReadTime(&t1);
		watchdog();
		PIT_disable();
		while(i--)
			aa=a*b;
		PIT_enable();
		wait(5);
		ReadTime(&t2);
		sprintf(cc,">>> %lf\r\n",dtime(&t2,&t1));
		puts_pc(cc);
		beep(100);
		break;
	case 'p':
		sscanf(++c,"%ld,%lf,&lf",&i,&a,&b);
		ReadTime(&t1);
		watchdog();
		PIT_disable();
		while(i--)
			aa=pow(a,b);
		PIT_enable();
		wait(5);
		ReadTime(&t2);
		sprintf(cc,">>> %lf\r\n",dtime(&t2,&t1));
		puts_pc(cc);
		beep(100);
		break;
	case 'e':
		sscanf(++c,"%ld,%lf",&i,&a);
		ReadTime(&t1);
		watchdog();
		PIT_disable();
		while(i--)
			aa=exp(a);
		PIT_enable();
		wait(5);
		ReadTime(&t2);
		sprintf(cc,">>> %lf\r\n",dtime(&t2,&t1));
		puts_pc(cc);
		beep(100);
		break;
	case 's':
		sscanf(++c,"%ld,%lf",&i,&a);
		ReadTime(&t1);
		watchdog();
		PIT_disable();
		while(i--)
			aa=sin(a);
		PIT_enable();
		wait(5);
		ReadTime(&t2);
		sprintf(cc,">>> %lf\r\n",dtime(&t2,&t1));
		puts_pc(cc);
		beep(100);
		break;
	case 't':
		sscanf(++c,"%ld,%lf",&i,&a);
		ReadTime(&t1);
		watchdog();
		PIT_disable();
		while(i--)
			aa=tan(a);
		PIT_enable();
		wait(5);
		ReadTime(&t2);
		sprintf(cc,">>> %lf\r\n",dtime(&t2,&t1));
		puts_pc(cc);
		beep(100);
		break;
	case 'a':
		sscanf(++c,"%ld,%lf",&i,&a);
		ReadTime(&t1);
		watchdog();
		PIT_disable();
		while(i--)
			aa=atan(a);
		PIT_enable();
		wait(5);
		ReadTime(&t2);
		sprintf(cc,">>> %lf\r\n",dtime(&t2,&t1));
		puts_pc(cc);
		beep(100);
		break;
*/

	default:
		if(chrtx(c))
		Ungetch(chrtx(c),0);
		break;
	}
	return(null);
}
/*------------------------------------------------------*/
int			getasc(char *p)
			{
			int	i;
			if(isdigit(*p))
				i = *p++ -'0';
			else
				i = *p++ -'A' + 10;

			i=i<<4;

			if(isdigit(*p))
				i += *p - '0';
			else
				i += *p - 'A' + 10;
			return(i);
			}
/*--------------------------------------------------------*/
int			GpsComm(char *p)
			{
			char *q;

			Putch(0x10,CHGPS);
			for(q=p; *q != 0; ++q,++q) {
				Putch(getasc(q),CHGPS);
				if(getasc(q) == 0x10)
					Putch(0x10,CHGPS);
				}
			Putch(0x10,CHGPS);
			Putch(0x03,CHGPS);
			return(null);
			}
/*------------------------------------------------------*/
void		ReadLib (char *p)
			{
			lib		*l;
			char	q[64];
			int		n;

			if(sscanf(p,"%2s-%02d",q,&n)==2) {
				l=Ln;
				do {
					if((checktype(l,q)) && (l->n==n)) {
						ListLib(l);
						return;
						}
					l=l->next;
					} while(l != Ln);
				puts_pc("!NOT_FOUND\r\n");
				return;
				}
			else
				{
				puts_pc("!SYNTAX\r\n");
				return;
				}
			}
/*------------------------------------------------------*/
void	LoadHexRec(char *p,char f,char **stack)
{
	int	i,j;
	char *r,*s,q[128],form[]="_[%d]";

	form[0]=f;
	if(sscanf(p,form,&i)==1)
	{
		r=strchr(p,']');
		++r;
		s=q;
		do
		{
			while(*r == ' ')
				++r;
			switch(*r)
			{
			case '.':
				++r;
				while(*r != ' ' && sscanf(r,"%03d",&j)==1)
				{
					*s++=j;
					++r;
					++r;
					++r;
				}
				break;
			case '"':
				while(*++r != '"')
					*s++=*r;
				break;
			default:
				while(*r != ' ' && sscanf(r,"%02X",&j)==1)
				{
					*s++=j;
					++r;
					++r;
				}
				break;

			}
			*s++=' ';
		} while(*r);
		*--s=0;
		s=stack[i];
		stack[i]=memalloc(strlen(s)+strlen(q)+2);
		strcpy(stack[i],q);
		strcat(stack[i],"|");
		strcat(stack[i],s);
		Ungetch(REFRESH,0);
	}
	else
		puts_pc("!SYNTAX\r\n");
}
/*------------------------------------------------------*/
void	DeleteFromPc(char *p)
{
	int		i,j;
	zone	*z;

	switch(*p)
	{
	case 'z':
		for(z=Zones; z; Zones=z)
		{
			for(i=0; i<z->nx; ++i)
				for(j=0; j<z->ny; ++j)
					if(z->id)
						if(z->id[i*z->ny +j])
							memfree(z->id[i*z->ny +j]);
			memfree(z->id);
			z=Zones->next;
			memfree(Zones);
		}
		Zones=NULL;
		return;
	}
	puts_pc("!SYNTAX\r\n");
}
/*------------------------------------------------------*/
void		LoadFromPc(char *p)
			{
			void	InProg(void);
			int		ReadPar(char *);

			char	q[LINK_LEN],*r,*s,*ss[16];
			int		i,j,k;
			long	y,z;
			lib		*l;
			command	*c,**cp;

			switch(*p) {
				case 'd':	j=0;
							if(sscanf(++p,"%2s-%02d,%d",q,&i,&j) >= 2) {
								l=checklib(Ln,addtype(q),i);
								if(l) {
									if(!Lr)
										Lr=makelib();
									copylib(l,Lr);
									if(j)
									{
										for(Lr->xc=Lr->x; --j && Lr->xc->next;)
											Lr->xc=Lr->xc->next;
									}
									else
										Lr->x=Lr->xc=freecoord(Lr->x);
									Ungetch(REFRESH,0);
									}
								else
									puts_pc("!NOT_FOUND\r\n");
								}
                            else
                            	Lr=scanLLA(Lr,++p,null);
							break;
				case 'l':	if(sscanf(++p," %2s-%02d",q,&i)==2) {
								freelib(Lc);
								Lc=makelib();
								Lc->ltype=addtype(q);
								Lc->n=i;
								}
							else
								InsertLib(&Lc);
							break;

				case 'M':	if(sscanf(++p," %d,%04X,%04X,%s",&k,&i,&j,q)==4) {
                                if(Mscan)
/* znak, da je editor odprt ! */   	if(Mscan->c->maxopt) {
/* vrstica in obseg aktivna ? */		if(k)
											if(--k <= Mscan->c->maxopt) {
												Mscan->active = k;
        	       		                    	Refresh();
            	           		            	}
                                   		break;
                                    	}
                                r=strchr(q,',');
                                *r++=0;
                                cp=NULL;
                                if(!strcmp(q,"message"))
                                	cp=&CP;
                               	if(!strcmp(q,"gun"))
                               		cp=&GUN;
								if(!strcmp(q,"meteo"))
                                	cp=&METEO;
                                if(!cp)
                                	break;
/*......................................................................................*/
	                            if(!*cp)
                                	new_command(cp);
                                else{
	                               	c=*cp;
									do
										if(c->chk == j)
											*cp=c;
										else
											c=c->next;
									while(c != *cp);
									if((*cp)->chk != j)
										new_command(cp);
            						}
								(*cp)->chk=j;
                                c=*cp;
                                s=(char *)c;
								i = sizeof(command) - sizeof(command *) - sizeof(int);
								while(i--) {
					 				sscanf(r,"%02X",&j);
                                    *s++ = j;
									++r;++r;
									}
                                c->chk *= -1;
								if(cp == &CP)
									Ungetch(GXM_MESSAGE_ID,0);
								if(cp == &GUN)
									Ungetch(GXM_GUN_ID,0);
								}
							break;
				case 'r':   if(crest && (sscanf(++p," %ld %ld",&y,&z) == 2)) {
								add_coord(crest,0,y,z);
								crest->xc->refp=null;
								}
							else
								DeleteTask(DMRtimeout,RTC_task);
							break;

				case 'c':   i=j=0;
							if(Lc)
								Lc=scanLLA(Lc,++p,eof);
							break;
				case 'g':	if(Lc && (sscanf(++p," %s %d %d",q,&j,&k) == 3)) {
							for(i=0; Guns[i].name; ++i)
								if(!strcmp(Guns[i].name,q)) {
									Lc->gun=i;
									Lc->refd=j;
									Lc->powder=k;
									}
								}
							break;
				case 'a':	if(Lc && (sscanf(++p,"%s %d",q,&j) == 2)) {
								for(i=0; Guns[Lc->gun].ammo[i]; ++i)
									if(!strcmp(Guns[Lc->gun].ammo[i],q)) {
										Lc->ammpcs[i]=j;
										r=strchr(p,'+');
										s=strrchr(p,'+');
										if(r)
											do	Lc->ammpcs[i] += 0x1000;
													while(r++ != s);
										r=strchr(p,'-');
										s=strrchr(p,'-');
										if(r)
											do	Lc->ammpcs[i] -= 0x1000;
													while(r++ != s);
										}
								}
							break;
				case 'p':	if(sscanf(++p,"%d,%d",&i,&j) == 2)
								if(i<12 && j<4) {
									xyLCD(i,j);
									Ungetch(GXM_CURSOR_POS,0);
									}
							break;

				case 'm':	LoadHexRec(p,'m',_MenuCode);
							break;
				case 'h':	LoadHexRec(p,'h',_MenuHead);
							break;
				case 't':	if(Lc)
							{
								++p;
								++p;
								Lc->txt=addtxt(Lc,p);
							}
							else
								LoadHexRec(p,'t',_TextCode);
							break;

				case 'T':	if(sscanf(++p," %x %x",&i,&j)==2)
							{
							sprintf(q,"%04X\r\n",readTEMP(i,j));
							puts_pc(q);
							}
							break;

				case 'f':   if(ReadPar(++p))
								break;
							i=strscan(p,ss,' ',16);
							if(i>=2)
							{
								sscanf(ss[0],"%lx",&eebott);
								sscanf(ss[1],"%lx",&eetop);
								if(eebott < (long)FLASHTOP)
								{
									puts_pc("!ILL_ADDRESS\r\n");
									break;
								}
								if(eetop > (long)FLASHTOP+0x80000L)
								{
									puts_pc("!ILL_ADDRESS\r\n");
									break;
								}
								if(i==3)
								{
									ss[2][8]=0;
									sprintf(pass,"%-8s",ss[2]);
								}
								if(CheckPass(&syspass))
								{
									puts_pc("!NOT_ALLOWED\r\n");
									break;
								}
								puts_pc("\7\r\nFLASH programming !!!\r\n");
								wait(50);
								InProg();
								puts_pc("Done\r\n");
							}
							else
								puts_pc("!SYNTAX\r\n");
							break;
				case 'b':	k=sscanf(++p," %d %d",&i,&j);
							if(k)
								if(Baud(CHPC,i)) {
									if(k==2)
										Xonoff(j);
									break;
									}
							puts_pc("!SYNTAX\r\n");
							break;

				case 'z':	EnterZone(++p);
							break;

				case 'G':	DeleteTask(ReadPc,RTC_task);
							Guns=ReadGunData(0,0,0,0,NULL);
							SetTask(ReadPc,0,RTC_task);
							break;
				default:	puts_pc("!SYNTAX\r\n");
							break;
				}
			}
/*------------------------------------------------------*/
int			SetAlarm (void)
			{
			rtime	t = Time;
			int		i,j,k,n;
			char	s[8][8];

			for(i=0; i<8; ++i) {
				writeRTC(i,0);
				s[i][0] = 0;
                }
			n=sscanf(&LinkBuff[1],"%2d:%02d:%02d%s%s%s%s%s",
				&i,&j,&k,&s[0],&s[1],&s[2],&s[3],&s[4]);
			if(n>=3) {
					if(n==3) {
						t.hour=i;
						t.min=j;
						t.sec=k;
						AlarmEnable(&t);
						puts_pc("Wakeup on\r\n");
						}
					if(n>3)  {
						writeRTC(0,i);
						writeRTC(1,j);
						writeRTC(2,k);
						n -=3;
						for(i=0; n--; ++i)
							writeRTC(i+3,chrtx(s[i]));
						puts_pc("Submit on\r\n");
						}
					}
			else	{
					AlarmDisable();
					puts_pc("Wakeup off\r\n");
					}
            shutdown_save(NULL);
			return(null);
			}
/*------------------------------------------------------*/
void		Submit(void)
			{
static int	i;
			if(!shutdown_load(NULL))
				if(readRTC(0) || readRTC(1) || readRTC(2)) {
					if(!CheckTask(Submit,RTC_task)) {
						SetTask(Submit,1,RTC_task);
						i=3;
						}
					else{
						Ungetch(readRTC(i),10);
						if(++i > 7)
							DeleteTask(Submit,RTC_task);
						}
					}
			}
/*------------------------------------------------------*/
void		ListToPc (char *s)
			{
			int		i;
			lib		*l;
			command *c;

			uchar	r[LINK_LEN];

			switch(*s) {
				case 'l':	l=Ln;
							do	{
								l->check=null;
								l=l->next;
								}	while(l != Ln);
							c=CP;
                            if(c)
								do	{
									c->chk=null;
									c=c->next;
									}	while(c != CP);
							SetTask(CheckScreen,0,RTC_task);
							SetTask(CheckLib,0,RTC_task);
							break;

				case 's':   CheckScr=null;
							if(!CheckTask(CheckScreen,RTC_task))
								CheckScreen();
							else
								SetTask(CheckScreen,0,RTC_task);
							break;

				case 'o':	sprintf(r,"=o %2s-%02d,%d\r\n",OBP->ltype->t,OBP->n,targets_xn(OBP));
							puts_pc(r);
							break;

				case 'p':	puts_pc("=p ");
								putsLLA(OBP);
                                puts_pc("\r\n");
							break;

				case 'm':   for(i=0; _MenuCode[i]; ++i)
							{
								sprintf(r,"=m[%d] ",i);
								for(s=_MenuCode[i]; *s; ++s)
									if(*s == ' ')
										sprintf(strchr(r,0)," ");
									else
									{
/*										if(*s < ENTER)
											sprintf(strchr(r,0),"%c",*s);
										else
*/											sprintf(strchr(r,0),"%02X",*s);
									}
									sprintf(strchr(r,0),"\r\n");
								puts_pc(r);
							}
							break;

				case 't':   for(i=0; _TextCode[i]; ++i)
							{
								sprintf(r,"=t[%d] ",i);
								for(s=_TextCode[i]; *s; ++s)
									sprintf(strchr(r,0),"%02X",*s);
								sprintf(strchr(r,0),"\r\n");
								puts_pc(r);
							}
							break;

				case 'h':   for(i=0; _MenuHead[i]; ++i)
							{
								sprintf(r,"=h[%d] ",i);
								for(s=_MenuHead[i]; *s; ++s)
									sprintf(strchr(r,0),"%02X",*s);
								sprintf(strchr(r,0),"\r\n");
								puts_pc(r);
							}
							break;

				case 'f':	ListPar();
							break;

				case 'z':	ListZone(NULL);
							break;

				case 'v':   puts_pc(">=v");
							SysMessage(CHPC);
							break;

				case 'G':	WriteGunData(Guns,NULL);
							break;

				case 'D':	ListDatum();
							break;

				default:	ReadLib(s);
							break;
				}
			}
/*------------------------------------------------------------------*/
int		chrtx(char *c)
		{
		if(!strncmp(c,"F1",2))		return(F1);
		if(!strncmp(c,"F2",2))		return(F2);
		if(!strncmp(c,"F3",2))		return(F3);
		if(!strncmp(c,"F4",2))		return(F4);
		if(!strncmp(c,"CLR",3))		return(CLEAR);
		if(!strncmp(c,"EDIT",4))	return(ENTER);
		if(!strncmp(c,"UP",2))		return(UP);
		if(!strncmp(c,"DOWN",4))	return(DOWN);
		if(!strncmp(c,"LEFT",4))	return(LEFT);
		if(!strncmp(c,"RIGHT",5))	return(RIGHT);
		if(!strncmp(c,"+/-",3))		return(SIGN);
		if(!strncmp(c,"MODE",4))	return(ALPHA);
		if(!strncmp(c,"ONOFF",5))	return(ONOFF);
		if(!strncmp(c,"BACKLIT",4))	return(BACKLIT);
		if(isalpha(*c))				return(*c);
		if(isdigit(*c))				return(*c);
		return(null);
		}
/*------------------------------------------------------------------*/
/*
void	LoadSrec()
		{
		char	*p,pp[128];
		int		i,j,n=0;
		long	l;
		char 	*a;
		void	(*f)(void);

		do	{
			do	{
				while(!Kbhit(CHPC))
					watchdog();
				}	while(Getch(CHPC) != 'S');

			puts_pc("+");
			if(!(++n % 32))
				puts_pc("\r\n");
			p=pp;
			l=0L;
			do	{
				while(!Kbhit(CHPC))
					watchdog();
				*p = Getch(CHPC);
				}	while(*p++ != 0x0d);
			p=pp;
			switch(*p++) {
				case '0':	break;
				case '1':	i=getasc(p);
							++p;++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							a=(char *)l;
							for(j=i-4;j; --j)	{
								*a = getasc(p);
								++p; ++p; ++a;
								}
							break;

				case '2':	i=getasc(p);
							++p;++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							a=(char *)l;
							for(j=i-4;j; --j)	{
								*a = getasc(p);
								++p; ++p; ++a;
								}
							break;

				case '9':	++p; ++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							l=(l << 8) + getasc(p);
							f=(void(*)(void))l;
							if(f)
								SetTask(f,0,RTC_task);
							p=pp;
							break;

				case '8':	++p; ++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							l=(l << 8) + getasc(p); ++p; ++p;
							l=(l << 8) + getasc(p);
							f=(void(*)(void))l;
							if(f)
								SetTask(f,0,RTC_task);
							p=pp;
							break;
				}
			}	while(p != pp);
		}
*/
/*------------------------------------------------------------------*/
void	ListLib(lib *lb)
		{
		gun		*g;
		lib		*ll;
		char	*p,c[256];
		int		i,j;

		copylib(lb,ll=makelib());
		if(lb == LL)
			sprintf(c,"=d");
        else{
			if(lb == NAV)
				sprintf(c,"=n");
			else
				sprintf(c,"=l");
            }
		sprintf(strchr(c,0)," %2s-%02d,%d\r\n",ll->ltype->t,ll->n,targets_xn(ll));
		puts_pc(c);
		for(ll->xc=ll->x; ll->xc; ll->xc=ll->xc->next) {
			puts_pc("=c ");
			putsLLA(ll);
			if(checktype(ll,TYPE_BT))
				sprintf(c," %d %d\r\n",ll->xc->muzzle,ll->xc->refp);
			else
				sprintf(c,"\r\n");
			puts_pc(c);
			}
		if(ll->txt) {
			for(p=ll->txt->t; (*p==' ') && *p; ++p);
			sprintf(c,"=t %s\r\n",p);
			}
		else
			sprintf(c,"=t ...\r\n");
		puts_pc(c);
		if(Guns && checktype(ll,TYPE_BT)) {
			g=&Guns[ll->gun];
			sprintf(c,"=g %s %d %d %d %d %d\r\n",g->name,ll->refd,ll->powder,g->left,g->right,g->mils);
			puts_pc(c);
			for(i=0; g->ammo[i]; ++i) {
				j=ll->ammpcs[i];
				sprintf(c,"=a %s %d ",g->ammo[i],j & 0xfff);
				j /= 0x1000;
				if(!j)
					sprintf(strchr(c,0),"N");
				if(j < 0)
					do  sprintf(strchr(c,0),"-"); while(++j);
				if(j > 0)
					do  sprintf(strchr(c,0),"+"); while(--j);
				sprintf(strchr(c,0),"\r\n");
				puts_pc(c);
				}
			}
		if(lb == LL)
			sprintf(c,"=d");
        else{
			if(lb == NAV)
				sprintf(c,"=n");
			else
				sprintf(c,"=l");
            }
		sprintf(strchr(c,0),"\r\n>");
		puts_pc(c);
		freelib(ll);
		}
/*------------------------------------------------------*/
void	getLCD(char *p)
		{
		int	i=0;	
		while(i<80)
			*p++=LcdBuffer[i++];
		}
/*------------------------------------------------------*/
void	CheckScreen(void)
{
	int		i,j;
	char	r[128];

	j=0;
	for(i=0;i<80;++i)
		j = Crc(j,LcdBuffer[i]);
	for(i=0;i<32;++i)
		j = Crc(j,LcdExt[i]);
	j = Crc(j,GetLCDaddr());
	if(CheckScr != j)
	{
		CheckScr = j;
		sprintf(r,"=s %d,%d,%c\r\n",(int)LcdBuffer[80]%20,(int)LcdBuffer[80]/20,(int)LcdBuffer[81]);
		puts_pc(r);
		for(i=0;i<4;++i)
		{
			strncpy(r,&LcdBuffer[20*i],20);
			r[20]=0;
			puts_pc(r);
			puts_pc("\r\n");
		}
		for(i=0;i<4;++i)
		{
			strncpy(r,&LcdExt[8*i],8);
			r[8]=0;
			puts_pc(r);
			puts_pc("\r\n");
		}
		puts_pc(">");
	}
	SetTask(CheckScreen,2,RTC_task);
}
/*--------------------------------------------------------*/
void	CheckLib(void)
		{
static	lib		*ll=NULL,*nav=NULL;
		lib		*l;
		char 	r[16];

		if(!LPP)
			LPP=OBP;

		CheckItem(LPP);
        LPP = LPP->next;


		if(ll)
		{
			if(!LL)
			{
				puts_pc("-d\r\n>");
				ll=NULL;
			}
			else
			{
				if(!LL->x || edit_flag!=eof)
				{
					puts_pc("-d\r\n>");
					ll=NULL;
				}
			}
		}
		if(LL)
			if(LL->x && edit_flag==eof)
				ll=CheckItem(LL);


		if(nav)
		{
			if(!NAV)
			{
				puts_pc("-n\r\n>");
				nav=NULL;
			}
			else
			{
				if(!NAV->x)
				{
					puts_pc("-n\r\n>");
					nav=NULL;
				}
			}
		}
		if(NAV)
			if(NAV->x)
				nav=CheckItem(NAV);

		while(LD) {
			sprintf(r,"-l %2s-%02d\r\n>",LD->ltype->t,LD->n);
			puts_pc(r);
			l=LD;
			LD=LD->next;
			if(LPP==l)
				LPP=OBP;
			freelib(l);
			}

		if(!CPM)
			CPM=CP;
		ListMenu(CPM,Cedit,null);
		if(CPM)
			CPM=CPM->next;

		ListMenu(GUN,edit_gun,null);
		ListMenu(METEO,set_meteo,null);
		ListMenu(SYS,set_system,null);
		ListMenu(LINK,SetupMenu,null);
		}
/*------------------------------------------------------------------*/
int		Crc(int i, int j)
		{
		if((i & 0x8000) == ((i & 0x4000)<<1))
			return(((i<<1) + j + 1) & 0xffff);
		else
			return(((i<<1) + j) & 0xffff);
		}
/*------------------------------------------------------------------*/
void	putsUTM(lib *ll)
		{
		char	r[64];
		lib *l=makelib();
		copylib(ll,l);
		Lla2Utm(l,l,NULL);
		sprintf(r,"%.0lf %.0lf %.0lf",l->xc->x,l->xc->y,l->xc->z);
		if(ll->xc->y < 0)
			puts_pc("S");
		puts_pc(r);
		freelib(l);
		}
/*------------------------------------------------------------------*/
void	putsLLA(lib *ll)
		{
		char	r[64];
        if(coordtx)
        	putsUTM(ll);
        else {
			sprintf(r,"%.8lf %.8lf %.3lf",ll->xc->x/M_PI*180.0,
				fmod(ll->xc->y/M_PI*180.0+360.0,360.0),
					ll->xc->z);
			puts_pc(r);
        	}
		}
/*------------------------------------------------------------------*/
lib		*scanLLA(lib *l,char *s, int n)
{
	int		i,j=0;
	char	*ss[8];
	char	p[128];
	lib		*ll;

	strncpy(p,s,127);
	p[127]=0;
	i=strscan(p,ss,',',8);
	if(i<3)
	{
		strncpy(p,s,127);
		p[127]=0;
		i=strscan(p,ss,' ',8);
	}
	if(i<3)
		return(NULL);
 	if(!l)
	{
		l=makelib();
		copylib(LL,l);
	}
	if(!l->ltype)
	{
		l->ltype=addtype(TYPE_TG);
		l->txt=l->ltype->typtxt;
		auto_ntyp(&l);
	}
	if(i>3)
		j=*ss[3];
	if(n || !l->x || j=='+')
		AddCoord(l,0,0,0);

	l->xc->x=atof(ss[0]);
	l->xc->y=atof(ss[1]);
	l->xc->z=atof(ss[2]);
	if(coordtx)
		Utm2Lla(l,l);	
	else
	{
		l->xc->x *= M_PI/180.0;
		l->xc->y *= M_PI/180.0;
	}

	l->xc->t=Time;
	if(i==6)
	{
		l->xc->muzzle=atoi(ss[4]);
		l->xc->refp=atoi(ss[5]);
	}
	else
	{
		l->xc->muzzle=0;
		ll=copylib(l,ll=makelib());
		LlaToCorr(l,ll,OBP,NULL,EL_AZ_R);
		l->xc->refp = ScaleAngle(ll->xc->y + maxmils/2,maxmils,MR10,eof)+0.5;
		freelib(ll);
	}
	return(l);
	}
/*--------------------------------------------------------*/
lib		*CheckItem(lib *ll)
		{
		int		i,j;
		coord	*x;
        char	*p;

/*
		if(checktype(ll,TYPE_MT)) {
			if(targets_n(ll->x) > 1) {
				if(dtime(&LastCoord(ll)->t,&t) <= 0) {
       		    	add_coord(ll,0,0,0);
                    if(track_delay)
						addsecs(&ll->xc->t,track_delay);
                    else
						addsecs(&ll->xc->t,1);
					if(!interpolate(ll))
						erase_coord(ll,ll->xc);
                    else
						while(targets_n(ll->x) > 3)
							erase_coord(ll,FirstCoord(ll));
					}
                }
			}
*/
		p = (char *)ll;
		i = sizeof(lib) - 3 * sizeof(void *) - sizeof(int);
		j = 0;
		while(i--)
			j = Crc(j,*p++);
		for(x=ll->x; x; x=x->next) {
			p = (char *)x;
			i = sizeof(coord) - sizeof(void *);
			while(i--)
				j = Crc(j,*p++);
			}
		j = Crc(j,targets_xn(ll));
		if(ll->check != ~j) {
			ll->check = ~j;
			ListLib(ll);
			}
        return(ll);
		}
