#include		"remmenu.h"
#include		"io332.h"
/*--------------------------------------------------------------------------*/
#include		<rcopy.h>		/* PROM compile option						*/
extern void		init_ram(void);	/* define init. segment 					*/
/*--------------------------------------------------------------------------*/
/* KEYPAD DRIVER VARIABLES													*/
/*																			*/
/*--------------------------------------------------------------------------*/
int				key_count,		/* debounce counter							*/
				key_slow,		/* repeat stroke slow counter				*/
				key_old,		/* active key								*/
				key;			/* debounced key							*/

#define			KEY_BOUNCE	2
/*--------------------------------------------------------------------------*/
uchar	shaft_buff = 0x40;		/* enc. register mirror, write only			*/
/*--------------------------------------------------------------------------*/
void	ResetInit()
		{
		int		i;
#ifdef	GRAPHIC_LCD
		PORTC=0x76;
#else
		PORTC=0x7E;
#endif
		SYPCR	=0x00C0;	/* fffa21	*/
		CSBARBT	=0x0006;	/* fffa48 FLASH512k, RW, 16-bit port, 0x000000	*/
		CSORBT	=0x7B70;	/* fffa4a	*/
		CSBAR0	=0x0C05;	/* fffa4c RAM 256k, RW, 16-bit port, 0x080000	*/
		CSOR0	=0x7B70;	/* fffa4e	*/
		CSBAR1	=0x2000;	/* fffa50 LCD   2k, RW, 16-bit port, 0x200000	*/
		CSOR1	=0x7B70;	/* fffa52	*/
		CSBAR2	=0x2010;	/* fffa54 DSP   2k, RW, 16-bit port, 0x201000	*/
		CSOR2	=0x7B70;	/* fffa56	*/
		CSBAR3  =0xfff8;	/* fffa58 DSPavec 2k,RW,16-bit port, 0xffff80	*/
		CSOR3   =0x7801;	/* fffa5a all external IRQs use AUTOVECTORS		*/
		CSBAR4	=0x2020;	/* fffa5c input 2k,  R, 16-bit port, 0x202000	*/
		CSOR4	=0x6B70;	/* fffa5e	*/
		CSBAR5	=0x2030;	/* fffa60 output2k,  W, 16-bit port, 0x203000	*/
		CSOR5	=0x7370;	/* fffa62	*/
		CSBAR9	=0x0007;	/* fffa70 WE lo 1M, Wl, 16-bit port, 0x000000	*/
		CSOR9	=0x3370;	/* fffa72	*/
		CSBAR10	=0x0007;	/* fffa74 WE hi 1M, Wh, 16-bit port, 0x000000	*/
		CSOR10	=0x5370;	/* fffa76	*/

		CSPAR0	=0x3cff;	/* fffac4	*/
		CSPAR1	=0x03c0;	/* fffac6	A19 = output !!!					*/ 

		DDRE	=0xFF;		/* fffa15 port E = outputs						*/
		PORTE	=0xBF;		/* fffa11 C100 = GPS = off 						*/
		ENCw	=0x40;		/* encoders off									*/

		watchdog();
		rcopy(init_ram);	/* inicializacija NV spremenljivk				*/
		watchdog();
  		for(i=2; i<25; ++i)	/* vector lookup table setup					*/
			SetVect(reset_exc,i);
		watchdog();
		initSPI();			/* cimhitrejsa inic. SPI zaradi dostopa			*/
		initLCD();			/* LCD inicializacija, obvezno pred				*/
		Initialize ();		/* vklopom -Vcc (PWM !!!), da se ne				*/
		InitTPUchannels ();	/* pojavi crn trak!								*/
		_SPL(0);			/* RS232, PWM (-Vcc) in temp. senzor			*/
		SYNCR	=0x7f00;	/* speedup to 16.777 MHz !!!					*/
		CSORBT	=0x7870;	
		CSOR0	=0x7870;
		/*--------------------------------------------------------------------------*/
		getT();
		if(readRTC(0x30) & 0x10)	{
			shutdown_save(NULL);
			writeRTC(0x20,0x00);	/* seconds						*/
			writeRTC(0x21,0x00);	/* minutes						*/
			writeRTC(0x22,0x00);	/* hours						*/
			writeRTC(0x23,0x06);	/* day							*/
			writeRTC(0x24,0x11);	/* date							*/
			writeRTC(0x25,0x03);	/* month						*/
			writeRTC(0x26,0x94);	/* year							*/
			for(i=0; i<8; ++i)
				writeRTC(i,0x00);	/* submit registers				*/
			}
		PIT_task = SetTask(NULL,null,PIT_task);
		RTC_task = SetTask(NULL,null,RTC_task);
        ReadTime(&Time);
/*		eerror=RSR & 0x7F;
*/		main();
		}
/*------------------------------------------------------------------*/
_IH		void	PITservice(void)
		{
void	IRQservice(void);
		IRQservice();
		}
/*------------------------------------------------------------------*/
void	PIT_enable(void)
		{
		SetVect(PITservice,0x24);	
		PITR=0x00A0;				/* 19.5 msec, ca. 50Hz			*/
		PICR=0x0124;				/* level 1, int 24H				*/
		}
/*------------------------------------------------------------------*/
void	PIT_disable(void)
		{
		PITR=0;						/* timer stop					*/
		}
/*------------------------------------------------------------------*/
void	SetVect(void (*f)(void),int n)
{
	void	*p;
	get_byte(&p,(void *)(n<<2),sizeof(void *));
	if(p!=f)
		prog_vect(&f,(uchar *)(n<<2),sizeof(void *));
}
/*------------------------------------------------------------------*/
/*	SPI driver														*/
/*																	*/
/*------------------------------------------------------------------*/
void	initSPI(void)
		{
/*------------------------------------------------------------------*/
		QPDR=0xe3;
		QPAR=0x7b;
		QDDR=0xfe;
/*------------------------------------------------------------------*/
/* MAX132 SPI na fix. lokacijah 0-5, RTC na lokaciji 6 !!!			*/

		tx_ram[0]=tx_ram[3]=0x40;
		tx_ram[1]=tx_ram[4]=0x44;
		tx_ram[2]=tx_ram[5]=0xD2;

		com_ram[0]=com_ram[1]=com_ram[2]=0x78; /* PCS2=1, PCR3=0    */
		com_ram[3]=com_ram[4]=com_ram[5]=0x74; /* PCS2=0, PCR3=1    */
		com_ram[6]=0x7E;					   /* DS1620 used		*/

		SPCR0	=	0xA108;		/* master, 8 bits, CPOL=0, CPHA _-  */
		SPCR2	=	0x0500;
		SPCR3	=	0x0000;
		SPCR1	|=	0x8404;
/*------------------------------------------------------------------*/
		writeRTC(0x31,0xB8);		/* clock start, xtal 32kHz		*/
									/* 50Hz, xtal output			*/
		writeRTC(0x32,0x0C);		/* watchdog off, alarm off		*/
									/* 1 Hz output					*/
		if((readRTC(0x31) != 0xB8) || (readRTC(0x32) != 0x0C)) {
			com_ram[6]=0x7F;	   /* DS1620 not used				*/
			writeRTC(0x31,0xB8);
			writeRTC(0x32,0x0C);
			}
		else
			readTEMP(0xEE,eof);
		}
/*------------------------------------------------------------------*/
int		readRTC(int a)
		{
		int	i;
		while(SPI_busy());

		i = _GPL();
		_SPL(7);
		tx_ram[6]=(a<<8) | 0xff;/* hibyte = a, lobyte = ff !!!		*/

		SPSR	&=	~0x80;
		SPCR0	=	0x8108;		/* master, 16 bits, CPOL=0, CPHA -_	*/
		SPCR2	=	0x0606;
		SPCR1	|=	0x8000;
		_SPL(i);

		while(SPI_busy());
		return(rx_ram[6] & 0xFF);
		}
/*------------------------------------------------------------------*/
int		writeRTC(int a, int x)
		{
		int	i;
		while(SPI_busy());

		i = _GPL();
		_SPL(7);
		tx_ram[6]=((a | 0x80)<<8) + (x & 0xff);

		SPSR	&=	~0x80;
		SPCR0	=	0x8108;		/* master, 16 bits, CPOL=0, CPHA -_ */
		SPCR2	=	0x0606;
		SPCR1 	|=	0x8000;
		_SPL(i);

		while(SPI_busy());
		return(rx_ram[6] & 0xFF);
		}
/*------------------------------------------------------------------*/
double	getT(void)
{
	double	Tsw,Toff;
	if(tcm2.status & 0x80)
		return(tcm2.t);
	if(com_ram[6] == 0x7F)
	{
		Tsw		= 	Tmax-Tmin;
		Tsw 	/=	tn_max-tn_min;
		Toff	= Tmax-Tsw*tn_max;
		return(Tsw * (temp >> 5) + Toff);
	}
	return(readTEMP(0xAA,eof)/256.0);
}
/*------------------------------------------------------------------*/
/*** 	MAX132 conversion		***/

void	writeADC(void) {
		int	i;
		while(SPI_busy());

		i = _GPL();
		_SPL(7);
		SPSR	&=	~0x80;
		SPCR0	=	0xA108;		/*** master, 8 bits, CPOL=0, CPHA _- ***/
		SPCR2	=	0x0500;
		SPCR1	|=	0x8000;
		_SPL(i);
		}
/*------------------------------------------------------------------*/
void	ADC_scan(void)
		{
		if(PITcnt % 2) {
				p_off[PITcnt/2] = (rx_ram[5]<<8) + rx_ram[4];
				r_off[PITcnt/2] = (rx_ram[2]<<8) + rx_ram[1];
				tx_ram[2]=tx_ram[5]=0xD2;
				}
		else	{
				p_filt[PITcnt/2] = (rx_ram[5]<<8) + rx_ram[4];
				r_filt[PITcnt/2] = (rx_ram[2]<<8) + rx_ram[1];
				tx_ram[2]=tx_ram[5]=0xC2;
				}

		writeADC();
		PITcnt = ++PITcnt % (2 * 8);
		}
/*------------------------------------------------------------------*/
/*		keyboard driver												*/
/*																	*/
/*------------------------------------------------------------------*/
/* old  PC, PT 1,2,3,4												*/
#ifdef	WIN32
int		KEY0_1st[]={			0x15,	0x14,	0x13,	0x12,
								0x25,	0x24,	0x23,	0x22,
								0x35,	0x34,	0x33,	0x32,
								0x45,	0x44,	0x43,	0x42,

											0x54,
	0x66,0x61,0x64,		0x63,0x62,		0x55,	0x52,
											0x53
																};
/*------------------------------------------------------------------*/
/* rev. PC, PT 5,6,7,8,9											*/
#else
int		KEY1_1st[]={			0x11,	0x14,	0x13,	0x12,
								0x21,	0x24,	0x23,	0x22,
								0x31,	0x34,	0x33,	0x32,
								0x41,	0x44,	0x43,	0x42,

											0x54,
	0x65,0x61,0x64,		0x63,0x62,		0x51,	0x52,
											0x53
																};
#endif
/*------------------------------------------------------------------*/
int		KEY_2nd[]={				F1,	'7',	'8',	'9',
								F2,	'4',	'5',	'6',
								F3,	'1',	'2',	'3',
								F4,	CLEAR,	'0',	ENTER,

											UP,
	ONOFF,ONOFF,BACKLIT,	ALPHA,	SIGN,	LEFT,	RIGHT,
											DOWN

																};
/*------------------------------------------------------------------*/
void	DecodeKey(void)
		{
		int		i,*p;

#ifdef	WIN32
		p=KEY0_1st;
#else
		p=KEY1_1st;
#endif

		Key=0;
		for(i=0;i<25;++i)
			if(key == p[i]) {
				Key=KEY_2nd[i];
				break;
				}
		}
/*------------------------------------------------------------------*/
void	key_scan(void)
		{
		int		i,j,k;
		int		log2i(int);

		for(i=1,j=0;i<64;i=i*2)
			{
			PORTE=((~i) & 0x3F)|(PORTE & 0xC0);
			for(k=0;k<10;++k);
			k=PORTF;
			if((k & 0x1F) != 0x1F)
				j=16*log2i(i)+log2i(~k & 0x1F);
			}

		if(j != key_old)
			{
			key_old=j;
			key_count=KEY_BOUNCE;
			key_slow=KEY_BOUNCE*10;
			}
		else
			{
			if(key_count)
				--key_count;
			else
				{
				key_count=key_slow;
				key=key_old;
				key_slow=KEY_BOUNCE*3;
				DecodeKey();
				}
			}
		if(PushCount)
			--PushCount;
		else
			if(PushKey) {
				Key=PushKey;
				PushKey=null;
				}
		}
/*----------------------------------------------------------*/
int		log2i(int n)
		{
		int	i=1;
		if(n==0)	return(0);
		while(n%2 == 0)
			{
			++i;
			n=n/2;
			}
		return(i);
		}
/*------------------------------------------------------*/
/* Heidenhain ROC415, GRAY driver						*/
/*------------------------------------------------------*/
void	shaft_ROC415G(void)
{
	int		i,j,k;
	long	a,e;

	if(shaft_buff & 0x40)
	{
		if(shaft & 0x80)
		{
			shaft_buff &= 0x80;		/* brisi vse + vklop	*/
			shaft_buff |= 0x09;		/* clock 1,2 = 1		*/
			ENCw=shaft_buff;
		}
	}
	else
	{
		for(i=j=k=0; i< 16; ++i)
		{
			shaft_buff &= 0x80;
			shaft_buff |= 0x12;		/* clock 1,2= 0			*/
			ENCw=shaft_buff;
			shaft_buff &= 0x80;
			shaft_buff |= 0x09;		/* clock 1,2= 1			*/
			ENCw=shaft_buff;
			j = (j<<1) + ((~ENCr & 0x02)>>1);
			k = (k<<1) + ((~ENCr & 0x08)>>3);
		}
		shaft_buff |= 0x40;
		ENCw=shaft_buff;
		for(a=e=0L,i=0; i<16; ++i)
		{
			a = (a<<1) | ((a ^ j) & 0x8000);
			j=(j<<1);
			e = (e<<1) | ((e ^ k) & 0x8000);
			k=(k<<1);
		}
		shaft_data[0]=e>>16;
		shaft_data[1]=a>>16;
		shaft &= 0x7f;
	}
}
/*------------------------------------------------------*/
/* Heidenhain ROC415, PURE BINARY driver				*/
/*------------------------------------------------------*/
void	shaft_ROC415(void)
{
	uint		i,j,k,jj,kk,jjj,kkk,n[16];

	if(shaft_buff & 0x40)
	{
		if(shaft & 0x80)
		{
			shaft_buff &= 0x80;					/* brisi vse + vklop */
			shaft_buff |= 0x09;					/* clock 1,2 = 1		*/
			ENCw=shaft_buff;
		}
	}
	else
	{
		for(i=0; i< 16; ++i)
		{
			ENCw=(shaft_buff & 0x80) | 0x12;	/* clock 1,2= 0	*/
			ENCw=(shaft_buff & 0x80) | 0x09;	/* clock 1,2= 1	*/
			n[i]=ENCr;
		}
		j=jj=jjj=k=kk=kkk=0;
		for(i=0; i< 16; ++i)
		{
			j = (j<<1) | ((~n[i] & 0x02)>>1);
			k = (k<<1) | ((~n[i] & 0x08)>>3);
			jj = (jj<<1) | (n[i] & 0x01);
			kk = (kk<<1) | ((n[i] & 0x04)>>2);
			jjj ^= n[i] & 0x01;
			kkk ^= n[i] & 0x04;
		}
		if(j==jj && k==kk && !jjj && !kkk)
		{
			shaft_buff |= 0x40;
			ENCw=shaft_buff;
			shaft_data[0]=k>>1;
			shaft_data[1]=j>>1;
			shaft &= 0x7f;
		}
	}
}
/*------------------------------------------------------*/
int		BatteryOK(void)
		{
		return(PORTF & 0x80);
		}
/*------------------------------------------------------*/
void	C100_on(void)
		{
		PORTE=PORTE | 0x40;
		c100.status &= ~0x80;
		DeleteTask(CheckRLD_off,RTC_task);
		DeleteTask(CheckRLD_on,RTC_task);
		DeleteTask(ScanRLD,RTC_task);
		SetTask(RLD_Remote,1,RTC_task);
		SetTask(RLD_PowerOff,1,RTC_task);
		RLD_PowerOff();		
		}
/*------------------------------------------------------*/
void	C100_off(void)
		{
		PORTE=PORTE & 0xBF;
		rld.status = null;
		DeleteTask(RLD_Remote,RTC_task);
		DeleteTask(RLD_PowerOff,RTC_task);
		SetTask(CheckRLD_off,1,RTC_task);
		SetTask(ScanRLD,0,RTC_task);
		}
/*------------------------------------------------------------------*/
void	watchdog(void)
		{
		SWSR=0x55;
		SWSR=0xAA;
		}
/*------------------------------------------------------------------*/
int		GPS_on(void)
		{
int		i=PORTE;
		PORTE=PORTE & 0x7F;
		return((~i) & 0x80);
		}
/*------------------------------------------------------------------*/
int		GPS_off(void)
		{
int		i=PORTE;
		PORTE=PORTE | 0x80;
		return(i & 0x80);
		}
/*------------------------------------------------------------------*/
void	BacklitOff(void)
		{
		shaft_buff &= ~0x80;
		ENCw = shaft_buff;
        Backlit &= ~1;
		DeleteTask(BacklitOff,RTC_task);
		}
/*------------------------------------------------------------------*/
void	BacklitOn(void)
		{
        if(Backlit/2)
			SetTask(BacklitOff,Backlit/2,RTC_task);
        Backlit |= 1;
		shaft_buff |= 0x80;
		ENCw = shaft_buff;
		}
/*------------------------------------------------------------------*/
int		LcdLevel(int i)
		{
		int j=i;

		if(i <= 0)
			return(1);
		if(i >= CHaPAR3)
			return(CHaPAR3-1);
		i -= (Tamb-25.0)/4.0;

		if(i && i<CHaPAR3)
			CHaPAR2=i;
		return(j);
		}
/*------------------------------------------------------------------*/
void	device_off(void)
		{
/* alarm mask + powerdown, periodic = off !!!						*/
		writeRTC(0x32,0x40 | (readRTC(0x32) & 0x10));
		}
/*------------------------------------------------------------------*/
void	HeatOn(void)
		{
		PORTC |= 0x01;
		}
/*------------------------------------------------------------------*/
void	HeatOff(void)
		{
		PORTC &= ~0x01;
		}
/*------------------------------------------------------------------*/
void	__SystemInit(void)
		{
		void	code_top();
		void	(*p)()=code_top;
		long	*f=(long *)FLASHTOP;

		PIT_enable();
/*------------------------------------------------------------------*/
		library=LIBRARY;
		fonts=FONTS;
		syspar=SYSPAR;
/* check for Am29F010							--------------------*/
		if(f[0] == f[0x10000])
			library=(char *)p;			/* konec kode, glej remflash*/
/*------------------------------------------------------------------*/
/*		PIT irq tasks												*/

		SetTask(ADC_scan,5,PIT_task);
		SetTask(key_scan,1,PIT_task);
		shutdown_load(NULL);
		watchdog();
		}
/*------------------------------------------------------------------*/
/* LCD driver														*/
#ifdef		GRAPHIC_LCD
#include	"lcdfont.inc"
void	initLCD(void)
		{
		 int i;
		 unsigned char aD[128]=
			{
			 1,0x40,	/* SYSTEM_SET									*/
			 0,0x35,	/*  DR TL IV 1   WS M2 M1 M0					*/
			 0,0x07,	/*	WF 0  0  0   0  ---FX---   char width		*/
			 0,0x0f,	/*  0  0  0  0   -----FY----   char height		*/
			 0,27,		/*  C/R characters/line							*/
			 0,0xa0,	/*	TC/R scan caracters per line				*/
			 0,0x3f,	/*  L/F lines per frame							*/
			 0,28,		/*  APL	virtual screen size						*/
			 0,0x00,	/*  APH											*/
	
			 1,0x44,	/* SCROLL										*/
			 0,0x00,	/*  SAD1 (low)									*/
			 0,0x00,	/*  SAD1 (high)									*/
			 0,80,		/*  SL1											*/
			 0,0x00,	/*  SAD2 (low)									*/
			 0,0x10,	/*  SAD2 (high)									*/
			 0,80,		/*  SL2											*/
			 0,0x00,	/*  SAD3 (low)									*/
			 0,0x20,	/*  SAD3 (high)									*/

			 1,0x5A,	/* HDOT_SCROLL									*/
			 0,0x00,	/*  0  0  0  0   0  D2 D1 D0   pixels			*/
	
			 1,0x5B,	/* OVLAY										*/
			 0,0x00,	/*  0  0  0  OV  DM2 DM1 MX1 MX0				*/
						/*	MX-screen combination DM-mode(0 text,1		*/
						/* graphics) OV(1 3-lay)						*/
	
			 1,0x59,	/* DISPLAY_ON									*/
			 0,0x00,	/*  FP5 FP4 FP3 FP2   FP1 FP0 FC1 FC0  3-2-1	*/
						/* layers and cursor (01=0N, 00=OFF else flash	*/
	
			 1,0x5D,	/* CSRFORM										*/
			 0,0x07,	/*  0  0  0  0  ---CRX---	cursor width		*/
			 0,0x0f,	/*  CM 0  0  0  ---CRY---	cursor height and	*/
						/* type (0=underline, 1=block)					*/

			 1,0x4c,	/* CSRDIR	0100 11xx	00=right 01=left 10=up	*/
						/* 11=down										*/
		
			 1,0x46,	/* CSRW cursor address							*/
			 0,0x00,
			 0,0x00,
		 
			 1,0x5c,	/* CGRAM address								*/
			 0,0x00,
			 0,0xf0,
			 1,0x46,	/* CSRW cursor address (point to CGRAM)			*/
			 0,0x00,
			 0,0xF0,
			 1,0x42,	/* WRITE_DATA									*/
			 2
			};

		 do
			{
			 LCD_WRITE(aD[i],aD[i+1]);
			 i+=2;
			} while(aD[i]!=2);

		 for(i=0;i<4096;i++)
			{
			 LCD_WRITE(0,aLcdFont[i]);
			}


		 LCD_WRITE(1,0x46);
		 LCD_WRITE(0,0x00);
		 LCD_WRITE(0,0x00);
		 LCD_WRITE(1,0x42);
		 for(i=0;i<30*4;i++)
			{
			 LCD_WRITE(0,' ');
			}
		 LCD_WRITE(1,0x46);
		 LCD_WRITE(0,0x00);
		 LCD_WRITE(0,0x00);

		 LCD_WRITE(1,0x59);
		 LCD_WRITE(0,0x04);
		}
/*--------------------------------------------------------------------*/
/* LCD driver, izpis slike v LcdBuffer na zaslon					  */
/* nivo PIT, inic. ob vsaki spremembi vsebine LcdBuffer, SetLCDaddr,  */
/* Cursor, putLCD in screen_swap skrajsajo klicni cas 5 tickov		  */
/*																	  */
int	Adec(uchar *, uchar *, int, int);
void	RefreshScreen(void)
{
	int		i,j,k;
	uchar	p[20];

	LCD_WRITE(1,0x59);				/* cursor off					*/
	LCD_WRITE(0,0x04);

	for(i=0; i<4; ++i)
	{
		LCD_WRITE(1,0x46);			/* zacetek vrstice				*/
		LCD_WRITE(0,28*i);
		LCD_WRITE(0,0x00);

		Adec(&LcdExt[8*i+7],&p[7],0,8);

		LCD_WRITE(1,0x42);			/* IZPIS NA LCD EXT				*/
		for(j=0; j<8; ++j)
			LCD_WRITE(0,p[j]);
	}

	for(i=0; i<4; ++i)
	{
		LCD_WRITE(1,0x46);			/* zacetek vrstice				*/
		LCD_WRITE(0,28*i+8);
		LCD_WRITE(0,0x00);

		Adec(&LcdBuffer[20*i+19],&p[19],0,20);

		LCD_WRITE(1,0x42);			/* IZPIS NA LCD					*/
		for(j=0; j<20; ++j)
			LCD_WRITE(0,p[j]);
	}

	j=(signed char)LcdBuffer[80]/20;
	k=(signed char)LcdBuffer[80]%20;
	j=28*j+k+8;
	
	LCD_WRITE(1,0x46);				/* KURZOR 						*/
	LCD_WRITE(0,j);
	LCD_WRITE(0,0x00);

	switch(LcdBuffer[81])			/* VRSTA KURZORJA				*/
	{
	case CURS_UND:
		 LCD_WRITE(1,0x5D);
		 LCD_WRITE(0,0x07);
		 LCD_WRITE(0,0x0f);
		 LCD_WRITE(1,0x59);
		 LCD_WRITE(0,0x07);
		break;
	case CURS_FULL:
		 LCD_WRITE(1,0x5D);
		 LCD_WRITE(0,0x07);
		 LCD_WRITE(0,0x8f);
		 LCD_WRITE(1,0x59);
		 LCD_WRITE(0,0x07);
		break;
	case CURS_OFF:
		 LCD_WRITE(1,0x59);
		 LCD_WRITE(0,0x04);
		break;
	}
	SetTask(RefreshScreen,500,PIT_task);
}

#else

/*------------------------------------------------------------------*/
int		LcdBusy(void)
{
	return(LCDstatus & 0x80);
}
/*------------------------------------------------------------------*/
void	initLCD(void)
		{
		while(LcdBusy());
		*LCD=0x38;
		while(LcdBusy());
		*LCD=0x01;
		while(LcdBusy());
		*LCD=0x02;
		while(LcdBusy());
		*LCD=0x06;
		while(LcdBusy());
		*LCD=0x0C;

		symLCD(1,sym_DELTA);
		symLCD(2,sym_FOTONA);
		symLCD(3,sym_DEGREE);
		symLCD(4,sym_ARROWd);
		symLCD(5,sym_GPSf);
		symLCD(6,sym_dGPS);
		symLCD(7,sym_dGPSf);

		while(LcdBusy());
		*LCD=0x80;
		while(LcdBusy());
		*LCD=0x01;
		while(LcdBusy());
		*LCD=0x02;
		}
/*--------------------------------------------------------------------*/
/* LCD driver, nivo PIT, inic. ob vsaki spremembi vsebine LcdBuffer	  */
/* SetLCDaddr, Cursor, putLCD in screen_swap skrajsajo klicni cas na  */
/* 0.1 sec (5 tick)													  */
void	RefreshScreen(void)
{
	int	i;

	for(i=0; i<80; ++i)
	{
		if(!(i%20))
		{
			while(LcdBusy());
			switch(i/20)
			{
			case 0:
				*LCD=0x80;
				break;
			case 1:
				*LCD=0x80+64;
				break;
			case 2:
				*LCD=0x80+20;
				break;
			case 3:
				*LCD=0x80+84;
				break;
			}
		}
		switch(LcdBuffer[i])
		{
		case symDELTA:
			LcdBuffer[i]=0x01;
			break;
		case symFOTONA:
			LcdBuffer[i]=0x02;
			break;
		case symDEGREE:
			LcdBuffer[i]=0x03;
			break;
		case symARROWd:
			LcdBuffer[i]=0x04;
			break;
		case symGPSf:
			LcdBuffer[i]=0x05;
			break;
		case symdGPS:
			LcdBuffer[i]=0x06;
			break;
		case symdGPSf:
			LcdBuffer[i]=0x07;
			break;
		}
		while(LcdBusy());
		LCDdr=LcdBuffer[i];
	}

	i=GetLCDaddr();
	switch(i/20)
	{
	case 0:	
		break;
	case 1:	
		i=i+44;
		break;
	case 2:
		i=i-20;
		break;
	case 3:
		i=i+24;
	}
	while(LcdBusy());
	*LCD=((i & 0x7F) | 0x80);
	while(LcdBusy());
	switch(LcdBuffer[81])
	{
	case CURS_UND:
		*LCD=0x0E;
		break;
	case CURS_FULL:
		*LCD=0x0F;
		break;
	case CURS_OFF:
		*LCD=0x0C;
		break;
	}
	SetTask(RefreshScreen,500,PIT_task);
}
/*------------------------------------------------------------------*/
void	symLCD(int n,char *p)
		{
		int	i, j;
		while(LcdBusy());
		j=GetLCDaddr();
		*LCD=0x40 + ( n<<3 );
		for(i=0; i<8; ++i) {
			while(LcdBusy());
			LCDdr=p[i];
			}
		while(LcdBusy());
		SetLCDaddr(j);
		}
#endif
/*------------------------------------------------------------------*/
_IH	void	reset_exc(void)
{
	reset_f();
}
/*------------------------------------------------------------------*/
void	Shutdown(void)
		{
		rtime	t1,t2;

		clrlcd();
		t2=t1=Time;
		addsecs(&t1, 3600.0 * readRTC(0) + 60.0 * readRTC(1) + readRTC(2));
		if(dtime(&t1,&t2))
			AlarmEnable(&t1);
		shutdown_save(NULL);
		SaveEE(NULL);
		DeleteTask(ScanRLD,RTC_task);
		DeleteTask(CheckRLD_on,RTC_task);
		DeleteTask(CheckRLD_off,RTC_task);
		DeleteTask(Shutdown,RTC_task);
		RLD_PowerOff();
		SysMessage(CHPC);
		wait(10);
		device_off();
		stopCPU();
		}
