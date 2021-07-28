#include	"remmenu.h"
/*------------------------------------------------------------------*/
int	smenu(void *vp)
{
	uchar  	i,p[128],pp[128],q[128],r,*s;

	int	  fret,k;
	menu  *m;
	curr_menu = m = vp;

	do
	{
		s=_MenuCode[m->n];
		for(i=language; i; --i)
			if(strchr(s,'|'))
			{
				s=strchr(s,'|');
				++s;
			}
		strcpy(pp,s);
		if(strchr(pp,'|'))
			*strchr(pp,'|')=0;
			
		k=GetLCDaddr();
		for(r=0;r<4;++r)
		{
			if(ScanMenu(pp,p,&r) == eof)
				continue;
			xyLCD(12,r);
			if(p[0] == '/')
				puts_lcd("        ");
			else
			{
				if(m->f)
					if(m->f[r])
						if(m->f[r][0] == CheckImpl)
							if(CheckImpl(m->arg[r][0]))
							{
								puts_lcd("        ");
								continue;
							}

				sprintf(q,"%8s",p);
				puts_lcd(q);
			}
		}
		if(!m->f || !m->arg)
			break;
		SetLCDaddr(k);
		r=Getch(CHLCD);
		switch(r)
		{
		case F1:
		case F2:
		case F3:
		case F4:
			r -= F1;
			for(i=0; i<4; ++i)
			{
				xyLCD(12,i);
				puts_lcd("        ");
			}
			SetLCDaddr(k);
			fret=menu_exe(m,r,NULL);
			break;
#ifdef	USER_KEYS
		default:
			if(r < USR_END)
#else							
		default:	if(r < MAXFKEY)
#endif
		fret=menu_test(r,pp,r,m);
			else
		fret=menu_test(r,pp,MAXFKEY,m);
			break;
		}
	}	while(!fret);
	if(fret==eof)
		return(fret);
	else
		return(fret-1);
}
/*------------------------------------------------------------------*/
int		s_menu(void *vp)
		{
		return(smenu((menu *)vp));
		}
/*------------------------------------------------------------------*/
int		s_menu_hold(void *vp)
		{
		smenu((menu *)vp);
		return(null);
		}
/*------------------------------------------------------------------*/
int		comm_enable(void *vp) {
		command **p = vp;
		if(!*p)
			new_command(p);
		if(!Mscan)
			Mscan=memalloc(sizeof(mscan));
		Mscan->c = *p;
		lmenu_key=eof;
		Mscan->level=eof;
		return(null);
		}
/*------------------------------------------------------------------*/
int		l_menu(void *vp)
		{
		return(lmenu((menu *)vp));
		}
/*------------------------------------------------------------------*/
int		l_menu_hold(void *vp)
		{
		int		i,d_off=0;
		menu	*m = vp;
		if(!Mscan->c)	{
				err(E_VOID_LIST);
				return(1);
				}
		SetTask(RefreshMenu,1,RTC_task);
		if(Kbhit(CHLCD))
			Getch(CHLCD);

		do	{
			if(Mscan->active < 0)
				Mscan->active = 0;
			Mscan->level = eof;
			if(lmenu(m)) {
				DeleteTask(RefreshMenu,RTC_task);
				clrlcd();
				Cursor(CURS_OFF);
				return(null);
				}
			if(Mscan->active > Mscan->c->maxopt)
				Mscan->active=0;
			if((Mscan->active - d_off) > 3)
				d_off = Mscan->active - 3;
			if(Mscan->active < d_off )
				d_off = Mscan->active;

			clrlcd();

			for(i=0;i<4 && i<= Mscan->level; ++i) {
				xyLCD(0,i);
				puts_lcd(Mscan->text[i+d_off]);
				}
			if(RightToLeft)
				xyLCD(abs(MSCANX-Mscan->eol[Mscan->active])-1,Mscan->active-d_off);
			else
				xyLCD(abs(Mscan->eol[Mscan->active]),Mscan->active-d_off);
			if(Mscan->eol[Mscan->active] > 0)
				Cursor(CURS_UND);
			else
				Cursor(CURS_OFF);
            Lr=freelib(Lr);
			lmenu_key=Getch(CHLCD);
			}	while(lmenu_key != ENTER);
		lmenu_key=eof;
		Mscan->c->maxopt = 0;
		DeleteTask(RefreshMenu,RTC_task);
		clrlcd();
		Cursor(CURS_OFF);
		return(null);
		}
/*-----------------------------------------------------------------*/
/* Inserts dummy character to rescan menu screen; shaft_read req.  */
/* is req'd for reference direction entry (int edit_gun(void))	   */
void	RefreshMenu(void)
		{
		if(!Kbhit(CHLCD)) {
			shaft_enable();
			Ungetch(REFRESH,0);
			}
		SetTask(RefreshMenu,1,RTC_task);
		}
/*-----------------------------------------------------------------*/
int		Refresh(void)
		{
        if(CheckTask(RefreshMenu,RTC_task)) {
			SetTask(RefreshMenu,0,RTC_task);
            return(eof);
            }
        else
        	return(null);
		}
/*-----------------------------------------------------------------*/
int		GetMpos()
		{
mscan	*m=Mscan;
		if(m)
			return(m->eol[m->level]);
		else
			return(null);
		}
/*-----------------------------------------------------------------*/
int		SetMpos(int i)
		{
mscan	*m=Mscan;
		if(m)
		{
			while(m->eol[m->level]<i)
				msprint(" ");
			return(m->eol[m->level]=i);
		}
		else
			return(null);
		}
/*-----------------------------------------------------------------*/
int	msprint(char *t)
{
	mscan	*m=Mscan;
	char	p[MSCANX+1];
	if(m) {
		if(RightToLeft)
		{
			strcpy(p,t);
			strcat(p,&m->text[m->level][MSCANX-m->eol[m->level]]);
			sprintf(m->text[m->level],"%*s",MSCANX,p);
		}
		else
			sprintf(&m->text[m->level][m->eol[m->level]],"%s",t);
		m->eol[m->level] += strlen(t);
		return(m->eol[m->level]);
	}
	else
		return(null);
}
/*-----------------------------------------------------------------*/
int		lmenu(void *vp)
		{
		int			i,l,fret;
		char		p[128],*pp,q[128];
		menu		*m;
		command 	*c = Mscan->c;

		curr_menu			= m = vp;
		l=curr_menu->level 	= ++Mscan->level;
		m->opt				= c->opt[l];
/*=================================================================*/
		pp=_MenuHead[m->n];
		for(i=language; i; --i)
			if(strchr(pp,'|'))
			{
				pp=strchr(pp,'|');
				++pp;
			}	
		strcpy(q,pp);
		if(strchr(q,'|'))
			*strchr(q,'|')=0;
/*=================================================================*/
		Mscan->eol[l] = 0;
		translate(q,'_',' ');
		if(RightToLeft)
			sprintf(p,":%7s",q);
		else
			sprintf(p,"%-7s:",q);
		msprint(p);
		if(Kbhit(CHLCD))   	 	/* ce program ki sledi, ne porabi  */
			Getch(CHLCD);	   	/* vpisanega znaka				   */

		if(l == Mscan->active) {
			switch(lmenu_key) {
				case	LEFT:	if(m->opt)
									--m->opt;
								Key=LEFT;
								break;
				case	RIGHT:	++m->opt;
								Key=RIGHT;
								break;
				case	UP:		if(Mscan->active > 0)
									--Mscan->active;
								break;
				case	DOWN: 	if(Mscan->active < c->maxopt)
									++Mscan->active;
								break;
				case	eof:    break;
		
				case	ENTER:	lmenu_key=eof;
								return(eof);
				default:		Key=lmenu_key;
								break;
				}
			lmenu_key=eof;
			}
/*=================================================================*/
		pp=_MenuCode[m->n];
		for(i=language; i; --i)
			if(strchr(pp,'|'))
			{
				pp=strchr(pp,'|');
				++pp;
			}	
		strcpy(q,pp);
		if(strchr(q,'|'))
			*strchr(q,'|')=0;
/*=================================================================*/
		ScanMenu(q,p,&m->opt);
		fret=menu_exe(m,m->opt,NULL);
		if(Kbhit(CHLCD))   		/* ce program ki sledi, ne porabi	*/
			Getch(CHLCD);	   	/* vpisanega znaka					*/
		ScanMenu(q,p,&m->opt);
		c->opt[l] = m->opt;
		curr_menu			= m;
		curr_menu->level 	= l;
		if(*p != '/')
		{
			i=Mscan->level;
			Mscan->level=l;
			msprint(p);
			Mscan->level=i;
		}
		if(l==0)
			c->maxopt = Mscan->level;
		if(fret==eof)
			return(eof);
		if(fret)
			return(fret-1);
		return(null);
		}
/*------------------------------------------------------------------*/
int		menu_test(int ch,char *c, unsigned char s,menu *m)
		{
		uchar	p[16],r;

		if(s != MAXFKEY)	{
			for(r=0;ScanMenu(c,p,&r) != eof;++r)
				if(*p == s)
					return(menu_exe(m,r,NULL));
			return(menu_test(ch,c,MAXFKEY,m));
			}
		else
			for(r=0;ScanMenu(c,p,&r) != eof;++r)
				if(*p == s) {
/*																	*/
/* ce so HW tipke (alpha, backlit etc, Getch  vrne eof !!!) se znak	*/
/* ne posreduje naprej												*/
/*																	*/
					if(ch != eof)
						Key=ch;
					return(menu_exe(m,r,NULL));
					}
		return(null);
		}
/*------------------------------------------------------------------*/
int		menu_exe(struct fmenu *m,int i,void *vp)
		{
		int		fret,(*** f)(void *vp),(**ff)(void *vp);
		void 		***a,**aa;

		if(!m->f || !m->arg)
			return(null);
		for(f=m->f,a=m->arg; i--; ++f,++a);
		ff=*f;
		aa=*a;
		if(!ff)
			return(null);
		while(*ff) {
			if(*aa)
				fret=(*ff)(*aa);
			else
				fret=(*ff)(vp);
			++ff;
			++aa;
			if(fret)
				if(fret == eof)
					return(fret);
				else
					return(fret-1);
			}
		return(null);
		}
/*------------------------------------------------------------------*/
/* Skanira menu strukturo m, prepise n-to opcijo v *c in vrne n, ce	*/
/* je vse OK, sicer vrne eof!. Ce je ob klicu n == 0xff, prepise in	*/
/* in vrne oznako zadnje opcije										*/
/*																	*/
int		ScanMenu(char *p,char *c, char *opt) {
		int		i=0;
		char	*q;

		do	{
			q=c;						/* zacetek stringa			*/
			if((*p==' ') || !*p)		/* prvi znak, ki ni _ ali 0	*/
				continue;
			while((*p != ' ') && *p) {  /* prepis v q do ' ' ali 0	*/
				if(*p == '_')           /* filter za '_' => BLANK 	*/
					*q = ' ';
				else
					*q = *p;
				++p,++q;
				}
			*q = null;                  /* konec stringa			*/
			if(i == *opt)
				return(i);
			++i;
			}	while(*p++);
		*opt = --i;
		return(eof);
		}
/*------------------------------------------------------------------*/
int		ret(void *vp)
		{
		if((int)vp == eof)
			return(eof);
		else
			return(2*(int)vp);
		}
/*------------------------------------------------------------------*/
int		ReadHMS(rtime *t)
		{
static	uint	i;
		int		k;
		double	w,s;

		i = i%6;
		if(Kbhit(CHLCD)) {
			switch(k = Getch(CHLCD)) {
				case CLEAR:	if(!t->hour && !t->min && !t->sec)
								*t=Time;
							else
								t->hour=t->min=t->sec=0;
							i=0;
							break;
				case REFRESH:
							break;
				default:	if(isdigit(k)) {
								k -= '0';
								if(RightToLeft)
									switch(i) {
/*										case 1:	t->hour = t->hour%10 + k*10;
												break;
										case 0:	t->hour = (t->hour/10)*10 + k;
												break;
										case 3:	t->min = t->min%10 + k*10;
												break;
										case 2:	t->min = (t->min/10)*10 + k;
												break;
										case 5:	t->sec = t->sec%10 + k*10;
												break;
										case 4:	t->sec = (t->sec/10)*10 + k;
												break;
*/
										case 5:	t->hour = t->hour%10 + k*10;
												break;
										case 4:	t->hour = (t->hour/10)*10 + k;
												break;
										case 3:	t->min = t->min%10 + k*10;
												break;
										case 2:	t->min = (t->min/10)*10 + k;
												break;
										case 1:	t->sec = t->sec%10 + k*10;
												break;
										case 0:	t->sec = (t->sec/10)*10 + k;
												break;

										default:i=-1;
												break;
										}
								else
									switch(i) {
										case 0:	t->hour = t->hour%10 + k*10;
												break;
										case 1:	t->hour = (t->hour/10)*10 + k;
												break;
										case 2:	t->min = t->min%10 + k*10;
												break;
										case 3:	t->min = (t->min/10)*10 + k;
												break;
										case 4:	t->sec = t->sec%10 + k*10;
												break;
										case 5:	t->sec = (t->sec/10)*10 + k;
												break;
										default:i=-1;
												break;
										}
								i = ++i%6;
								}
							else
								i=0;
							break;
				}
/* precisti vpis !!! */
			w=calc_week(t);
			s=calc_secs(t);
			calc_rtime(w,s,t);
			}
		return(i+i/2);	/*** cursor pos., format hh:mm:ss ***/
		}
/*------------------------------------------------------------------*/
int		ReadDMY(void *vp)
		{
static	uint	i;
		rtime	t=*(rtime *)vp;
		int		k;
		double	w,s;

		i = i%6;
		if(vp && Kbhit(CHLCD)) {
			switch(k = Getch(CHLCD)) {
				case CLEAR:	t.date=0;
							t.month=0;
							t.year=0;
							i=0;
							break;
				case LEFT:	if(t.month)
								--t.month;
							else
							{
								t.month=11;
								if(t.year)
									--t.year;
							}
							break;
				case RIGHT:	t.month = ++t.month % 12;
							if(!t.month)
								++t.year;
							break;
				case REFRESH:
							break;
				default:	if(isdigit(k)) {
								k -= '0';
								++t.date;
								++t.month;
								if(RightToLeft)
									switch(i) {
										case 1:	t.date = t.date%10 + k*10;
												break;
										case 0:	t.date = (t.date/10)*10 + k;
												break;
										default:i=-1;
												break;
										}
								else
									switch(i) {
										case 0:	t.date = t.date%10 + k*10;
												break;
										case 1:	t.date = (t.date/10)*10 + k;
												break;
										default:i=-1;
												break;
										}
								--t.date;
								--t.month;
								i = ++i%2;
								}
							else
								i=0;
							break;
				}
/* precisti vpis !!! */
			w=calc_week(&t);
			s=calc_secs(&t);
			calc_rtime(w,s,&t);
			*(rtime *)vp=t;
			}
		return(i);		/*** cursor pos., format hh:mm:ss ***/
		}
/*--------------------------------------------------------*/
int		ReadInt(int p)
		{
		int		q;

		if(Kbhit(CHLCD)) {
			switch(q = Getch(CHLCD)) {
				case CLEAR:	p = 0;
							break;
				case SIGN:	p = -p;
							break;
				default:	if(isdigit(q)) {
								if(p < 0)
									p = p * 10 - q + '0';
								else
									p = p * 10 + q - '0';
								}
							else
								return(p);
							break;
				}
			}
		return(p);
		}
/*--------------------------------------------------------*/
int		EnterInt(void *vp)
		{
		int		q,*p = vp;
		char	c[16];
		if(p) {
			if(Kbhit(CHLCD)) {
				switch(q = Getch(CHLCD)) {
					case CLEAR:	*p = 0;
								break;
					case SIGN:	*p = -*p;
								break;
					default:	if(isdigit(q)) {
									if(*p < 0)
										*p = *p * 10 - q + '0';
									else
										*p = *p * 10 + q - '0';
									}
								break;
					}
				}
			sprintf(c,"%d",*p);
			msprint(c);
			}
		return(null);
		}
/*--------------------------------------------------------*/
int		EnterLong(void *vp)
		{
		int		q;
		long	*p = vp;
		char	c[16];
		if(p) {
			if(Kbhit(CHLCD)) {
				switch(q = Getch(CHLCD)) {
					case CLEAR:	*p = 0;
								break;
					case SIGN:	*p = -*p;
								break;
					default:	if(isdigit(q)) {
									if(*p < 0)
										*p = *p * 10 - q + '0';
									else
										*p = *p * 10 + q - '0';
									}
								break;
					}
				}
			sprintf(c,"%ld",*p);
			msprint(c);
			}
		return(null);
		}
/*--------------------------------------------------------*/
int		EnterOpt(void *vp)
		{
		int 	*i	=vp;
		if(i)
			if(!Kbhit(CHLCD))
				curr_menu->opt=*i;
			else
				*i=curr_menu->opt;
		return(null);
		}
/*--------------------------------------------------------*/
long	IncLong(long *p, int i)
		{
		if(p && Kbhit(CHLCD)) {
			switch(Getch(CHLCD)) {
				case CLEAR:	*p=0;
							break;
				case SIGN:	*p = -*p;
							break;
				case LEFT:	*p -= i;
							break;
				case RIGHT:	*p += i;
							break;
				}
			}
		return(*p);
		}
/*--------------------------------------------------------*/
int		IncInt(int *p)
		{
		if(p && Kbhit(CHLCD)) {
			switch(Getch(CHLCD)) {
				case CLEAR:	*p=0;
							break;
				case SIGN:	*p = -*p;
							break;
				case LEFT:	*p -= 1;
							break;
				case RIGHT:	*p += 1;
							break;
				default:	break;
				}
			}
		return(null);
		}
/*------------------------------------------------------------------*/
/* prevedba koordinat v zeljeno obliko in preracun na			    */
/* izhodisce in smer z upostevanjem NORTH kot osnove za korekcijo	*/
/*                                                      			*/
/*------------------------------------------------------------------*/
int		LlaToCorr(lib *l,lib *ll,lib *pos, lib *n,int xmode)
		{
		int	i;
		lib	*nn;
		if(!n)
			return(LlaToRel(l,ll,pos,NULL,xmode));
		if(!pos)
			return(LlaToRel(l,ll,n,NULL,xmode));

		copylib(l, nn=makelib());
		i=LlaToRel(l,ll,pos,n,xmode);
		LlaToRel(n,nn,pos,n,xmode);
		ll->xc->x -= nn->xc->x;
		ll->xc->y -= nn->xc->y;
		ll->xc->z -= nn->xc->z;
		freelib(nn);
		return(i);
		}
/*------------------------------------------------------------------*/
int		CorrToLla(lib *l,lib *ll,lib *pos, lib *n,int xmode)
		{
		lib	*nn;

		if(!n)
			return(RelToLla(l,ll,pos,NULL,xmode));
		if(!pos)
			return(RelToLla(l,ll,n,NULL,xmode));
		copylib(l,nn=makelib()); /* da se ohrani tip, stevilo tock etc. */
		LlaToRel(n,nn,pos,n,xmode);
		l->xc->x += nn->xc->x;
		l->xc->y += nn->xc->y;
		l->xc->z += nn->xc->z;
		freelib(nn);
		return(RelToLla(l,ll,pos,n,xmode));
		}
/*------------------------------------------------------------------*/
/* prevedba lla koordinat v zeljeno obliko in preracun na			*/
/* izhodisce in smer !                                  			*/
/*                                                      			*/
/*------------------------------------------------------------------*/
int		LlaToRel(lib *l,lib *ll,lib *pos, lib *n,int xmode)
		{
		void	Lla2Utm(lib *,lib *,lib *);
		double	north=0,del=0;
		lib		*o,*b;

		if(!l || !ll)
			return(xmode);
		if(checktype(l,".p") || checktype(l,".r")) {
			xmode=N_E_Z;
			pos=NULL;
			n=NULL;
			}
		if(checktype(l,".b")) {
			xmode=EL_AZ;
			pos=OBP;
			n=NULL;
			}
		if(checktype(l,".m")) {
			xmode=EL_AZ_R;
			pos=OBP;
			n=NULL;
			}
		if(!l->xc || !ll->xc)
			return(xmode);

		if(!pos || !pos->xc ) {
			Lla2Utm(l,ll,NULL);
			return(N_E_Z);
			}
		else{
			copylib(pos,o=makelib());
			Lla2Utm(pos,o,NULL);	/* ~~~*/
			}
		if(n && n->xc)
		{
			copylib(pos,b=makelib());
			Lla2Utm(n,b,pos);
			b->xc->x -= o->xc->x;
			b->xc->y -= o->xc->y;
			b->xc->z -= o->xc->z;
			if(b->xc->x || b->xc->y || b->xc->z)
			{
				north=-atan2(b->xc->y,b->xc->x);
				del=-atan2(b->xc->z,sqrt(b->xc->x * b->xc->x +
					b->xc->y * b->xc->y));
			}
			freelib(b);
		}

		Lla2Utm(l,ll,pos);
		ll->xc->x -= o->xc->x;
		ll->xc->y -= o->xc->y;
		ll->xc->z -= o->xc->z;
		gridto(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		ll->xc->y += north;
		ll->xc->x += del;
		togrid(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		gridto(ll->xc,ll->xc,xmode,maxmils);
		freelib(o);
		return(xmode);
		}
/*------------------------------------------------------------------*/
int		RelToLla(lib *l,lib *ll,lib *pos, lib *n,int xmode)
		{
		double	north=0,del=0;
		lib		*o,*b;

		if(!l || !ll)
			return(xmode);
		if(checktype(l,".p") || checktype(l,".r")) {
			xmode=N_E_Z;
			pos=NULL;
			n=NULL;
			}
		if(checktype(l,".b")) {
			xmode=EL_AZ;
			pos=OBP;
			n=NULL;
			}
		if(checktype(l,".m")) {
			xmode=EL_AZ_R;
			pos=OBP;
			n=NULL;
			}
		if(!l->xc || !ll->xc)
			return(xmode);

		if(!pos || !pos->xc ) {
			Utm2Lla(l,ll);
			return(N_E_Z);
			}
		else{
			copylib(pos,o=makelib());
			Lla2Utm(pos,o,NULL);
			}
		if(n)
		{
			if(n->xc)
			{
				copylib(pos,b=makelib());
				Lla2Utm(n,b,pos);
				b->xc->x -= o->xc->x;
				b->xc->y -= o->xc->y;
				b->xc->z -= o->xc->z;
				if(b->xc->x || b->xc->y || b->xc->z)
				{
					north=-atan2(b->xc->y,b->xc->x);
					del=-atan2(b->xc->z,sqrt(b->xc->x * b->xc->x +
						b->xc->y * b->xc->y));
				}
				freelib(b);
			}
		}
		togrid(l->xc,ll->xc,xmode,maxmils);
		gridto(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		ll->xc->y -= north;
		ll->xc->x -= del;
		togrid(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		ll->xc->x += o->xc->x;
		ll->xc->y += o->xc->y;
		ll->xc->z += o->xc->z;
		freelib(o);
		Utm2Lla(ll,ll); 		/* !!! */
		return(xmode);
		}
/*------------------------------------------------------------------*/
/* prevedba koordinat v zeljeno obliko in preracun na			    */
/* izhodisce in smer !                                  			*/
/*                                                      			*/
/*------------------------------------------------------------------*/
int		GridToRel(lib *l,lib *ll,lib *pos, lib *n,int xmode)
		{
		double	x,y,z,north=0,del=0;
		coord	origin;

		if(!l || !ll)
			return(xmode);
		if(checktype(l,".p") || checktype(l,".r")) {
			xmode=N_E_Z;
			pos=NULL;
			n=NULL;
			}
		if(checktype(l,".b")) {
			xmode=EL_AZ;
			pos=OBP;
			n=NULL;
			}
		if(checktype(l,".m")) {
			xmode=EL_AZ_R;
			pos=OBP;
			n=NULL;
			}
		if(!l->xc || !ll->xc)
			return(xmode);

		if(!pos)
			origin.x=origin.y=origin.z=0;
		else {
			if(pos->xc) {
				origin.x=pos->xc->x;
				origin.y=pos->xc->y;
				origin.z=pos->xc->z;
				}
			else {
				err(E_VOID_DATA);
				origin.x=origin.y=origin.z=0;
				}
			}
		if(n) {
			if(n->xc) {
				x=n->xc->x-origin.x;
				y=n->xc->y-origin.y;
				z=n->xc->z-origin.z;
				if((x==0) && (y==0) && (z==0)) {
					err(E_NORTH_ORG);
					DeleteTask(DisplPoint,RTC_task);
					}
				else {
					north=-atan2(y,x);
					del=-atan2(z,sqrt(x*x+y*y));
					}
				}
			else
				err(E_VOID_DATA);
			}
		ll->xc->x = l->xc->x - origin.x;
		ll->xc->y = l->xc->y - origin.y;
		ll->xc->z = l->xc->z - origin.z;
		gridto(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		ll->xc->y += north;
		ll->xc->x += del;
		togrid(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		gridto(ll->xc,ll->xc,xmode,maxmils);
		return(xmode);
		}
/*------------------------------------------------------------------*/
int		RelToGrid(lib *l,lib *ll,lib *pos, lib *n,int xmode)
		{
		double	x,y,z,north=0,del=0;
		coord	origin;

		if(!l || !ll)
			return(xmode);
		if(checktype(l,".p") || checktype(l,".r")) {
			xmode=N_E_Z;
			pos=NULL;
			n=NULL;
			}
		if(checktype(l,".b")) {
			xmode=EL_AZ;
			pos=OBP;
			n=NULL;
			}
		if(checktype(l,".m")) {
			xmode=EL_AZ_R;
			pos=OBP;
			n=NULL;
			}
		if(!l->xc || !ll->xc)
			return(xmode);

		if(!pos)
			origin.x=origin.y=origin.z=0;
		else {
			if(pos->xc) {
				origin.x=pos->xc->x;
				origin.y=pos->xc->y;
				origin.z=pos->xc->z;
				}
			else {
				err(E_VOID_DATA);
				origin.x=origin.y=origin.z=0;
				}
			}
		if(n) {
			if(n->xc) {
				x=n->xc->x-origin.x;
				y=n->xc->y-origin.y;
				z=n->xc->z-origin.z;
				if((x==0) && (y==0) && (z==0)) {
					err(E_NORTH_ORG);
					DeleteTask(DisplPoint,RTC_task);
					}
				else {
					north=-atan2(y,x);
					del=-atan2(z,sqrt(x*x+y*y));
					}
				}
			 else
				err(E_VOID_DATA);
			 }
		togrid(l->xc,ll->xc,xmode,maxmils);
		gridto(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		ll->xc->y -= north;
		ll->xc->x -= del;
		togrid(ll->xc,ll->xc,EL_AZ_R,2.0*M_PI);
		ll->xc->x += origin.x;
		ll->xc->y += origin.y;
		ll->xc->z += origin.z;
		return(xmode);
		}
/*------------------------------------------------------------------*/
coord		*togrid(coord *p,coord *q,int xmode,double units)
			{
			double	x,y,z,az,el,r,dz;

			if(!p || !q)
				return(NULL);
			switch(xmode) {
				case EL_AZ_R:	el=p->x/units*2.0*M_PI;
								az=p->y/units*2.0*M_PI;
								r=p->z;

								x=r*cos(el)*cos(az);
								y=r*cos(el)*sin(az);
								z=r*sin(el);
								break;

				case EL_AZ:		el=p->x/units*2.0*M_PI;
								az=p->y/units*2.0*M_PI;
								if(az > M_PI)
									az=az-2.0*M_PI;
								r=p->z;

								x=r*cos(el)*cos(az);
								y=r*cos(el)*sin(az);
								z=r*sin(el);
								break;

				case AZ_R_dZ:   az=p->x/units*2.0*M_PI;
								r=p->y;
								dz=p->z;

								x=r*cos(az);
								y=r*sin(az);
								z=dz;
								break;

				default:		x=p->x;
								y=p->y;
								z=p->z;
								break;
				}

			q->x=x;
			q->y=y;
			q->z=z;
			return(q);
			}
/*------------------------------------------------------------------*/
coord		*gridto(coord *p,coord *q,int xmode,double units)
			{
			double	x,y,z,el,az,r;

			if(!p || !q)
				return(NULL);

			x=p->x;
			y=p->y;
			z=p->z;
			r=sqrt(x*x+y*y);

			if(!z && !r)
				el=0;
			else
				el=atan2(z,r);

			if(!x && !y)
				az=0;
			else
				az=atan2(y,x);

			r=sqrt(x*x+y*y+z*z);
/*
!!!			if(az<0)
				az += 2.0*M_PI;
*/
			switch(xmode)
				{
				case EL_AZ_R:
				case EL_AZ:		q->x=el/2.0/M_PI*units;
								q->y=az/2.0/M_PI*units;
								q->z=r;
								break;
				case AZ_R_dZ:	q->x=az/2.0/M_PI*units;
								q->y=r*cos(el);
								q->z=z;
								break;
				default:		q->x=p->x;
								q->y=p->y;
								q->z=p->z;
								break;
				}
			return(q);
			}
/*------------------------------------------------------*/
double	read_AZ(void)
		{
		double a,e,r;

		while(!shaft_ready());
		a =	(shaft_data[SHAFT_AZ] - az_align) * SHtoR;
		r =	roll * cos(a) + pitch * sin(a);

		e = (shaft_data[SHAFT_EL] - el_null + el_align) * SHtoR;
		a =	atan2(sin(a)*cos(roll),
				cos(a)*cos(pitch) + sin(a)*sin(roll)*sin(pitch));
		a += atan2(sin(r)*sin(e),
				cos(e)) - az_off;

		while(a >= M_PI)
			a -= 2.0*M_PI;
		while(a < -M_PI)
			a += 2.0*M_PI;

		return(a*maxmils/2.0/M_PI);
		}
/*------------------------------------------------------*/
double	read_EL(void)
		{
		double a,e,r;

		while(!shaft_ready());
		a = (shaft_data[SHAFT_AZ] + az_align) * SHtoR;
		e = (shaft_data[SHAFT_EL]  - el_null + el_align) * SHtoR;

		r = roll * cos(a) + pitch * sin(a);
		e = atan2(cos(r)*sin(e), cos(e));

		e+=	pitch * cos(a) - roll * sin(a);

		while(e >= M_PI)		/* elev. range +/- pi			*/
			e -= 2.0*M_PI;		/*								*/
		while(e < -M_PI)		/*								*/
			e += 2.0*M_PI;		/*								*/

		return(e*maxmils/2.0/M_PI);
		}
/*------------------------------------------------------*/
coord	*AddCoord(lib *l,double x,double y,double z)
		{
		int	i;
		if(!l || !l->ltype) {
			err(E_VOID_TYPE);
			return(NULL);
			}
		i=targets_n(l->x);
		if((checktype(l,TYPE_RP) || checktype(l,TYPE_OP)) && (i>=1)) {
			err(E_1PT_ALLW);
			return(NULL);
			}
		if(checktype(l,TYPE_TG) && (i>=3)) {
			err(E_3PT_ALLW);
			return(NULL);
			}
		if(checktype(l,TYPE_BT) && (i>=8)) {
			err(E_8PT_ALLW);
			return(NULL);
			}
		return(add_coord(l,x,y,z));
		}
/*------------------------------------------------------*/
coord	*add_coord(lib *l,double x,double y,double z)
		{
		coord	*p;

		if(!l || !l->ltype) {
			err(E_VOID_TYPE);
			return(NULL);
			}
		p=memalloc(sizeof(coord));
		if(!l->xc) {
			l->x=l->xc=p;
			p->next=NULL;
			}
		else {
			p->next=l->xc->next;
			l->xc->next=p;
			l->xc=p;
			}
		l->xc->t		= Time;
		l->xc->x		= x;
		l->xc->y		= y;
		l->xc->z		= z;
		l->xc->refp		= 0;
		l->xc->muzzle	= 0;
		return(p);
		}
/*--------------------------------------------------------*/
coord	*FirstCoord(lib *l)
		{
		double	dt;
		coord	*p,*q;
		rtime	t=Time;
		if(!l || !l->x)
			return(NULL);
		p=q=l->x;
		dt=dtime(&t,&p->t);
		p=p->next;
		while(p)	{
					if(dtime(&t,&p->t) > dt)	{
						q=p;
						dt=dtime(&t,&p->t);
						}
					p=p->next;
					}
		return(q);
		}
/*-------------------------------------------------------*/
coord	*LastCoord(lib *l)
		{
		double	dt;
		coord	*p,*q;
		rtime	t=Time;

		if(!l || !l->x)
			return(NULL);
		p=q=l->x;
		dt=dtime(&t,&p->t);
		p=p->next;
		while(p)	{
					if(dtime(&t,&p->t) < dt)	{
						q=p;
						dt=dtime(&t,&p->t);
						}
					p=p->next;
					}
		return(q);
		}
/*-----------------------------------------------------------------*/
coord  	*erase_coord(lib *l,coord *x)
		{
		coord	*p;
		if(x == l->x) {
			l->x=x->next;
			if(x == l->xc)
				l->xc = l->x;
			}
		else {
			for(p=l->x; p->next != x; p=p->next);
			p->next=x->next;
			if(l->xc == x)
				l->xc = p;
			}
		memfree(x);
		return(l->x);
		}
/*------------------------------------------------------------------*/
int	strscan(char *s,char *ss[],int c,int n)
{
	int		i=0;
	char	*p=s,*q;
	while(i<n)
		{
			while(*p && *p==' ')
				++p;
			if(!*p)
				return(i);
			q=p;
			p=strchr(p,c);
			if(p)
			{
				ss[i++]=q;
				q=p++;
				do *q--=0; while (*q==' ');
			}
			else
			{
				ss[i++]=q;
				q=strchr(q,0);
				do *q--=0; while (*q==' ');
				break;
			}
		}
	return(i);
}
/*------------------------------------------------------------------*/
/*	prevrne vrstni red v stringu c glede na delimiterje v cc
	Za potrebe RightToLeft
*/
void strswap(char *c,char *cc)
{
	int		i=0;
	char	s[32],*ss[32];
	char	*p=s,*q;

	if(!RightToLeft)
		return;
	strcpy(s,c);
	while(*cc)
		{
			if(!*p)
				return;
			q=p;
			p=strchr(p,*cc++);
			if(p)
			{
				*p++=0;
				ss[i++]=q;
			}
			else
				p=q;
		}
	ss[i++]=p;
	p=c;
	do
	{
		strcpy(p,ss[--i]);
		p=strchr(p,0);
		if(i)
			*p++ = *--cc;
	} while(i);
}
/*------------------------------------------------------------------*/
lib		*Displc(lib *l)
		{
		lib		*ll;
		int		i;

		if(!l || !l->ltype)
			return(NULL);
		copylib(l,ll=makelib());
		Display(ll,i=LlaToCorr(l,ll,ORG,NORTH,d_mode),maxmils);
		if(!ORG && i ==N_E_Z && l->x)
		{
			if(FindZone(l->xc->x,l->xc->y))
				ShowZone(ll);
			else
				ShowUtm(ll);
		}
		else
		{
			ZoneId=NULL;
			for(i=0; i<32; ++i)
				LcdExt[i]=' ';
		}
		freelib(ll);
		return(l);
		}
/*------------------------------------------------------------------*/
lib		*Display(lib	*ll, int dmode, int mils)
		{
		char			p[128];
		int				i,j;

		i=GetLCDaddr();

		if(!ll->x)
		{
			sprintf(p," -----      ");
			xyLCD(0,0);
			puts_lcd(p);
			xyLCD(0,1);
			puts_lcd(p);
			xyLCD(0,2);
			puts_lcd(p);
		}		
		else
		{
			switch(time_mode) {
				case HOUR_MIN:
/*						if(RightToLeft)
							sprintf(p,"%2d:%02d ",ll->xc->t.min,ll->xc->t.hour);
						else
*/							sprintf(p,"%2d:%02d ",ll->xc->t.hour,ll->xc->t.min);
						break;
				case MIN_SEC:
/*						if(RightToLeft)
							sprintf(p,"%2d.%02d ",ll->xc->t.sec,ll->xc->t.min);
						else
*/							sprintf(p,"%2d.%02d ",ll->xc->t.min,ll->xc->t.sec);
						break;
				case DAY_DATE:
						sprintf(p,"%2s-%-2d ",GetMessCode(_DAY_1+ll->xc->t.day),1 + ll->xc->t.date);
						break;
				case MONTH_YEAR:
						sprintf(p,"%3s%02d ",GetMessCode(_MONTH_1+ll->xc->t.month),ll->xc->t.year);
						break;
				}
			xyLCD(7,0);
			puts_lcd(p);

			switch(dmode) {
				case N_E_Z:		sprintf(p,"%6.0f ",nround(ll->xc->x,6));
								xyLCD(0,0);
								puts_lcd(p);
								sprintf(p,"%6.0f ",nround(ll->xc->y,6));
								xyLCD(0,1);
								puts_lcd(p);
								sprintf(p,"%6.0f ",nround(ll->xc->z,6));
								xyLCD(0,2);
								puts_lcd(p);
								break;
				case EL_AZ_R:	if(!NORTH && (ll->xc->y < 0))
									ll->xc->y += mils;
								sprintf(p,"%6.2f ",fmod(nround(ll->xc->y,4),mils)/100);
								translate(p,'.','-');
								xyLCD(0,0);
								puts_lcd(p);	
								sprintf(p,"%6.0f ",nround(ll->xc->z,5));
								xyLCD(0,1);
								puts_lcd(p);									
								sprintf(p,"%6.2f ",fmod(nround(ll->xc->x,4),mils)/100);	
								translate(p,'.','-');
								xyLCD(0,2);
								puts_lcd(p);
								break;
				case AZ_R_dZ:	if(!NORTH && (ll->xc->x < 0))
									ll->xc->x += mils;
								sprintf(p,"%6.2f ",fmod(nround(ll->xc->x,4),mils)/100);
								translate(p,'.','-');
								xyLCD(0,0);
								puts_lcd(p);	
								sprintf(p,"%6.0f ",nround(ll->xc->y,5));
								xyLCD(0,1);
								puts_lcd(p);									
								sprintf(p,"%6.0f ",nround(ll->xc->z,5));	
								xyLCD(0,2);
								puts_lcd(p);
								break;
				case EL_AZ:		if(!NORTH && (ll->xc->y < 0))
									ll->xc->y += mils;
								sprintf(p,"%6.2f ",fmod(nround(ll->xc->y,4),mils)/100);
								translate(p,'.','-');
								xyLCD(0,0);
								puts_lcd(p);	
								xyLCD(0,1);
								puts_lcd("       ");
								sprintf(p,"%6.2f ",fmod(nround(ll->xc->x,4),mils)/100);	
								translate(p,'.','-');
								xyLCD(0,2);
								puts_lcd(p);
								break;
				}
		}
		if(NORTH) {
			for(j=0; j<3; ++j) {
				xyLCD(6,j);
				Putch(symDELTA,CHLCD);
				}
			}
		xyLCD(0,3);

/*-- izpis opisnega teksta  -------------------------------*/

		if(ll->txt)
			puts_lcd(ll->txt->t);
		else
			puts_lcd(" ...        ");

/*-- izpis tipa objekta -----------------------------------*/
		LinkAddrText(LinkAddr(ll),p);
		xyLCD(7,1);
		puts_lcd(p);

/*--- izpis stevila koordinatnih tock    ------------------*/
		if(ll->x)
			sprintf(p,"%-d/%-3d",targets_xn(ll),targets_n(ll->x));
		else
			sprintf(p,"     ");
		xyLCD(7,2);
		puts_lcd(p);
		SetLCDaddr(i);

		WriteSophie(NULL);
		return(ll);
		}
/*------------------------------------------------------------------*/
int		translate(char *p,char a,char b)
		{
		char 	*q;
		int		i=0;

		for(q=p; *q; ++q)
			if(*q==a) {
				*q=b;
				++i;
				}
		return(i);
		}
/*------------------------------------------------------------------*/
double	nround(double i, int n)
		{
		if(i >= 0)
			return(fmod(floor(i+0.5),pow(10,n)));
		else
			return(fmod(ceil(i-0.5),pow(10,n)));
		}
/*------------------------------------------------------------------*/
lib			*makelib(void)
			{
			lib	*l=memalloc(sizeof(lib));
			l->n=1;
			l->next=l;
			return(l);
			}
/*------------------------------------------------------------------*/
lib			*copylib(lib *a,lib *b)
			{
			lib	   	*p;
			if(!a || !b)
				return(NULL);
			p=b->next;
			freecoord(b->x);
			*b=*a;
			b->x=b->xc=NULL;
			copycoord(a,b);
			b->next=p;
			b->check=null;
			return(b);
			}
/*------------------------------------------------------------------*/
void		*copycoord(lib *a,lib *b)
			{
			coord	*p;

			if(!a || !b)
				return(NULL);
			b->x=b->xc=freecoord(b->x);
			b->x=b->xc=copyc(a->x);
			p=a->x;
			while(p != a->xc) {
				p=p->next,
				b->xc=b->xc->next;
				}
			return(b->xc);
			}
/*------------------------------------------------------------------*/
coord		*copyc(coord *p)
			{
			coord	*q;
			if(!p)
				return(NULL);
			q=memalloc(sizeof(coord));
			*q=*p;
			q->next=copyc(p->next);
			return(q);
			}
/*------------------------------------------------------------------*/
void		*freecoord(coord *p)
			{
			if(p)
				{
				if(p->next)
					freecoord(p->next);
				memfree(p);
				return(NULL);
				}
			return(NULL);
			}
/*------------------------------------------------------------------*/
void		*freelib(lib	*p)
			{
			if(p) {
				freecoord(p->x);
				memfree(p);
				}
			return(NULL);
			}
/*------------------------------------------------------------------*/
void		*freelibrary(lib	*p)
			{
				lib		*q,*r;
				q=p;
				if(p) {
					do
					{
					r=q->next;
					freecoord( q->x );
					memfree( q );
					q=r;
					}	while( q != p );
				}
			return(NULL);
			}
/*------------------------------------------------------------------*/
char		*SaveLib(lib *l,uchar *f)
			{
			int		nxc=-1;
			lib		*p;

			p=l;
			if(p)
				do	{
					watchdog();
					f=SaveLibItem(p,f);
					p=p->next;
				}	while(p != l);
			f=prog_byte(&nxc,f,sizeof(int));
			watchdog();
			return(SaveComm(f));
			}
/*------------------------------------------------------------------*/
uchar	*SaveLibItem(lib *p, uchar *f)
		{
char	*tblanks_off(char *);
		coord	*xp;
		int		nxc,i;

		f=prog_byte(&p->n,f,sizeof(int));
        if(p->ltype)
			f=prog_byte(p->ltype->t,f,LIBTYPE_LEN*sizeof(char));
		else
			f=prog_byte(&p->ltype,f,sizeof(void *));

		if(p->txt)
			f=prog_byte(p->txt->t,f,LIBTEXT_LEN*sizeof(char));
		else
			f=prog_byte(&p->txt,f,sizeof(void *));
			nxc=targets_n(p->x);
		f=prog_byte(&nxc,f,sizeof(int));
		for(xp=p->x,i=nxc=0; xp; xp=xp->next,++i)
			{
			f=prog_byte(&xp->x,f,sizeof(double));
			f=prog_byte(&xp->y,f,sizeof(double));
			f=prog_byte(&xp->z,f,sizeof(double));
			f=prog_byte(&xp->t,f,(sizeof(rtime)/2)*2+2);
			f=prog_byte(&xp->refp,f,sizeof(int));
			f=prog_byte(&xp->muzzle,f,sizeof(int));
			if(xp == p->xc)
				nxc=i;
		}
		f=prog_byte(&nxc,f,sizeof(int));
		f=prog_byte(&p->gun,f,sizeof(int));
		f=prog_byte(&p->refd,f,sizeof(int));
		f=prog_byte(&p->powder,f,sizeof(int));
		for(i=0; i<MAXAMM;++i)
			f=prog_byte(&p->ammpcs[i],f,sizeof(int));
		return(f);
		}
/*------------------------------------------------------------------*/
uchar	*LoadComm(uchar *f)
		{
		int	i;
		command *p;
/***												***/
/*** check commands on CP							***/
/*** if 1.st word = eof -> empty command list		***/
/*** else erase all existing messages				***/
/***												***/
		while(CP)
			EraseCommand(&CP);
/***												***/
/*** make new command & copy flash EE				***/
/***												***/
		f=load_byte(&i,f,sizeof(int));
		if(i != eof) {
			while(i--)	{
				new_command(&CP);
				p=CP->next;
				f=load_byte(CP,f,sizeof(command)-sizeof(void *)-sizeof(int));
				CP->next=p;
				CP->chk = CP->maxopt = null;
				}
			}
		return(f);
		}
/*------------------------------------------------------------------*/
uchar	*SaveComm(uchar *f)
		{
		int		i;
		uchar	*ff;
		command *p;

		ff = f;
		f += sizeof(int);
		p = CP;
		i = 0;
		if(p) {
				do	{
				f=prog_byte(p,f,sizeof(command)-sizeof(void *)-sizeof(int));
				p=p->next;
				++i;
				}	while(p != CP);
			}
		prog_byte(&i,ff,sizeof(int));
		return(f);
		}
/*------------------------------------------------------------------*/
char		*tblanks_off(char *s)
			{
			char *p,*q;
			for(p=q=s; *p;)
				if(*p++ != ' ')
					q=p;
			*q=null;
			for(p=s; *p; ++p)
				if(*p != ' ')
					return(p);
			return(p);
			}
/*------------------------------------------------------------------*/
lib			*LoadLib(uchar *f)
			{
			lib	*p,*q;

			q=LoadLibItems(f);
			if(!q)
				return(q);
			for(p=q; p->next; p=p->next);
			p->next=q;
			return(q);
			}
/*------------------------------------------------------------------*/
lib			*LoadLibItems(uchar *f)
			{
			lib		*p;
			int		i;
			char	c[LIBTEXT_LEN];

			f = load_byte(&i,f,sizeof(int));
			if(i == eof) {
				LoadComm(f);
				return(NULL);
				}
			p=makelib();
			p->n=i;

			f = load_byte(c,f,LIBTYPE_LEN*sizeof(char));
			p->ltype=addtype(c);
			f = load_byte(c,f,LIBTEXT_LEN*sizeof(char));
			p->txt=addtxt(p,c);

			f = load_byte(&i,f,sizeof(int));
			if(i)
				add_coord(p,0.0,0.0,0.0);
			while(i) {
				f = load_byte(&p->xc->x,f,sizeof(double));
				f = load_byte(&p->xc->y,f,sizeof(double));
				f = load_byte(&p->xc->z,f,sizeof(double));
				f = load_byte(&p->xc->t,f,(sizeof(rtime)/2)*2+2);
				f = load_byte(&p->xc->refp,f,sizeof(int));
				f = load_byte(&p->xc->muzzle,f,sizeof(int));
				if(--i)
					add_coord(p,0.0,0.0,0.0);
				}
			f = load_byte(&i,f,sizeof(int));
			for(p->xc=p->x; i; --i)
				p->xc = p->xc->next;
			f = load_byte(&p->gun,f,sizeof(int));
			f = load_byte(&p->refd,f,sizeof(int));
			f = load_byte(&p->powder,f,sizeof(int));
			for(i=0; i<MAXAMM;++i)
				f = load_byte(&p->ammpcs[i],f,sizeof(int));
			watchdog();
			p->next=LoadLibItems(f);
			return(p);
			}
/*------------------------------------------------------------------*/
int			clear_screen(void *vp)
{
			int		i;
			char	*p,q[]="                ";
			if(!CheckTask(screen_unswap,PIT_task))
       			p = LcdBuffer;
			else
       			p = LcdBackup;
			for(i=0; i<4; ++i)
			{
				strncpy(&p[20*i],q,12);
				strncpy(&LcdExt[8*i],q,8);
			}
			Cursor(CURS_OFF);			
			SetTask(RefreshScreen,5,PIT_task);
			return(null);
}
/*------------------------------------------------------------------*/
int			targets_n(coord *p)
			{
			int i;
			for(i=0; p; p=p->next)
				++i;
			return(i);
			}             
/*------------------------------------------------------------------*/
int			targets_xn(lib *l)
			{
			int		i;
			coord	*p;

			for(i=0, p=l->x; p; p=p->next) {
				++i;
				if(p==l->xc)
					return(i);
				}
			return(i);
			}
/*------------------------------------------------------------------*/
int			fromBCD(int c)
			{
			return((c/16)*10 + (c%16));
			}
/*------------------------------------------------------------------*/
int			toBCD(int c)
			{
			return((c/10)*16 + (c%10));
			}
/*------------------------------------------------------------------*/
queue	*SetTask(void (*t)(void),int n,queue *q)
		{
		queue	*p;
/* ce je pointer prazen -> prvi task		*/
		if(!q) {
			q=memalloc(sizeof(queue));
			q->rate=n;
			q->count=n;
			q->task=t;
			return(q);
			}
/* update obstojecega taska 				*/
		for(p=q; p; p=p->next) {
			if(p->task==t) {
				p->rate=n;
				p->count=n;
				return(p);
				}
			}
/* iskanje praznega slota					*/
		for(p=q; p->next; p=p->next)
		  if(!p->task) {
			p->rate=n;
			p->count=n;
			p->task=t;
			return(p);
			}
		p->next=SetTask(t,n,NULL);
		return(p->next);
		}
/*------------------------------------------------------------------*/
void	DeleteTask(void (*t)(void),queue *q)
		{
		queue	*p;
		for(p=q; p; p=p->next)
			if(p->task == t)
				p->task = NULL;
		}
/*------------------------------------------------------------------*/
queue	*CheckTask(void (*t)(void),queue *q)
		{
		queue *p;
		for(p=q; p; p=p->next) {
			if(t == p->task)
				return(p);
			}
		return(NULL);
		}
/*------------------------------------------------------------------*/
void	*help(char *p)
		{
		return(NULL);
		}
/*------------------------------------------------------------------*/
void	Lobatt(void)
		{
		warn(W_LOW_BATT);
		if(!BatteryOK()) {
 			if(!CheckTask(power_off,RTC_task))
				SetTask(power_off,300,RTC_task);
			SetTask(Lobatt,10,RTC_task);
			}
		else{
			if(!pwoff)
				DeleteTask(power_off,RTC_task);
			DeleteTask(Lobatt,RTC_task);
			}
		}
/*------------------------------------------------------------------*/
void	power_off(void)
{
	Key=ONOFF;
}
/*------------------------------------------------------------------*/
int		puts_lcd(char *c)
{
	char *p = c;
	while(*p)
		Putch(*p++,CHLCD);
	return(0);
}
/*------------------------------------------------------------------*/
/* Izracuna teden iz strukture t									*/
/* referencni teden #0 se zacne 6 jan. 1980 GPS ref. page 17		*/
/*																	*/
double	calc_week(rtime *t)
		{
		int 	i,monthdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		double	d;
		int		y=1900 + t->year;

		if(y < 1980)
			y += 100;

		for(i=1980, d=0.0; i<y; ++i)
			if ( !(i % 4) && (i % 100) || !(i % 400) )
				d += 366.0;
			else
				d += 365.0;

		i = y;
		if ( !(i % 4) && (i % 100) || !(i % 400))
			monthdays[1] = 29;

		for (i = 0; i < t->month; i++)
			d += monthdays[i];
		d += (t->date - 5.0);
		t->day=fmod(d,7.0);
		return(floor(d/7.0));
		}
/*------------------------------------------------------------------*/
/* izracuna sekunde iz strukture t									*/
/* uporablja parameter t->day, ki se mora predhodno nastaviti s		*/
/* klicem calc_week													*/
double	calc_secs(rtime *t)
		{
		return(t->day * 86400.0 +
			t->hour * 3600.0 +
				t->min * 60.0 +
					t->sec);
		}
/*------------------------------------------------------------------*/
void 	calc_rtime (double wk,double sec,rtime *t)
		{
		int 	i,j;
		double	days;

		int monthdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		double ws = 7.0*24.0*3600.0;

		while(sec < 0)	 {
			sec += ws;
			--wk;
			}
		while(sec >= ws) {
			++wk;
			sec -= ws;;
			}

		t->day 	=  floor(sec/86400.0);
		sec 	-= (t->day * 86400.0);

		days = wk * 7.0 + t->day + 5.0;
		for(i=0; days >= 0; ++i) {
			j = 1980 + i/12;
			if ( (j % 4) == 0 && (j % 100) != 0 || (j % 400) == 0 )
				monthdays[1] = 29;
			else
				monthdays[1] = 28;
			days -= monthdays[i%12];
			}
		days += monthdays[--i%12];
		t->year	= (80 + i/12) % 100;
		t->month= i % 12;
		t->date = days;
		t->hour =  floor(sec / 3600.0);
		sec 	-= (t->hour * 3600.0);
		t->min	=  floor(sec / 60.0);
		sec		-= t->min * 60;
		t->sec	=  floor(sec + 0.5);
		}
/*------------------------------------------------------------------*/
size_t	put_byte(char **dest, void *src, size_t n)
		{
		int	i;
		char *p = *dest;
		char *q = src;

		for(i=0; i<n; ++i)
			*p++ = *q++;
		*dest=p;
		return(n);
		}
/*-----------------------------------------------------------------*/
char 	*get_byte(void *dest, char *src, size_t n)
		{
		int	i;
		char *p = dest;
		char *q = src;

		for(i=0; i<n; ++i)
			*p++ = *q++;
		return(q);
		}
/*------------------------------------------------------------------*/
void	err(char *p)
		{
		char c[64];
		sprintf(c,"%s%c%s",ERROR_P,':',p);
/*		WriteSophie(c);
*/		sprintf(error_screen[1],"%20s",p);
		sprintf(error_screen[0],"%20s",ERROR_P);
	 	FlipMessage();
		}
/*------------------------------------------------------------------*/
void	warn(char *p)
		{
		char c[64];
		sprintf(c,"%s%c%s",WARNING_P,':',p);
/*		WriteSophie(c);
*/		sprintf(error_screen[1],"%20s",p);
		sprintf(error_screen[0],"%20s",WARNING_P);
	 	FlipMessage();
		}
/*------------------------------------------------------------------*/
void	mess(char *p)
		{
/*		WriteSophie(p);
*/		sprintf(error_screen[0],"%20s",p);
		sprintf(error_screen[1],"%20s"," ");
	 	FlipMessage();
	 	}
/*------------------------------------------------------------------*/
void	FlipMessage(void)
		{
		beep(100);
		sprintf(error_screen[2],"%20s"," ");
		sprintf(error_screen[3],"%20s"," ");
		screen_swap(SWAP_T);
		}
/*-----------------------------------------------------------------*/
int		sign(int i)
		{
		if(i)
			return(i/abs(i));
		else
			return(0);
		}
/*-----------------------------------------------------------------*/
long		lsign(long i)
			{
			if(i)
				return(i/labs(i));
			else
				return(0L);
			}
/*------------------------------------------------------------------*/
double		fsign(double i)
			{
			if(i)
				return(i/fabs(i));
			else
				return(0.0);
			}
/*------------------------------------------------------------------*/
int         Pmode,Org,North;    /*                                  */
/*..................................................................*/
int			PushMode(void *vp)  /*                                  */
			{                   /*                                  */
			Pmode=d_mode;      	/* save coordinates mode			*/
			if(ORG==OBP)        /* eof if local mode				*/
				Org=eof;        /*                                  */
			else
				Org=LinkAddr(ORG);
			North=LinkAddr(NORTH);
			return(null);
			}
/*..................................................................*/
int			PullMode(void *vp)
			{
			d_mode=Pmode;
			if(Org==eof)
				ORG=OBP;
			else
				ORG=FindLib(Org);
			NORTH=FindLib(North);
			return(null);
			}
/*--------------------------------------------------------*/
/* vklop knjiznice in izpis, ce je LL predhodno prazen    */
/*--------------------------------------------------------*/
/* objekt je prazen => display Ln						  */
/* poseben objekt	=> poisci tip RP					  */
/* objekt je v knjiznici => prepisi LL					  */

int		library_on(void *vp)
		{
		lib **p=vp;
		if(!*p) {
			freelibrary(LL);
			LL=makelib();
			}
		else{
			if(checklib(*p,LL->ltype,LL->n)) {
				*p=checklib(*p,LL->ltype,LL->n);
				copylib(*p,LL);
                }
			else{
				if(checktype(LL,".r") || checktype(LL,".b"))
					if(findtype(*p,TYPE_RP))
						*p=findtype(*p,TYPE_RP);
				if(findtype(*p,LL->ltype->t))
					*p=findtype(*p,LL->ltype->t);
				if(!LL->ltype)
					copylib(*p,LL);
				}
			}
		Displc(LL);
		return(null);
		}
/*--------------------------------------------------------*/
/* prepis iz *vp v LL in izpis	                          */
/*--------------------------------------------------------*/
int		toLL(void *vp)
		{
		lib **p=vp;
		if(*p) {
			copylib(*p,LL);
			Displc(LL);
			}
		else
			warn(W_LIB_EMPTY);
		return(null);
		}
/*--------------------------------------------------------*/
/* prepis koordinat iz *vp v LL in izpis                  */
/*--------------------------------------------------------*/
int		XtoLL(void *vp)
		{
		lib **p=vp;
		if(!*p) {
			freelibrary(LL);
			LL=makelib();
			}
		else
			copycoord(*p,LL);
		Displc(LL);
		return(null);
		}
/*--------------------------------------------------------*/
/* prepis v *vp iz LL			                          */
/*--------------------------------------------------------*/
int		fromLL(void *vp)
		{
		lib **p=vp;
		if(!*p)
			*p=makelib();
		copylib(LL,*p);
		return(null);
		}
/*--------------------------------------------------------*/
int		NumObj(void *vp)
		{
		int	i;
        lib	*l;
		if(Kbhit(CHLCD)) {
			i = abs(ReadInt(LL->n)) % 100;
			if(!LL || !LL->ltype)
				return(null);
			LL->n = i;
            l=checklib(Ln,LL->ltype,LL->n);
            if(l)
            	copylib(l,LL);
			Displc(LL);
			}
		return(null);
		}
/*--------------------------------------------------------*/
int		InitObject(void *vp)
		{
		if(!LL->ltype)
			return(null);
		LL->x=LL->xc=freecoord(LL->x);
		return(auto_ntyp(NULL));
		}
/*--------------------------------------------------------*/
/* poveca vrednost oznake tipa na prvo prosto vrednost !  */
/*--------------------------------------------------------*/
int		auto_ntyp(void *vp)
		{
		int	i;
		lib	**l=vp;
		if(!l)
			l=&LL;
		for(i=1;i<100;++i)
			{
			if(!checklib(Ln,(*l)->ltype,i)) {
				(*l)->n=i;
				if(l == &LL)
					Displc(LL);
				return(null);
				}
			}
		 err(E_MAX_VAL);
		 return(null);
		 }
/*-------------------------------------------------------------*/
struct 	 ParList	{void *a;int s;char *n;char t;} ParList[] =
		 {
		 {ser_no,-8*sizeof(char),"ser_no",0},
		 {syspass,-8*sizeof(char),NULL,0},
		 {&p_null,sizeof(double),"p_null",0},
		 {&r_null,sizeof(double),"r_null",0},
		 {&p_scale,sizeof(double),"p_scale",0},
		 {&r_scale,sizeof(double),"r_scale",0},
		 {&cpr,sizeof(double),"cpr",0},
		 {&crp,sizeof(double),"crp",0},
		 {&Pscale,sizeof(double),"Pscale",0},
		 {&Pnull,sizeof(double),"Pnull",0},
		 {&aC100,sizeof(double),"aC100",0},
		 {&bC100,sizeof(double),"bC100",0},
		 {&cC100,sizeof(double),"cC100",0},
		 {&tn_min,sizeof(int),"tn_min",0},
		 {&tn_max,sizeof(int),"tn_max",0},
		 {&t_ref,sizeof(double),"t_ref",0},
		 {&key_delay,sizeof(int),"key_delay",0},
		 {&shaft,sizeof(int),"shaft",0},
		 {&GPS_datum,sizeof(int),"GPS_datum",0},
		 {&GPS_geo,sizeof(int),"GPS_geo",0},
		 {&GPS_dx,sizeof(int),"GPS_dx",0},
		 {&GPS_dy,sizeof(int),"GPS_dy",0},
		 {&GPS_dz,sizeof(int),"GPS_dz",0},
		 {&CM_off,sizeof(int),"CM_off",0},
		 {&CM_sect,sizeof(int),"CM_sect",0},
		 {&el_null,sizeof(int),"el_null",0},
		 {&aux_mode,sizeof(int),"aux_mode",0},
		 {&rld_mode,sizeof(int),"rld_mode",0},
		 {&swvers,sizeof(int),"swvers",0},
		 {&dGPS,sizeof(int),"dGPS",0},
		 {&GPS_pdop,sizeof(int),"GPS_pdop",0},
		 {&FalseNorth,sizeof(long),"FalseNorth",0},
		 {&FalseEast,sizeof(long),"FalseEast",0},
		 {&language,sizeof(int),"language",0},
		 {&coordtx,sizeof(int),"coordtx",0},
		 {&RightToLeft,sizeof(int),"RightToLeft",0},
 		 {&az_align,sizeof(int),"az_align",0},
		 {&el_align,sizeof(int),"el_align",0},
		 {&Rscale,sizeof(double),"Rscale",0},
		 {&Rnull,sizeof(double),"Rnull",0},

		 {&DSPgain[0],sizeof(int),"VOLmic",1},
		 {&DSPgain[1],sizeof(int),"VOLhead",1},
		 {&DSPgain[2],sizeof(int),"VOLin",1},
		 {&DSPgain[3],sizeof(int),"VOLout",1},
		 {&C100_decl,sizeof(int),"C100_decl",1},
		 {&meteo_A,sizeof(int),"meteo_A",1},
		 {&diff_GPS,sizeof(int),"diff_GPS",1},
		 {userpass,-8*sizeof(char),NULL,1},
		 {meteo,64*sizeof(int),NULL,1},
		 {&track_delay,sizeof(int),"track_delay",1},
		 {&pwoff,sizeof(int),"pwoff",1},
		 {&GPS_dt,sizeof(int),"GPS_dt",1},
		 {&lcd,sizeof(int),"lcd",1},
		 {&Backlit,sizeof(int),"backlit",1},

		 {&maxmils,sizeof(int),"maxmils",2},
		 {&pitch,sizeof(double),"pitch",2},
		 {&roll,sizeof(double),"roll",2},
		 {&az_off,sizeof(double),"az_off",2},
		 {NULL,null,NULL,null}
		 };
/*-------------------------------------------------------------*/
uchar	*SavePar(uchar *f,int t)
		{
struct	ParList *p;

		f=prog_byte(&t,f,sizeof(int));
		for(p=ParList; p->a; ++p)
			if(p->t == t)
				f=prog_byte(p->a,f,abs(p->s));
		return(f);
		}
/*-------------------------------------------------------------*/
uchar	*LoadPar(uchar *f, int t)
		{
struct	ParList *p;
int		i;
		f=load_byte(&i,f,sizeof(int));
		if(i != -1)
			for(p=ParList; p->a; ++p)
				if(p->t == t)
					f=load_byte(p->a,f,abs(p->s));
		return(f);
		}

/*--------------------------------------------------------------*/
uchar	*LoadFonts(uchar *f)
{
		int		i,j=0;

		f=load_byte(&i,f,sizeof(int));
		while(i != -1)
		{
			_MenuCode[j]=memalloc((i+1)*sizeof(char));
			_MenuCode[j][i]=0;			
			f=load_byte(_MenuCode[j++],f,i);
			f=load_byte(&i,f,sizeof(int));
		}

		j=0;
		f=load_byte(&i,f,sizeof(int));
		while(i != -1)
		{
			_MenuHead[j]=memalloc((i+1)*sizeof(char));
			_MenuHead[j][i]=0;			
			f=load_byte(_MenuHead[j++],f,i);
			f=load_byte(&i,f,sizeof(int));
		}

		j=0;
		f=load_byte(&i,f,sizeof(int));
		while(i != -1)
		{
			_TextCode[j]=memalloc((i+1)*sizeof(char));
			_TextCode[j][i]=0;			
			f=load_byte(_TextCode[j++],f,i);
			f=load_byte(&i,f,sizeof(int));
		}
		return(f);
}/*--------------------------------------------------------------*/
uchar	*SaveFonts(uchar *f)
		{
		int	i,j;

		for(i=0; _MenuCode[i]; ++i)
		{
			j=strlen(_MenuCode[i]);
			f=prog_byte(&j,f,sizeof(int));
			f=prog_byte(_MenuCode[i],f,j);
		}
		j=-1;
		f=prog_byte(&j,f,sizeof(int));
		for(i=0; _MenuHead[i]; ++i)
		{
			j=strlen(_MenuHead[i]);
			f=prog_byte(&j,f,sizeof(int));
			f=prog_byte(_MenuHead[i],f,j);
		}
		j=-1;
		f=prog_byte(&j,f,sizeof(int));
		for(i=0; _TextCode[i]; ++i)
		{
			j=strlen(_TextCode[i]);
			f=prog_byte(&j,f,sizeof(int));
			f=prog_byte(_TextCode[i],f,j);
		}
		j=-1;
		f=prog_byte(&j,f,sizeof(int));
		return(f);
		}
/*--------------------------------------------------------------*/
void	ListPar(void)
		{
struct 	ParList *p;
		char c[32];
		int		*i;
		long	*j;
		double	*d;
		char	*s;

		for(p=ParList; p->a; ++p)
			if(p->n)	{
				switch(p->s) {
					case 2:	i=p->a;
							sprintf(c,"=f %s=%d\r\n",p->n,*i);
							break;
					case 4:	j=p->a;
							sprintf(c,"=f %s=%ld\r\n",p->n,*j);
							break;
					case 8:	d=p->a;
							sprintf(c,"=f %s=%+6le\r\n",p->n,*d);
							break;
					default:s=p->a;
							sprintf(c,"=f %s=%s\r\n",p->n,s);
							break;
					}
				puts_pc(c);
				}
		}
/*------------------------------------------------------------------*/
int	ReadPar(char *c)
{
	struct 	ParList *p;
	int		*i;
	long	*j;
	double	*d;
	char	*s,cc[64],*ccc;

	strcpy(cc,++c);
	ccc=strchr(cc,'=');
	if(!ccc)
		return(null);
	*ccc=0;
	++ccc;
	for(p=ParList; p->a; ++p)
		if(p->n)
			if(!strcmp(cc,p->n))
			{
				switch(p->s)
				{
				case 2:
					i=p->a;
					*i=atoi(ccc);
					break;
				case 4:
					j=p->a;
					*j=atol(ccc);
					break;
				case 8:
					d=p->a;
					*d=atof(ccc);
					break;
				default:
					s=p->a;
					strcpy(s,ccc);
					break;
				}
				return(eof);
			}
			return(null);
}
/*------------------------------------------------------------------*/
/* Vpis na RTC RAM; parametri se nahajajo na lokacijah 8-30			*/
/* lokacije 0-7 so rezervirane za funkcijo Submit 					*/
/* lokacija 31 -> checksum											*/
/* !!! sizeof(double) je predviden za kote 0-2pi in se spremeni v	*/
/*     sizeof(int) s predhodnim mnozenjem z 10000 !					*/
int		shutdown_save(void *vp)
		{
		int i,j;
struct 	ParList *p;
		for(i=8,p=ParList; p->a; ++p)
			if(p->t == 2)
				switch(p->s) {
					case 2:	j = *(int *)p->a;
							writeRTC(i++, j >> 8);
							writeRTC(i++, j & 0xff);
							break;
					case 8: j = (*(double *)p->a * 10000.0) + 0.5;
							writeRTC(i++, j >> 8);
							writeRTC(i++, j & 0xff);
							break;
					}
		for(i=j=0; i<31; ++i)
			j = Crc(j,readRTC(i));
		writeRTC(i,j);
		return(null);
		}
/*------------------------------------------------------------------*/
int		shutdown_load(void *vp)
		{
struct 	ParList *p;
		int 	i,j;
		for(i=j=0; i<31; ++i)
			j = Crc(j,readRTC(i));
		if(readRTC(i) != (j & 0xff))
			return(eof);
			for(i=8,p=ParList; p->a; ++p) {
			if(p->t == 2) {
				j = (readRTC(i) << 8) + readRTC(i+1);
				switch(p->s) {
					case 2: *(int *)p->a = j;
							break;
					case 8:	*(double *)p->a = (double)j/10000.0;
							break;
					}
				++i;++i;
				}
			}
		return(null);
		}
/*------------------------------------------------------------------*/
int		GetKey(void)
{
		int		i;

		do
		{
			if(!Key)
			{
				watchdog();
				RTCservice();
			}
			switch(Key)
			{
				case ONOFF:
					SetTask(Shutdown,0,RTC_task);
					break;
				case BACKLIT:
					Key=0;
					do	
					{
						if(!Key)
						{
							watchdog();
							RTCservice();
						}
						switch(Key)
						{
						case UP:
							BacklitOn();
							Key=eof;
							break;
						case DOWN:
							BacklitOff();
							Key=eof;
							break;
						case LEFT:
							DspGain(HEAD,-1);
							Key=eof;
							break;
						case RIGHT:
							DspGain(HEAD,1);
							Key=eof;
							break;
						case ALPHA:	
							screen_swap(3*SWAP_T);
							WriteSophie(SophieBuff);
							Key=eof;
							break;
						case BACKLIT:	
							if(Tamb < 0)
							{
								HeatOn();
								SetTask(HeatOff,-5*(int)Tamb,RTC_task);
							}
							Key=eof;
							break;
						case CLEAR:
							Key=SHIFTR;
							break;
						case SIGN:
							if(RightToLeft)
								RightToLeft=0;
							else
								RightToLeft=1;
							Ungetch(REFRESH,1);
							Key=eof;
							break;
						default:
							if(isdigit(Key))
							{
								language=Key-'0';
								Ungetch(REFRESH,0);
								Key=eof;
							}
						}
					}	while(!Key);
					break;
					case LCD_U:
						lcd=LcdLevel(++lcd);
						Key=eof;
						break;
					case LCD_D:
						lcd=LcdLevel(--lcd);
						Key=eof;
						break;
			}
		} while(!Key);
		DeleteTask(FlipMessage,RTC_task);
		i = Key;
		Key = null;
		return(i);
}
/*------------------------------------------------------*/
int		shaft_ready(void)
		{
		if((shaft & 0x7f) == NOSHAFT)
			return(eof);
		if(shaft & 0x80)
		{
			return(null);
		}
		else
			return(eof);
		}
/*------------------------------------------------------*/
/* driver za enkoder; zagon v remmain.c s SetTask(..PIT	*/
/* procedura starta z vpisom  MSB v shaft register		*/
/* shaft_enable; pri zagonu potrebno nastaviti tudi 	*/
/* stevilo bitov -> SHtoR = 2.0*M_PI/pow(2,n)			*/
/*														*/
/* shaft_ITEK16uS(void) => Itek 16uS, 14-bit			*/
/* shaft_ROC415(void)	=> Heidenhain ROC415,15-bit,bin */
/* shaft_ROC415G(void)	=> Heidenhain ROC415,15-bit,gray*/
/*------------------------------------------------------*/
void	shaft_enable(void)
{
	switch(shaft & 0x7f)
	{
	case NOSHAFT:
		shaft_data[SHAFT_EL]=0;
		shaft_data[SHAFT_AZ]=0;
		SHtoR = 2.0*M_PI/0x8000;
		break;
	case ROC_G:
		if(!CheckTask(shaft_ROC415G,PIT_task))
		{
			SHtoR = 2.0*M_PI/0x8000;
			DeleteTask(shaft_ROC415,PIT_task);
			SetTask(shaft_ROC415G,3,PIT_task);
		}
		break;
	case ROC:
	default:
		shaft=ROC;
		if(!CheckTask(shaft_ROC415,PIT_task))
		{
			SHtoR = 2.0*M_PI/0x8000;
			DeleteTask(shaft_ROC415G,PIT_task);
			SetTask(shaft_ROC415,20,PIT_task);
		}
		break;
	}
	shaft |= 0x80;
}
/*------------------------------------------------------------------*/
/* Rutina podstavi znak z zakasnitvijo n							*/
/* PIT interrupt period; ekvivalent => ungetch !!!					*/
/*------------------------------------------------------------------*/
int		Ungetch(int c,int n)
		{
		if(!n)
			Key=c;
		else {
			PushCount = n;
			PushKey=c;
			}
		if(CheckTask(RefreshMenu,RTC_task))
			SetTask(RefreshMenu,n/50+1,RTC_task);
        return(c);
		}
/*------------------------------------------------------------------*/
void	*memalloc(size_t n)
		{
		void *ralloc(size_t);
		void *p=ralloc(n);
        char *q=p;
		if(!p) {
	        clrlcd();
    	    xyLCD(0,0);
			puts_lcd("out of memory");
			while(1);
			}
        else
        	while(n--)
            	*q++=0;
        return(p);
		}
/*------------------------------------------------------------------*/
void	memfree(void *p)
		{
		void rfree(void *);
		rfree(p);
		}
/*------------------------------------------------------------------*/
/* PIT interrupt (50 msec)											*/
/*																	*/
void IRQservice(void)
		{
		queue	*p;

		for(p=PIT_task; p; p=p->next)
			if(p->task && !(--(p->count)))
			{
				p->count = p->rate;
				p->task();
			}

		temp += GetPeriod()-(temp >> 5);

		if(wait_count)
			--wait_count;
		}
/*------------------------------------------------------------------*/
/*	RTC queue, v zanki Kbhit !!! Ce je task.rate == 0, se klice		*/
/*	se v vsakem obhodu, sicer na vsakih N sec !!!					*/
/*																	*/
void	RTCservice(void)
		{
		int		i;
		queue	*p;

		while(Kbhit(CHC100))
			Getch(CHC100);
		while(Kbhit(CHGPS))
			Getch(CHGPS);
		while(Kbhit(CHRLD))
			Getch(CHRLD);
		while(Kbhit(CHIR))
			Getch(CHIR);
/***	fast queue service						***/
/***--------------------------------------------***/
		for(p=RTC_task; p; p=p->next)
			if(p->task && !p->rate)
				p->task();
/***	RTC 1Hz service							***/
/***--------------------------------------------***/
		i=readRTC(0x30);
		if(i & 0x02)
			beep(100);		/*** beep, if alarm ***/
		if(i & 0x01) {
			for(p=RTC_task; p; p=p->next)
				if(p->task && p->rate && !(--(p->count))) {
					p->count = p->rate;
					p->task();
					}
			Tamb=getT();
			ReadTime(&Time);
			LcdLevel(lcd);
			}
/***	low battery service						***/
/***--------------------------------------------***/
		if(!BatteryOK()) {
			if(!CheckTask(Lobatt,RTC_task))
				SetTask(Lobatt,10,RTC_task);
			}
/***	countdown service						***/
/***--------------------------------------------***/
		if(Kbhit(CHPC) || Kbhit(CHLCD) || !CheckTask(power_off,RTC_task)) {
			if(pwoff > 10)
				SetTask(power_off,pwoff,RTC_task);
			else
				if(pwoff)
					SetTask(power_off,10,RTC_task);
			}
		}
/*------------------------------------------------------------------*/
void	wait(int	i)
		{
		wait_count=i;
		while(wait_count) {
			watchdog();
			RTCservice();
			}
		}
/*------------------------------------------------------------------*/
void	ReadTime(rtime *t)
		{
		t->sec	=fromBCD(readRTC(0x20));
		t->min	=fromBCD(readRTC(0x21));
		t->hour	=fromBCD(readRTC(0x22));

		t->day	=(fromBCD(readRTC(0x23))-1) % 7;
		t->date	=fromBCD(readRTC(0x24))-1;
		t->month=(fromBCD(readRTC(0x25))-1) % 12;
		t->year	=fromBCD(readRTC(0x26));
		}
/*------------------------------------------------------------------*/
void	WriteTime(rtime *t)
		{
		writeRTC(0x22,toBCD(t->hour));
		writeRTC(0x21,toBCD(t->min ));
		writeRTC(0x20,toBCD(t->sec ));

		writeRTC(0x23,toBCD((t->day  )+1));
		writeRTC(0x24,toBCD((t->date )+1));
		writeRTC(0x25,toBCD((t->month)+1));
		writeRTC(0x26,toBCD(t->year));
		}
/*------------------------------------------------------------------*/
void	AlarmEnable(rtime *t)
		{
		writeRTC(0x2A,toBCD(t->hour));
		writeRTC(0x29,toBCD(t->min ));
		writeRTC(0x28,toBCD(t->sec ));
		writeRTC(0x32,readRTC(0x32) | 0x10);
		}
/*------------------------------------------------------------------*/
void	AlarmDisable(void)
		{
		writeRTC(0x32,readRTC(0x32) & ~0x10);
		}
/*------------------------------------------------------------------*/
char	*GetMessCode(int n)
{
static	char	q[8][128];
static	int		qq=0;
		int		i;
		char	*p=_TextCode[n];

		for(i=language; i > 0; --i)
		{
			if(strchr(p,'|'))
			{
				p=strchr(p,'|');
				++p;
			}
		}
		if(strchr(p,'|'))
		{
			*strchr(p,'|')=0;
			strcpy(q[qq],p);
			*strchr(p,0)='|';
		}
		else
			strcpy(q[qq],p);
			p=q[qq++];
			qq=qq%8;
		return(p);
}
/*------------------------------------------------------------------*/
void	RemoveString(char *s[])
{
	int		i,j;
	char	*p,*q;
	for(i=0; s[i]; ++i)
	{
		p=s[i];
		for(j=language; *p && j > 0; --j)
			if(strchr(p,'|'))
			{
				p=strchr(p,'|');
				++p;
			}
			else
				p=strchr(p,0);
		if(*p)
		{
			q=strchr(p,'|');
			if(q)
				strcpy(p,++q);
			else
				if(p != s[i])
					*--p=0;
		}
	}
}
/*------------------------------------------------------------------*/
int		SetLanguage(void *vp)
{
		if(Kbhit(CHLCD))
		{
			switch(Getch(CHLCD)) {
				case RIGHT:	language = ++language % 8;
							Ungetch(REFRESH,1);
							break;
				case LEFT:	if(language)
								--language;
							Ungetch(REFRESH,1);
							break;
				case CLEAR:	RemoveString(_TextCode);
							RemoveString(_MenuCode);
							RemoveString(_MenuHead);
							Ungetch(REFRESH,1);
							break;
				case SIGN:	if(RightToLeft)
								RightToLeft=0;
							else
								RightToLeft=1;
							Ungetch(REFRESH,1);
							break;
			}
		}
		return(null);
}
#ifdef	WIN32

float	getfloat (char *bp)
{
	uval	u;
	u.bd[3] = *bp++;
	u.bd[2] = *bp++;
	u.bd[1] = *bp++;
	u.bd[0] = *bp;
	return(u.fv);
}
/*------------------------------------------------------------------*/
double	getdouble (char *bp)
	{
	uval	u;
	u.bd[7] = *bp++;
	u.bd[6] = *bp++;
	u.bd[5] = *bp++;
	u.bd[4] = *bp++;
	u.bd[3] = *bp++;
	u.bd[2] = *bp++;
	u.bd[1] = *bp++;
	u.bd[0] = *bp;
	return(u.dv);
	}
/*------------------------------------------------------------------*/
int	getint (char *bp)
	{
	uval u;
	u.bd[1] = *bp++;
	u.bd[0] = *bp;
	return(u.sv);
	}
/*------------------------------------------------------------------*/
long getlong (char *bp)
	{
	uval u;
	u.bd[3] = *bp++;
	u.bd[2] = *bp++;	
	u.bd[1] = *bp++;
	u.bd[0] = *bp;
	return(u.lv);
	}
#else
/*------------------------------------------------------------------*/
float	getfloat (char *bp)
{
	uval	u;
	u.bd[0] = *bp++;
	u.bd[1] = *bp++;
	u.bd[2] = *bp++;
	u.bd[3] = *bp;
	return(u.fv);
}
/*------------------------------------------------------------------*/
double	getdouble (char *bp)
	{
	uval	u;
	u.bd[0] = *bp++;
	u.bd[1] = *bp++;
	u.bd[2] = *bp++;
	u.bd[3] = *bp++;
	u.bd[4] = *bp++;
	u.bd[5] = *bp++;
	u.bd[6] = *bp++;
	u.bd[7] = *bp;
	return(u.dv);
	}
/*------------------------------------------------------------------*/
int	getint (char *bp)
	{
	uval u;
	u.bd[0] = *bp++;
	u.bd[1] = *bp;
	return(u.iv);
	}
/*------------------------------------------------------------------*/
long getlong (char *bp)
	{
	uval u;
	u.bd[0] = *bp++;
	u.bd[1] = *bp++;	
	u.bd[2] = *bp++;
	u.bd[3] = *bp;
	return(u.lv);
	}
#endif

/*------------------------------------------------------------------*/
char	*get_float(float *f,char *p)
{
	*f=getfloat(p);
	return(p += sizeof(float));
}

char	*get_double(double *f,char *p)
{
	*f=getdouble(p);
	return(p += sizeof(double));
}

char	*get_long(long *f,char *p)
{
	*f=getlong(p);
	return(p += sizeof(long));
}

char	*get_int(int *f,char *p)
{
	*f=getint(p++);
	return(++p);
}

char	*get_char(char *f,char *p)
{
	*f=*p++;
	return(p);
}
#ifdef WIN32
/*------------------------------------------------------------------*/
int	put_double (char **bp, double f)
	{
	uval	u;
	u.dv = f;
	*(*bp)++ = u.bd[7];
	*(*bp)++ = u.bd[6];
	*(*bp)++ = u.bd[5];
	*(*bp)++ = u.bd[4];
	*(*bp)++ = u.bd[3];
	*(*bp)++ = u.bd[2];
	*(*bp)++ = u.bd[1];
	*(*bp)++ = u.bd[0];
	return(8);
	}
/*------------------------------------------------------------------*/
int	put_float (char **bp, float f)
	{
	uval	u;
	u.fv = f;
	*(*bp)++ = u.bd[3];
	*(*bp)++ = u.bd[2];
	*(*bp)++ = u.bd[1];
	*(*bp)++ = u.bd[0];
	return(4);
	}
/*------------------------------------------------------------------*/
int	put_long (char **bp, long f)
	{
	uval	u;
	u.lv = f;
	*(*bp)++ = u.bd[3];
	*(*bp)++ = u.bd[2];
	*(*bp)++ = u.bd[1];
	*(*bp)++ = u.bd[0];
	return(4);
	}
/*------------------------------------------------------------------*/
int	put_int (char **bp, int f)
	{
	uval	u;
	u.sv = f;
	*(*bp)++ = u.bd[1];
	*(*bp)++ = u.bd[0];
	return(2);
	}
/*------------------------------------------------------------------*/
int	put_char (char **bp, char f)
	{
	*(*bp)++=f;
	return(1);
	}
#else
/*------------------------------------------------------------------*/
int	put_double (char **bp, double f)
	{
	uval	u;
	u.dv = f;
	*(*bp)++ = u.bd[0];
	*(*bp)++ = u.bd[1];
	*(*bp)++ = u.bd[2];
	*(*bp)++ = u.bd[3];
	*(*bp)++ = u.bd[4];
	*(*bp)++ = u.bd[5];
	*(*bp)++ = u.bd[6];
	*(*bp)++ = u.bd[7];
	return(8);
	}
/*------------------------------------------------------------------*/
int	put_float (char **bp, float f)
	{
	uval	u;
	u.fv = f;
	*(*bp)++ = u.bd[0];
	*(*bp)++ = u.bd[1];
	*(*bp)++ = u.bd[2];
	*(*bp)++ = u.bd[3];
	return(4);
	}
/*------------------------------------------------------------------*/
int	put_long (char **bp, long f)
	{
	uval	u;
	u.lv = f;
	*(*bp)++ = u.bd[0];
	*(*bp)++ = u.bd[1];
	*(*bp)++ = u.bd[2];
	*(*bp)++ = u.bd[3];
	return(4);
	}
/*------------------------------------------------------------------*/
int	put_int (char **bp, int f)
	{
	uval	u;
	u.iv = f;
	*(*bp)++ = u.bd[0];
	*(*bp)++ = u.bd[1];
	return(2);
	}
/*------------------------------------------------------------------*/
int	put_char (char **bp, char f)
	{
	*(*bp)++=f;
	return(1);
	}
#endif