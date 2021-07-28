#include	"remmenu.h"
/*--------------------------------------------------------------------------*/
void	amain(void)
			{
			int		i;
			uchar	*l;
			command	*PASS=NULL;
			__SystemInit();					/* system initialization		*/
/*--------------------------------------------------------------------------*/
			InitMeteo(NULL);
			for(i=0; i<4; ++i)
			{
				error_screen[i]=memalloc(21*sizeof(char));
				sprintf(error_screen[i],"%20s"," ");
			}
			clear_screen(NULL);
			SysMessageLCD();
/*--------------------------------------------------------------------------*/
/*	Loading fonts and gun data, pred dolocanjem default tipov, zaradi moznih 
	sprememb BT, OP, TG etc !
*/
#ifndef	WIN32
			if(!eerror)
			{
				Guns=ReadGunData(0,0,0,0,&fonts);
				fonts=LoadZone(LoadFonts(fonts));
				syspar=LoadPar(fonts,0);
			}
#else
			l = GetBlock(library);
			Guns=ReadGunData(0,0,0,0,&l);
			l=LoadZone(LoadFonts(LoadPar(LoadPar(l,0),1)));
#endif
/*--------------------------------------------------------------------------*/
/*... enter default target types and description text						*/
/*... observer's position													*/
			LL=makelib();
			LL->ltype=addtype(TYPE_OP);
			addtxt(LL,OP_UNDEF);

/*... gun's position														*/
			LL->ltype=addtype(TYPE_BT);
			addtxt(LL,BT_UNDEF);
			if(Guns)
				for(i=0; Guns[i].name; ++i)
					addtxt(LL,Guns[i].name);

/*... commander's position													*/
			LL->ltype=addtype(TYPE_CO);
			addtxt(LL,CO_UNDEF);
			addtxt(LL,INF_HQ);
			addtxt(LL,ART_HQ);
			addtxt(LL,ARM_HQ);

/*... target type															*/
			LL->ltype=addtype(TYPE_TG);
			addtxt(LL,TG_UNDEF);
			addtxt(LL,TG_INF);
			addtxt(LL,TG_ART);
			addtxt(LL,TG_ARM);
			addtxt(LL,OP_UNDEF);
			addtxt(LL,INF_HQ);
			addtxt(LL,ART_HQ);
			addtxt(LL,ARM_HQ);

/*... moving target															*/
			LL->ltype=addtype(TYPE_MT);
			addtxt(LL,TG_UNDEF);

/*... reference point														*/
			LL->ltype=addtype(TYPE_RP);
			addtxt(LL,RP_UNDEF);

/*... obstacle, crest														*/
			LL->ltype=addtype(TYPE_CR);
			addtxt(LL,CR_UNDEF);

/*... reset display registFr => power-on LCD empty							*/
			LL->ltype = NULL;
			LL->n = null;

/*... loading the library block OBP first, default types must be preloaded !*/ 

#ifdef	WIN32
			Ln=OBP=LoadLib(l);
#else
			if(!eerror)
			{
				l = GetBlock(library);
				Ln=OBP=LoadLib(LoadPar(l,1));
			}
#endif
			if(!Ln) {
				Ln=OBP=make_set(GPS_lat/3600.0/180.0*M_PI,
						GPS_lon/3600.0/180.0*M_PI,
							GPS_alt,TYPE_OP,OP_UNDEF);
				Ln->n=1;
				Ln->next=Ln;
				}
			else
			{
				GPS_lat=OBP->xc->x/M_PI*180.0*3600.0;
				GPS_lon=OBP->xc->y/M_PI*180.0*3600.0;
			}		
/* default mode EL_AZ_R in local
*/								  						
			ORG=OBP;
			d_mode=EL_AZ_R;

/* lcd contrast, backlight in zacetni vklop grelca
*/
			Tamb=getT();
			LcdLevel(lcd);
			if(Backlit % 2)
	       		BacklitOn();
			else
    	   		BacklitOff();
			if(Tamb < 0)
			{
				HeatOn();
				SetTask(HeatOff,-5*(int)Tamb,RTC_task);
			}
/* caka na konec loga
*/
			while(CheckTask(SysMessageEnd,PIT_task))	{
				watchdog();
				wait(1);
				}

			if(!BatteryOK())			/* initial battery check		*/
				Shutdown();
			else
				DeleteTask(Lobatt,RTC_task);

			SetTask(ScanRLD,0,RTC_task);
			SetTask(CheckRLD_off,1,RTC_task);
			SetTask(link_scan,0,RTC_task);
			if(key_delay > 0)
				SetTask(ReadPc,0,RTC_task);
			else
				SetTask(ReadRf,0,RTC_task);
			switch(aux_mode) {          /*	mode depending tasks		*/
				case AUX_OFF:	SetTask(AuxDef,0,RTC_task);
								break;
				case AUX_JW:	SetTask(JwRec,0,RTC_task);
								SetTask(IbitRes,0,RTC_task);
								break;
				case AUX_DGPS: 	SetTask(AuxRTCM,0,RTC_task);
								break;
				}
			if(GPS_CONTINOUS)
				GpsInit();
			dspinit(LinkAddr(OBP));
			SetDSPgain((void *)0);
			SetDSPgain((void *)1);
			SetDSPgain((void *)2);
			SetDSPgain((void *)3);

			WriteSophie("ARTES-1000 V2.2");
			SysMessage(CHPC);
			Putch('>',CHPC);
			beep(100);

			SetDatum();
			RTCMFixPos();
			Submit();
			
			comm_enable(&PASS);
			while(CheckPass(&userpass) &&
					CheckPass(&syspass) &&
							CheckPass(&ser_no) &&
								CheckPass(NULL))
				l_menu_hold(EnterPasswrd);
			clrlcd();
/*			Mgrs();
*/
			}
/*--------------------------------------------------------*/
uchar		*GetBlock(uchar *lib)
			{
			uchar	*l=lib,*ll;
			lib=load_byte(&l,l,sizeof(uchar *));
			while(l != (uchar *)-1) {
				ll=load_byte(&l,l,sizeof(uchar *));
				if(l != (uchar *)-1)
					lib = ll;
				}
			return(lib);
			}
/*--------------------------------------------------------*/
uchar		*PutBlock(uchar *lib)
			{
			uchar	*l;
			lib=load_byte(&l,lib,sizeof(uchar *));
			while(l != (uchar *)-1)
			{
				lib = l;
				lib=load_byte(&l,lib,sizeof(uchar *));
			}
			return(lib);
			}
/*--------------------------------------------------------*/
int			EraseSingle(void *vp)
			{
int			CheckList(command *, lib *);

			lib 	*p,*q,**s;
			char	c[32];
			ltype	*r;
			int		i;

			if(!vp)
				s=&Ln;
			p=checklib(*s,LL->ltype,LL->n);
			if(!p){
				err(W_ITEM_N_FOUND);
				return(null);
				}
			else
				*s=p;
			if(*s) {
				r=(*s)->ltype;
				i=CheckList(CP,*s);
				if(i) {
					sprintf(c,"%s %03d",E_USED_BY,i);
					err(c);
					return(null);
					}
				if(*s == OBP) {
					err(E_OBP_DATA);
					return(null);
					}
				if(*s == ORG) {
					err(E_ORG_DATA);
					return(null);
					}
				if(*s == NORTH) {
					err(E_NORTH_DATA);
					return(null);
					}
				if((*s)->next == *s) {
					(*s)->next = LD;
					LD=*s;
					*s=NULL;
					}
				else {
					for(p=*s;p->next != *s;p=p->next);
					p->next=(*s)->next;
					(*s)->next = LD;
					LD=*s;
					*s=p;
					}
				}
			if(*s) {
				q=*s;
				for(p=(*s)->next; p != *s; p=p->next)
					if(checktype(p,r->t))
						q=p;
				*s=q;
				copylib(*s,LL);
				}
			else
				{
				warn(W_LIB_EMPTY);
				LL->x=LL->xc=freecoord(LL->x);
				LL->ltype=NULL;
				LL->txt=NULL;
				}
			mess(M_OBJ_DEL);
			Displc(LL);
			return(null);
			}
/*--------------------------------------------------------*/
int			EraseAll(void *vp)
			{
int			CheckList(command *, lib *);

			lib 	*p,*q;

			if(!findtype(Ln,LL->ltype->t)) {
				err(W_ITEM_N_FOUND);
				return(null);
				}

			if(!vp)
				p=Ln;
			else
				p=vp;

			do	{
				if((LL->ltype == p->ltype) &&
				!CheckList(CP,p) &&
				(p != OBP) && (p != ORG) && (p != NORTH)) {
					if(p->next != Ln)
						EraseAll(p->next);
					for(q=p; q->next != p; q=q->next);
					q->next = p->next;
					if(Ln==p)
						Ln=q->next;
					p->next = LD;
					LD=p;
					p=q;
					}
				p=p->next;
				}	while(p != Ln);

			if(!vp) {
				p=findtype(Ln,LL->ltype->t);
				if(p)
					Ln=p;
				else
					Ln=q;
				copylib(Ln,LL);
				Displc(LL);
				}
			return(null);
			}
/*--------------------------------------------------------*/
int			CheckList(command *cc, lib *s)
			{
			command *cp;
			if(cc) {
				cp=cc;
				do	{
					if(FindLib(cp->tg) == s)
						return(cp->n);
					if(FindLib(cp->to) == s)
						return(cp->n);
					if(FindLib(cp->from) == s)
						return(cp->n);
					cp=cp->next;
					}	while(cp != cc);
				}
			return(null);
			}
/*--------------------------------------------------------*/
int			InsertLib(void *vp)
			{
			lib	**l=vp,*q;

			if(!l)
				l=&LL;

			if(!*l || !(*l)->ltype)
				{
				err(E_VOID_TYPE);
				return(null);
				}
			if(!(*l)->xc) {
				err(E_VOID_DATA);
				return(null);
				}

			q=checklib(Ln,(*l)->ltype,(*l)->n);
/* copy from *l if item exists already					  */
			if(q){
				copylib(*l,q);
/* no messages, if called with parameter				  */
				if(vp)
					return(null);
/* check items otherwise								  */
				if(q == OBP) {
					warn(W_OBP_REP);
					}
				else
					if(q == ORG)
						warn(W_ORG_REP);
					else
						if(q == NORTH)
							warn(W_NORTH_REP);
						else
							mess(M_OBJ_REP);
				}
/* insert item on Ln                                      */
			else{
				q=makelib();
				if(!q)
					return(null);
				copylib(*l,q);
				if(Ln) {
					q->next=Ln->next;
					Ln->next=q;
					Ln=Ln->next;
					}
				else{
					Ln=q;
					q->next=Ln;
					}
				if(!vp)
					mess(M_OBJ_INS);
				}
			return(null);
			}
/*--------------------------------------------------------*/
int			SaveEE(void *vp)
			{
			clrlcd();
			freelib(LL);
			LL=makelib();
			shutdown_save(NULL);
			if(!CheckLibrary(vp))
				return(null);
			SaveLibrary(vp);
			mess(M_SAVE_LIB);
			return(null);
			}
/*--------------------------------------------------------*/
int			SaveLibrary(void *vp)
			{
			uchar	*l,*ll;
/*--------------------------------------------------------*/
/* sector =		32k		( 29F010 ) >> old flash			  */
/*         		128k	( 29F040 )						  */
/*														  */
			eerror = 0;
			l = ll = PutBlock(library);
#ifdef	WIN32
			WriteGunData(Guns,&ll);
			ll=SaveLib(OBP,SaveZone(SaveFonts(SavePar(SavePar(ll,0),1))));
#else
			ll=SaveLib(OBP,SavePar(l,1));
#endif
			--(uchar **)l;
			prog_byte(&ll,l,sizeof(uchar **));
			if(eerror)
			{								/* first try ???  */
				sector_erase(LIBRARY,0);
				return(SaveLibrary(NULL));
			}			            		/* first try ???	*/
#ifndef	WIN32
			SavePar(SYSPAR,0);
			if(eerror & ~0x80)
			{
				sector_erase(SYSPAR,0);
				return(SaveLibrary(NULL));
			}
#endif
			clrlcd();
			freelib(LL);
			LL=makelib();

			DspAddr(LinkAddr(OBP));
			SetDatum();
			RTCMFixPos();
			shutdown_save(NULL);
			DeleteTask(power_off,RTC_task);

			return(null);
			}
/*--------------------------------------------------------*/
int			CheckLibrary(void *vp)
			{
			uchar	*l,*ll;
/*--------------------------------------------------------*/
/* sector =		32k		( 29F010 ) >> old flash			  */
/*         		128k	( 29F040 )						  */
/*														  */
			eerror = 0x80;			/* blokada vpisa	  */
			l = ll = GetBlock(library);
#ifdef	WIN32
			WriteGunData(Guns,&ll);
			ll=SaveLib(OBP,SaveZone(SaveFonts(SavePar(SavePar(ll,0),1))));
#else
			ll=SaveLib(OBP,SavePar(l,1));
			SavePar(SYSPAR,0);
#endif
			--(uchar **)l;
			prog_byte(&ll,l,sizeof(uchar **));
			return(eerror ^= 0x80);
			}
/*--------------------------------------------------------*/
int			LoadEE(void *vp)
			{
			int		Org=eof,North=eof;
			lib		*p;
			char	c[16];
			uchar	*l;

			shutdown_load(NULL);
/*--------------------------------------------------------*/
/* if ORG & NORTH pointers in main library => recover	  */
			if(ORG)
				if(ORG == checklib(Ln,ORG->ltype,ORG->n))
					Org=LinkAddr(ORG);
			if(NORTH)
				if(NORTH == checklib(Ln,NORTH->ltype,NORTH->n))
					North=LinkAddr(NORTH);

			if(LPP) {
				p = Ln;
				do	{
					sprintf(c,"-l %2s-%02d\r\n>",p->ltype->t,p->n);
					puts_pc(c);
					p = p->next;
					} while(p != Ln);
				LPP = NULL;
				}
			Ln=freelibrary(Ln);
			l = GetBlock(library);
#ifdef	WIN32
			Guns=ReadGunData(0,0,0,0,&l);
			Ln=OBP=LoadLib(LoadZone(LoadFonts(LoadPar(LoadPar(l,0),1))));
#else
			syspar=LoadPar(SYSPAR,0);
			Ln=OBP=LoadLib(LoadPar(l,1));
#endif
			if(!Ln) {
				Ln=OBP=make_set(GPS_lat/3600.0/180.0*M_PI,
						GPS_lon/3600.0/180.0*M_PI,
							GPS_alt,TYPE_OP,OP_UNDEF);
				Ln->n=1;
				Ln->next=Ln;
				}
			else
			{
				GPS_lat=OBP->xc->x/M_PI*180.0*3600.0;
				GPS_lon=OBP->xc->y/M_PI*180.0*3600.0;
			}


			if(Org != eof)
				ORG		=FindLib(Org);
			if(North != eof)
				NORTH	=FindLib(North);

/*** always recover user password !!! ***/
			strcpy(pass,userpass);

			clrlcd();
			freelib(LL);
			LL=makelib();

			DspAddr(LinkAddr(OBP));
			LcdLevel(lcd);
			SetDatum();
			RTCMFixPos();
			DeleteTask(power_off,RTC_task);

			return(null);
			}
/*--------------------------------------------------------*/
int			NextList(void *vp)
			{
			lib	**ref,*p;

			if(!vp)
				ref=&Ln;
			else
				ref=vp;
			if(!*ref) {
				warn(W_LIB_EMPTY);
				return(null);
				}
			for(p=(*ref)->next; p != (*ref); p=p->next) {
				if(checktype(p,(*ref)->ltype->t)) {
					if(p==OBP)
						mess(M_OBP_DATA);
					(*ref)=p;
					if(vp) {
						copylib((*ref),LL);
						Displc(LL);
						}
					return(null);
					}
				}
			if(vp) {
				copylib((*ref),LL);
				Displc(LL);
				}
			warn(W_SINGLE_ITEM);
			return(null);
			}
/*--------------------------------------------------------*/
int			NextType(void *vp)
			{
			ltype	*p;
			lib		**ref,*q,*r;

			if(!vp)
				ref=&Ln;
			else
				ref=vp;

			if(!*ref) {
				warn(W_LIB_EMPTY);
				return(null);
				}
			if((*ref)->next == *ref) {
				warn(W_SINGLE_ITEM);
				return(null);
				}

			p=(*ref)->ltype->next;
			do	{
				if(!p)
					p=LTYPES;
				q=(*ref)->next;
				do	{
					if(checktype(q,p->t)) {
						r=checklib(*ref,q->ltype,(*ref)->n);
						if(r)
							q=r;
						(*ref)=q;
						if(vp) {
							copylib((*ref),LL);
							Displc(LL);
                            }
						if(q==OBP)
							mess(M_OBP_DATA);
						return(null);
						}
					q=q->next;
					}	while(q != (*ref));
				p=p->next;
				}	while(1);
			}
/*--------------------------------------------------------*/
int			LastList(void *vp)
			{
			lib **ref,*p,*q;

			if(!vp)
				ref=&Ln;
			else
				ref=vp;
			if(!*ref)	{
				warn(W_LIB_EMPTY);
				return(null);
				}
			for(p=(*ref)->next, q=NULL; p && (p!=(*ref)); p=p->next)
				if(checktype(p,(*ref)->ltype->t))
					q=p;
			if(!q)	{
				if(vp) {
					copylib((*ref),LL);
					Displc(LL);
					}
				warn(W_SINGLE_ITEM);
				}
			else {
				if(q==OBP)
					mess(M_OBP_DATA);
				(*ref)=q;
				if(vp) {
					copylib((*ref),LL);
					Displc(LL);
					}
				}
			return(null);
			}
/*--------------------------------------------------------*/
int			LastType(void *vp)
			{
			ltype	*p,*r;
			lib		**ref,*q,*s;

			if(!vp)
				ref=&Ln;
			else
				ref=vp;
			if(!*ref) {
				warn(W_LIB_EMPTY);
				return(null);
				}
			if((*ref)->next == *ref) {
				warn(W_SINGLE_ITEM);
				return(null);
				}
			if((*ref)->ltype) {
				r=(*ref)->ltype;
				do	{
					for(p=LTYPES; p->next && (p->next != r); p=p->next);
					for(q=(*ref)->next; q!= (*ref); q=q->next)
						if(checktype(q,p->t)) {
							s=checklib(*ref,q->ltype,(*ref)->n);
							if(s)
								q=s;
							(*ref)=q;
							if(vp) {
								copylib((*ref),LL);
								Displc(LL);
								}
							if(q==OBP)
								mess(M_OBP_DATA);
							return(null);
							}
					r=p;
					}	while(1);
				}
			err(E_VOID_TYPE);
			return(null);
			}
/*--------------------------------------------------------*/
int			point_on(void *vp)
			{
			if(!LL || !LL->xc)
			{
				err(E_VOID_DATA);
				return(eof);
			}
			edit_flag=d_mode;
			copylib(LL,NORTH=makelib());
			LlaToCorr(LL,NORTH,OBP,NULL,EL_AZ_R);
			SetTask(DisplPoint,0,RTC_task);
			shaft_enable();
			return(null);
			}
/*--------------------------------------------------------*/
int			point_off(void *vp)
			{
			DeleteTask(DisplPoint,RTC_task);
			edit_flag=eof;
			NORTH=freelib(NORTH);
			Displc(LL);
			return(null);
			}
/*--------------------------------------------------------*/
void		DisplPoint(void)
			{
			lib		*l;
 			if(shaft_ready())
			{
				copylib(NORTH,l=makelib());
				l->xc->x = read_EL() - NORTH->xc->x;
				l->xc->y = fmod(read_AZ() - NORTH->xc->y + maxmils, maxmils);
				Display(l,EL_AZ_R,maxmils);
				SetTask(DisplPoint,0,RTC_task);
				freelib(l);
				shaft_enable();
				RefreshScreen();		/* prehitro izpisovanje */
			}
			}
/*------------------------------------------------------*/
int			RLD_enable(void *vp)	{
			rld.status &= ~RLD_OFF;
			return(null);
			}
/*------------------------------------------------------*/
int			RLD_disable(void *vp)	{
			rld.status |= RLD_OFF;
			return(null);
			}
/*------------------------------------------------------*/
void	ScanRLD(void)
		{
		char 	c[24];
		int	 	i = rld.buttons;

		rld.buttons = 0;
		if(!LL)
			return;
		if((rld.status & RLD_OFF) && i) {
			err(E_RLD_OFF);
			return;
		}
		switch(i)	{
			case FIRSTLAST_R:	if(rld_mode & 0x100)
/* postavi indikator in */			rld_mode |= 0x8000;
/* nadaljuje v trigger	*/		else{
									wait(2); /* rld jeba !!! */
									rld.N = (++rld.N) % 2;
									RLD_GetTarget(rld.N+1);
									return;
								}
			case TRIGGER_R:		if(checktype(LL,".r"))  {
									err(E_RLD_OFF);
									return;
								}
/* ref. direction entry support	*/
/* see <int edit_gun(void *)> 	*/
								if(Refresh())
								{
									freelib(Lr);
									Lr=makelib();
									Lr->refd = ScaleAngle(read_AZ(),maxmils,MR10,eof) + 0.5;
                                    Refresh();
									return;
								}
								if(checktype(LL,".b"))	{
									shaft_enable();
									if(!LL->xc)
										add_coord(LL,0,0,0);
									LL->xc->t = Time;
									LL->xc->x = read_EL();
									LL->xc->y = read_AZ();
									LL->xc->z = ELAZ_defr;
									CorrToLla(LL,LL,OBP,NULL,EL_AZ_R);
									if(edit_flag != eof) {
										LlaToCorr(LL,LL,ORG,NORTH,d_mode);
										Display(LL,d_mode,maxmils);
									}
									else
										Displc(LL);
									return;
								}

								if(!(rld.status & HV_READY)) {
									RLD_StartHV();
									err(E_RLD_N_READY);
									return;
								}
/*
	- brisi registre daljine
	- trigger
	- podaljsaj periodo za CheckRLD_on, RLD v casu meritve ne odgovarja
	  problem na spodnji temperaturi zaradi pocasnega motorja prizme
	  (date 18.6.96 !!!)
*/
								rld.N=rld.R[0]=rld.R[1]=rld.R[2]=rld.R[3]=0;
								RLD_Trigger();
								SetTask(CheckRLD_on,3,RTC_task);
								shaft_enable();
		}
		if(rld.status & RLD_TREQ)
		{
			rld.status &= ~RLD_TREQ;
			if(rld.R[rld.N] != 0)
			{
				if(!LL->ltype)
				{
					LL->ltype=addtype(TYPE_TG);
					LL->txt=LL->ltype->typtxt;
					auto_ntyp(NULL);
				}
				if(!LL->xc || (rld_mode & 0x8000))
					add_coord(LL,0,0,0);
				rld_mode &= ~0x8000;
				LL->xc->t = Time;
				LL->xc->z = rld.R[rld.N];
				LL->xc->x = read_EL();
				LL->xc->y = read_AZ();
				LL->xc->refp = ScaleAngle(LL->xc->y + maxmils/2,maxmils,MR10,eof);
/*..................................................................*/
/* priprava za Elbit, full circle je 6400							*/

				if(LL->xc->x >= 0)
					JwEL = LL->xc->x / maxmils * 6400.0 + 0.5;
				else
					JwEL = LL->xc->x / maxmils * 6400.0 - 0.5;

				if(JwEL >= 3200)
					JwEL -= 6400;
				if(JwEL < -3200)
					JwEL += 6400;

				if(LL->xc->y >= 0)
					JwAZ = LL->xc->y / maxmils * 6400.0 + 0.5;
				else
					JwAZ = LL->xc->y / maxmils * 6400.0 - 0.5;

				if(JwAZ >= 6400)
					JwAZ -= 6400;
				if(JwAZ < 0)
					JwAZ += 6400;
				JwR  = LL->xc->z;
/*..................................................................*/
				RLD_ShowTarget(rld.R[rld.N]);
				CorrToLla(LL,LL,OBP,NULL,EL_AZ_R);
				if(edit_flag != eof)
				{
					LlaToCorr(LL,LL,ORG,NORTH,d_mode);
					Display(LL,d_mode,maxmils);
				}
				else
					Displc(LL);
				AdsData();
				if(rld.status & RLD_MULTG)
				{
					rld.status &= ~RLD_MULTG;
					warn(W_RLD_MULTG);
				}
			}
			else
/*		izklop indikacije blokiranega cilja zaradi jebe s
		prvimi sestimi kosi modification kita
*/
				if(rld.status & RLD_BLOCTG)
				{
					rld.status &= ~RLD_BLOCTG;
					warn(W_RLD_BLOCTG);
				}
				else
				{
					sprintf(c,"%s %d",W_RLD_NOMULTG,rld.N+1);
					rld.N=0;
					warn(c);
				}
		}
		if(Lr && !Refresh() && !(rld.status & RLD_OFF))
		{
			if(edit_flag != eof)
				EditClose(NULL);
			copylib(Lr,LL);
			Lr=freelib(Lr);
			Displc(LL);
		}
}
/*--------------------------------------------------------*/
void		CheckRLD_on(void)
{
	if(rld.status & RLD_CHECK)
		SetTask(CheckRLD_on,1,RTC_task);
	else
	{
		DeleteTask(CheckRLD_on,RTC_task);
		mess(M_LASER_OFF);
		SetTask(CheckRLD_off,1,RTC_task);
	}
	rld.status &= ~RLD_CHECK;
	RLD_Noecho();
	RLD_Remote();
	RLD_StartHV();
	RLD_GetStatus();
/*	SophieReticle();
*/	}
/*--------------------------------------------------------*/
void		CheckRLD_off(void)
{
	if(!(rld.status & RLD_CHECK))
		SetTask(CheckRLD_off,1,RTC_task);
	else
	{
		DeleteTask(CheckRLD_off,RTC_task);
		rld.status &= ~RLD_CHECK;
		mess(M_LASER_ON);
		SetTask(CheckRLD_on,1,RTC_task);
	}
	RLD_GetStatus();
/*	SophieReticle();
*/	}
/*--------------------------------------------------------*/
int			set_EL_AZ_R(void *vp)
			{
			if(!ORG)
				ORG=OBP;
			d_mode=EL_AZ_R;
			return(null);
			}
/*--------------------------------------------------------*/
int			set_EL_AZ(void *vp)
			{
			if(!ORG)
				ORG=OBP;
			d_mode=EL_AZ;
			return(null);
			}
/*--------------------------------------------------------*/
int			set_AZ_R_dZ(void *vp)
			{
			if(!ORG)
				ORG=OBP;
			d_mode=AZ_R_dZ;
			return(null);
			}
/*--------------------------------------------------------*/
int			set_N_E_Z(void *vp)
			{
			if(!ORG)
				ORG=OBP;
			d_mode=N_E_Z;
			return(null);
			}
/*--------------------------------------------------------*/
int			set_mils6000(void *vp)
			{
			maxmils=6000;
			return(null);
			}
/*--------------------------------------------------------*/
int			set_mils6400(void *vp)
			{
			maxmils=6400;
			return(null);
			}
/*--------------------------------------------------------*/
int			bearing_local(void *vp)
			{
			lib **p=vp;
/* freelib, ce je vneseno z meritvijo					  */
			if(NORTH)
				if(NORTH != checklib(Ln,NORTH->ltype,NORTH->n))
					freelib(NORTH);
/* if bearing in main library							  */
			if(p)
				if(*p == checklib(Ln,(*p)->ltype,(*p)->n))
					NORTH=*p;
				else
					copylib(*p,NORTH=makelib());
			else
				NORTH=NULL;
			return(null);
			}
/*--------------------------------------------------------*/
int			origin_local(void *vp)
			{
			lib	**p=vp;
/* freelib, ce je vneseno z meritvijo					  */
			if(ORG)
				if(ORG != checklib(Ln,ORG->ltype,ORG->n))
					ORG=freelib(ORG);
/* if origin in main library							  */
			if(p)
				if(*p == checklib(Ln,(*p)->ltype,(*p)->n))
					ORG=*p;
				else
					copylib(*p,ORG=makelib());
			else
				ORG=NULL;
			if(!ORG)
				d_mode=N_E_Z;
			return(null);
			}
/*--------------------------------------------------------*/
int			rewrite_LCD(void *vp)
			{
			Displc(LL);
			return(null);
			}
/*------------------------------------------------------------------*/
int		EditStr(int i,char *s,int n)
{
		char	*p,q[]="+-._\x86()//////////";
static	int		alpha=1,
				alpha_n=0,
				oldkey=null;

		switch(alpha)
		{
		case -1:
			p=q;
			break;
		case 0:
			p=KEY_NUMERIC;
			break;
		case 1:
			p=KEY_ALPHA0;
			break;
		case 2:
			p=KEY_ALPHA1;
			break;
		case 3:
			p=KEY_ALPHA2;
			break;
		case 4:
			p=KEY_ALPHA3;
			break;		
		default:
			p=KEY_ALPHA0;
			alpha=1;
			break;
		}

		if(!*p)
		{
			p=KEY_ALPHA0;
			alpha=1;
		}
		
		switch(i)
		{
			case CLEAR: i=' ';
						oldkey=null;
						break;

			case ALPHA:	if(alpha)
						{
							alpha=0;
							mess(M_NUM_ON);
						}
						else
						{
							alpha=1;
							mess(M_UPP_ON);
						}
						return(EditStr(oldkey,s,n));

			case SIGN:	if(alpha)
						{
							++alpha;
							if(alpha_n)
								--alpha_n;
						}
						else
							alpha=-1;
						return(EditStr(oldkey,s,n));

			case RIGHT:	if(RightToLeft)
						{
							if(n)
								--n;
						}
						else
						{
							if(s[n+1])
								++n;
						}
						break;
						
			case LEFT:	if(RightToLeft)
						{
							if(s[n+1])
								++n;
						}
						else
						{
							if(n)
								--n;
						}
						break;
		}

		if(!isalnum(i) && !isspace(i))
		{
			oldkey=null;
			return(n);
		}

		if(oldkey != i)
			oldkey=i;

		if(isdigit(i))
		{
			i-='0';
			while(i--)
				while(*p++ != '/');
			i=p[alpha_n++];
			if(i=='/' || !i)
			{
				alpha_n=1;
				i=*p;
			}
		}

		if(RightToLeft)
			s[strlen(s)-1-n]=i;
		else
			s[n]=i;

		if(n < strlen(s)-1 && (p[1] == '/' || !p[1]))
		{
			++n;
			Ungetch(null,1);
		}
		else
		{
			if(RightToLeft)
				Ungetch(LEFT,100);
			else
				Ungetch(RIGHT,100);
		}
	return(n);
}
/*--------------------------------------------------------*/
int			SetSerial(void *vp)
			{
static		int		n=0;
			int		i;
			char	*p=vp;

			i=GetMpos();
			if(!strcmp(pass,p) || !CheckPass(NULL))
				if(Kbhit(CHLCD))
				{
					n=EditStr(Getch(CHLCD),p,n);
					if(CheckPass(NULL))
						strcpy(pass,p);
				}
			msprint(p);
			SetMpos(i+n);
			return(null);
			}
/*--------------------------------------------------------*/
int			SetPass(void *vp)
			{
static		int		n=0;
			int		i;
			char	*p=vp;

			i=GetMpos();
			if(!strcmp(pass,p) || !CheckPass(NULL))
			{
				if(Kbhit(CHLCD))
				{
					n=EditStr(Getch(CHLCD),p,n);
					if(CheckPass(NULL))
						strcpy(pass,p);
				}
				msprint(p);
				SetMpos(i+n);
			}
			else
			{
			   	for(i=0;p[i];++i)
                    if(p[i] != ' ')
                    	msprint(".");
                    else
                    	msprint(" ");
 			}
			return(null);
			}
/*--------------------------------------------------------*/
int			EnterPass(void *vp)
			{
static		int		n=0;
			int		i;

			if(!strcmp(userpass,pass) ||
					!strcmp(syspass,pass) ||
							!strcmp(ser_no,pass) ||
								!CheckPass(NULL))
									return(eof);
			i=GetMpos();
			if(Kbhit(CHLCD))
				n=EditStr(Getch(CHLCD),pass,n);
			msprint(pass);
			SetMpos(i+n);
			return(null);
			}
/*--------------------------------------------------------*/
int			CheckPass(void *vp)
			{
char		magic[]="412185  ";
			char	*p=vp;
			if(!p)
			{
				if(!strcmp(pass,magic))
					return(null);
				else
					return(1);
			}
			if(!strcmp(pass,p))
				return(null);
			return(CheckPass(NULL));
			}
/*--------------------------------------------------------*/
void		SysMessageLCD_03(void)
			{
			xyLCD(0,3);
			puts_lcd("    System");
			DeleteTask(SysMessageLCD_03,PIT_task);
			SetTask(SysMessageEnd,75,PIT_task);
			}

void		SysMessageLCD_02(void)
			{
			xyLCD(0,2);
			puts_lcd("   Engagement");
			DeleteTask(SysMessageLCD_02,PIT_task);
			SetTask(SysMessageLCD_03,10,PIT_task);
			}

void		SysMessageLCD_01(void)
			{
			xyLCD(0,1);
			puts_lcd("  Target");
			DeleteTask(SysMessageLCD_01,PIT_task);
			SetTask(SysMessageLCD_02,10,PIT_task);
			}

void	SysMessageLCD_00(void)
			{
			clrlcd();
			xyLCD(0,0);
			puts_lcd("ARtillery");
			DeleteTask(SysMessageLCD_00,PIT_task);
			SetTask(SysMessageLCD_01,10,PIT_task);
			}

void	SysMessageLCD(void)
			{
			clrlcd();
			xyLCD(0,0);
			puts_lcd("Fotona");
			Putch(symFOTONA,CHLCD);
			xyLCD(0,3);
			puts_lcd("ARTES-IOOO");
			DeleteTask(SysMessageLCD,PIT_task);
			SetTask(SysMessageLCD_00,75,PIT_task);
			SetTask(SysMessageEnd,eof,PIT_task);
			}
/*--------------------------------------------------------*/
void		SysMessageEnd(void)
			{
/*** task samo signalizira konec inic. sekvence ***/
			DeleteTask(SysMessageEnd,PIT_task);
			}
/*--------------------------------------------------------*/
void		SysMessage(int ch)
			{
			rtime	t=Time;
			char	c[32],*p;

			sprintf(c,"\r\nARTES-1000 V2.2\r\n");
			for(p=c; *p; ++p)
				Putch(*p,ch);
			sprintf(c,"ser.number:%10s\r\n",ser_no);
			for(p=c; *p; ++p)
				Putch(*p,ch);
			sprintf(c,"%2s %02d-%2s-%02d %2d:%02d:%02d\r\n",
				GetMessCode(_DAY_1+t.day),t.date+1,GetMessCode(_MONTH_1+t.month),t.year,t.hour,t.min,t.sec);
			for(p=c; *p; ++p)
				Putch(*p,ch);
			}
