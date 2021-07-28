/* 332defs.h
 *
 * Definitions for 68332 peripherals SIM, QSM, RAM and TPU
 *
 */

/*** System Integration Module ***/

#define	MCR		(*(portw *) 0xFFFA00)	/* SIM Module Configuration */
#define	SYNCR	(*(portw *) 0xFFFA04)	/* Clock Synthesizer Control */
#define	RSR		(*(portb *) 0xFFFA07)	/* Reset Status */

#define PORTE	(*(portb *) 0xFFFA11)	/* Port E Data */
#define DDRE	(*(portb *) 0xFFFA15)	/* Port E Data Direction */
#define	PEPAR	(*(portb *) 0xFFFA17)	/* Port E Pin Assignment */
#define PORTF	(*(portb *) 0xFFFA1B)	/* Port F Data */
#define DDRF	(*(portb *) 0xFFFA1D)	/* Port F Data Direction */
#define	PFPAR	(*(portb *) 0xFFFA1F)	/* Port F Pin Assignment */

#define	SYPCR	(*(portb *) 0xFFFA20)	/* System Protection Control */
#define	PICR	(*(portw *) 0xFFFA22)	/* Periodic Interrupt Control */
#define	PITR	(*(portw *) 0xFFFA24)	/* Periodic Interrupt Timing */
#define	SWSR	(*(portb *) 0xFFFA27)	/* Software Service / Watchdog */

#define	PORTC	(*(portb *) 0xFFFA41)	/* Port C Data */
#define	CSPAR0	(*(portw *) 0xFFFA44)	/* Chip-Select Pin Assignment 0 */
#define	CSPAR1	(*(portw *) 0xFFFA46)	/* CS Pin Assignment 1 */
#define	CSBARBT	(*(portw *) 0xFFFA48)	/* CS Base Boot */
#define	CSORBT	(*(portw *) 0xFFFA4A)	/* CS Option Boot */
#define	CSBAR0	(*(portw *) 0xFFFA4C)	/* CS Base 0 */
#define	CSOR0	(*(portw *) 0xFFFA4E)	/* CS Option 0 */
#define	CSBAR1	(*(portw *) 0xFFFA50)	/* CS Base 1 */
#define	CSOR1	(*(portw *) 0xFFFA52)	/* CS Option 1 */
#define	CSBAR2	(*(portw *) 0xFFFA54)	/* CS Base 2 */
#define	CSOR2	(*(portw *) 0xFFFA56)	/* CS Option 2 */
#define	CSBAR3	(*(portw *) 0xFFFA58)	/* CS Base 3 */
#define	CSOR3	(*(portw *) 0xFFFA5A)	/* CS Option 3 */
#define	CSBAR4	(*(portw *) 0xFFFA5C)	/* CS Base 4 */
#define	CSOR4	(*(portw *) 0xFFFA5E)	/* CS Option 4 */
#define	CSBAR5	(*(portw *) 0xFFFA60)	/* CS Base 5 */
#define	CSOR5	(*(portw *) 0xFFFA62)	/* CS Option 5 */
#define	CSBAR6	(*(portw *) 0xFFFA64)	/* CS Base 6 */
#define	CSOR6	(*(portw *) 0xFFFA66)	/* CS Option 6 */
#define	CSBAR7	(*(portw *) 0xFFFA68)	/* CS Base 7 */
#define	CSOR7	(*(portw *) 0xFFFA6A)	/* CS Option 7 */
#define	CSBAR8	(*(portw *) 0xFFFA6C)	/* CS Base 8 */
#define	CSOR8	(*(portw *) 0xFFFA6E)	/* CS Option 8 */
#define	CSBAR9	(*(portw *) 0xFFFA70)	/* CS Base 9 */
#define	CSOR9	(*(portw *) 0xFFFA72)	/* CS Option 9 */
#define	CSBAR10	(*(portw *) 0xFFFA74)	/* CS Base 10 */
#define	CSOR10	(*(portw *) 0xFFFA76)	/* CS Option 10 */

/* Chip Select Options */

#define	ASYNC	0		/* mode */
#define SYNC	0x8000
#define LOWR	0x2000	/* byte */
#define UPPR	0x4000
#define	WORD	0x6000
#define	RD		0x0800	/* read/write */
#define	WR		0x1000
#define	RW		0x1800
#define	AS		0		/* address strobe/data strobe */
#define	DS		0x0400
#define WAIT0	0		/* data strobe ack */
#define WAIT1	0x0040
#define WAIT2	0x0080
#define WAIT3	0x00C0
#define WAIT4	0x0100
#define WAIT5	0x0140
#define WAIT6	0x0180
#define WAIT7	0x01C0
#define WAIT8	0x0200
#define WAIT9	0x0240
#define WAIT10	0x0280
#define WAIT11	0x02C0
#define WAIT12	0x0300
#define WAIT13	0x0340
#define FASTT	0x0380
#define EXTER	0x03C0
#define	CPU		0		/* address space */
#define USER	0x0010
#define SUPR	0x0020
#define SUSR	0x0030
#define	IPLX	0		/* interrupt priority level */
#define	IPL1	0x0002
#define	IPL2	0x0004
#define	IPL3	0x0006
#define	IPL4	0x0007
#define	IPL5	0x000A
#define	IPL6	0x000C
#define	IPL7	0x000E
#define AVEC	0x0001

/*** RAM ***/

#define	RAMMCR	(*(portw *) 0xFFFB00)	/* RAM Module Configuration */
#define	RAMBAR	(*(portw *) 0xFFFB04)	/* RAM Base Address and Status */

/*** QSM - QSPI/SCI ***/

#define	QMCR	(*(portw *) 0xFFFC00)	/* QSM Configuration */
#define	QILR	(*(portb *) 0xFFFC04)	/* QSM Interrupt Level */
#define	QIVR	(*(portb *) 0xFFFC05)	/* QSM Interrupt Vector */

#define SCCR0	(*(portw *) 0xFFFC08)	/* SCI Control 0, baud-rate */
#define SCCR1	(*(portw *) 0xFFFC0A)	/* SCI Control 1 */
#define SCSR	(*(portw *) 0xFFFC0C)	/* SCI Status */
#define SCDR	(*(portw *) 0xFFFC0E)	/* SCI Data */

#define	QDPR	(*(portb *) 0xFFFC15)	/* QSM Port Data */
#define	QPAR	(*(portb *) 0xFFFC16)	/* QSM Pin Assignment */
#define	QDDR	(*(portb *) 0xFFFC17)	/* QSM Data Direction */
#define	SPCR0	(*(portw *) 0xFFFC18)	/* QSM Control 0 */
#define	SPCR1	(*(portw *) 0xFFFC1A)	/* QSM Control 1 */
#define	SPCR2	(*(portw *) 0xFFFC1C)	/* QSM Control 2 */
#define	SPCR3	(*(portb *) 0xFFFC1E)	/* QSM Control 3 */
#define	SPSR	(*(portb *) 0xFFFC1F)	/* QSM Status */

#define TDR		SCDR					/* Transmit Data Register */
#define RDR		SCDR					/* Receive Data Register */
#define TDRE	0x0100					/* TDR Empty Flag */
#define RDRF    0x0040					/* RDR Full Flag */

/*** TPU ***/

#define TMCR	(*(portw *) 0xFFFE00)	/* TPU Module Configuration */
#define TICR	(*(portw *) 0xFFFE08)	/* TPU Interrupt Configuration */
#define CIER	(*(portw *) 0xFFFE0A)	/* Channel Interrupt Enable */
#define CFSR0	(*(portw *) 0xFFFE0C)	/* Channel Function Select 0 */
#define CFSR1	(*(portw *) 0xFFFE0E)	/* Channel Function Select 1 */
#define CFSR2	(*(portw *) 0xFFFE10)	/* Channel Function Select 2 */
#define CFSR3	(*(portw *) 0xFFFE12)	/* Channel Function Select 3 */
#define HSQR0	(*(portw *) 0xFFFE14)	/* Host Sequence 0 */
#define HSQR1	(*(portw *) 0xFFFE16)	/* Host Sequence 1 */
#define HSRR0	(*(portw *) 0xFFFE18)	/* Host Service Request 0 */
#define HSRR1	(*(portw *) 0xFFFE1A)	/* Host Service Request 1 */
#define CPR0	(*(portw *) 0xFFFE1C)	/* Channel Priority 0 */
#define CPR1	(*(portw *) 0xFFFE1E)	/* Channel Priority 1 */
#define CISR	(*(portw *) 0xFFFE20)	/* Channel Interrupt Status */
#define LR		(*(portw *) 0xFFFE22)	/* Link Register */
#define SGLR	(*(portw *) 0xFFFE24)	/* Service Grant Latch */
#define DCNR	(*(portw *) 0xFFFE26)	/* Decoded Channel Number */

#define TCR		(*(portw *) 0xFFFE02)	/* Test Configuration */
#define DSCR	(*(portw *) 0xFFFE04)	/* Development Support Control */
#define DSSR	(*(portw *) 0xFFFE06)	/* Development Support Status */

#define TPRAM0(i) (*(portw *)(0xFFFF00+i))	/* TPU Channels 0-15 */
#define TPRAM1(i) (*(portw *)(0xFFFF10+i))	/* Parameter RAM Addresses */
#define TPRAM2(i) (*(portw *)(0xFFFF20+i))
#define TPRAM3(i) (*(portw *)(0xFFFF30+i))
#define TPRAM4(i) (*(portw *)(0xFFFF40+i))
#define TPRAM5(i) (*(portw *)(0xFFFF50+i))
#define TPRAM6(i) (*(portw *)(0xFFFF60+i))
#define TPRAM7(i) (*(portw *)(0xFFFF70+i))
#define TPRAM8(i) (*(portw *)(0xFFFF80+i))
#define TPRAM9(i) (*(portw *)(0xFFFF90+i))
#define TPRAM10(i) (*(portw *)(0xFFFFA0+i))
#define TPRAM11(i) (*(portw *)(0xFFFFB0+i))
#define TPRAM12(i) (*(portw *)(0xFFFFC0+i))
#define TPRAM13(i) (*(portw *)(0xFFFFD0+i))
#define TPRAM14(i) (*(portw *)(0xFFFFE0+i))
#define TPRAM15(i) (*(portw *)(0xFFFFF0+i))

/* Predefined TPU functions */

#define DIO		0x08	/* Discrete Input/Output */
#define ITC		0x0A	/* Input Capture/Input Transition Counter */
#define OC		0x0E	/* Output Compare */
#define PWM		0x09	/* Pulse-Width Modulation */
#define SPWM	0x07	/* Synchronized Pulse-Width Modulation */
#define PMAPMM	0x0B	/* Period Meas. + Additional/Missing Transition Detect */ */
#define PSP		0x0C	/* Position-Synchronized Pulse Generator */
#define SM		0x0D	/* Stepper Motor */
#define PPWA	0x0F	/* Period/Pulse-Width Accumulator */
