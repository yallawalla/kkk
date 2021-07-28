char *RTCMDec(char *buff);
/* RTCM definitions */
#define MAXINLEN 400
#define MAXOUTLEN 500
#define MAXDECLEN 140
/* error definitions*/
#define ERR_HEADER 1
#define ERR_CRC 2
#define ERR_LEN 3
#define ERR_PREAMBLE 4
#define ERR_INSTR 5
/* index definitions for rtcm_dec */
#define PREAMBLE 0
#define MESSAGETYPE 1
#define STATIONID 2
#define MODZCOUNT 3
#define SEQUENCE 4
#define FRAMELEN 5
#define HEALTH 6
#define DATALEN 7
#define DATA 10
#define UDRE 0
#define RANGECORR 1
#define RATECORR 2
#define IOD 3
/* bit sizes and startbits for rtcm parameters */
#define PREAMBLE_START 0
#define PREAMBLE_LEN 8
#define MESSAGETYPE_START 8
#define MESSAGETYPE_LEN 6
#define STATIONID_START 14
#define STATIONID_LEN 10
#define MODZCOUNT_START 24
#define MODZCOUNT_LEN 13
#define SEQUENCE_START 37
#define SEQUENCE_LEN 3
#define FRAMELEN_START 40
#define FRAMELEN_LEN 5
#define HEALTH_START 45
#define HEALTH_LEN 3
#define UDRE_START 48
#define UDRE_LEN 8
#define RANGECORR_START 56
#define RANGECORR_LEN 16
#define RATECORR_START 72
#define RATECORR_LEN 8
#define IOD_START 80
#define IOD_LEN 8
#define RTCM_SIZE 40


int RTCMDecode(char *rtcm_in,unsigned int *rtcm_dec);
int RTCMReverseBits(int rtcm);
int RTCMEncode(unsigned int *rtcm_dec,char *rtcm_out);
unsigned int RTCMGetData(char *rtcm_in,int start_bit,int num_bits);


