#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
/*..............................................................*/
/* 	definicija strukture samples
	tp - delovna vsota t
	fp - delovna vsota f(t)
	rp - vrsta koeficientov resitve
	n  - zahtevana stopnja resitve
*/
typedef	struct samples samples;
struct 	samples	{
		double	*tp,*fp,*rp;
		int		n;
		};
/*..............................................................*/
/* vrne stevilo vzorcev v strukturi								*/
int		n_samples(samples *p) {
		return((int)*p->tp);
		}
/*..............................................................*/
/* izprazni pomnilnik											*/
samples *freesamples(samples *p) {
		free(p->tp);
		free(p->rp);
		free(p->fp);
		free(p);
		return(NULL);
		}
/*..............................................................*/
/* inicializacija strukture pred zacetkom			*/
samples	*init_samples(samples *p, int n) {
		int	i,j;
		if(!p) {
			p=(samples *)malloc(sizeof(samples));
			p->tp=(double *)malloc(4 * n * n *sizeof(double));
			p->fp=(double *)malloc(2 * n * sizeof(double));
			p->rp=(double *)malloc(2 * n * sizeof(double));
			}
		p->n = n;
		for(i=0; i<n; ++i) {
			p->fp[i]=0;
			for(j=0; j<n; ++j)
				p->tp[n*i + j]=0;
			}
		return(p);
		}
/*..............................................................*/
/* dodajanje vzorcev v strukturo				*/
void	add_sample(samples *p, double t, double f) {
		int	i,j;
		for(i=0; i<p->n; ++i) {
			for(j=0; j<p->n; ++j)
				if(i+j)
					p->tp[i*(p->n)+j]  +=  pow(t,i+j);
				else
					p->tp[i*(p->n)+j]  +=  1.0;
			if(i)
				p->fp[i]  +=  f*pow(t,i);
			else
				p->fp[i]  +=  f;
			}
		}
/*..............................................................*/
/*   Izracun determinante matrike reda n x n, p=element (0,0)	*/
/*..............................................................*/
double	det(double *p, int x,int y, int n) {
		int		i,j,sign;
		double 	D;
		for(i=1, j=0, D=0, sign=1; j<n ; ++j, i *= 2)
			if((i & y)  ==  0)
				if(x < n-1)
					{
					D  +=  p[n*j+x]*det(p, x+1, y+i, n)*sign;
					sign *= -1;
					}
				else
					D=p[n*j+x];
		return(D);
		}
/*..............................................................*/
/* Resitev lin. enacbe reda n-1, p= matrika koef. reda n x n	*/
/* a[0] + a[1]*p + a[2]*p^2 + ... a[n-1]*p^n-1 = q		*/
/* r = vektor resitev						*/
/* ce je DET(p)  ==  0, ni resitve, funkcija vrne NULL		*/
/*..............................................................*/
double	*solve(samples *p) {
		int		i,j;
		double	*c,d;

		d=det(p->tp,0,0,p->n);
		if(!d)
			return(NULL);

		c=(double *)malloc(p->n * p->n * sizeof(double));
		for(i=0; i<p->n; ++i) {
			for(j=0;j< p->n * p->n; ++j)
				if(j % p->n  ==  i)
					c[j]=p->fp[j / p->n];
				else
					c[j]=p->tp[j];
			p->rp[i]=det(c,0,0,p->n)/d;
			}
		free(c);
		return(p->rp);
		}
/*..............................................................*/
/* Izracun polinoma s koeficienti a, reda n, za vzorec t !!!	*/
/*..............................................................*/
double	polyp(double t, double *a, int n)
{
		int		i;
		double	ft;
		for(i=1, ft=a[0]; i<n; ++i)
			ft  +=  a[i]*pow(t,i);
		return(ft);
}
///*..............................................................*/
///* primer tretjega reda */

//void main()
//{
//	samples *p=NULL;
//	double	*k;
//	double	d;

//	p=init_samples(p,3);

//	add_sample(p,1,1);
//	add_sample(p,2,4);
//	add_sample(p,3,9);
//	add_sample(p,4,16);
//	add_sample(p,5,25);
//	add_sample(p,6,36);
//	add_sample(p,7,49);
//	add_sample(p,8,64);
//	add_sample(p,9,81);

//	k=solve(p);
//	if(k)
//		d=polyp(-4,k,3);
//}