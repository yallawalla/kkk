#define	GPSDEF	1
#define sp	sprintf

#define DLE 0x10
#define ETX 0x03
#define D2R 3.141592654/180.0
#define R2D 180.0/3.141592654


unsigned char pktbuff[250];
/* unsigned char bfr[82];
*/
int rxcnt = 0;
int last_id = 0;
int have_dle = 0, idcode = 0;

void DecodeBlock_gps (int i)
{
	double	t;
	char	c[128];
	if (!have_dle) {
		if (i == DLE) {		/*  first DLE  */
			have_dle = 1;
			return;
			}
		if (rxcnt < 250)
			pktbuff[rxcnt++] = i;	/*  plain data byte  */
		return;
		}
	if (i == DLE) {		/* double DLE is data DLE  */
		if (rxcnt < 250)
		pktbuff[rxcnt++] = DLE;
		have_dle = 0;
		return;
		}
	if (i != ETX)  {		/*  start new packet  */
		rxcnt = 0;
		idcode = i;
		have_dle = 0;
		return;
		}
/*	echo iz GPS na PC port, ce je MSB na GPS_pdop true!
	Lahko se nastavi samo iz PC porta !!!
*/
	if(GPS_ECHO)
	{
		sprintf(c,".%02X",idcode);
		puts_pc(c);
		for(i=0; i<rxcnt; ++i) {
			sprintf(c,"%02X",pktbuff[i]);
			puts_pc(c);
			}
		puts_pc("\r\n");
	}

	switch (idcode)  {
/*------------------------------------------------------------------*/
/* software ID															*/
		case 0x45:
			if (rxcnt == 10)
			{
				GpsComm("2204");		/* man 3D mode			*/
				GpsComm("6202");		/* auto diff. mode		*/
				GpsComm("3511021100");	/* I/O options,page A32	*/
				GpsComm("24");
				GpsComm("26");
				GpsComm("37");
				Gps2C();				/* dynamics, elev.		*/
				Gps2E();    			/* init. time			*/
				Gps71();    			/* reset filter			*/
			}
			break;	
/*------------------------------------------------------------------*/
/* health															*/
		case 0x46:
			if (rxcnt == 2)
			{
				gps.status = (gps.status & 0xf0) | (pktbuff[0] & 0x0f)+1;
				gps.error = pktbuff[1];
			}
			break;
/*------------------------------------------------------------------*/
/* best-4 select CM3 manual, page A-44								*/
/* updates gps.gmode, bits 0-2										*/
/*																	*/
		case 0x44:
			if (rxcnt == 21) {
				gps.gmode &= ~0x07;
				switch(pktbuff[0]) {
					case 0x01:	gps.gmode |= 1;
								break;
					case 0x03:	gps.gmode |= 2;
								break;
					case 0x04:	gps.gmode |= 3;
								break;
					case 0x11:	gps.gmode |= 4;
								break;
					case 0x13:	gps.gmode |= 5;
								break;
					case 0x14:	gps.gmode |= 6;
								break;
					}
				gps.sat[0]=pktbuff[1];
				gps.sat[1]=pktbuff[2];
				gps.sat[2]=pktbuff[3];
				gps.sat[3]=pktbuff[4];
				gps.sat[4]=0;
				gps.sat[5]=0;
				gps.pdop = getfloat(&pktbuff[5]);
				gps.hdop = getfloat(&pktbuff[9]);
				gps.vdop = getfloat(&pktbuff[13]);
				gps.tdop = getfloat(&pktbuff[17]);
				}
			break;
/*------------------------------------------------------------------*/
/* best-6 select CM3 manual, page A-74								*/
/* updates gps.gmode, bits 0-2										*/
/*																	*/
		case 0x6D:
			if (rxcnt >= 17) {
				gps.gmode &= ~0x07;
				switch(pktbuff[0] & 0x0f) {
					case 0x03:	gps.gmode |= 2;
								break;
					case 0x04:	gps.gmode |= 3;
								break;
					case 0x0b:	gps.gmode |= 5;
								break;
					case 0x0c:	gps.gmode |= 6;
								break;
					}
				gps.pdop = getfloat(&pktbuff[1]);
				gps.hdop = getfloat(&pktbuff[5]);
				gps.vdop = getfloat(&pktbuff[9]);
				gps.tdop = getfloat(&pktbuff[13]);

			for(i=0; i<6; ++i)
				gps.sat[i]=0;
			for(i=0; i < (pktbuff[0] >> 4); ++i)
					gps.sat[i]=pktbuff[17+i];
				}
			break;
/*------------------------------------------------------------------*/
/* diff. mode CM3 manual, page A80									*/
/* updates diff. mode, bits 6-7										*/
/*																	*/
		case 0x82:
			gps.gmode = (gps.gmode & ~0xC0) | (pktbuff[0]<<6);
			break;
/*------------------------------------------------------------------*/
/* GPS time, page 43												*/
		case 0x41:
			if ((rxcnt == 10) || (rxcnt == 12))  {
				t = getfloat(&pktbuff[0]);			/* time of week */
				if(calc_time(t)) {
					gps.week = getint(&pktbuff[4]);		/* week number */
					calc_date (gps.week);
					if (rxcnt == 10)  {
						gps.UTCoffset = getfloat(&pktbuff[6]);	/* UTC offset in seconds */
						}
					gps.status |= 0x10;
					}
				}
			break;
/*------------------------------------------------------------------
		case 0x42:
			if (rxcnt == 16)  {
				getfloat(&pktbuff[0]);
				gps.x = uval.fv;
				getfloat(&pktbuff[4]);
				gps.y = uval.fv;
				getfloat(&pktbuff[8]);
				gps.z = uval.fv;
				gps.status |= 0x80;
			}
			break;
------------------------------------------------------------------
		case 0x4A:
			if (rxcnt == 20)  {
				getfloat(&pktbuff[0]);
				gps.lat = uval.fv;
				getfloat(&pktbuff[4]);
				gps.lon = uval.fv;
				getfloat(&pktbuff[8]);
				gps.alt = uval.fv;
				gps.status |= 0x40;
			}
			break;
------------------------------------------------------------------*/
/* single precision ENU velocities									*/
		case 0x56:
			if (rxcnt == 20)  {
				gps.vy = getfloat(&pktbuff[0]);
				gps.vx = getfloat(&pktbuff[4]);
				gps.vz = getfloat(&pktbuff[8]);
				gps.status |= 0x20;
			}
			break;
/*------------------------------------------------------------------*/
/* double precision XYZ position and bias							*/
		case 0x83:
			if (rxcnt == 36)  {
				gps.x = getdouble(&pktbuff[0]);
				gps.y = getdouble(&pktbuff[8]);
				gps.z = getdouble(&pktbuff[16]);
				gps.status |= 0x80;
			}
			break;
/*------------------------------------------------------------------
		case 0x84:
			if (rxcnt == 36)  {
				getdouble(&pktbuff[0]);
				gps.lat = uval.dv;
				getdouble(&pktbuff[8]);
				gps.lon = uval.dv;
				getdouble(&pktbuff[16]);
				gps.alt = uval.dv;
				gps.status |= 0x40;
			}
			break;
------------------------------------------------------------------*/
	}
	last_id = idcode;
	rxcnt = idcode = have_dle = 0;
}

/*
void calc_date_o (uint wk)
{
	uint i, days, weeks, d = 0;
	uint monthdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	gps.week = wk;
	weeks = wk % 52;
	days = --weeks * 7;	
	gps.year = 1980 + wk / 52;

	i = gps.year;
	if ( (i % 4) == 0 && (i % 100) != 0 || (i % 400) == 0 )
		monthdays[1] = 29;

	for (i = 0; i < 12; i++) {
		d += monthdays[i];
		gps.month = i + 1;
		if (d > days) break;
	}

	d -= monthdays[i];
	gps.day = days - d - 5 + gps.weekday;
}
*/
void calc_date (uint wk)
	{
	int		i,j;
	double	days;
	uint monthdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	gps.week = wk;
	days = wk * 7.0 + gps.weekday + 5.0;

	for(i=0; days >= 0; ++i) {
		j = 1980 + i/12;
		if ( (j % 4) == 0 && (j % 100) != 0 || (j % 400) == 0 )
			monthdays[1] = 29;			/* step-year */
		else
			monthdays[1] = 28;
		days -= monthdays[i%12];
		}
	days += monthdays[--i%12];
	gps.year  = 1980 + i/12;
	gps.month = i % 12 + 1;
	gps.day = days + 1.0;
}

int	 calc_time (double t)
{
	if (fabs(t) >= 604800.0 || t < 0)
		return(0);	/* Bad time */

	gps.weekday = t / 86400.0;
    t -= (gps.weekday * 86400.0);
    gps.hour = t / 3600.0;
    t -= (gps.hour * 3600.0);
    gps.minute = t / 60.0;
    gps.seconds = (int)(t - gps.minute * 60.0 + 0.5);
	return(-1);
}

