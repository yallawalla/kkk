#include "remmenu.h"
#include "rtcm.h"

void	RTCMFixPos(void)
		{
		char c[256];
	    if(!CheckTask(RTCMFixPos,RTC_task)) {
			if(aux_mode == AUX_DGPS) {
				sprintf(c,"\r\n\r\ncreset\r\n");
				puts_aux(c);
         		SetTask(RTCMFixPos,3,RTC_task);
                }
            }
        else
         	{
			sprintf(c,"rtcmrule 6cr\r\n");
			puts_aux(c);
			sprintf(c,"undulation 0\r\n");
			puts_aux(c);
			sprintf(c,"fix position %.7f %.7f %.0f %d %d\r\n",
	     	OBP->xc->x/M_PI*180.0,OBP->xc->y/M_PI*180.0,OBP->xc->z,987,0);
			puts_aux(c);
            if(dGPS)
		 		sprintf(c,"log com1 rtcma ontime %d 1\r\n",dGPS);
            else
		 		sprintf(c,"log com1 rtcma ontime 5 1\r\n");
		 	puts_aux(c);
     		DeleteTask(RTCMFixPos,RTC_task);
            }
		}

char *RTCMDec(char *buff)
	{
	 unsigned int dec[128],n,i;
	 char out[500];
	 unsigned int err;


	 if(RTCMDecode(buff,dec))
		return NULL;
	 if(RTCMEncode(dec,out))
		return NULL;
	 n=(strlen(out)-1)/2;
	 RTCMSize=n;
	 for(i=0;i<n;i++)
		{
		 sscanf(out+1+i*2,"%2X",&err);
		 RTCMLog[i]=(unsigned char)err;
		}
	 return buff;
	}

int RTCMEncode(unsigned int *rtcm_dec,char *rtcm_out)
	{
	 int k,i;

	 k=0;
	 k+=sprintf(rtcm_out+k,".60%04X%02X%02X",
		rtcm_dec[MODZCOUNT],rtcm_dec[HEALTH],rtcm_dec[DATALEN]|0x80);
	 for(i=0;i<rtcm_dec[DATALEN];i++)
		{
		 k+=sprintf(rtcm_out+k,"%02X%04X%02X%02X",
			rtcm_dec[DATA+UDRE+i*4],rtcm_dec[DATA+RANGECORR+i*4],
			rtcm_dec[DATA+RATECORR+i*4],rtcm_dec[DATA+IOD+i*4]);
		}
	 sprintf(rtcm_out+k,"\r");
	 return 0;
	}


int RTCMDecode(char *rtcm_in,unsigned int *rtcm_dec)
	{
	 char *rtcm_start;
	 int i,j,rtcm_len,crc=0x24,rtcm_crc,rtcm,rtcm_neg;

	 /* find beginning of data */
	 rtcm_start=strstr(rtcm_in,"$RTCM,");
	 if(rtcm_start==NULL)
		return ERR_HEADER;
	 rtcm_start+=6;

	 /* get number of bytes (excluding crc)*/
	 rtcm_len=(strstr(rtcm_start,"*")-rtcm_start)/2;
	 if(rtcm_len>MAXINLEN)
		return ERR_INSTR;
	 /* get crc */
	 sscanf(rtcm_start+rtcm_len*2+1,"%2X",&rtcm_crc);
	 /* set negate flag */
	 rtcm_neg=rtcm_start[0]-0x36;
	 /* get valid data bytes, each byte contains only 6 valid bits*/
	 j=0;
	 for(i=0;i<rtcm_len;i++)
		{
		 /* calculate crc */
		 crc^=*(rtcm_start+2*i);
		 crc^=*(rtcm_start+2*i+1);
		 /* get one byte */
		 sscanf(rtcm_start+2*i,"%2X",&rtcm);
		 /* negate if necessary */
		 if(rtcm_neg)
			rtcm=~rtcm;
		 /* bit 7 and bit 6 =0 */
		 rtcm&=0x3F;
		 /* reverse bits */
		 rtcm=RTCMReverseBits(rtcm);
		 /* discard every 5th byte and correct rtcm_neg */
		 if((i+1)%5)
			rtcm_in[j++]=(char)rtcm;
		 else
			rtcm_neg=(rtcm_neg ? ~rtcm : rtcm)&0x1;
		}
	 if(rtcm_crc!=crc)
		return ERR_CRC;
	 /* get header data */
	 rtcm_dec[PREAMBLE]=RTCMGetData(rtcm_in,PREAMBLE_START,PREAMBLE_LEN);
	 rtcm_dec[MESSAGETYPE]=RTCMGetData(rtcm_in,MESSAGETYPE_START,MESSAGETYPE_LEN);
	 rtcm_dec[STATIONID]=RTCMGetData(rtcm_in,STATIONID_START,STATIONID_LEN);
	 rtcm_dec[MODZCOUNT]=RTCMGetData(rtcm_in,MODZCOUNT_START,MODZCOUNT_LEN);
	 rtcm_dec[SEQUENCE]=RTCMGetData(rtcm_in,SEQUENCE_START,SEQUENCE_LEN);
	 rtcm_dec[FRAMELEN]=RTCMGetData(rtcm_in,FRAMELEN_START,FRAMELEN_LEN);
	 rtcm_dec[HEALTH]=RTCMGetData(rtcm_in,HEALTH_START,HEALTH_LEN);
	 if(rtcm_dec[PREAMBLE]!=0x66)
		return ERR_PREAMBLE;
	 /* calculate number of satellites in view
		 each word has 24 data bits, each satellite record has 40 bits*/
	 rtcm_dec[DATALEN]=(rtcm_dec[FRAMELEN]*24)/RTCM_SIZE;
	 /* get data for all satellites in view*/
	 if(!rtcm_dec[DATALEN] || (rtcm_dec[DATALEN]>32))
		return ERR_LEN;
	 for(i=0;i<rtcm_dec[FRAMELEN];i++)
		{
		 rtcm_dec[DATA+UDRE+i*4]=
				RTCMGetData(rtcm_in,UDRE_START+i*RTCM_SIZE,UDRE_LEN);
		 rtcm_dec[DATA+RANGECORR+i*4]=
				RTCMGetData(rtcm_in,RANGECORR_START+i*RTCM_SIZE,RANGECORR_LEN);
		 rtcm_dec[DATA+RATECORR+i*4]=
				RTCMGetData(rtcm_in,RATECORR_START+i*RTCM_SIZE,RATECORR_LEN);
		 rtcm_dec[DATA+IOD+i*4]=
				RTCMGetData(rtcm_in,IOD_START+i*RTCM_SIZE,IOD_LEN);
		}
	 return 0;
	}

int RTCMReverseBits(int rtcm)
	{
	 int i,rtcm_out=0;

	 for(i=0;i<6;i++)
		rtcm_out|=(((rtcm&(1<<i))>0)*(0x20>>i));
	 return rtcm_out;
	}

unsigned int RTCMGetData(char *rtcm_in,int start_bit,int num_bits)
	{
	 int byte,i;
	 unsigned long rtcm=0;

	 /* get index of first byte in sequenece */
	 byte=start_bit/6;
	 /*get next 5 bytes */
	 for(i=0;i<5;i++)
		rtcm|=((long)(rtcm_in[i+byte])<<(26-6*i));
	 /* shift left */
	 rtcm=rtcm<<(start_bit%6);
	 /* shift right */
	 rtcm=(rtcm>>(32-num_bits));
	 return (unsigned int)rtcm;
	}


