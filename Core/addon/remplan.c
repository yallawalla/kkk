#include "remmenu.h"
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
		Point-to-Multiple messages
		Multiple response expected
		CTRL_F_UNM block call, timed CTRL_F_UNM answer
อออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Call(void *vp) {
		command **cp = vp;

		switch((*cp)->opt[0]) {
		case 0:
		case 1: return(CommandCall(vp));
		case 2:	return(CommandCall(vp));
		}
	return(null);
}
/*อออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CommandCall(void *vp)
		{
		char	c[LINK_LEN],*p=c;
		int		i,j;
		lib		*tg;
		command **cp = vp;
		long	dt;
		
		if(!*cp) {
			err(E_EMPTY_LIST);
			return(null);
			}
		if(!(*cp)->to) {
			err(E_NO_UNIT);
			return(null);
			}
		if((*cp)->to == LinkAddr(OBP)) {
			err(E_NO_UNIT);
			return(null);
			}
		tg=FindLib((*cp)->tg);
		if(!tg || !tg->xc) {
			err(E_NO_TARGET);
			return(null);
			}
/*อออ  vpis prenosne sekvence v *p    ออออออออออออออออออออออ*/
		*p++ = COMM_REQ;
		j = sizeof(char);
/*อออ  stevilska oznaka sekvence      ออออออออออออออออออออออ*/
		j += put_int(&p,(*cp)->n);
		j += put_char(&p,(*cp)->opt[0]);
/*อออ  korekcijski faktorji           ออออออออออออออออออออออ*/
		j += put_double(&p,(*cp)->corrx);
		j += put_double(&p,(*cp)->corry);
		j += put_double(&p,(*cp)->corrz);
/*อออ parametri ukaza                  อออออออออออออออออออออ*/
		j += put_char(&p,(*cp)->effect);
		j += put_char(&p,(*cp)->fmode);
		j += put_char(&p,(*cp)->rounds);
		j += put_char(&p,(*cp)->interval);
		j += put_char(&p,(*cp)->section);
		j += put_char(&p,(*cp)->ammo);
		j += put_char(&p,(*cp)->traj);
		j += put_char(&p,(*cp)->charge);
		j += put_char(&p,(*cp)->fuze);

		dt=dtime(&(*cp)->ftime,&Time);
		j += put_char(&p,(*cp)->tmode);
		j += put_long(&p,dt);
/*อออ target data                      อออออออออออออออออออออ*/	
		j +=tx_lib(&p,tg);
		j += put_rtime(&p,&Time);
		RFCall((*cp)->to,c,j);
/*อออ command status                   อออออออออออออออออออออ*/
		for(i=0;i<MAXGUN;++i)
			(*cp)->txmode[i]=M_UNDEF;
		return(null);
		}
/*อออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CallGun(void *vp)
		{
		command **cp = vp;
		if(!*cp) {
			err(E_EMPTY_LIST);
			return(null);
			}
		(*cp)->tg = (*cp)->to;
		(*cp)->n = null;
		return(CommandCall(vp));
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CommandAbort(void *vp)
		{
		command	**cp = vp;
		char	c[LINK_LEN],*p=c;
		int		i;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(null);
			}
		if(!(*cp)->to) {
			err(E_NO_UNIT);
			return(null);
			}
		if((*cp)->to == LinkAddr(OBP)) {
			err(E_NO_UNIT);
			return(null);
			}

		i=sizeof(char);
		*p++ = ABORT_REQ;
		i += put_int(&p,((*cp)->n));
		RFCall((*cp)->to,c,i);

		for(i=0;i<MAXGUN;++i)
			(*cp)->txmode[i]=M_UNDEF;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UpdateTarget(void *vp)
		{
		char	c[LINK_LEN],*p=c;
		command **cp = vp;
		int		i,j=1;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(null);
			}
		if(!(*cp)->to) {
			err(E_NO_UNIT);
			return(null);
			}
		if((*cp)->to == LinkAddr(OBP)) {
			err(E_NO_UNIT);
			return(null);
			}
		if(!LL || !LL->ltype) {
			err(E_VOID_TYPE);
			return(null);
			}
		LL->x=LL->xc=freecoord(LL->x);
		Displc(LL);
		i=LinkAddr(LL);
		*p++ = TARGT_REQ;
		j += put_int(&p,i);
		RFCall((*cp)->to,c,j);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	UpdateAmm(command *c, int m)
		{
		lib *l=FindLib(c->to);
		int	i;

		if(l && (m == M_ACK)) {
			i = l->ammpcs[c->ammo] & 0x0fff;
			i -= c->rounds;
			if(i < 0)
				i = 0;
			l->ammpcs[c->ammo] &= ~0xfff;
			l->ammpcs[c->ammo] += i;
			}
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UpdateUnit(void *vp)
		{
		char	c[LINK_LEN],*p=c;
		int		i,j=1;

		if(!LL || !LL->ltype) {
			err(E_VOID_TYPE);
			return(null);
			}
		if((LL->ltype == OBP->ltype) && (LL->n == OBP->n)) {
			err(E_NO_UNIT);
			return(null);
			}
		LL->x=LL->xc=freecoord(LL->x);
		for(i=0; i<MAXAMM; ++i)
			LL->ammpcs[i]=0;
		Displc(LL);
		i=LinkAddr(LL);
		*p++ = UNIT_REQ;
		j += put_int(&p,i);
		RFCall(i,c,j);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
	Single Point-to-Point messages
	Single response required
	CTRL_F_NMB block call, timed CTRL_F_ACK answer
	if multiple acknowledgments, redundant are aborted
ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UpdateMeteo(void *vp)
		{
		char c[LINK_LEN],*p=c;
		if(LinkAddrType(meteo_A))
		{
			*p = METEO_REQ;
			RFCall(meteo_A,c,1);
		}
		else
			err(E_NO_STATION);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UpdateRtcm(void *vp)
		{
		char c[LINK_LEN],*p=c;
		if(LinkAddrType(diff_GPS))
		{
			*p = RTCM_REQ;
			RFCall(diff_GPS,c,1);
		}
		else
			err(E_NO_STATION);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ManualAck(int	m, void *vp)
		{
		command **cp = vp;
		char	c[LINK_LEN],*p=c;
		int		i=targets_xn(OBP),j;


		if(!cp)
			cp=&CPEXE;
		if(!(*cp)) {
			err(E_EMPTY_LIST);
			return(null);
			}
		if(!(*cp)->from) {
			err(E_NO_UNIT);
			return(null);
			}
		*p++ = ACK_REQ;
		j=sizeof(char);
		j += put_int(&p,((*cp)->n));
		j += put_int(&p,i);
		j += put_int(&p,m);
		RFCall((*cp)->from,c,j);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Ack(void *vp)
		{
		return(ManualAck(M_ACK,vp));
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Nack (void *vp)
		{
		return(ManualAck(M_NACK,vp));
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		RFCall(int a,char *q,int n)
		{
		int i = SendBlk(a,q,n,CTRL_F_UNM);
		if(i == eof) {
			err(E_LINKBUSY);
			return(null);
			}
		return(eof);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetGun(void *vp)
		{
		char	cc[32];
		static int n,nn;
        command	*c=Mscan->c;
		lib		*l=FindLib(c->to);
		if(l)
		{
			if(Kbhit(CHLCD))
			{
				switch(nn=Getch(CHLCD))
				{
				case LEFT:
					if(l->gun)
					{
						--l->gun;
						c->ammo=0;
					}
					break;

				case RIGHT:
					if(!Guns[++l->gun].name)
						--l->gun;
                    else
						c->ammo=0;
					break;
/* jeba z Tajce */
				case REFRESH:
					break;
#ifndef	WIN32
				case F1:
					sector_erase(FONTS,0);
					fonts=FONTS;
					WriteGunData(Guns,&fonts);
					fonts=SaveZone(SaveFonts(fonts));
					break;
#endif
				case ALPHA:
					n=++n % 5;
				default:
					Ungetch(nn,0);
					switch(n)
					{
					case 0:
						Guns[l->gun].scaleA=ReadInt(Guns[l->gun].scaleA);
						sprintf(cc,"scaleA = %-5d",Guns[l->gun].scaleA);
						break;
					case 1:
						Guns[l->gun].scaleE=ReadInt(Guns[l->gun].scaleE);
						sprintf(cc,"scaleE = %-5d",Guns[l->gun].scaleE);
						break;
					case 2:
						Guns[l->gun].gmode=ReadInt(Guns[l->gun].gmode);
						sprintf(cc," gmode = %-5d",Guns[l->gun].gmode);
						break;
					case 3:
						Guns[l->gun].zeroA=ReadInt(Guns[l->gun].zeroA);
						sprintf(cc," zeroA = %-5d",Guns[l->gun].zeroA);
						break;
					case 4:
						Guns[l->gun].zeroE=ReadInt(Guns[l->gun].zeroE);
						sprintf(cc," zeroE = %-5d",Guns[l->gun].zeroE);
						break;
					}
					screen_swap(250);
					strncpy(LcdBuffer,cc,14);
				}
			}
			msprint(Guns[l->gun].name);
		}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetAmm(void *vp)
{
	command	*c=Mscan->c;
	lib		*l=FindLib(c->to);
	int		i;

	if(Guns && l)
	{
		if(Kbhit(CHLCD))
           	switch(Getch(CHLCD))
			{
       	    case LEFT:
				if(c->ammo)
          			--c->ammo;
                break;
			case RIGHT:
				++c->ammo;
            break;
     	    }
/* preveri, ce je ammo  za podano opcijo (zaradi zamenjave tipa orozja !!! */
		for(i=0; (i < c->ammo) && Guns[l->gun].ammo[i+1]; ++i);
		c->ammo = i;
		msprint(Guns[l->gun].ammo[c->ammo]);
	}
	return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
double	GetRefp(lib *l)
{
	double	d = l->xc->refp;
	gun		*g = &Guns[l->gun];

	d = g->zeroA + g->scaleA * (d - l->refd)/MR10*g->mils;
	d= ScaleAngle(d,g->mils,g->mils,null);
	if(g->gmode & BEAR_DBL)
		d=fmod(d,g->mils/2.0);
	return(d);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetGunref(void *vp)
{
        gun		*g;
		char	c[16];
		coord	*x;
		int		i,j=0;
		double	d;

		command *cp 	=Mscan->c;
		lib 	*ll,*l	=FindLib(cp->to);

		if(l)
		{
			if((l != OBP) && Kbhit(CHLCD))
			{
				switch(j=Getch(CHLCD))
				{
				case LEFT:
					if(l->x != l->xc)
					{
						for(x=l->x; x->next != l->xc; x=x->next);
							l->xc=x;
					}
					break;
				case RIGHT:
					if(l->xc->next)
						l->xc=l->xc->next;
					break;
				}
			}
			g = &Guns[l->gun];
			if(Lr)
			{
/*	Ce sta prisotna veljavna Lr in Lr->x (iz GXM ?), sluzi tocka kot veljavni
	orientir za vse elemente orozja
*/				if(Lr->xc)
				{
					if((checklib(Ln,Lr->ltype,Lr->n)==l) && (l != OBP))
					{
						i=targets_xn(Lr);
						for(l->xc = l->x; --i; l->xc = l->xc->next);
					}
					else
					{
						ll=copylib(Lr,ll=makelib());
						x=l->xc;
						for(l->xc=l->x; l->xc; l->xc=l->xc->next)
						{
							LlaToCorr(Lr,ll,l,NULL,EL_AZ_R);
							l->xc->refp = ScaleAngle(ll->xc->y,maxmils,MR10,eof) + 0.5;
						}
						l->xc=x;
						freelib(ll);
					}
				}
/*	Ce je samo Lr brez Lr->x, se pobere ven samo smer za trenutno
    izbrano orozje
*/
				else
					l->xc->refp = ScaleAngle(Lr->refd+MR10/2,MR10,MR10,eof) + 0.5;
				Lr=freelib(Lr);
			}
/* Rocna modifikacija

	Default je vnos kota orientirja glede na smer cevi in merjeno z
    kotomerom namerilne naprave; postopno racunanje z refd in refp
    je potrebno v izogib overflow pri velikem obsegu +- pi*10000 !

			d = l->xc->refp;
			d = g->zeroA + g->scaleA * (d - l->refd)/MR10*g->mils;
			d = ScaleAngle(d,g->mils,g->mils,null);
*/			d=GetRefp(l);
			if(j)
				edit_pol(&d,g->mils,j,null);
			d = ScaleAngle((d - g->zeroA)*g->scaleA,g->mils,MR10,eof);
			l->xc->refp = ScaleAngle(d + l->refd,MR10,MR10,eof);

/* ponoven izracun, samo za izpis !!!

		 	d = l->xc->refp;
			d = g->zeroA + g->scaleA * (d - l->refd)/MR10*g->mils;
            d = ScaleAngle(d,g->mils,g->mils,null);
*/			sprintf(c,"%.2lf %s %d",d=GetRefp(l)/100.0,GUN_MESS,targets_xn(l));
			strswap(c,"  ");
			translate(c,'.','-');
			msprint(c);
		}
	return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetRefDir(void *vp)
		{
		char	c[16];

		command *cp =Mscan->c;
		lib 	*l	=FindLib(cp->to);
		double	d;
		int i;

		if(l)
		{
			if(Lr && Kbhit(CHLCD))
			{
				if(Lr->xc)
					if((checklib(Ln,Lr->ltype,Lr->n)==l) && (l != OBP))
					{
						i=targets_xn(Lr);
						for(l->xc = l->x; --i; l->xc = l->xc->next);
					}
					else
					{
						LlaToCorr(Lr,Lr,l,NULL,EL_AZ_R);
						l->refd = ScaleAngle(Lr->xc->y,maxmils,MR10,eof);
					}
				else
					l->refd = Lr->refd;
				Lr=freelib(Lr);
				}
			d = ScaleAngle(l->refd,MR10,maxmils,null);
			if(Kbhit(CHLCD))
				edit_pol(&d,maxmils,Getch(CHLCD),null);
			l->refd = ScaleAngle(d,maxmils,MR10,eof);
			sprintf(c,"%.2lf",d/100.0);
			translate(c,'.','-');
			msprint(c);
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetMuzzle(void *vp)
{
		char	c[16];
		coord	*x;
		int		i,j;
		double	d;

		command *cp =Mscan->c;
		lib 	*l	=FindLib(cp->to);

		if(l)
		{
			if((l != OBP) && Kbhit(CHLCD))
			{
				switch(j=Getch(CHLCD))
				{
				case LEFT:
					if(l->x != l->xc)
					{
						for(x=l->x; x->next != l->xc; x=x->next);
							l->xc=x;
					}
					break;
				case RIGHT:
					if(l->xc->next)
						l->xc=l->xc->next;
					break;
				default:	
					Ungetch(j,0);
					break;
				}
			}
			i=targets_xn(l);
			j=l->xc->muzzle;
			j = ReadInt(j);
			if(abs(j) > 500)
			{
				err(E_MAX_VAL);
				j /= 10;
			}
			d=l->xc->muzzle=j;
			sprintf(c,"%.1lf%c %s %d",d/10.0,'%',GUN_MESS,i);
			strswap(c,"%  ");
			msprint(c);
		}
		return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* podatek ->ammo ima 12 LSB=stevilo projektilov			  */
/*                   4  MSB=masna oznaka 0=N, 1-5=+,6-10=-*/
int		SetPcs(void *vp)
		{
		char	c[16];
		int		i,j;
		command *cp =Mscan->c;
		lib 	*l 	=FindLib(cp->to);

		if(l) {
			i=(l->ammpcs[cp->ammo]) & 0xfff;
			j=(l->ammpcs[cp->ammo]) & ~0xfff;
			i=abs(ReadInt(i));
			if(i > 0x0fff) {
				err(E_MAX_VAL);
				i/=10;
				}
			l->ammpcs[cp->ammo]=j | i;
			sprintf(c,"%d",i);
			msprint(c);
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* podatek ->ammo ima 12 LSB=stevilo projektilov			  */
/*                   4  MSB=masna oznaka +/- 5            */
int		SetMass(void *vp)
		{
		char	c[16]="N",*p=c;
		int		i,j,k;

		command *cp =Mscan->c;
		lib 	*l	=FindLib(cp->to);

		if(l) {
			k=cp->ammo;
			i=(l->ammpcs[k]) & 0xfff;
			j=(l->ammpcs[k]);
			j = j>>12;
			IncInt(&j);
			if(j < -5)
				j = -5;
			if(j > 5)
				j =  5;
			l->ammpcs[k]= (j<<12) | i;
			if(j < 0) {
				do
					*p++='-';
						while(++j);
				*p=0;
				}
			if(j > 0) {
				do
					*p++='+';
						while(--j);
				*p=0;
				}
			msprint(c);
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		PowderT(void *vp)
		{
		char	c[16];
		int		i;
		command *cp =Mscan->c;
		lib 	*l	=FindLib(cp->to);

		if(l) {
			i=l->powder;
			i=ReadInt(i);
			if(abs(i) > 100) {
				err(E_MAX_VAL);
				i /= 10;
				}
			l->powder=i;
			sprintf(c,"%d %c",l->powder,symDEGREE);
			strswap(c," ");
			msprint(c);
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetDSPgain(void *vp)
{
		int		i=(int )vp,j;
		char	c[32];
		int		g[4][9]={{-6,0,6,17,-1,-1,-1,-1,-1},
					   {-20,-15,-10,-5,0,6,12,18,-1},
					   {-6,0,6,17,-1,-1,-1,-1,-1},
					   {-20,-15,-10,-5,0,6,12,18,-1}};	

		watchdog();
		if(Kbhit(CHLCD))
		{
			switch(Getch(CHLCD))
			{
				case LEFT:	DspGain(i,-1);
							break;
				case RIGHT:	DspGain(i,+1);
							break;
			}
			if(curr_menu)
				DSPgain[i]=curr_menu->opt=DspGain(i,0);
		}
		else
		{
			j=DspGain(i,0) - DSPgain[i];
			if(j<0)
				while(j++) DspGain(i,1);
			else
				while(j--) DspGain(i,-1);
			if(curr_menu)
				curr_menu->opt=DSPgain[i];
		}
		sprintf(c,"%3ddb",g[i][DspGain(i,0)]);
		msprint(c);
		return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
command	*rx_command(char *buff, int m)
		{
		int		i,j;
		char 	*p = buff;
		command	*c=make_command();
		long	dt;

		c->from=m;
		c->to=LinkAddr(OBP);
		++p;
/*อออ serijska stev. ukaza อออออออออออออออออออออออออออออ*/
		p=get_int(&c->n,p);
		p=get_char(&c->opt[0],p);
/*อออ korekcijski faktorji อออออออออออออออออออออออออออออ*/
		p=get_double(&c->corrx,p);
		p=get_double(&c->corry,p);
		p=get_double(&c->corrz,p);
/*อออ parametri ukaza                  อออออออออออออออออ*/
		p=get_char(&c->effect,p);
		p=get_char(&c->fmode,p);
		p=get_char(&c->rounds,p);
		p=get_char(&c->interval,p);
		p=get_char(&c->section,p);
		p=get_char(&c->ammo,p);
		p=get_char(&c->traj,p);
		p=get_char(&c->charge,p);
		p=get_char(&c->fuze,p);
		p=get_char(&c->tmode,p);
		p=get_long(&dt,p);
/*อออ skaniranje cilja                 อออออออออออออออออ*/
		c->tg=LinkAddr(rx_lib(scan_lib(&p)));
		p=get_rtime(&Time,p);
/* firetime setup										*/
		c->ftime = Time;
		addsecs(&c->ftime,dt);
/* izracun zakasnitve prenosa podatkov; 1000 bps modem, 10 	*/
/* dodatnih bytov v DSP, key_del v msecs, faktor 500 je		*/
/* zaokrozanje na 0.5 sec !									*/
		for(i=0; p != buff; --p)
			++i;
		dt=((i + 10) * 8 + abs(key_delay) + 500)/1000;
		addsecs(&Time,-dt);
		WriteTime(&Time);
/*อออ nastavitev rxmode = PENDING !    อออออออออออออออออ*/
		c->rxmode=M_PEND;
/*อออ preveri izbrano orozje na OBP    อออออออออออออออออ*/
		i=targets_xn(OBP);
		j=targets_n(OBP->x);
		c->chk=null;
		switch((c->section) & 0x0f)	{
			case 0:	break;							/* baterija				*/
			case 1: if((i > j/2) && (j > 1 ))		/* 1. vod				*/
						i=null;						/*						*/
					break;							/*						*/
			case 2: if((i <= j/2) && (j > 1))		/* 2.vod				*/
						i=null;						/*						*/
					break;							/*						*/
			case 3: if(i != (c->section)>>4)		/* > oznaka = hi nibble	*/
						i=null;
					break;
			}
		if(!i)
		{
				memfree(c);
				c=NULL;
				
		}
		return(c);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		next_command(void *vp)
		{
		command **cp = vp;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}
		if((*cp)->next != *cp)
			*cp=(*cp)->next;
		else
			err(E_ENDLIST);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		last_command(void *vp)
		{
		command **cp = vp,*p;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}
		if((*cp)->next == *cp)	{
			err(E_ENDLIST);
			return(1);
			}
		for(p=*cp; (*cp)->next != p; *cp=(*cp)->next);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	   	next_status(void *vp)
		{
		command 	**cp = vp;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}

		if((*cp)->to == LinkAddr(OBP))
			return(null);
		if((*cp)->ntx < MAXGUN-1)
			++(*cp)->ntx;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	   	last_status(void *vp)
		{
		command 	**cp = vp;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}
		if((*cp)->to == LinkAddr(OBP))
			return(null);
		if((*cp)->ntx > 0)
			--(*cp)->ntx;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CheckExe(void *vp)
		{
		command **cp=vp;
		if((*cp)->to == LinkAddr(OBP) && checktype(OBP,TYPE_BT)) {
			CPEXE=*cp;
			if(CPEXE == CP)
				Ungetch(GXM_MESSAGE_ID,0);
			else
				Ungetch(GXM_GUN_ID,0);
			return(null);
		}
		else
			return(1);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		list_on(void *vp)
		{
		command **cp=vp;

		if(Kbhit(CHLCD))
			Getch(CHLCD);
        DeleteTask(CountDown,RTC_task);
        DeleteTask(CountBlink,PIT_task);	
		CPEXE = *cp;

		LL	=freelib(LL);
		LL=makelib();
		blll=freelib(blll);
		if(LPP) {
        	if(crest)
				puts_pc("-r\r\n>");		/* izklop preseka terena			*/
			if(*cp)
				if((*cp)->chk > 0)		/* aktiviranje izbranega sporocila	*/
			        (*cp)->chk *= -1;
            }
		crest=freelib(crest);
		return(list_display(cp));
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		list_CP(void *vp)
		{
		char 	c[32],cc[32];
		command **cp = vp;
		int		m;
		if(!*cp)
			return(null);
/*........................................................*/
		xyLCD(0,0);
		if(LinkAddrText((*cp)->tg,cc))
			sprintf(c,"%-4d:%7s",(*cp)->n,cc);
		else
			sprintf(c,"%-4d:   --- ",(*cp)->n);
		strswap(c,":");
		puts_lcd(c);
/*........................................................*/
		xyLCD(0,1);
		if(LinkAddrText((*cp)->from,cc))
			sprintf(c,"%-4s:%7s",MESS_SOURCE,cc);
		else
			sprintf(c,"%-4s:   --- ",MESS_SOURCE);
		strswap(c,":");
		puts_lcd(c); 
/*........................................................*/
		xyLCD(0,2);
		if(LinkAddrText((*cp)->to,cc))
			sprintf(c,"%-4s:%7s",MESS_DEST,cc);
		else
			sprintf(c,"%-4s:   --- ",MESS_DEST);
		strswap(c,":");
		puts_lcd(c);
/*........................................................*/
		xyLCD(0,3);
		if((*cp)->to == LinkAddr(OBP))
		{
			m=(*cp)->rxmode;
			(*cp)->ntx=targets_xn(OBP)-1;
		}
		else
			m=(*cp)->txmode[(*cp)->ntx];
		if(m)
			sprintf(c,"%-10s",GetMessCode(_MESS_UNDEF+m));
        else
			sprintf(c,"%-10s"," .......");
		m=(*cp)->ntx;
		sprintf(cc,"%2d",++m);
		strcat(c,cc);
		puts_lcd(c);
/*........................................................*/
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		list_GUN(void *vp)
{
		char 	c[32],cc[32];
		command **cp = vp;
		int		i,m;
/*... izhod v slucaju, ce ni nalozene balistike			..*/
		if(!Guns)
			return(eof);
/*........................................................*/
		comm_enable(cp);
		Cn=FindLib(LinkAddr(Cn));
		
		if(Lr)
		{
			if(checktype(Lr,TYPE_BT))
			{
				Cn=FindLib(LinkAddr(Lr));
				if(Cn && (Cn != OBP))
				{
					i=targets_xn(Cn);
					(*cp)->ntx = i-1;
					for(Cn->xc = Cn->x; --i; Cn->xc = Cn->xc->next);
				}
				(*cp)->to = LinkAddr(Cn);
			}
		Lr=freelib(Lr);
		}
		else
			if(!checktype(Cn,TYPE_BT))
				Cn=findtype(Ln,TYPE_BT);
		(*cp)->to=LinkAddr(Cn);
/*........................................................*/
		if(checktype(OBP,TYPE_BT))
			(*cp)->to=LinkAddr(Cn=OBP);
/*........................................................*/
        if(Cn && LinkAddrText((*cp)->to,cc))
		{
			xyLCD(0,0);
			sprintf(c,"%-12s",cc);
			puts_lcd(c);
/*........................................................*/
			xyLCD(0,1);
		
			sprintf(c,"%-12s",Guns[Cn->gun].name);
			puts_lcd(c);
/*........................................................*/
			xyLCD(0,3);
		
			if((*cp)->to == LinkAddr(OBP))
			{
				m=(*cp)->rxmode;
				(*cp)->ntx=targets_xn(OBP)-1;
			}
			else
				m=(*cp)->txmode[(*cp)->ntx];
			if(m)
				sprintf(c,"%-10s",GetMessCode(_MESS_UNDEF+m));
        	else
				sprintf(c,"%-10s"," .......");
			m=(*cp)->ntx;
			sprintf(cc,"%2d",++m);
			strcat(c,cc);
			puts_lcd(c);
		}
/*........................................................*/
return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		list_display(void *vp)
{ /* ??? */
		command	**cp=vp;
		clear_screen(NULL);
		if(cp)
		{
			if(cp == &CP)
				return(list_CP(vp));
			if(cp == &GUN)
				return(list_GUN(vp));
			if(cp == &CPEXE)
				if(*cp)
				{
					if(*cp == CP)
						return(list_CP(vp));
					if(*cp == GUN)
						return(list_GUN(vp));
				}
		}
		return(eof);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	   	NextTarget(void *vp)
		{
		command 	**cp = vp;
		lib			*l=FindLib((*cp)->to);
		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}
		if(checklib(Ln,l->ltype,l->n)==OBP)
			return(1);
		if(bll->xc->next) {
			l->xc	= l->xc->next;
			bll->xc = bll->xc->next;
			blll=freelib(blll);
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	   	LastTarget(void *vp)
		{
		command	**cp = vp;
		lib		*l=FindLib((*cp)->to);
		coord	*p;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}
		if(checklib(Ln,l->ltype,l->n)==OBP)
			return(1);

        if(bll->x != bll->xc) {
			for(p=bll->x; p->next != bll->xc; p=p->next);
           	bll->xc = p;
			for(p=l->x; p->next != l->xc; p=p->next);
           	l->xc = p;
			blll=freelib(blll);
	      	}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		EditGun(void *vp)
		{
		lib		*l;
		char 	p[32];
		command *cp =Mscan->c;
		int		i;
/*---------------------------------------------------------
	if device type = BT => default unit
----------------------------------------------------------*/
		if(checktype(OBP,TYPE_BT))
			l=OBP;
		else {
			l=FindLib(cp->to);
			if(!l)
				l=findtype(Ln,TYPE_BT);
			if(Lr && Kbhit(CHLCD))
				if(Lr->xc && checktype(Lr,TYPE_BT)) {
					l=FindLib(LinkAddr(Lr));
					if(l != OBP) {
						i=targets_xn(Lr);
						for(l->xc = l->x; --i; l->xc = l->xc->next);
					}
					Lr=freelib(Lr);
			}
			if(l && Kbhit(CHLCD))
				switch(i=Getch(CHLCD)) {
				case LEFT:
					Ln=l;
					LastList(NULL);
					l=Ln;
					break;

				case RIGHT:	
					Ln=l;
					NextList(NULL);
					l=Ln;
					break;
				default: 
					if(isdigit(i)) {
						i -= '0';
						if(i && targets_n(l->x) >= i) {
							l->xc = l->x;
							while(--i)
								l->xc = l->xc->next;
						}
					}
					break;
				}
			}
		cp->to = LinkAddr(l);
		if(!LinkAddrText(cp->to,p))
			sprintf(p," --  ");
		msprint(p);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SelectUnitType(void *vp)
		{
		int		i;
		lib		*l;

		if(Kbhit(CHLCD)) {
			i=Getch(CHLCD);
			switch(i) {
				case LEFT:	if(checktype(LL,TYPE_BT))
								LL->ltype=addtype(TYPE_OP);
							if(checktype(LL,TYPE_CO))
								LL->ltype=addtype(TYPE_BT);
							l=findtype(Ln,LL->ltype->t);
							if(l)
								LL->n = l->n;
							else
								LL->n = 1;
							break;
				case RIGHT:	if(checktype(LL,TYPE_BT))
								LL->ltype=addtype(TYPE_CO);
							if(checktype(LL,TYPE_OP))
								LL->ltype=addtype(TYPE_BT);
							l=findtype(Ln,LL->ltype->t);
							if(l)
								LL->n = l->n;
							else
								LL->n = 1;
							break;
				default:	Ungetch(i,0);
							LL->n=abs(ReadInt(LL->n)) % 100;
							break;
				}
			Cn=checklib(Ln,LL->ltype,LL->n);
			if(Cn)
				copylib(Cn,LL);
			else{
				LL->x=LL->xc=freecoord(LL->x);
				LL->txt=LL->ltype->typtxt;
				}
			}
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SelectTargetType(void *vp)
		{
		int		i;
		lib		*l;

		if(Kbhit(CHLCD)) {
			i=Getch(CHLCD);
			switch(i) {
				case LEFT:	if(checktype(LL,TYPE_TG))
								LL->ltype=addtype(TYPE_MT);
							l=findtype(Ln,LL->ltype->t);
							if(l)
								LL->n = l->n;
							else
								LL->n = 1;
							break;
				case RIGHT:	if(checktype(LL,TYPE_MT))
								LL->ltype=addtype(TYPE_TG);
							l=findtype(Ln,LL->ltype->t);
							if(l)
								LL->n = l->n;
							else
								LL->n = 1;
							break;
				default:	Ungetch(i,0);
							LL->n=abs(ReadInt(LL->n)) % 100;
							break;
				}
			Cn=checklib(Ln,LL->ltype,LL->n);
			if(Cn)
				copylib(Cn,LL);
			else{
				LL->x=LL->xc=freecoord(LL->x);
				LL->txt=LL->ltype->typtxt;
				}
			}
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		EditMeteo(void *vp)
		{
		char	p[32];
		int		i;

		if(Kbhit(CHLCD)) {
			i=Getch(CHLCD);
			switch(i) {
				case LEFT:	if(LinkAddrType(meteo_A - 0x100))
								meteo_A -= 0x100;
							break;
				case RIGHT:	if(LinkAddrType(meteo_A + 0x100))
								meteo_A += 0x100;
							break;
				case CLEAR:	meteo_A=1;
							break;
				default:	Ungetch(i,0);
							meteo_A = (meteo_A & ~0xff)
								| (abs(ReadInt(meteo_A & 0xff)) % 100);
							break;
				}
			}
		if(!(meteo_A & 0xff))
			meteo_A |= 1;
		if(!LinkAddrType(meteo_A))
			sprintf(p," --  ");
		else
			LinkAddrText(meteo_A,p);
		msprint(p);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		DiffGps(void *vp)
		{
		char	p[32];
		int		i;

		if(Kbhit(CHLCD)) {
			i=Getch(CHLCD);
			switch(i) {
				case LEFT:	if(LinkAddrType(diff_GPS - 0x100))
								diff_GPS -= 0x100;
							break;
				case RIGHT:	if(LinkAddrType(diff_GPS + 0x100))
								diff_GPS += 0x100;
							break;
				case CLEAR:	diff_GPS=1;
							break;
				case REFRESH:break;
				default:	Ungetch(i,0);
							diff_GPS = (diff_GPS & ~0xff)
								| (abs(ReadInt(diff_GPS & 0xff)) % 100);
							break;
				}
			}
		if(!(diff_GPS & 0xff))
			diff_GPS |= 1;
		if(!LinkAddrType(diff_GPS))
			sprintf(p," --  ");
		else
			LinkAddrText(diff_GPS,p);
		msprint(p);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		new_command(void *vp)
		{
		command **p = vp,*c=make_command();
		if(!*p)
			*p = c->next = c;
		else{
			c->n = CommEnum(*p);
			c->next = (*p)->next;
			(*p)->next = c;
			*p=c;
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
command	*make_command(void)
		{
		int		i;
		command *c	= memalloc(sizeof(command));
		c->n = 1;
		for(i=0;i<MAXGUN;++i)
			c->txmode[i]=M_UNDEF;
		c->rxmode=M_UNDEF;
        c->ftime = Time;
 		c->next = c;
		return(c);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis iz comm *p->to v LL in izpis	                  */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UnitToLL(void *vp)
		{
		command **cp = vp;
		if(!*cp)
			new_command(cp);
		Cn=FindLib((*cp)->to);
		if(!Cn) {
			freelib(LL);
			LL=makelib();
			LL->ltype=LinkAddrType((*cp)->to);
			LL->n=LinkAddrN((*cp)->to);
			if(!LL->ltype)	{
				LL->ltype=addtype(TYPE_BT);
				auto_ntyp(NULL);
				}
			LL->txt=LL->ltype->typtxt;
			Cn=findtype(Ln,LL->ltype->t);
			}
		else
			copylib(Cn,LL);
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis iz LL v comm *p->t 	 		                  */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UnitFromLL(void *vp)
		{
		command **cp = vp;
		InsertLib(&LL);
		(*cp)->to = LinkAddr(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		InitUnit(void *vp)
		{
		int	i;
		LL->x=LL->xc=freecoord(LL->x);
		for(i=0; i<MAXAMM; ++i)
			LL->ammpcs[i]=0;
		if(!checktype(LL,TYPE_OP) && !checktype(LL,TYPE_BT) && !checktype(LL,TYPE_CO))
			LL->ltype=addtype(TYPE_BT);
		LL->txt=LL->ltype->typtxt;
		auto_ntyp(NULL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis iz comm *p->target v LL in izpis                */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		TargetToLL(void *vp)
		{
		command **cp = vp;
		if(!*cp)
			new_command(cp);
		Cn=FindLib((*cp)->tg);
		if(!Cn) {
			freelib(LL);
			LL=makelib();
			LL->ltype=LinkAddrType((*cp)->tg);
			LL->n=LinkAddrN((*cp)->tg);
			if(!LL->ltype)	{
				LL->ltype=addtype(TYPE_TG);
				auto_ntyp(NULL);
				}
			LL->txt=LL->ltype->typtxt;
			Cn=findtype(Ln,LL->ltype->t);
			}
		else
			copylib(Cn,LL);
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		InitTarget(void *vp)
		{
		LL->x=LL->xc=freecoord(LL->x);
		if(!checktype(LL,TYPE_TG) && !checktype(LL,TYPE_MT))
			LL->ltype=addtype(TYPE_TG);
		LL->txt=LL->ltype->typtxt;
		auto_ntyp(NULL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis iz LL v comm *p->tg								                */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		TargetFromLL(void *vp)
		{
		command **cp = vp;
		InsertLib(&LL);
		(*cp)->tg = LinkAddr(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis iz comm *p->corrx,y,z v LL in izpis  					            */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CorrToLL(void *vp)
		{
		command **cp = vp;
		lib		*tg;

		if(!*cp)					/* ce je list prazen, formiraj nov ukaz */
			new_command(cp);        /*                                      */
		tg=FindLib((*cp)->tg);      /* poisci listu pripadajoc cilj			*/
		if(!tg || !tg->x) {         /* error message in izhod, ce ga ni	oz.	*/
			err(E_NO_TARGET);       /* nima veljavnih koordinat				*/
			return(1);
			}
		if(!checktype(tg,TYPE_TG)) {   /* korekcijo izvaja samo nad tipom TG	*/
			err(E_NOT_TG);          /* max. stevilo tock naj bo 3 !!!		*/
			return(1);
			}
/* Izracun centra cilja iz LL v NORTH za vpis referencne tocke				*/
/* za oceno centra ognja												    */
		NORTH=Distribute(tg,NULL,1);
		if(!NORTH) {
			err(E_NO_TARGET);       /*tg nima veljavnih koordinat			*/
			return(1);
			}
		freelib(LL);
		copylib(NORTH,LL=makelib());
/* izpis veljavne korekcije v (samo za informacijo)							*/
		LL->xc->x += (*cp)->corrx;
		LL->xc->y += (*cp)->corry;
		LL->xc->z += (*cp)->corrz;
		Displc(LL);
/* ponovni vnos NORTH (korekcija = 0,0,0) tocka je ena sama !!!				*/
		copylib(NORTH,LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		InitCorr(void *vp)
		{
		LL->x=LL->xc=freecoord(LL->x);
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CorrAvrg(void *vp)
		{
		double	x=0,y=0,z=0;
		if(LL->x) {
			for(LL->xc=LL->x;LL->xc;LL->xc=LL->xc->next) {
				x += LL->xc->x;
				y += LL->xc->y;
				z += LL->xc->z;
				}
			x /= targets_n(LL->x);
			y /= targets_n(LL->x);
			z /= targets_n(LL->x);
			LL->x=LL->xc=freecoord(LL->x);
			add_coord(LL,x,y,z);
			}
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis iz LL v comm *p->corrx,y & z 			          */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CorrFromLL(void *vp)
		{
		command **cp = vp;
		CorrAvrg(NULL); 				/* povprecenje vpisanih tock	*/
										/* na NORTH v top. sever !!!	*/
		if(LL->x) {                     /* ce rezultat obstoja na ->x 	*/
			(*cp)->corrx += LL->xc->x - NORTH->xc->x;
			(*cp)->corry += LL->xc->y - NORTH->xc->y;
			(*cp)->corrz += LL->xc->z - NORTH->xc->z;
			}
		else
			(*cp)->corrx = (*cp)->corry = (*cp)->corrz = 0.0;
		LL->x=LL->xc=freecoord(LL->x);
		return(null);
		}
/*อออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CommandCopy(void *vp)
		{
		int		i;
		command *c,**cp = vp;

		if(!*cp)
			return(new_command(cp));

		c=make_command();
		*c = **cp;
		c->n = CommEnum(*cp);
		for(i=0;i<MAXGUN;++i)
			c->txmode[i]=M_UNDEF;
		c->rxmode=M_UNDEF;
		c->chk=null;
		c->next = (*cp)->next;
		(*cp)->next = c;
		*cp=c;
		return(null);
		}
/*อออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		EraseCommand(void *vp)
		{
		command *c,**cp = vp;
		char	cc[32];
        if(!cp) {
        	while(CP)
            	EraseCommand(&CP);
			return(null);
			}
		if(!*cp)
			return(null);
		if(LPP) {
			sprintf(cc,"-M %04X\r\n>",(*cp)->chk);
			puts_pc(cc);
			}
		if((*cp)->next == *cp) {
			memfree(*cp);
			*cp=CPM=NULL;
			}
		else {
			if(CPM == *cp)
				CPM=CPM->next;
			for(c=*cp; c->next != *cp; c=c->next);
			c->next = (*cp)->next;
			memfree(*cp);
			*cp = c;
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* Predikcija premicnega cilja naprej					*/
/*														*/
/*														*/
int		NextPredict(void *vp)
		{
		if(!checktype(LL,TYPE_MT) || (targets_n(LL->x) < 3)) {
			err(E_NOT_MT);
			return(null);
			}
		switch(CP->tmode) {
		case 0:
		case 1:	addsecs(&LastCoord(LL)->t,track_delay);
			break;
		default:err(E_FIX_FTIME);
		}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* Predikcija premicnega cilja nazaj					*/
/*														*/
/*														*/
int		LastPredict(void *vp)
		{
		rtime t = Time;
		if(!checktype(LL,TYPE_MT) || (targets_n(LL->x) < 3)) {
			err(E_NOT_MT);
			return(null);
			}
		switch(CP->tmode) {
		case 0:
		case 1:	addsecs(&LastCoord(LL)->t,-track_delay);
				if(dtime(&LastCoord(LL)->t,&t) < 0) {
					warn(W_FTIME_ELAPSE);
					return(NextPredict(vp));
					}
				break;
		default:err(E_FIX_FTIME);
		}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CheckImpl(void *vp)
		{
		switch((int)vp)
		{
		case HW_GPS:
			if(GPS_ON)
				return(null);
			break;
		case HW_DGPS:
			if(GPS_DIFF)
				return(null);
			break;
		default:
			if(swvers & (int)vp)
				return(null);
			break;			}
		return(1);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* Ballistic module initialization							*/
/*															*/
int		InitBallistic(void *vp)
		{
		command **cp = vp;
		lib		*un;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}
/* check, if unit exists, if BT type						*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
		un=FindLib((*cp)->to);
		if(!un || !un->x) {
			err(E_NO_UNIT);
			return(1);
			}
		if(!checktype(un,TYPE_BT)) {
			err(E_NOT_BT);
			return(1);
			}
		Guns[un->gun].ABC = NULL;
        beep(1000);
        return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* Checking the moving target; valid firetime as defined	*/
/* in command par. (*cp)->par[FIRETIME]			 			*/
/* Set the <B> structure by calling lmenu !!!				*/
/*															*/
int		CheckTarget(void *vp)
		{
		command **cp = vp;
		lib		*tg,*un;

		if(!*cp) {
			err(E_EMPTY_LIST);
			return(1);
			}

/* check, if target exists, if TG or MT type				*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
		tg=FindLib((*cp)->tg);
		if(!tg || !tg->x) {
			err(E_NO_TARGET);
			return(1);
			}
		if(!checktype(tg,TYPE_TG) && !checktype(tg,TYPE_MT)) {
			err(E_NOT_MTG);
			return(1);
			}

/* check, if unit exists, if BT type						*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
		un=FindLib((*cp)->to);
		if(!un || !un->x) {
			err(E_NO_UNIT);
			return(1);
			}
		if(!checktype(un,TYPE_BT)) {
			err(E_NOT_BT);
			return(1);
			}
		if(!Guns)
			return(1);
		if(!Guns[un->gun].ABC)
			if(CheckPass(NULL))
				return(1);
		freelib(LL);
		copylib(tg,LL=makelib());

		LL->xc = LastCoord(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* Izracun vpadnih tock v LL; stevilo definirano s					*/
/* stevilom tock v ->un in zahtevano enoto za delovanje				*/
/* v parametru GUN_SELECT											*/

int		CalcTarget(void *vp)
		{
		command 	**cp = vp;
		rtime		t=Time;
		int			i,j,k;
		coord		*q;
		lib 		*l=FindLib((*cp)->to);
		if(!l || !l->x) {
			err(E_NO_UNIT);
			return(1);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*	Za premicni cilj se doda nova koordinata s tekocim casom !     	*/
/*  Ce je le-ta po casu zadnja v nizu, predikcija se ni bila      	*/
/*	izracunana, ali pa je cas ze potekel! V tem primeru se na tekoci*/
/*	cas doda interval track_delay in izracuna predikcija polozaja!  */
		if(checktype(LL,TYPE_MT)) {
			LL->xc = LastCoord(LL);
			if(dtime(&LL->xc->t,&t) < 0) {
				add_coord(LL,0.0,0.0,0.0);
				addsecs(&LL->xc->t,track_delay);
				}
		}
		switch((*cp)->tmode) {
			case 2:
			case 3:	if(dtime(&(*cp)->ftime,&t) < 0)
					{
						warn(W_FTIME_ELAPSE);
						return(1);
					} else
						LL->xc->t = (*cp)->ftime;
					break;
		}			
		if(checktype(LL,TYPE_MT))
			if(!interpolate(LL)) {
				erase_coord(LL,LL->xc);
				err(E_ILL_DATA);
				return(1);
				}
/* Stevilo tock se omeji na 3 z brisanjem predhodnih meritev

			while(targets_n(LL->x) > 3)
				erase_coord(LL,FirstCoord(LL));
*/
/*......................................................*/
/* stevilo orozij za streljanje; glej SelectGun()		*/
/* i =	n;                                              */
/* j =	n prvega                                        */

			i = targets_n(l->x);
			j = 1;
			switch(0x0f & (*cp)->section)	{
/* baterija */	case 0: break;
/* 1. vod	*/	case 1: if(i>1)
							i /= 2;
						break;
/* 2. vod	*/  case 2: if(i>1) {
							j = i/2 + 1;
							i = i - i/2;
							}
						break;
/* single	*/	case 3: j=((*cp)->section)>>4;
						i = 1;
						break;
				}

		bll=freelib(bll);
		blll=freelib(blll);
		bll=Distribute(LL,l,i);
		if(!bll) {
			err(E_NO_TARGET);
			return(1);
			}
		if(checklib(Ln,l->ltype,l->n) == OBP) {
			k = targets_xn(OBP) - j;
			if((k < 0) || (k >= i)) {
				err(E_GUN_NOTACT);
				return(1);
				}
			while(k--)
				bll->xc = bll->xc->next;
			}
		else{
			k = targets_xn(l) - j;
			if((k < 0) || (k >= i)) {
				l->xc=l->x;
				while(--j)
					l->xc=l->xc->next;
				}
			else
				while(k--)
					bll->xc = bll->xc->next;
			}

/*	upostevanje korekcije:
	  - pristevanje corr
	  - pretvorba v az-r-dz
	  - display

	  mnozenje korekcije s faktorjem
		-1	-normalno premikanje
		 0	brez korekcije, priprava za spremembo izh. hitrosti
		 1	izracun spr. hitrosti [ NaklonMuzz() ]
*/

		for(q = bll->x; q; q=q->next) {
			q->x -= (*cp)->corrx * corr;
			q->y -= (*cp)->corry * corr;
			q->z -= (*cp)->corrz * corr;
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* izracun <i> vpadnih tock na cilj v <ll>				*/
/*														*/
lib		*Distribute(lib *ll, lib *lll,int i)
		{
		lib			*l,*p;
		int			j;
		coord		*c;
		samples		*x=NULL,*y=NULL,*z=NULL;
		double		*xx,*yy,*zz,ax,ay,az,df;

/*		funkcija vrne lib item istega tipa kot vhodni	*/
		copylib(ll,l=makelib());
		l->x=l->xc=freecoord(l->x);
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/***	stevilo tock v ll == 1 ali moving target !!!	*/
/*		vse vpadne tocke so enake 						*/
		if((targets_n(ll->x)==1) || checktype(ll,TYPE_MT)) {
			for(j=0; j<i; ++j) {
				add_coord(l,ll->xc->x,ll->xc->y,ll->xc->z);
				l->xc->t=ll->xc->t;
				}
			l->xc=l->x;
			return(l);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/***	stevilo tock v ll == 2, linijski cilj			*/
		if(targets_n(ll->x)==2) {
			x=init_samples(x,2);
			y=init_samples(y,2);
			z=init_samples(z,2);
			add_sample(x,0,ll->x->x);
			add_sample(y,0,ll->x->y);
			add_sample(z,0,ll->x->z);
			add_sample(x,i,ll->x->next->x);
			add_sample(y,i,ll->x->next->y);
			add_sample(z,i,ll->x->next->z);
			xx=solve(x);
			yy=solve(y);
			zz=solve(z);
			if(xx && yy && zz)
				for(j=0; j<i; ++j) {
					add_coord(l,polyp(j+0.5,xx,2),
									polyp(j+0.5,yy,2),
										polyp(j+0.5,zz,2));
					}
			freesamples(z);
			freesamples(y);
			freesamples(x);
			watchdog();
			IsectOff(lll->x,l->x);
			l->xc = l->x;
			return(l);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/***	stevilo tock v ll > 2, povrsinski cilj			*/
		if(targets_n(ll->x) > 2) {
			ax=ay=az=0.0;
			j = targets_n(ll->x);
			for(c=ll->x; c; c=c->next) {
				ax += c->x;
				ay += c->y;
				az += c->z;
				}
			ax /= j;
			ay /= j;
			az /= j;

			x=init_samples(x,3);
			y=init_samples(y,3);
			z=init_samples(z,3);

			for(c=ll->x; c; c=c->next) {
				df=atan2(c->y - ay,c->x - ax);
				add_sample_t(x,df,c->x - ax,2.0*M_PI);
				add_sample_t(y,df,c->y - ay,2.0*M_PI);
				add_sample_t(z,df,c->z - az,2.0*M_PI);
				}

			xx=solve(x);
			yy=solve(y);
			zz=solve(z);

/* ce velja		y = A*cos(f) + B*sin(f)    in  d/df (x^2 + y^2) == 0
				x = C*cos(f) + D*sin(f)
				f = atan( (2AB + 2CD) / AA-BB+CC-DD) / 2
*/
			if(xx && yy && zz) {
				df = atan2(2*yy[1]*yy[2] + 2*xx[1]*xx[2],
							yy[1]*yy[1]-yy[2]*yy[2] +
								xx[1]*xx[1]-xx[2]*xx[2])/2;
				add_coord(l,ax + polyt(df,xx,2.0*M_PI,2),
							ay + polyt(df,yy,2.0*M_PI,2),
							az + polyt(df,zz,2.0*M_PI,2));
				df += M_PI;
				add_coord(l,ax + polyt(df,xx,2.0*M_PI,2),
							ay + polyt(df,yy,2.0*M_PI,2),
							az + polyt(df,zz,2.0*M_PI,2));
				p=Distribute(l,lll,i);
				freelib(l);
				l=p;
				}
			l->xc = l->x;
			freesamples(z);
			freesamples(y);
			freesamples(x);
			return(l);
			}
		return(NULL);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		DisplBallist(void *vp)
{
		command **cp=vp;
		char	c[16];
		int		i;
		lib		*l=FindLib((*cp)->to);
		if(cp == &CP) {
			if(!blll) {
				CountMode = 1;	
		        DeleteTask(CountDown,RTC_task);
			    DeleteTask(CountBlink,PIT_task);	
				clear_screen(NULL);
				LinkAddrText(LinkAddr(l),c);
				switch((*cp)->section & 0x0f)
				{
				case 0:
				case 1:
					i=targets_xn(bll);
					break;
				case 2:
					i=targets_n(l->x)/2+targets_xn(bll);
					break;
				case 3:
					i=(*cp)->section >> 4;
					break;
				}
				sprintf(strchr(c,0),"%c%d",'/',i);
				xyLCD(0,0);
				puts_lcd(c);
				sprintf(error_screen[0],"%-20s",c);
				CalcCrest(vp);
				copylib(bll,blll=makelib());
				CalcBallist(l);
				}
			if(!blll->xc)
				return(1);
		}
		if(cp == &GUN) 
			CountMode=0;

	CountDown();
	return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SwitchPage(void *vp)
{
		switch(CountMode) {
		case 0:
			break;
		case 1:
			++CountMode;
			break;
		case 2:
			--CountMode;
			break;
		default:
			CountMode=1;
			break;
		}
		CountDown();
		return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	CountBlink(void)
{
	char	c[16];
	DeleteTask(CountBlink,PIT_task);
	sprintf(c,"%12s"," ");
	xyLCD(0,3);
	puts_lcd(c);

}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	CountDown(void)
{
	char	c[32],cc[32],*p;
	lib		*l;
	rtime	t;
	double	tof,mils,d;
	gun		*g;
	int		i;
	command	*cp;

	if(!CountMode)
		cp=GUN;
	else
		cp=CP;

	l=FindLib(cp->to);
	g=&Guns[l->gun];
	mils=Guns[l->gun].mils;
	tof=cp->tof;

	switch(CountMode) {

	case 0:	d = ScaleAngle(l->refd,MR10,maxmils,null);
			sprintf(c,"%s:%.2lf",M_AZIMUTH,d/100.0);
			translate(c,'.','-');
			strswap(c,":");
			sprintf(cc,"%-12s",c);
			xyLCD(0,0);
			puts_lcd(cc);

/*			d = l->xc->refp;
			d = g->zeroA + g->scaleA * (d - l->refd)/MR10*mils;
            d = ScaleAngle(d,mils,mils,null);
*/			sprintf(c,"%s:%.2lf",M_BEARING,GetRefp(l)/100.0);
			translate(c,'.','-');
			strswap(c,":");
			sprintf(cc,"%-12s",c);
			xyLCD(0,1);
			puts_lcd(cc);

			sprintf(c,"%12s"," ");
			xyLCD(0,2);
			puts_lcd(c);
			xyLCD(0,3);
			puts_lcd(c);
			break;

	case 1:	sprintf(c,"%6.2f",fmod(nround(blll->xc->y,4),mils)/100);
			translate(c,'.','-');

			sprintf(cc,"\x86");
			for(i=cp->charge;i>0;--i)
				sprintf(strchr(cc,0),"%c",'+');
			if(Guns[l->gun].charge[cp->ammo])
				if(Guns[l->gun].charge[cp->ammo][cp->charge])
				   sprintf(cc,"%s",Guns[l->gun].charge[cp->ammo][cp->charge]);
			strswap(cc,"\x86");
			sprintf(strchr(c,0)," %5s",cc);
			xyLCD(0,0);
			puts_lcd(c);

			p=strchr(Guns[l->gun].fuze[cp->ammo][cp->fuze],'/');
			sprintf(c,"%6.2f",fmod(nround(blll->xc->x,4),mils)/100);
			translate(c,'.','-');
			if(p)
				sprintf(strchr(c,0),"%6s",++p );
			else
				sprintf(strchr(c,0),"%6.1lf",tof/10);
			xyLCD(0,1);
			puts_lcd(c);

			if(Guns[l->gun].gmode & ELEV_DBL)
			{
				sprintf(c,"%6.2f      ",fmod(nround(blll->xc->z,4),mils)/100);
				translate(c,'.','-');
			}
			else
				sprintf(c,"            ");
			xyLCD(0,2);
			puts_lcd(c);
			break;

	case 2:	sprintf(c,"%12s",Guns[l->gun].ammo[cp->ammo]);
			xyLCD(0,0);
			puts_lcd(c);

			if(cp->rounds)
				sprintf(cc,"%d %s",cp->rounds,M_ROUNDS);
			else
				sprintf(cc,"   ...");
			strswap(cc," ");
			sprintf(c,"%12s",cc);
			xyLCD(0,1);
			puts_lcd(c);

			if(cp->interval)
				sprintf(cc,"%d %s",cp->interval,M_SECOND);
			else
				sprintf(cc,"   ...");
			strswap(cc," ");
			sprintf(c,"%12s",cc);
			xyLCD(0,2);
			puts_lcd(c);
			break;

	default:CountMode = 1;
			CountDown();
			return;
	}

	if(cp->to == LinkAddr(OBP))
		i=cp->rxmode;
	else
		i=cp->txmode[cp->ntx];

	switch(i) {
	case M_UNDEF:
	case M_PEND:
		switch(CountMode) {
	
		case 1:
		case 2:	if(checktype(LL,TYPE_MT)) {
					cp->ftime = LL->xc->t;
					i=3;
				}
				else
					i=cp->tmode;
	
				switch(i) {
				case 0:	sprintf(cc,FIRE_HOLD);
						break;	
				case 1:	sprintf(cc,FIRE_WHEN_READY);
						break;
				case 2:
				case 3:	if((dtime(&cp->ftime,&Time)-tof/10.0) > 0) {
							calc_rtime(0,dtime(&cp->ftime,&Time)-tof/10.0,&t);
							sprintf(cc,"%02d:%02d:%02d",t.hour,t.min,t.sec);
						}
						else
							sprintf(cc," ");
/*						strswap(cc,"::");
*/						break;
				}
				sprintf(c,"%12s",cc);
		}
		break;
	default:
		sprintf(c,"%12s",GetMessCode(_MESS_UNDEF+i));
		break;
	}
	xyLCD(0,3);
	puts_lcd(c);
	
	SetTask(CountDown,1,RTC_task);
	SetTask(CountBlink,30,PIT_task);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		CalcBallist(lib *l)
		{
double	NaklonMuzz(double);

		int			j;
		lib 		*p,*q,*r;
		char		c[32];
		double		dm,dr;

		p=blll;
		if(!p || !l)
			return(eof);
		p->xc = copycoord(l,p);
		p->xc->x = bll->xc->x;
		p->xc->y = bll->xc->y;
		p->xc->z = bll->xc->z;
		LlaToRel(p,p,l,NULL,AZ_R_dZ);
		j=null;
		dr=p->xc->x;

		do  {
			switch(Ballistic(l,p,crest)) {
				case 0:	if(j)
							warn(W_CHARGE_MODF);
						j=eof;
						break;

				case 1: if(j==2) {
							err(E_TARG_OUTR);
							j=eof;
							}
						else
							if(CP->charge) {
								--CP->charge;
								j=1;
								}
							else{
								err(E_TARG_UNDERR);
								j=eof;
								}
						break;
				case 2:
				case 3:
				case 4:
				case 5: if(j==1) {
							err(E_TARG_OUTR);
							j=eof;
							}
						else{
							if(!Guns[l->gun].mv[CP->ammo][++CP->charge]){
								--CP->charge;
								err(E_TARG_OVERR);
								j=eof;
								}
							else
								j=2;
							}
						break;
				case eof:sprintf(c,"%s ",E_CREST);
						LinkAddrText(crest->xc->refp,strchr(c,null));
						err(c);
						j=eof;
						break;
				}
			} while(j != eof);

		if(!corr) {				/* brez korekcije => izracun dV0	*/
			q=bll;				/* bll backup				*/
			r=blll;
			bll=blll=NULL;
			corr = -1;          /* preracun cilja na koord. zadetka	*/
			CalcTarget(&Mscan->c);	/* naredi se nov bll !!!		*/
			LlaToRel(bll,bll,l,NULL,AZ_R_dZ);
			dr = ScaleAngle(bll->xc->x - dr,maxmils,MR10,eof);
			dm = NaklonMuzz(bll->xc->y);
			dm *= l->xc->muzzle/1000.0 + 1.0;
			freelib(bll);
			bll=q;
			blll=r;
			copylib(l,q=makelib());	/* make temp. gun				*/
			q->xc->muzzle = bll->xc->muzzle = (dm - 1.0) * 1000;
			q->xc->refp = bll->xc->refp = l->xc->refp + dr;
			CalcBallist(q);   /* ponoven izracun balistike na dV0	*/
			freelib(q);
			corr = 0;
			}
		if(!CP->tof)
			p->x = p->xc = freecoord(p->x);
		return(null);
		}
/************************************************/
int		SetDMuzz(void *vp)
		{
		corr=0;
		return(null);
		}
/************************************************/
int		SetDGrid(void *vp)
		{
		corr=1;
		return(null);
		}
/************************************************/
int		SaveCorr(void *vp)
		{
		command	**cp = vp;
		lib     *l=FindLib((*cp)->to);
		if(l && bll && bll->x) {
			l->xc->muzzle = bll->xc->muzzle;
			l->xc->refp = bll->xc->refp;
			}
		return(null);
		}
/****************************************************/
/* IsectOff odpravi krizanje linij streljanja med	*/
/* l->x (orozje) in bll->x (cilj po distribuciji)	*/
/* s preurejanjem vrstnega reda v bll				*/
/* Objekta morata imeti enako stevilo koordinat !	*/
/*									 				*/
coord	*IsectOff(coord *a, coord *b)
		{
		coord *c = a->next;
		coord *d = b->next;
		coord temp;
		while(c && d) {
			if(Isect(a,b,c,d,NULL,NULL)) {
				temp=*d;
				*d=*b;
				d->next=temp.next;
				temp.next=b->next;
				*b=temp;
				IsectOff(a,b);
				}
			c = c->next;
			d = d->next;
			}
		if(a->next && b->next)
			IsectOff(a->next, b->next);
		return(b);
		}
/****************************************************/
/* preverjanje preseka daljic AB in CD				*/
/* return 0=> ni preseka else return eof			*/
int	Isect(coord *a,coord *b,coord *c,coord *d,double *xx,double *yy)
	{
	double x,y;

	double a1,k1 = a->x - b->x;
	double a2,k2 = c->x - d->x;

	if(!k1 && !k2)
		return(null);

	k1 = (a->y - b->y)/k1;
	k2 = (c->y - d->y)/k2;
	
	a1 = a->y - k1*a->x;
	a2 = c->y - k2*c->x;

	if(k1 == k2)
		return(null);
	
	x = (a2 - a1)/(k1 - k2);
	y = x*k1 + a1;

	if(xx)
		*xx = x;
	if(yy)
		*yy = y;

	if(	((x > a->x) && (x > b->x)) ||
		((x < a->x) && (x < b->x)) ||
		((x > c->x) && (x > d->x)) ||
		((x < c->x) && (x < d->x)))
		return(null);
	else
		return(eof);
	}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* Vrne prvo prosto stevilcno oznako za sporocilo na	*/
/* v skladu ->c											*/
int		CommEnum(command *c)/*							*/
		{                   /*                          */
		int		i=1;		/*							*/
		command *p=c,*q=c;	/*							*/
		if(!p)				/* ce je ->c NULL,          */
			return(i);		/* vrne #1					*/
		do	{
			if(p->n == i) {
				++i;
				q=p;
				}
			p=p->next;
			} while(p != q);
		return(i);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
double	ScaleAngle(double a, double m1, double m2, int sign)
		{
		double b = floor(a/m1*m2 + 0.5);
		if(sign){
			while(b >= m2/2)
				b -= m2;
			while(b < -m2/2)
				b += m2;
			}
		else{
			while(b >= m2)
				b -= m2;
			while(b < 0)
				b += m2;
			}
		return(b);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
		Message scanning:
Klice se ciklicno iz CheckLib() po preverjanju c->check !
Ce je editor odprt (cp->maxopt != 0), se v checksum stevje
se vsebina zaslona in aktivna vrstica !!!
ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	ListMenu(command *cp, menu *mp, int flag)
		{
		int		i,j,k,kk,key;
		char 	c[64],cc[32];
		mscan	*m;
        char	*p=(char *)cp;
		double  min,mid,max,A,B,C;
        lib		*l;
        gun		*g;

		if(p) {
            k=kk=cp->maxopt;
            if(k)
            	j=Mscan->active;
            else
            	j=0;
			i = sizeof(command) - sizeof(command *) - sizeof(int);
        	for(p = (char *)cp; i; --i)
				j = Crc(j,*p++);
            if(k)
				for(i=0;i<=k;++i)
		            for(p=Mscan->text[i]; *p; ++p)
						j = Crc(j,*p);
			if((cp->chk != abs(j)) || flag) {
				sprintf(cc,"...");
                if(mp == Cedit)
					sprintf(cc,"message");
                if(mp == edit_gun)
					sprintf(cc,"gun");
                if(mp == set_meteo)
					sprintf(cc,"meteo");
				if(k)
					sprintf(c,"=M %d,",Mscan->active+1);
				else {
					sprintf(c,"=M 0,");
					m=Mscan;            /* pointer backup			*/
					key = Key;			/* Ungetch save				*/
					Mscan=NULL;         /* prisili na inicializacijo*/
					comm_enable(&cp);   /* inic. nove strukture		*/
					lmenu(mp);
                    k=cp->maxopt;
					cp->maxopt=null;
					}
	            if(kk)
    	        	j=Mscan->active;
        	    else
            		j=0;
				i = sizeof(command) - sizeof(command *) - sizeof(int);
    	    	for(p = (char *)cp; i; --i)
					j = Crc(j,*p++);
            	if(kk)
					for(i=0;i<=k;++i)
		            	for(p=Mscan->text[i]; *p; ++p)
							j = Crc(j,*p);
				sprintf(strchr(c,0),"%04X,%04X,%s,",abs(cp->chk),abs(j),cc);
				puts_pc(c);
				cp->chk = abs(j);			/* update checksum			*/
				j = sizeof(command) - sizeof(command *) - sizeof(int);
                p = (char *)cp;
				for(i=0;i<j;++i) {
					sprintf(c,"%02X",*p++);
					puts_pc(c);
					}
				puts_pc("\r\n");
				sprintf(c,"%d,",cp->n);
				if(LinkAddrText(cp->from,cc))
					sprintf(strchr(c,0),"%s,",cc);
				else
					sprintf(strchr(c,0),",");
				if(LinkAddrText(cp->to,cc))
					sprintf(strchr(c,0),"%s,",cc);
				else
					sprintf(strchr(c,0),",");
				j=cp->rxmode;
 				sprintf(strchr(c,0),"%d",j);
				for(i=0; i<MAXGUN; ++i) {
					j=cp->txmode[i];
	 				sprintf(strchr(c,0),",%d",j);
                	}
                l=FindLib(cp->to);
                if(l && Guns) {
					g = &Guns[l->gun];
					if(checktype(l,TYPE_BT) && g->ABC) {
						A=g->ABC[cp->ammo][cp->charge][2];
						B=g->ABC[cp->ammo][cp->charge][1];
						C=g->ABC[cp->ammo][cp->charge][0];
						min = 2.0 * M_PI * g->lo / g->mils;
						mid= -B/2.0/A;
						max = 2.0 * M_PI * g->hi / g->mils;
						if(mid < min)
							mid = min;
						if(mid > max)
							mid = max;
						min = min*min*A + min*B + C;
						mid = mid*mid*A + mid*B + C;
						max = max*max*A + max*B + C;
						if(cp->traj)
							sprintf(strchr(c,0),",%.0lf,%.0lf",max,mid);
						else
							sprintf(strchr(c,0),",%.0lf,%.0lf",min,mid);
						}
	                else
    	            	sprintf(strchr(c,0),",,");
                    }
                else
                	sprintf(strchr(c,0),",,");
				sprintf(strchr(c,0),"\r\n");
				puts_pc(c);
				for(i=0;i<=k;++i) {
					puts_pc(Mscan->text[i]);
					puts_pc("\r\n");
					}
				if(!cp->maxopt) {
					memfree(Mscan);
					Mscan=m;           	/* restore Mscan			*/
					Key = key;
				}
				puts_pc("=M\r\n>");
				}
			}
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	CalcCrest(void *vp)
		{
		lib		*p;
		coord	*q;
		double	x,y;
		command 	**cp = vp;
		lib			*l=FindLib((*cp)->to);

		freelib(crest);
		crest=makelib();
		crest->ltype=addtype(TYPE_CR);

		if(LPP) {
			CheckItem(LL);
            p=LL;
            LL=bll;
			ListLib(LL);		/* izpis vzorca cilja na DMR kot LL */
            LL=p;
			puts_pc("?r ");
			putsLLA(l);
			puts_pc(" ");
			putsLLA(bll);
			puts_pc("\r\n>");
			SetTask(DMRtimeout,3,RTC_task);
			while(CheckTask(DMRtimeout,RTC_task)) {
				RTCservice();
				watchdog();
				}
			}

		p=Ln;
		do	{
			if(checktype(p,TYPE_CR)) {
				switch(targets_n(p->x)) {
					case 1:	add_coord(crest,p->xc->x,p->xc->y,p->xc->z);
							LlaToRel(crest,crest,l,NULL,AZ_R_dZ);
							crest->xc->refp=LinkAddr(p);
							break;
					default:for(q=p->x;q->next;q=q->next)
								if(Isect(q,q->next,l->xc,bll->xc,&x,&y)) {
									add_coord(crest,x,y,q->z);
									crest->xc->z += (x - q->x)/(q->next->x - q->x)
										 * (q->next->z - q->z);
									LlaToRel(crest,crest,l,NULL,AZ_R_dZ);
									crest->xc->refp=LinkAddr(p);
									}
							break;
					}
				}
			p=p->next;
			} while(p != Ln);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	DMRtimeout(void)
		{
		DeleteTask(DMRtimeout,RTC_task);
		}


