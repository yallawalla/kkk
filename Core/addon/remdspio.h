/* definicije naslovov */
#define DSPBASE         0x201000L
#define DSPICR          (*(volatile unsigned char *)(DSPBASE))
#define DSPICRi         (*(volatile unsigned int *)(DSPBASE))
#define DSPCVR          (*(volatile unsigned char *)(DSPBASE+1L))
#define DSPISR          (*(volatile unsigned char *)(DSPBASE+2L))
#define DSPISRi         (*(volatile unsigned int *)(DSPBASE+2L))
#define DSPIVR          (*(volatile unsigned char *)(DSPBASE+3L))
#define DSPRXH          (*(volatile unsigned char *)(DSPBASE+6L))
#define DSPRXHi         (*(volatile unsigned int *)(DSPBASE+6L))
#define DSPRXL          (*(volatile unsigned char *)(DSPBASE+7L))
#define DSPTXH          (*(volatile unsigned char *)(DSPBASE+6L))
#define DSPTXL          (*(volatile unsigned char *)(DSPBASE+7L))

#define DSPIRQ          30

/* flagi */
/* DSP_ICR */
#define DSP_F_RREQ      1
#define DSP_F_TREQ      2
#define DSP_F_HF0       8
#define DSP_F_HF1       0x10
#define DSP_F_HM0       0x20
#define DSP_F_INIT      0x80
#define DSP_F_HM1       0x40
/* DSP_CVR */
#define DSP_F_HC        0x80
/* DSP_ISR */
#define DSP_F_RXDF      1
#define DSP_F_TXDE      2
#define DSP_F_TRDY      4
#define DSP_F_HF2       8
#define DSP_F_HF3       0x10
#define DSP_F_DMA       0x40
#define DSP_F_HREQ      0x80

/*  pomen flagov v status registru */

#define STAT_F_RXf      1           /* 1 => znak sprejet */
#define STAT_F_TXf      2           /* 1 => znak pripravljen za oddajo */
#define STAT_F_stuf0    4           /* 1 => 0 bit stuffing ON (za znak v txreg) */
#define STAT_F_shstuf0  8           /* 1 => 0 bit stuffing ON (za znak v txshreg) */
#define STAT_F_xmit     0x10        /* 1 => oddaja, 0=> sprejem */
#define STAT_F_CDflg    0x20        /* 1 => carrier detect */
#define STAT_F_frmerr   0x40        /* 1 => frame err ( vec kot 6 enic v vrsti ) */
#define STAT_F_flagg    0x80        /* 1 => FLAG pattern ($7E) received */
#define STAT_F_RXb      0x100       /* 1 => blok sprejet */
#define STAT_F_TXb      0x200       /* 1 => blok pripravljen za oddajo */
#define STAT_F_TASK     0x400       /* 1 => tece task 1, 0=> tece task 0 */
#define STAT_F_RCVing   0x800       /* 1 => v procesu sprejemanja bloka */
#define STAT_F_tsmit    0x1000      /* 1 => rele na oddaji */
#define STAT_F_sync_f   0x2000      /* 1 => v rxshreg zaznan SYNC ali njegova negacija */
#define STAT_F_dlytx    0x4000      /* 1 => oddajnik mora se pocakati */

#define BLOCK_R         1
#define BLOCK_E         2
#define BLOCK_F         4
#define BLOCK_T         8

#define CTRL_F_NMB      0           /* information(0) */
#define CTRL_F_LOC      0x2000      /* lokalni blok (odda se samo lokalno)*/
									/* ravno obratno kot pri asm programu */
#define CTRL_F_UNM      0x4000      /* unnumbered block (ne zahteva potrditev)*/
#define CTRL_F_ACK      0x8000      /* information(0)/acknowledge block(1) */

#define BEEP            0x9a        /* beep */
#define SCRAMB          0x9b        /* nastavitev scramblerja */
#define VOLUME          0x9c        /* nastavitev ojacanj */
#define GET_STATUS      0x9d        /* preberi statusno besedo v DSP */
#define SET_ADDR        0x9e        /* DSP vektor za nastavitev naslova */
#define SEND_BLOCK      0x9f        /* DSP vektor za oddajo bloka */

enum tip_kanala {MIC, HEAD, LINE_IN, LINE_OUT};

int dspinit(unsigned int addr);
void beep(int);
void beeep(int, int);
void volume_up(void);
void volume_dn(void);
int sendblk(unsigned int addr, char *c, int n, unsigned int type);
int recblk(unsigned int *addr, char *p);
void DspAddr(unsigned int);
int DspGain(int, int);
int SendStat(int);
int DspStat(void);
void DspScrSet(int);


int errf(void);           			/* error flag */
