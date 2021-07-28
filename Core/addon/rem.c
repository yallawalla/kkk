#include "remmenu.h"

#ifdef	WIN32
#include	<share.h>
#endif

#define	XON         0x11        /* ctrlQ								*/
#define XOFF        0x13        /* ctrlS                            	*/
#define BLKSTRT1	'*'			/* marks beginning of the rld block		*/
#define BLKEND		4			/* marks end of the (rcv)block			*/
#define BLKSTRT2	'$'			/* marks beginning of the TCM2 block	*/
#define BLKSTRT3	0xCA		/* marks beginning of the SOPHIE block	*/
#define BLKEND3		0xF1		/* marks end of the (SOPHIE rcv)block			*/

#define RXpc		0x3FFF
#define TXpc		0x3FFF
#define RXgps		0x3FF
#define TXgps		0x3FF
#define RXaux		0x3FF
#define TXaux		0x3FF
#define RXrld		0x3F
#define TXrld		0x3F
#define RXc100		0xFF
#define TXc100		0xFF
#define RXir		0xFF
#define TXir		0xFF

typedef volatile unsigned char	portb;
typedef volatile unsigned int	portw;
typedef volatile unsigned char	vchar;
typedef volatile unsigned int	vint;

typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned long	ulong;

#ifndef	WIN32
#include "332defs.h"
#define	RAMSTART	0xF00000 /* 1st RAM address */
#define	RAMSIZE		2048

#include "tpuRAMld.c"		 /* TPU RS232 comm emulation and more */

/* interrupt vector numbers */

#define SERIALINT	0x44	/* QSM SCI interrupt vector number */
#define QSPI_INT	SERIALINT+1 /* QSM QSPI interrupt vector number */
#define TPUINT		0x50	/* MS-nibble of all 16 TPU interrupts' numbers */

/* function prototypes */

void Initialize ();
void InitTPUchannels ();

_IH void sciint ();
_IH void tpuint_rld ();
_IH void tpuint_aux ();
_IH void tpuint_C100 ();
_IH void tpuint_gps ();
_IH void tpuint_ir ();
_IH void tpuint ();
_IH void PowerFail ();
_IH void OnePulsePerSecond ();

_ASM void disable_interrupts ()
{
	or	#$0700,sr
}

_ASM void enable_interrupts ()
{
	and	#$F8FF,sr
}
#endif

/*** global variables ***/

/* RX/TX buffers for SCI serial I/O						*/
vchar *rxbuff;
vchar *txbuff;
vint rxstart = 0, rxend = 0;	/* rx buffer pointers	*/
vint txstart = 0, txend = 0;	/* tx buffer pointers	*/
vint rxcount = 0;
vint xofrx = 0;
vint xoftx = 0;
vint xmode = 0;
/* defs for TPU Channels 0-2, Tx and Rx  RLDE			*/
vchar *rxbuff2;
vchar *txbuff0;
vint rxstart2 = 0, rxend2 = 0;	/* rx buffer pointers	*/
vint txstart0 = 0, txend0 = 0;	/* tx buffer pointers 	*/

/* defs for TPU Channels 1-3, Tx and Rx  AUX			*/
vchar *rxbuff3;
vchar *txbuff1;
vint rxstart3 = 0, rxend3 = 0;	/* rx buffer pointers	*/
vint txstart1 = 0, txend1 = 0;	/* tx buffer pointers	*/

/* defs for TPU Channels 7-6, Tx and Rx  C100 Compass	*/
vchar *rxbuff6;
vchar *txbuff7;
vint rxstart6 = 0, rxend6 = 0; /* rx buffer pointers	*/
vint txstart7 = 0, txend7 = 0;	/* tx buffer pointers	*/

/* defs for TPU Channels 9-8, Tx and Rx   GPS			*/
vchar *rxbuff8;
vchar *txbuff9;
vint rxstart8 = 0, rxend8 = 0;	/* rx buffer pointers	*/
vint txstart9 = 0, txend9 = 0;	/* tx buffer pointers	*/

/* defs for TPU Channels 14-15, Tx and Rx   IR			*/
vchar *rxbuff15;
vchar *txbuff14;
vint rxstart15 = 0, rxend15 = 0;	/* rx buffer pointers	*/
vint txstart14 = 0, txend14 = 0;	/* tx buffer pointers	*/

/*volatile uint time = 0, tmt;
*/
#include "gps332.c"
/*----------------------------------------------------------*/
void Initialize ()
{
	disable_interrupts();

	rxbuff15=memalloc(RXir);
	txbuff14=memalloc(TXir);
	rxbuff8=memalloc(RXgps);
	txbuff9=memalloc(TXgps);
	rxbuff6=memalloc(RXc100);
	txbuff7=memalloc(TXc100);
	rxbuff3=memalloc(RXaux);
	txbuff1=memalloc(TXaux);
	rxbuff2=memalloc(RXrld);
	txbuff0=memalloc(TXrld);
	rxbuff=memalloc(RXpc);
	txbuff=memalloc(TXpc);
	gps.status = gps.error = 0;

#ifndef WIN32

	SetVect (sciint, SERIALINT);
	QMCR	= 0x0083;		/* SUPV, IARB = 3 */
	QILR	= 3;			/* QSM Interrupt Level 3 */
	QIVR	= SERIALINT;	/* SCI/QSPI Vector # */
	SCCR0	= 55;			/* 9600Bd @ 16.8MHz */
	SCCR1	= 0x002C;		/* enable SCI and its receiver interrupt */

	RAMBAR	= (uint)((ulong)RAMSTART >> 8);	/* RAM Base Address */
	ClearRAM ();
	LoadEmulationRAM (tpu_microcode);		/* module tpuramld.c */
	LoadEmulationRAM (tpu_entrypoints);

	TMCR	= 0x04C5;			/* TPU and RAM in emulation mode, IARB=5 */
	TICR	= 0x0500 | TPUINT;	/* set INT level 5 & base vector # */

	SetVect (tpuint_rld,	TPUINT + 0);
	SetVect (tpuint_aux,	TPUINT + 1);
	SetVect (tpuint_rld,	TPUINT + 2);
	SetVect (tpuint_aux,	TPUINT + 3);
	SetVect (tpuint,		TPUINT + 4);
	SetVect (tpuint,		TPUINT + 5);
	SetVect (tpuint_C100,	TPUINT + 6);
	SetVect (tpuint_C100,	TPUINT + 7);
	SetVect (tpuint_gps,	TPUINT + 8);
	SetVect (tpuint_gps,	TPUINT + 9);
	SetVect (tpuint,		TPUINT + 10);
	SetVect (tpuint,		TPUINT + 11);
	SetVect (tpuint,		TPUINT + 12);
	SetVect (tpuint,		TPUINT + 13);
	SetVect (tpuint_ir,		TPUINT + 14);
	SetVect (tpuint_ir,		TPUINT + 15);
#endif
}
/*----------------------------------------------------------*/
#ifndef	WIN32
void InitTPUchannels ()
{
/*			RxD  TxD
	RLDE     2    0
	AUX      3    1
	C100     6    7
	GPS      8    9
	IR      15   14
*/
	CPR0 = CPR1 = 0;		/* Priorities = 0 => Stop TPU channels */
	CIER = CISR = 0;
	HSRR0 = HSRR1 = 0;
	CFSR0 = 0xEF00;			/* Ch15 RX, CH14 TX, CH13 NOP, CH12 NOP */
	CFSR1 = 0xBCFE;			/* Ch11 ITC, Ch10 PWM, Ch9 TX , Ch8 RX  */
	CFSR2 = 0xFEDD;			/* Ch7  TX , Ch6  RX , Ch5 DIO, Ch4 DIO */
	CFSR3 = 0xEEFF;			/* Ch3  RX , Ch2  RX , Ch1 TX , Ch0 TX  */

	/* TPU Channel 0 - RLDE TxD */

	HSRR1 |= 0x0003;		/* Host Service Request for Send */
	TPRAM0(0) = 0x0080;		/* output (T2-19) */
	TPRAM0(2) = 440;		/* 9600Bd */
	TPRAM0(4) = 0;			/* Tx data */
	TPRAM0(6) = 8;			/* 8 data bits */
	TPRAM0(8) = 0;			/* actual bit count */
	TPRAM0(10) = 0;			/* Tx shift register */

	/* TPU Channel 2 - RLDE RxD */

	HSRR1 |= 0x0030;		/* Host Service Request for Receive */
	TPRAM2(0) = 0;			/* input (T2-19) */
	TPRAM2(2) = 440;		/* 9600Bd */
	TPRAM2(4) = 0;			/* Rx data */
	TPRAM2(6) = 8;			/* 8 data bits */
	TPRAM2(8) = 0;			/* actual bit count */
	TPRAM2(10) = 0;			/* Rx shift register */

	CIER |= 0x0004;			/* enable Rx interrupt from Ch2 */

	/* TPU Channel 1 - AUX TxD */

	HSRR1 |= 0x000C;		/* Host Service Request for Send */
	TPRAM1(0) = 0x0080;		/* output (T2-19) */
	TPRAM1(2) = 440;			/* 9600Bd */
	TPRAM1(4) = 0;			/* Tx data */
	TPRAM1(6) = 8;			/* 8 data bits */
	TPRAM1(8) = 0;			/* actual bit count */
	TPRAM1(10) = 0;			/* Tx shift register */

	/* TPU Channel 3 - AUX RxD */

	HSRR1 |= 0x00C0;		/* Host Service Request for Receive */
	TPRAM3(0) = 0;			/* input (T2-19) */
	TPRAM3(2) = 440;		/* 9600Bd */
	TPRAM3(4) = 0;			/* Rx data */
	TPRAM3(6) = 8;			/* 8 data bits */
	TPRAM3(8) = 0;			/* actual bit count */
	TPRAM3(10) = 0;			/* Rx shift register */

	CIER |= 0x0008;			/* enable Rx interrupt from Ch3 */

	/* TPU Channel 4 - Discrete Input */

	HSQR1 |= 0x0200;		/* sequence field '10' to update pin on init */
	HSRR1 |= 0x0300;		/* initiate with host service request '11' */
	TPRAM4(0) = 0x000F;		/* input (T2-19) */

	/* TPU Channel 5 - Discrete Output */

	HSRR1 |= 0x0C00;		/* initiate with host service request '11' */
	TPRAM5(0) = 0x008F;		/* output (T2-19) */

	/* TPU Channel 7 - C100 TxD */

	HSRR1 |= 0xC000;		/* Host Service Request for Send */
	TPRAM7(0) = 0x0080;		/* output (T2-19) */
	TPRAM7(2) = 440;		/* 9600Bd */
	TPRAM7(4) = 0;			/* Tx data */
	TPRAM7(6) = 8;			/* 8 data bits */
	TPRAM7(8) = 0;			/* actual bit count */
	TPRAM7(10) = 0;			/* Tx shift register */

	/* TPU Channel 6 - C100 RxD */

	HSRR1 |= 0x3000;		/* Host Service Request for Receive */
	TPRAM6(0) = 0;			/* input (T2-19) */
	TPRAM6(2) = 440;		/* 9600Bd */
	TPRAM6(4) = 0;			/* Rx data */
	TPRAM6(6) = 8;			/* 8 data bits */
	TPRAM6(8) = 0;			/* actual bit count */
	TPRAM6(10) = 0;			/* Rx shift register */

	CIER |= 0x0040;			/* enable Rx interrupt from Ch6 */

	/* TPU Channel 9 - GPS TxD */

	HSRR0 |= 0x000C;		/* Host Service Request for Send */
	TPRAM9(0) = 0x0080;		/* output (T2-19) */
	TPRAM9(2) = 440;		/* 9600Bd */
	TPRAM9(4) = 0;			/* Tx data */
	TPRAM9(6) = 9;			/* 8 data bits + parity */
	TPRAM9(8) = 0;			/* actual bit count */
	TPRAM9(10) = 0;			/* Tx shift register */

	/* TPU Channel 8 - GPS RxD */

	HSRR0 |= 0x0003;		/* Host Service Request for Receive */
	TPRAM8(0) = 0;			/* input (T2-19) */
	TPRAM8(2) = 440;		/* 9600Bd */
	TPRAM8(4) = 0;			/* Rx data */
	TPRAM8(6) = 9;			/* 8 data bits + parity */
	TPRAM8(8) = 0;			/* actual bit count */
	TPRAM8(10) = 0;			/* Rx shift register */

	CIER |= 0x0100;			/* enable Rx interrupt from Ch8 */

	/* TPU Channel 14 - IR TxD */

	HSRR0 |= 0x3000;		/* Host Service Request for Send */
	TPRAM14(0) = 0x0080;	/* output (T2-19) */
	TPRAM14(2) = 220;		/* 19200Bd */
	TPRAM14(4) = 0;			/* Tx data */
	TPRAM14(6) = 9;			/* 8 data bits + parity */
	TPRAM14(8) = 0;			/* actual bit count */
	TPRAM14(10) = 0;		/* Tx shift register */

	/* TPU Channel 15 - IR RxD */

	HSRR0 |= 0xC000;		/* Host Service Request for Receive */
	TPRAM15(0) = 0;			/* input (T2-19) */
	TPRAM15(2) = 220;		/* 19200Bd */
	TPRAM15(4) = 0;			/* Rx data */
	TPRAM15(6) = 9;			/* 8 data bits + parity */
	TPRAM15(8) = 0;			/* actual bit count */
	TPRAM15(10) = 0;		/* Rx shift register */

	CIER |= 0x8000;			/* enable Rx interrupt from Ch15 */

	/* TPU Channel 10 - Square Wave for generating LCD's VEE */

	HSRR0 |= 0x0020;		/* Host Service Request for Initialization */
	TPRAM10(0) = 0x0080;	/* Ch. control: output (T3-36) 	*/
#ifdef	GRAPHIC_LCD
	TPRAM10(4) = 75;		/* jeba z graf. lcd !!!			*/
#else
	TPRAM10(4) = 50;		/* PWMHI cca 30% duty cycle 	*/
#endif
	TPRAM10(6) = 0x0068;	/* PWMPER = 32768*512/32 / 5kHz */

	/* TPU Channel 11 - ITC Frequency Measurement */

	HSRR0 |= 0x0040;		/* Host Service Request for Initialization */
	HSQR0 |= 0x0040;		/* sequence field '01': continual, no links */
	TPRAM11(0) = 0x0004;	/* Ch_control: detect rising edge (T3-15) */
	TPRAM11(2) = 0x000E;	/* no links */
	TPRAM11(4) = 0x0002;	/* MAX_COUNT = 2 edges */

	/* setting priority != 0 , enables channels		*/
	/* MUST be last step in TPU channels init !		*/
	/* Channels 4, 5, 10 have low priority			*/
	/* Channels 12, 13 disabled						*/

	CPR0 = 0xF0DF;			/* channels 15-8 */
	CPR1 = 0xF5FF;			/* channels  7-0 */

	HSQR1 &= ~0x0300;		/* Ch4: sequence field '00' to update pin-level
						   	   bit on transition */
}
/*----------------------------------------------------------*/
_IH void sciint ()
	{
	if (SCSR & RDRF)				/* new data ? */
		{							/* RECEIVE */
        int i = RDR;
        if(xmode) {
			if(i == XOFF) {
   				xofrx = 1;
				SCCR1 &= ~0x0080;	/* transmit interrupt disable */
        		return;
            	}
       		if(i == XON) {
	   			xofrx = 0;
				SCCR1 |= 0x0080;	/* transmit interrupt enable */
        	    return;
            	}
			}
		rxbuff[rxend++] = i;		/* put in buffer */
		rxend &= RXpc;				/* cyclic buffer */
   		if(++rxcount > RXpc*3/4)
        	if(xmode)
				SCCR1 |= 0x0080;		/* transmit interrupt enable */
		}
/* oddajni del	*/
	if (SCSR & TDRE)					/* TDR empty ? */
		{
			if(xmode)
			{
    			if((rxcount > RXpc*3/4) && !xoftx)
				{
	 				TDR = XOFF;
					xoftx = 1;
					return;
				}

				if(!rxcount && xoftx)
				{
					TDR = XON;
					xoftx = 0;
					return;
				}

				if(xofrx)
				{
					SCCR1 &= ~0x0080;	/* transmit interrupt disable */
					return;
				}
			}
		if (txstart == txend)
			SCCR1 &= ~0x0080;			/* transmit interrupt disable */
		else {
			txstart++;
			txstart &= TXpc;			/* cyclic buffer */
			TDR = txbuff[txstart];		/* TRANSMIT */
			}
		}
	}
/*----------------------------------------------------------*/
_IH void tpuint_rld ()	/* ISR for RLD-E serial I/O */
{						/* TP0 for send, TP2 for receive */
	uint rtxd;

	rtxd = TPRAM2(4);
	if (rtxd & 0x8000)						/* RDR Full ? */
	{										/* RECEIVE */
		if ( !(rtxd & 0x4000) )	{			/* check Framing Error */
			rxbuff2[rxend2++] = rtxd;		/* put in buffer */
			rxend2 &= RXrld; }				/* cyclic buffer */
		TPRAM2(4) = 0;
		CISR &= ~0x0004;
	}

	rtxd = TPRAM0(4);
	if (rtxd & 0x8000)						/* TDR Empty ? */
	{
		if (txstart0 == txend0)
			CIER &= ~0x0001;				/* transmit interrupt disable */
		else {
			txstart0++;
			txstart0 &= TXrld;				/* cyclic buffer */
			TPRAM0(4) = (uint)txbuff0[txstart0];	/* TRANSMIT */
		}
		CISR &= ~0x0001;
	}
}
/*----------------------------------------------------------*/
_IH void tpuint_aux ()		/* ISR for auxiliary serial I/O */
{							/* TP1 for send, TP3 for receive*/
	uint rtxd;

	rtxd = TPRAM3(4);
	if (rtxd & 0x8000)						/* RDR Full ? */
	{										/* RECEIVE */
		if ( !(rtxd & 0x4000) )	{			/* check Framing Error */
			rxbuff3[rxend3++] = rtxd;		/* put in buffer */
			rxend3 &= RXaux; }				/* cyclic buffer */
		TPRAM3(4) = 0;
		CISR &= ~0x0008;					/* !!! prej 10		*/
	}

	rtxd = TPRAM1(4);
	if (rtxd & 0x8000)						/* TDR Empty ? */
	{
		if (txstart1 == txend1)
			CIER &= ~0x0002;				/* transmit interrupt disable */
		else {								/* !!! prej 80				*/
			txstart1++;
			txstart1 &= TXaux;				/* cyclic buffer */
			TPRAM1(4) = (uint)txbuff1[txstart1];	/* TRANSMIT */
		}
		CISR &= ~0x0002;					/* !!! prej 80				*/
	}
}
/*----------------------------------------------------------*/
_IH void tpuint_C100 ()	/* ISR for serial I/O with C100 compass */
{						/* TP7 for send, TP6 for receive */
	uint rtxd;

	rtxd = TPRAM6(4);
	if (rtxd & 0x8000)						/* RDR Full ? */
	{										/* RECEIVE */
		if ( !(rtxd & 0x4000) )	{			/* check Framing Error */
			rxbuff6[rxend6++] = rtxd;		/* put in buffer */
			rxend6 &= RXc100; }				/* cyclic buffer */
		TPRAM6(4) = 0;
		CISR &= ~0x0040;
	}

	rtxd = TPRAM7(4);
	if (rtxd & 0x8000)						/* TDR Empty ? */
	{
		if (txstart7 == txend7)
			CIER &= ~0x0080;				/* transmit interrupt disable */
		else {
			txstart7++;
			txstart7 &= TXc100;				/* cyclic buffer */
			TPRAM7(4) = (uint)txbuff7[txstart7];	/* TRANSMIT */
		}
		CISR &= ~0x0080;
	}
}
/*----------------------------------------------------------*/
_IH void tpuint_gps ()	/* ISR for serial I/O with GPS*/
{						/* TP9 for send, TP8 for receive */
	uint rtxd;

	rtxd = TPRAM8(4);
	if (rtxd & 0x8000)						/* RDR Full ? */
	{										/* RECEIVE */
		if ( !(rtxd & 0x4000) )	{			/* check Framing Error */
			rxbuff8[rxend8++] = rtxd;		/* put in buffer */
			rxend8 &= RXgps; }				/* cyclic buffer */
		TPRAM8(4) = 0;
		CISR &= ~0x0100;
	}

	rtxd = TPRAM9(4);
	if (rtxd & 0x8000)						/* TDR Empty ? */
	{
		if (txstart9 == txend9)
			CIER &= ~0x0200;				/* transmit interrupt disable */
		else {
			txstart9++;
			txstart9 &= TXgps;			/* cyclic buffer */
			TPRAM9(4) = ParityOdd (txbuff9[txstart9]); /* TRANSMIT */
		}
		CISR &= ~0x0200;
	}
}
/*----------------------------------------------------------*/
_IH void tpuint_ir ()	/* ISR for serial input with IR		*/
{						/* TP14 for send, TP15 for receive	*/
	uint rtxd;

	rtxd = TPRAM15(4);
	if (rtxd & 0x8000)					/* RDR Full ? */
	{										/* RECEIVE */
		if ( !(rtxd & 0x4000) )	{			/* check Framing Error */
			rxbuff15[rxend15++] = rtxd;		/* put in buffer */
			rxend15 &= RXir; }				/* cyclic buffer */
		TPRAM15(4) = 0;
		CISR &= ~0x8000;
	}

	rtxd = TPRAM14(4);
	if (rtxd & 0x8000)					/* TDR Empty ? */
	{
		if (txstart14 == txend14)
			CIER &= ~0x4000;				/* transmit interrupt disable */
		else {
			txstart14++;
			txstart14 &= TXir;				/* cyclic buffer */
			TPRAM14(4) = ParityOdd (txbuff14[txstart14]); /* TRANSMIT */
		}
		CISR &= ~0x4000;
	}
}
/*----------------------------------------------------------*/
_IH void tpuint ()
{
	uint rtxd;

	rtxd = CISR;		/* whodunit */
	CIER &= ~rtxd;		/* disable spurious TPU interrupt */
	CISR &= ~rtxd;		/* clear spurious TPU interrupt status */
}
#endif
/*----------------------------------------------------------*/
uint ParityOdd (uint c)	/* add 9th=parity bit */
{
	uint i, ret = c ^ 0x0100;

	for (i = 0; i < 8; i++)
	{
		if (c & 1)	ret ^= 0x0100;
		c >>= 1;
	}
	return ret;
}
/*----------------------------------------------------------*/
/*** Character I/O ***/

int Kbhit (int channel)
{
	int	ret = 0;

	switch (channel)
	{
		case CHPC:
			if (rxstart != rxend) ret = -1;
			break;
		case CHAUX:
			if (rxstart3 != rxend3) ret = -1;
			break;
		case CHRLD:
			if (rxstart2 != rxend2) ret = -1;
			break;
		case CHC100:
			if (rxstart6 != rxend6) ret = -1;
			break;
		case CHGPS:
			if (rxstart8 != rxend8) ret = -1;
			break;
		case CHIR:
			if (rxstart15 != rxend15) 
				ret = -1;
			break;
        case CHLCD:	ret=Key;
		default:
			break;
	}
	return ret;
}
/*----------------------------------------------------------*/
int Getch (int channel)
{
	int	ret = -1;

	switch (channel)
	{
		case CHPC:
			while (rxstart == rxend);
			ret = (int)ReadBuffer_pc () & 0x00FF;
			break;
		case CHAUX:
			while (rxstart3 == rxend3);
			ret = (int)ReadBuffer_aux () & 0x00FF;
			break;
		case CHRLD:
			while (rxstart2 == rxend2);
			ret = (int)ReadBuffer_rld () & 0x00FF;
			break;
		case CHC100:
			while (rxstart6 == rxend6);
			ret = ((int)ReadBuffer_C100 ()) & 0x00FF;
			break;
		case CHGPS:
			while (rxstart8 == rxend8);
			ret = ReadBuffer_gps ();
			break;
		case CHIR:
			while (rxstart15 == rxend15);
			ret = ReadBuffer_ir ();
			break;
		case CHLCD:
			ret = GetKey();
			break;
		default:
			break;
	}
	return ret;
}
/*----------------------------------------------------------*/
int Putch (int c, int channel)
{
	watchdog();
	switch (channel)
	{
		case CHPC:
			put_pc (c);
			break;
		case CHAUX:
			put_aux (c);
			break;
		case CHRLD:
			put_rld (c);
			break;
		case CHC100:
			put_C100 (c);
			break;
		case CHGPS:
			put_gps (c);
			break;
		case CHIR:
			put_ir (c);
			break;
		case CHLCD:
			put_lcd (c);
			break;
		default:
			c = 0;
			break;
	}
	return c;
}
/*----------------------------------------------------------*/
void put_pc (int c)		/* put a character in transmit buffer */
{
	uint t;

#ifdef WIN32
	static FILE *f;
	if(!f)
		f=fopen("fpcw.dat","w");
	fputc(c,f);
#endif

	t = txend + 1;
	t &= TXpc;
	while (t == txstart);			/* prevent overflow */
	disable_interrupts();
	txbuff[t] = (uchar)c;
	txend = t;
	enable_interrupts();
#ifndef	WIN32
	SCCR1 |= 0x0080;				/* transmit interrupt enable */
#endif
}
/*----------------------------------------------------------*/
void put_aux (int c)
{
	uint t;

	t = txend1 + 1;
	t &= TXaux;
	while (t == txstart1);				/* prevent overflow */
	disable_interrupts();
	txbuff1[t] = (uchar)c;
	txend1 = t;
	enable_interrupts();
#ifndef	WIN32
	if (!(CIER & 0x0002)) {				/* transmit idle ? */
		while (!(TPRAM1(4) & 0x8000));	/* TDR Empty ? */
		TPRAM1(4) = (uint) c;	 		/* TRANSMIT 1st char */
		txstart1++;
		txstart1 &= TXaux;				/* cyclic buffer */
	}
	CIER |= 0x0002;						/* transmit interrupt enable */
#endif
}
/*----------------------------------------------------------*/
void put_rld (int c)
{
	uint t;

	t = txend0 + 1;
	t &= TXrld;
	while (t == txstart0);				/* prevent overflow */
	disable_interrupts();
	txbuff0[t] = (uchar)c;
	txend0 = t;
	enable_interrupts();
#ifndef	WIN32
	if (!(CIER & 0x0001)) {				/* transmit idle ? */
		while (!(TPRAM0(4) & 0x8000));	/* TDR Empty ? */
		TPRAM0(4) = (uint) c;	 		/* TRANSMIT 1st char */
		txstart0++;
		txstart0 &= TXrld;				/* cyclic buffer */
	}
	CIER |= 0x0001;					/* transmit interrupt enable */
#endif
}
/*----------------------------------------------------------*/
void put_C100 (int c)
{
	uint t;

	t = txend7 + 1;
	t &= TXc100;
	while (t == txstart7);				/* prevent overflow */
	disable_interrupts();
	txbuff7[t] = (uchar)c;
	txend7 = t;
	enable_interrupts();
#ifndef	WIN32
	if (!(CIER & 0x0080)) {				/* transmit idle ? */
		while (!(TPRAM7(4) & 0x8000));	/* TDR Empty ? */
		TPRAM7(4) = (uint) c;	 		/* TRANSMIT 1st char */
		txstart7++;
		txstart7 &= TXc100;				/* cyclic buffer */
	}
	CIER |= 0x0080;						/* transmit interrupt enable */
#endif
}
/*----------------------------------------------------------*/
void put_gps (int c)
{
	uint t;

	t = txend9 + 1;
	t &= TXgps;
	while (t == txstart9);				/* prevent overflow */
	disable_interrupts();
	txbuff9[t] = (uchar)c;
	txend9 = t;
	enable_interrupts();
#ifndef	WIN32
	if (!(CIER & 0x0200)) {				/* transmit idle ? */
		while (!(TPRAM9(4) & 0x8000));	/* TDR Empty ? */
		TPRAM9(4) = ParityOdd (c); 		/* TRANSMIT 1st char */
		txstart9++;
		txstart9 &= TXgps;				/* cyclic buffer */
	}
	CIER |= 0x0200;						/* transmit interrupt enable */
#endif
}
/*----------------------------------------------------------*/
void put_ir (int c)
{
	uint t;

	t = txend14 + 1;
	t &= TXir;

/*	BUG !!! CIER je aktiven, pa ne sprozi interrupta pri praznem 
	registru
*/
	while (t == txstart14)
	{
#ifndef	WIN32
		while (!(TPRAM14(4) & 0x8000));		/* TDR Empty ? */
		TPRAM14(4) = ParityOdd (txstart14);	/* TRANSMIT 1st char */
		txstart14 = ++txstart14 & TXir;		/* cyclic buffer */
#endif
	}										

	disable_interrupts();
	txbuff14[t] = (uchar)c;
	txend14 = t;
	enable_interrupts();
#ifndef	WIN32
	if (!(CIER & 0x4000)) {				/* transmit idle ? */
		while (!(TPRAM14(4) & 0x8000));	/* TDR Empty ? */
		TPRAM14(4) = ParityOdd (c); 	/* TRANSMIT 1st char */
		txstart14 = ++txstart14 & TXir;	/* cyclic buffer */
	}
	CIER |= 0x4000;						/* transmit interrupt enable */
#endif
}
/*----------------------------------------------------------*/
uint GetCTS  ()						/* TP4 */
{
#ifndef	WIN32
	if (TPRAM4(2) & 0x8000)	/* read last input pin-level */
		return 1;
	else
#endif
		return 0;
}
/*----------------------------------------------------------*/
void SetRTS (uint state)			/* TP5 */
{
#ifndef	WIN32
	HSRR1 &= ~0x0C00;		/* clear Ch5 bits */
	state &= 1;
	if (state == 1)
		HSRR1 |=  0x0400;	/* set pin high w/ host service request '01' */
	else if (state == 0)
		HSRR1 |=  0x0800;	/* set pin low  w/ host service request '10' */
#endif
}
/*----------------------------------------------------------*/
/*** Standard unformatted output - puts pc ***/

void puts_pc (char *string)
{
	uint c, i = 0;

	while ( (c = string[i++]) != 0 )	/* till the end of string */
		Putch (c, CHPC);
}
/*----------------------------------------------------------*/
/*** Standard unformatted output - puts rf ***/

void puts_rf (char *string)
{
	uint c, i = 0;

	while ( (c = string[i++]) != 0 )	/* till the end of string */
		Putch (c, CHRF);
}
/*----------------------------------------------------------*/
/*** Standard unformatted output - puts aux ***/

void puts_aux (char *string)
{
	uint c, i = 0;

	while ( (c = string[i++]) != 0 )	/* till the end of string */
		Putch (c, CHAUX);
}
/*----------------------------------------------------------*/
void puts_rld (char *string)
{
	uint c, i = 0;

	while ( (c = string[i++]) != 0 )	/* till the end of string */
		Putch (c, CHRLD);
}
/*----------------------------------------------------------*/
void puts_C100 (char *string)
{
	uint c, i = 0;

	while ( (c = string[i++]) != 0 )	/* till the end of string */
		Putch (c, CHC100);
}
/*----------------------------------------------------------*/
void puts_gps (char *string)
{
uint i,j;

do
{
	j=string[i++];
	Putch (j, CHGPS);
}	while((j != '\x10') || (string[i] != '\x03'));
Putch('\x03',CHGPS);
}
/*----------------------------------------------------------*/
uchar ReadBuffer_pc ()
{
	uchar c = rxbuff[rxstart++];
	rxstart &= RXpc;
	--rxcount;
	return c;
}
/*----------------------------------------------------------*/
void DecodeBlock_pc (char *blk)
{
}
/*----------------------------------------------------------*/
uchar ReadBuffer_aux ()
{
	uchar c = rxbuff3[rxstart3++];
	rxstart3 &= RXaux;
	return c;
}
/*----------------------------------------------------------*/
uchar ReadBuffer_C100 ()
{
	static char rcvblk6[RXc100+1];	/* buffer for DecodeBlock */
	static uint iblk6;				/* rcvblk index */

	uchar c = rxbuff6[rxstart6++];
	rxstart6 &= RXc100;

	iblk6 &= RXc100;
	rcvblk6[iblk6++] = c;

	if(C100_ECHO)
		Putch(c,CHPC);

	switch (c)						/* handle block start and block end separately */
	{
		case '$':					/* beginning of the rcvblock */
			iblk6 = 1;				/* reset rcvblk index */
			rcvblk6[0] = c;
			break;
		case '\r':					/* end of the rcvblock */
		case '\n':
			rcvblk6[iblk6] = 0;		/* NULL termination of rcvblk string */
			DecodeBlock_C100 (rcvblk6);	/* decode block */
			iblk6 = 0;
			break;
		default:
			break;
	}
	return c;
}
/*----------------------------------------------------------*/
void DecodeBlock_C100 (char *blk)
{
	int		i,j;
	char	c[32];
	if(sscanf(blk,"$%5lf,D,OK*%2x",&c100.az,&i)) {
		c100.status |= 0x80;
/*		puts_C100("?cs2\r");
*/		}

/*	if(sscanf(blk,"?cs2 %d,%d,%d",&i,&j,&n))
	{
		sprintf(c,"Noise score %d,%d",i,j);
		sprintf(error_screen[0],"%-20s",c);
		sprintf(c,"Count %d",n);
		sprintf(error_screen[1],"%-20s",c);
	}
*/
	if(sscanf(blk,"Noise score: %d,%d",&i,&j))
	{
		sprintf(c,"Noise score %d,%d",i,j);
		sprintf(error_screen[0],"%-20s",c);
	}
	if(sscanf(blk,"Calibration count %d",&i))
	{
		sprintf(c,"Count %d",i);
		sprintf(error_screen[1],"%-20s",c);
	}
}
/*----------------------------------------------------------*/
int ReadBuffer_gps ()
{
	int c = rxbuff8[rxstart8++];
	rxstart8 &= RXgps;
	DecodeBlock_gps ( c );				/* v gps332.c!		*/
	return c;
}
/*----------------------------------------------------------*/
int ReadBuffer_ir ()
{
	static char rcvblk2[RXir+1];		/* buffer for DecodeBlock */
	static uint iblk2;					/* rcvblk index */
	int			i;
	char		jj;

	uchar c = rxbuff15[rxstart15++];
	rxstart15 &= RXir;

	iblk2 &= RXir;
	rcvblk2[iblk2++] = c;

	switch (c)							/* handle block start and block end separately */
	{
		case BLKSTRT3:					/* beginning of the SOPHIE rcvblock 	*/
			iblk2 = 1;					/* reset rcvblk index 				*/
			rcvblk2[0] = c;
			break;
		case BLKEND3:
			jj=0;						/* end of the rcvblock 				*/
			for(i=0;i<iblk2-1;++i)
				jj ^= rcvblk2[i];
			if(jj==0)
			{
				switch (rcvblk2[1])
				{
					case 0x11:
						rcvblk2[--i]=0;
						translate(rcvblk2,0x3F,' ');
						rcvblk2[20]=0;
						mess(&rcvblk2[3]);
						break;
				}
			}			
			iblk2 = c = 0;
			break;
		default:
			break;
	}
	return c;
}
/*----------------------------------------------------------*/
uchar ReadBuffer_rld ()
{
	static char rcvblk2[RXrld+1];		/* buffer for DecodeBlock */
	static uint iblk2;					/* rcvblk index */

	uchar c = rxbuff2[rxstart2++];
	rxstart2 &= RXrld;

	iblk2 &= RXrld;
	rcvblk2[iblk2++] = c;

	switch (c)							/* handle block start and block end separately */
	{
		case BLKSTRT1:					/* beginning of the RLD-E rcvblock 	*/
            DecodeBlock_tcm2(rcvblk2);
			iblk2 = 1;					/* reset rcvblk index 				*/
			rcvblk2[0] = c;
			break;
		case BLKSTRT2:					/* beginning of the TCM2 rcvblock 	*/
			iblk2 = 1;					/* reset rcvblk index 				*/
			rcvblk2[0] = c;
			break;
		case BLKEND:					/* end of the rcvblock 				*/
			rcvblk2[iblk2++] = 0;		/* NULL termination of rcvblk string*/
			DecodeBlock_rld (rcvblk2);	/* decode block 					*/
			iblk2 = c = 0;
			break;
		default:
			break;
	}
	return c;
}
/*----------------------------------------------------------*/
void	DecodeBlock_tcm2 (char *blk)
		{
		char	*c,*p,*r,*t;

		if(*blk != '$')
			return;
        c=strchr(blk,'C');
        p=strchr(blk,'P');
        r=strchr(blk,'R');
        t=strchr(blk,'T');

		if(c && p && r && t) {
        	*c++ = *p++ = *r++ = *t++ = 0;
            tcm2.c = atof(c);
            tcm2.p = atof(p);
            tcm2.r = atof(r);
            tcm2.t = atof(t);
            tcm2.status = 0x80;
            }
		}
/*----------------------------------------------------------*/
void DecodeBlock_rld (char *blk)
{
	char number[6];

	if (blk[0] == '*')	
		{
		switch (blk[1]) 
			{
			case 's':
				if (blk[2] & 0x10)
					rld.status |= HV_READY;
				else
					rld.status &= ~HV_READY;
				break;

			case 'R':
			case 'r':
			case 'l':
				break;

			case 'B':
				rld.buttons = blk[2];
				break;
			case 'D':
				rld.status |= HV_READY;		/* HV ready */
				break;
			default:
				if (blk[7] & 0x04)
					{
					strncpy (number, blk+1, 5);
					number[5]=0;
					rld.R[rld.N] = atoi (number);
					rld.status |= RLD_TREQ;

					if (blk[6] & 0x10)
						rld.status |= HV_READY;
					else
						rld.status &= ~HV_READY;					
					if (blk[6] & 0x01)
						rld.status |= RLD_BLOCTG;
					if (blk[6] & 0x02)
						rld.status |= RLD_MULTG;
				}
				break;
			}
		rld.status |= RLD_CHECK;
		}
}
/*----------------------------------------------------------*/
void RLD_Local ()			/* put RLDE in local/stand-alone mode */
{
	puts_rld ("*L\x04");
}
/*----------------------------------------------------------*/
void RLD_Remote ()			/* put RLDE in remote/slave mode */
{
	puts_rld ("*R\x04");
}
/*----------------------------------------------------------*/
void RLD_Trigger ()			/* measure distance */
{
	puts_rld ("*C\x04");
}
/*----------------------------------------------------------*/
void RLD_GetTarget (uint t)	/* get desired target */
{
	puts_rld ("*E");
	Putch ('0' | t, CHRLD);
	puts_rld ("\x04");
}
/*----------------------------------------------------------*/
void RLD_ShowTarget (uint r)/* display target */
{
	char c[16];
	sprintf (c,"*W%5d \x04",r);
	puts_rld (c);
}
/*----------------------------------------------------------*/
void RLD_GetStatus ()		/* get RLDE's status */
{
	char	c[16];
	sprintf(c,"*S\x04");
	puts_rld (c);
}
/*----------------------------------------------------------*/
void RLD_StartHV ()			/* start RLDE's HV converter */
{
	puts_rld ("*D\x04");
}
/*----------------------------------------------------------*/
void RLD_Echo ()			/* echo enable 				 */
{
	puts_rld ("*A\x04");
}
/*----------------------------------------------------------*/
void RLD_Noecho ()			/* echo disable				 */
{
	puts_rld ("*B\x04");
}
/*----------------------------------------------------------*/
void RLD_PowerOff ()		/* put RLDE to deep sleep */
{
	puts_rld ("*PO\x04");
}
/*----------------------------------------------------------*/
/*** GPS ***/
void getGPS ()
{
	puts_gps ("\x10\x24\x10\x03");	/* request GPS health 				*/
	puts_gps ("\x10\x26\x10\x03");	/* request mode select				*/
	puts_gps ("\x10\x37\x10\x03");	/* status & values of last pos fix	*/
}
/*----------------------------------------------------------*/
/**** Period Measurement ****/

uint GetPeriod ()
{
#ifndef	WIN32
	return abs(TPRAM11(8) - TPRAM11(10));	/* FINAL_ - LAST_TRANS_TIME */
#else
	return(null);
#endif
}											/* 104 = 0x68 = 5kHz = 0.2ms */
/*----------------------------------------------------------*/
int	Xonoff(uint i)
	{
	xmode = i;
	return(i);
	}
/*----------------------------------------------------------*/
int	Baud(uint ch,uint s)
	{
#ifndef	WIN32
	int	i,pcrate[10][10]={{110,300,600,1200,2400,4800,9600,19200,38400,57600},
				{4766,1748,874,437,218,109,55,27,14,9}};
	switch(ch) {
		case CHC100:	TPRAM6(2)=TPRAM7(2)=(38400.0/(double)s)*110;
						return(eof);
		case CHGPS:		TPRAM8(2)=TPRAM9(2)=(38400.0/(double)s)*110;
						return(eof);
		case CHAUX:		TPRAM1(2)=TPRAM3(2)=(38400.0/(double)s)*110;
						return(eof);
		case CHRLD:		TPRAM0(2)=TPRAM2(2)=(38400.0/(double)s)*110;
						return(eof);
		case CHPC:		for(i=0;i<10;++i)
							if(pcrate[0][i]==s) {
								SCCR0=pcrate[1][i];
								return(eof);
								}
						return(null);
		}
#endif
	return(null);
	}

/* rld.status bit definitions:
 *
 * bit 7	laser ready
 * bit 2	target 2 present
 * bit 1	target 1 present
 * bit 0	target present

 * rld.buttons bit definitions:
 *
 * bit 3	FIRST/LAST released
 * bit 2	FIRST/LAST pressed
 * bit 1	TRIG released
 * bit 0	TRIG pressed
 */

/* C100.status bit definitions:
 *
 * bit 7	new compass heading collected
 */
/* gps.status bit definitions:
 *
 * bit 7	data for last position fix collected
 * bit 2	GPS is in 4-Sat (3-D) mode
 * bit 1	GPS is in 3-Sat (2-D) mode
 * bit 0	GPS has time
 */
/*----------------------------------------------------------*/
#ifdef	WIN32
vint	SysCtl0(FILE *f,vint ch, vchar *p,vint in, vint out,int max)
{
	if(in != out)
	{
		fprintf(f,"c%d=",ch);
		while(in != out)
		{
			in++;
			in &= max;
			fprintf(f,"%02X",p[in]);
		}
		fprintf(f,"\n");
	}
	return(in);
}
/*----------------------------------------------------------*/
vint	SysCtl1(char *p, vchar *q,vint in, vint out,int max)
{
	int	i;
	while(*p)
	{
		sscanf(p,"%02X",&i);
		q[out++] = i;
		out &= max;
		++p;++p;
	}
	return(out);
}
/*----------------------------------------------------------*/
void	SysCtl(void)
{
	extern	char	auxLcdBuffer[], auxLcdFlag;
	static long		fpos;
	int				i;
	char			c[1024];

	FILE *f1=_fsopen("\\sysctlw.dat","a",SH_DENYNO);
	FILE *f2=_fsopen("\\sysctlr.dat","r",SH_DENYNO);
	txstart=SysCtl0(f1,CHPC,txbuff,txstart,txend,TXpc);
	txstart0=SysCtl0(f1,CHRLD,txbuff0,txstart0,txend0,TXrld);
	txstart1=SysCtl0(f1,CHAUX,txbuff1,txstart1,txend1,TXaux);
	txstart7=SysCtl0(f1,CHC100,txbuff7,txstart7,txend7,TXc100);
	txstart9=SysCtl0(f1,CHGPS,txbuff9,txstart9,txend9,TXgps);
	txstart14=SysCtl0(f1,CHIR,txbuff14,txstart14,txend14,TXir);

	if(auxLcdFlag)
	{
		fprintf(f1,"c0=");
		for(i=0; i<82; ++i)
			fprintf(f1,"%02X",auxLcdBuffer[i]);
		fprintf(f1,"\n");
		auxLcdFlag=0;
	}
	if(f2)
	{
		fseek(f2,fpos,0);
		if(fscanf(f2,"c%d=%s\n",&i,c)==2)
		{
			switch(i)
			{
			case CHLCD:
				Key=chrtx(c);
				break;
			case CHPC:
				rxend=SysCtl1(c,rxbuff,rxstart,rxend,RXpc);
				break;
			case CHRLD:
				rxend2=SysCtl1(c,rxbuff2,rxstart2,rxend2,RXrld);
				break;
			case CHAUX:
				rxend3=SysCtl1(c,rxbuff3,rxstart3,rxend3,RXaux);
				break;
			case CHC100:
				rxend6=SysCtl1(c,rxbuff6,rxstart6,rxend6,RXc100);
				break;
			case CHGPS:
				rxend8=SysCtl1(c,rxbuff8,rxstart8,rxend8,RXgps);
				break;
			case CHIR:
				rxend15=SysCtl1(c,rxbuff15,rxstart15,rxend15,RXir);
				break;
			}
			fpos=ftell(f2);
		}
		fclose(f2);
	}
	fclose(f1);
//	SetTask(SysCtl,0,RTC_task);
}
/*--------------------------------------------------------*/
void	ScanConsole(void)
{
	static	FILE	*f;
	char	c[1024];
	long	i;

	if(!f)
		f=fopen("fpcr.dat","r");
	if(f)
	{
		SetTask(ScanConsole,0,RTC_task);
		if(fgets(c,1024,f))
		{
			for(i=0; c[i]; ++i)
			{
				rxbuff[rxend++] = c[i];
				rxend &= RXpc;
			}
		}
	}
}
#endif
