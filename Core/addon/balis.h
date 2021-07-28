#define N 200				/* za domet do 20km pri min koraku 100m */
#define INIT 1
#define NORM 0

/* --------------- Strukture podatkov ------------------ */
struct Vlaga {               /* osnovni podatki atmosfere */
   double temp;              /* v stopinjah Celzija */
   double tau;               /* fiktivna temperatura */
   double h;                 /* pritisk v mm Hg */
   double em;                /* pritisk v zasicenem zraku */
   double rv;                /* procent relativne vlage */
   int err;                  /* parameter napake */
};

struct Atmosfera {          /* normalna atmosfera */
   double tau0;             /* temperatura na povrsini zemlje */
   double ha0;              /* pritisk na povrsini zemlje v mm Hg */
   double ro0;
   double a0;
   double ro0a0;
   double y;                /* visina nad povrsino zemlje */
   double tau;              /* temperatura */
   double hm;               /* pritisk v mm Hg */
   double ro;	    	    /* gostota */
   double a;    	        /* hitrost zvoka */
};

typedef struct tocbal {     /* tocka balisticnega koeficienta */
  double kot;               /* kot */
  double balk;              /* balisticni koeficient */
} POIBAL;

typedef struct tocder {     /* tocka derivacije */
  int kot;                  /* kot */
  int der;                  /* derivacija */
} POIDER;

typedef struct daljica {    /* opis daljice */
  double n;                 /* dvig */
  double k;                 /* naklonski koeficient */
} DALJ;

typedef struct tabbal {     /* opis tabele bal. koeficientov */
	float **kz;              /* kazalec na tabelo c-jev */
	int nk;                   /* stevilo kotov */
	int pv;                   /* stevilo polnjenj */
	float thm;               /* najmanjsi kot */
	float dth;               /* korak kota */
} BALMUN;

struct Lokalni {            /* Podatki poti v lokalnem koord.sist. */
   int vm;                  /* vrsta municije */
   int vp;                  /* vrsta polnjenja */
   int vo;					/* vrsta orozja	*/
   double y0;               /* startna nadmorska visina */
   double y1;               /* relativna visina cilja */
   double v0;               /* zacetna hitrost */
   double km;               /* popravek zaradi spremembe mase */
   double odsk;             /* odskocni kot */
   double tau0;             /* temperatura pri orozju */
   double ha0;              /* pritisk pri orozju */
   double a0;               /* hitrost zvoka pri orozju */
   double ro0;              /* gostota zraka pri orozju */
   double ro0a0;            /* produkt ro0 * a0 */
   double Wx;               /* vzdolzni veter */
   double Wz;               /* precni veter */
   double c;                /* balisticni koeficient */
   double *bkk;             /* na tabelo bal.koeficientov */
   int Pad;                 /* cilj na: 0-nar.delu,1-pad.delu */
   int Vse;                 /* 1-izracunaj vse, 0-ne vsega */
   int Zgor;                /* 1-zgornja pot, 0-spodnja pot */
   double theta0;           /* startni kot(rd) */
   double X;                /* Domet */
   double Xs;               /* oddaljenost temena */
   double Ys;               /* Visina temena */
   double Zs;               /* Odklon temena */
   double T;                /* Cas leta */
   double Ts;               /* Cas leta do temena */
   double vc;               /* hitrost padca */
   double thc;              /* kot padca(rd) */
   double Z;                /* odklon dometa */
   double fi;               /* odklon orozja */
   int Err;                 /* parameter napake */
   double der;              /* derivacija */
   double h;                /* korak x-a */
   double x;                /* trenutni x */
   int St;                  /* nacin racunanja v x */
   /* pomozni podatki - za hitrejsi racun */
   double C1;               /* pomozna konstanta */
   /* podatki za veckoracne sheme */
   int j;                   /* indeks koraka */
   double u[N];             /* tekoci u */
   double p[N];             /* tekoci p */
   double y[N];             /* tekoci y */
   double t[N];             /* tekoci t */
   double uc[N];            /* tekoci du/dx */
   double pc[N];            /* tekoci dp/dx */
   double tc[N];            /* tekoci dt/dx */
};

struct Lokalni1 {           /* Podatki poti v lokalnem koord.sist. */
   int vm;                  /* vrsta municije */
   int vp;                  /* vrsta polnjenja */
   double y0;               /* startna nadmorska visina */
   double y1;               /* relativna visina cilja */
   double v0;               /* zacetna hitrost */
   double km;               /* popravek zaradi spremembe mase */
   double odsk;             /* odskocni kot */
   double tau0;             /* temperatura pri orozju */
   double ha0;              /* pritisk pri orozju */
   double a0;               /* hitrost zvoka pri orozju */
   double ro0;              /* gostota zraka pri orozju */
   double Wx;               /* vzdolzni veter */
   double Wz;               /* precni veter */
   double c;                /* balisticni koeficient */
   double *bkk;             /* na tabelo bal.koeficientov */
   int Pad;                 /* cilj na: 0-nar.delu,1-pad.delu */
   int Vse;                 /* 1-izracunaj vse, 0-ne vsega */
   int Zgor;                /* 1-zgornja pot, 0-spodnja pot */
   double theta0;           /* startni kot(rd) */
   double X;                /* Domet */
   double Xs;               /* oddaljenost temena */
   double Ys;               /* Visina temena */
   double T;                /* Cas leta */
   double vc;               /* hitrost padca */
   double thc;              /* kot padca(rd) */
   double Z;                /* odklon dometa */
   double fi;               /* odklon orozja */
   int Err;                 /* parameter napake */
   double der;              /* derivacija */
   double h;                /* korak x-a */
   double x;                /* trenutni x */
   int St;                  /* nacin racunanja v x */
   /* pomozni podatki - za hitrejsi racun */
   double C1;               /* pomozna konstanta */
   /* podatki za enokoracne sheme */
   double y[4];             /* spremenljivke v x0 */
   double dy[4];            /* prirastki v x0 */
   double oy[4];            /* odvodi v x0 */
};

/* ---------------- Prototipi ------------------ */
double CD30(double m);                         /* etalonske funkcije */
double Franc(double ifp, double m, double j);
double CD43(double m);
double CD43A(double m);
double CD43B(double m);
double Siacci(double m);
double CD58(double m);

void FiktT(struct Vlaga *p);                   /* upostevanje vlage */
void RealT(struct Vlaga *p);

double TempSmod(double v, double dts);         /* korekcije podatkov */
double SpreMas(double v, double sprm, double *kc);

void Atmos(double y, struct Atmosfera *p);     /* atmosfere */
void AtmVent(double y, struct Atmosfera *p);
void AtmNaZac(struct Lokalni *I);

int Municija(int *vm);                         /* vnos podatkov */
int Polnjenje(int vm, int *kp);
int StartKot(double *kk, int vm);
int Domet(double *p);
int VisCilja(double *p);
int SprTemp(double *p);
int SprMas(double *p);
int SprObra(double *p);
int DelPoti(int *dp);
int TipPoti(int *tp);

double BalKoef0(double, double, int, double, double *);  /* bal.koef. */
double BalKoef1(int vm, double v0, double th0);
double BalKoef2(double, int, POIBAL *);
void PripBal(int vm, double v, double km, double *p);
double BalKoefI(double, double, int, double, double *);
void IzrBal(struct Lokalni1 *I);

void Deriv(struct Lokalni *);                 /* derivacija */

int PolHit(double v, float *pv, int nv);         /* pomozne funkcije */
void IzrHitr(struct Lokalni1 *I);               /* hitrost */

double Dudx(double c, double y, double v, struct Atmosfera *p);
double Dudx1(double u,double p,double y,struct Atmosfera *pp);
double FunkE(double u,double p,double y,struct Atmosfera *pp);

void KotPotRKx(struct Lokalni1 *I);             /* kot --> pot */
void KotPotRKx00(struct Lokalni1 *I);
int Doloci_X(struct Lokalni1 *I,double h0,double eps,double *p,int *i);
void OptKorRK1(struct Lokalni1 *I,double ep,double ep1,double *h,int *n);
double KorakH(double, double, int, double, double *);
void RKvarX(struct Lokalni1 *I);
void Merson(struct Lokalni1 *I);

void CiljnaTocka1(struct Lokalni1 *I);        /* elementi poti */
void Teme1(struct Lokalni1 *I);
void Teme(struct Lokalni *I);
void CiljnaTocka(struct Lokalni *I);
void ElCilja(struct Lokalni *I);
void RKkorak(struct Lokalni1 *I, struct Atmosfera *A);
void ElCilja1(struct Lokalni1 *I);

float BalKoef(float th, int vm, int vp);
void Rocni1(struct Lokalni *I);
void Rocni1A(struct Lokalni *I);
void Megla(double *Sp, double *Zg, double *Vsp, double *Vzg, double Xc,
	   struct Lokalni *I);
void MeglaV(double *Sp, double *Zg, double *Vsp, double *Vzg, double Xc,
	   struct Lokalni *I);
void Rocni2(struct Lokalni *I);
void Naklon1(struct Lokalni *I);
void RocniVet(struct Lokalni *I);
void RocniVetA(struct Lokalni *I);
void NaklonVetH(struct Lokalni *I);
void NaklonVetL(struct Lokalni *I);

/*****************************************************/
/* ----------------- Makro ukazi ------------------- */
/*****************************************************/

#define Koef(v,v0,v1) ((v-v0)/(v1-v0))
#define LinInt(k1,k2,c) ((k1+(k2-k1)*c))


