#ifndef	HW332
#define HW332
#endif

#define SYNCR		(*(volatile unsigned int *)0xfffa04L)
#define SWSR		(*(volatile unsigned char *)0xfffa27L)
#define SYPCR		(*(volatile unsigned char *)0xfffa21L)
#define	RSR			(*(volatile unsigned char *)0xfffa07L)

#define DDRE		(*(volatile unsigned char *)0xfffa15L)
#define PORTE		(*(volatile unsigned char *)0xfffa11L)

#define	PFPAR		(*(volatile unsigned char *)0xfffa1fL)
#define DDRF		(*(volatile unsigned char *)0xfffa1dL)
#define PORTF		(*(volatile unsigned char *)0xfffa1bL)

#define	TMCR		(*(volatile unsigned int *)0xfffe00L)
#define TICR		(*(volatile unsigned int *)0xfffe02L)
#define CFSR1		(*(volatile unsigned int *)0xfffe0eL)
#define	CHaPAR0		(*(volatile unsigned int *)0xffffa0L)
#define CHaPAR2		(*(volatile unsigned int *)0xffffa4L)
#define CHaPAR3		(*(volatile unsigned int *)0xffffa6L)
#define	HSPRO		(*(volatile unsigned int *)0xfffe18L)
#define CPRO		(*(volatile unsigned int *)0xfffe1cL)

#define CSBARBT		(*(volatile unsigned int *)0xfffa48L)
#define CSORBT		(*(volatile unsigned int *)0xfffa4AL)
#define CSBAR0		(*(volatile unsigned int *)0xfffa4CL)
#define CSOR0		(*(volatile unsigned int *)0xfffa4EL)
#define CSBAR1		(*(volatile unsigned int *)0xfffa50L)
#define CSOR1		(*(volatile unsigned int *)0xfffa52L)
#define CSBAR2		(*(volatile unsigned int *)0xfffa54L)
#define CSOR2		(*(volatile unsigned int *)0xfffa56L)
#define CSBAR3		(*(volatile unsigned int *)0xfffa58L)
#define CSOR3		(*(volatile unsigned int *)0xfffa5AL)
#define CSBAR4		(*(volatile unsigned int *)0xfffa5CL)
#define CSOR4		(*(volatile unsigned int *)0xfffa5EL)
#define CSBAR5		(*(volatile unsigned int *)0xfffa60L)
#define CSOR5		(*(volatile unsigned int *)0xfffa62L)
#define CSBAR7		(*(volatile unsigned int *)0xfffa68L)
#define CSOR7		(*(volatile unsigned int *)0xfffa6AL)
#define CSBAR8		(*(volatile unsigned int *)0xfffa6CL)
#define CSOR8		(*(volatile unsigned int *)0xfffa6EL)
#define CSBAR9		(*(volatile unsigned int *)0xfffa70L)
#define CSOR9		(*(volatile unsigned int *)0xfffa72L)
#define CSBAR10		(*(volatile unsigned int *)0xfffa74L)
#define CSOR10		(*(volatile unsigned int *)0xfffa76L)
#define PEPAR		(*(volatile unsigned int *)0xfffa16L)
#define CSPAR0		(*(volatile unsigned int *)0xfffa44L)
#define CSPAR1		(*(volatile unsigned int *)0xfffa46L)
#define PORTC		(*(volatile unsigned char *)0xfffa41L)
#define RAMBAR		(*(volatile unsigned int *)0xfffb04L)

#define QMCR 		(*(volatile unsigned int  *)0xfffc00L)
#define QTEST		(*(volatile unsigned int  *)0xfffc02L)
#define QILR		(*(volatile unsigned char *)0xfffc04L)
#define QIVR		(*(volatile unsigned char *)0xfffc05L)
#define QPDR		(*(volatile unsigned char *)0xfffc15L)
#define QPAR		(*(volatile unsigned char *)0xfffc16L)
#define	QDDR		(*(volatile unsigned char *)0xfffc17L)
#define	SPCR0		(*(volatile unsigned int  *)0xfffc18L)
#define	SPCR1		(*(volatile unsigned int  *)0xfffc1aL)
#define	SPCR2		(*(volatile unsigned int  *)0xfffc1cL)
#define	SPCR3		(*(volatile unsigned int  *)0xfffc1eL)
#define	SPSR		(*(volatile unsigned char *)0xfffc1fL)

#define	PITR		(*(volatile unsigned int *)0xfffa24L)
#define	PICR		(*(volatile unsigned int *)0xfffa22L)

#define	PRAMA_2		(*(volatile unsigned int *)0xffffa4L)
#define	PRAMA_3		(*(volatile unsigned int *)0xffffa6L)
#define	PRAMB_8		(*(volatile unsigned int *)0xffffb8L)
#define	PRAMB_A		(*(volatile unsigned int *)0xffffbaL)

#define LCD			((volatile unsigned char *)0x200000L)
#define LCDdr		(*(volatile unsigned char *)0x200001L)
#define LCDstatus	(*(volatile unsigned int  *)0x200000L)

#define ENCr		(*(volatile unsigned int  *)0x202000L)
#define ENCw		(*(volatile unsigned int  *)0x203000L)
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/
#define	rx_ram		((volatile unsigned int  *)0xfffd00L)
#define	tx_ram		((volatile unsigned int  *)0xfffd20L)
#define	com_ram		((volatile unsigned char *)0xfffd40L)
/*様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様*/

_IH	void		PITservice(void);
_IH	void		reset_exc(void);

_ASM void stopCPU ()
{
	lpstop #$0700
}

_ASM void reset_f ()
{
	reset
	jmp		(4)
}
/*.............................*/
_ASM void debug ()
{
	bgnd
}


