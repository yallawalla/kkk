/* */
/* Programi za izracun balisticnih koeficientov */
/* in derivacije */
/* */

#include "remmenu.h"
#include "balis.h"

extern float **adrvji;      /* hitrosti */
extern BALMUN *Bali;       /* c-ji ~~~ array v pointer */
/* extern POIDER **ptder[];     derivacije */
/* extern int *nder[];      	 stevilo podatkov */
/* extern double Philj;         hilj / 2 */

/*------------------------------------------------*/
/*::::::::::: BALISTICNI KOEFICIENTI :::::::::::::*/
/*------------------------------------------------*/

/**********************************************/
/* BalKoef0 izracuna balisticni koeficient za */
/* ekvidistancno podane kote. th..startni kot */
/* thz..zacetni kot tabele, n..podatkov v     */
/* tabeli, dth..korak tabele, p..kazalec na   */
/* tabelo. Ce je kot premajhen,vzame vrednost */
/* ki pripada najmanjsemu kotu.Podobno naredi */
/* pri prevelikem kotu                        */
/**********************************************/
/*
double BalKoef0(double th, double thz, int n, double dth, double *p)
{
   double ifg, ksi1, c, *p1, k1, k2;
   int i, n1;

   n1 = n - 1;
   ifg = (th - thz) / dth;
   i = ifg;
   if (i < 0) return (*p);
   if (i >= n1) return (*(p+n1));
   ksi1 = ifg - i;
   p1 = p + i;
   k1 = *p1; p1++;
   k2 = *p1;
   c = (k1 + ksi1*(k2 - k1));
	  return (c);
}
*/
/**********************************************/
/* PolHit vrne indeks na polozaj hitrosti v   */
/* tabeli hitrosti. v..hitrost, pv..kazalec   */
/* na tabelo hitrosti, nv..stevilo podatkov   */
/**********************************************/
int PolHit(double v, float *pv, int nv)
{
   int		iv, j;
   float	*pp;

   iv = v; pp = pv;
   for (j = 0; j < nv; j++) {
	 if (iv < *pp) return (j);
	 pp++;
   }
   return (nv);
}

/**************************************************/
/* BalKoef1 izracuna balisticni koeficient za po- */
/* dano hitrost v0 in startni kot th. th..startni */
/* kot, v..hitrost, vm..vrsta municije            */
/* Ce je kot premajhen, vzame vrednost,ki pripada */
/* najmanjsemu kotu.Podobno naredi pri prevelikem */
/* kotu (v radianih). Korekcijo zaradi mase upos- */
/* tevamo takole:                       	  */
/*    cb = km * BalKoef1()                        */
/**************************************+***********/
double BalKoef1(int vm, double v, double th)
{
   int i, j, n1, n2, nv;
   float ifg, c, ksi1, **pb;
   double cA, cB;
   float  *pv;

   nv = Bali[vm].pv;              /* polnjenj */
   n1 = Bali[vm].nk - 1;          /* kotov */
   n2 = nv - 1;
   pv = adrvji[vm];               /* hitrosti */
   pb = Bali[vm].kz;              /* tabela c-jev */
   ifg = (th - Bali[vm].thm) / Bali[vm].dth;
   i = ifg;
   if (i < 0) {
	  j = PolHit(v, pv, nv);
      if (j == 0) return (**pb);
      if (j == nv) return (pb[n2][0]);
	  c = Koef(v, pv[j-1], pv[j]);
      return(LinInt(pb[j - 1][0], pb[j][0], c ));
   }
   if (i >= n1) {
	  j = PolHit(v, pv, nv);
      if (j == 0) return (pb[0][n1]);
      if (j == nv) return (pb[n2][n1]);
	  c = Koef(v, pv[j-1], pv[j]);
      return (LinInt(pb[j - 1][n1], pb[j][n1], c));
   }
   ksi1 = (ifg - i);
   j = PolHit(v, pv, nv);
   if (j == 0) {
     return (LinInt(pb[0][i],pb[0][i+1],ksi1));
   }
   if (j == nv) {
     return (LinInt(pb[n2][i],pb[n2][i+1],ksi1));
   }
   cA = LinInt(pb[j-1][i],pb[j-1][i+1],ksi1);
   cB = LinInt(pb[j][i],pb[j][i+1],ksi1);
   c = Koef(v, pv[j-1], pv[j]);
   c = cA + (cB - cA) * c;
   return (c);
}

/**********************************************/
/* BalKoef2 izracuna balisticni koeficient za */
/* neekvidistancno podane kote. th..startni   */
/* n..podatkov v tabeli,p..kazalec na tabelo. */
/* Ce je kot premajhen,vzame vrednost, ki     */
/* pripada najmanjsemu kotu.Podobno je  pri   */
/* prevelikem kotu                            */
/**********************************************/
/*
double BalKoef2(double th, int n, POIBAL *p)
{
   int j;
   double c;
   POIBAL *q;

   if (th <= p->kot) return(p->balk);
   for (j = 0; j < n - 1; j++) {
	 p++;
	 if (th < p->kot) {
	   q = p--;
       c = Koef(th, q->kot, p->kot);
       return(LinInt(q->balk, p->balk, c));
     }
   }
   return(p->balk);
}
*/
/*****************************************/
/* PripBal pripravi tabelo balisticnih   */
/* koeficientov za hitrost v, in vrsto   */
/* municije vm. Izvedena je tudi korek-  */
/* cija zaradi spremembe mase (km).      */
/* Rezultati so v vektorju, na katerega  */
/* kaze kazalec p                        */
/*****************************************/
/*
void PripBal(int vm, double v, double km, double *p)
{
  int i, j, nk, np;
  float c, *pp, *pp1, *pv;

  np = Bali[vm].pv;
  nk = Bali[vm].nk;
  pv = adrvji[vm];
  j = PolHit(v, pv, np);
  if (j == 0) {
	pp = Bali[vm].kz[0];
	for (i = 0; i < nk; i++) {
      *p = km * (*pp);
      p++; pp++;
    }
    return;
  }
  if (j == np) {
    pp = Bali[vm].kz[np-1];
    for (i = 0; i < nk; i++) {
      *p = km * (*pp);
      p++; pp++;
    }
    return;
  }
  c = Koef(v, pv[j-1], pv[j]);
  pp = Bali[vm].kz[j-1];
  pp1 = Bali[vm].kz[j];
  for (i = 0; i < nk; i++) {
	*p = km * LinInt(*pp, *pp1, c);
	p++; pp++; pp1++;
  }
}
*/
/**********************************************/
/* BalKoefI izracuna balisticni koeficient za */
/* ekvidistancno podane kote. th..startni kot */
/* thz..zacetni kot tabele, n..podatkov v     */
/* tabeli, dth..korak tabele, p..kazalec na   */
/* tabelo. Ce je kot premajhen,vzame vrednost */
/* ki pripada najmanjsemu kotu.Podobno naredi */
/* pri prevelikem kotu                        */
/**********************************************/
/*
double BalKoefI(double th, double thz, int n, double dth, double *p)
{
   double ifg, ksi1, c, *p1, k1, k2;
   int i, n1;

   n1 = n - 1;
   ifg = (th - thz) / dth;
   i = ifg;
   if (i < 0) return (*p);
   if (i >= n1) return (*(p+n1));
   ksi1 = ifg - i;
   p1 = p + i;
   k1 = *p1; p1++;
   k2 = *p1;
   c = (k1 + ksi1*(k2 - k1));
   return (c);
}
*/

/**********************************************/
/* Deriv izracuna derivacijo za podatke iz    */
/* strukture Lokalni (vm, vp, theta0). Tudi   */
/* rezultat se je v strukturi Lokalni (der)   */
/**********************************************/
/*
void Deriv(struct Lokalni *I)
{
   int j, n;
   double kp, Hi;
   POIDER *p, *q;

   kp = Philj / pi;
   Hi = kp * I->theta0;
   p = ptder[I->vm][I->vp];
   n = nder[I->vm][I->vp];
   if (Hi <= p->kot) {I->der = p->der/kp; return;}
   for (j = 0; j < n - 1; j++) {
	 p++;
	 if (Hi < p->kot) {
	   q = p--;
	   I->der = (q->der+ (p->der - q->der)*(Hi - q->kot)/
		(p->kot - q->kot))/kp;
	   return;
	 }
   }
   I->der =  p->der / kp;
}
*/

