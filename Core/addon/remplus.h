/*----------------------------------------------------------------------*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include	<ctype.h>
/*----------------------------------------------------------------------*/
#define		FLASHTOP	(char *)0x00000L
#define		LIBRARY		(char *)0x20000L
#define		SYSPAR		(char *)0x40000L
#define		FONTS		(char *)0x60000L
/*----------------------------------------------------------------------*/
#define		M_E         2.71828182845904523536
#define		M_LOG2E     1.44269504088896340736
#define		M_LOG10E    0.434294481903251827651
#define		M_LN2       0.693147180559945309417
#define		M_LN10      2.30258509299404568402
#define		M_PI        3.14159265358979323846
#define		M_PI_2      1.57079632679489661923
#define		M_PI_4      0.785398163397448309616
#define		M_1_PI      0.318309886183790671538
#define		M_2_PI      0.636619772367581343076
#define		M_1_SQRTPI  0.564189583547756286948
#define		M_2_SQRTPI  1.12837916709551257390
#define		M_SQRT2     1.41421356237309504880
#define		M_SQRT_2    0.707106781186547524401
/*----------------------------------------------------------------------*/
/* graficni simboli														*/
/*																		*/
#define		GRAPHIC_LCD
#define		LCD_WRITE(a,b)	LCD[a]=b

#define		symDELTA		'\x86'	
#define		sym_DELTA		"\x0\x0\x0\x0\x04\x0a\x1f\x0"
#define		symFOTONA		'\xA4'
#define		sym_FOTONA		"\x0e\x1f\x1f\x0e\x0\x0\x0\x0"
#define		symDEGREE		'\xB0'
#define		sym_DEGREE		"\x06\x09\x09\x06\x0\x0\x0\x0"
#define		symARROWd		'\x87'
#define		sym_ARROWd		"\x0\x0\x0\x0\x11\x0a\x04\x0"
#define		symGPSf			'\x99'
#define		sym_GPSf		"\x0\x0\x04\x0a\x1f\x0a\x04\x0"
#define		symdGPS			'\xA7'
#define		sym_dGPS		"\x0\x0\x0\x0e\x0a\x0e\x0\x0"
#define		symdGPSf		'\xAE'
#define		sym_dGPSf		"\x0\x0\x1f\x11\x15\x11\x1f\x0"
#define		symARROWu		'^'
#define		symARROWl		'<'
#define		symARROWr		'>'
/*------------------------------------------------------*/
#define		GPS_ON			(GPS_pdop & 0x0f00)
#define		GPS_CONTINOUS	((GPS_pdop & 0x0f00) == 0x0200)
#define		GPS_SWITCHED	((GPS_pdop & 0x0f00) == 0x0100)
#define		GPS_DIFF		(GPS_pdop & 0x1000)
#define		GPS_ECHO		(GPS_pdop & 0x8000)
#define		C100_ECHO		(GPS_pdop & 0x4000)
/*------------------------------------------------------*/
#define		HW_GPS			128
#define		HW_DGPS			129
#define		SW_V1			1	/* MESSAGE				*/
/*------------------------------------------------------*/
#define		FIRSTLAST_R	0x2C	/* RLD button defs		*/
#define		FIRSTLAST_P	0x24	/*                      */
#define		TRIGGER_R	0x23	/*                      */
#define		TRIGGER_P	0x21	/*                      */
								/* status reg. defs		*/
#define		RLD_TREQ    0x01	/* target request		*/
#define		RLD_MULTG	0x02	/* multiple targets     */
#define		RLD_BLOCTG	0x04	/* block range target	*/
#define		RLD_CHECK	0x20
#define		RLD_OFF		0x40
#define		HV_READY	0x80
/*----------------------------------------------------------------------*/
#define		MAXTASK		16		/* max. no. of periodic	tasks	*/
#define		BACKSP 		0x08	/*								*/
#define		ESC			0x1B	/*								*/
#define		SWAP_T		30		/* error screen swap time n*PIT	*/
#define		eof			(int)-1	/*								*/
#define		null		(int)0	/*								*/
								/* prepisano iz C++ <dos.h>  !!!*/
/*----------------------------------------------------------------------*/
typedef		unsigned char		byte;
typedef		unsigned char		uchar;
typedef		unsigned int		uint;
typedef		unsigned long		ulong;
typedef		struct lib			lib;
typedef		struct gun			gun;
typedef		struct libtext		libtext;
typedef		struct coord		coord;
typedef		struct ltype		ltype;
typedef		struct queue		queue;
typedef		struct rtime		rtime;
typedef		struct samples		samples;
typedef		struct mscan		mscan;
typedef 	struct command		command;
typedef		struct GPS			GPS;
typedef		struct COMPASS		COMPASS;
typedef		struct TCM2			TCM2;
typedef		struct RLD			RLD;
typedef		union  uval			uval;
/*----------------------------------------------------------------------*/
#define		MSCANX		20
#define		MSCANY		20
struct 	mscan	{
				int		level,active,eol[MSCANY];
				char	text[MSCANY][MSCANX+1];
				command	*c;
				};
/*----------------------------------------------------------------------*/
struct 	samples	{
				double	*tp,*fp,*rp;
				int		n;
				};
/*----------------------------------------------------------------------*/
struct	rtime	{
				uchar	hour,min,sec;
				uchar	day,date,month,year;
				};
/*----------------------------------------------------------------------*/
struct	coord	{                       /*								*/
				double		x,y,z;		/* x,y,z coordinates			*/
				rtime		t;			/* entry time					*/
										/* -- GUN RELATED DATA --		*/
				int			muzzle; 	/* sprememba v promilih			*/
				int			refp;		/* aim ref. value				*/
				coord		*next;      /*                              */
				};                      /*                              */
/*----------------------------------------------------------------------*/
#define	MAXAMM	8
#define MAXCHRG 8
#define	AMMTYPE 8
#define	TRAJ_LOWER	1					/* zgornja trajektorija	*/
#define	TRAJ_UPPER	2					/* spodnja trajektorija	*/
#define	ELEV_DBL	4					/* dvojni daljinar		*/
#define	BEAR_DBL	8					/* dvojna panorama		*/
/*----------------------------------------------------------------------*/
struct	gun		{
				char	*name;			/* gun designation				*/
				int		left;			/* max. left boundary			*/
				int		right;			/*		right					*/
				int		hi;				/* max. elevation				*/
				int		lo; 			/* min.                 	    */
				int		mils;		    /* mils to full circle			*/
                int		zeroA;
                int		scaleA;
                int		zeroE;
                int     scaleE;
				int		gmode;			/*								*/
				float	**mv;			/* muzzle vel. per amm/charge	*/
				char	**ammo;			/* ammunition designation   	*/
				float	*mass;			/* ammunition weight	  		*/
				void	*B;				/* ball. coeff. table			*/
                char	***fuze;		/* fuze select					*/
				int		**alt;			/* fuze active altitude			*/
                char	***charge;		/* charge name string			*/
				float	***drift;		/* drift coeff.					*/
				float	***ABC;    		/* initial range coeff.			*/
				float	*dmass;			/* mass change per mark			*/
				float	**dvmass;		/* muzzle vel. vs mass  change	*/
				float	***dvtemp;		/* muzzle vel. vs powder temp	*/
				};
extern			gun		*Guns;
/*----------------------------------------------------------------------*/
struct lib		{
				ltype	*ltype;			/* item type					*/
				libtext	*txt;       	/* item description	text		*/
				int	n;					/* item designation				*/
										/* -- GUN RELATED DATA --		*/
				int		gun;			/* gun type						*/
				int		refd;			/* ref. direction				*/
				int		powder;			/* powder temp.					*/
				int		ammpcs[MAXAMM];	/* no. of proj.					*/
				coord	*x;				/* item coordinates	pointer		*/
				coord	*xc;			/* currently active coordinate	*/
				lib		*next;			/* pointer to next				*/
				int		check;          /* checksum 					*/
				};
/*----------------------------------------------------------------------*/
#define LINK_LEN		600
#define	LIBTEXT_LEN		12
#define	LIBTYPE_LEN		2

struct	libtext	{
				char	t[ LIBTEXT_LEN +2 ];
				libtext	*next;
				};
/*----------------------------------------------------------------------*/
struct	ltype	{
				char	t[ LIBTYPE_LEN +2 ];
				libtext	*typtxt;
				ltype	*next;
				};
/*----------------------------------------------------------------------*/
struct			queue
				{
				void	(*task)(void);
				int		rate;
				int		count;
				queue	*next;
				};
/*----------------------------------------------------------------------*/
union uval		{
				char    bd[8];
				short   sv;
				int     iv;
				long	lv;
				float   fv;
				double  dv;
				};
/*----------------------------------------------------------*/
#define	MAXOPT	16
#define	MAXPAR	16
#define MAXGUN	8
struct	command {
				int		n;
				uint	from,to,tg;
				double	corrx,corry,corrz;
				uchar	txmode[MAXGUN],rxmode,ntx;
				uchar	opt[MAXOPT],maxopt;
                uchar	effect,tmode,fmode,rounds,interval,section;
				uchar	ammo,traj,charge,fuze;
                rtime	ftime;
				uint	tof;
				command	*next;
				int		chk;
				};
/*------------------------------------------------------------*/
struct GPS 		{
				uchar	status,error;
				double	x,y,z,vx,vy,vz;
				double	lat,lon,alt;
				uint	gmode,hour,minute,seconds;
				uint	week,weekday;
				uint	day,month,year;
				float	UTCoffset;
				uint	sat[6];
				float	pdop,hdop,vdop,tdop;
				};
/*------------------------------------------------------------*/
struct COMPASS	{
				uchar	status;
				double	az;
				};
/*------------------------------------------------------------*/
struct RLD		{
				uchar status, buttons;
				uint R[4],N;
				};
/*------------------------------------------------------------*/
struct TCM2		{
				uchar	status;
				double	c,p,r,t;
				};
/*----------------------------------------------------------------------*/
enum fkeys		{ENTER=128,F1,F2,F3,F4,LEFT,RIGHT,UP,DOWN,CLEAR,BACKLIT,ALPHA,
				SIGN,ONOFF,REFRESH,LCD_U,LCD_D,SHIFTR,MAXFKEY};
enum t_mode		{HOUR_MIN,MIN_SEC,DAY_DATE,MONTH_YEAR,MAX_TMODE};
enum dismode	{EL_AZ_R,AZ_R_dZ,N_E_Z,EL_AZ,MAX_DMODE};
enum link_comm	{COMM_REQ,ABORT_REQ,COMM_UPDT,ACK_REQ,ACK_UPDT,
				UNIT_REQ,UNIT_UPDT,TARGT_REQ,TARGT_UPDT,
				METEO_REQ,METEO_UPDT,RTCM_REQ,RTCM_UPDT,
				ACK_MESS,NACK_MESS};
enum comm_stat	{M_UNDEF,M_PEND,M_ACK,M_NACK,M_ABORT};
enum shafttype	{ROC,ROC_G,NOSHAFT};
enum meteo_par	{HEIGHT,WIND_S,WIND_D,AIR_T,AIR_P,AIR_H};
enum auxmode	{AUX_OFF,AUX_JW,AUX_DGPS};
enum ioch		{CHLCD,CHPC,CHAUX,CHRLD,CHC100,CHGPS,CHIR,CHRF};
/*----------------------------------------------------------------------*/
/* file=remmisc.c														*/
/*																		*/
#define		CURS_FULL	'\xDC'
#define		CURS_UND	'_'
#define		CURS_OFF	' '

int			sign(int);
long		lsign(long);
double		fsign(double);
int			puts_lcd(char *), put_lcd(int);
void		getLCD(char *);
int			mousehit(void), getfx(void);
void		xyLCD(int,int), RemMain(void);
void		symLCD(int, char *),watchdog(void);
void		DeleteTask(void(* )(void),queue *);
queue		*CheckTask(void(* )(void),queue *),*SetTask(void(* )(void),int,queue *);
void		*SetBar(void),ClrBar(void);
int			getasc(char *);
lib			*Displc(lib	*),*Display(lib *,int,int);
void		ScanRLD(void),CheckRLD_on(void),CheckRLD_off(void);
void		rGPSoff(void),DisplGPS(void),DisplCompass(void),DisplLevel(void);
void		Collect(double *, double *);
void		DisplTrack(void),DisplTilt(void),DisplTiltX(void),DisplTiltXc(void),DisplPoint(void);
void		GpsTime(void),GpsTimeTrigg(void);
int			Gps2E(void),Gps2C(void),Gps71(void);
int			SetGpsLon(void *vp),SetGpsLat(void *vp);
void		GpsFix(void);
void		*Displnc(lib	*,int);
coord		*togrid(coord *,coord *,int,double);
coord		*gridto(coord *,coord *,int,double);
int			LlaToRel(lib *,lib *,lib *,lib *,int);
int			RelToLla(lib *,lib *,lib *,lib *,int);
int			GridToRel(lib *,lib *,lib *,lib *,int);
int			RelToGrid(lib *,lib *,lib *,lib *,int);
int			LlaToCorr(lib *,lib *,lib *,lib *,int);
int			CorrToLla(lib *,lib *,lib *,lib *,int);
void		Utm2Lla(lib *,lib *),Lla2Utm(lib *,lib *,lib *);
void 		SetDatum(void);
void		wgs_lla2xyz(double, double, double, double *, double *, double *);
void		xyz2wgs_lla(double, double, double, double *, double *, double *);
int			counttype(lib *,ltype *),counttext(lib *,libtext *);
lib			*checklib(lib *,ltype *, int),*findtype(lib *,char *);
lib			*make_survy_lib(void *);
void		*FirstMenu(void);
int			InsertLib(void *);
void		*freelib(lib *),*freelibrary(lib *),*freecoord(coord *);
void		*freetype(ltype *),*freetext(libtext *);
lib			*copylib(lib *,lib *);
int			edit_xyz(double *,int),edit_pol(double *,int,int,int);
int			EditStr(int,char *,int);
int			BatteryOK(void);
coord		*add_coord(lib *,double,double,double),*AddCoord(lib *,double,double,double);
coord		*LastCoord(lib *),*FirstCoord(lib *),*erase_coord(lib *,coord *),*copyc(coord *);
void		*copycoord	(lib *,lib *);
lib			*makelib(void);
lib			*make_set(double,double,double,char *,char *);
void		ReadTime(rtime *),WriteTime(rtime *);
char		*GetsDate(char *), *GetsTime(char *);
void		AlarmEnable(rtime *),AlarmDisable(void);
void		CountTime(void);
int			Kbhit(int), GetKey(void);
int			Ungetch(int, int), Getch(int), Putch(int, int);
void		link_scan(void),key_scan(void);
void		AuxDef(void),AuxRTCM(void),ReadPc(void),ReadRf(void),SortLib(void);
void		CheckGun(void),CheckScreen(void);
lib			*CheckItem(lib *ll);
void		CheckMenu(void),CheckLib(void);
int			Crc(int, int);
void		Submit(void);
int			targets_n(coord *),targets_xn(lib *);
int			translate(char *,char,char), chrtx(char *);
char		*LinkAddrText(int, char *);
ltype		*LinkAddrType(int);
int			LinkAddrN(int),LinkAddr(lib *);
lib			*FindLib(int);
void		*help(char *);
ltype		*addtype(char *),*checktype(lib *,char *);
libtext		*addtxt(lib *,char *);
void		sort_x(coord *,rtime *);
double		dtime(rtime *, rtime *),calc_week(rtime *), calc_secs(rtime *);
void		addtime(rtime *,rtime *),calc_rtime (double,double,rtime *),addsecs(rtime *,double);
void		calc_date (uint);
double		linterp(double *,double *,int);
lib			*interpolate(lib *),*Distribute(lib *,lib *,int);
void		*memalloc(size_t),memfree(void *),SetVect(void (*f)(void),int n);
void		wait(int);
void		backlit_on(void),backlit_off(void),backlit_toggle(void);
void		GpsInit(void),SendDGPS(int),AlignLLA(void);
int			GPS_on(void),GPS_off(void);
void		RTCMFixPos(void);
char		*RTCMDec(char *);
double		C100comp(void),DeltaGrid(void);
void		C100cal(void),C100_on(void),C100_off(void);
void		SysMessageLCD(void), SysMessageEnd(void), SysMessage(int);
int			SetRXmode(command *,int),SetTXmode(command *,int),GetCmode(command *);
int			CallTo(command **,char *,int,int),SetFirstGun(command *);
int			RFCall(int,char *,int);
uchar		*SaveFonts(uchar *),*LoadFonts(uchar *);
gun			*ReadGunData(int, int, int, int, uchar **);
void		WriteGunData(gun *, uchar **f);
int			strscan(char *,char *[],int,int);
void		strswap(char *,char *);
/*--------------------------------------------------------------*/
int			msprint(char *),GetMpos();
int			SetMpos(int);
/*--------------------------------------------------------------*/
/* file = jw.c													*/
/*																*/
void		AdsData(void),IbitRes(void),JwRec(void);
void		JwSend(void),JwDrop(void);
int			JwExe(void);
/*--------------------------------------------------------------*/
/* file = det.c													*/
/*																*/
int			n_samples(samples *);
double		det(double *p, int, int, int);
double		*solve(samples *);
samples		*init_samples(samples *,int), *freesamples(samples *);
void		add_sample(samples *,double,double);
void		add_sample_t(samples *,double,double,double);
double		polyp(double, double *, int n);
double		polyt(double, double *, double, int n);
/*--------------------------------------------------------------*/
double		get_pitch(void),get_roll(void);
void		RTCservice(void);
int			DisplAmm(void *vp);
void		RefreshMenu(void),RefreshScreen(void),Cursor(char);
int			Refresh(void);
void		HeatOn(void),HeatOff(void),BacklitOn(void),BacklitOff(void);
/*---------------------------------------------------------------------*/
void		ResetInit(void),__SystemInit(void);
void		gotoxy(int,int);
int			GetLCDaddr(void);
int			LCD_busy(void);
void		SetLCDaddr(int);
void		initLCD(void),initSPI(void),PIT_enable(void),PIT_disable(void);
void		Lobatt(void),power_off(void);
void		writeADC(void),ADC_scan(void);
int			readRTC(int),writeRTC(int,int),readTEMP(int, int);
int			toBCD(int),fromBCD(int);
void		clrlcd(void),screen_swap(int),screen_unswap(void);
int			LcdLevel(int);
void		sector_erase(uchar *,int);
void		*prog_byte(void *,uchar *,size_t);
void		*prog_vect(void *,uchar *,size_t);
void		*load_byte(void *, uchar *, size_t);
char		*SaveLib(lib *p,uchar *f);
uchar		*GetBlock(uchar *),*PutBlock(uchar *);
lib			*LoadLibItems(uchar *),*LoadLib(uchar *);
uchar		*SaveLibItem(lib *, uchar *);
uchar		*SavePar(uchar *, int),*LoadPar(uchar *, int);
uchar		*SaveComm(uchar *),*LoadComm(uchar *);
int			SaveLibrary(void *vp),CheckLibrary(void *vp);
void		ListPar(void);
void		error(int),err(char *),warn(char *),mess(char *),FlipMessage(void);
double	nround(double, int);
lib			*rx_lib(lib *),*scan_lib(char **),*scan_coord(lib *);
int			tx_coord(char **s,lib *ll),tx_lib(char **,lib *),tx_amm(char **,lib *);
void		G2P(double, double, double *, double *);
void		P2G(double, double, double *, double *);
/*----------------------------------------------------------*/
#define		DeltaT		(((temp >> 5) - t_ref)/t_ref)
void		collect(double *, double *);
/*----------------------------------------------------------*/
int			ReadInt(int),ReadHM(void *);
uint	   	ReadUint(uint);
int			ReadString(char *),ReadHMS(rtime *),ReadDMY(void *);
long		IncLong(long *,int);

void		UpdateAmm(command *, int);
int			Update(int( *)(void *));
int			TargetCall(command *);
int			FireMission(command *);
command		*rx_command(char *, int),*SearchTo(void),*SearchFrom(void);
command		*make_command(void);
int			CommandCopy(void *),CommEnum(command *),new_command(void *);
void		list_update(uint, uint);
int			list_display(void *);
double		getT(void);
void		ListLib(lib *),ListMenu(command *, menu *, int);
void		ReadLib(char *),LoadFromPc(char *),ListToPc(char *),DeleteFromPc(char *);
int			SetAlarm(void),PcLinkDecode(char *);
int			TiltComm(char *),GpsComm(char *);
void		LoadSrec(void),Shutdown(void);
/*------------------------------------------------------------------*/
#define		M_AREA_ACTIVE	((mlevel-dlevel < 5) && (mlevel-dlevel > 0))
#define		M_LEVEL_ACTIVE	(mlevel == alevel+dlevel+1)
int			menu_test(int, char *, unsigned char, menu *);
int			menu_exe(struct fmenu *, int, void *);
int			ScanMenu(char *, char *, char *);
int			s_menu(void *),s_menu_hold(void *);
int			l_menu(void *),l_menu_hold(void *),l_menu_print(void *);
/*------------------------------------------------------------------*/
#define		SHAFT_EL	0
#define		SHAFT_AZ	1
void		shaft_enable(void);
void		shaft_ITEK16uS(void),shaft_ROC415(void);
void		shaft_ROC415G(void);
int			shaft_ready(void);
int			Baud(uint,uint),Xonoff(uint);
/*------------------------------------------------------------------*/
#define		SPI_busy()			(!(SPSR & 0x80))
/*------------------------------------------------------------------*/
void 		Initialize (void);
void 		InitTPUchannels (void);

int 		kbhitd(int);
int 		getd(uint);
int 		putd(int,int);

void 		puts_pc(char *);		/* put string */
void 		puts_rld(char *);
void 		puts_aux(char *);
void 		puts_C100(char *);
void 		puts_gps(char *);
uint 		ParityOdd(uint);

void		put_pc (int);			/* put char in transmit buffer */
void		put_ir (int);			/* put char in transmit buffer */
void		put_rld (int);
void		put_aux (int);
void		put_C100 (int);
void		put_gps (int);

void		DecodeBlock_pc (char *);		/* decode received message */
void		DecodeBlock_tcm2 (char *);
void		DecodeBlock_rld (char *);
void		DecodeBlock_C100 (char *);
void		DecodeBlock_gps (int);

uchar		ReadBuffer_pc (void);			/* pick char from receive-buffer */
uchar		ReadBuffer_rld (void);
uchar		ReadBuffer_aux (void);
uchar		ReadBuffer_C100 (void);
int			ReadBuffer_gps (void);
/*------------------------------------------------------------*/
extern		GPS		gps;
extern		RLD		rld;
extern		COMPASS c100;
/*------------------------------------------------------------*/
void		getGPS(void);
void		getC100(void);
void		getRLD(void);
int			GpsComm(char *);

uchar		*get_DSP(void);
uchar		*put_DSP(void);
int			DSP_status(void);
void    	DspAddress(int);

double		read_AZ(void);
double		read_EL(void);
/*------------------------------------------------------------*/
void		RLD_Local(void),RLD_Remote(void),RLD_Trigger(void),RLD_GetTarget(uint);
void		RLD_ShowTarget(uint),RLD_GetStatus(void),RLD_StartHV(void);
void		RLD_Echo(void),RLD_Noecho(void),getRLD(void),RLD_PowerOff(void);
uint		GetPeriod(void);
/*------------------------------------------------------------*/
#define	ELAZ_defr	1000.0

int			SendBlk(int,char *,int, int);
void		SendAck(void);
int			RFAck(int,char *,int);
int			SingleAck(int,char *,int);
int			UnitFromLink(void *),TargetFromLink(void *);


#define	MR10	(M_PI * 20000.0)
double	ScaleAngle(double, double, double, int);
int		Isect(coord *,coord *,coord *,coord *, double *, double *);
int		Ballistic(lib *,lib *,lib *),CalcBallist(lib *);
void	CalcCrest(void *),DMRtimeout(void);
/*----------------------------------------------------------*/
void	putsUTM(lib *),putsLLA(lib *);
lib		*scanUTM(lib *,char *,int),*scanLLA(lib *,char *,int);
coord	*IsectOff(coord *, coord *);
void	WriteSophie(char *);
/*--------------------------------------------------------------
file utm1.c
*/
typedef struct datum	datum;
typedef	struct zone		zone;

zone	*FindZone(double, double);
uchar	*SaveZone(uchar *);
uchar	*LoadZone(uchar *);
uchar	*ListZone(uchar *);
void	ShowUtm(lib *l), ShowZone(lib *l);
struct datum
{
	char	*ref, *ellipsoid;
	float	dr, df;
	int		dx, dy, dz;
};

struct	zone
{
	double	clat, clon, wlat, wlon, stepx, stepy, xfalse, yfalse;
	double	a,b;
	int		nx, ny, x, y;
	char	**id, *name;
	zone	*next;
};

size_t	put_byte(char **, void *, size_t);
char 	*get_byte(void *, char *, size_t);
#define get_rtime(a,b) get_byte(a,b,sizeof(rtime))
#define put_rtime(a,b) put_byte(a,b,sizeof(rtime))

int		getint(char *);
long	getlong(char *);
float	getfloat(char *);
double	getdouble(char *);

char	*get_double(double *,char *);
char	*get_float(float *,char *);
char	*get_long(long *,char *);
char	*get_int(int *,char *);
char	*get_char(char *,char *);

int		put_double(char **, double);
int		put_float(char **, float);
int		put_long(char **, long);
int		put_int(char **, int);
int		put_char(char **, char);

void	CountBlink(void);
void	CountDown(void);
