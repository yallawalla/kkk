//#include	<windows.h>
#include	<stdio.h>
#include	<time.h>
#include	<io.h>
#include	<string.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<dos.h>
#include	<conio.h>
#include	<share.h>
#include	"remmenu.h"
#define		SECTLEN	0x20000

void	ADCexe(void);
void	IRQservice(void);
void	SysCtl(void);
void	ScanConsole(void);

char	PORTE;
char	dsp_gain[4];
int		nrf,arf,Elevation,Azimuth,Pitch,Roll;
uint	rx_ram[32],tx_ram[32];

void	RTCtime(void);

FILE	*fflash,*frtc;

uchar	rtc[64];
char	auxExtBuffer[256],auxLcdBuffer[256], auxLcdFlag;
/*--------------------------------------------------------*/
void	stack_top(void)
{
}
void	_main(void)
{
}
/*--------------------------------------------------------*/
void	__SystemInit(void)
{
		long		i;
		char	c[256];
/*--------------------------------------------------------*/
		FILE	*f=fopen("\\sysctlw.dat","w");
		fclose(f);
		f=fopen("\\rflink.dat","w");
		fclose(f);
/*--------------------------------------------------------*/
		library=malloc(SECTLEN*sizeof(char));
		f=fopen("libflash.art","rb");
		if(f)
		{
			fread(library,sizeof(char),SECTLEN,f);
			fclose(f);
		}
		else
		{
			for(i=0; i<SECTLEN; ++i)
				library[i]=0xff;
		}
/*--------------------------------------------------------*/
		frtc=fopen("rtcW32.dat","rb");
		if(frtc)
		{
			fread(rtc,sizeof(char),64,frtc);
			fclose(frtc);
		}
/*--------------------------------------------------------*/
		clrlcd();
		for(i=0; i<256; ++i)
		{
			auxLcdBuffer[i]=' ';
			auxExtBuffer[i]=' ';
		}
		PIT_task=	SetTask(NULL,null,PIT_task);
		RTC_task=	SetTask(NULL,null,RTC_task);
		PIT_task=	SetTask(ADCexe,5,PIT_task);

		SetTask(RTCtime,0,RTC_task);
		shaft_enable();

// mišmaš zaradi inicializacije const *[] 
		for(i=0; _MenuCode[i]; ++i)
		{
			strcpy(c,_MenuCode[i]);
			_MenuCode[i]=memalloc((strlen(c)+1)*sizeof(char));
			strcpy(_MenuCode[i],c);
		}
		for(i=0; _TextCode[i]; ++i)
		{
			strcpy(c,_TextCode[i]);
			_TextCode[i]=memalloc((strlen(c)+1)*sizeof(char));
			strcpy(_TextCode[i],c);
		}
		for(i=0; _MenuHead[i]; ++i)
		{
			strcpy(c,_MenuHead[i]);
			_MenuHead[i]=memalloc((strlen(c)+1)*sizeof(char));
			strcpy(_MenuHead[i],c);
		}
		Initialize();
		SetTask(SysCtl,0,RTC_task);
		SetTask(ScanConsole,10,RTC_task);
}
/*--------------------------------------------------------*/
int		BatteryOK(void)
		{
		return(-1);
		}
/*--------------------------------------------------------*/
void	watchdog(void)
		{
		}
/*--------------------------------------------------------*/
void	Shutdown(void)
		{
		DeleteTask(Shutdown,RTC_task);
		shutdown_save(NULL);
		SaveEE(NULL);

		frtc=fopen("rtcW32.dat","wb");
		fwrite(rtc,sizeof(char),64,frtc);
		fcloseall();
		ExitThread(0);
		}
/*--------------------------------------------------------*/
void	beep(int i)
		{
		MessageBeep(0xFFFFFFFF);
		}
/*--------------------------------------------------------*/
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
/*--------------------------------------------------------*/
int   	LcdLevel(int i)
		{
		lcd=i;
		return(i);
		}
/*--------------------------------------------------------*/
void	C100_on(void)
		{}
/*--------------------------------------------------------*/
void	C100_off(void)
		{}
/*--------------------------------------------------------*/
int		DspGain(int i, int j)
		{
		dsp_gain[i] += j;
		return(dsp_gain[i]);
		}
/*--------------------------------------------------------*/
void	DspAddr(unsigned int i)
		{
		arf=i;
		}
/*--------------------------------------------------------*/
int		dspinit(unsigned int i)
		{
		arf=i;
		return(null);
		}
//*--------------------------------------------------------*/
void	shaft_driver(void)
		{
        if(shaft & 0x80) {
        	shaft_data[0] = Elevation / SHtoR;
			shaft_data[1] = Azimuth / SHtoR;
			shaft &= 0x7f;
            }
        }
void	shaft_ITEK16uS(void)
		{
        shaft_driver();
		}
void	shaft_ROC415(void)
		{
        shaft_driver();
		}
void	shaft_ROC415G(void)
		{
		shaft_driver();
		}
/*--------------------------------------------------------*/
void	key_scan(void)
		{}
/*--------------------------------------------------------*/
void	writeADC(void)
		{}
/*--------------------------------------------------------*/
void	sector_erase(uchar *p, int r)
{
	long	i;
	FILE	*f;
	for(i=0; i<SECTLEN; ++i)
		library[i]=0xff;
	f=fopen("libflash.art","wb");
	fwrite(library,sizeof(char),SECTLEN,f);
	fclose(f);
	}
/*--------------------------------------------------------*/
void	FileFlash(void)
{
	FILE	*f;
	f=fopen("libflash.art","wb");
	fwrite(library,sizeof(char),SECTLEN,f);
	fclose(f);
	DeleteTask(FileFlash,RTC_task);
	}
/*--------------------------------------------------------*/
void	*prog_byte(void *src,uchar *dst,size_t n)
{
	uchar	*from=src;
	while(n--)
	{
		if(dst == &library[SECTLEN])
		{
			eerror |= 0x80;
			continue;
		}
		if(*dst==*from)
		{
			++dst;
			++from;
			continue;
		}
		eerror |= 0x01;
		if(*dst == 0xff)
		{
			if(eerror & 0x80)
			{
				++dst;
				++from;
				continue;
			}
			else
			{
				*dst++ = *from++;
				eerror &= ~0x01;
			}
		}
		else
		{
			++dst;
			++from;
			eerror |= 0x80;
		}
	}
SetTask(FileFlash,0,RTC_task);
return(dst);
}
/*--------------------------------------------------------*/
void	*load_byte(void *dst, uchar *src, size_t n)
{
	uchar	*to=dst;
	while(n--)
		*to++ = *src++;
	return(src);
}
/*--------------------------------------------------------*/
int		SendStat(int i)
		{
		return(null);
		}
/*--------------------------------------------------------*/
int		sendblk(unsigned int addr, char *c, int n, unsigned int type)
		{
		char	cc[2*LINK_LEN];
		FILE 	*f;
		int		i,j;
		n = (n+1) & ~1;
		if(n >= 256)
			return(eof);
		do f=_fsopen("\\rflink.dat","a",SH_DENYNO); while(!f);
		sprintf(cc,"M=%04X,%04X,%04X.",n,addr,LinkAddr(OBP));
		for(i=0;i<n;++i) {
			j=(uchar)c[i];
			sprintf(strchr(cc,0),"%02X.",j);
		}
		fprintf(f,"%s",cc);
		fclose(f);
		return(1);
		}
/*--------------------------------------------------------*/
int		recblk(unsigned int *a, char *p)
		{
		char	c[2*LINK_LEN],*cc=c;
		int		i,j,n,obp=eof;
		FILE	*f;
static	int		fpos;
		f=_fsopen("\\rflink.dat","r",SH_DENYNO);
		if(f) {
			fseek(f,fpos,0);
			if(fgets(c,2*LINK_LEN,f))
				if(sscanf(c,"M=%04X,%04X,%04X.",&n,&obp,a)==3) {
					if(strlen(c) >= 3*n + 17)
						for(i=0; i<n; ++i) {
							cc=strchr(cc,'.');
							++cc;
							sscanf(cc,"%02X.",&j);
							p[i]=j;
							fpos=ftell(f);
							}
					else
						obp=eof;
					}
			fclose(f);
			if(obp ==  LinkAddr(OBP) || !obp)  {
				return(n);
				}
			}
		return(null);
		}
/*--------------------------------------------------------
void   	ExtCtl(void)
{
static	int		i;
		char	c[128],cc[32];
		FILE	*f=_fsopen("\\fkey.dat","r",SH_DENYNO);
		if(f)
		{
			fseek(f,i,0);
			if(fgets(c,127,f))
			{
			    if(sscanf(c,"k=%s\n",cc))
					Key=chrtx(cc);
				if(sscanf(c,"p=%6d\n",&i))
		        	Pitch=(double)i*10.0;
			    if(sscanf(c,"r=%6d\n",&i))
					Roll=(double)i*10.0;
				if(sscanf(c,"e=%6d\n",&i))
		        	Elevation= fmod((double)i/10000.0+2.0*M_PI,2.0*M_PI)/M_PI*0x4000;
			    if(sscanf(c,"a=%6d\n",&i))
					Azimuth= fmod((double)i/10000.0+2.0*M_PI,2.0*M_PI)/M_PI*0x4000;
				if(sscanf(c,"c=%6d\n",&i))
		        	c100.az = fmod((double)i/10000.0+2.0*M_PI,2.0*M_PI)/M_PI*180.0;
				i=ftell(f);
			}
			fclose(f);
		}
}
--------------------------------------------------------*/
void	RTCtime(void)
		{
char	i=rtc[0x20];

		SYSTEMTIME tt;
		GetLocalTime(&tt);

		rtc[0x22]=toBCD(tt.wHour);
		rtc[0x21]=toBCD(tt.wMinute);
		rtc[0x20]=toBCD(tt.wSecond);
		rtc[0x23]=toBCD(tt.wDayOfWeek+1);
		rtc[0x24]=toBCD(tt.wDay);
		rtc[0x25]=toBCD(tt.wMonth);
		rtc[0x26]=toBCD(tt.wYear % 100);
		
		if(i != rtc[0x20])
			rtc[0x30] |= 0x01;
		else
			rtc[0x30] &= ~0x01;
		}
/*--------------------------------------------------------*/
void	ADCexe(void)
		{
extern	int	PITcnt;
void	ADC_scan(void);

		if(!(PITcnt % 2))	{
			rx_ram[1] = Roll  % 256;
			rx_ram[2] = Roll  / 256;
			rx_ram[4] = Pitch % 256;
			rx_ram[5] = Pitch / 256;
			}
		else {
			rx_ram[1] = 0;
			rx_ram[2] = 0;
			rx_ram[4] = 0;
			rx_ram[5] = 0;
			}
		ADC_scan();
}
/*--------------------------------------------------------*/
void	PIT_enable(void)
		{}
void	PIT_disable(void)
		{}
int		heat_on(void *vp)
		{
		return(null);
		}
int		heat_off(void *vp)
		{
		return(null);
		}
void	InProg(void)
		{}
/*--------------------------------------------------------*/
void	ADC_scan(void)
		{
#define	AVRG		8
extern	int			r_filt[AVRG];			/*					*/
extern	int			p_filt[AVRG];			/* filter podatkov	*/
extern	int			r_off[AVRG];			/*					*/
extern	int			p_off[AVRG];			/* offset A/D		*/
extern	int			PITcnt;

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
		PITcnt = ++PITcnt % (2 * AVRG);
		}
/*--------------------------------------------------------*/
int		writeRTC(int a,int dd)
		{
		rtc[a]=dd;
		return(dd);
		}
/*--------------------------------------------------------*/
int		readRTC(int a)
		{
		return(rtc[a]);
		}
/*--------------------------------------------------------*/
void	HeatOn(void)
		{}
/*--------------------------------------------------------*/
void	HeatOff(void)
		{}
/*--------------------------------------------------------*/
void	BacklitOn(void)
		{
		}
/*--------------------------------------------------------*/
void	BacklitOff(void)
		{
		}
/*--------------------------------------------------------*/
int		readTEMP(int a, int b)
		{
		return(a);
		}
/*--------------------------------------------------------*/
int		disable_interrupts(void)
{
	return(null);
}
void	enable_interrupts(int i)
{
}
/*--------------------------------------------------------*/
double	getT(void)
		{
		return(35.0);
		}
/*--------------------------------------------------------*/
void	*ralloc(size_t n)
		{
        return(malloc(n));
        }
/*--------------------------------------------------------*/
void	rfree(void *p)
		{
        free(p);
		}
/*--------------------------------------------------------*/
void	RefreshScreen(void)
{
int	Adec(uchar *, uchar *, int, int);
	uchar	i;
	char	c[32];

	for(i=0; i<4; ++i)
	{
		Adec(&LcdBuffer[20*i+19],&auxLcdBuffer[20*i+19],0,20);
		Adec(&LcdExt[8*i+7],&auxExtBuffer[8*i+7],0,8);
	}

	auxLcdBuffer[80]=LcdBuffer[80];
	auxLcdBuffer[81]=LcdBuffer[81];
	SetTask(RefreshScreen,500,PIT_task);
	auxLcdFlag=1;
}
/*--------------------------------------------------------*/
/* called when key is pressed */
void ButtonPress(int nNum)
{
	 Key=nNum;
}
/* get display data */
void GetDisplay(char *pBuff)
{
	signed char i,j;
	i=(signed char)LcdBuffer[80]/20;
	j=(signed char)LcdBuffer[80]%20;


	pBuff[125]=28*i+j+8;
	pBuff[126]=auxLcdBuffer[81];

	for(i=0; i<4;++i)
	{
		memcpy(&pBuff[i*28],&auxExtBuffer[i*8],8);
		memcpy(&pBuff[i*28+8],&auxLcdBuffer[i*20],20);
	}
}