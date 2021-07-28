#include	"remmenu.h"
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			close_srv(void *vp)
			{
			Sn=freelibrary(Sn);
			freelib(LL);
			LL=makelib();
			clrlcd();
			PullMode(NULL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			MakePos(void *vp)
			{
			freelib(LL);
			copylib(OBP,LL=makelib());

			origin_local(NULL);
			bearing_local(NULL);

			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			MakeTraverse(void *vp)
			{
			lib	*p;
			Sn=freelibrary(Sn);
			Sn=p=makelib();

			Sn->ltype=addtype(".r");
			Sn->txt=addtxt(Sn,T_REFP1);
			Sn->n=1;
			Sn->next=makelib();

			Sn=Sn->next;
			Sn->ltype=addtype(".m");
			Sn->txt=addtxt(Sn,T_MEAS1);
			Sn->n=1;
			Sn->next=makelib();

			Sn->next=p;
			Sn=p;

			origin_local(NULL);
			bearing_local(NULL);

			copylib(Sn,LL);
			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			MakeTrilat(void *vp)
			{
			lib	*p;
			Sn=freelibrary(Sn);
			Sn=p=makelib();

			Sn->ltype=addtype(".r");
			Sn->txt=addtxt(Sn,T_REFP1);
			Sn->n=1;
			Sn->next=makelib();
			Sn=Sn->next;

			Sn->ltype=addtype(".r");
			Sn->txt=addtxt(Sn,T_REFP2);
			Sn->n=2;
			Sn->next=makelib();
			Sn=Sn->next;

			Sn->ltype=addtype(".m");
			Sn->txt=addtxt(Sn,T_MEAS2);
			Sn->n=2;
			Sn->next=makelib();
			Sn=Sn->next;

			Sn->ltype=addtype(".m");
			Sn->txt=addtxt(Sn,T_MEAS1);
			Sn->n=1;

			Sn->next=p;
			Sn=p;

			origin_local(NULL);
			bearing_local(NULL);

			copylib(Sn,LL);
			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			MakeTriang(void *vp)
			{
			lib	*p;
			Sn=freelibrary(Sn);
			Sn=p=makelib();

			Sn->ltype=addtype(".r");
			Sn->txt=addtxt(Sn,T_REFP1);
			Sn->n=1;
			Sn->next=makelib();

			Sn=Sn->next;
			Sn->ltype=addtype(".r");
			Sn->txt=addtxt(Sn,T_REFP2);
			Sn->n=2;
			Sn->next=makelib();

			Sn=Sn->next;
			Sn->ltype=addtype(".r");
			Sn->txt=addtxt(Sn,T_REFP3);
			Sn->n=3;
			Sn->next=makelib();

			Sn=Sn->next;
			Sn->ltype=addtype(".b");
			Sn->txt=addtxt(Sn,T_BEAR3);
			Sn->n=3;
			Sn->next=makelib();

			Sn=Sn->next;
			Sn->ltype=addtype(".b");
			Sn->txt=addtxt(Sn,T_BEAR1);
			Sn->n=1;
			Sn->next=makelib();

			Sn=Sn->next;
			Sn->ltype=addtype(".b");
			Sn->txt=addtxt(Sn,T_BEAR2);
			Sn->n=2;

			Sn->next=p;
			Sn=p;

			origin_local(NULL);
			bearing_local(NULL);

			copylib(Sn,LL);
			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			MakeNorth(void *vp)
			{
			freelib(LL);
			copylib(OBP,LL=makelib());
			LL->x=LL->xc=freecoord(LL->x);
			shaft_enable();
			add_coord(LL,read_EL(),read_AZ(),ELAZ_defr);
			RelToLla(LL,LL,OBP,NULL,EL_AZ_R);

			origin_local(&OBP);
			bearing_local(NULL);
			d_mode=EL_AZ;
			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis Ln v LL in izpis		                          */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			LntoLL(void *vp)
			{
			copylib(Ln,LL);
			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* prepis Sn v LL in izpis		                          */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			SntoLL(void *vp)
			{
			copylib(Sn,LL);
			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/* Prenos knjiznice LL v Sn 							  */
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			LLtoSn(void *vp)
			{
			if(LL && LL->xc) {
				copycoord(LL,Sn);
				Sn->txt=addtxt(Sn,LL->txt->t);
				copylib(Sn,LL);
				}
			copylib(Sn,LL);
			Displc(LL);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int			trilaterate(void *vp)
			{
			lib		*l,*P1,*P2,*A1,*A2;
			double	x;

			l=checklib(Sn,addtype(".r"),1);
			if(!l || !l->xc) {
				err(E_ITEMR01);
				return(1);
				}
			copycoord(l,P1=makelib());

			l=checklib(Sn,addtype(".r"),2);
			if(!l || !l->xc) {
				err(E_ITEMM01);
				freelib(P1);
				return(1);
				}
			copycoord(l,P2=makelib());

			l=checklib(Sn,addtype(".m"),1);
			if(!l || !l->xc) {
				err(E_ITEMR02);
				freelib(P1);
				freelib(P2);
				return(1);
				}
			copycoord(l,A1=makelib());
			LlaToRel(A1,A1,OBP,NULL,AZ_R_dZ);

			l=checklib(Sn,addtype(".m"),2);
			if(!l || !l->xc) {
				err(E_ITEMM02);
				freelib(P1);
				freelib(P2);
				freelib(A1);
				return(1);
				}
			copycoord(l,A2=makelib());
			LlaToRel(A2,A2,OBP,NULL,AZ_R_dZ);

/*		kopija trenutnega OBP v LL
*/
			freelib(LL);
			copylib(OBP,LL=makelib());

/*      poravnava referencnih tock P1 in P2 v ravnino OBP
		glede na izmerjene vrednosti v A1 in A2
		altitude = povprecje obeh tock
*/
			P1->xc->z -= A1->xc->z;
			P2->xc->z -= A2->xc->z;

/*		kot pri P1 = atan*L2*sin(fi)/(L1-L2*cos(fi))(
		fi = razlika kotov do P1 in P2
*/
			x=(A2->xc->x - A1->xc->x)/maxmils*2.0*M_PI;
			LL->xc->x = atan2(A2->xc->y * sin(x),
							A1->xc->y - A2->xc->y * cos(x));
			LL->xc->x *= maxmils/2.0/M_PI;

/*		razdalja do P1
*/
			LL->xc->y = A1->xc->y;
			LL->xc->z = 0;

			RelToLla(LL,LL,P1,P2,AZ_R_dZ);
			LL->xc->z = (P1->xc->z + P2->xc->z)/2 ;
/*
		sprememba orientacije
*/
			copycoord(P1,A2);
			LlaToRel(A2,A2,LL,NULL,AZ_R_dZ);
			az_offset = az_off + (A1->xc->x - A2->xc->x)/maxmils*2.0*M_PI;
			while(az_offset > M_PI)
				az_offset -= 2.0*M_PI;

			Displc(LL);

			freelib(P1);
			freelib(P2);
			freelib(A1);
			freelib(A2);
			return(null);
			}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int	traverse(void *vp)
{
	lib		*Ro,*Mo;

	Ro=checklib(Sn,addtype(".r"),1);
	Mo=checklib(Sn,addtype(".m"),1);
	if(!Ro || !Ro->xc)
	{
		err(E_ITEMR01);
		return(1);
	}
	if(!Mo || !Mo->xc)
	{
		err(E_ITEMM01);
		return(1);
	}
	copylib(OBP,LL);
	LlaToRel(Mo,LL,OBP,NULL,EL_AZ_R);
	LL->xc->x *= -1;
	LL->xc->y += maxmils/2;
	RelToLla(LL,LL,Ro,NULL,EL_AZ_R);
	Displc(LL);
	return(null);
}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		triangulate(void *vp)
		{
		int		i=1;
		lib		*l;
		double	alfa,beta,x;
		lib		*q	=	makelib();
		lib		*c1	=	makelib();
		lib		*c2	=	makelib();
		lib		*P0	=	makelib();
		lib		*P1	=	makelib();
		lib		*P2	=	makelib();
		lib		*A0	=	makelib();
		lib		*A1	=	makelib();
		lib		*A2	=	makelib();
		do {
/* preverjanje potrebnih podatkov			*/
			l=checklib(Sn,addtype(".r"),1);
			if(!l || !l->xc) {
				err(E_ITEMR01);
				continue;
				}
			copycoord(l,P0);
/*...*/
			l=checklib(Sn,addtype(".r"),2);
			if(!l || !l->xc)
				{
				err(E_ITEMR02);
				continue;
				}
			copycoord(l,P1);
/*...*/
			l=checklib(Sn,addtype(".r"),3);
			if(!l || !l->xc) {
				err(E_ITEMR03);
				continue;
				}
			copycoord(l,P2);
/*...*/
			l=checklib(Sn,addtype(".b"),1);
			if(!l || !l->xc) {
				err(E_ITEMB01);
				continue;
				}
			copycoord(l,A0);
/*...*/
			l=checklib(Sn,addtype(".b"),2);
			if(!l || !l->xc) {
				err(E_ITEMB02);
				continue;
				}
			copycoord(l,A1);
/*...*/
			l=checklib(Sn,addtype(".b"),3);
			if(!l || !l->xc) {
			err(E_ITEMB03);
				continue;
				}
			copycoord(l,A2);

/*		prevedba in premik ref. tock v grid ravnino OBP
		( zaradi hitrosti racunanja )
*/
			LlaToRel(P0,P0,OBP,NULL,N_E_Z);
			LlaToRel(P1,P1,OBP,NULL,N_E_Z);
			LlaToRel(P2,P2,OBP,NULL,N_E_Z);

/*		izracun kotov med P1,P2 in P2,P3
*/
			LlaToRel(A0,A0,OBP,NULL,EL_AZ_R);
			LlaToRel(A1,A1,OBP,NULL,EL_AZ_R);
			LlaToRel(A2,A2,OBP,NULL,EL_AZ_R);

			alfa=A1->xc->y - A0->xc->y;
			beta=A2->xc->y - A1->xc->y;

			copylib(P0,q);
			copylib(P0,c1);
			copylib(P0,c2);

/*		izracun centra prvega kroga v c1:
		R = polovica od P0 do P1
*/
			GridToRel(P1,q,P0,P1,AZ_R_dZ);
			q->xc->x  = maxmils/4-alfa;
			q->xc->y /= 2*sin(alfa/maxmils*2.0*M_PI);
			RelToGrid(q,c1,P0,P1,AZ_R_dZ);

/* izracun centra drugega kroga v c2
*/
			GridToRel(P2,q,P1,P2,AZ_R_dZ);
			q->xc->x  = maxmils/4-beta;
			q->xc->y /= 2*sin(beta/maxmils*2.0*M_PI);
			RelToGrid(q,c2,P1,P2,AZ_R_dZ);

/* preslikava P1, iz c1 na c2 z zamenjavo predznaka y
*/
			freelib(LL);
			copylib(OBP,LL=makelib());

			GridToRel(P1,q,c1,c2,N_E_Z);
			q->xc->y *= -1;
			RelToGrid(q,LL,c1,c2,N_E_Z);

/* zasuk azimuta
*/
			GridToRel(P0,q,LL,NULL,EL_AZ_R);
			x =  fmod(A0->xc->y - q->xc->y + maxmils,maxmils);
			A0->xc->z = q->xc->z;

			GridToRel(P1,q,LL,NULL,EL_AZ_R);
			x += fmod(A1->xc->y - q->xc->y + maxmils,maxmils);
			A1->xc->z = q->xc->z;

			GridToRel(P2,q,LL,NULL,EL_AZ_R);
			x += fmod(A2->xc->y - q->xc->y + maxmils,maxmils);
			A2->xc->z = q->xc->z;

			az_offset = az_off + x/3.0 * 2.0*M_PI/maxmils;
			while(az_offset > M_PI)
				az_offset -= 2.0*M_PI;

			RelToGrid(A0,q,LL,NULL,EL_AZ_R);
			x  = P0->xc->z - q->xc->z;
			RelToGrid(A1,q,LL,NULL,EL_AZ_R);
			x += P1->xc->z - q->xc->z;
			RelToGrid(A2,q,LL,NULL,EL_AZ_R);
			x += P2->xc->z - q->xc->z;

			LL->xc->z += x/3.0;

			RelToLla(LL,LL,OBP,NULL,N_E_Z);
			Displc(LL);
			i=null;
			} while(0);
		freelib(q);
		freelib(c1);
		freelib(c2);
		freelib(P0);
		freelib(P1);
		freelib(P2);
		freelib(A0);
		freelib(A1);
		freelib(A2);
		return(i);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		setup_post(void *vp)
		{
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		setup_north(void *vp)
		{
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UpdatePost(void *vp)
		{
		if(!LL || !LL->ltype) {
			err(E_VOID_TYPE);
			return(null);
			}
		if(!LL || !LL->xc) {
			err(E_VOID_DATA);
			return(null);
			}
		InsertLib(&LL);
		OBP=checklib(Ln,LL->ltype,LL->n);
		DspAddress(LinkAddr(OBP));
		RTCMFixPos();
		Displc(OBP);

		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		ComputeNorth(void *vp)
		{
		az_offset = az_off;
		if(!LL || !LL->ltype) {
			err(E_VOID_TYPE);
			return(null);
			}
		if(!LL || !LL->xc) {
			err(E_VOID_DATA);
			return(null);
			}
		shaft_enable();
		LlaToRel(LL,LL,OBP,NULL,EL_AZ_R);
		az_offset = az_off + (read_AZ() - LL->xc->y)/maxmils*2.0*M_PI;
		while(az_offset > M_PI)
			az_offset -= 2.0*M_PI;
		return(null);
		}
/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
int		UpdateNorth(void *vp)
		{
		az_off=az_offset;
		return(null);
		}
