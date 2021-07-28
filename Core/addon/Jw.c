#include "remmenu.h"
/*......................................................................*/
#define	ADS_DATA	0x01
#define	IBIT_RES	0x02
#define	AZ_DATA 	0x81
#define	IBIT_COM	0x82
#define	ACK_MESS	0x7e
#define	NACK_MESS	0x7f

#define	JW_LASER	0x01
#define	JW_LOBATT	0x02
/*......................................................................*/
void	AdsData(void)
		{
		uint i;
		if(aux_mode != AUX_JW)
			return;
		if(JwCount || CheckTask(JwSend,PIT_task))
			SetTask(AdsData,1,RTC_task);
		else{
			DeleteTask(AdsData,RTC_task);
			Jw = JwTx;
			*Jw++=7;
			*Jw++=ADS_DATA;
			i=JwAZ;
			*Jw++=i / 256;
			*Jw++=i % 256;
			i=JwR;
			*Jw++=i  / 256;
			*Jw++=i  % 256;
			i=JwEL;
			*Jw++=i / 256;
			*Jw++=i % 256;
			JwSend();
			}
		}
/*......................................................................*/
void	IbitRes(void)
		{
static	int	Ibit_old=0x80;

		if(aux_mode != AUX_JW)
			return;

		if(!BatteryOK())
			Ibit |= JW_LOBATT;
		else
			Ibit &= ~JW_LOBATT;

		if(CheckTask(CheckRLD_on,RTC_task))
			Ibit |= JW_LASER;
		else
			Ibit &= ~JW_LASER;

		if(Ibit != Ibit_old) {
			if(!JwCount && !CheckTask(JwSend,PIT_task)) {
				Jw = JwTx;
				*Jw++=2;
				*Jw++=IBIT_RES;
				Ibit &= 0x7f;
				put_char(&Jw,Ibit);
				JwSend();
				Ibit_old=Ibit;
				}
			}
		}
/*......................................................................*/
void	JwSend(void)
		{
static	int	n;
		int	i,j;

		if(!CheckTask(JwSend,PIT_task)) {
			n=0;
			SetTask(JwSend,100,PIT_task);
			}
		for(i=j=0; i<=JwTx[0]; ++i)
			j += Putch(JwTx[i],CHAUX);
		Putch(j,CHAUX);
		if(++n >= 5)
			DeleteTask(JwSend,PIT_task);
		}
/*......................................................................*/
void	JwRec(void)
		{
static	int ack_flag;
		int	j;

		if(Kbhit(CHAUX))
			if(CheckTask(JwSend,PIT_task))
				switch(j=Getch(CHAUX)) {
					case ACK_MESS:  ack_flag = ACK_MESS;
									break;
					case NACK_MESS:	JwSend();
									break;
					default:		if((ack_flag == ACK_MESS) && (j == JwTx[0]))
										DeleteTask(JwSend,PIT_task);
									ack_flag = 0;
									break;
					}
			else
				switch(JwCount)	{
					case	0:	JwCount=JwCheck=Getch(CHAUX);
								if(JwCount > 16)
									JwCount=0;
								else{
									JwCount++;
									Jw=JwRx;
									SetTask(JwDrop,50,PIT_task);
									}
								break;
					case	1:  if(JwExe()) {
									Putch(ACK_MESS,CHAUX);
									Putch(JwRx[0],CHAUX);
									}
								else
									Putch(NACK_MESS,CHAUX);
								JwCount--;
								DeleteTask(JwDrop,PIT_task);
								break;
					default: 	*Jw = Getch(CHAUX);
								JwCheck += *Jw++;
								JwCount--;
								break;
					}
		}
/*......................................................................*/
int		JwExe(void)
		{
		int		i;

		if(Getch(CHAUX) != JwCheck)
			return(null);
		switch(JwRx[0]) {
			case AZ_DATA:   shaft_enable();
							i = JwRx[1]*256 + JwRx[2];
							az_off += read_AZ()/maxmils*2.0*M_PI
										- (double)i / 3200.0 * M_PI;
							break;
			case IBIT_COM:	Ibit |= 0x80;
							break;
			default:        return(null);
			}
		return(eof);
		}
/*......................................................................*/
void	JwDrop(void)
		{
		JwCount=0;
		DeleteTask(JwDrop,PIT_task);
		}
/*......................................................................*/
