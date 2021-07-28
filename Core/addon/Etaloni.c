/* */
/* Etalonske funkcije zunanje balistike */
/* Standardne atmosfere */
/* Upostevanje vlage zraka */
/* */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "balis.h"

/* ------------------ Konstante ---------------------*/
extern double cro;
/*------------------------------------------------*/
/*:::::::::::: STANDARDNI ATMOSFERI ::::::::::::::*/
/*------------------------------------------------*/

/**************************************************/
/* Atmos izracuna podatke ICAO normalne atmosfere */
/* y..visina nad povrsjem. p..podatki atmosfere   */
/**************************************************
void Atmos(double y, struct Atmosfera *p)
{
    p->y = y;
    p->tau = 288.16 - 0.0065 * y;
    p->hm = 760.0 * pow(1.0 - 0.00002256 * y, 5.2561);
    p->a = 20.0484413 * sqrt(p->tau);
    p->ro = 0.4645673 * p->hm / p->tau;
}
*/
/**************************************************/
/* AtmVent izracuna podatke Ventceljeve atmosfere */
/* y..visina nad morjem. p..podatki atmosfere     */
/**************************************************/
void AtmVent(double y, struct Atmosfera *p)
	{
	p->y = y;
	p->tau = p->tau0 - 0.006328 * y;
	p->hm = p->ha0 * pow(p->tau/p->tau0, 5.4);
	p->a = 20.0484413 * sqrt(p->tau);
	p->ro = 0.4643761 * p->hm / p->tau;
}
/***************************************************/
/* AtmNaZac izracuna podatke Ventceljeve atmosfere */
/* ob izstreliscu. V strukturi I pusti rezultate.  */
/* Tam dobi tudi visino izstrelisca y0             */
/***************************************************/
void AtmNaZac(struct Lokalni *I)
{
    I->tau0 = 288.9 - 0.006328 * I->y0;
    I->ha0 = 750.0 * pow(I->tau0/288.9, 5.4);
    I->a0 = 20.0484413 * sqrt(I->tau0);
    I->ro0 = 0.4643761 * I->ha0 / I->tau0;
    I->ro0a0 = I->a0 * I->ro0;
}
/*------------------------------------------------*/
/*:::: Prevedba realne atmosfere na fiktivno :::::*/
/*------------------------------------------------*/
/* ------------ Pritisk zasicenega zraka -------- */
/* od -20 do +40								  */
float emji[62] = 
	{0.77, 0.85, 0.94, 1.03, 1.13, 1.24, 1.36, 1.49,
	1.63, 1.78, 1.95, 2.13, 2.32, 2.53, 2.76, 3.01,
	3.29, 3.57, 3.88, 4.22, 4.58, 4.90, 5.30, 5.70,
	6.10, 6.50, 7.00, 7.50, 8.10, 8.60, 9.20, 9.80,
	10.5, 11.2, 12.0, 12.8, 13.6, 14.5, 15.5, 16.5,
	17.5, 18.7, 19.8, 21.1, 22.4, 23.8, 25.2, 26.7,
	28.4, 30.0, 31.8, 33.7, 35.7, 37.7, 39.9, 42.2,
	44.6, 47.1, 49.7, 52.4, 55.3, 55.3};
/*************************************************/
/* FiktT dobi podatek o realni temperaturi, pri- */
/* tisku in relativni vlaznosti. Izracuna fiktiv */
/* no temperaturo                                */
/*************************************************/
void FiktT(struct Vlaga *p) {
	int i, ti;
	double temp;

	ti = floor(p->temp);
	i = ti + 20;
	if(i<0)
		i=0;
	if(i>60)
		i=60;
	p->em = emji[i] + (p->temp - ti) * (emji[i+1] - emji[i]);
	temp = 1.0 - (3.0 * p->rv * p->em)/(800.0 * p->h);
	p->tau = (273.0 + p->temp)/ temp;
}
/***********************************************/
/* RealT dobi podatek o pritisku, relativni    */
/* vlaznosti in fiktivni temperaturi. Izracuna */
/* dejansko temperaturo v stopinjah Celzija    */
/***********************************************/
/*
void RealT(struct Vlaga *p)
{
   double A, B, pom, t, flti;
   int ti, i;

   p->err = 0;
   pom = (3.0 * p->rv) / (800.0 * p->h);
   if (p->tau < (253.0/(1.0 - pom * emji[0]))) {p->err = 1; return;}
   if (p->tau > (313.0/(1.0 - pom * emji[60]))) {p->err = 2; return;}
   A = p->tau - 273;
   B = pom * p->tau;
   ti = floor(A);
   if (ti > 39) ti = 39;
   flti = ti;
   i = ti + 20;
   A = A / B;
   B = 1.0 / B;
   while (1) {
     pom = emji[i+1] - emji[i];
	 t = (A - emji[i] + flti * pom)/(B + pom);
     if (t < flti) {
       i--; flti = flti - 1.0;
       if (i < 0) {p->err = 1; return;}
     } else
       if (t > (flti + 1.0)) {
       i++; flti = flti + 1.0;
	   if (i > 59) {p->err = 2; return;}
     } else {
       p->temp = t;
       return;
	 }
   }
}
*/
/*------------------------------------------------*/
/*::::::::::::: ETALONSKE FUNKCIJE :::::::::::::::*/
/*------------------------------------------------*/
/**************************************************/
/*    Franc izracuna CD(M) za francoske poskuse   */
/* ifp.faktor oblike, m..Machovo stevilo, j..par  */
/**************************************************/
/*
double Franc(double ifp, double m, double j)
{
    double cx1, cx2, pom, pom1, ma, no, nop;
    double mi, lambda, alfa, f;

    if (m > 1.0) {
       cx2 = 2.105 /(m * m);
       pom = (m - 1.94); pom = 5.5 * pom * pom;
       cx1 = cx2 * (m - 0.5 + 0.166 * (m - 2.05) / pow(10.0, pom));
    } else {
       if ( m > 0.8414) {
	   pom = 2.105 / (m * m);
	   pom1 = pow(10.0, 5.8 * (1.0 - m));
	   cx1 = pom * (0.14 + 0.36 / pom1);
	   cx2 = pom * (0.35 + 0.65 / pom1);
       } else {
	   pom = 0.0372 - 0.03 * j;
	   ma = 0.8414 / pow(10.0, pom);
	   if (m > ma) {
		  no = (4.670 - 8.432 * j)/(1.760 - 4.110 * j);
	      nop = no * (12.237 - no)/0.4343;
		  mi = nop * pom /(no - 2.0) - 1.0;
	      lambda = nop/ pow(pom, mi);
	      alfa = lambda / ((mi + 1.0) * (mi + 2.0));
	      f = alfa * pow(log10(m / ma), (mi + 2.0));
		  pom1 = pow(10.0, f);
		  cx1 = 0.538 * pom1;
		  cx2 = 1.283 * pom1;
	   } else {
		  cx1 = 0.538;
		  cx2 = 1.283;
	   }
	   }
	}
	return (ifp * (cx1 - j * cx2));
}
*/
/**************************************************/
/*   CD30(M) izracuna ruski etalon iz leta 1930   */
/**************************************************/
/*
double CD30(double m)
{
   return(Franc(0.365, m, 0.03));
}
*/
/**************************************************/
/*   CD43(M) izracuna ruski etalon iz leta 1943   */
/**************************************************/

double CD43(double m)
{
   static double a1[] = {-2.083333, 1.180556, -0.3125,
			 0.038056, -0.001667, 0.157};
   static double a2[] = {613.333335, -233.33334, 39.0,
			-2.813333, 0.068667, 0.158};
   static double a4[] = {-78.411093, 43.245043, 1.803950,
			-3.74780, 0.88697, 0.325};
   static double a5[] = {1829.806015, -426.848831, 42.480376,
			-1.984508, 0.015829, 0.385};
   static double a6[] = {0.030874, -0.117636, 0.209691,
			-0.151862, -0.05038, 0.385};
   double cd, sm;
   int i;

   if (m < 0.6) {
      cd = 1.388889;
      sm = m;
      for (i = 0; i < 6; i++)
	 cd = cd * sm + a1[i];
   } else if (m < 0.9) {
      cd = -533.333337;
      sm = m - 0.6;
      for (i = 0; i < 6; i++)
	 cd = cd * sm + a2[i];
   } else if (m < 1.0) {
      cd = -1.025 + 1.35 * m;
   } else if (m < 1.2) {
      cd = -445.15561;
      sm = m - 1.0;
      for (i = 0; i < 6; i++)
	 cd = cd * sm + a4[i];
   } else if (m < 1.4) {
      cd = -2831.379493;
      sm = m - 1.2;
      for (i = 0; i < 6; i++)
	 cd = cd * sm + a5[i];
   } else if (m < 3.56) {
      cd = -0.003049;
	  sm = m - 1.2;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a6[i];
   } else {
	  cd = 0.260;
   }
   return cd;
}

/**************************************************/
/*   CD43A(M) izracuna ruski etalon iz leta 1943  */
/**************************************************/
/*
double CD43A(double m)
{
   int mc;
   double fmc, osc;

   static double m0 = 0.156993;
   static double mx = 0.260;
   static DALJ dalj[] = {{0.156881, 0.009617},{0.158331,-0.00502},
	  {0.158012,0.019063},{0.0,0.0},{0.190,1.35},{0.0,0.0},
	  {0.0,0.0},{0.3853,-0.029659},{0.3825,-0.122665},
	  {0.370395,-0.093554},{0.36101,-0.097426},
	  {0.351246,-0.096451},{0.341587,-0.092151},
	  {0.332364,-0.085774},{0.323783,-0.078319},
	  {0.315952,-0.070557},{0.308898,-0.063053},
	  {0.302597,-0.056189},{0.296983,-0.050184},
	  {0.291970,-0.045117},{0.287462,-0.040951},
	  {0.283371,-0.037550},{0.279618,-0.034705},
	  {0.276148,-0.032157},{0.272932,-0.029613},
	  {0.269968,-0.026774},{0.267287,-0.023354},
	  {0.264947,-0.019104},{0.263031,-0.013830},
	  {0.261642,-0.007421}};

   fmc = floor(10.0 * m); osc = m - 0.1 * fmc;
   mc = fmc;
   switch (mc) {
     case  8:if (m < 0.85) return (0.160067+osc*(0.015797+osc*2.028127));
	     return (0.168735+osc*(-0.317364+osc*5.290673));
     case 10:return (0.325241+osc*(0.846737-osc*2.858832));
     case 11:return (0.382013+osc*(0.255183+osc*0.592637));
     case 35:if (m < 3.56) return (0.260926-osc*0.001485);
	     return (mx);
     default:if (mc < 5) return (m0);
	     if (mc > 35) return (mx);
	     mc -= 5;
	     return (dalj[mc].n + osc * dalj[mc].k);
   }
}
*/
/**************************************************/
/*   CD43B(M) izracuna ruski etalon iz leta 1943  */
/**************************************************
double CD43B(double m)
{
   int mc;

   static double m0 = 0.156993;
   static double mx = 0.260;
   static DALJ dalj[] = {{0.152072, 0.009617},{0.161343,-0.00502},
	  {0.144668,0.019063},{0.0,0.0},{-1.025,1.35},{0.0,0.0},
	  {0.0,0.0},{0.420891,-0.029659},{0.541965,-0.122665},
	  {0.501370,-0.093554},{0.507148,-0.097426},
	  {0.505568,-0.096451},{0.498244,-0.092151},
	  {0.486757,-0.085774},{0.472589,-0.078319},
	  {0.457065,-0.070557},{0.441309,-0.063053},
	  {0.426212,-0.056189},{0.412405,-0.050184},
	  {0.400251,-0.045117},{0.389839,-0.040951},
	  {0.381000,-0.037550},{0.373322,-0.034705},
	  {0.366187,-0.032157},{0.358808,-0.029613},
	  {0.350289,-0.026774},{0.339685,-0.023354},
	  {0.326079,-0.019104},{0.308671,-0.013830},
	  {0.286873,-0.007421}};

   mc = 10.0 * m;
   switch (mc) {
     case  8:if (m < 0.85) return (1.445432+m*(-3.229207+m*2.028127));
	     return (3.808657+m*(-8.782441+m*5.290673));
     case 10:return (-3.380328+m*(6.564402-m*2.858832));
     case 11:return (0.818403+m*(-1.048619+m*0.592637));
     case 35:if (m < 3.56) return (0.266124-m*0.001485);
	     return (mx);
     default:if (mc < 5) return (m0);
	     if (mc > 35) return (mx);
	     mc -= 5;
	     return (dalj[mc].n + m * dalj[mc].k);
   }
}
*/
/**************************************************/
/*   Siacci izracuna CDS(m). m..Machovo stevilo   */
/**************************************************/
/*
double Siacci(double m)
{
    double p, r;

    p = 15.06 * m * (340.8 * m - 300.0);
    p = p / (371.0 + pow(1.704 * m, 10.0));
    r = 56.16 * m - 47.95;
    p = p + sqrt(r * r + 9.6);
    p = p + 68.23 * m - 48.05;
    p = 0.018193 * p / (m * m);
    return (p);
}
*/
/**************************************************/
/*   CD58(M) izracuna ruski etalon iz leta 1958   */
/**************************************************/
/*
double CD58(double m)
{
   static double a1[] = {21.25, -16.180556, 5.270833,
			-0.687222, 0.030167, 0.305};
   static double a2[] = {226.666656, -147.777774, 27.833333,
			-1.231111, 0.049, 0.308};
   static double a3[] = {-4419.755242, 2248.203898, -537.940326,
			54.743605, -0.244607, 0.382};
   static double a4[] = {145.877457, -12.416404, -1.823884,
			-0.661406, -0.045178, 0.618};
   static double a5[] = {-8.690476, 7.384921, -3.115476,
			0.735754, -0.258976, 0.559};
   static double a6[] = {6.750595, -6.739633, 3.218899,
			-0.712594, -0.054839, 0.449};
   static double a7[] = {0.012492, -0.038792, 0.052014,
			-0.017006, -0.072389, 0.367};
   double cd, sm;
   int i;

   if (m < 0.6) {
	  cd = -9.722222;
	  sm = m - 0.1;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a1[i];
   } else if (m < 0.9) {
	  cd = 88.8889;
	  sm = m - 0.6;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a2[i];
   } else if (m < 1.2) {
	  cd = 3342.112572;
	  sm = m - 0.9;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a3[i];
   } else if (m < 1.5) {
	  cd = -261.433011;
	  sm = m - 1.2;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a4[i];
   } else if (m < 2.2) {
	  cd = 3.968254;
	  sm = m - 1.5;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a5[i];
   } else if (m < 3.0) {
	  cd = -2.600198;
	  sm = m - 2.2;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a6[i];
   } else if (m < 5.2) {
	  cd = -0.001397;
	  sm = m - 3.0;
	  for (i = 0; i < 6; i++)
	 cd = cd * sm + a7[i];
   } else {
	  cd = 0.256;
   }
   return cd;
}

*/


