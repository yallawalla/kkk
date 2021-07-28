/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
#define M_ITEMS 16
#define M_FUN 16
struct fmenu {char opt; char level; int n; int(***f)(void *farg); void ***arg;};
typedef struct fmenu menu;
#include "remmenu.inc"
#include "remplus.h"
#include "remdspio.h"
double  pitch;
double  roll;
int         maxmils=6400 /* initial mils scale */;
int   corr=1   /* initial corr mode */;
double  p_null   /* pitch offset   */;
double  r_null   /* roll offset   */;
double  p_scale =1e-6;
double  r_scale =1e-6;
double  cpr    /* pitch/roll align  */;
double  crp;
int   el_null   /* RLD alignment/elev. */;
double  Pscale          /* pitch    */;
double  Pnull         /* coeff.    */;
double  Rscale          /* roll     */;
double  Rnull         /* coeff.    */;
int   Tiltmode        /* Temp coeff select */;
int   pwoff           /* poweroff delay  */;
int         track_delay=60 /* predict. interval */;
int   GPS_dt          /* GMT offset, hours */;
int   GPS_pdop=6  /* GPS pdop    */;
int   dGPS   /* dGPS rate   */;
int   tn_max=10390 /* scan values at -20 */;
int   tn_min=6814  /*   ...   at  55 */;
double  t_ref=2.35e+5 /* ref. temp -> tilt */;
int   DSPgain[4]={2,2,2,5};
int   key_delay=500 /* gain values, tx delay*/;
int   az_align  /* RLD alignment/azimuth*/;
int   el_align  /* RLD alignment/elevat.*/;
double     az_off   /* az. offset   */;
int   GPS_alt = 331;
long  GPS_lat =46L*3600L + 5L*60L;
long  GPS_lon =14L*3600L + 29L*60L;
zone  *Zones,*ZoneId;
int   ZoneUndef,SouthFlag;
char  userpass[] ="        ";
char  syspass[] ="        ";
char  ser_no[] ="        ";
int   heat_temp;
int   shaft;
int   meteo[8][8];
int   meteo_A = 1;
int   diff_GPS = 1;
int   GPS_datum=108 /* GPS map datum def.SLO*/;
int   GPS_geo   /* GPS geoid model  */;
int   GPS_dx;
int   GPS_dy;
int   GPS_dz;
int   C100_decl   /* magn. declin.  */;
double  aC100;
double  bC100;
double  cC100;
int   CM_off  =-183 /* grid format UTM   */;
int   CM_sect  =6  /*      */;
long  FalseEast =500000L;
long  FalseNorth =10000000L;
int   rld_mode   /* general rld port mode*/;
int   aux_mode    =1  /* default JW   */;
int   lcd   =50;
int   swvers;
int   Backlit;
int   coordtx;
int   language;
int   RightToLeft;
int         d_mode  =EL_AZ_R    /* GLOBALS  */;
int   edit_flag =eof;
char        time_mode =HOUR_MIN;
int   Tmax  =55.0  /* max.temp */;
int   Tmin  =-20.0  /*  min.temp */;
int   shaft_data[2];
double      maxgrid     =1e8  /* 9 digits */;
double  SHtoR;
queue       *PIT_task;
queue       *RTC_task;
int   wait_count =50;
menu  *curr_menu;;
int   Key,PushKey,PushCount;
char        *error_screen[4],SophieBuff[50];;
char  LcdBuffer[82],LcdBackup[82],LcdExt[32];
double      az_offset;
int   RTCMSize;
long  temp;
char  *library,*fonts,*syspar,eerror;
long  eebott,eetop;
lib         *LL,*Ln,*Lc,*Cn,*Sn,*NAV;
lib   *Lr,*LD,*LPP,*bll,*blll,*crest;
ltype      *LTYPES,*STYPES;
lib         *OBP,*ORG,*NORTH;
command  *CP,*CPM,*GUN,*SYS,*METEO,*LINK,*CPEXE;
int   CountMode;;
RLD         rld;
GPS         gps;
gun   *Guns;
int   lmenu_key;
mscan  *Mscan;
double  GPSn_avrg;
double  GPSdx_avrg;
double  GPSdy_avrg;
double  GPSdz_avrg;
char  RTCMLog[256];
COMPASS     c100;
TCM2  tcm2;
char  JwTx[16],JwRx[16],*Jw,Ibit,JwStat;;
int   JwCount,JwCheck,JwAZ,JwEL,JwR;;
char  pass[] ="        ";
int   r_filt[8],p_filt[8];
int   r_off[8],p_off[8];
int   PITcnt;
rtime  Time;
double  Tamb=25;
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int smenu(void *);
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int EraseAll(void *);
int ret(void *);
int EraseSingle(void *);
int ret(void *);
int ret(void *);

 int (*froot_02_00[])(void *)=
{EraseAll,ret,NULL};

void *aroot_02_00[]=
{NULL,(void *)1};

 int (*froot_02_01[])(void *)=
{EraseSingle,ret,NULL};

void *aroot_02_01[]=
{NULL,(void *)1};

 int (*froot_02_03[])(void *)=
{ret,NULL};

void *aroot_02_03[]=
{(void *)1};

 int(**f_root_02[])(void *)=
{froot_02_00,froot_02_01,NULL,froot_02_03};

 void **a_root_02[]=
{aroot_02_00,aroot_02_01,NULL,aroot_02_03};

menu root_02 = {0,0,0,f_root_02,a_root_02};
int SaveEE(void *);
int ret(void *);
int LoadEE(void *);
int ret(void *);

 int (*froot_03_00[])(void *)=
{SaveEE,ret,NULL};

void *aroot_03_00[]=
{NULL,(void *)eof};

 int (*froot_03_03[])(void *)=
{LoadEE,ret,NULL};

void *aroot_03_03[]=
{NULL,(void *)eof};

 int(**f_root_03[])(void *)=
{froot_03_00,NULL,NULL,froot_03_03};

 void **a_root_03[]=
{aroot_03_00,NULL,NULL,aroot_03_03};

menu root_03 = {0,0,1,f_root_03,a_root_03};
int InsertLib(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);
int EditX(void *);
int s_menu_hold(void *);
int s_menu_hold(void *);
int InitObject(void *);

 int (*froot_01_00[])(void *)=
{InsertLib,NULL};

void *aroot_01_00[]=
{NULL};

 int (*froot_01_01[])(void *)=
{smenu,NULL};

void *aroot_01_01[]=
{&root_02};

 int (*froot_01_03[])(void *)=
{smenu,NULL};

void *aroot_01_03[]=
{&root_03};

 int (*froot_01_04[])(void *)=
{NextList,NULL};

void *aroot_01_04[]=
{(void *)&Ln};

 int (*froot_01_05[])(void *)=
{LastList,NULL};

void *aroot_01_05[]=
{(void *)&Ln};

 int (*froot_01_06[])(void *)=
{LastType,NULL};

void *aroot_01_06[]=
{(void *)&Ln};

 int (*froot_01_07[])(void *)=
{NextType,NULL};

void *aroot_01_07[]=
{(void *)&Ln};

 int (*froot_01_08[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_01_08[]=
{NULL,(void *)edit_x};

 int (*froot_01_09[])(void *)=
{s_menu_hold,NULL};

void *aroot_01_09[]=
{(void *)mode};

 int (*froot_01_10[])(void *)=
{InitObject,NULL};

void *aroot_01_10[]=
{NULL};

 int(**f_root_01[])(void *)=
{froot_01_00,froot_01_01,NULL,froot_01_03,froot_01_04,froot_01_05,froot_01_06,froot_01_07,froot_01_08,froot_01_09,froot_01_10};

 void **a_root_01[]=
{aroot_01_00,aroot_01_01,NULL,aroot_01_03,aroot_01_04,aroot_01_05,aroot_01_06,aroot_01_07,aroot_01_08,aroot_01_09,aroot_01_10};

menu root_01 = {0,0,2,f_root_01,a_root_01};
int toLL(void *);
int ret(void *);
int toLL(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);

 int (*froot_06_00[])(void *)=
{toLL,ret,NULL};

void *aroot_06_00[]=
{(void *)&Ln,(void *)1};

 int (*froot_06_03[])(void *)=
{toLL,ret,NULL};

void *aroot_06_03[]=
{(void *)&OBP,(void *)1};

 int (*froot_06_04[])(void *)=
{NextList,NULL};

void *aroot_06_04[]=
{(void *)&Ln};

 int (*froot_06_05[])(void *)=
{LastList,NULL};

void *aroot_06_05[]=
{(void *)&Ln};

 int (*froot_06_06[])(void *)=
{LastType,NULL};

void *aroot_06_06[]=
{(void *)&Ln};

 int (*froot_06_07[])(void *)=
{NextType,NULL};

void *aroot_06_07[]=
{(void *)&Ln};

 int(**f_root_06[])(void *)=
{froot_06_00,NULL,NULL,froot_06_03,froot_06_04,froot_06_05,froot_06_06,froot_06_07};

 void **a_root_06[]=
{aroot_06_00,NULL,NULL,aroot_06_03,aroot_06_04,aroot_06_05,aroot_06_06,aroot_06_07};

menu root_06 = {0,0,3,f_root_06,a_root_06};
int GpsFixClose(void *);
int RLD_enable(void *);
int ret(void *);
int CheckImpl(void *);
int UpdateRtcm(void *);
int CheckPass(void *);
int AlignEcef(void *);
int GpsFixClose(void *);
int RLD_enable(void *);
int MakePos(void *);
int ret(void *);
int GpsFixMode(void *);
int SetGpsRef(void *);
int CheckPass(void *);
int GpsStatus(void *);

 int (*froot_07_00[])(void *)=
{GpsFixClose,RLD_enable,ret,NULL};

void *aroot_07_00[]=
{NULL,NULL,(void *)1};

 int (*froot_07_01[])(void *)=
{CheckImpl,UpdateRtcm,NULL};

void *aroot_07_01[]=
{(void *)HW_DGPS,NULL};

 int (*froot_07_02[])(void *)=
{CheckPass,AlignEcef,NULL};

void *aroot_07_02[]=
{(void *)&syspass,NULL};

 int (*froot_07_03[])(void *)=
{GpsFixClose,RLD_enable,MakePos,ret,NULL};

void *aroot_07_03[]=
{NULL,NULL,NULL,(void *)1};

 int (*froot_07_04[])(void *)=
{GpsFixMode,NULL};

void *aroot_07_04[]=
{NULL};

 int (*froot_07_05[])(void *)=
{SetGpsRef,NULL};

void *aroot_07_05[]=
{NULL};

 int (*froot_07_06[])(void *)=
{CheckPass,GpsStatus,NULL};

void *aroot_07_06[]=
{(void *)&syspass,NULL};

 int(**f_root_07[])(void *)=
{froot_07_00,froot_07_01,froot_07_02,froot_07_03,froot_07_04,froot_07_05,froot_07_06};

 void **a_root_07[]=
{aroot_07_00,aroot_07_01,aroot_07_02,aroot_07_03,aroot_07_04,aroot_07_05,aroot_07_06};

menu root_07 = {0,0,4,f_root_07,a_root_07};
int toLL(void *);
int CheckImpl(void *);
int GpsFixInit(void *);
int RLD_disable(void *);
int UpdatePost(void *);
int SaveEE(void *);
int close_srv(void *);
int ret(void *);
int LoadEE(void *);
int close_srv(void *);
int ret(void *);
int last_xyz(void *);
int next_xyz(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_05_00[])(void *)=
{toLL,smenu,NULL};

void *aroot_05_00[]=
{(void *)&Ln,&root_06};

 int (*froot_05_01[])(void *)=
{CheckImpl,GpsFixInit,RLD_disable,smenu,NULL};

void *aroot_05_01[]=
{(void *)HW_GPS,NULL,NULL,&root_07};

 int (*froot_05_02[])(void *)=
{UpdatePost,SaveEE,close_srv,ret,NULL};

void *aroot_05_02[]=
{NULL,NULL,NULL,(void *)eof};

 int (*froot_05_03[])(void *)=
{LoadEE,close_srv,ret,NULL};

void *aroot_05_03[]=
{NULL,NULL,(void *)eof};

 int (*froot_05_04[])(void *)=
{last_xyz,NULL};

void *aroot_05_04[]=
{NULL};

 int (*froot_05_05[])(void *)=
{next_xyz,NULL};

void *aroot_05_05[]=
{NULL};

 int (*froot_05_06[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_05_06[]=
{NULL,(void *)edit_x};

 int(**f_root_05[])(void *)=
{froot_05_00,froot_05_01,froot_05_02,froot_05_03,froot_05_04,froot_05_05,froot_05_06};

 void **a_root_05[]=
{aroot_05_00,aroot_05_01,aroot_05_02,aroot_05_03,aroot_05_04,aroot_05_05,aroot_05_06};

menu root_05 = {0,0,5,f_root_05,a_root_05};
int toLL(void *);
int ret(void *);
int MakeNorth(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);

 int (*froot_09_00[])(void *)=
{toLL,ret,NULL};

void *aroot_09_00[]=
{(void *)&Ln,(void *)1};

 int (*froot_09_03[])(void *)=
{MakeNorth,ret,NULL};

void *aroot_09_03[]=
{NULL,(void *)1};

 int (*froot_09_04[])(void *)=
{NextList,NULL};

void *aroot_09_04[]=
{(void *)&Ln};

 int (*froot_09_05[])(void *)=
{LastList,NULL};

void *aroot_09_05[]=
{(void *)&Ln};

 int (*froot_09_06[])(void *)=
{LastType,NULL};

void *aroot_09_06[]=
{(void *)&Ln};

 int (*froot_09_07[])(void *)=
{NextType,NULL};

void *aroot_09_07[]=
{(void *)&Ln};

 int(**f_root_09[])(void *)=
{froot_09_00,NULL,NULL,froot_09_03,froot_09_04,froot_09_05,froot_09_06,froot_09_07};

 void **a_root_09[]=
{aroot_09_00,NULL,NULL,aroot_09_03,aroot_09_04,aroot_09_05,aroot_09_06,aroot_09_07};

menu root_09 = {0,0,6,f_root_09,a_root_09};
int C100_disable(void *);
int RLD_enable(void *);
int ret(void *);
int C100_disable(void *);
int RLD_enable(void *);
int MakeNorth(void *);
int ret(void *);
int s_menu_hold(void *);
int CheckPass(void *);
int InitKVH(void *);

 int (*froot_10_00[])(void *)=
{C100_disable,RLD_enable,ret,NULL};

void *aroot_10_00[]=
{NULL,NULL,(void *)1};

 int (*froot_10_03[])(void *)=
{C100_disable,RLD_enable,MakeNorth,ret,NULL};

void *aroot_10_03[]=
{NULL,NULL,NULL,(void *)1};

 int (*froot_10_04[])(void *)=
{s_menu_hold,NULL};

void *aroot_10_04[]=
{(void *)cal};

 int (*froot_10_05[])(void *)=
{CheckPass,InitKVH,NULL};

void *aroot_10_05[]=
{(void *)&syspass,NULL};

 int(**f_root_10[])(void *)=
{froot_10_00,NULL,NULL,froot_10_03,froot_10_04,froot_10_05};

 void **a_root_10[]=
{aroot_10_00,NULL,NULL,aroot_10_03,aroot_10_04,aroot_10_05};

menu root_10 = {0,0,7,f_root_10,a_root_10};
int toLL(void *);
int C100_enable(void *);
int RLD_disable(void *);
int ComputeNorth(void *);
int UpdateNorth(void *);
int SaveEE(void *);
int close_srv(void *);
int ret(void *);
int LoadEE(void *);
int close_srv(void *);
int ret(void *);
int last_xyz(void *);
int next_xyz(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_08_00[])(void *)=
{toLL,smenu,NULL};

void *aroot_08_00[]=
{(void *)&Ln,&root_09};

 int (*froot_08_01[])(void *)=
{C100_enable,RLD_disable,smenu,NULL};

void *aroot_08_01[]=
{NULL,NULL,&root_10};

 int (*froot_08_02[])(void *)=
{ComputeNorth,UpdateNorth,SaveEE,close_srv,ret,NULL};

void *aroot_08_02[]=
{NULL,NULL,NULL,NULL,(void *)eof};

 int (*froot_08_03[])(void *)=
{LoadEE,close_srv,ret,NULL};

void *aroot_08_03[]=
{NULL,NULL,(void *)eof};

 int (*froot_08_04[])(void *)=
{last_xyz,NULL};

void *aroot_08_04[]=
{NULL};

 int (*froot_08_05[])(void *)=
{next_xyz,NULL};

void *aroot_08_05[]=
{NULL};

 int (*froot_08_06[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_08_06[]=
{NULL,(void *)edit_x};

 int(**f_root_08[])(void *)=
{froot_08_00,froot_08_01,froot_08_02,froot_08_03,froot_08_04,froot_08_05,froot_08_06};

 void **a_root_08[]=
{aroot_08_00,aroot_08_01,aroot_08_02,aroot_08_03,aroot_08_04,aroot_08_05,aroot_08_06};

menu root_08 = {0,0,8,f_root_08,a_root_08};
int LLtoSn(void *);
int ret(void *);
int SntoLL(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_13_00[])(void *)=
{LLtoSn,ret,NULL};

void *aroot_13_00[]=
{NULL,(void *)1};

 int (*froot_13_03[])(void *)=
{SntoLL,ret,NULL};

void *aroot_13_03[]=
{NULL,(void *)1};

 int (*froot_13_04[])(void *)=
{NextList,NULL};

void *aroot_13_04[]=
{(void *)&Ln};

 int (*froot_13_05[])(void *)=
{LastList,NULL};

void *aroot_13_05[]=
{(void *)&Ln};

 int (*froot_13_06[])(void *)=
{LastType,NULL};

void *aroot_13_06[]=
{(void *)&Ln};

 int (*froot_13_07[])(void *)=
{NextType,NULL};

void *aroot_13_07[]=
{(void *)&Ln};

 int (*froot_13_08[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_13_08[]=
{NULL,(void *)edit_x};

 int(**f_root_13[])(void *)=
{froot_13_00,NULL,NULL,froot_13_03,froot_13_04,froot_13_05,froot_13_06,froot_13_07,froot_13_08};

 void **a_root_13[]=
{aroot_13_00,NULL,NULL,aroot_13_03,aroot_13_04,aroot_13_05,aroot_13_06,aroot_13_07,aroot_13_08};

menu root_13 = {0,0,9,f_root_13,a_root_13};
int UpdatePost(void *);
int SaveEE(void *);
int close_srv(void *);
int ret(void *);
int LoadEE(void *);
int SntoLL(void *);
int ret(void *);

 int (*froot_14_00[])(void *)=
{UpdatePost,SaveEE,close_srv,ret,NULL};

void *aroot_14_00[]=
{NULL,NULL,NULL,(void *)eof};

 int (*froot_14_03[])(void *)=
{LoadEE,SntoLL,ret,NULL};

void *aroot_14_03[]=
{NULL,NULL,(void *)1};

 int(**f_root_14[])(void *)=
{froot_14_00,NULL,NULL,froot_14_03};

 void **a_root_14[]=
{aroot_14_00,NULL,NULL,aroot_14_03};

menu root_14 = {0,0,10,f_root_14,a_root_14};
int toLL(void *);
int LLtoSn(void *);
int traverse(void *);
int close_srv(void *);
int ret(void *);
int LLtoSn(void *);
int NextList(void *);
int LLtoSn(void *);
int LastList(void *);
int LLtoSn(void *);
int LastType(void *);
int LLtoSn(void *);
int NextType(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_12_00[])(void *)=
{toLL,smenu,NULL};

void *aroot_12_00[]=
{(void *)&Ln,&root_13};

 int (*froot_12_01[])(void *)=
{LLtoSn,traverse,smenu,NULL};

void *aroot_12_01[]=
{NULL,NULL,&root_14};

 int (*froot_12_03[])(void *)=
{close_srv,ret,NULL};

void *aroot_12_03[]=
{NULL,(void *)eof};

 int (*froot_12_04[])(void *)=
{LLtoSn,NextList,NULL};

void *aroot_12_04[]=
{NULL,(void *)&Sn};

 int (*froot_12_05[])(void *)=
{LLtoSn,LastList,NULL};

void *aroot_12_05[]=
{NULL,(void *)&Sn};

 int (*froot_12_06[])(void *)=
{LLtoSn,LastType,NULL};

void *aroot_12_06[]=
{NULL,(void *)&Sn};

 int (*froot_12_07[])(void *)=
{LLtoSn,NextType,NULL};

void *aroot_12_07[]=
{NULL,(void *)&Sn};

 int (*froot_12_08[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_12_08[]=
{NULL,(void *)edit_x};

 int(**f_root_12[])(void *)=
{froot_12_00,froot_12_01,NULL,froot_12_03,froot_12_04,froot_12_05,froot_12_06,froot_12_07,froot_12_08};

 void **a_root_12[]=
{aroot_12_00,aroot_12_01,NULL,aroot_12_03,aroot_12_04,aroot_12_05,aroot_12_06,aroot_12_07,aroot_12_08};

menu root_12 = {0,0,11,f_root_12,a_root_12};
int LLtoSn(void *);
int ret(void *);
int SntoLL(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_16_00[])(void *)=
{LLtoSn,ret,NULL};

void *aroot_16_00[]=
{NULL,(void *)1};

 int (*froot_16_03[])(void *)=
{SntoLL,ret,NULL};

void *aroot_16_03[]=
{NULL,(void *)1};

 int (*froot_16_04[])(void *)=
{NextList,NULL};

void *aroot_16_04[]=
{(void *)&Ln};

 int (*froot_16_05[])(void *)=
{LastList,NULL};

void *aroot_16_05[]=
{(void *)&Ln};

 int (*froot_16_06[])(void *)=
{LastType,NULL};

void *aroot_16_06[]=
{(void *)&Ln};

 int (*froot_16_07[])(void *)=
{NextType,NULL};

void *aroot_16_07[]=
{(void *)&Ln};

 int (*froot_16_08[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_16_08[]=
{NULL,(void *)edit_x};

 int(**f_root_16[])(void *)=
{froot_16_00,NULL,NULL,froot_16_03,froot_16_04,froot_16_05,froot_16_06,froot_16_07,froot_16_08};

 void **a_root_16[]=
{aroot_16_00,NULL,NULL,aroot_16_03,aroot_16_04,aroot_16_05,aroot_16_06,aroot_16_07,aroot_16_08};

menu root_16 = {0,0,12,f_root_16,a_root_16};
int UpdatePost(void *);
int UpdateNorth(void *);
int SaveEE(void *);
int close_srv(void *);
int ret(void *);
int LoadEE(void *);
int SntoLL(void *);
int ret(void *);

 int (*froot_17_00[])(void *)=
{UpdatePost,UpdateNorth,SaveEE,close_srv,ret,NULL};

void *aroot_17_00[]=
{NULL,NULL,NULL,NULL,(void *)eof};

 int (*froot_17_03[])(void *)=
{LoadEE,SntoLL,ret,NULL};

void *aroot_17_03[]=
{NULL,NULL,(void *)1};

 int(**f_root_17[])(void *)=
{froot_17_00,NULL,NULL,froot_17_03};

 void **a_root_17[]=
{aroot_17_00,NULL,NULL,aroot_17_03};

menu root_17 = {0,0,13,f_root_17,a_root_17};
int toLL(void *);
int LLtoSn(void *);
int trilaterate(void *);
int close_srv(void *);
int ret(void *);
int LLtoSn(void *);
int NextList(void *);
int LLtoSn(void *);
int LastList(void *);
int LLtoSn(void *);
int LastType(void *);
int LLtoSn(void *);
int NextType(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_15_00[])(void *)=
{toLL,smenu,NULL};

void *aroot_15_00[]=
{(void *)&Ln,&root_16};

 int (*froot_15_01[])(void *)=
{LLtoSn,trilaterate,smenu,NULL};

void *aroot_15_01[]=
{NULL,NULL,&root_17};

 int (*froot_15_03[])(void *)=
{close_srv,ret,NULL};

void *aroot_15_03[]=
{NULL,(void *)eof};

 int (*froot_15_04[])(void *)=
{LLtoSn,NextList,NULL};

void *aroot_15_04[]=
{NULL,(void *)&Sn};

 int (*froot_15_05[])(void *)=
{LLtoSn,LastList,NULL};

void *aroot_15_05[]=
{NULL,(void *)&Sn};

 int (*froot_15_06[])(void *)=
{LLtoSn,LastType,NULL};

void *aroot_15_06[]=
{NULL,(void *)&Sn};

 int (*froot_15_07[])(void *)=
{LLtoSn,NextType,NULL};

void *aroot_15_07[]=
{NULL,(void *)&Sn};

 int (*froot_15_08[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_15_08[]=
{NULL,(void *)edit_x};

 int(**f_root_15[])(void *)=
{froot_15_00,froot_15_01,NULL,froot_15_03,froot_15_04,froot_15_05,froot_15_06,froot_15_07,froot_15_08};

 void **a_root_15[]=
{aroot_15_00,aroot_15_01,NULL,aroot_15_03,aroot_15_04,aroot_15_05,aroot_15_06,aroot_15_07,aroot_15_08};

menu root_15 = {0,0,14,f_root_15,a_root_15};
int LLtoSn(void *);
int ret(void *);
int SntoLL(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_19_00[])(void *)=
{LLtoSn,ret,NULL};

void *aroot_19_00[]=
{NULL,(void *)1};

 int (*froot_19_03[])(void *)=
{SntoLL,ret,NULL};

void *aroot_19_03[]=
{NULL,(void *)1};

 int (*froot_19_04[])(void *)=
{NextList,NULL};

void *aroot_19_04[]=
{(void *)&Ln};

 int (*froot_19_05[])(void *)=
{LastList,NULL};

void *aroot_19_05[]=
{(void *)&Ln};

 int (*froot_19_06[])(void *)=
{LastType,NULL};

void *aroot_19_06[]=
{(void *)&Ln};

 int (*froot_19_07[])(void *)=
{NextType,NULL};

void *aroot_19_07[]=
{(void *)&Ln};

 int (*froot_19_08[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_19_08[]=
{NULL,(void *)edit_x};

 int(**f_root_19[])(void *)=
{froot_19_00,NULL,NULL,froot_19_03,froot_19_04,froot_19_05,froot_19_06,froot_19_07,froot_19_08};

 void **a_root_19[]=
{aroot_19_00,NULL,NULL,aroot_19_03,aroot_19_04,aroot_19_05,aroot_19_06,aroot_19_07,aroot_19_08};

menu root_19 = {0,0,15,f_root_19,a_root_19};
int UpdatePost(void *);
int UpdateNorth(void *);
int SaveEE(void *);
int close_srv(void *);
int ret(void *);
int LoadEE(void *);
int SntoLL(void *);
int ret(void *);

 int (*froot_20_00[])(void *)=
{UpdatePost,UpdateNorth,SaveEE,close_srv,ret,NULL};

void *aroot_20_00[]=
{NULL,NULL,NULL,NULL,(void *)eof};

 int (*froot_20_03[])(void *)=
{LoadEE,SntoLL,ret,NULL};

void *aroot_20_03[]=
{NULL,NULL,(void *)1};

 int(**f_root_20[])(void *)=
{froot_20_00,NULL,NULL,froot_20_03};

 void **a_root_20[]=
{aroot_20_00,NULL,NULL,aroot_20_03};

menu root_20 = {0,0,16,f_root_20,a_root_20};
int toLL(void *);
int LLtoSn(void *);
int triangulate(void *);
int close_srv(void *);
int ret(void *);
int LLtoSn(void *);
int NextList(void *);
int LLtoSn(void *);
int LastList(void *);
int LLtoSn(void *);
int LastType(void *);
int LLtoSn(void *);
int NextType(void *);
int EditX(void *);
int s_menu_hold(void *);

 int (*froot_18_00[])(void *)=
{toLL,smenu,NULL};

void *aroot_18_00[]=
{(void *)&Ln,&root_19};

 int (*froot_18_01[])(void *)=
{LLtoSn,triangulate,smenu,NULL};

void *aroot_18_01[]=
{NULL,NULL,&root_20};

 int (*froot_18_03[])(void *)=
{close_srv,ret,NULL};

void *aroot_18_03[]=
{NULL,(void *)eof};

 int (*froot_18_04[])(void *)=
{LLtoSn,NextList,NULL};

void *aroot_18_04[]=
{NULL,(void *)&Sn};

 int (*froot_18_05[])(void *)=
{LLtoSn,LastList,NULL};

void *aroot_18_05[]=
{NULL,(void *)&Sn};

 int (*froot_18_06[])(void *)=
{LLtoSn,LastType,NULL};

void *aroot_18_06[]=
{NULL,(void *)&Sn};

 int (*froot_18_07[])(void *)=
{LLtoSn,NextType,NULL};

void *aroot_18_07[]=
{NULL,(void *)&Sn};

 int (*froot_18_08[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_18_08[]=
{NULL,(void *)edit_x};

 int(**f_root_18[])(void *)=
{froot_18_00,froot_18_01,NULL,froot_18_03,froot_18_04,froot_18_05,froot_18_06,froot_18_07,froot_18_08};

 void **a_root_18[]=
{aroot_18_00,aroot_18_01,NULL,aroot_18_03,aroot_18_04,aroot_18_05,aroot_18_06,aroot_18_07,aroot_18_08};

menu root_18 = {0,0,17,f_root_18,a_root_18};
int MakeTraverse(void *);
int MakeTrilat(void *);
int MakeTriang(void *);
int close_srv(void *);
int ret(void *);

 int (*froot_11_00[])(void *)=
{MakeTraverse,smenu,NULL};

void *aroot_11_00[]=
{NULL,&root_12};

 int (*froot_11_01[])(void *)=
{MakeTrilat,smenu,NULL};

void *aroot_11_01[]=
{NULL,&root_15};

 int (*froot_11_02[])(void *)=
{MakeTriang,smenu,NULL};

void *aroot_11_02[]=
{NULL,&root_18};

 int (*froot_11_03[])(void *)=
{close_srv,ret,NULL};

void *aroot_11_03[]=
{NULL,(void *)eof};

 int(**f_root_11[])(void *)=
{froot_11_00,froot_11_01,froot_11_02,froot_11_03};

 void **a_root_11[]=
{aroot_11_00,aroot_11_01,aroot_11_02,aroot_11_03};

menu root_11 = {0,0,18,f_root_11,a_root_11};
int MakePos(void *);
int MakeNorth(void *);
int ret(void *);

 int (*froot_04_00[])(void *)=
{MakePos,smenu,NULL};

void *aroot_04_00[]=
{NULL,&root_05};

 int (*froot_04_01[])(void *)=
{MakeNorth,smenu,NULL};

void *aroot_04_01[]=
{NULL,&root_08};

 int (*froot_04_02[])(void *)=
{smenu,NULL};

void *aroot_04_02[]=
{&root_11};

 int (*froot_04_03[])(void *)=
{ret,NULL};

void *aroot_04_03[]=
{(void *)eof};

 int(**f_root_04[])(void *)=
{froot_04_00,froot_04_01,froot_04_02,froot_04_03};

 void **a_root_04[]=
{aroot_04_00,aroot_04_01,aroot_04_02,aroot_04_03};

menu root_04 = {0,0,19,f_root_04,a_root_04};
int SaveEE(void *);
int ret(void *);
int LoadEE(void *);
int ret(void *);

 int (*froot_22_00[])(void *)=
{SaveEE,ret,NULL};

void *aroot_22_00[]=
{NULL,(void *)2};

 int (*froot_22_03[])(void *)=
{LoadEE,ret,NULL};

void *aroot_22_03[]=
{NULL,(void *)2};

 int(**f_root_22[])(void *)=
{froot_22_00,NULL,NULL,froot_22_03};

 void **a_root_22[]=
{aroot_22_00,NULL,NULL,aroot_22_03};

menu root_22 = {0,0,20,f_root_22,a_root_22};
int EnableLevel(void *);
int CloseLevel(void *);
int ret(void *);
int DisableLevel(void *);
int CloseLevel(void *);
int ret(void *);
int CloseLevel(void *);
int shutdown_load(void *);
int ret(void *);
int CheckPass(void *);
int ResetLevel(void *);

 int (*froot_23_00[])(void *)=
{EnableLevel,CloseLevel,ret,NULL};

void *aroot_23_00[]=
{NULL,NULL,(void *)2};

 int (*froot_23_01[])(void *)=
{DisableLevel,CloseLevel,ret,NULL};

void *aroot_23_01[]=
{NULL,NULL,(void *)2};

 int (*froot_23_03[])(void *)=
{CloseLevel,shutdown_load,ret,NULL};

void *aroot_23_03[]=
{NULL,NULL,(void *)2};

 int (*froot_23_04[])(void *)=
{CheckPass,ResetLevel,NULL};

void *aroot_23_04[]=
{(void *)&syspass,NULL};

 int(**f_root_23[])(void *)=
{froot_23_00,froot_23_01,NULL,froot_23_03,froot_23_04};

 void **a_root_23[]=
{aroot_23_00,aroot_23_01,NULL,aroot_23_03,aroot_23_04};

menu root_23 = {0,0,21,f_root_23,a_root_23};
int CallGun(void *);
int list_on(void *);
int Ack(void *);
int SaveEE(void *);
int ret(void *);
int LoadEE(void *);
int ret(void *);
int next_status(void *);
int list_on(void *);
int last_status(void *);
int list_on(void *);
int NextList(void *);
int list_on(void *);
int LastList(void *);
int list_on(void *);
int comm_enable(void *);
int RLD_enable(void *);
int l_menu_hold(void *);
int list_on(void *);
int RLD_disable(void *);
int list_on(void *);

 int (*froot_24_00[])(void *)=
{CallGun,list_on,NULL};

void *aroot_24_00[]=
{(void *)&GUN,(void *)&GUN};

 int (*froot_24_01[])(void *)=
{Ack,NULL};

void *aroot_24_01[]=
{(void *)&GUN};

 int (*froot_24_02[])(void *)=
{SaveEE,ret,NULL};

void *aroot_24_02[]=
{NULL,(void *)2};

 int (*froot_24_03[])(void *)=
{LoadEE,ret,NULL};

void *aroot_24_03[]=
{NULL,(void *)2};

 int (*froot_24_04[])(void *)=
{next_status,list_on,NULL};

void *aroot_24_04[]=
{(void *)&GUN,(void *)&GUN};

 int (*froot_24_05[])(void *)=
{last_status,list_on,NULL};

void *aroot_24_05[]=
{(void *)&GUN,(void *)&GUN};

 int (*froot_24_06[])(void *)=
{NextList,list_on,NULL};

void *aroot_24_06[]=
{(void *)&Cn,(void *)&GUN};

 int (*froot_24_07[])(void *)=
{LastList,list_on,NULL};

void *aroot_24_07[]=
{(void *)&Cn,(void *)&GUN};

 int (*froot_24_08[])(void *)=
{comm_enable,RLD_enable,l_menu_hold,list_on,RLD_disable,NULL};

void *aroot_24_08[]=
{(void *)&GUN,NULL,(void *)edit_gun,(void *)&GUN,NULL};

 int (*froot_24_09[])(void *)=
{list_on,NULL};

void *aroot_24_09[]=
{(void *)&GUN};

 int(**f_root_24[])(void *)=
{froot_24_00,froot_24_01,froot_24_02,froot_24_03,froot_24_04,froot_24_05,froot_24_06,froot_24_07,froot_24_08,froot_24_09};

 void **a_root_24[]=
{aroot_24_00,aroot_24_01,aroot_24_02,aroot_24_03,aroot_24_04,aroot_24_05,aroot_24_06,aroot_24_07,aroot_24_08,aroot_24_09};

menu root_24 = {0,0,22,f_root_24,a_root_24};
int UpdateMeteo(void *);
int SaveEE(void *);
int ret(void *);
int LoadEE(void *);
int ret(void *);
int comm_enable(void *);
int l_menu_hold(void *);
int InitMeteo(void *);

 int (*froot_25_00[])(void *)=
{UpdateMeteo,NULL};

void *aroot_25_00[]=
{NULL};

 int (*froot_25_02[])(void *)=
{SaveEE,ret,NULL};

void *aroot_25_02[]=
{NULL,(void *)2};

 int (*froot_25_03[])(void *)=
{LoadEE,ret,NULL};

void *aroot_25_03[]=
{NULL,(void *)2};

 int (*froot_25_04[])(void *)=
{comm_enable,l_menu_hold,NULL};

void *aroot_25_04[]=
{(void *)&METEO,(void *)set_meteo};

 int (*froot_25_05[])(void *)=
{InitMeteo,NULL};

void *aroot_25_05[]=
{NULL};

 int(**f_root_25[])(void *)=
{froot_25_00,NULL,froot_25_02,froot_25_03,froot_25_04,froot_25_05};

 void **a_root_25[]=
{aroot_25_00,NULL,aroot_25_02,aroot_25_03,aroot_25_04,aroot_25_05};

menu root_25 = {0,0,23,f_root_25,a_root_25};
int SaveEE(void *);
int ret(void *);
int LoadEE(void *);
int ret(void *);

 int (*froot_26_00[])(void *)=
{SaveEE,ret,NULL};

void *aroot_26_00[]=
{NULL,(void *)2};

 int (*froot_26_03[])(void *)=
{LoadEE,ret,NULL};

void *aroot_26_03[]=
{NULL,(void *)2};

 int(**f_root_26[])(void *)=
{froot_26_00,NULL,NULL,froot_26_03};

 void **a_root_26[]=
{aroot_26_00,NULL,NULL,aroot_26_03};

menu root_26 = {0,0,24,f_root_26,a_root_26};
int comm_enable(void *);
int l_menu_hold(void *);
int InitLevel(void *);
int CheckImpl(void *);
int list_on(void *);
int CheckImpl(void *);
int CheckPass(void *);
int comm_enable(void *);
int l_menu_hold(void *);
int CheckPass(void *);
int MenuView(void *);

 int (*froot_21_00[])(void *)=
{comm_enable,l_menu_hold,smenu,NULL};

void *aroot_21_00[]=
{(void *)&SYS,(void *)set_system,&root_22};

 int (*froot_21_01[])(void *)=
{InitLevel,smenu,NULL};

void *aroot_21_01[]=
{NULL,&root_23};

 int (*froot_21_02[])(void *)=
{CheckImpl,list_on,smenu,NULL};

void *aroot_21_02[]=
{(void *)SW_V1,(void *)&GUN,&root_24};

 int (*froot_21_03[])(void *)=
{CheckImpl,smenu,NULL};

void *aroot_21_03[]=
{(void *)SW_V1,&root_25};

 int (*froot_21_04[])(void *)=
{CheckPass,comm_enable,l_menu_hold,smenu,NULL};

void *aroot_21_04[]=
{(void *)&syspass,(void *)&LINK,(void *)SetupMenu,&root_26};

 int (*froot_21_05[])(void *)=
{CheckPass,MenuView,NULL};

void *aroot_21_05[]=
{(void *)&syspass,NULL};

 int(**f_root_21[])(void *)=
{froot_21_00,froot_21_01,froot_21_02,froot_21_03,froot_21_04,froot_21_05};

 void **a_root_21[]=
{aroot_21_00,aroot_21_01,aroot_21_02,aroot_21_03,aroot_21_04,aroot_21_05};

menu root_21 = {0,0,25,f_root_21,a_root_21};
int UpdateTarget(void *);
int TargetFromLL(void *);
int RLD_disable(void *);
int list_on(void *);
int ret(void *);
int list_on(void *);
int RLD_disable(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int InitTarget(void *);
int EditX(void *);
int s_menu_hold(void *);
int s_menu_hold(void *);
int SelectTargetType(void *);

 int (*froot_29_00[])(void *)=
{UpdateTarget,NULL};

void *aroot_29_00[]=
{(void *)&CP};

 int (*froot_29_02[])(void *)=
{TargetFromLL,RLD_disable,list_on,ret,NULL};

void *aroot_29_02[]=
{(void *)&CP,NULL,(void *)&CP,(void *)2};

 int (*froot_29_03[])(void *)=
{list_on,RLD_disable,ret,NULL};

void *aroot_29_03[]=
{(void *)&CP,NULL,(void *)2};

 int (*froot_29_04[])(void *)=
{NextList,NULL};

void *aroot_29_04[]=
{(void *)&Cn};

 int (*froot_29_05[])(void *)=
{LastList,NULL};

void *aroot_29_05[]=
{(void *)&Cn};

 int (*froot_29_06[])(void *)=
{InitTarget,NULL};

void *aroot_29_06[]=
{NULL};

 int (*froot_29_07[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_29_07[]=
{NULL,(void *)edit_x};

 int (*froot_29_08[])(void *)=
{s_menu_hold,NULL};

void *aroot_29_08[]=
{(void *)mode};

 int (*froot_29_09[])(void *)=
{SelectTargetType,NULL};

void *aroot_29_09[]=
{NULL};

 int(**f_root_29[])(void *)=
{froot_29_00,NULL,froot_29_02,froot_29_03,froot_29_04,froot_29_05,froot_29_06,froot_29_07,froot_29_08,froot_29_09};

 void **a_root_29[]=
{aroot_29_00,NULL,aroot_29_02,aroot_29_03,aroot_29_04,aroot_29_05,aroot_29_06,aroot_29_07,aroot_29_08,aroot_29_09};

menu root_29 = {0,0,26,f_root_29,a_root_29};
int SetDGrid(void *);
int ret(void *);
int SetDMuzz(void *);
int ret(void *);
int SaveCorr(void *);
int ret(void *);
int ret(void *);

 int (*froot_31_00[])(void *)=
{SetDGrid,ret,NULL};

void *aroot_31_00[]=
{NULL,(void *)1};

 int (*froot_31_01[])(void *)=
{SetDMuzz,ret,NULL};

void *aroot_31_01[]=
{NULL,(void *)1};

 int (*froot_31_02[])(void *)=
{SaveCorr,ret,NULL};

void *aroot_31_02[]=
{(void *)&CP,(void *)1};

 int (*froot_31_03[])(void *)=
{ret,NULL};

void *aroot_31_03[]=
{(void *)1};

 int(**f_root_31[])(void *)=
{froot_31_00,froot_31_01,froot_31_02,froot_31_03};

 void **a_root_31[]=
{aroot_31_00,aroot_31_01,aroot_31_02,aroot_31_03};

menu root_31 = {0,0,27,f_root_31,a_root_31};
int list_on(void *);
int RLD_disable(void *);
int PullMode(void *);
int ret(void *);
int CorrFromLL(void *);
int list_on(void *);
int RLD_disable(void *);
int PullMode(void *);
int ret(void *);
int list_on(void *);
int RLD_disable(void *);
int PullMode(void *);
int ret(void *);
int EditX(void *);
int s_menu_hold(void *);
int InitCorr(void *);
int s_menu_hold(void *);

 int (*froot_30_00[])(void *)=
{smenu,list_on,RLD_disable,PullMode,ret,NULL};

void *aroot_30_00[]=
{&root_31,(void *)&CP,NULL,NULL,(void *)2};

 int (*froot_30_02[])(void *)=
{CorrFromLL,list_on,RLD_disable,PullMode,ret,NULL};

void *aroot_30_02[]=
{(void *)&CP,(void *)&CP,NULL,NULL,(void *)2};

 int (*froot_30_03[])(void *)=
{list_on,RLD_disable,PullMode,ret,NULL};

void *aroot_30_03[]=
{(void *)&CP,NULL,NULL,(void *)2};

 int (*froot_30_04[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_30_04[]=
{NULL,(void *)edit_x};

 int (*froot_30_05[])(void *)=
{InitCorr,NULL};

void *aroot_30_05[]=
{NULL};

 int (*froot_30_06[])(void *)=
{s_menu_hold,NULL};

void *aroot_30_06[]=
{(void *)mode};

 int(**f_root_30[])(void *)=
{froot_30_00,NULL,froot_30_02,froot_30_03,froot_30_04,froot_30_05,froot_30_06};

 void **a_root_30[]=
{aroot_30_00,NULL,aroot_30_02,aroot_30_03,aroot_30_04,aroot_30_05,aroot_30_06};

menu root_30 = {0,0,28,f_root_30,a_root_30};
int UpdateUnit(void *);
int UnitFromLL(void *);
int list_on(void *);
int RLD_disable(void *);
int ret(void *);
int list_on(void *);
int RLD_disable(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int InitUnit(void *);
int EditX(void *);
int s_menu_hold(void *);
int s_menu_hold(void *);
int SelectUnitType(void *);

 int (*froot_32_00[])(void *)=
{UpdateUnit,NULL};

void *aroot_32_00[]=
{NULL};

 int (*froot_32_02[])(void *)=
{UnitFromLL,list_on,RLD_disable,ret,NULL};

void *aroot_32_02[]=
{(void *)&CP,(void *)&CP,NULL,(void *)2};

 int (*froot_32_03[])(void *)=
{list_on,RLD_disable,ret,NULL};

void *aroot_32_03[]=
{(void *)&CP,NULL,(void *)2};

 int (*froot_32_04[])(void *)=
{NextList,NULL};

void *aroot_32_04[]=
{(void *)&Cn};

 int (*froot_32_05[])(void *)=
{LastList,NULL};

void *aroot_32_05[]=
{(void *)&Cn};

 int (*froot_32_06[])(void *)=
{InitUnit,NULL};

void *aroot_32_06[]=
{NULL};

 int (*froot_32_07[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_32_07[]=
{NULL,(void *)edit_x};

 int (*froot_32_08[])(void *)=
{s_menu_hold,NULL};

void *aroot_32_08[]=
{(void *)mode};

 int (*froot_32_09[])(void *)=
{SelectUnitType,NULL};

void *aroot_32_09[]=
{NULL};

 int(**f_root_32[])(void *)=
{froot_32_00,NULL,froot_32_02,froot_32_03,froot_32_04,froot_32_05,froot_32_06,froot_32_07,froot_32_08,froot_32_09};

 void **a_root_32[]=
{aroot_32_00,NULL,aroot_32_02,aroot_32_03,aroot_32_04,aroot_32_05,aroot_32_06,aroot_32_07,aroot_32_08,aroot_32_09};

menu root_32 = {0,0,29,f_root_32,a_root_32};
int TargetToLL(void *);
int RLD_enable(void *);
int PushMode(void *);
int CorrToLL(void *);
int RLD_enable(void *);
int UnitToLL(void *);
int RLD_enable(void *);
int ret(void *);

 int (*froot_28_00[])(void *)=
{TargetToLL,RLD_enable,smenu,NULL};

void *aroot_28_00[]=
{(void *)&CP,NULL,&root_29};

 int (*froot_28_01[])(void *)=
{PushMode,CorrToLL,RLD_enable,smenu,NULL};

void *aroot_28_01[]=
{NULL,(void *)&CP,NULL,&root_30};

 int (*froot_28_02[])(void *)=
{UnitToLL,RLD_enable,smenu,NULL};

void *aroot_28_02[]=
{(void *)&CP,NULL,&root_32};

 int (*froot_28_03[])(void *)=
{ret,NULL};

void *aroot_28_03[]=
{(void *)1};

 int(**f_root_28[])(void *)=
{froot_28_00,froot_28_01,froot_28_02,froot_28_03};

 void **a_root_28[]=
{aroot_28_00,aroot_28_01,aroot_28_02,aroot_28_03};

menu root_28 = {0,0,30,f_root_28,a_root_28};
int Ack(void *);
int ret(void *);
int Nack(void *);
int ret(void *);
int ret(void *);

 int (*froot_34_00[])(void *)=
{Ack,ret,NULL};

void *aroot_34_00[]=
{(void *)&CP,(void *)2};

 int (*froot_34_01[])(void *)=
{Nack,ret,NULL};

void *aroot_34_01[]=
{(void *)&CP,(void *)2};

 int (*froot_34_03[])(void *)=
{ret,NULL};

void *aroot_34_03[]=
{(void *)2};

 int(**f_root_34[])(void *)=
{froot_34_00,froot_34_01,NULL,froot_34_03};

 void **a_root_34[]=
{aroot_34_00,aroot_34_01,NULL,aroot_34_03};

menu root_34 = {0,0,31,f_root_34,a_root_34};
int Call(void *);
int list_on(void *);
int ret(void *);
int CommandAbort(void *);
int list_on(void *);
int ret(void *);
int list_on(void *);
int ret(void *);

 int (*froot_33_00[])(void *)=
{Call,list_on,ret,NULL};

void *aroot_33_00[]=
{(void *)&CP,(void *)&CP,(void *)1};

 int (*froot_33_01[])(void *)=
{CommandAbort,list_on,ret,NULL};

void *aroot_33_01[]=
{(void *)&CP,(void *)&CP,(void *)1};

 int (*froot_33_02[])(void *)=
{smenu,list_on,NULL};

void *aroot_33_02[]=
{&root_34,(void *)&CP};

 int (*froot_33_03[])(void *)=
{ret,NULL};

void *aroot_33_03[]=
{(void *)1};

 int(**f_root_33[])(void *)=
{froot_33_00,froot_33_01,froot_33_02,froot_33_03};

 void **a_root_33[]=
{aroot_33_00,aroot_33_01,aroot_33_02,aroot_33_03};

menu root_33 = {0,0,32,f_root_33,a_root_33};
int Ack(void *);
int Nack(void *);
int list_on(void *);
int ret(void *);
int NextTarget(void *);
int DisplBallist(void *);
int LastTarget(void *);
int DisplBallist(void *);
int LastPredict(void *);
int CalcTarget(void *);
int DisplBallist(void *);
int NextPredict(void *);
int CalcTarget(void *);
int DisplBallist(void *);
int list_on(void *);
int CheckTarget(void *);
int CalcTarget(void *);
int DisplBallist(void *);
int SwitchPage(void *);

 int (*froot_35_00[])(void *)=
{Ack,NULL};

void *aroot_35_00[]=
{(void *)&CP};

 int (*froot_35_01[])(void *)=
{Nack,NULL};

void *aroot_35_01[]=
{(void *)&CP};

 int (*froot_35_03[])(void *)=
{list_on,ret,NULL};

void *aroot_35_03[]=
{(void *)&CP,(void *)1};

 int (*froot_35_04[])(void *)=
{NextTarget,DisplBallist,NULL};

void *aroot_35_04[]=
{(void *)&CP,(void *)&CP};

 int (*froot_35_05[])(void *)=
{LastTarget,DisplBallist,NULL};

void *aroot_35_05[]=
{(void *)&CP,(void *)&CP};

 int (*froot_35_06[])(void *)=
{LastPredict,CalcTarget,DisplBallist,NULL};

void *aroot_35_06[]=
{(void *)&CP,(void *)&CP,(void *)&CP};

 int (*froot_35_07[])(void *)=
{NextPredict,CalcTarget,DisplBallist,NULL};

void *aroot_35_07[]=
{(void *)&CP,(void *)&CP,(void *)&CP};

 int (*froot_35_08[])(void *)=
{list_on,CheckTarget,CalcTarget,DisplBallist,NULL};

void *aroot_35_08[]=
{(void *)&CP,(void *)&CP,(void *)&CP,(void *)&CP};

 int (*froot_35_09[])(void *)=
{SwitchPage,NULL};

void *aroot_35_09[]=
{NULL};

 int(**f_root_35[])(void *)=
{froot_35_00,froot_35_01,NULL,froot_35_03,froot_35_04,froot_35_05,froot_35_06,froot_35_07,froot_35_08,froot_35_09};

 void **a_root_35[]=
{aroot_35_00,aroot_35_01,NULL,aroot_35_03,aroot_35_04,aroot_35_05,aroot_35_06,aroot_35_07,aroot_35_08,aroot_35_09};

menu root_35 = {0,0,33,f_root_35,a_root_35};
int EraseCommand(void *);
int list_on(void *);
int ret(void *);
int EraseCommand(void *);
int list_on(void *);
int ret(void *);
int ret(void *);

 int (*froot_37_00[])(void *)=
{EraseCommand,list_on,ret,NULL};

void *aroot_37_00[]=
{NULL,(void *)&CP,(void *)2};

 int (*froot_37_01[])(void *)=
{EraseCommand,list_on,ret,NULL};

void *aroot_37_01[]=
{(void *)&CP,(void *)&CP,(void *)2};

 int (*froot_37_03[])(void *)=
{ret,NULL};

void *aroot_37_03[]=
{(void *)2};

 int(**f_root_37[])(void *)=
{froot_37_00,froot_37_01,NULL,froot_37_03};

 void **a_root_37[]=
{aroot_37_00,aroot_37_01,NULL,aroot_37_03};

menu root_37 = {0,0,34,f_root_37,a_root_37};
int CommandCopy(void *);
int list_on(void *);
int ret(void *);
int ret(void *);
int ret(void *);

 int (*froot_36_00[])(void *)=
{CommandCopy,list_on,ret,NULL};

void *aroot_36_00[]=
{(void *)&CP,(void *)&CP,(void *)1};

 int (*froot_36_01[])(void *)=
{smenu,NULL};

void *aroot_36_01[]=
{&root_37};

 int (*froot_36_03[])(void *)=
{ret,NULL};

void *aroot_36_03[]=
{(void *)1};

 int (*froot_36_04[])(void *)=
{ret,NULL};

void *aroot_36_04[]=
{(void *)1};

 int(**f_root_36[])(void *)=
{froot_36_00,froot_36_01,NULL,froot_36_03,froot_36_04};

 void **a_root_36[]=
{aroot_36_00,aroot_36_01,NULL,aroot_36_03,aroot_36_04};

menu root_36 = {0,0,35,f_root_36,a_root_36};
int list_on(void *);
int CheckTarget(void *);
int CalcTarget(void *);
int DisplBallist(void *);
int SaveEE(void *);
int RLD_enable(void *);
int ret(void *);
int CheckPass(void *);
int InitBallistic(void *);
int next_command(void *);
int list_on(void *);
int last_command(void *);
int list_on(void *);
int next_status(void *);
int list_on(void *);
int last_status(void *);
int list_on(void *);
int comm_enable(void *);
int l_menu_hold(void *);
int list_on(void *);
int list_on(void *);

 int (*froot_27_00[])(void *)=
{smenu,NULL};

void *aroot_27_00[]=
{&root_28};

 int (*froot_27_01[])(void *)=
{smenu,list_on,NULL};

void *aroot_27_01[]=
{&root_33,(void *)&CP};

 int (*froot_27_02[])(void *)=
{CheckTarget,CalcTarget,DisplBallist,smenu,NULL};

void *aroot_27_02[]=
{(void *)&CP,(void *)&CP,(void *)&CP,&root_35};

 int (*froot_27_03[])(void *)=
{SaveEE,RLD_enable,ret,NULL};

void *aroot_27_03[]=
{NULL,NULL,(void *)1};

 int (*froot_27_04[])(void *)=
{CheckPass,InitBallistic,NULL};

void *aroot_27_04[]=
{(void *)syspass,(void *)&CP};

 int (*froot_27_05[])(void *)=
{next_command,list_on,NULL};

void *aroot_27_05[]=
{(void *)&CP,(void *)&CP};

 int (*froot_27_06[])(void *)=
{last_command,list_on,NULL};

void *aroot_27_06[]=
{(void *)&CP,(void *)&CP};

 int (*froot_27_07[])(void *)=
{next_status,list_on,NULL};

void *aroot_27_07[]=
{(void *)&CP,(void *)&CP};

 int (*froot_27_08[])(void *)=
{last_status,list_on,NULL};

void *aroot_27_08[]=
{(void *)&CP,(void *)&CP};

 int (*froot_27_09[])(void *)=
{comm_enable,l_menu_hold,list_on,NULL};

void *aroot_27_09[]=
{(void *)&CP,(void *)Cedit,(void *)&CP};

 int (*froot_27_10[])(void *)=
{smenu,NULL};

void *aroot_27_10[]=
{&root_36};

 int (*froot_27_11[])(void *)=
{list_on,NULL};

void *aroot_27_11[]=
{(void *)&CP};

 int(**f_root_27[])(void *)=
{froot_27_00,froot_27_01,froot_27_02,froot_27_03,froot_27_04,froot_27_05,froot_27_06,froot_27_07,froot_27_08,froot_27_09,froot_27_10,froot_27_11};

 void **a_root_27[]=
{aroot_27_00,aroot_27_01,aroot_27_02,aroot_27_03,aroot_27_04,aroot_27_05,aroot_27_06,aroot_27_07,aroot_27_08,aroot_27_09,aroot_27_10,aroot_27_11};

menu root_27 = {0,0,36,f_root_27,a_root_27};
int library_on(void *);
int clear_screen(void *);
int PushMode(void *);
int clear_screen(void *);
int RLD_disable(void *);
int shutdown_save(void *);
int RLD_enable(void *);
int ret(void *);
int CheckImpl(void *);
int RLD_disable(void *);
int list_on(void *);
int EditX(void *);
int s_menu_hold(void *);
int s_menu_hold(void *);
int SelectLeft(void *);
int InitObject(void *);
int SelectRight(void *);
int NumObj(void *);
int CheckExe(void *);
int s_menu_hold(void *);
int CheckExe(void *);
int s_menu_hold(void *);

 int (*froot_00_00[])(void *)=
{library_on,smenu,NULL};

void *aroot_00_00[]=
{(void *)&Ln,&root_01};

 int (*froot_00_01[])(void *)=
{clear_screen,PushMode,smenu,NULL};

void *aroot_00_01[]=
{NULL,NULL,&root_04};

 int (*froot_00_02[])(void *)=
{clear_screen,RLD_disable,smenu,shutdown_save,RLD_enable,ret,NULL};

void *aroot_00_02[]=
{NULL,NULL,&root_21,NULL,NULL,(void *)1};

 int (*froot_00_03[])(void *)=
{CheckImpl,RLD_disable,list_on,smenu,NULL};

void *aroot_00_03[]=
{(void *)SW_V1,NULL,(void *)&CP,&root_27};

 int (*froot_00_04[])(void *)=
{EditX,s_menu_hold,NULL};

void *aroot_00_04[]=
{NULL,(void *)edit_x};

 int (*froot_00_05[])(void *)=
{s_menu_hold,NULL};

void *aroot_00_05[]=
{(void *)mode};

 int (*froot_00_06[])(void *)=
{SelectLeft,NULL};

void *aroot_00_06[]=
{NULL};

 int (*froot_00_07[])(void *)=
{InitObject,NULL};

void *aroot_00_07[]=
{NULL};

 int (*froot_00_08[])(void *)=
{SelectRight,NULL};

void *aroot_00_08[]=
{NULL};

 int (*froot_00_09[])(void *)=
{NumObj,NULL};

void *aroot_00_09[]=
{NULL};

 int (*froot_00_10[])(void *)=
{CheckExe,s_menu_hold,NULL};

void *aroot_00_10[]=
{(void *)&CP,(void *)dm_exe};

 int (*froot_00_11[])(void *)=
{CheckExe,s_menu_hold,NULL};

void *aroot_00_11[]=
{(void *)&GUN,(void *)dm_exe};

 int(**f_root_00[])(void *)=
{froot_00_00,froot_00_01,froot_00_02,froot_00_03,froot_00_04,froot_00_05,froot_00_06,froot_00_07,froot_00_08,froot_00_09,froot_00_10,froot_00_11};

 void **a_root_00[]=
{aroot_00_00,aroot_00_01,aroot_00_02,aroot_00_03,aroot_00_04,aroot_00_05,aroot_00_06,aroot_00_07,aroot_00_08,aroot_00_09,aroot_00_10,aroot_00_11};

menu root_00 = {0,0,37,f_root_00,a_root_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int Ack(void *);
int Nack(void *);
int list_on(void *);
int SaveEE(void *);
int RLD_enable(void *);
int ret(void *);
int SwitchPage(void *);
int list_on(void *);
int CheckTarget(void *);
int CalcTarget(void *);
int DisplBallist(void *);
int list_on(void *);
int DisplBallist(void *);
int list_on(void *);

 int (*fdm_exe_00_00[])(void *)=
{Ack,NULL};

void *adm_exe_00_00[]=
{NULL};

 int (*fdm_exe_00_01[])(void *)=
{Nack,NULL};

void *adm_exe_00_01[]=
{NULL};

 int (*fdm_exe_00_03[])(void *)=
{list_on,SaveEE,RLD_enable,ret,NULL};

void *adm_exe_00_03[]=
{(void *)&CP,NULL,NULL,(void *)1};

 int (*fdm_exe_00_04[])(void *)=
{SwitchPage,NULL};

void *adm_exe_00_04[]=
{NULL};

 int (*fdm_exe_00_05[])(void *)=
{list_on,CheckTarget,CalcTarget,DisplBallist,NULL};

void *adm_exe_00_05[]=
{(void *)&CP,(void *)&CP,(void *)&CP,(void *)&CP};

 int (*fdm_exe_00_06[])(void *)=
{list_on,DisplBallist,NULL};

void *adm_exe_00_06[]=
{(void *)&GUN,(void *)&GUN};

 int (*fdm_exe_00_07[])(void *)=
{list_on,NULL};

void *adm_exe_00_07[]=
{(void *)&CPEXE};

 int(**f_dm_exe_00[])(void *)=
{fdm_exe_00_00,fdm_exe_00_01,NULL,fdm_exe_00_03,fdm_exe_00_04,fdm_exe_00_05,fdm_exe_00_06,fdm_exe_00_07};

 void **a_dm_exe_00[]=
{adm_exe_00_00,adm_exe_00_01,NULL,adm_exe_00_03,adm_exe_00_04,adm_exe_00_05,adm_exe_00_06,adm_exe_00_07};

menu dm_exe_00 = {0,0,38,f_dm_exe_00,a_dm_exe_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int erase_xyz_s(void *);
int ret(void *);
int erase_xyz_all(void *);
int ret(void *);
int ret(void *);

 int (*fedit_x_01_00[])(void *)=
{erase_xyz_s,ret,NULL};

void *aedit_x_01_00[]=
{NULL,(void *)1};

 int (*fedit_x_01_01[])(void *)=
{erase_xyz_all,ret,NULL};

void *aedit_x_01_01[]=
{NULL,(void *)1};

 int (*fedit_x_01_03[])(void *)=
{ret,NULL};

void *aedit_x_01_03[]=
{(void *)1};

 int(**f_edit_x_01[])(void *)=
{fedit_x_01_00,fedit_x_01_01,NULL,fedit_x_01_03};

 void **a_edit_x_01[]=
{aedit_x_01_00,aedit_x_01_01,NULL,aedit_x_01_03};

menu edit_x_01 = {0,0,39,f_edit_x_01,a_edit_x_01};
int EditClose(void *);
int next_xyz(void *);
int EditX(void *);
int EditClose(void *);
int last_xyz(void *);
int EditX(void *);
int EditClose(void *);
int add_xyz(void *);
int EditX(void *);
int EditClose(void *);
int EditX(void *);
int EditY(void *);
int s_menu(void *);
int EditUtm(void *);
int s_menu(void *);
int EditClose(void *);
int EditTL(void *);
int s_menu(void *);
int EditClose(void *);
int s_menu_hold(void *);
int EditX(void *);
int EditRefresh(void *);
int EditClose(void *);
int ret(void *);
int EditX(void *);

 int (*fedit_x_00_00[])(void *)=
{EditClose,next_xyz,EditX,NULL};

void *aedit_x_00_00[]=
{NULL,NULL,NULL};

 int (*fedit_x_00_01[])(void *)=
{EditClose,last_xyz,EditX,NULL};

void *aedit_x_00_01[]=
{NULL,NULL,NULL};

 int (*fedit_x_00_02[])(void *)=
{EditClose,add_xyz,EditX,NULL};

void *aedit_x_00_02[]=
{NULL,NULL,NULL};

 int (*fedit_x_00_03[])(void *)=
{EditClose,smenu,EditX,NULL};

void *aedit_x_00_03[]=
{NULL,&edit_x_01,NULL};

 int (*fedit_x_00_04[])(void *)=
{EditY,s_menu,NULL};

void *aedit_x_00_04[]=
{NULL,(void *)edit_y};

 int (*fedit_x_00_05[])(void *)=
{EditUtm,s_menu,NULL};

void *aedit_x_00_05[]=
{NULL,(void *)edit_utm};

 int (*fedit_x_00_06[])(void *)=
{EditClose,EditTL,s_menu,NULL};

void *aedit_x_00_06[]=
{NULL,NULL,(void *)edit_time_l};

 int (*fedit_x_00_07[])(void *)=
{EditClose,s_menu_hold,EditX,NULL};

void *aedit_x_00_07[]=
{NULL,(void *)mode,NULL};

 int (*fedit_x_00_08[])(void *)=
{EditRefresh,NULL};

void *aedit_x_00_08[]=
{NULL};

 int (*fedit_x_00_09[])(void *)=
{EditClose,ret,NULL};

void *aedit_x_00_09[]=
{NULL,(void *)eof};

 int (*fedit_x_00_10[])(void *)=
{EditX,NULL};

void *aedit_x_00_10[]=
{NULL};

 int(**f_edit_x_00[])(void *)=
{fedit_x_00_00,fedit_x_00_01,fedit_x_00_02,fedit_x_00_03,fedit_x_00_04,fedit_x_00_05,fedit_x_00_06,fedit_x_00_07,fedit_x_00_08,fedit_x_00_09,fedit_x_00_10};

 void **a_edit_x_00[]=
{aedit_x_00_00,aedit_x_00_01,aedit_x_00_02,aedit_x_00_03,aedit_x_00_04,aedit_x_00_05,aedit_x_00_06,aedit_x_00_07,aedit_x_00_08,aedit_x_00_09,aedit_x_00_10};

menu edit_x_00 = {0,0,40,f_edit_x_00,a_edit_x_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int erase_xyz_s(void *);
int ret(void *);
int erase_xyz_all(void *);
int ret(void *);
int ret(void *);

 int (*fedit_y_01_00[])(void *)=
{erase_xyz_s,ret,NULL};

void *aedit_y_01_00[]=
{NULL,(void *)1};

 int (*fedit_y_01_01[])(void *)=
{erase_xyz_all,ret,NULL};

void *aedit_y_01_01[]=
{NULL,(void *)1};

 int (*fedit_y_01_03[])(void *)=
{ret,NULL};

void *aedit_y_01_03[]=
{(void *)1};

 int(**f_edit_y_01[])(void *)=
{fedit_y_01_00,fedit_y_01_01,NULL,fedit_y_01_03};

 void **a_edit_y_01[]=
{aedit_y_01_00,aedit_y_01_01,NULL,aedit_y_01_03};

menu edit_y_01 = {0,0,41,f_edit_y_01,a_edit_y_01};
int EditClose(void *);
int next_xyz(void *);
int EditY(void *);
int EditClose(void *);
int last_xyz(void *);
int EditY(void *);
int EditClose(void *);
int add_xyz(void *);
int EditY(void *);
int EditClose(void *);
int EditY(void *);
int EditX(void *);
int s_menu(void *);
int EditZ(void *);
int s_menu(void *);
int EditClose(void *);
int EditType(void *);
int s_menu(void *);
int EditClose(void *);
int s_menu_hold(void *);
int EditY(void *);
int EditRefresh(void *);
int EditClose(void *);
int ret(void *);
int EditY(void *);

 int (*fedit_y_00_00[])(void *)=
{EditClose,next_xyz,EditY,NULL};

void *aedit_y_00_00[]=
{NULL,NULL,NULL};

 int (*fedit_y_00_01[])(void *)=
{EditClose,last_xyz,EditY,NULL};

void *aedit_y_00_01[]=
{NULL,NULL,NULL};

 int (*fedit_y_00_02[])(void *)=
{EditClose,add_xyz,EditY,NULL};

void *aedit_y_00_02[]=
{NULL,NULL,NULL};

 int (*fedit_y_00_03[])(void *)=
{EditClose,smenu,EditY,NULL};

void *aedit_y_00_03[]=
{NULL,&edit_y_01,NULL};

 int (*fedit_y_00_04[])(void *)=
{EditX,s_menu,NULL};

void *aedit_y_00_04[]=
{NULL,(void *)edit_x};

 int (*fedit_y_00_05[])(void *)=
{EditZ,s_menu,NULL};

void *aedit_y_00_05[]=
{NULL,(void *)edit_z};

 int (*fedit_y_00_06[])(void *)=
{EditClose,EditType,s_menu,NULL};

void *aedit_y_00_06[]=
{NULL,NULL,(void *)edit_type};

 int (*fedit_y_00_07[])(void *)=
{EditClose,s_menu_hold,EditY,NULL};

void *aedit_y_00_07[]=
{NULL,(void *)mode,NULL};

 int (*fedit_y_00_08[])(void *)=
{EditRefresh,NULL};

void *aedit_y_00_08[]=
{NULL};

 int (*fedit_y_00_09[])(void *)=
{EditClose,ret,NULL};

void *aedit_y_00_09[]=
{NULL,(void *)eof};

 int (*fedit_y_00_10[])(void *)=
{EditY,NULL};

void *aedit_y_00_10[]=
{NULL};

 int(**f_edit_y_00[])(void *)=
{fedit_y_00_00,fedit_y_00_01,fedit_y_00_02,fedit_y_00_03,fedit_y_00_04,fedit_y_00_05,fedit_y_00_06,fedit_y_00_07,fedit_y_00_08,fedit_y_00_09,fedit_y_00_10};

 void **a_edit_y_00[]=
{aedit_y_00_00,aedit_y_00_01,aedit_y_00_02,aedit_y_00_03,aedit_y_00_04,aedit_y_00_05,aedit_y_00_06,aedit_y_00_07,aedit_y_00_08,aedit_y_00_09,aedit_y_00_10};

menu edit_y_00 = {0,0,42,f_edit_y_00,a_edit_y_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int erase_xyz_s(void *);
int ret(void *);
int erase_xyz_all(void *);
int ret(void *);
int ret(void *);

 int (*fedit_z_01_00[])(void *)=
{erase_xyz_s,ret,NULL};

void *aedit_z_01_00[]=
{NULL,(void *)1};

 int (*fedit_z_01_01[])(void *)=
{erase_xyz_all,ret,NULL};

void *aedit_z_01_01[]=
{NULL,(void *)1};

 int (*fedit_z_01_03[])(void *)=
{ret,NULL};

void *aedit_z_01_03[]=
{(void *)1};

 int(**f_edit_z_01[])(void *)=
{fedit_z_01_00,fedit_z_01_01,NULL,fedit_z_01_03};

 void **a_edit_z_01[]=
{aedit_z_01_00,aedit_z_01_01,NULL,aedit_z_01_03};

menu edit_z_01 = {0,0,43,f_edit_z_01,a_edit_z_01};
int EditClose(void *);
int next_xyz(void *);
int EditZ(void *);
int EditClose(void *);
int last_xyz(void *);
int EditZ(void *);
int EditClose(void *);
int add_xyz(void *);
int EditZ(void *);
int EditClose(void *);
int EditZ(void *);
int EditY(void *);
int s_menu(void *);
int EditClose(void *);
int EditTxt(void *);
int s_menu(void *);
int EditClose(void *);
int s_menu_hold(void *);
int EditZ(void *);
int EditRefresh(void *);
int EditClose(void *);
int ret(void *);
int EditZ(void *);

 int (*fedit_z_00_00[])(void *)=
{EditClose,next_xyz,EditZ,NULL};

void *aedit_z_00_00[]=
{NULL,NULL,NULL};

 int (*fedit_z_00_01[])(void *)=
{EditClose,last_xyz,EditZ,NULL};

void *aedit_z_00_01[]=
{NULL,NULL,NULL};

 int (*fedit_z_00_02[])(void *)=
{EditClose,add_xyz,EditZ,NULL};

void *aedit_z_00_02[]=
{NULL,NULL,NULL};

 int (*fedit_z_00_03[])(void *)=
{EditClose,smenu,EditZ,NULL};

void *aedit_z_00_03[]=
{NULL,&edit_z_01,NULL};

 int (*fedit_z_00_04[])(void *)=
{EditY,s_menu,NULL};

void *aedit_z_00_04[]=
{NULL,(void *)edit_y};

 int (*fedit_z_00_05[])(void *)=
{EditClose,EditTxt,s_menu,NULL};

void *aedit_z_00_05[]=
{NULL,NULL,(void *)edit_txt};

 int (*fedit_z_00_06[])(void *)=
{EditClose,s_menu_hold,EditZ,NULL};

void *aedit_z_00_06[]=
{NULL,(void *)mode,NULL};

 int (*fedit_z_00_07[])(void *)=
{EditRefresh,NULL};

void *aedit_z_00_07[]=
{NULL};

 int (*fedit_z_00_08[])(void *)=
{EditClose,ret,NULL};

void *aedit_z_00_08[]=
{NULL,(void *)eof};

 int (*fedit_z_00_09[])(void *)=
{EditZ,NULL};

void *aedit_z_00_09[]=
{NULL};

 int(**f_edit_z_00[])(void *)=
{fedit_z_00_00,fedit_z_00_01,fedit_z_00_02,fedit_z_00_03,fedit_z_00_04,fedit_z_00_05,fedit_z_00_06,fedit_z_00_07,fedit_z_00_08,fedit_z_00_09};

 void **a_edit_z_00[]=
{aedit_z_00_00,aedit_z_00_01,aedit_z_00_02,aedit_z_00_03,aedit_z_00_04,aedit_z_00_05,aedit_z_00_06,aedit_z_00_07,aedit_z_00_08,aedit_z_00_09};

menu edit_z_00 = {0,0,44,f_edit_z_00,a_edit_z_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int mode_time(void *);
int incr_time_l(void *);
int decr_time_l(void *);
int set_time(void *);
int EditType(void *);
int s_menu(void *);
int EditX(void *);
int s_menu(void *);
int EditTR(void *);
int s_menu(void *);
int s_menu_hold(void *);
int EditRefresh(void *);
int EditClose(void *);
int ret(void *);

 int (*fedit_time_l_00_00[])(void *)=
{mode_time,NULL};

void *aedit_time_l_00_00[]=
{NULL};

 int (*fedit_time_l_00_01[])(void *)=
{incr_time_l,NULL};

void *aedit_time_l_00_01[]=
{NULL};

 int (*fedit_time_l_00_02[])(void *)=
{decr_time_l,NULL};

void *aedit_time_l_00_02[]=
{NULL};

 int (*fedit_time_l_00_03[])(void *)=
{set_time,NULL};

void *aedit_time_l_00_03[]=
{NULL};

 int (*fedit_time_l_00_04[])(void *)=
{EditType,s_menu,NULL};

void *aedit_time_l_00_04[]=
{NULL,(void *)edit_type};

 int (*fedit_time_l_00_05[])(void *)=
{EditX,s_menu,NULL};

void *aedit_time_l_00_05[]=
{NULL,(void *)edit_x};

 int (*fedit_time_l_00_06[])(void *)=
{EditTR,s_menu,NULL};

void *aedit_time_l_00_06[]=
{NULL,(void *)edit_time_r};

 int (*fedit_time_l_00_07[])(void *)=
{s_menu_hold,NULL};

void *aedit_time_l_00_07[]=
{(void *)mode};

 int (*fedit_time_l_00_08[])(void *)=
{EditRefresh,NULL};

void *aedit_time_l_00_08[]=
{NULL};

 int (*fedit_time_l_00_09[])(void *)=
{EditClose,ret,NULL};

void *aedit_time_l_00_09[]=
{NULL,(void *)eof};

 int(**f_edit_time_l_00[])(void *)=
{fedit_time_l_00_00,fedit_time_l_00_01,fedit_time_l_00_02,fedit_time_l_00_03,fedit_time_l_00_04,fedit_time_l_00_05,fedit_time_l_00_06,fedit_time_l_00_07,fedit_time_l_00_08,fedit_time_l_00_09};

 void **a_edit_time_l_00[]=
{aedit_time_l_00_00,aedit_time_l_00_01,aedit_time_l_00_02,aedit_time_l_00_03,aedit_time_l_00_04,aedit_time_l_00_05,aedit_time_l_00_06,aedit_time_l_00_07,aedit_time_l_00_08,aedit_time_l_00_09};

menu edit_time_l_00 = {0,0,45,f_edit_time_l_00,a_edit_time_l_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int mode_time(void *);
int incr_time_r(void *);
int decr_time_r(void *);
int set_time(void *);
int EditType(void *);
int s_menu(void *);
int EditTL(void *);
int s_menu(void *);
int s_menu_hold(void *);
int EditRefresh(void *);
int EditClose(void *);
int ret(void *);

 int (*fedit_time_r_00_00[])(void *)=
{mode_time,NULL};

void *aedit_time_r_00_00[]=
{NULL};

 int (*fedit_time_r_00_01[])(void *)=
{incr_time_r,NULL};

void *aedit_time_r_00_01[]=
{NULL};

 int (*fedit_time_r_00_02[])(void *)=
{decr_time_r,NULL};

void *aedit_time_r_00_02[]=
{NULL};

 int (*fedit_time_r_00_03[])(void *)=
{set_time,NULL};

void *aedit_time_r_00_03[]=
{NULL};

 int (*fedit_time_r_00_04[])(void *)=
{EditType,s_menu,NULL};

void *aedit_time_r_00_04[]=
{NULL,(void *)edit_type};

 int (*fedit_time_r_00_05[])(void *)=
{EditTL,s_menu,NULL};

void *aedit_time_r_00_05[]=
{NULL,(void *)edit_time_l};

 int (*fedit_time_r_00_06[])(void *)=
{s_menu_hold,NULL};

void *aedit_time_r_00_06[]=
{(void *)mode};

 int (*fedit_time_r_00_07[])(void *)=
{EditRefresh,NULL};

void *aedit_time_r_00_07[]=
{NULL};

 int (*fedit_time_r_00_08[])(void *)=
{EditClose,ret,NULL};

void *aedit_time_r_00_08[]=
{NULL,(void *)eof};

 int(**f_edit_time_r_00[])(void *)=
{fedit_time_r_00_00,fedit_time_r_00_01,fedit_time_r_00_02,fedit_time_r_00_03,fedit_time_r_00_04,fedit_time_r_00_05,fedit_time_r_00_06,fedit_time_r_00_07,fedit_time_r_00_08};

 void **a_edit_time_r_00[]=
{aedit_time_r_00_00,aedit_time_r_00_01,aedit_time_r_00_02,aedit_time_r_00_03,aedit_time_r_00_04,aedit_time_r_00_05,aedit_time_r_00_06,aedit_time_r_00_07,aedit_time_r_00_08};

menu edit_time_r_00 = {0,0,46,f_edit_time_r_00,a_edit_time_r_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int sel_txt_r(void *);
int sel_txt_l(void *);
int add_txt(void *);
int EditZ(void *);
int s_menu(void *);
int EditRefresh(void *);
int EditClose(void *);
int ret(void *);
int EditTxt(void *);

 int (*fedit_txt_00_00[])(void *)=
{sel_txt_r,NULL};

void *aedit_txt_00_00[]=
{NULL};

 int (*fedit_txt_00_01[])(void *)=
{sel_txt_l,NULL};

void *aedit_txt_00_01[]=
{NULL};

 int (*fedit_txt_00_02[])(void *)=
{add_txt,NULL};

void *aedit_txt_00_02[]=
{NULL};

 int (*fedit_txt_00_04[])(void *)=
{EditZ,s_menu,NULL};

void *aedit_txt_00_04[]=
{NULL,(void *)edit_z};

 int (*fedit_txt_00_05[])(void *)=
{EditRefresh,NULL};

void *aedit_txt_00_05[]=
{NULL};

 int (*fedit_txt_00_06[])(void *)=
{EditClose,ret,NULL};

void *aedit_txt_00_06[]=
{NULL,(void *)eof};

 int (*fedit_txt_00_07[])(void *)=
{EditTxt,NULL};

void *aedit_txt_00_07[]=
{NULL};

 int(**f_edit_txt_00[])(void *)=
{fedit_txt_00_00,fedit_txt_00_01,fedit_txt_00_02,NULL,fedit_txt_00_04,fedit_txt_00_05,fedit_txt_00_06,fedit_txt_00_07};

 void **a_edit_txt_00[]=
{aedit_txt_00_00,aedit_txt_00_01,aedit_txt_00_02,NULL,aedit_txt_00_04,aedit_txt_00_05,aedit_txt_00_06,aedit_txt_00_07};

menu edit_txt_00 = {0,0,47,f_edit_txt_00,a_edit_txt_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int select_type(void *);
int find_ntyp(void *);
int auto_ntyp(void *);
int EditTL(void *);
int s_menu(void *);
int EditZ(void *);
int s_menu(void *);
int EditY(void *);
int s_menu(void *);
int s_menu_hold(void *);
int EditRefresh(void *);
int EditClose(void *);
int ret(void *);
int EditType(void *);

 int (*fedit_type_00_00[])(void *)=
{select_type,NULL};

void *aedit_type_00_00[]=
{NULL};

 int (*fedit_type_00_01[])(void *)=
{find_ntyp,NULL};

void *aedit_type_00_01[]=
{NULL};

 int (*fedit_type_00_02[])(void *)=
{auto_ntyp,NULL};

void *aedit_type_00_02[]=
{NULL};

 int (*fedit_type_00_04[])(void *)=
{EditTL,s_menu,NULL};

void *aedit_type_00_04[]=
{NULL,(void *)edit_time_l};

 int (*fedit_type_00_05[])(void *)=
{EditZ,s_menu,NULL};

void *aedit_type_00_05[]=
{NULL,(void *)edit_z};

 int (*fedit_type_00_06[])(void *)=
{EditY,s_menu,NULL};

void *aedit_type_00_06[]=
{NULL,(void *)edit_y};

 int (*fedit_type_00_07[])(void *)=
{s_menu_hold,NULL};

void *aedit_type_00_07[]=
{(void *)mode};

 int (*fedit_type_00_08[])(void *)=
{EditRefresh,NULL};

void *aedit_type_00_08[]=
{NULL};

 int (*fedit_type_00_09[])(void *)=
{EditClose,ret,NULL};

void *aedit_type_00_09[]=
{NULL,(void *)eof};

 int (*fedit_type_00_10[])(void *)=
{EditType,NULL};

void *aedit_type_00_10[]=
{NULL};

 int(**f_edit_type_00[])(void *)=
{fedit_type_00_00,fedit_type_00_01,fedit_type_00_02,NULL,fedit_type_00_04,fedit_type_00_05,fedit_type_00_06,fedit_type_00_07,fedit_type_00_08,fedit_type_00_09,fedit_type_00_10};

 void **a_edit_type_00[]=
{aedit_type_00_00,aedit_type_00_01,aedit_type_00_02,NULL,aedit_type_00_04,aedit_type_00_05,aedit_type_00_06,aedit_type_00_07,aedit_type_00_08,aedit_type_00_09,aedit_type_00_10};

menu edit_type_00 = {0,0,48,f_edit_type_00,a_edit_type_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int set_EL_AZ_R(void *);
int rewrite_LCD(void *);
int ret(void *);
int set_AZ_R_dZ(void *);
int rewrite_LCD(void *);
int ret(void *);
int set_N_E_Z(void *);
int rewrite_LCD(void *);
int ret(void *);
int origin_local(void *);
int rewrite_LCD(void *);
int ret(void *);
int ret(void *);

 int (*fmode_01_00[])(void *)=
{set_EL_AZ_R,rewrite_LCD,ret,NULL};

void *amode_01_00[]=
{NULL,NULL,(void *)eof};

 int (*fmode_01_01[])(void *)=
{set_AZ_R_dZ,rewrite_LCD,ret,NULL};

void *amode_01_01[]=
{NULL,NULL,(void *)eof};

 int (*fmode_01_02[])(void *)=
{set_N_E_Z,rewrite_LCD,ret,NULL};

void *amode_01_02[]=
{NULL,NULL,(void *)eof};

 int (*fmode_01_03[])(void *)=
{origin_local,rewrite_LCD,ret,NULL};

void *amode_01_03[]=
{NULL,NULL,(void *)eof};

 int (*fmode_01_04[])(void *)=
{ret,NULL};

void *amode_01_04[]=
{(void *)eof};

 int(**f_mode_01[])(void *)=
{fmode_01_00,fmode_01_01,fmode_01_02,fmode_01_03,fmode_01_04};

 void **a_mode_01[]=
{amode_01_00,amode_01_01,amode_01_02,amode_01_03,amode_01_04};

menu mode_01 = {0,0,49,f_mode_01,a_mode_01};
int origin_local(void *);
int rewrite_LCD(void *);
int ret(void *);
int rewrite_LCD(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);
int ret(void *);

 int (*fmode_03_00[])(void *)=
{origin_local,rewrite_LCD,ret,NULL};

void *amode_03_00[]=
{(void *)&Ln,NULL,(void *)eof};

 int (*fmode_03_03[])(void *)=
{rewrite_LCD,ret,NULL};

void *amode_03_03[]=
{NULL,(void *)2};

 int (*fmode_03_04[])(void *)=
{NextList,NULL};

void *amode_03_04[]=
{(void *)&Ln};

 int (*fmode_03_05[])(void *)=
{LastList,NULL};

void *amode_03_05[]=
{(void *)&Ln};

 int (*fmode_03_06[])(void *)=
{LastType,NULL};

void *amode_03_06[]=
{(void *)&Ln};

 int (*fmode_03_07[])(void *)=
{NextType,NULL};

void *amode_03_07[]=
{(void *)&Ln};

 int (*fmode_03_08[])(void *)=
{ret,NULL};

void *amode_03_08[]=
{(void *)eof};

 int(**f_mode_03[])(void *)=
{fmode_03_00,NULL,NULL,fmode_03_03,fmode_03_04,fmode_03_05,fmode_03_06,fmode_03_07,fmode_03_08};

 void **a_mode_03[]=
{amode_03_00,NULL,NULL,amode_03_03,amode_03_04,amode_03_05,amode_03_06,amode_03_07,amode_03_08};

menu mode_03 = {0,0,50,f_mode_03,a_mode_03};
int origin_local(void *);
int rewrite_LCD(void *);
int ret(void *);
int toLL(void *);
int origin_local(void *);
int rewrite_LCD(void *);
int ret(void *);
int ret(void *);

 int (*fmode_02_00[])(void *)=
{origin_local,rewrite_LCD,ret,NULL};

void *amode_02_00[]=
{(void *)&OBP,NULL,(void *)eof};

 int (*fmode_02_01[])(void *)=
{toLL,smenu,NULL};

void *amode_02_01[]=
{(void *)&Ln,&mode_03};

 int (*fmode_02_03[])(void *)=
{origin_local,rewrite_LCD,ret,NULL};

void *amode_02_03[]=
{(void *)&LL,NULL,(void *)eof};

 int (*fmode_02_04[])(void *)=
{ret,NULL};

void *amode_02_04[]=
{(void *)eof};

 int(**f_mode_02[])(void *)=
{fmode_02_00,fmode_02_01,NULL,fmode_02_03,fmode_02_04};

 void **a_mode_02[]=
{amode_02_00,amode_02_01,NULL,amode_02_03,amode_02_04};

menu mode_02 = {0,0,51,f_mode_02,a_mode_02};
int bearing_local(void *);
int rewrite_LCD(void *);
int ret(void *);
int rewrite_LCD(void *);
int ret(void *);
int NextList(void *);
int LastList(void *);
int LastType(void *);
int NextType(void *);
int ret(void *);

 int (*fmode_05_00[])(void *)=
{bearing_local,rewrite_LCD,ret,NULL};

void *amode_05_00[]=
{(void *)&Ln,NULL,(void *)eof};

 int (*fmode_05_03[])(void *)=
{rewrite_LCD,ret,NULL};

void *amode_05_03[]=
{NULL,(void *)2};

 int (*fmode_05_04[])(void *)=
{NextList,NULL};

void *amode_05_04[]=
{(void *)&Ln};

 int (*fmode_05_05[])(void *)=
{LastList,NULL};

void *amode_05_05[]=
{(void *)&Ln};

 int (*fmode_05_06[])(void *)=
{LastType,NULL};

void *amode_05_06[]=
{(void *)&Ln};

 int (*fmode_05_07[])(void *)=
{NextType,NULL};

void *amode_05_07[]=
{(void *)&Ln};

 int (*fmode_05_08[])(void *)=
{ret,NULL};

void *amode_05_08[]=
{(void *)eof};

 int(**f_mode_05[])(void *)=
{fmode_05_00,NULL,NULL,fmode_05_03,fmode_05_04,fmode_05_05,fmode_05_06,fmode_05_07,fmode_05_08};

 void **a_mode_05[]=
{amode_05_00,NULL,NULL,amode_05_03,amode_05_04,amode_05_05,amode_05_06,amode_05_07,amode_05_08};

menu mode_05 = {0,0,52,f_mode_05,a_mode_05};
int point_off(void *);
int ret(void *);
int point_off(void *);
int ret(void *);

 int (*fmode_06_00[])(void *)=
{point_off,ret,NULL};

void *amode_06_00[]=
{NULL,(void *)eof};

 int (*fmode_06_04[])(void *)=
{point_off,ret,NULL};

void *amode_06_04[]=
{NULL,(void *)eof};

 int(**f_mode_06[])(void *)=
{fmode_06_00,NULL,NULL,NULL,fmode_06_04};

 void **a_mode_06[]=
{amode_06_00,NULL,NULL,NULL,amode_06_04};

menu mode_06 = {0,0,53,f_mode_06,a_mode_06};
int bearing_local(void *);
int rewrite_LCD(void *);
int ret(void *);
int toLL(void *);
int point_on(void *);
int bearing_local(void *);
int rewrite_LCD(void *);
int ret(void *);
int ret(void *);

 int (*fmode_04_00[])(void *)=
{bearing_local,rewrite_LCD,ret,NULL};

void *amode_04_00[]=
{NULL,NULL,(void *)eof};

 int (*fmode_04_01[])(void *)=
{toLL,smenu,NULL};

void *amode_04_01[]=
{(void *)&Ln,&mode_05};

 int (*fmode_04_02[])(void *)=
{point_on,smenu,NULL};

void *amode_04_02[]=
{NULL,&mode_06};

 int (*fmode_04_03[])(void *)=
{bearing_local,rewrite_LCD,ret,NULL};

void *amode_04_03[]=
{(void *)&LL,NULL,(void *)eof};

 int (*fmode_04_04[])(void *)=
{ret,NULL};

void *amode_04_04[]=
{(void *)eof};

 int(**f_mode_04[])(void *)=
{fmode_04_00,fmode_04_01,fmode_04_02,fmode_04_03,fmode_04_04};

 void **a_mode_04[]=
{amode_04_00,amode_04_01,amode_04_02,amode_04_03,amode_04_04};

menu mode_04 = {0,0,54,f_mode_04,a_mode_04};
int set_mils6000(void *);
int rewrite_LCD(void *);
int ret(void *);
int set_mils6400(void *);
int rewrite_LCD(void *);
int ret(void *);

 int (*fmode_07_00[])(void *)=
{set_mils6000,rewrite_LCD,ret,NULL};

void *amode_07_00[]=
{NULL,NULL,(void *)eof};

 int (*fmode_07_01[])(void *)=
{set_mils6400,rewrite_LCD,ret,NULL};

void *amode_07_01[]=
{NULL,NULL,(void *)eof};

 int(**f_mode_07[])(void *)=
{fmode_07_00,fmode_07_01,NULL,NULL};

 void **a_mode_07[]=
{amode_07_00,amode_07_01,NULL,NULL};

menu mode_07 = {0,0,55,f_mode_07,a_mode_07};
int ret(void *);

 int (*fmode_00_00[])(void *)=
{smenu,NULL};

void *amode_00_00[]=
{&mode_01};

 int (*fmode_00_01[])(void *)=
{smenu,NULL};

void *amode_00_01[]=
{&mode_02};

 int (*fmode_00_02[])(void *)=
{smenu,NULL};

void *amode_00_02[]=
{&mode_04};

 int (*fmode_00_03[])(void *)=
{smenu,NULL};

void *amode_00_03[]=
{&mode_07};

 int (*fmode_00_04[])(void *)=
{ret,NULL};

void *amode_00_04[]=
{(void *)eof};

 int(**f_mode_00[])(void *)=
{fmode_00_00,fmode_00_01,fmode_00_02,fmode_00_03,fmode_00_04};

 void **a_mode_00[]=
{amode_00_00,amode_00_01,amode_00_02,amode_00_03,amode_00_04};

menu mode_00 = {0,0,56,f_mode_00,a_mode_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int Enter8pt(void *);
int ret(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_08_00[])(void *)=
{Enter8pt,ret,NULL};

void *acal_08_00[]=
{NULL,(void *)eof};

 int (*fcal_08_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_08_03[]=
{NULL,(void *)eof};

 int(**f_cal_08[])(void *)=
{fcal_08_00,NULL,NULL,fcal_08_03};

 void **a_cal_08[]=
{acal_08_00,NULL,NULL,acal_08_03};

menu cal_08 = {0,0,57,f_cal_08,a_cal_08};
int Enter8pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_07_00[])(void *)=
{Enter8pt,smenu,NULL};

void *acal_07_00[]=
{NULL,&cal_08};

 int (*fcal_07_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_07_03[]=
{NULL,(void *)eof};

 int(**f_cal_07[])(void *)=
{fcal_07_00,NULL,NULL,fcal_07_03};

 void **a_cal_07[]=
{acal_07_00,NULL,NULL,acal_07_03};

menu cal_07 = {0,0,58,f_cal_07,a_cal_07};
int Enter8pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_06_00[])(void *)=
{Enter8pt,smenu,NULL};

void *acal_06_00[]=
{NULL,&cal_07};

 int (*fcal_06_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_06_03[]=
{NULL,(void *)eof};

 int(**f_cal_06[])(void *)=
{fcal_06_00,NULL,NULL,fcal_06_03};

 void **a_cal_06[]=
{acal_06_00,NULL,NULL,acal_06_03};

menu cal_06 = {0,0,59,f_cal_06,a_cal_06};
int Enter8pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_05_00[])(void *)=
{Enter8pt,smenu,NULL};

void *acal_05_00[]=
{NULL,&cal_06};

 int (*fcal_05_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_05_03[]=
{NULL,(void *)eof};

 int(**f_cal_05[])(void *)=
{fcal_05_00,NULL,NULL,fcal_05_03};

 void **a_cal_05[]=
{acal_05_00,NULL,NULL,acal_05_03};

menu cal_05 = {0,0,60,f_cal_05,a_cal_05};
int Enter8pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_04_00[])(void *)=
{Enter8pt,smenu,NULL};

void *acal_04_00[]=
{NULL,&cal_05};

 int (*fcal_04_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_04_03[]=
{NULL,(void *)eof};

 int(**f_cal_04[])(void *)=
{fcal_04_00,NULL,NULL,fcal_04_03};

 void **a_cal_04[]=
{acal_04_00,NULL,NULL,acal_04_03};

menu cal_04 = {0,0,61,f_cal_04,a_cal_04};
int Enter8pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_03_00[])(void *)=
{Enter8pt,smenu,NULL};

void *acal_03_00[]=
{NULL,&cal_04};

 int (*fcal_03_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_03_03[]=
{NULL,(void *)eof};

 int(**f_cal_03[])(void *)=
{fcal_03_00,NULL,NULL,fcal_03_03};

 void **a_cal_03[]=
{acal_03_00,NULL,NULL,acal_03_03};

menu cal_03 = {0,0,62,f_cal_03,a_cal_03};
int Enter8pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_02_00[])(void *)=
{Enter8pt,smenu,NULL};

void *acal_02_00[]=
{NULL,&cal_03};

 int (*fcal_02_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_02_03[]=
{NULL,(void *)eof};

 int(**f_cal_02[])(void *)=
{fcal_02_00,NULL,NULL,fcal_02_03};

 void **a_cal_02[]=
{acal_02_00,NULL,NULL,acal_02_03};

menu cal_02 = {0,0,63,f_cal_02,a_cal_02};
int Enter8pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_01_00[])(void *)=
{Enter8pt,smenu,NULL};

void *acal_01_00[]=
{NULL,&cal_02};

 int (*fcal_01_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_01_03[]=
{NULL,(void *)eof};

 int(**f_cal_01[])(void *)=
{fcal_01_00,NULL,NULL,fcal_01_03};

 void **a_cal_01[]=
{acal_01_00,NULL,NULL,acal_01_03};

menu cal_01 = {0,0,64,f_cal_01,a_cal_01};
int Enter3pt(void *);
int ret(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_11_00[])(void *)=
{Enter3pt,ret,NULL};

void *acal_11_00[]=
{NULL,(void *)eof};

 int (*fcal_11_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_11_03[]=
{NULL,(void *)eof};

 int(**f_cal_11[])(void *)=
{fcal_11_00,NULL,NULL,fcal_11_03};

 void **a_cal_11[]=
{acal_11_00,NULL,NULL,acal_11_03};

menu cal_11 = {0,0,65,f_cal_11,a_cal_11};
int Enter3pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_10_00[])(void *)=
{Enter3pt,smenu,NULL};

void *acal_10_00[]=
{NULL,&cal_11};

 int (*fcal_10_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_10_03[]=
{NULL,(void *)eof};

 int(**f_cal_10[])(void *)=
{fcal_10_00,NULL,NULL,fcal_10_03};

 void **a_cal_10[]=
{acal_10_00,NULL,NULL,acal_10_03};

menu cal_10 = {0,0,66,f_cal_10,a_cal_10};
int Enter3pt(void *);
int AbortKVHcal(void *);
int ret(void *);

 int (*fcal_09_00[])(void *)=
{Enter3pt,smenu,NULL};

void *acal_09_00[]=
{NULL,&cal_10};

 int (*fcal_09_03[])(void *)=
{AbortKVHcal,ret,NULL};

void *acal_09_03[]=
{NULL,(void *)eof};

 int(**f_cal_09[])(void *)=
{fcal_09_00,NULL,NULL,fcal_09_03};

 void **a_cal_09[]=
{acal_09_00,NULL,NULL,acal_09_03};

menu cal_09 = {0,0,67,f_cal_09,a_cal_09};
int Init8pt(void *);
int Init3pt(void *);
int ret(void *);
int CheckPass(void *);
int C100align(void *);
int CheckPass(void *);
int C100sign(void *);
int CheckPass(void *);
int C100clear(void *);
int ret(void *);

 int (*fcal_00_00[])(void *)=
{Init8pt,smenu,NULL};

void *acal_00_00[]=
{NULL,&cal_01};

 int (*fcal_00_01[])(void *)=
{Init3pt,smenu,NULL};

void *acal_00_01[]=
{NULL,&cal_09};

 int (*fcal_00_03[])(void *)=
{ret,NULL};

void *acal_00_03[]=
{(void *)1};

 int (*fcal_00_04[])(void *)=
{CheckPass,C100align,NULL};

void *acal_00_04[]=
{(void *)&syspass,NULL};

 int (*fcal_00_05[])(void *)=
{CheckPass,C100sign,NULL};

void *acal_00_05[]=
{(void *)&syspass,NULL};

 int (*fcal_00_06[])(void *)=
{CheckPass,C100clear,NULL};

void *acal_00_06[]=
{(void *)&syspass,NULL};

 int (*fcal_00_07[])(void *)=
{ret,NULL};

void *acal_00_07[]=
{(void *)1};

 int(**f_cal_00[])(void *)=
{fcal_00_00,fcal_00_01,NULL,fcal_00_03,fcal_00_04,fcal_00_05,fcal_00_06,fcal_00_07};

 void **a_cal_00[]=
{acal_00_00,acal_00_01,NULL,acal_00_03,acal_00_04,acal_00_05,acal_00_06,acal_00_07};

menu cal_00 = {0,0,68,f_cal_00,a_cal_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int lmenu(void *);
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int SetSerial(void *);

 int (*fset_system_01_00[])(void *)=
{SetSerial,NULL};

void *aset_system_01_00[]=
{(void *)&ser_no};

 int(**f_set_system_01[])(void *)=
{fset_system_01_00};

 void **a_set_system_01[]=
{aset_system_01_00};

menu set_system_01 = {0,0,69,f_set_system_01,a_set_system_01};
int ReadPwoff(void *);

 int (*fset_system_02_00[])(void *)=
{ReadPwoff,NULL};

void *aset_system_02_00[]=
{(void *)&pwoff};

 int(**f_set_system_02[])(void *)=
{fset_system_02_00};

 void **a_set_system_02[]=
{aset_system_02_00};

menu set_system_02 = {0,0,70,f_set_system_02,a_set_system_02};
int ReadTrack(void *);

 int (*fset_system_03_00[])(void *)=
{ReadTrack,NULL};

void *aset_system_03_00[]=
{(void *)&track_delay};

 int(**f_set_system_03[])(void *)=
{fset_system_03_00};

 void **a_set_system_03[]=
{aset_system_03_00};

menu set_system_03 = {0,0,71,f_set_system_03,a_set_system_03};
int SetPass(void *);

 int (*fset_system_04_00[])(void *)=
{SetPass,NULL};

void *aset_system_04_00[]=
{(void *)&userpass};

 int(**f_set_system_04[])(void *)=
{fset_system_04_00};

 void **a_set_system_04[]=
{aset_system_04_00};

menu set_system_04 = {0,0,72,f_set_system_04,a_set_system_04};
int SetPass(void *);

 int (*fset_system_05_00[])(void *)=
{SetPass,NULL};

void *aset_system_05_00[]=
{(void *)&syspass};

 int(**f_set_system_05[])(void *)=
{fset_system_05_00};

 void **a_set_system_05[]=
{aset_system_05_00};

menu set_system_05 = {0,0,73,f_set_system_05,a_set_system_05};
int SetScreen(void *);

 int (*fset_system_06_00[])(void *)=
{SetScreen,NULL};

void *aset_system_06_00[]=
{NULL};

 int(**f_set_system_06[])(void *)=
{fset_system_06_00};

 void **a_set_system_06[]=
{aset_system_06_00};

menu set_system_06 = {0,0,74,f_set_system_06,a_set_system_06};
int SetTime(void *);

 int (*fset_system_07_00[])(void *)=
{SetTime,NULL};

void *aset_system_07_00[]=
{NULL};

 int(**f_set_system_07[])(void *)=
{fset_system_07_00};

 void **a_set_system_07[]=
{aset_system_07_00};

menu set_system_07 = {0,0,75,f_set_system_07,a_set_system_07};
int SetDate(void *);

 int (*fset_system_08_00[])(void *)=
{SetDate,NULL};

void *aset_system_08_00[]=
{NULL};

 int(**f_set_system_08[])(void *)=
{fset_system_08_00};

 void **a_set_system_08[]=
{aset_system_08_00};

menu set_system_08 = {0,0,76,f_set_system_08,a_set_system_08};
int ReadGpsoff(void *);

 int (*fset_system_09_00[])(void *)=
{ReadGpsoff,NULL};

void *aset_system_09_00[]=
{(void *)&GPS_dt};

 int(**f_set_system_09[])(void *)=
{fset_system_09_00};

 void **a_set_system_09[]=
{aset_system_09_00};

menu set_system_09 = {0,0,77,f_set_system_09,a_set_system_09};
int DiffGps(void *);

 int (*fset_system_10_00[])(void *)=
{DiffGps,NULL};

void *aset_system_10_00[]=
{NULL};

 int(**f_set_system_10[])(void *)=
{fset_system_10_00};

 void **a_set_system_10[]=
{aset_system_10_00};

menu set_system_10 = {0,0,78,f_set_system_10,a_set_system_10};
int ReadGpsoff(void *);

 int (*fset_system_11_00[])(void *)=
{ReadGpsoff,NULL};

void *aset_system_11_00[]=
{(void *)&GPS_alt};

 int(**f_set_system_11[])(void *)=
{fset_system_11_00};

 void **a_set_system_11[]=
{aset_system_11_00};

menu set_system_11 = {0,0,79,f_set_system_11,a_set_system_11};
int ReadGpsLat(void *);

 int (*fset_system_12_00[])(void *)=
{ReadGpsLat,NULL};

void *aset_system_12_00[]=
{NULL};

 int(**f_set_system_12[])(void *)=
{fset_system_12_00};

 void **a_set_system_12[]=
{aset_system_12_00};

menu set_system_12 = {0,0,80,f_set_system_12,a_set_system_12};
int ReadGpsLon(void *);

 int (*fset_system_13_00[])(void *)=
{ReadGpsLon,NULL};

void *aset_system_13_00[]=
{NULL};

 int(**f_set_system_13[])(void *)=
{fset_system_13_00};

 void **a_set_system_13[]=
{aset_system_13_00};

menu set_system_13 = {0,0,81,f_set_system_13,a_set_system_13};
int ReadDecl(void *);

 int (*fset_system_14_00[])(void *)=
{ReadDecl,NULL};

void *aset_system_14_00[]=
{NULL};

 int(**f_set_system_14[])(void *)=
{fset_system_14_00};

 void **a_set_system_14[]=
{aset_system_14_00};

menu set_system_14 = {0,0,82,f_set_system_14,a_set_system_14};
int ShowDgrid(void *);

 int (*fset_system_15_00[])(void *)=
{ShowDgrid,NULL};

void *aset_system_15_00[]=
{NULL};

 int(**f_set_system_15[])(void *)=
{fset_system_15_00};

 void **a_set_system_15[]=
{aset_system_15_00};

menu set_system_15 = {0,0,83,f_set_system_15,a_set_system_15};

 int (*fset_system_00_00[])(void *)=
{lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aset_system_00_00[]=
{&set_system_01,&set_system_02,&set_system_03,&set_system_04,&set_system_05,&set_system_06};

 int (*fset_system_00_01[])(void *)=
{lmenu,lmenu,lmenu,NULL};

void *aset_system_00_01[]=
{&set_system_07,&set_system_08,&set_system_09};

 int (*fset_system_00_02[])(void *)=
{lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aset_system_00_02[]=
{&set_system_10,&set_system_11,&set_system_12,&set_system_13,&set_system_14,&set_system_15};

 int(**f_set_system_00[])(void *)=
{fset_system_00_00,fset_system_00_01,fset_system_00_02};

 void **a_set_system_00[]=
{aset_system_00_00,aset_system_00_01,aset_system_00_02};

menu set_system_00 = {0,0,84,f_set_system_00,a_set_system_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int ShowEncoder(void *);

 int (*fSetupMenu_01_00[])(void *)=
{ShowEncoder,NULL};

void *aSetupMenu_01_00[]=
{NULL};

 int(**f_SetupMenu_01[])(void *)=
{fSetupMenu_01_00};

 void **a_SetupMenu_01[]=
{aSetupMenu_01_00};

menu SetupMenu_01 = {0,0,85,f_SetupMenu_01,a_SetupMenu_01};
int ShowPitch(void *);

 int (*fSetupMenu_02_00[])(void *)=
{ShowPitch,NULL};

void *aSetupMenu_02_00[]=
{NULL};

 int(**f_SetupMenu_02[])(void *)=
{fSetupMenu_02_00};

 void **a_SetupMenu_02[]=
{aSetupMenu_02_00};

menu SetupMenu_02 = {0,0,86,f_SetupMenu_02,a_SetupMenu_02};
int ShowRoll(void *);

 int (*fSetupMenu_03_00[])(void *)=
{ShowRoll,NULL};

void *aSetupMenu_03_00[]=
{NULL};

 int(**f_SetupMenu_03[])(void *)=
{fSetupMenu_03_00};

 void **a_SetupMenu_03[]=
{aSetupMenu_03_00};

menu SetupMenu_03 = {0,0,87,f_SetupMenu_03,a_SetupMenu_03};
int ToffTilt(void *);

 int (*fSetupMenu_04_00[])(void *)=
{ToffTilt,NULL};

void *aSetupMenu_04_00[]=
{NULL};

 int(**f_SetupMenu_04[])(void *)=
{fSetupMenu_04_00};

 void **a_SetupMenu_04[]=
{aSetupMenu_04_00};

menu SetupMenu_04 = {0,0,88,f_SetupMenu_04,a_SetupMenu_04};
int TscaleTilt(void *);

 int (*fSetupMenu_05_00[])(void *)=
{TscaleTilt,NULL};

void *aSetupMenu_05_00[]=
{NULL};

 int(**f_SetupMenu_05[])(void *)=
{fSetupMenu_05_00};

 void **a_SetupMenu_05[]=
{aSetupMenu_05_00};

menu SetupMenu_05 = {0,0,89,f_SetupMenu_05,a_SetupMenu_05};
int ShowTemp(void *);

 int (*fSetupMenu_06_00[])(void *)=
{ShowTemp,NULL};

void *aSetupMenu_06_00[]=
{NULL};

 int(**f_SetupMenu_06[])(void *)=
{fSetupMenu_06_00};

 void **a_SetupMenu_06[]=
{aSetupMenu_06_00};

menu SetupMenu_06 = {0,0,90,f_SetupMenu_06,a_SetupMenu_06};
int SelDatum(void *);

 int (*fSetupMenu_07_00[])(void *)=
{SelDatum,NULL};

void *aSetupMenu_07_00[]=
{NULL};

 int(**f_SetupMenu_07[])(void *)=
{fSetupMenu_07_00};

 void **a_SetupMenu_07[]=
{aSetupMenu_07_00};

menu SetupMenu_07 = {0,0,91,f_SetupMenu_07,a_SetupMenu_07};
int SelGeoid(void *);

 int (*fSetupMenu_08_00[])(void *)=
{SelGeoid,NULL};

void *aSetupMenu_08_00[]=
{NULL};

 int(**f_SetupMenu_08[])(void *)=
{fSetupMenu_08_00};

 void **a_SetupMenu_08[]=
{aSetupMenu_08_00};

menu SetupMenu_08 = {0,0,92,f_SetupMenu_08,a_SetupMenu_08};
int GeoOffset(void *);

 int (*fSetupMenu_09_00[])(void *)=
{GeoOffset,NULL};

void *aSetupMenu_09_00[]=
{(void *)&GPS_dx};

 int(**f_SetupMenu_09[])(void *)=
{fSetupMenu_09_00};

 void **a_SetupMenu_09[]=
{aSetupMenu_09_00};

menu SetupMenu_09 = {0,0,93,f_SetupMenu_09,a_SetupMenu_09};
int GeoOffset(void *);

 int (*fSetupMenu_10_00[])(void *)=
{GeoOffset,NULL};

void *aSetupMenu_10_00[]=
{(void *)&GPS_dy};

 int(**f_SetupMenu_10[])(void *)=
{fSetupMenu_10_00};

 void **a_SetupMenu_10[]=
{aSetupMenu_10_00};

menu SetupMenu_10 = {0,0,94,f_SetupMenu_10,a_SetupMenu_10};
int GeoOffset(void *);

 int (*fSetupMenu_11_00[])(void *)=
{GeoOffset,NULL};

void *aSetupMenu_11_00[]=
{(void *)&GPS_dz};

 int(**f_SetupMenu_11[])(void *)=
{fSetupMenu_11_00};

 void **a_SetupMenu_11[]=
{aSetupMenu_11_00};

menu SetupMenu_11 = {0,0,95,f_SetupMenu_11,a_SetupMenu_11};
int ReadPdop(void *);

 int (*fSetupMenu_12_00[])(void *)=
{ReadPdop,NULL};

void *aSetupMenu_12_00[]=
{NULL};

 int(**f_SetupMenu_12[])(void *)=
{fSetupMenu_12_00};

 void **a_SetupMenu_12[]=
{aSetupMenu_12_00};

menu SetupMenu_12 = {0,0,96,f_SetupMenu_12,a_SetupMenu_12};
int GPS_pmode(void *);
int GPS_pmode(void *);
int GPS_pmode(void *);

 int (*fSetupMenu_13_00[])(void *)=
{GPS_pmode,NULL};

void *aSetupMenu_13_00[]=
{NULL};

 int (*fSetupMenu_13_01[])(void *)=
{GPS_pmode,NULL};

void *aSetupMenu_13_01[]=
{NULL};

 int (*fSetupMenu_13_02[])(void *)=
{GPS_pmode,NULL};

void *aSetupMenu_13_02[]=
{NULL};

 int(**f_SetupMenu_13[])(void *)=
{fSetupMenu_13_00,fSetupMenu_13_01,fSetupMenu_13_02};

 void **a_SetupMenu_13[]=
{aSetupMenu_13_00,aSetupMenu_13_01,aSetupMenu_13_02};

menu SetupMenu_13 = {0,0,97,f_SetupMenu_13,a_SetupMenu_13};
int GPS_dmode(void *);
int GPS_dmode(void *);

 int (*fSetupMenu_14_00[])(void *)=
{GPS_dmode,NULL};

void *aSetupMenu_14_00[]=
{NULL};

 int (*fSetupMenu_14_01[])(void *)=
{GPS_dmode,NULL};

void *aSetupMenu_14_01[]=
{NULL};

 int(**f_SetupMenu_14[])(void *)=
{fSetupMenu_14_00,fSetupMenu_14_01};

 void **a_SetupMenu_14[]=
{aSetupMenu_14_00,aSetupMenu_14_01};

menu SetupMenu_14 = {0,0,98,f_SetupMenu_14,a_SetupMenu_14};
int EnterInt(void *);

 int (*fSetupMenu_15_00[])(void *)=
{EnterInt,NULL};

void *aSetupMenu_15_00[]=
{(void *)&CM_sect};

 int(**f_SetupMenu_15[])(void *)=
{fSetupMenu_15_00};

 void **a_SetupMenu_15[]=
{aSetupMenu_15_00};

menu SetupMenu_15 = {0,0,99,f_SetupMenu_15,a_SetupMenu_15};
int EnterInt(void *);

 int (*fSetupMenu_16_00[])(void *)=
{EnterInt,NULL};

void *aSetupMenu_16_00[]=
{(void *)&CM_off};

 int(**f_SetupMenu_16[])(void *)=
{fSetupMenu_16_00};

 void **a_SetupMenu_16[]=
{aSetupMenu_16_00};

menu SetupMenu_16 = {0,0,100,f_SetupMenu_16,a_SetupMenu_16};
int EnterLong(void *);

 int (*fSetupMenu_17_00[])(void *)=
{EnterLong,NULL};

void *aSetupMenu_17_00[]=
{(void *)&FalseNorth};

 int(**f_SetupMenu_17[])(void *)=
{fSetupMenu_17_00};

 void **a_SetupMenu_17[]=
{aSetupMenu_17_00};

menu SetupMenu_17 = {0,0,101,f_SetupMenu_17,a_SetupMenu_17};
int EnterLong(void *);

 int (*fSetupMenu_18_00[])(void *)=
{EnterLong,NULL};

void *aSetupMenu_18_00[]=
{(void *)&FalseEast};

 int(**f_SetupMenu_18[])(void *)=
{fSetupMenu_18_00};

 void **a_SetupMenu_18[]=
{aSetupMenu_18_00};

menu SetupMenu_18 = {0,0,102,f_SetupMenu_18,a_SetupMenu_18};
int EnterInt(void *);

 int (*fSetupMenu_19_00[])(void *)=
{EnterInt,NULL};

void *aSetupMenu_19_00[]=
{(void *)&key_delay};

 int(**f_SetupMenu_19[])(void *)=
{fSetupMenu_19_00};

 void **a_SetupMenu_19[]=
{aSetupMenu_19_00};

menu SetupMenu_19 = {0,0,103,f_SetupMenu_19,a_SetupMenu_19};
int SetDSPgain(void *);

 int (*fSetupMenu_20_00[])(void *)=
{SetDSPgain,NULL};

void *aSetupMenu_20_00[]=
{(void *)0};

 int(**f_SetupMenu_20[])(void *)=
{fSetupMenu_20_00};

 void **a_SetupMenu_20[]=
{aSetupMenu_20_00};

menu SetupMenu_20 = {0,0,104,f_SetupMenu_20,a_SetupMenu_20};
int SetDSPgain(void *);

 int (*fSetupMenu_21_00[])(void *)=
{SetDSPgain,NULL};

void *aSetupMenu_21_00[]=
{(void *)1};

 int(**f_SetupMenu_21[])(void *)=
{fSetupMenu_21_00};

 void **a_SetupMenu_21[]=
{aSetupMenu_21_00};

menu SetupMenu_21 = {0,0,105,f_SetupMenu_21,a_SetupMenu_21};
int SetDSPgain(void *);

 int (*fSetupMenu_22_00[])(void *)=
{SetDSPgain,NULL};

void *aSetupMenu_22_00[]=
{(void *)2};

 int(**f_SetupMenu_22[])(void *)=
{fSetupMenu_22_00};

 void **a_SetupMenu_22[]=
{aSetupMenu_22_00};

menu SetupMenu_22 = {0,0,106,f_SetupMenu_22,a_SetupMenu_22};
int SetDSPgain(void *);

 int (*fSetupMenu_23_00[])(void *)=
{SetDSPgain,NULL};

void *aSetupMenu_23_00[]=
{(void *)3};

 int(**f_SetupMenu_23[])(void *)=
{fSetupMenu_23_00};

 void **a_SetupMenu_23[]=
{aSetupMenu_23_00};

menu SetupMenu_23 = {0,0,107,f_SetupMenu_23,a_SetupMenu_23};
int ReadDgps(void *);

 int (*fSetupMenu_25_00[])(void *)=
{ReadDgps,NULL};

void *aSetupMenu_25_00[]=
{NULL};

 int(**f_SetupMenu_25[])(void *)=
{fSetupMenu_25_00};

 void **a_SetupMenu_25[]=
{aSetupMenu_25_00};

menu SetupMenu_25 = {0,0,108,f_SetupMenu_25,a_SetupMenu_25};
int EnterOpt(void *);
int EnterOpt(void *);
int EnterOpt(void *);

 int (*fSetupMenu_24_00[])(void *)=
{EnterOpt,NULL};

void *aSetupMenu_24_00[]=
{(void *)&aux_mode};

 int (*fSetupMenu_24_01[])(void *)=
{EnterOpt,NULL};

void *aSetupMenu_24_01[]=
{(void *)&aux_mode};

 int (*fSetupMenu_24_02[])(void *)=
{EnterOpt,lmenu,NULL};

void *aSetupMenu_24_02[]=
{(void *)&aux_mode,&SetupMenu_25};

 int(**f_SetupMenu_24[])(void *)=
{fSetupMenu_24_00,fSetupMenu_24_01,fSetupMenu_24_02};

 void **a_SetupMenu_24[]=
{aSetupMenu_24_00,aSetupMenu_24_01,aSetupMenu_24_02};

menu SetupMenu_24 = {0,0,109,f_SetupMenu_24,a_SetupMenu_24};
int EnterInt(void *);

 int (*fSetupMenu_26_00[])(void *)=
{EnterInt,NULL};

void *aSetupMenu_26_00[]=
{(void *)&swvers};

 int(**f_SetupMenu_26[])(void *)=
{fSetupMenu_26_00};

 void **a_SetupMenu_26[]=
{aSetupMenu_26_00};

menu SetupMenu_26 = {0,0,110,f_SetupMenu_26,a_SetupMenu_26};
int EnterOpt(void *);
int EnterOpt(void *);

 int (*fSetupMenu_27_00[])(void *)=
{EnterOpt,NULL};

void *aSetupMenu_27_00[]=
{(void *)&coordtx};

 int (*fSetupMenu_27_01[])(void *)=
{EnterOpt,NULL};

void *aSetupMenu_27_01[]=
{(void *)&coordtx};

 int(**f_SetupMenu_27[])(void *)=
{fSetupMenu_27_00,fSetupMenu_27_01};

 void **a_SetupMenu_27[]=
{aSetupMenu_27_00,aSetupMenu_27_01};

menu SetupMenu_27 = {0,0,111,f_SetupMenu_27,a_SetupMenu_27};
int RLD_tmode(void *);
int RLD_tmode(void *);

 int (*fSetupMenu_28_00[])(void *)=
{RLD_tmode,NULL};

void *aSetupMenu_28_00[]=
{NULL};

 int (*fSetupMenu_28_01[])(void *)=
{RLD_tmode,NULL};

void *aSetupMenu_28_01[]=
{NULL};

 int(**f_SetupMenu_28[])(void *)=
{fSetupMenu_28_00,fSetupMenu_28_01};

 void **a_SetupMenu_28[]=
{aSetupMenu_28_00,aSetupMenu_28_01};

menu SetupMenu_28 = {0,0,112,f_SetupMenu_28,a_SetupMenu_28};
int SetLanguage(void *);

 int (*fSetupMenu_29_00[])(void *)=
{SetLanguage,NULL};

void *aSetupMenu_29_00[]=
{NULL};

 int(**f_SetupMenu_29[])(void *)=
{fSetupMenu_29_00};

 void **a_SetupMenu_29[]=
{aSetupMenu_29_00};

menu SetupMenu_29 = {0,0,113,f_SetupMenu_29,a_SetupMenu_29};
int RLD_irmode(void *);
int RLD_irmode(void *);

 int (*fSetupMenu_30_00[])(void *)=
{RLD_irmode,NULL};

void *aSetupMenu_30_00[]=
{NULL};

 int (*fSetupMenu_30_01[])(void *)=
{RLD_irmode,NULL};

void *aSetupMenu_30_01[]=
{NULL};

 int(**f_SetupMenu_30[])(void *)=
{fSetupMenu_30_00,fSetupMenu_30_01};

 void **a_SetupMenu_30[]=
{aSetupMenu_30_00,aSetupMenu_30_01};

menu SetupMenu_30 = {0,0,114,f_SetupMenu_30,a_SetupMenu_30};

 int (*fSetupMenu_00_00[])(void *)=
{lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aSetupMenu_00_00[]=
{&SetupMenu_01,&SetupMenu_02,&SetupMenu_03,&SetupMenu_04,&SetupMenu_05,&SetupMenu_06};

 int (*fSetupMenu_00_01[])(void *)=
{lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aSetupMenu_00_01[]=
{&SetupMenu_07,&SetupMenu_08,&SetupMenu_09,&SetupMenu_10,&SetupMenu_11,&SetupMenu_12,&SetupMenu_13,&SetupMenu_14};

 int (*fSetupMenu_00_02[])(void *)=
{lmenu,lmenu,lmenu,lmenu,NULL};

void *aSetupMenu_00_02[]=
{&SetupMenu_15,&SetupMenu_16,&SetupMenu_17,&SetupMenu_18};

 int (*fSetupMenu_00_03[])(void *)=
{lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aSetupMenu_00_03[]=
{&SetupMenu_19,&SetupMenu_20,&SetupMenu_21,&SetupMenu_22,&SetupMenu_23};

 int (*fSetupMenu_00_04[])(void *)=
{lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aSetupMenu_00_04[]=
{&SetupMenu_24,&SetupMenu_26,&SetupMenu_27,&SetupMenu_28,&SetupMenu_29,&SetupMenu_30};

 int(**f_SetupMenu_00[])(void *)=
{fSetupMenu_00_00,fSetupMenu_00_01,fSetupMenu_00_02,fSetupMenu_00_03,fSetupMenu_00_04};

 void **a_SetupMenu_00[]=
{aSetupMenu_00_00,aSetupMenu_00_01,aSetupMenu_00_02,aSetupMenu_00_03,aSetupMenu_00_04};

menu SetupMenu_00 = {0,0,115,f_SetupMenu_00,a_SetupMenu_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int SetHeight(void *);

 int (*fset_meteo_01_00[])(void *)=
{SetHeight,NULL};

void *aset_meteo_01_00[]=
{(void *)&METEO};

 int(**f_set_meteo_01[])(void *)=
{fset_meteo_01_00};

 void **a_set_meteo_01[]=
{aset_meteo_01_00};

menu set_meteo_01 = {0,0,116,f_set_meteo_01,a_set_meteo_01};
int AirTemp(void *);

 int (*fset_meteo_02_00[])(void *)=
{AirTemp,NULL};

void *aset_meteo_02_00[]=
{(void *)&METEO};

 int(**f_set_meteo_02[])(void *)=
{fset_meteo_02_00};

 void **a_set_meteo_02[]=
{aset_meteo_02_00};

menu set_meteo_02 = {0,0,117,f_set_meteo_02,a_set_meteo_02};
int AirPress(void *);

 int (*fset_meteo_03_00[])(void *)=
{AirPress,NULL};

void *aset_meteo_03_00[]=
{(void *)&METEO};

 int(**f_set_meteo_03[])(void *)=
{fset_meteo_03_00};

 void **a_set_meteo_03[]=
{aset_meteo_03_00};

menu set_meteo_03 = {0,0,118,f_set_meteo_03,a_set_meteo_03};
int AirHum(void *);

 int (*fset_meteo_04_00[])(void *)=
{AirHum,NULL};

void *aset_meteo_04_00[]=
{(void *)&METEO};

 int(**f_set_meteo_04[])(void *)=
{fset_meteo_04_00};

 void **a_set_meteo_04[]=
{aset_meteo_04_00};

menu set_meteo_04 = {0,0,119,f_set_meteo_04,a_set_meteo_04};
int WindSpeed(void *);

 int (*fset_meteo_05_00[])(void *)=
{WindSpeed,NULL};

void *aset_meteo_05_00[]=
{(void *)&METEO};

 int(**f_set_meteo_05[])(void *)=
{fset_meteo_05_00};

 void **a_set_meteo_05[]=
{aset_meteo_05_00};

menu set_meteo_05 = {0,0,120,f_set_meteo_05,a_set_meteo_05};
int WindDir(void *);

 int (*fset_meteo_06_00[])(void *)=
{WindDir,NULL};

void *aset_meteo_06_00[]=
{(void *)&METEO};

 int(**f_set_meteo_06[])(void *)=
{fset_meteo_06_00};

 void **a_set_meteo_06[]=
{aset_meteo_06_00};

menu set_meteo_06 = {0,0,121,f_set_meteo_06,a_set_meteo_06};
int EditMeteo(void *);

 int (*fset_meteo_00_00[])(void *)=
{EditMeteo,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aset_meteo_00_00[]=
{NULL,&set_meteo_01,&set_meteo_02,&set_meteo_03,&set_meteo_04,&set_meteo_05,&set_meteo_06};

 int(**f_set_meteo_00[])(void *)=
{fset_meteo_00_00};

 void **a_set_meteo_00[]=
{aset_meteo_00_00};

menu set_meteo_00 = {0,0,122,f_set_meteo_00,a_set_meteo_00};
int  comm_par[4];
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int EnterEffect(void *);

 int (*fCedit_01_00[])(void *)=
{EnterEffect,NULL};

void *aCedit_01_00[]=
{(void *)&CP};

 int(**f_Cedit_01[])(void *)=
{fCedit_01_00};

 void **a_Cedit_01[]=
{aCedit_01_00};

menu Cedit_01 = {0,0,123,f_Cedit_01,a_Cedit_01};
int EnterFiretime(void *);

 int (*fCedit_02_00[])(void *)=
{EnterFiretime,NULL};

void *aCedit_02_00[]=
{(void *)&CP};

 int(**f_Cedit_02[])(void *)=
{fCedit_02_00};

 void **a_Cedit_02[]=
{aCedit_02_00};

menu Cedit_02 = {0,0,124,f_Cedit_02,a_Cedit_02};
int EnterSect(void *);

 int (*fCedit_03_00[])(void *)=
{EnterSect,NULL};

void *aCedit_03_00[]=
{(void *)&CP};

 int(**f_Cedit_03[])(void *)=
{fCedit_03_00};

 void **a_Cedit_03[]=
{aCedit_03_00};

menu Cedit_03 = {0,0,125,f_Cedit_03,a_Cedit_03};
int EnterProj(void *);

 int (*fCedit_04_00[])(void *)=
{EnterProj,NULL};

void *aCedit_04_00[]=
{(void *)&CP};

 int(**f_Cedit_04[])(void *)=
{fCedit_04_00};

 void **a_Cedit_04[]=
{aCedit_04_00};

menu Cedit_04 = {0,0,126,f_Cedit_04,a_Cedit_04};
int EnterFuze(void *);

 int (*fCedit_05_00[])(void *)=
{EnterFuze,NULL};

void *aCedit_05_00[]=
{(void *)&CP};

 int(**f_Cedit_05[])(void *)=
{fCedit_05_00};

 void **a_Cedit_05[]=
{aCedit_05_00};

menu Cedit_05 = {0,0,127,f_Cedit_05,a_Cedit_05};
int EnterCharge(void *);

 int (*fCedit_06_00[])(void *)=
{EnterCharge,NULL};

void *aCedit_06_00[]=
{(void *)&CP};

 int(**f_Cedit_06[])(void *)=
{fCedit_06_00};

 void **a_Cedit_06[]=
{aCedit_06_00};

menu Cedit_06 = {0,0,128,f_Cedit_06,a_Cedit_06};
int EnterRounds(void *);

 int (*fCedit_07_00[])(void *)=
{EnterRounds,NULL};

void *aCedit_07_00[]=
{(void *)&CP};

 int(**f_Cedit_07[])(void *)=
{fCedit_07_00};

 void **a_Cedit_07[]=
{aCedit_07_00};

menu Cedit_07 = {0,0,129,f_Cedit_07,a_Cedit_07};
int EnterInterval(void *);

 int (*fCedit_08_00[])(void *)=
{EnterInterval,NULL};

void *aCedit_08_00[]=
{(void *)&CP};

 int(**f_Cedit_08[])(void *)=
{fCedit_08_00};

 void **a_Cedit_08[]=
{aCedit_08_00};

menu Cedit_08 = {0,0,130,f_Cedit_08,a_Cedit_08};
int EnterTraj(void *);

 int (*fCedit_09_00[])(void *)=
{EnterTraj,NULL};

void *aCedit_09_00[]=
{(void *)&CP};

 int(**f_Cedit_09[])(void *)=
{fCedit_09_00};

 void **a_Cedit_09[]=
{aCedit_09_00};

menu Cedit_09 = {0,0,131,f_Cedit_09,a_Cedit_09};
int EnterFiretime(void *);

 int (*fCedit_10_00[])(void *)=
{EnterFiretime,NULL};

void *aCedit_10_00[]=
{(void *)&CP};

 int(**f_Cedit_10[])(void *)=
{fCedit_10_00};

 void **a_Cedit_10[]=
{aCedit_10_00};

menu Cedit_10 = {0,0,132,f_Cedit_10,a_Cedit_10};
int ShowTg(void *);
int ShowTg(void *);

 int (*fCedit_00_00[])(void *)=
{ShowTg,lmenu,lmenu,NULL};

void *aCedit_00_00[]=
{NULL,&Cedit_01,&Cedit_02};

 int (*fCedit_00_01[])(void *)=
{ShowTg,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aCedit_00_01[]=
{NULL,&Cedit_03,&Cedit_04,&Cedit_05,&Cedit_06,&Cedit_07,&Cedit_08,&Cedit_09,&Cedit_10};

 int(**f_Cedit_00[])(void *)=
{fCedit_00_00,fCedit_00_01};

 void **a_Cedit_00[]=
{aCedit_00_00,aCedit_00_01};

menu Cedit_00 = {0,0,133,f_Cedit_00,a_Cedit_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int SetRefDir(void *);

 int (*fedit_gun_01_00[])(void *)=
{SetRefDir,NULL};

void *aedit_gun_01_00[]=
{NULL};

 int(**f_edit_gun_01[])(void *)=
{fedit_gun_01_00};

 void **a_edit_gun_01[]=
{aedit_gun_01_00};

menu edit_gun_01 = {0,0,134,f_edit_gun_01,a_edit_gun_01};
int SetGunref(void *);

 int (*fedit_gun_02_00[])(void *)=
{SetGunref,NULL};

void *aedit_gun_02_00[]=
{NULL};

 int(**f_edit_gun_02[])(void *)=
{fedit_gun_02_00};

 void **a_edit_gun_02[]=
{aedit_gun_02_00};

menu edit_gun_02 = {0,0,135,f_edit_gun_02,a_edit_gun_02};
int SetGun(void *);

 int (*fedit_gun_03_00[])(void *)=
{SetGun,NULL};

void *aedit_gun_03_00[]=
{NULL};

 int(**f_edit_gun_03[])(void *)=
{fedit_gun_03_00};

 void **a_edit_gun_03[]=
{aedit_gun_03_00};

menu edit_gun_03 = {0,0,136,f_edit_gun_03,a_edit_gun_03};
int SetAmm(void *);

 int (*fedit_gun_04_00[])(void *)=
{SetAmm,NULL};

void *aedit_gun_04_00[]=
{NULL};

 int(**f_edit_gun_04[])(void *)=
{fedit_gun_04_00};

 void **a_edit_gun_04[]=
{aedit_gun_04_00};

menu edit_gun_04 = {0,0,137,f_edit_gun_04,a_edit_gun_04};
int SetPcs(void *);

 int (*fedit_gun_05_00[])(void *)=
{SetPcs,NULL};

void *aedit_gun_05_00[]=
{NULL};

 int(**f_edit_gun_05[])(void *)=
{fedit_gun_05_00};

 void **a_edit_gun_05[]=
{aedit_gun_05_00};

menu edit_gun_05 = {0,0,138,f_edit_gun_05,a_edit_gun_05};
int SetMass(void *);

 int (*fedit_gun_06_00[])(void *)=
{SetMass,NULL};

void *aedit_gun_06_00[]=
{NULL};

 int(**f_edit_gun_06[])(void *)=
{fedit_gun_06_00};

 void **a_edit_gun_06[]=
{aedit_gun_06_00};

menu edit_gun_06 = {0,0,139,f_edit_gun_06,a_edit_gun_06};
int PowderT(void *);

 int (*fedit_gun_07_00[])(void *)=
{PowderT,NULL};

void *aedit_gun_07_00[]=
{NULL};

 int(**f_edit_gun_07[])(void *)=
{fedit_gun_07_00};

 void **a_edit_gun_07[]=
{aedit_gun_07_00};

menu edit_gun_07 = {0,0,140,f_edit_gun_07,a_edit_gun_07};
int SetMuzzle(void *);

 int (*fedit_gun_08_00[])(void *)=
{SetMuzzle,NULL};

void *aedit_gun_08_00[]=
{NULL};

 int(**f_edit_gun_08[])(void *)=
{fedit_gun_08_00};

 void **a_edit_gun_08[]=
{aedit_gun_08_00};

menu edit_gun_08 = {0,0,141,f_edit_gun_08,a_edit_gun_08};
int EditGun(void *);

 int (*fedit_gun_00_00[])(void *)=
{EditGun,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,lmenu,NULL};

void *aedit_gun_00_00[]=
{NULL,&edit_gun_01,&edit_gun_02,&edit_gun_03,&edit_gun_04,&edit_gun_05,&edit_gun_06,&edit_gun_07,&edit_gun_08};

 int(**f_edit_gun_00[])(void *)=
{fedit_gun_00_00};

 void **a_edit_gun_00[]=
{aedit_gun_00_00};

menu edit_gun_00 = {0,0,142,f_edit_gun_00,a_edit_gun_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int EnterPass(void *);

 int (*fEnterPasswrd_00_00[])(void *)=
{EnterPass,NULL};

void *aEnterPasswrd_00_00[]=
{NULL};

 int(**f_EnterPasswrd_00[])(void *)=
{fEnterPasswrd_00_00};

 void **a_EnterPasswrd_00[]=
{aEnterPasswrd_00_00};

menu EnterPasswrd_00 = {0,0,143,f_EnterPasswrd_00,a_EnterPasswrd_00};
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int smenu(void *);
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int UtmNorth(void *);
int UtmSouth(void *);
int UtmEast(void *);
int UtmWest(void *);
int EditX(void *);
int s_menu(void *);
int EditClose(void *);
int ret(void *);

 int (*fedit_utm_00_00[])(void *)=
{UtmNorth,NULL};

void *aedit_utm_00_00[]=
{NULL};

 int (*fedit_utm_00_01[])(void *)=
{UtmSouth,NULL};

void *aedit_utm_00_01[]=
{NULL};

 int (*fedit_utm_00_02[])(void *)=
{UtmEast,NULL};

void *aedit_utm_00_02[]=
{NULL};

 int (*fedit_utm_00_03[])(void *)=
{UtmWest,NULL};

void *aedit_utm_00_03[]=
{NULL};

 int (*fedit_utm_00_04[])(void *)=
{EditX,s_menu,NULL};

void *aedit_utm_00_04[]=
{NULL,(void *)edit_x};

 int (*fedit_utm_00_05[])(void *)=
{EditClose,ret,NULL};

void *aedit_utm_00_05[]=
{NULL,(void *)eof};

 int(**f_edit_utm_00[])(void *)=
{fedit_utm_00_00,fedit_utm_00_01,fedit_utm_00_02,fedit_utm_00_03,fedit_utm_00_04,fedit_utm_00_05};

 void **a_edit_utm_00[]=
{aedit_utm_00_00,aedit_utm_00_01,aedit_utm_00_02,aedit_utm_00_03,aedit_utm_00_04,aedit_utm_00_05};

menu edit_utm_00 = {0,0,144,f_edit_utm_00,a_edit_utm_00};
char *_TextCode[]={
"*** WARNING",
"*** ERROR",
"origin modified",
"obs.post modified",
"north modified",
"library empty",
"single item",
"item not found",
"DGPS off",
"low battery",
"fire time elapsed",
"no target",
"charge modified",
"multiple targets",
"target blocked",
"obs. post data",
"origin data",
"north data",
"void data",
"illegal data",
"void type",
"void list",
"library full",
"laser not allowed",
"laser not ready",
"north eq. origin",
"max.value exceeded",
"list empty",
"no target data",
"no station data",
"no unit data",
"gun not selected",
"command pending",
"illegal r-01",
"illegal r-02",
"illegal r-03",
"illegal m-01",
"illegal m-02",
"illegal b-01",
"illegal b-02",
"illegal b-03",
"no command",
"link busy",
"end of list",
"GPS error",
"BT type required",
"TG type required",
"MT type required",
"TG or MT required",
"1 point only",
"3 points only",
"8 points only",
"fire time fixed",
"ill. password",
"out of range",
"too close",
" out of range",
"m-01 equal m-02",
"r-01 equal r-02",
"list no. ",
"crest",
"obs. post data",
"object erased",
"object inserted",
"object modified",
"numeric",
"letters",
"lower case",
"list updated",
"gun updated",
"command ready",
"command finished",
"command rejected",
"command aborted",
"laser on",
"laser off",
"init. meteo",
"meteo updated",
"wait",
"saving library",
"reload library",
"batt.HQ",
"battery",
"undefined",
"ref.point",
"obs.post",
"crest",
"inf.HQ",
"art.HQ",
"arm.HQ",
"infantry",
"artillery",
"armour",
"pos.",
"ref. north",
"ref.point 1",
"ref.point 2",
"ref.point 3",
"measure 1",
"measure 2",
"measure 3",
"bearing to 1",
"bearing to 2",
"bearing to 3",
"est.position",
"pitch  roll",
"enabled",
"disabled",
"password",
" ",
" ..",
"battery",
"sect. 1",
"sect. 2",
"ref.",
"message",
"gun",
"meteo",
"to",
"from",
"HIGH",
"LOW",
"destroy",
"neutralize",
"smoke",
"illuminate",
"HOLD",
"FIRE",
"WHEN READY",
"...",
"pending",
"ready",
"not ready",
"aborted",
"jan",
"feb",
"mar",
"apr",
"may",
"jun",
"jul",
"aug",
"sep",
"oct",
"nov",
"dec",
"SU",
"MO",
"TU",
"WE",
"TH",
"FR",
"SA",
"OP",
"CO",
"BT",
"TG",
"MT",
"CR",
"RP",
"0/1/2/3/4/5/6/7/8/9",
".+-/ABC/DEF/GHI/JKL/MNO/PQR/STU/VWX/YZ ",
".+-/abc/def/ghi/jkl/mno/pqr/stu/vwx/yz ",
"",
"",
"m",
"h",
"s",
"azimuth",
"bearing",
"rounds",
"N",
"S",
"E",
"W",
NULL};


char *_MenuCode[]={
"ALL SINGLE / CANCEL",
"SAVE / / CANCEL",
"ENTER ERASE / EXIT \x87 \x88 \x85 \x86 \x80 \x8B \x89",
"SELECT / / CANCEL \x87 \x88 \x85 \x86",
"ENTER DGPS / CANCEL \x8B \x89 \x80",
"LIBRARY GPS ENTER CANCEL \x85 \x86 \x80",
"SELECT / / CANCEL \x87 \x88 \x85 \x86",
"ENTER / / CANCEL \x8B \x89",
"LIBRARY COMPASS ENTER CANCEL \x85 \x86 \x80",
"SELECT / / CANCEL \x87 \x88 \x85 \x86 \x80",
"ENTER / / CANCEL",
"LIBRARY COMPUTE / CANCEL \x87 \x88 \x85 \x86 \x80",
"SELECT / / CANCEL \x87 \x88 \x85 \x86 \x80",
"ENTER / / CANCEL",
"LIBRARY COMPUTE / CANCEL \x87 \x88 \x85 \x86 \x80",
"SELECT / / CANCEL \x87 \x88 \x85 \x86 \x80",
"ENTER / / CANCEL",
"LIBRARY COMPUTE / CANCEL \x87 \x88 \x85 \x86 \x80",
"1_POINT 2_POINT 3_POINT CANCEL",
"POSITION NORTH COMPUTE CANCEL",
"SAVE / / CANCEL",
"ENABLE DISABLE / CANCEL \x92",
"CALL ACKN SAVE CANCEL \x86 \x85 \x87 \x88 \x80 \x92",
"UPDATE / SAVE CANCEL \x80 \x89",
"save / / cancel",
"SYSTEM LEVEL GUN METEO \x80 \x8B",
"UPDATE / ENTER CANCEL \x87 \x88 \x89 \x80 \x8B \x92",
"DEFAULT \x86Vo SAVE CANCEL",
"MODE / ENTER CANCEL \x80 \x89 \x8B",
"UPDATE / ENTER CANCEL \x87 \x88 \x89 \x80 \x8B \x92",
"TARGET CORRECT UNIT CANCEL",
"YES NO / CANCEL",
"COMMAND ABORT ACK CANCEL",
"ACKN NOTACK / EXIT \x86 \x85 \x88 \x87 \x93 \x8B",
"all single / cancel",
"new erase / cancel \x8B",
"DEFINE CALL COMPUTE EXIT \x89 \x87 \x88 \x86 \x85 \x80 \x8B \x92",
"LIBRARY SURVEY SETUP MESSAGE \x80 \x8B \x85 \x89 \x86 \x92 \x93 \x94",
"ACKN NOTACK / EXIT \x8B \x93 \x94 \x92",
"single all / cancel",
"next prev new erase \x88 \x85 \x86 \x8B \x95 \x80 \x92",
"single all / cancel",
"next prev new erase \x87 \x88 \x86 \x8B \x95 \x80 \x92",
"single all / cancel",
"next prev new erase \x87 \x88 \x8B \x95 \x80 \x92",
"select + - current \x88 \x85 \x86 \x8B \x95 \x80",
"select + - current \x88 \x85 \x8B \x95 \x80",
"next prev new / \x87 \x95 \x80 \x92",
"select find enum / \x87 \x88 \x85 \x8B \x95 \x80 \x92",
"polar cylind xyz map \x8B",
"enter / / cancel \x87 \x88 \x85 \x86 \x8B",
"local library / enter \x8B",
"enter / / cancel \x87 \x88 \x85 \x86 \x8B",
"exit / / / \x8B",
"north library freerun enter \x8B",
"6000 6400 / /",
"coord origin bearing mils \x8B",
"315\xb0 / / cancel",
"270\xb0 / / cancel",
"225\xb0 / / cancel",
"180\xb0 / / cancel",
"135\xb0 / / cancel",
"90\xb0 / / cancel",
"45\xb0 / / cancel",
"0\xb0 / / cancel",
"240\xb0 / / cancel",
"120\xb0 / / cancel",
"0\xb0 / / cancel",
"field 3-ref / cancel \x80 \x8C \x89 \x8B",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"SYSTEM TIME GPS",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"off switched continous",
"off on",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"NONE MORS DGPS",
"/",
"LATLON MAP",
"tg_2 tg_new",
"/",
"off on",
"LEVEL GPS MAP LINK SYSTEM",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"FIRE_REQUEST ENGAGEMENT",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"/",
"north south east west \x86 \x80",
NULL};
char *_MenuHead[]={
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"ser.no.",
"timeout",
"track",
"passwrd",
"service",
"screen",
"time",
"date",
"GMT_\x86t",
"station",
"alt",
"lat",
"lon",
"decl.",
"\x86grid",
"setup",
"___",
"pitch",
"roll",
"Toffset",
"Tscale",
"temp",
"datum",
"geoid",
"dx",
"dy",
"dz",
"PDOP",
"mode",
"dGPS",
"sector",
"offset",
"North",
"East",
"delay",
"mic",
"head",
"input",
"output",
"rate",
"aux",
"Version",
"coord",
"rld_T2",
"ENGLISH",
"Sophie",
"setup",
"alt",
"air___T",
"air___P",
"rel___H",
"wind_SP",
"wind_AZ",
"station",
"effect",
"fire",
"guns",
"proj.",
"fuze",
"charge",
"rounds",
"interv.",
"traj.",
"fire",
"___",
"azimuth",
"bearing",
"gun",
"proj.",
"pcs.",
"\x86m",
"\x86Tp",
"\x86Vo",
"unit",
"passwrd",
"",
NULL};
