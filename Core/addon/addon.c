#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery_lcd.h"
#include	"remmenu.h"

void	*ralloc(size_t t)	{
			return malloc(t);
}

void	rfree(void *p) {
			free(p);
}

int		Getch(int ch) {
			switch(ch) {
				case CHLCD:
					return GetKey();
				default:
					return 0;
			}
}
int		Kbhit(int ch) { return 0; }

int		Putch(int data, int ch) {
			switch(ch) {
				case CHLCD:
					put_lcd (data);
					break;
				default:
					return 0;
			}
			return data;
}

void	watchdog(void)	{}
void	PIT_disable(void)	{}
void	PIT_enable(void)	{}
int		BatteryOK(void)	{return EOF;}

void	BacklitOff(void)	{}
void	BacklitOn(void)	{}
void	HeatOff(void)	{}
void	HeatOn(void)	{}
void	C100_off(void)	{}
void	C100_on(void)	{}
int		GPS_off(void) { return EOF; }
int		GPS_on(void) { return EOF; }

void	RefreshScreen(void)	{}       
void	Shutdown(void)	{}    
double getT(void) {return 24.5;}	
uint	GetPeriod(void) {return 1;}	
	
void	puts_pc(char *p)		{};
void	puts_rld(char *p)		{};
void	puts_aux(char *p)		{};
void	puts_C100(char *p)	{};
void	puts_gps(char *p)		{};
int		Baud(uint i,uint j)	{return i;};
int		Xonoff(uint i)			{return i;};
int		LcdLevel(int i)			{return i;};
	
uint8_t	rtc[256];

void	__SystemInit(void) {
		library=(char *)0x8080000;
		fonts=(char *)0x80a0000;
		syspar=(char *)0x80c0000;

		PIT_task=	SetTask(NULL,null,PIT_task);
		RTC_task=	SetTask(NULL,null,RTC_task);
		watchdog();
}
		
int		writeRTC(int a,int d) {
			rtc[a]=d;
			return rtc[a];
}

int		readRTC(int a) {
int		i=rtc[a];
			if(a==0x30)
				rtc[a]=0;
			return i;
}

int		readTEMP(int a, int b) {
			return(a);
}

void	InProg(void){}
void	beep(int t) {}

void	*prog_byte(void *v,uchar *p,size_t n) {return v;}
void	*prog_vect(void *v,uchar *p,size_t n) {return v;}

void	*load_byte(void *dst, uchar *src, size_t n) {
			uint8_t	*to=dst;
			while(n--)
				*to++ = *src++;
			return(src);
}

void	sector_erase(uchar *p, int r) {}
	
int		sendblk(unsigned int addr, char *c, int n, unsigned int type) {
			return(1);
}

int		recblk(unsigned int *a, char *p) {
			return(null);
}

int 	dspinit(unsigned int addr) {
			return NULL;
}

void	DspAddr(unsigned int k) {};
	
int		DspGain(int i, int j) {
			return j;
}

void	RLD_Remote(void)	{}
void	RLD_Noecho(void)	{}
void	RLD_StartHV(void)	{}
void	RLD_Trigger(void)	{}
void	RLD_GetStatus(void)	{}
void	RLD_GetTarget(uint32_t i)	{}
void	RLD_ShowTarget(uint32_t i)	{}
	
void	shaft_ITEK16uS(void) {}
void	shaft_ROC415(void) {}
void	shaft_ROC415G(void) {}
