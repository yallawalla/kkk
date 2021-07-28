#include	"remmenu.h"
/*--------------------------------------------------------*/
datum dat[] = 
{
{"WGS-84","WGS-84",0,0,0,0,0},
{"Adindan","Clarke1880",-112.145,-.54750714,-166,-15,204},
{"Afgooye","Krassovsky",-108,.00480795,-43,-163,45},
{"AinElAbd1970","Int'national",-251,-.14192702,-150,-251,-2},
{"AlaskaNAD-27","Clarke1866",-69.4,-.37264639,-5,135,172},
{"CanadaNAD-27","Clarke1866",-69.4,-.37264639,-9,151,185},
{"Anna1Astro65","AusNational",-23,-.00081204,-491,-22,435},
{"ARC-1950mean","Clarke1880",-112.145,-.54750714,-143,-90,-294},
{"ARC-1960mean","Clarke1880",-112.145,-.54750714,-160,-8,-300},
{"Ascension 58","Int'national",-251,-.14192702,-207,107,52},
{"AstroB4Sor","Int'national",-251,-.14192702,114,-116,-333},
{"AstroBeacon","Int'national",-251,-.14192702,145,75,-272},
{"AstroPos71","Int'national",-251,-.14192702,-320,550,-494},
{"Astronomic52","Int'national",-251,-.14192702,124,-234,-25},
{"Aus 1984","AusNational",-23,-.00081204,-134,-48,149},
{"BahamaNAD-27","Clarke1866",-69.4,-.37264639,-4,154,178},
{"Bellevue IGN","Int'national",-251,-.14192702,-127,-769,472},
{"Bermuda 1957","Clarke1866",-69.4,-.37264639,-73,213,296},
{"Bogota Obs.","Int'national",-251,-.14192702,307,304,-318},
{"BukitRimpah","Bessel1841",739.845,.10037483,-384,664,-48},
{"CampArea","Int'national",-251,-.14192702,-104,-129,239},
{"CampoInchaus","Int'national",-251,-.14192702,-148,136,90},
{"CanadaMean","Clarke1866",-69.4,-.37264639,-10,158,187},
{"CanalZone","Clarke1866",-69.4,-.37264639,0,125,201},
{"CantonIsland","Int'national",-251,-.14192702,298,-304,-375},
{"Cape","Clarke1880",-112.145,-.54750714,-136,-108,-292},
{"CapeCanavera","Clarke1866",-69.4,-.37264639,-2,150,181},
{"Carribean","Clarke1866",-69.4,-.37264639,-7,152,178},
{"Carthage","Clarke1880",-112.145,-.54750714,-263,6,431},
{"CentralAmeri","Clarke1866",-69.4,-.37264639,0,125,194},
{"Chatham1971","Int'national",-251,-.14192702,175,-38,113},
{"ChuaAstro","Int'national",-251,-.14192702,-134,229,-29},
{"CorregoAlegr","Int'national",-251,-.14192702,-206,172,-6},
{"Cuba NAD27","Clarke1866",-69.4,-.37264639,-9,152,178},
{"Cyprus","Int'national",-251,-.14192702,-104,-101,-140},
{"Djakarta","Bessel1841",739.845,.10037483,-377,681,-50},
{"DOS1968","Int'national",-251,-.14192702,230,-199,-752},
{"Easterlsland","Int'national",-251,-.14192702,211,147,111},
{"Egypt","Int'national",-251,-.14192702,-130,-117,-151},
{"European1950","Int'national",-251,-.14192702,-87,-98,-121},
{"European1979","Int'national",-251,-.14192702,-86,-98,-119},
{"FinnishN'cal","Int'national",-251,-.14192702,-78,-231,-97},
{"GandajikaBas","Int'national",-251,-.14192702,-133,-321,50},
{"Geodetic 49","Int'national",-251,-.14192702,84,-22,209},
{"Ghana","WGS-84",0,0,0,0,0},
{"Greenland","Clarke1866",-69.4,-.37264639,11,114,195},
{"Guam1963","Clarke1866",-69.4,-.37264639,-100,-248,259},
{"GunungSegara","Bessel1841",739.845,.10037483,-403,684,41},
{"GunungSer.62","WGS-84",0,0,0,0,0},
{"GUX1Astro","Int'national",-251,-.14192702,252,-209,-751},
{"HeratNorth","Int'national",-251,-.14192702,-333,-222,114},
{"Hermannskogl","Bessel1841",739.845,.10037483,575,93,466},
{"Hjorsey1955","Int'national",-251,-.14192702,-73,46,-86},
{"HongKong1963","Int'national",-251,-.14192702,-156,-271,-189},
{"Hu-Tzu-Shan","Int'national",-251,-.14192702,-634,-549,-201},
{"Indian","Everest",860.655,.28361368,289,734,257},
{"Iran","Int'national",-251,-.14192702,-117,-132,-164},
{"Ireland1965","Mod.Airy",796.811,.11960023,506,-122,611},
{"ISTS073Ast69","Int'national",-251,-.14192702,208,-435,-229},
{"JohnstonIs61","Int'national",-251,-.14192702,191,-77,-204},
{"Kandawala","Everest",860.655,.28361368,-97,787,86},
{"Kerguelen","Int'national",-251,-.14192702,145,-187,103},
{"Kertau '48","Mod.Everest",832.937,.28361368,-11,851,5},
{"L.C.5Astro","Clarke1866",-69.4,-.37264639,42,124,147},
{"LaReunion","Int'national",-251,-.14192702,94,-948,-1262},
{"Liberia1964","Clarke1880",-112.145,-.54750714,-90,40,88},
{"Luzon","Clarke1866",-69.4,-.37264639,-133,-77,-51},
{"Mahe1971","Clarke1880",-112.145,-.54750714,41,-220,-134},
{"MarcoAstro","Int'national",-251,-.14192702,-289,-124,60},
{"Masirah Isl.","Clarke1880",-112.145,-.54750714,-247,-148,369},
{"Massawa","Bessel1841",739.845,.10037483,639,405,60},
{"Merchich","Clarke1880",-112.145,-.54750714,31,146,47},
{"Mexico NAD27","Clarke1866",-69.4,-.37264639,-12,130,190},
{"MidwayAstro","Int'national",-251,-.14192702,912,-58,1227},
{"Mindanao","Clarke1866",-69.4,-.37264639,-133,-79,-72},
{"Minna","Clarke1880",-112.145,-.54750714,-92,-93,122},
{"MontjongLowe","WGS-84",0,0,0,0,0},
{"Nahrwan","Clarke1880",-112.145,-.54750714,-231,-196,482},
{"NaparimaBWI","Int'national",-251,-.14192702,-2,374,172},
{"NorthAm 83","GRS80",0,-.00000016,0,0,0},
{"NorthAm 27m","Clarke1866",-69.4,-.37264639,-8,160,176},
{"Observat. 66","Int'national",-251,-.14192702,-425,-169,81},
{"OldEgyptian","Helmert1906",-63,.00480795,-130,110,-13},
{"OldHaw. mean","Clarke1866",-69.4,-.37264639,89,-279,-183},
{"OldHaw.Kauai","Clarke1866",-69.4,-.37264639,45,-290,-172},
{"OldHaw. Maui","Clarke1866",-69.4,-.37264639,65,-290,-190},
{"OldHaw. Oahu","Clarke1866",-69.4,-.37264639,56,-284,-181},
{"Oman","Clarke1880",-112.145,-.54750714,-346,-1,224},
{"Ord.Survy 36","Airy",573.604,.11960023,375,-111,431},
{"PicoDeLasNvs","Int'national",-251,-.14192702,-307,-92,127},
{"Pitcairn 67","Int'national",-251,-.14192702,185,165,42},
{"Potsdam DHDN","Bessel1841",739.845,.10037483,606,23,413},
{"Prov.SouthAm","Int'national",-251,-.14192702,-288,175,-376},
{"Prov.SouthCh","Int'national",-251,-.14192702,16,196,93},
{"PuertoRico","Clarke1866",-69.4,-.37264639,11,72,-101},
{"Pulkovo1942","Krassovsky",-108,.00480795,28,-130,-95},
{"Qornoq","Int'national",-251,-.14192702,164,138,-189},
{"QuatarNat'l","Int'national",-251,-.14192702,-128,-283,22},
{"Rome1940","Int'national",-251,-.14192702,-225,-65,9},
{"S42","Krassovsky",-108,.00480795,28,-121,-77},
{"S.E.Asia","Everest",860.655,.28361368,173,750,264},
{"SAD-69Brazil","SouthAm 1969",-23,-.00081204,-60,-2,-41},
{"SantaBraz","Int'national",-251,-.14192702,-203,141,53},
{"Santo(DOS)","Int'national",-251,-.14192702,170,42,84},
{"SapperHill43","Int'national",-251,-.14192702,-355,16,74},
{"Schwarzeck","Namibia",653.135,.10037483,616,97,-251},
{"Sicily","Int'national",-251,-.14192702,-97,-88,-135},
{"SierraLeone","WGS-84",0,0,0,0,0},
{"Slovenia","Bessel1841",739.845,.10037483,1731,91,-582},
{"SouthAm 69","SouthAm 1969",-23,-.00081204,-57,1,-41},
{"SouthAsia","Fischer 1960",-18,.00480795,7,-10,-26},
{"SoutheastBas","Int'national",-251,-.14192702,-499,-249,314},
{"SouthwestBas","Int'national",-251,-.14192702,-104,167,-38},
{"Tananarive25","Int'national",-251,-.14192702,-189,-242,-91},
{"Thai/Viet","Everest",860.655,.28361368,214,836,303},
{"Timbalai1948","Everest",860.655,.28361368,-689,691,-45},
{"Tokyomean","Bessel1841",739.845,.10037483,-128,481,664},
{"TristanAstro","Int'national",-251,-.14192702,-632,438,-609},
{"UAE(Nahrwan)","Clarke1880",-112.145,-.54750714,-249,-156,381},
{"VitiLevu1916","Clarke1880",-112.145,-.54750714,51,391,-36},
{"WakeEniwetok","Hough",-133,-.14192702,101,52,-39},
{"WGS-72","WGS-72",2,.0003121057,0,0,5},
{"Yacare","Int'national",-251,-.14192702,-155,171,37},
{"Zanderij","Int'national",-251,-.14192702,-265,120,-358},
{NULL,"WGS-84",0,0,0,0,0}
};
/*--------------------------------------------------------*/
#define R2D		(180.0/M_PI)
#define D2R		(M_PI/180.0)
#define	arcmile	1.0/60.0*D2R
#define k0		0.9996
#define	wgs_ax	6378137.0
#define	wgs_e2	0.00669437999013
#define	wgsf	(1.0 / 298.257223563)
/*--------------------------------------------------------*/
double	a_axis,e_2;
double 	ep2,e_4,e_6,utm_term1,utm_term2,utm_term3,utm_term4;
double 	e1,e1_2,utm8_19,utm_term5,utm_term6,utm_term7;
/*--------------------------------------------------------------*/
void P2G(double lat, double lon, double *north, double *east)
{
	double sinlat,coslat,tanlat,rpv,t,c,m;
	double temp,a_2,a_3;

	if(lat < 0)
	{
		P2G(-lat, lon, north, east);
		*north *= -1;
		return;
	}
	if(lon < 0)
	{
		P2G(lat, -lon, north, east);
		*east *= -1;
		return;
	}

	sinlat = sin(lat);
	coslat = cos(lat);
	tanlat = sinlat/coslat;
	temp = 1.0 - e_2 * sinlat*sinlat;
	rpv = sqrt(temp);
	rpv = a_axis / rpv;
	t = tanlat * tanlat;
	c = ep2 * coslat * coslat;
	m = a_axis * ((lat * utm_term1) -
					(sin(2.0 * lat) * utm_term2) +
					(sin(4.0 * lat) * utm_term3) -
					(sin(6.0 * lat) * utm_term4));
	lon *= coslat;
	a_2 = lon*lon;
	a_3 = a_2*lon;
	*east = k0 * rpv * (lon + (1.0 - t + c) * a_3*(1.0/6.0) +
		(5.0 + t*(t-18.0) + c*72.0 - ep2*58.0) * a_2*a_3*(1.0/120.0));
	*north = k0 * (m + rpv*tanlat * (a_2*(1.0/2.0) + a_2*a_2*(1.0/24.0) *
		(5.0 - t + c*(9.0+c*4.0)) + a_3*a_3*(1.0/720.0) *
	(61.0 + t*(t-58.0) + c*600.0 - ep2*330.0)));
}
/*--------------------------------------------------------------*/
void G2P(double north, double east, double *lat, double *lon)
{
	double	mu,phip,nu,dphi, cphip, sphip, tphip;
	double	c1, c1_2, t1, t1_2, r1, d, d_2, temp, tempsqrt;

	if(north < 0)
	{
		G2P(-north, east, lat, lon);
		*lat *= -1;
		return;
	}
	if(east < 0)
	{
		G2P(north, -east, lat, lon);
		*lon *= -1;
		return;
	}
	
	mu = north / utm8_19;
	phip = mu + (sin(mu+mu) * utm_term5) + (sin(4.0 * mu) * utm_term6) +
			(sin(6.0 * mu) * utm_term7);
	cphip = cos(phip);
	sphip = sin(phip);
	tphip = sphip/cphip;

	c1 = ep2 * cphip * cphip;
	c1_2 = c1*c1;
	t1 = tphip * tphip;
	t1_2 = t1*t1;
	temp = 1.0 - e_2 * sphip * sphip;
	tempsqrt = sqrt(temp);
	nu = a_axis / tempsqrt;
	r1 = nu * (1.0 - e_2) / temp;
	d = east/(nu*k0);
	d_2 = d*d;
	dphi = nu * tphip/r1 * d_2*(1.0/2.0);
	dphi = dphi * (1.0- (5.0+ t1+t1+t1 + c1*10.0- c1_2+c1_2+c1_2+c1_2 -
		ep2*9.0) * d_2*(1.0/12.0) + (61.0 + t1*90.0 + c1*298.0 +
		t1_2*45.0 - ep2*252.0- c1*c1*3.0) * d_2*d_2*(1.0/360.0));
	*lon = d - (1.0+t1+t1+c1) * d_2*d*(1.0/6.0);
	temp= (5.0- c1+c1 + t1*28.0- c1_2+c1_2+c1_2 + ep2*8.0+ t1_2*24.0) *
		d_2*d_2*d*(1.0/120.0);

	*lon = (*lon + temp)/cphip;
	*lat=phip-dphi;
}
/*--------------------------------------------------------------*/
void initPG(double a, double f)
{
	double temp,tempsqrt;

	a_axis = a;
	e_2 = 2 * f - f * f;

	ep2 = e_2 / ( 1.0 - e_2);
	e_4 = e_2 * e_2;
	e_6 = e_4 * e_2;
	utm_term1 = 1.0 - e_2 *(1.0/4.0) - e_4*(3.0/64.0) - e_6*(5.0/256.0);
	utm_term2 = e_2 *(3.0/8.0) + e_4*(3.0/32.0) + e_6*(45.0/1024.0);
	utm_term3 = e_4*(15.0/256.0) + e_6*(45.0/1024.0);
    utm_term4 = e_6*(35.0/3072.0);
    temp = 1.0-e_2; 
	tempsqrt = sqrt(temp);
    e1 = (1.0-tempsqrt)/(1.0+tempsqrt);
    e1_2 = e1 * e1; 
    ep2 = e_2 / ( 1.0 - e_2);
	utm8_19 = k0 * a_axis * (1.0 - e_2*(1.0/4.0) -
				e_2*e_2*(3.0/64.0) -
					e_2*e_2*e_2*(5.0/256.0));
	utm_term5 = 1.5 * e1 - (1.5*9.0/16.0) * e1_2 * e1;
	utm_term6 = e1_2*(21.0/16.0) - (21.0/16.0 * 55.0/42.0) * e1_2 * e1_2;
	utm_term7 = (151.0/96.0) * e1_2 * e1;
}
/*--------------------------------------------------------*/
void lla2xyz(double ax, double e2,double lat, double lon, double alt, double *x, double *y, double *z)
{
	double  d1,d2,Rn;

	d1 = sin(lat);
	d2 = cos(lat);

	Rn = ax / sqrt(1.0 - (e2 * d1 * d1));
	*x = (Rn + alt) * d2 * cos(lon);
	*y = (Rn + alt) * d2 * sin(lon);
	*z = (Rn * (1.0 - e2) + alt) * d1;
}
/*--------------------------------------------------------*/	
void xyz2lla(double ax, double e2,double x, double y, double z, double *lat, double *lon, double *alt)

{
	int     i;
	double  sinlat, sinlat2;
	double  p, q, a, b, d;
	double  r;

	if ((x==0.0)&&(y==0.0)&&(z==0.0))
	{
		*lat=*lon=*alt=0;
		return;
	}
	*lon = atan2(y,x);
	if (*lon > M_PI)
		*lon -= (M_PI+M_PI);
	p = sqrt((x * x) + (y * y));
	if (p < 0.1)
		p = 0.1;
	q = z / p;
	*alt = 0.0;
	*lat = atan2(q * (1.0 / (1.0-e2)),1.0);
	i = 0;
	do
	{   
		sinlat = sin(*lat);
		sinlat2 = sinlat * sinlat;
		r = ax / sqrt(1.0 - (e2 * sinlat2));
		d = *alt;
		*alt = (p / cos(*lat)) - r;
		a = q * (r + *alt);
		b = ((1.0-e2) * r) + *alt;
		*lat = atan2(a,b);
		if (*lat > M_PI)
			*lat -= (M_PI + M_PI);
		a = fabs(*alt - d);
		i++;
	}  while ((a > 0.2) && (i < 20));
}
/*--------------------------------------------------------------*/
/* Align ECEF offset acc. to the current value of the OBP       */
/*																*/
int	AlignEcef(void *vp)
{
	double	dx1,dy1,dz1,dx2,dy2,dz2;
	if(LL)
		if(LL->xc)
		{
			lla2xyz(wgs_ax,wgs_e2,OBP->xc->x,OBP->xc->y,OBP->xc->z,&dx1,&dy1,&dz1);
			lla2xyz(wgs_ax,wgs_e2,LL->xc->x,LL->xc->y,LL->xc->z,&dx2,&dy2,&dz2);

			GPS_dx -= dx1 - dx2 - 0.5;
			GPS_dy -= dy1 - dy2 - 0.5;
			GPS_dz -= dz1 - dz2 - 0.5;
			
			OBP->xc->x = LL->xc->x;
			OBP->xc->y = LL->xc->y;
			OBP->xc->z = LL->xc->z;

			mess("aligned");
		}
	return(null);
}
/*--------------------------------------------------------------*/
void Lla2Utm(lib *l, lib *ll, lib *p)
{
	double	lat,lon,alt;
	double	x,y,z;

/*... najprej pretvorba wgs -> user datum ...					*/

	lla2xyz(wgs_ax,wgs_e2,l->xc->x, l->xc->y, l->xc->z, &x, &y, &z);
	x -= GPS_dx;
	y -= GPS_dy;
	z -= GPS_dz;
	xyz2lla(a_axis,e_2,x, y, z, &lat, &lon, &alt);
/* ... èe ni referenène toèke, iskanje ident. zone	*/
	if(!p)
		ZoneId=FindZone(lat,lon);
/* ... ce je zona definirana, odstej nespremenljivi del ...		*/
	if(ZoneId)
	{
		P2G(lat, lon - D2R*ZoneId->clon, &x, &y);

		if(!p)
			ZoneId->x = floor((x-ZoneId->a)/ZoneId->stepx);
		x = x - ZoneId->a - ZoneId->stepx * ZoneId->x;

		if(!p)
			ZoneId->y = floor((y-ZoneId->b)/ZoneId->stepy);
		y = y - ZoneId->b  - ZoneId->stepy * ZoneId->y;
	}
	else
	{
		if(!p)
			ZoneUndef=floor((lon*R2D+(double)CM_sect/2.0-CM_off) / CM_sect);
		P2G(lat, lon-D2R*(ZoneUndef*CM_sect+CM_off), &x, &y);
		x += FalseNorth;
		y += FalseEast;
		if(!p)
			SouthFlag=x/1000000.0;
		x -= SouthFlag*1000000.0;
	}
/* sicer ???	*/

	ll->xc->x=x;
	ll->xc->y=y;
	ll->xc->z=alt;
}
/*--------------------------------------------------------------*/
void Utm2Lla(lib *l, lib *ll)
{
	double lat, lon, alt;
	double	x,y,z;
		
	x=l->xc->x;
	y=l->xc->y;

	if(ZoneId)
	{
		x += ZoneId->x * ZoneId->stepx + ZoneId->a;
		y += ZoneId->y * ZoneId->stepy + ZoneId->b;
		G2P(x, y, &lat, &lon);
		lon += D2R*ZoneId->clon;
	}
	else
	{
		x += SouthFlag*1000000.0;
		x -= FalseNorth;
		y -= FalseEast;
		G2P(x, y, &lat, &lon);
		lon += (ZoneUndef*CM_sect+CM_off)*D2R;
	}

	alt = l->xc->z;

	lla2xyz(a_axis,e_2,lat, lon, alt, &x, &y, &z);
	x += GPS_dx;
	y += GPS_dy;
	z += GPS_dz;
	xyz2lla(wgs_ax,wgs_e2,x, y, z, &ll->xc->x, &ll->xc->y, &ll->xc->z);
}
/*--------------------------------------------------------*/
int	SelDatum(void *vp)
{
	char	c[16];
	int		i;

	for(i=0; dat[i].ref; ++i)
		if(i==GPS_datum)
			break;

	GPS_datum=i;
	if(Kbhit(CHLCD))
	{
		switch(Getch(CHLCD))
		{
		case LEFT:
			if(GPS_datum)
				--GPS_datum;
			break;
		case RIGHT:
			if(dat[GPS_datum].ref)
				++GPS_datum;
			break;
		case CLEAR:
			if(dat[GPS_datum].ref)
				while(dat[GPS_datum].ref)
					++GPS_datum;
			else
				GPS_datum=0;
			break;
		}
	}
	if(dat[GPS_datum].ref)
	{
		strncpy(c,dat[GPS_datum].ref,12);
		c[12]=0;
		msprint(c);
	}
	else
		msprint("custom");
	return(null);
}
/*--------------------------------------------------------*/
int	SelGeoid(void *vp)
{
	char	c[16];
	int		i;

	for(i=0; dat[i].ref; ++i)
		if(i==GPS_geo)
			break;
	GPS_geo = i;

	if(dat[GPS_datum].ref)
	{
		strncpy(c,dat[GPS_geo].ellipsoid,12);
		c[12]=0;
		msprint(c);
		GPS_geo=GPS_datum;
	}
	else
	{
		if(Kbhit(CHLCD))
		{
			switch(Getch(CHLCD))
			{
			case LEFT:
				while(GPS_geo--)
				{
					for(i=0; i<GPS_geo ;++i)
						if(!strcmp(dat[GPS_geo].ellipsoid,dat[i].ellipsoid))
							break;
					if(i==GPS_geo)
						break;
				}
				if(GPS_geo < 0)
					++GPS_geo;
				break;
			case RIGHT:
				while(dat[++GPS_geo].ref)
				{
					for(i=0; i<GPS_geo ;++i)
						if(!strcmp(dat[GPS_geo].ellipsoid,dat[i].ellipsoid))
							break;
					if(i==GPS_geo)
						break;
				}
				if(!dat[GPS_geo].ref)
					--GPS_geo;
				break;
			}
		}
		strncpy(c,dat[GPS_geo].ellipsoid,12);
		c[12]=0;
		msprint(c);
	}
	return(null);
}
/*--------------------------------------------------------*/
int	GeoOffset(void *vp)
{
	char	c[16];
	int		*i=vp;

	if(!dat[GPS_datum].ref)
		EnterInt(vp);
	else
	{
		GPS_dx=dat[GPS_datum].dx;
		GPS_dy=dat[GPS_datum].dy;
		GPS_dz=dat[GPS_datum].dz;
		sprintf(c,"%d",*i);
		msprint(c);
	}
	return(null);
}
/*--------------------------------------------------------*/
void	SetDatum()
{
	int	i;
	for(i=0; dat[i].ref;++i)
		if(i==GPS_datum)
			break;

	if(dat[i].ref)
	{
		GPS_geo=i;
		initPG(wgs_ax - dat[i].dr, wgsf - dat[i].df / 10000.0);
		GPS_dx=dat[i].dx;
		GPS_dy=dat[i].dy;
		GPS_dz=dat[i].dz;
	}
/* else custom datum */
	else
		for(i=0; dat[i].ref;++i)
			if(i==GPS_geo)
				break;
		initPG(wgs_ax - dat[i].dr, wgsf - dat[i].df / 10000.0);
}
/*--------------------------------------------------------*/
zone *FindZone(double lat, double lon)
{
	zone	*z;

	for(z=Zones; z; z=z->next)
	{
		if(lon < (z->clon + z->wlon/2)*D2R && 
			lon >= (z->clon - z->wlon/2)*D2R && 
				lat < (z->clat + z->wlat/2)*D2R &&
					lat >= (z->clat - z->wlat/2)*D2R)
						return(z);
	}
	return(NULL);
}
/*--------------------------------------------------------*/
void *InitZone(char *s[])
{
	int		i,j;
	char	**q;
	double	ao,a,b,c,d;

	zone *p=memalloc(sizeof(zone));
	p->clat=atof(s[0]);
	p->clon=atof(s[1]);
	p->wlat=atof(s[2]);
	p->wlon=atof(s[3]);
	p->stepx=atof(s[4]);
	p->stepy=atof(s[5]);
/*	Ce je string prazen, je False North Utm koordinata po modulu
	koraka; na ta nacin se lahko definira polarni sektor pri utm. 
	Ne False North ne East ne moreta biti vecja od pripadajocega
	koraka stepx ali stepy !!!
*/
	if(*s[6])
		p->xfalse=atof(s[6]);
	else
	{
		P2G(D2R*p->clat,0,&a,&b);
		p->xfalse=a;
	}
	p->xfalse=fmod(p->xfalse,p->stepx);
/*	analogno se v primeru geog. dolzine pri praznem stringu nastavi
	False East na 0
*/
	if(*s[7])
		p->yfalse=atof(s[7]);
	else
		p->yfalse=0;
	p->yfalse=fmod(p->yfalse,p->stepy);
/**/
	p->name=memalloc(sizeof(char)*strlen(s[8])+1);
	strcpy(p->name,s[8]);
	p->next=NULL;
/*	nx steje od start latitude, dokler ne preseze zgornjega desnega
	kota cone
*/
	P2G(p->clat*D2R, 0, &ao, &b);
	if(p->clat > 0)
	{
		P2G((p->clat-p->wlat/2)*D2R, 0, &a, &b);
		P2G((p->clat+p->wlat/2)*D2R, p->wlon/2*D2R, &c, &d);
	}
	else
	{
		P2G((p->clat-p->wlat/2)*D2R, p->wlon/2*D2R, &a, &b);
		P2G((p->clat+p->wlat/2)*D2R, p->wlon/2*D2R, &c, &d);
	}
	a = p->stepx * floor(((a-ao) + p->xfalse)/p->stepx) - p->xfalse + ao;
	p->a=a;
	p->nx=0;
	while(a < c)
	{
		p->nx++;
		a += p->stepx;
	}
/*	Spodnji levi kot zmanjsa za False East (d je negativen !) in 
	steje po stepy, dokler ne preseze spodnjega desnega kota !
*/
	if(p->clat > 0)
		P2G((p->clat-p->wlat/2)*D2R, -p->wlon/2*D2R, &c, &d);
	else
		P2G((p->clat+p->wlat/2)*D2R, -p->wlon/2*D2R, &c, &d);
	d = p->stepy * floor((d + p->yfalse)/p->stepy) - p->yfalse;
	p->b=d;
	p->ny=0;
	while(d < -p->b)
	{
		p->ny++;
		d += p->stepy;
	}
	q=p->id = memalloc(p->nx * p->ny  * sizeof(char *));
	for(i=0; i<p->nx; ++i)
		for(j=0; j<p->ny; ++j)
			*q++ = NULL;
	return(p);
}
/*--------------------------------------------------------*/
void	ListDatum(void)
{
	int		i;
	char	c[1024];
	for(i=0; dat[i].ref;++i)
	{
		sprintf(c,"%2d) %s,%s,%.0f,%e,%d,%d,%d\r\n",i,
			dat[i].ref,dat[i].ellipsoid,
				wgs_ax - dat[i].dr,
					wgsf - dat[i].df / 10000.0,
						dat[i].dx,dat[i].dy,dat[i].dz);
		puts_pc(c);
	}
}
/*--------------------------------------------------------*/
uchar	*ListZone(uchar *f)
{
	int		i,j,k;
	zone	*z;
	char	p[1024];

	
	for(z=Zones; z; z=z->next)
	{
		sprintf(p,"=z %lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%s\r\n",
													z->clat,
													z->clon,
													z->wlat,
													z->wlon,
													z->stepx,
													z->stepy,
													z->xfalse,
													z->yfalse,
													z->name
													);
		if(f)
		{
			*strchr(p,'\r')=0;
			k=strlen(p);
			f=prog_byte(&k,f,sizeof(int));
			f=prog_byte(p,f,k);
		}
		else
			puts_pc(p);
		for(i=0; i<z->nx; ++i)
		{
			for(j=0; j<z->ny; ++j)
			{
				if(z->id)
				{
					if(z->id[i*z->ny +j])     
					{
						sprintf(p,"=z %s,%d,%d\r\n",z->id[i*z->ny +j],i,j);
						if(f)
						{
							*strchr(p,'\r')=0;
							k=strlen(p);
							f=prog_byte(&k,f,sizeof(int));
							f=prog_byte(p,f,k);
						}
					else
						puts_pc(p);
					}
				}
			}
		}
	}
	if(f)
	{
		k=null;
		f=prog_byte(&k,f,sizeof(int));
	}
	return(f);
}
/*----- Scanning zone parameters from PC port or flash ---*/
void	EnterZone(char *s)
{
	int		i,j;
	char	*c[16],*p,*q;
	zone	*z;
/*----- Zone dimensions ----------------------------------*/
	i=0;
	p=q=s;

	i=strscan(s,c,',',16);
	switch(i)
	{
	case 6:
		if(Zones)
		{
			for(z=Zones; z->next; z=z->next);
			i=atoi(c[4]);
			j=atoi(c[5]);
			if(i < z->nx && j < z->ny)
			{
				z->id[i*z->ny+j] = memalloc((4+strlen(c[0])+strlen(c[1])+strlen(c[2])+strlen(c[3]))*sizeof(char));
				sprintf(z->id[i*z->ny+j],"%s,%s,%s,%s",c[0],c[1],c[2],c[3]);
				return;
			}
		}
		break;
	case 9:
		if(Zones)
		{
			for(z=Zones; z->next; z=z->next);
			z->next=InitZone(c);
		}
		else
			Zones=InitZone(c);
		return;
	}
puts_pc("!SYNTAX\r\n");
}
/*-----------------------------------------------------------------*/
void	ShowZone(lib *ll)
{
	char	s[32],*ss[4],p[32];
	int		i;
	for(i=0; i<32; ++i)
		LcdExt[i]=' ';
	if(ZoneId)
	{
		if(ZoneId->id[ZoneId->x * ZoneId->ny + ZoneId->y])
			strcpy(s,ZoneId->id[ZoneId->x * ZoneId->ny + ZoneId->y]);
		else
			*s=0;
		i=strscan(s,ss,',',4);
		while(i--)
		{
			translate(ss[i],'_',' ');
			sprintf(p,"%8s",ss[i]);
			strncpy(&LcdExt[8*i],p,8);
		}
	}
	else
		ShowUtm(ll);
}
/*-----------------------------------------------------------------*/
void	ShowUtm(lib *l)
{
	char	s[32];
	int		i,j;
	lib		*ll;

	copylib(l,ll=makelib());
	for(i=0; i<32; ++i)
		LcdExt[i]=' ';
	Utm2Lla(ll,ll);
	ll->xc->x*=R2D;
	ll->xc->y*=R2D;
	i=floor(ll->xc->x/8)+'N';
	j=floor(ll->xc->y/CM_sect)-(double)CM_off/(double)CM_sect+0.5;
	if(i>'N')
		++i;
	if(i<'J')
		--i;
	sprintf(s,"%6d%c ",j,i);
	strncpy(LcdExt,s,8);
	freelib(ll);
}
/*-----------------------------------------------------------------*/
uchar	*SaveZone(uchar *f)
{
	static	int	i,j,k;
	zone	*z=Zones;

	if(Zones)
	{
		++i;
		Zones=Zones->next;
		f=SaveZone(f);
		Zones=z;
		f=prog_byte(z,f,sizeof(zone));
		j=strlen(z->name);
		f=prog_byte(&j,f,sizeof(int));
		f=prog_byte(z->name,f,j);
		for(k=0; k<z->nx * z->ny;++k)
			if(z->id[k])
			{
				f=prog_byte(&k,f,sizeof(int));
				j=strlen(z->id[k]);
				f=prog_byte(&j,f,sizeof(int));
				f=prog_byte(z->id[k],f,j);
			}
		k=eof;
		f=prog_byte(&k,f,sizeof(int));
	}
	else
	{
		f=prog_byte(&i,f,sizeof(int));
		i=0;
	}
	return(f);
}
/*-----------------------------------------------------------------*/
uchar	*LoadZone(uchar *f)
{
	int		i,j,k;
	zone	*z;
	f=load_byte(&i,f,sizeof(int));
	while(i-- > 0)
	{
		z=memalloc(sizeof(zone));
		f=load_byte(z,f,sizeof(zone));
		f=load_byte(&j,f,sizeof(int));
		z->name=memalloc(j+1);
		f=load_byte(z->name,f,j);
		z->name[j]=0;
		z->id=memalloc(z->nx * z->ny * sizeof(char *));
		for(k=0; k<z->nx * z->ny; ++k)
			z->id[k]=NULL;
		do
		{
			f=load_byte(&k,f,sizeof(int));
			if(k != eof)
			{
				f=load_byte(&j,f,sizeof(int));
				z->id[k]=memalloc(j+1);
				f=load_byte(z->id[k],f,j);
				z->id[k][j]=0;
			}
		} while(k!=eof);
		z->next = Zones;
		Zones=z;
	}
	return(f);
}
/*-----------------------------------------------------------------*/
int	EditUtm(void *vp)
{
	if(!LL)
		return(1);
	if(!LL->xc)
		return(1);
	xyLCD(-8,0);
	Cursor(CURS_FULL);
	return(null);
}
/*-----------------------------------------------------------------*/
int	UtmSouth(void *vp)
{
	lib *l;
	zone *z=ZoneId;
	copylib(LL,l=makelib());
	if(z)
	{
		--z->x;
		Utm2Lla(l,l);
		if(z->x < 0)
			l->xc->x=D2R*(z->clat-z->wlat/2)-arcmile;
		Lla2Utm(l,l,NULL);
		if(!ZoneId)
		{
			ZoneId=z;
			++z->x;
		}
	}
	else
	{
		--SouthFlag;
		if(SouthFlag < 0)
			SouthFlag=0;
	}
	ShowZone(l);
	return(null);
}
/*-----------------------------------------------------------------*/
int	UtmNorth(void *vp)
{
	lib *l;
	zone *z=ZoneId;
	copylib(LL,l=makelib());
	if(z)
	{
		++z->x;
		Utm2Lla(l,l);
		if(z->x >= z->nx)
			l->xc->x=D2R*(z->clat+z->wlat/2)+arcmile;
		Lla2Utm(l,l,NULL);
		if(!ZoneId)
		{
			ZoneId=z;
			--z->x;
		}
	}
	else
	{
		++SouthFlag;
		if(SouthFlag >= 18)
			SouthFlag=18;
	}
	ShowZone(l);
	return(null);
}
/*-----------------------------------------------------------------*/
int	UtmWest(void *vp)
{
	lib *l;
	zone	*z=ZoneId;
	copylib(LL,l=makelib());
	if(z)
	{
		--z->y;
		Utm2Lla(l,l);
		if(z->y < 0)
			l->xc->y=D2R*(z->clon-z->wlon/2)-arcmile;
		Lla2Utm(l,l,NULL);
		if(!ZoneId)
		{
			ZoneId=z;
			++z->y;
		}
	}
	else
	{
		--ZoneUndef;
		if(ZoneUndef < 0)
			ZoneUndef=59;
	}
	ShowZone(l);
	return(null);
}
/*-----------------------------------------------------------------*/
int	UtmEast(void *vp)
{
	lib *l;
	zone *z=ZoneId;
	copylib(LL,l=makelib());
	if(z)
	{
		++z->y;
		Utm2Lla(l,l);
		if(z->y >=z->ny)
			l->xc->y=D2R*(z->clon+z->wlon/2)+arcmile;
		Lla2Utm(l,l,NULL);
		if(!ZoneId)
		{
			ZoneId=z;
			--z->y;
		}
	}
	else
	{
		++ZoneUndef;
		if(ZoneUndef >= 60)
			ZoneUndef=0;
	}
	ShowZone(l);
	return(null);
}
/*
void GetMgrs(int UTM_gx,int Northing,int Easting,char *p)
{
	char mgrs[]="ABCDEFGHJKLMNPQRSTUVWXYZ";
	int	MGRS_gx, MGRS_gy;

	MGRS_gx = (UTM_gx % 3)*8 + (uint)(Easting/100000.0);
	if ( UTM_gx % 2 )
		MGRS_gy = (uint)((Northing - (uint)(Northing/2000000.0)*
		2000000.0) / 100000.0);
		else MGRS_gy = (uint)((Northing+500000.0 -
			(uint)((Northing+500000.0)/2000000.0)*2000000.0) / 100000.0);
		p[0]=mgrs[MGRS_gx-1];
		p[1]=mgrs[MGRS_gy];
		p[2]=0;
}


void	Mgrs(void)
{
	FILE	*f=fopen("ttt.ttt","w");
	char	p[16];
	int		i,j,k;
	double	lt,ln;
	for(k=0; k<6;++k)
	{
		fprintf(f,"=z %d,%d,80,3,100000,100000,g,zp%c%d\n",0,3+k*6,'-',2*k);

		i=0;
		do
		{
			j=0;
			do
			{
				GetMgrs(k,i*100000,500000+j*100000+10000,p);
				fprintf(f,"=z %s,,,,%d,%d\n",p,i,j);
				j+=1;
				G2P(100000*i,100000*j,&lt,&ln);
			}	while (R2D*ln < 3);
		i+=1;
		G2P(100000*i,100000*j,&lt,&ln);
		}	while (R2D*lt < 80);

		fprintf(f,"=z %d,%d,80,3,100000,-100000,g,zn%c%d\n",0,3+k*6,'-',2*k+1);
		i=0;
		do
		{
			j=0;
			do
			{
				GetMgrs(k,i*100000,500000-j*100000-10000,p);
				fprintf(f,"=z %s,,,,%d,%d\n",p,i,j);
				j+=1;
				G2P(100000*i,100000*j,&lt,&ln);
			}	while (R2D*ln < 3);
		i+=1;
		G2P(100000*i,100000*j,&lt,&ln);
		}	while (R2D*lt < 80);
	}
	fclose(f);
}
*/