#include	"remmenu.h"
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
lib		*GpsRef;
samples	*c;
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		GpsFixInit(void *vp)
		{
		LL->x=LL->xc=freecoord(LL->x);
		Displc(LL);
		GpsInit();
		GpsFix();
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		GpsFixClose(void *vp)
		{
		if(!GPS_CONTINOUS) {
			gps.status = 0;
			DeleteTask(GpsInit,RTC_task);
			GPS_off();
			C100_off();
			NAV=freelib(NAV);
			}
		ORG=GpsRef=freelib(GpsRef);
		d_mode=N_E_Z;
		DeleteTask(GpsFix,RTC_task);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetGpsRef(void *vp)
		{
		if(LL && LL->x && ORG)
			ORG=copylib(LL,GpsRef);
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		GpsFixMode(void *vp)
		{
		if(ORG) {
			ORG=NULL;
			d_mode=N_E_Z;
			}
		else{
			if(GpsRef) {
				ORG=GpsRef;
				d_mode=AZ_R_dZ;
				}
			else
				if(LL && LL->x) {
					GpsRef=makelib();
					ORG=copylib(LL,GpsRef);
					d_mode=AZ_R_dZ;
					}
			}
		Displc(LL);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	GpsFix(void)
		{
static	int		m;
		int		i;
		char	sGPS;

		if(NAV) {
			if(!LL->xc)
				if(!copylib(checklib(Ln,LL->ltype,LL->n),LL))
					add_coord(LL,0,0,0);
			LL->xc->x = NAV->x->x;
			LL->xc->y = NAV->x->y;
			LL->xc->z = NAV->x->z;
			LL->xc->t = NAV->x->t;
			}
		else
			LL->x = LL->xc = freecoord(LL->x);
		Displc(LL);

		switch(gps.gmode & 0xE0) {				/* LCD symbol select	*/
			case 0x40:							/* bits 6 & 7, gps.mode	*/
			case 0xC0:
						if((gps.status & 0x0f) == 0x01)
							sGPS=symdGPSf;
						else
							sGPS=symdGPS;
						break;
			case 0x00:
			case 0x80:	if((gps.status & 0x0f) == 0x01)
							sGPS=symGPSf;
						else
							sGPS=symDELTA;
						break;
			default:	sGPS='*';
						break;
			}
		for(i=0; i<6; ++i) {					/* symbol display		*/
			xyLCD(12+i/4,i%4);					/* acc. to the no. of	*/
			if(gps.sat[i] && (m%2))						/* satellites used !	*/
				Putch(sGPS,CHLCD);
			else
				Putch(' ',CHLCD);
			}
		++m;
		SetTask(GpsFix,1,RTC_task);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	ConCat(char *c,union uval u,size_t n)
		{
		int	i;
		for(i=0; i<n; ++i)
			sprintf(strchr(c,0),"%02X",u.bd[i]);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		GpsStatus(void *vp)
		{
		int		i;
		char	*p,c[32];

		sprintf(error_screen[0],"%c ",symGPSf);
		for(p=error_screen[0];*p;++p);
		for(i=0; i<6; ++i) {
			if(gps.sat[i])
				sprintf(p,"%3d",gps.sat[i]);
			else
				sprintf(p," ..");
			++p;++p;++p;
			}

		sprintf(c,"%d,",GPS_pdop & 0xf);
		switch(gps.gmode & 0xE0) {
			case 0x00:
			case 0x80:	strcat(c,"GPS,");
						break;
			case 0x40:
			case 0xC0:	strcat(c,"dGPS,");
						break;
			}
		switch(gps.gmode & 0x07) {
			case 0x00:	strcat(c,"undef");
						break;
			case 0x01:	strcat(c,"auto 0D");
						break;
			case 0x02:	strcat(c,"auto 2D");
						break;
			case 0x03:	strcat(c,"auto 3D");
						break;
			case 0x04:	strcat(c,"man 0D");
						break;
			case 0x05:	strcat(c,"man 2D");
						break;
			case 0x06:	strcat(c,"man 3D");
						break;
			}

		sprintf(error_screen[1],"mode:%15s",c);
		sprintf(error_screen[2]," P%.1f H%.1f V%.1f T%.1f",gps.pdop,gps.hdop,gps.vdop,gps.tdop);
		switch(gps.status & 0x0f) {
			case 0x01:	sprintf(error_screen[3],"      position fixed");
						break;
			case 0x02:	sprintf(error_screen[3],"       no satellites");
						break;
			case 0x04:	sprintf(error_screen[3],"        bad geometry");
						break;
			case 0x09:	sprintf(error_screen[3],"no usable satellites");
						break;
			case 0x0D:	sprintf(error_screen[3],"       bad satellite");
						break;
			default:	sprintf(error_screen[3],"                    ");
			}

		if(gps.error)
			err(E_GPS);
		else
			screen_swap(2*SWAP_T);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int   	Gps71(void)
		{
		char	c[128];
		union	uval u;

		sprintf(c,"71");				/* parameters code		*/
		u.bd[0] = 0;					/* disable filters   	*/
		ConCat(c,u,sizeof(char));
		u.fv = 0.0625;
		ConCat(c,u,sizeof(float));
		u.bd[0] = 1;
		ConCat(c,u,sizeof(char));
		u.bd[0] = 1;
		ConCat(c,u,sizeof(char));
		u.bd[0] = 0;
		ConCat(c,u,sizeof(char));
		u.bd[0] = 6;
		ConCat(c,u,sizeof(char));
		u.fv = 0.1;
		ConCat(c,u,sizeof(float));
		u.fv = 100.0;
		ConCat(c,u,sizeof(float));
		u.fv = 1.21;
		ConCat(c,u,sizeof(float));
		u.fv = 1000000.0;
		ConCat(c,u,sizeof(float));
		u.bd[0]=2;
		ConCat(c,u,sizeof(char));
		GpsComm(c);

		sprintf(c,"73");				/* parameters code		*/
		u.fv = 1.0;
		ConCat(c,u,sizeof(float));
		GpsComm(c);
        return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	CheckNav(void)
		{
		GpsComm("21");
		if(((gps.status & 0x0f) != 0x01) && NAV) {
			DeleteTask(CheckNav,RTC_task);
        	NAV = freelib(NAV);
            }
        }
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
#define	wgs_ax	6378137.0
#define	wgs_e2	0.00669437999013
#define D2R			(M_PI/180.0)
#define MILS2R		(2.0*M_PI/(double)maxmils)
#define arcmile		(M_PI/180.0/60.0)

/*
	Driver za Trimble; aktivira se og vklopu ARTES, ce je
	GPS mode continous (glej SETUP/EDIT) ali pa sele ob
	SURVEY/POSITION/GPS. V library formira spec. objekt NAV.
*/
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	GpsInit(void)
		{
		int		i;
		if(!GPS_on()) {		        /* Trimble power-on		*/
			C100_on();              /* prvi klic			*/
			gps.status=gps.gmode=gps.error=0;
			gps.pdop=gps.hdop=gps.vdop=gps.tdop=0;
			for(i=0; i<6; ++i)
				gps.sat[i]=0;
			SetDatum();
		}
		else{						/* naslednji klici		*/
			if((gps.status & 0x90) == 0x90) {
				gps.status &= ~0x90;		/* gps.status, bits 8,5 */
				if(!NAV) {                  /* Vklop nav. znaka		*/
					copylib(OBP,NAV=makelib());
					NAV->x = NAV->xc = freecoord(NAV->x);
					add_coord(NAV,0,0,0);	/* position				*/
					add_coord(NAV,0,0,0);   /* speed				*/
					add_coord(NAV,0,0,0);  	/* azimuth				*/
					}
				NAV->x->t.hour	=gps.hour;
				NAV->x->t.min	=gps.minute;
				NAV->x->t.sec	=gps.seconds;
				NAV->x->t.day	=gps.weekday;
				NAV->x->t.date	=gps.day-1;
				NAV->x->t.month	=gps.month-1;
				NAV->x->t.year	=gps.year % 100;
				addsecs(&NAV->x->t,GPS_dt * 3600.0);
				WriteTime(&NAV->x->t);

				xyz2lla(wgs_ax, wgs_e2,gps.x,gps.y,gps.z,
								&NAV->x->x,&NAV->x->y,&NAV->x->z);

				NAV->x->next->x =NAV->x->x;
				NAV->x->next->y =NAV->x->y;
				NAV->x->next->z =NAV->x->z;

				NAV->x->next->x += 1000.0 * gps.vx/wgs_ax;
				NAV->x->next->y += 1000.0 * gps.vy/wgs_ax;
				NAV->x->next->z += 1000.0 * gps.vz;

				GPS_lat=NAV->x->x/M_PI*180.0*3600.0 + 0.5;
				GPS_lon=NAV->x->y/M_PI*180.0*3600.0 + 0.5;
				GPS_alt=NAV->x->z;
/* Casovna kontrola NAV elementa
*/
				SetTask(CheckNav,10,RTC_task);
				GpsComm("21");
				}
/* Dodatek koord. grupe za kompas
*/
			if(NAV) {
				NAV->xc->x = NAV->x->x + arcmile * cos(MILS2R * C100comp());
				NAV->xc->y = NAV->x->y + arcmile * sin(MILS2R * C100comp());
				NAV->xc->z = NAV->x->z;
				}
			}
		SetTask(GpsInit,1,RTC_task);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
void	rGPSoff(void)
		{
        DeleteTask(rGPSoff,RTC_task);
		GpsComm("390100");
        gps.gmode &= ~0x20;
        }
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* C100 support												*/
/*															*/									
void	DisplCompass(void)
		{
		if(c100.status & 0x80)
		{
			c100.status &= ~0x80;
			if(!LL->xc)
				add_coord(LL,0,0,0);
			LL->xc->t=Time;
			LL->xc->x = read_EL();
			LL->xc->y = C100comp();
			LL->xc->z = ELAZ_defr;
			RelToLla(LL,LL,OBP,NULL,EL_AZ_R);
			Displc(LL);
			shaft_enable();
		}
		SetTask(DisplCompass,1,RTC_task);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		C100clear(void *vp)
		{
		bC100 = cC100 = 0.0;
		c=freesamples(c);
		mess("cleared");
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		C100sign(void *vp)
		{
		if(aC100) {
			aC100=0;
			mess("normal");
			}
		else{
			aC100=-1;
			mess("reversed");
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		C100align(void *vp)
		{
		double	*p;
		if(!c)
			c=init_samples(c,3);
		add_sample_t(c,read_EL()/maxmils,C100comp()/maxmils,1.0);
		if(n_samples(c) >= 3) {
			p=solve(c);
			c=freesamples(c);
			if(p) {
				bC100 = p[1];
				cC100 = p[2];
				mess("aligned");
				}
			else
				mess("not aligned");
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
double	C100comp(void)
		{
		double a,i,e = read_EL()/maxmils;

        if(tcm2.status & 0x80)
        	a = tcm2.c;
        else
        	a = c100.az;


		if(!aC100)
			i=(a + DeltaGrid()) / 360.0;
		else
			i=(DeltaGrid() - a) / 360.0;

		while(i < 0.0)
			i += 1.0;
		while(i >= 1.0)
			i -= 1.0;

		i -= bC100 * cos(e * 2.0 * M_PI) + cC100 * sin(e * 2.0 * M_PI);
/*		i -= bC100 * (cos(e * 2.0 * M_PI) - 1) + cC100 * sin(e * 2.0 * M_PI);
*/		return(i * maxmils);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		C100_enable(void *vp)
		{
		clear_screen(NULL);
		C100_on();
		LL->x=LL->xc=freecoord(LL->x);
		Displc(LL);
		SetTask(DisplCompass,1,RTC_task);
		shaft_enable();
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		C100_disable(void *vp)
		{
		C100_off();
		DeleteTask(DisplCompass,RTC_task);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
double	DeltaGrid(void)
{
	zone *z=FindZone(GPS_lat/3600.0*D2R,GPS_lon/3600.0*D2R);
	if(z)
		return(-sin(GPS_lat/180.0/3600.0*M_PI)*
					(GPS_lon/3600.0 - z->clon) + C100_decl/60.0);
	else
	{
		ZoneUndef=floor((GPS_lon/3600.0+(double)CM_sect/2.0-CM_off) / CM_sect);
		return(-sin(GPS_lat/180.0/3600.0*M_PI)*
			(GPS_lon/3600.0-ZoneUndef*CM_sect-CM_off)+ C100_decl/60.0);
	}
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadTrack(void *vp)
		{
		int		i;
		char	c[16];

		i = abs(ReadInt(track_delay));
		if(i > 3600) {
			err(E_MAX_VAL);
			return(ReadTrack(vp));
			}
		else
			track_delay=i;

		if(track_delay)
		{
			sprintf(c,"%d %s",i,M_SECOND);
			strswap(c," ");
			msprint(c);
			SetMpos(GetMpos()-5);
			}
		else
		{
			sprintf(c,"..");
			msprint(c);
		}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadPwoff(void *vp)
		{
		int		i;
		char	c[16];

		i = abs(ReadInt(pwoff));
		if(i > 3600) {
			err(E_MAX_VAL);
			return(ReadPwoff(vp));
			}
		else
			pwoff=i;

		if(pwoff)	{
					sprintf(c,"%d %s",i,M_SECOND);
					strswap(c," ");
					msprint(c);
					SetMpos(GetMpos()-5);
					}
		else		{
					sprintf(c,"..");
					msprint(c);
					}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetScreen(void *vp)
		{
		int		i;
		char	c[16];

		if(Kbhit(CHLCD)) {
        	i=Getch(CHLCD);
			switch(i) {
				case LEFT:	lcd=LcdLevel(--lcd);
							break;
				case RIGHT: lcd=LcdLevel(++lcd);
							break;
                default:   	Ungetch(i,0);
							i = abs(ReadInt(Backlit/2));
							if(i > 3600) {
								err(E_MAX_VAL);
                            	i /= 10;
								}
							Backlit=2*i;
                }
            }
		if(Backlit/2) {
			sprintf(c,"%d %s",Backlit/2,M_SECOND);
			strswap(c," ");
			msprint(c);
			SetMpos(GetMpos()-5);
			}
		else{
			sprintf(c,"..");
			msprint(c);
			}
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadPdop(void *vp)
		{
		int		i;
		char	c[16];

		i = abs(ReadInt(GPS_pdop & 0xf));
		if(i > 12) {
			err(E_MAX_VAL);
			return(ReadPdop(vp));
			}
		else
			GPS_pdop = (GPS_pdop & ~0xf) | i;

		sprintf(c,"%d",i);
		msprint(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadDgps(void *vp)
		{
		int		i;
		char	c[16];

		i = abs(ReadInt(dGPS));
		if(i > 60) {
			err(E_MAX_VAL);
			return(ReadDgps(vp));
			}
		else
			dGPS = i;

        if(i)
			sprintf(c,"%d s",i);
        else
        	sprintf(c,"off");
		msprint(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadGpsoff(void *vp)
		{
		int		i;
		char	c[16];

		i = ReadInt(*(int *)vp);
		if(abs(i) > 10000) {
			err(E_MAX_VAL);
			return(ReadGpsoff(vp));
			}
		else
			*(int *)vp=i;
			if((int *)vp == &GPS_dt)
				sprintf(c,"%d %s",i,M_HOUR);
			else
				sprintf(c,"%d %s",i,M_METER);
		strswap(c," ");
		msprint(c);
		SetMpos(GetMpos()-2);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadGpsLat(void *vp)
		{
		char	c[16];
		long	*i=&GPS_lat;

		*i = IncLong(i,600);
		if(labs(*i) > 90L*3600L) {
			err(E_MAX_VAL);
			*i = 90L*3600L*lsign(*i);
			}
		if(*i >= 0)
			sprintf(c,"%ld%c%02ld'%02ld%c %s",labs(*i)/3600,symDEGREE,(labs(*i)%3600)/60,labs(*i)%60,'"',ABBRV_NORTH);
		else
			sprintf(c,"%ld%c%02ld'%02ld%c %s",labs(*i)/3600,symDEGREE,(labs(*i)%3600)/60,labs(*i)%60,'"',ABBRV_SOUTH);
		strswap(c," ");
		msprint(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadGpsLon(void *vp)
		{
		char	c[16];
		long	*i=&GPS_lon;

		*i = IncLong(i,600);
		if(labs(*i) > 180L*3600L) {
			err(E_MAX_VAL);
			*i = 180L*3600L*lsign(*i);
			}
		if(*i >= 0)
			sprintf(c,"%ld%c%02ld'%02ld%c %s",labs(*i)/3600,symDEGREE,(labs(*i)%3600)/60,labs(*i)%60,'"',ABBRV_EAST);
		else
			sprintf(c,"%ld%c%02ld'%02ld%c %s",labs(*i)/3600,symDEGREE,(labs(*i)%3600)/60,labs(*i)%60,'"',ABBRV_WEST);
		strswap(c," ");
		msprint(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ReadDecl(void *vp)
		{
		float	MainFld(float,float,float,float);
		char	c[16];
		int		*i=&C100_decl,j;
		rtime   t;
		double	tt;

		if(Kbhit(CHLCD)) {
			j=Getch(CHLCD);
			if(j==CLEAR && !C100_decl) {
				t=Time;
				tt=t.year + (t.month + t.day/30.0)/12.0;
				tt += 2000.0;
				C100_decl=MainFld(GPS_lat/3600.0,GPS_lon/3600.0,GPS_alt,tt);
				}
			else
				Ungetch(j,0);
			}
		IncInt(i);
		if(*i < 0)
			sprintf(c,"%c%d%c%02d'",'-',abs(*i/60),symDEGREE,abs(*i%60));
		else
			sprintf(c,"%d%c%02d'",abs(*i/60),symDEGREE,abs(*i%60));
		strswap(c,"-");
		msprint(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ShowDgrid(void *vp)
		{
		char	c[32];
		long	i=DeltaGrid() * 3600.0;
		if(i >= 0)
			sprintf(c,"%ld%c%02ld'%02ld%c %s",labs(i)/3600,symDEGREE,(labs(i)%3600)/60,labs(i)%60,'"',ABBRV_EAST);
		else
			sprintf(c,"%ld%c%02ld'%02ld%c %s",labs(i)/3600,symDEGREE,(labs(i)%3600)/60,labs(i)%60,'"',ABBRV_WEST);
		strswap(c," ");
		msprint(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*	packet 2C, page A-29										*/
/*  key setup parameters, page A-15								*/

int		Gps2C(void)
		{
		char	c[64];
		union	uval u;
		float	f=GPS_pdop & 0xf;

		sprintf(c,"2C01");				/* 2C + dynamics = land	*/
		u.fv = 0.1745;					/* elev. mask 10 deg.	*/
		ConCat(c,u,sizeof(float));
		u.fv = 6.0;						/* sig. mask			*/
		ConCat(c,u,sizeof(float));
		u.fv = f;						/* PDOP mask			*/
		ConCat(c,u,sizeof(float));
		u.fv = 8.0;						/* PDOP switch, ignore!	*/
		ConCat(c,u,sizeof(float));
		GpsComm(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Gps2E(void)
		{
		rtime	t;
		char	c[64];
		union	uval u;
		float	i,j;

		t=Time;
		i=calc_week(&t);
		j=calc_secs(&t) - GPS_dt * 3600.0;
		sprintf(c,"2E");
		u.fv=j;
		ConCat(c,u,sizeof(float));
		u.iv=i;
		ConCat(c,u,sizeof(int));
		GpsComm(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Init8pt(void *vp)
		{
		puts_C100("=ce1\r");
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Enter8pt(void *vp)
		{
		puts_C100("=ce1\r");
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Init3pt(void *vp)
		{
		puts_C100("=ce4,0\r");
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		Enter3pt(void *vp)
		{
		char	c[32];
		shaft_enable();
		if(!aC100)
			sprintf(c,"=ce4,%5.1f\r",fmod(read_AZ()/maxmils*360.0+360.0-DeltaGrid(),360.0));
		else
			sprintf(c,"=ce4,%5.1f\r",fmod(360.0 - read_AZ()/maxmils*360.0-DeltaGrid(),360.0));
		puts_C100(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		InitRot(void *vp)
		{
		puts_C100("=ce3\r");
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		AbortKVHcal(void *vp)
		{
		puts_C100("=cez\r");
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		InitKVH(void *vp)
		{
		Baud(CHC100,4800);
		wait(25);

		puts_C100("=t,1\r");	/* set for KVH format			*/
		wait(25);
		puts_C100("=i,d\r");	/* set for degrees				*/
		wait(25);
		puts_C100("=dt0,3\r");	/* internal double IIR filter	*/
		wait(25);
		puts_C100("=d0,9\r"); 	/* IIR filter time 24 secs		*/
		wait(25);
		puts_C100("=b,096\r");	/* set 9600						*/
		wait(25);
		puts_C100("=r,60\r");	/* set for 1Hz output rate		*/
		wait(25);
		puts_C100("s\r");		/* output enable				*/
		wait(25);
		puts_C100("zap\r");		/* warm startup					*/
		wait(150);
		Baud(CHC100,9600);

		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		AirTemp(void *vp)
		{
		char	c[16];
		int		*i=&meteo[AIR_T][0];

		*i = ReadInt(*i);
		if(abs(*i) > 100) {
			err(E_MAX_VAL);
			*i /= 10;
			}
		sprintf(c,"%d %cC",*i,symDEGREE);
		strswap(c," ");
		msprint(c);
		SetMpos(GetMpos()-3);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		AirPress(void *vp)
		{
		char	c[16];
		int		*i=&meteo[AIR_P][0];

		*i = abs(ReadInt(*i));
		if(*i > 1500) {
			err(E_MAX_VAL);
			*i /= 10;
			}
		sprintf(c,"%d mm",*i);
		strswap(c," ");
		msprint(c);
		SetMpos(GetMpos()-3);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		WindDir(void *vp)
		{
		char	c[16];
		int		i=(uint)meteo[WIND_D][0]/64000.0*(double)maxmils + 0.5;
		i = abs(ReadInt(i));
		if(i >= maxmils) {
			err(E_MAX_VAL);
			i /= 10.0;
			}
		meteo[WIND_D][0]=(double)i/maxmils*64000.0;
		sprintf(c,"%d%c%02d",i/100,'-',i%100);
		msprint(c);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		WindSpeed(void *vp)
		{
		char	c[16];
		int		*i=&meteo[WIND_S][0];

		*i = ReadInt(*i);
		if(*i > 100) {
			err(E_MAX_VAL);
			*i /= 10;
			}
		sprintf(c,"%d m/s",*i);
		strswap(c," ");
		msprint(c);
		SetMpos(GetMpos()-4);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	   	AirHum(void *vp)
		{
		char	c[16];
		int		*i=&meteo[AIR_H][0];

		*i = abs(ReadInt(*i));
		if(*i > 100) {
			err(E_MAX_VAL);
			*i /= 10;
			}
		sprintf(c,"%d %c",*i,'%');
		strswap(c," ");
		msprint(c);
		SetMpos(GetMpos()-2);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		SetHeight(void *vp)
		{
		char	c[16];
		int		*i=&meteo[HEIGHT][0];

		*i = abs(ReadInt(*i));
		if(*i > 10000) {
			err(E_MAX_VAL);
			*i /= 10;
			}
		sprintf(c,"%d m",*i);
		strswap(c," ");
		msprint(c);
		SetMpos(GetMpos()-2);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		InitMeteo(void *vp)
		{
		meteo[HEIGHT][0]=0;
		meteo[AIR_T][0]=15;
		meteo[AIR_P][0]=750;
		meteo[AIR_H][0]=50;
		meteo[WIND_S][0]=0;
		meteo[WIND_D][0]=0;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	   	SetTime(void *vp)
		{
		char	c[16];
		int		i;

		i=ReadHMS(&Time) + GetMpos();
		WriteTime(&Time);
		sprintf(c,"%02d:%02d:%02d",Time.hour,Time.min,Time.sec);
/*		strswap(c,"::");
*/		msprint(c);
		SetMpos(i);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	   	SetDate(void *vp)
		{
		char	c[16];
		int		i;

		i=ReadDMY(&Time) + GetMpos();
		WriteTime(&Time);
		sprintf(c,"%02d%c%s%c%02d",Time.date+1,'-',GetMessCode(_MONTH_1+Time.month),'-',Time.year);
		strswap(c,"--");
		msprint(c);
		SetMpos(i);
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		GPS_pmode(void *vp)
		{
		if(Kbhit(CHLCD)) {
			GPS_pdop &= ~0x0f00;
			GPS_pdop |= curr_menu->opt << 8;
			if(GPS_CONTINOUS)
				GpsInit();
		}
		else
			curr_menu->opt = (GPS_pdop >> 8) & 0xf;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		GPS_dmode(void *vp)
		{
		if(Kbhit(CHLCD)) {
			GPS_pdop &= ~0xf000;
			GPS_pdop |= curr_menu->opt << 12;
			}
		else
			curr_menu->opt = (GPS_pdop >> 12) & 0xf;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		RLD_tmode(void *vp)
		{
		if(Kbhit(CHLCD)) {
			rld_mode &= ~0x100;
			rld_mode |= curr_menu->opt << 8;
			}
		else
			curr_menu->opt = (rld_mode >> 8) & 0x001;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		RLD_irmode(void *vp)
		{
		if(Kbhit(CHLCD)) {
			rld_mode &= ~0x200;
			rld_mode |= curr_menu->opt << 9;
			}
		else
			curr_menu->opt = (rld_mode >> 9) & 0x001;
		return(null);
		}
