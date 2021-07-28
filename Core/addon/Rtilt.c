#include	"remmenu.h"

/*様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		InitLevel(void *vp)
		{
		shaft_enable();
		clear_screen(NULL);
		SetTask(DisplLevel,1,RTC_task);
		return(null);
		}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		CloseLevel(void *vp)
		{
		DeleteTask(DisplLevel,RTC_task);
		clear_screen(NULL);
		return(null);
		}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		DisableLevel(void *vp)
		{
		pitch=roll=0.0;
		return(null);
		}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		EnableLevel(void *vp)
		{
		double	p,r,az,el;

		Collect(&p,&r);

/* trenutni horiz. kot	=> q; za azimut							*/
/* uporabimo direktno odbiranje enk. da se ne uposteva az_off,	*/
/* ker se le-ta lahko kasneje spremeni pri isti nivelaciji		*/

		while(!shaft_ready());
		az = shaft_data[SHAFT_AZ] * SHtoR;
		el = (shaft_data[SHAFT_EL] - el_null) * SHtoR;

		while(el >= M_PI)
			el -= 2.0*M_PI;
		while(el < -M_PI)
			el += 2.0*M_PI;

		p -= el*cos(r);

/* zasuk nazaj (!!!) na nicelni azimut	  様様様様様様様様様様様様様*/

		pitch = p * cos(-az) - r * sin(-az);
		roll  = r * cos(-az) + p * sin(-az);

		return(null);
		}
/*--------------------------------------------------------------*/
void	TiltCompute(samples *ps, samples *rs)
{
double	*p,*r,pp,rr;
char c[256];

	if(n_samples(ps) >= 8)
	{
		clrlcd();
		xyLCD(0,2);
		puts_lcd("WAIT, PLEASE !");
		wait(25);
		p=solve(ps);
		r=solve(rs);
		if(p && r)
		{
/* izpis ripple
*/
			sprintf(c,"ripple:%5.1lf%c,%5.1lf%c",
				100.0*sqrt((p[2]*p[2]+p[3]*p[3]+p[5]*p[5]+p[6]*p[6])/(p[1]*p[1]+p[4]*p[4])),'%',
					100.0*sqrt((r[2]*r[2]+r[3]*r[3]+r[5]*r[5]+r[6]*r[6])/(r[1]*r[1]+r[4]*r[4])),'%');
			strncpy(error_screen[0],c,20);
/* kot in razlika v scale faktorjih
*/
			pp=atan2(p[1],p[4])-atan2(r[1],r[4])-M_PI/2.0;
			rr=sqrt(p[1]*p[1]+p[4]*p[4])/sqrt(r[1]*r[1]+r[4]*r[4]);
			cpr=pp;
			r_scale = p_scale*rr;
			sprintf(c,"p/r   :%6.1lf,%5.1lf%c",pp/2.0/M_PI*maxmils,100.0*rr,'%');
			strncpy(error_screen[2],c,20);
			/* offset values
*/
			pp=p_scale*p[0];
			rr=r_scale*r[0];
			p_null=pp;
			r_null=rr;
			sprintf(c,"offset:%6.1lf,%6.1lf",pp/2.0/M_PI*maxmils,rr/2.0/M_PI*maxmils);
			strncpy(error_screen[1],c,20);
			memfree(p);
			memfree(r);
			screen_swap(10*SWAP_T);
		}
	}
	else
		err("more samples !!!");
}
/*--------------------------------------------------------------*/
int		ResetLevel(void *vp)
{
		switch(Getch(CHLCD))
		{
		case LEFT:
			--az_align;
			break;
		case RIGHT:
			++az_align;
			break;
		case UP:
			++el_align;
			break;
		case DOWN:
			--el_align;
			break;
		case CLEAR:
			el_align = az_align=0;
			break;
		}
	return(null);
}
/*--------------------------------------------------------------*/
void	DisplLevel(void)
		{
		char	pc[16],rc[16],c[16];
		double	p,r;

		Collect(&p,&r);
		p += SHtoR*el_align*cos(r)-SHtoR*az_align*sin(r);
/* preveri veljaven obseg p & r 				*/

		if((fabs(p) <= 100.0/maxmils*2.0*M_PI) &&
			(fabs(r) <= 100.0/maxmils*2.0*M_PI)){

			sprintf(pc,"%6.1lf",p/2.0/M_PI*maxmils);
			sprintf(rc,"%6.1lf",r/2.0/M_PI*maxmils);
			}
		else
			{
			sprintf(pc,"      ");
			sprintf(rc,"      ");
			if(p > 100.0/maxmils*2.0*M_PI)
				sprintf(pc," %c %c %c",symARROWd,symARROWd,symARROWd);
			if(p < -100.0/maxmils*2.0*M_PI)
				sprintf(pc," %c %c %c",symARROWu,symARROWu,symARROWu);
			if(r > 100.0/maxmils*2.0*M_PI)
				sprintf(rc," %c %c %c",symARROWl,symARROWl,symARROWl);
			if(r < -100.0/maxmils*2.0*M_PI)
				sprintf(rc," %c %c %c",symARROWr,symARROWr,symARROWr);
			}
		sprintf(c,"%12s",T_PITCH_ROLL);
		xyLCD(0,0);
		puts_lcd(c);
		xyLCD(0,1);
		puts_lcd(pc);
		xyLCD(6,1);
		puts_lcd(rc);

		if(!pitch && !roll)
			sprintf(c,"%12s",T_LEVEL_OFF);
		else
			sprintf(c,"%12s",T_LEVEL_ON);
		xyLCD(0,3);
		puts_lcd(c);

		shaft_enable();
		SetTask(DisplLevel,1,RTC_task);
		}

/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		ShowPitch(void *vp)
		{
		char	c[64];
		double	p,r;
		int		i;

		static	int		a,n;
		static	samples *ps,*rs;

		Collect(&p,&r);
		sprintf(c,"%.1lf",p*maxmils/2.0/M_PI);
		msprint(c);
		
		if(Kbhit(CHLCD))
		{
			switch(Getch(CHLCD))
			{
			case LEFT:
				p_scale /= 1.001;
				r_scale /= 1.001;
				p_null  /= 1.001;
				r_null  /= 1.001;
				break;
			case RIGHT:
				p_scale *= 1.001;
				r_scale *= 1.001;
				p_null  *= 1.001;
				r_null  *= 1.001;
				break;
/*			case CLEAR:
				p_null += p * cos(-crp) - r * sin(-crp);
				r_null += r * cos(-crp) + p * sin(-crp);
				break;
*/			case REFRESH:
				if(ps && rs && shaft_data[SHAFT_AZ] == a)
				{
					if(!--n)
					{
						for(p=r=i=0; i<8; ++i)
						{
							p += p_filt[i]-p_off[i];
							r += r_filt[i]-r_off[i];
						}
					add_sample_t(ps,shaft_data[SHAFT_AZ]*SHtoR,p,2.0*M_PI);
					add_sample_t(rs,shaft_data[SHAFT_AZ]*SHtoR,r,2.0*M_PI);
					mess("next sample !!!");
					}
				}
				else
					n=5;
				a=shaft_data[SHAFT_AZ];
				break;
			case ALPHA:
				if(ps && rs)
				{
					TiltCompute(ps,rs);
					ps=freesamples(ps);
					rs=freesamples(rs);
				}
				else
				{
					ps=init_samples(ps,7);
					rs=init_samples(rs,7);
					t_ref=Tamb;
					a=-1;
					mess("sampling begin !!!");
				}
				break;
			}
		}
		return(null);
		}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		ToffTilt(void *vp)
		{
		char	c[64],cc;
		double	*p;
		if (Tiltmode)
		{
			p=&Rnull;
			cc='R';
		}
		else
		{
			p=&Pnull;
			cc='P';
		}

		if(Kbhit(CHLCD))
			switch(Getch(CHLCD)) {
				case ALPHA:	Tiltmode ^= 1;
							break;
				case SIGN:	*p *= -1;
							break;
				case CLEAR:	*p  = 0;
							break;
				case LEFT:	*p -= M_PI/180.0/3600.0;
							break;
				case RIGHT:	*p += M_PI/180.0/3600.0;
							break;
				}
		sprintf(c,"%.0lf %c/%cC %c",*p/M_PI*180.0*3600.0,'"',symDEGREE,cc);
		msprint(c);
		return(null);
		}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		TscaleTilt(void *vp)
		{
		char	c[64],cc;
		double	*p;
		if (Tiltmode)
		{
			p=&Rscale;
			cc='R';
		}
		else
		{
			p=&Pscale;
			cc='P';
		}

		if(Kbhit(CHLCD))
			switch(Getch(CHLCD)) {
				case ALPHA:	Tiltmode ^= 1;
							break;
				case SIGN:	*p *= -1;
							break;
				case CLEAR:	*p  = 0;
							break;
				case LEFT:	*p -= 0.0001;
							break;
				case RIGHT:	*p += 0.0001;
							break;
				}
		sprintf(c,"%.2lf %c/%cC %c",*p * 100.0,'%',symDEGREE,cc);
		msprint(c);
		return(null);
		}
/*------------------------------------------------------------------*/
void	CheckLobattSense(void)
		{
		if(BatteryOK())
			BacklitOn();
		else
			BacklitOff();
		}
/*------------------------------------------------------------------*/
int		ShowEncoder(void *vp)
{
	char	c[64],*s[]={"ROC415","ROC415G"," --- "};
	double	p,r,e;

	SetTask(CheckLobattSense,0,RTC_task);
	if(Kbhit(CHLCD))
	{
		switch(Getch(CHLCD))
		{
			case CLEAR:
				el_null=shaft_data[SHAFT_EL];
				break;
			case LEFT:
				shaft &= 0x7f;
				if(shaft)
					--shaft;
				break;
			case RIGHT:
				shaft &= 0x7f;
				if(shaft != NOSHAFT)
					++shaft;
				break;
		}
	}
	Collect(&p,&r);
	sprintf(c,"%-7s%c",s[shaft & 0x7f],':');
	strswap(c,":");
	SetMpos(0);
	msprint(c);
	e=(shaft_data[SHAFT_EL]-el_null) * SHtoR/M_PI/2.0*maxmils;
	while(e >= maxmils/2)
		e -= maxmils;
	while(e < -maxmils/2)
		e += maxmils;
	sprintf(c,"%.1lf",e * cos(r));
	msprint(c);
	return(null);
}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int		ShowRoll(void *vp)
		{
		char	c[46];
		double	p,r;

		Collect(&p,&r);
		if(Kbhit(CHLCD))
		{
			switch(Getch(CHLCD))
			{
			case LEFT:	
				crp -= 0.0001;
				break;
			case RIGHT:
				crp += 0.0001;
				break;
			case CLEAR:
				crp=0;
				break;
			}
		}
		sprintf(c,"%.1lf",r * maxmils / 2.0 / M_PI);
		msprint(c);
		return(null);
		}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
int	   	ShowTemp(void *vp)
		{
		char	c[16];
		if(Kbhit(CHLCD))
			if(Getch(CHLCD) == CLEAR) {
				readTEMP(0x0C,0xFF02);
				return(null);
				}
		sprintf(c,"%.1lf%cC",Tamb,symDEGREE);
		msprint(c);
		return(null);
		}
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
void	Collect(double *p, double *r) {
		int	n;
		double P,R,Po,Ro,dT;

        if(tcm2.status & 0x80) {
        	*p = tcm2.p/180.0*M_PI;
            *r = tcm2.r/180.0*M_PI;
            return;
            }

		dT = Tamb - t_ref;

		Po = p_null  + Pnull * dT;
		Ro = r_null  + Rnull * dT;

		for(P=R=0,n=0; n<8; ++n) {
			P += p_filt[n]-p_off[n];
			R += r_filt[n]-r_off[n];
			}
		P *= p_scale * (1 - Pscale * dT);
		R *= r_scale * (1 - Rscale * dT);

		P += R * sin(cpr);
		R  = R * cos(cpr);

		*p = (P-Po)*cos(crp) - (R-Ro)*sin(crp);
		*r = (R-Ro)*cos(crp) + (P-Po)*sin(crp);
		}

