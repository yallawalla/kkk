#include    "remmenu.h"
#include    "io332.h"

#define		mess_rpt		2
#define     REP_CNT         3		/* stevilo ponovitev */
#define     DATALEN         128		/* dolzina podatkov v wordih */
#define     BEEPLEN         300		/* dolzina beepa v ms */
#define     BUF_NR          15		/* dolzina krozne vrste */
#define     DSP_WAIT        999		/* cakanje da lahko zacnem oddajati*/
									/* potem, ko je nekdo drug koncal v ms*/
#define		PROCESTIME     (300/20) /* cas za procesiranje bloka v ms */
unsigned char dsprog[] = {
#include    "modemprg.inc"      	/* izberi pravi file glede na hitrost DSP*/
};

/* okno za scrambler
unsigned int scramwin[] = {
#include    "scramwin.dat"
};


int mxT[]={
		   1, 3,
		  };

int mxTi[]={
		   1,
		  };
*/
#define   MAXMODE       1

volatile struct  block {
	volatile struct block	*naslednji;
	int             rptcnt;         /* stevec stevila ponovitev */
	unsigned int    izvor;
	unsigned int    ponor;
	unsigned int    control;        /* kontrolno polje 			*/
	unsigned int    dolzina;        /* dolzina informacije 		*/
	unsigned int    data[DATALEN];
} *paketinG, *paketinR, *paketoutG, *paketoutR, *potrdi;

volatile struct postaje {
	volatile struct postaje  *naslednji;
    unsigned int    id;
    unsigned int    last_block;
}  *ostali;

volatile unsigned int paketnr;                     /* zaporedna stevilka paketa */
unsigned int moj_naslov;
unsigned int preamblen;

unsigned int local_vol=0x0000;
unsigned int line_vol=0x0000;
volatile int timer, tflag, errfl;           /* timer, timer flag, error flag */
int     dsperr;                             /* ce predolgo cakas na odziv dsp */
unsigned int last_blk;
unsigned int getdsp(void);
void    putdsp(unsigned int i);
_IH     void recdsp(void);
void    senddsp(void);
void    dsp_irq_on(void);
void    dsp_irq_off(void);
void    SetVect(void(*f)(),int n);
int     dspicr(void);
int     dspisr(void);
int     dsprxh(void);
void    DspVolSet(void);
void    beeep(int, int);

int errf(void)
{
    return(errfl);
}
/***********************************************************************/
/*      inicializacija DSP                                             */
/***********************************************************************/
int dspinit(unsigned int addr)
{
    int i;

    i=0;
	dsperr=0;
	while (i<=sizeof(dsprog)) {			/* prepisi program v DSP */
		DSPTXH=dsprog[i++];
		DSPTXL=dsprog[i++];
    }
    DSPICR=( dspicr() | DSP_F_HF0 );	/* postavi HF0 na 1 */
	SetVect(recdsp,DSPIRQ);				/* nastavitev vektorja za dsp_irq !!!!*/
    PFPAR|=0x40;

    /* vpisi okno za scrambler */
/*    i=0;
    while (i<=(sizeof(scramwin)/sizeof(unsigned int))-1) {
        putdsp(scramwin[i++]);
    }
*/
	/* alociraj bufferje */
	paketinG=paketinR=memalloc(sizeof(struct block));
	paketoutG=paketoutR=memalloc(sizeof(struct block));
    for(i=0;i<BUF_NR;i++)
    {
		paketinR->naslednji=memalloc(sizeof(struct block));
		paketoutR->naslednji=memalloc(sizeof(struct block));
		paketinR=paketinR->naslednji;
		paketoutR=paketoutR->naslednji;
	}
	paketinR->naslednji=paketinG;               /* zakljuci krozno vrsto */
	paketoutR->naslednji=paketoutG;             /* zakljuci krozno vrsto */
	paketinR=paketinG;
	paketoutR=paketoutG;

	potrdi=NULL;
	ostali=NULL;
	paketnr=0;

	DspAddr(addr);
	if  (dsperr==1) return(1);

	DspVolSet();

	SetTask(senddsp,1,PIT_task);          /* nastavitev vektorja za periodic irq !!!!*/
	dsp_irq_on();                         /* in omogoci irq ob sprejemu */
    return(0);
}
/***************************************************************/
int sendblk(unsigned int addr, char *c, int n, unsigned int type)
{
    int i;

    preamblen=abs(key_delay)/16;
	if (moj_naslov==addr)
		{
        if(paketinR->naslednji==paketinG) return(-1);
		if(n>DATALEN*2) return(-1);
		paketinR->rptcnt=mess_rpt;
		paketinR->izvor=moj_naslov;
        paketinR->ponor=addr;
        paketinR->dolzina=(n+1)/2;
        for(i=0;i<(n+1)/2;i++)
            paketinR->data[i]= ((*c++)<<8)+(*c++);
        paketnr=(++paketnr)%1024;
		if (paketnr==0) ++paketnr;
        paketinR->control=paketnr|(type^CTRL_F_LOC) ;
        paketinR=paketinR->naslednji;
		errfl=0;
        }
    else
        {
        if(paketoutR->naslednji==paketoutG) return(-1);
		if(n>DATALEN*2) return(-1);
		paketoutR->rptcnt=mess_rpt;
		paketoutR->izvor=moj_naslov;
        paketoutR->ponor=addr;
        paketoutR->dolzina=(n+1)/2;
        for(i=0;i<(n+1)/2;i++)
            paketoutR->data[i]= ((*c++)<<8)+(*c++);
        paketnr=(++paketnr)%1024;
        if (paketnr==0) ++paketnr;
        paketoutR->control=paketnr|(type^CTRL_F_LOC) ;
        paketoutR=paketoutR->naslednji;
        }
    return(paketnr);
}
/**************************************************************/
int recblk(unsigned int *addr, char *c)
{
	int i,n;
	volatile struct postaje *st;

	if (paketinG==paketinR) return(0);

	/*  ce ze sprejeti blok ga zavrzi */
	st=ostali;
	while(st!=NULL)
	{
		if(st->id==paketinG->izvor)
            if (st->last_block==paketinG->control)
            {                                   /* ce blok ze sprejet */
				paketinG=paketinG->naslednji;   /* ga preskoci */
                return(0);
            }
            else break;
        else st=st->naslednji;
    }
    if (st==NULL)
    {
		st=memalloc(sizeof(struct postaje));
        st->naslednji=ostali;               /* novi blokec na zacetek vrste */
        ostali=st;
    }
    st->last_block=paketinG->control;       /* zapomni si zadnji sprejeti blok */
/**********************************************************************/
    n=2*paketinG->dolzina;

    for(i=0;i<paketinG->dolzina;i++)        /* prepisi podatke v string */
	{
		*c++=paketinG->data[i]/256;
		*c++=paketinG->data[i]%256;
	}
	*addr=paketinG->izvor;
	paketinG=paketinG->naslednji;
	return(n);
}
/**********************************************************************/
/**********************************************************************/
/*      Interruptni nivo:   sprejem tece na svojem vektorju,          */
/*                          oddaja pa na 50 Hz ticku                  */
/**********************************************************************/
/**********************************************************************/
/*  prebere blok iz dsp */
_IH void recdsp(void)
{
	int i;
	unsigned int c,d,stat;
	volatile struct block *paket;
	char s[32];

	dsp_irq_off();
	stat=getdsp();
	switch(stat)
	{
		case BLOCK_R:     					/* block received */
			getdsp();                  		/* statusni register */
			getdsp();                  		/* dolzina celotnega bloka (drop)*/
			if(paketinR->naslednji==paketinG)
			{               /* ce ni prostora gre vse skupaj v odtok */
				getdsp();                   /* naslov izvira */
				getdsp();                   /* naslov ponora */
				c=getdsp();                 /* kontrolno polje */
				d=getdsp();                 /* dolzina podatkov */
				for(i=0;i<d;i++)
							getdsp();       /* podatki */
				dsp_irq_on();
				return;
			}
			paket=paketinR;                 /* alociraj prostor za naslednji blok */
			paket->izvor=getdsp();          /* naslov izvira */
			paket->ponor=getdsp();          /* naslov ponora */
			paket->control=getdsp();        /* kontrolno polje */
			paket->dolzina=getdsp();        /* dolzina podatkov */
			for(i=0;i<paket->dolzina;i++)
				paket->data[i]=getdsp();    /* podatki */
/*********************************************************************/
			dsp_irq_on();
			if (potrdi!=NULL)
			{
				if ((paket->control&CTRL_F_ACK)&&(paket->izvor==potrdi->ponor))
				{                           /* ce potrjevalni paket ga obdelaj */
					paketoutG=potrdi->naslednji;
					potrdi=NULL;
					errfl=0;
				}
			}
/*********************************************************************/
			/* nov blok v vrsto */
			paketinR=paket->naslednji;
			return;

		case BLOCK_E:   /* error on receive */
			c=getdsp();
			timer =0;
			dsp_irq_on();
			return;

		case BLOCK_F:   /* pojav flaga */
			timer=DATALEN*2;
			dsp_irq_on();
			return;

		case BLOCK_T:   /* block transmitted */
/* ce je bil oddani blok UNM ali ACK in ce cakalna vrsta ni prazna */
			if ((potrdi==NULL)&&(paketoutG!=paketoutR))
					putdsp(1);
				else
					putdsp(0);

			timer=abs(key_delay)/20+PROCESTIME;   /*15!! cakaj da se nekaj zgodi */
			SetTask(senddsp,1,PIT_task);
			dsp_irq_on();
			return;

		default:sprintf(s,"dsp error %04X",stat);
			clear_screen(NULL);
			xyLCD(0,0);
			puts_lcd(s);
			return;
	}
}

/**********************************************************************/
/*  vpise vsebino bloka v dsp                                         */

void senddsp(void)
{
	int i;
	unsigned int c;

	if ((paketoutG==paketoutR)&&(potrdi==NULL)) return;  /* ce ni bloka za oddajo */
	if (!(dspisr() & DSP_F_TXDE)) return;
	if (DSPCVR & DSP_F_HC) return;

	dsp_irq_off();

	if (potrdi!=NULL)
	{
		if (timer && tflag)			/* ce se stevec za potrditev se ni*/
		{
			--timer;				/* iztekel ga zmanjsaj */
			dsp_irq_on();
			return;					/* koncaj */
		}
		else                        /* ce se je ali se je ravnokar */
		{                           /* DA, oznaci, ponovi oddajo bloka */
				tflag=0;
				if(potrdi->rptcnt==0)/* ce stevec ponavljanj pri 0 javi napako */
				{
					errfl=-1;
/* tukaj pride, kar se mora dogoditi, ce po n ponovitvah se vedno ni odgovora */
					potrdi=NULL;
					paketoutG=paketoutG->naslednji;
					dsp_irq_on();
					return;
				}
				--(potrdi->rptcnt);             /* zmanjsaj stevec ponavljanj */
				potrdi=NULL;                    /* ne cakaj vec na potrditev */
		}                                       /* (tega bloka). S tem se  */
	}                                           /* povzroci ponovna oddaja */

	DSPCVR=SEND_BLOCK;                      /* komanda za oddjo bloka */
	c=getdsp();
	if ((c&STAT_F_TXb) || (c&STAT_F_RCVing) || (c&STAT_F_dlytx))
	{
			dsp_irq_on();
			return;                          /* ce DSP zavrne oddajo bloka */
	}

	putdsp(preamblen);                       /* dolzina preambla */
	putdsp(4+paketoutG->dolzina);            /* dolzina celotnega bloka */
	putdsp(paketoutG->izvor);                /* naslov izvira */
	putdsp(paketoutG->ponor);                /* naslov ponora */
	putdsp(paketoutG->control);              /* kontrolno polje */
	putdsp(paketoutG->dolzina);              /* dolzina podatkov */
	for(i=0;i<paketoutG->dolzina;i++)
	{
		putdsp(paketoutG->data[i]);      /* podatki */
	}
	if((paketoutG->control&CTRL_F_ACK) || (paketoutG->control&CTRL_F_UNM))
	{                                        /* ce acknowledge ali unnumbered */
			paketoutG=paketoutG->naslednji;
			potrdi=NULL;
			timer=0;
			tflag=0;
			errfl=0;

	}
	else
	{
		errfl=0;
		tflag=1;
		timer=500;
		potrdi=paketoutG;
	}
	SetTask(senddsp,250,PIT_task);
	dsp_irq_on();
	return;
}
/**********************************************************************/
int SendStat(int stevilka_paketa)
{
	volatile struct block *paket;

	for (paket=paketoutG;paket!=paketoutR;paket=paket->naslednji)
		if ((paket->control)%1024==stevilka_paketa) return(stevilka_paketa);

	return(errfl);
}

/**********************************************************************/
/*  vpisi znak v DSP                                                  */
/**********************************************************************/
void putdsp(unsigned int i)
{
	int j;

	dsperr=j=0;
	while (((dspisr() & DSP_F_TXDE)==0) && j<10000) ++j;
	if  (j==10000)
	{
		dsperr=1;
		return;
	}
	DSPTXH = (unsigned char)(i>>8);
	DSPTXL = (unsigned char)(i%256);
}
/**********************************************************************/
/*  izpisi znak iz DSP                                                */
/**********************************************************************/
unsigned int getdsp(void)
{
	unsigned int i;
	long j;

	dsperr=j=0;
	while (((dspisr() & DSP_F_RXDF)==0)&& j<1000000L) ++j;
	if  (j==1000000L)
	{
		dsperr=1;
		return(-1);
	}

	i=dsprxh();
	return(i * 256 + DSPRXL);
}
/**********************************************************************/
/*  omogocanje in onemogocanje dsp irq                                */
/**********************************************************************/
void dsp_irq_off(void)
{
    DSPICR=dspicr() & (~DSP_F_RREQ);
}

void dsp_irq_on(void)
{
    DSPICR=dspicr() | DSP_F_RREQ;
}
/**********************************************************/
int dspicr(void)
{
    return(DSPICRi);
}

int dspisr(void)
{
    return(DSPISRi);
}

int dsprxh(void)
{
    return(DSPRXHi);
}
/*********************************************************/
void DspAddr(unsigned int a)
{
	moj_naslov=a;
	DSPCVR=SET_ADDR;               /* nastavitev naslova */
	putdsp(a);
	putdsp(DSP_WAIT);
}
/*********************************************************/
void beep(int len)
{
	beeep(len,3);
}

void beeep(int lenght,int tone)
{
	if (DSPCVR & DSP_F_HC) return;
    DSPCVR=BEEP;
    putdsp(10*lenght);
    tone=(tone%7)+1;
    putdsp(tone);
}
/**********************************************************/
/*      nastavitev jakosti na vhodih in izhodih           */
void DspVolSet(void)
{
	while (DSPCVR & DSP_F_HC)
		watchdog();
    DSPCVR=VOLUME;                          /* jakosti */
    putdsp(local_vol);
    putdsp(line_vol);
}
/**********************************************************/
/*      nastavljanje ojacanj                              */
int DspGain(enum tip_kanala kanal, int g)
{
    int x;

    if  ( g > 1 ) g=1;
    if  ( g < -1 ) g=-1;

    switch(kanal)
    {
     case MIC:      x=local_vol >> 11;
                    if ((x==0)&&(g!=1)) break;
                    if ((x==3)&&(g!=-1)) break;
                    x+=g;
                    local_vol = (local_vol | 0x1800) & ( (x<<11) | 0xf);
                    break;

     case HEAD:     x=local_vol & 0xf;
                    switch(x)
                    {
                        case 0:     if (g==-1) break;
                                    x+=g;                       
                                    break;                      
                        case 7:     if (g==1) break;
                                    x+=g;                       
                                    break;                      
                        default:    x+=g;
                    }
                    local_vol = (local_vol | 0xf) & ( x | 0x1800);
                    break;

     case LINE_IN:  x=line_vol >> 11;
                    if ((x==0)&&(g!=1)) break;
                    if ((x==3)&&(g!=-1)) break;
                    x+=g;                                       
                    line_vol = (line_vol | 0x1800) & ( (x<<11) | 0xf);
                    break;

     case LINE_OUT: x=line_vol & 0xf;
                    switch(x)
                    {
                        case 0:     if (g==-1) break;
                                    x+=g;                       
                                    break;                      
                        case 7:     if (g==1) break;
                                    x+=g;                       
                                    break;                      
                        default:    x+=g;
                    }
                    line_vol = (line_vol | 0xf) & ( x | 0x1800);
                    break;

     default:       break;
    }
    DspVolSet();
    return(x);
}
/********************************************************************/
int DspStat(void)
{
	while (DSPCVR & DSP_F_HC)
		watchdog();
	DSPCVR=GET_STATUS;
	return(getdsp());
}
/********************************************************************/
/* nastavitev scramblerja                                           
void DspScrSet(int smode)
{
	if (smode>=MAXMODE) smode=0;
    DSPCVR=SCRAMB;
    putdsp(mxT[smode]);
    putdsp(mxTi[smode]);
    return;
}
*/