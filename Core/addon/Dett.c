#include	"remmenu.h"
/*..............................................................*/
int		n_samples(samples *p)
{
	return(*p->tp);
}
/*..............................................................*/
samples *freesamples(samples *p)
{
	if(p)
	{
		memfree(p->tp);
		memfree(p->rp);
		memfree(p->fp);
		memfree(p);
	}
	return(NULL);
}
/*..............................................................*/
samples	*init_samples(samples *p, int n)
{
	int	i,j;
	if(!p)
	{
		p=memalloc(sizeof(samples));
		p->tp=memalloc(4 * n * n *sizeof(double));
		p->fp=memalloc(2 * n * sizeof(double));
		p->rp=memalloc(2 * n * sizeof(double));
	}
	p->n = n;
	for(i=0; i<n; ++i)
	{
		p->fp[i]=0;
		for(j=0; j<n; ++j)
			p->tp[n*i + j]=0;
	}
	return(p);
}
/*..............................................................*/
void	add_sample(samples *p, double t, double f)
{
	int	i,j;
	for(i=0; i<p->n; ++i)
	{
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
void	add_sample_t(samples *p, double t, double f, double per)
{
	int	i,j;
	double	*q = memalloc(2*(p->n) * sizeof(double));

	for(i=0; i < (p->n)/2+1; ++i)
		q[i]=cos(2.0*M_PI*t/per*i);
	for(i=1; i < (p->n)/2+1; ++i)
		q[i+(p->n)/2]=sin(2.0*M_PI*t/per*i);
	for(i=0; i < p->n; ++i)
	{
		for(j=0; j < p->n ; ++j)
			p->tp[(p->n)*i+j]  +=  q[j]*q[i];
		p->fp[i]  +=  q[i]*f;
	}
	memfree(q);
}
/*..............................................................*/
/* Resitev lin. enacbe reda n-1, p= matrika koef. reda n x n	*/
/* a[0] + a[1]*p + a[2]*p^2 + ... a[n-1]*p^n-1 = q				*/
/* r = vektor resitev											*/
/* ce je DET(p)  ==  0, ni resitve, funkcija vrne NULL			*/
/*..............................................................*/
double	*solve(samples *p)
{
	int		i,j;
	double	*c,d;
	d=det(p->tp,0,0,p->n);
	if(!d)
		return(NULL);

	c=memalloc(p->n * p->n * sizeof(double));
	for(i=0; i<p->n; ++i)
	{
		for(j=0;j< p->n * p->n; ++j)
			if(j % p->n  ==  i)
				c[j]=p->fp[j / p->n];
			else
				c[j]=p->tp[j];
		p->rp[i]=det(c,0,0,p->n)/d;
	}
	memfree(c);
	return(p->rp);
}
/*..............................................................*/
/*   Izracun determinante matrike reda n x n, p=element (0,0)	*/
/*..............................................................*/
double	det(double *p, int x,int y, int n)
{
	int		i,j,sign;
	double 	D;

	watchdog();
	for(i=1, j=0, D=0, sign=1; j<n ; ++j, i *= 2)
	{
		if((i & y)  ==  0)
		{
			if(x < n-1)
			{
				D  +=  p[n*j+x]*det(p, x+1, y+i, n)*sign;
				sign *= -1;
			}
			else
				D=p[n*j+x];
		}
	}

	return(D);
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
/*..............................................................*/
/* Izracun trig. polinoma s koeficienti a, reda n, za vzorec t	*/
/*..............................................................*/
double	polyt(double t, double *a, double per,int n)
{
	int		i;
	double	ft;
	for(i=1, ft=a[0]; i<n; ++i)
		ft  +=  a[i]*cos(i*2.0*M_PI*t/per) + a[i+n-1]*sin(i*2.0*M_PI*t/per);
	return(ft);
}


