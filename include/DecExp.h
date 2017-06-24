/*
*	Header File for "DecExp.cpp" 2016 December Experiment at RCNP
*/
#ifndef DecExp_h_
#define DecExp_h_

/***************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<glut.h>

#include"MyGLUT.h"
#include"Object.h"
#include"Colors.h"
#include"define.h"
#include"neutron.h"
#include"PMT.h"

/* flags for control */
bool AFLAG  = false; /* Animation On_Off */
bool MFLAG  = false; /* Mouse Mode */
bool RTFLAG = true;  /* Rotattion_Translation Mode */
bool SFLAG  = true;  /* Status On_Off */

/* a number of neutron,photon */
const int NeutN = 100000;
const int PhotN = 1000;

/* animation maximum time (nsec) */
const double t_max = 300;

/* time constance of neutron occurence */
const double Ntau = t_max/(double)NeutN;

/* absolute time */
double t = 0.0;

/* neutron start time */
double tn0[NeutN+1];

/* neutron energy (neutron.h) */
double En[NeutN]    = {0.0};
double range[NeutN] = {0.0}; 

/* neutron velocity vector */
double vn[NeutN]   = {0.0};
double vn_x[NeutN] = {0.0};
double vn_y[NeutN] = {0.0};
double vn_z[NeutN] = {0.0};

/* neutron position vector */
double Pn_x[NeutN] = {0.0};
double Pn_y[NeutN] = {0.0};
double Pn_z[NeutN] = {0.0};

/* neutron initial position */	
double x_17N = 0.0;
double y_17N = 0.0;
double z_17N =-150.0;

/* detect flag */
int Tcount[NeutN] = {0};

/* neutron montecalro counter */
int nCounter       = 0;
int nLoopCounter   = 1;
int TUNAincident   = 0;
int MAGROincident  = 0;
bool InFlag[NeutN] = {false};

/* histogram for TOF Spector */
const double bin1 = 0.5;
const int ARRAY1  = t_max/bin1;
const double bin2 = 0.1;
const int ARRAY2  = t_max/bin2;
int hist_Li[ARRAY2] = {0};

/* histogram for Neutron Hit Point (0~150ch : 75ch = center) */
const double dPos = 0.5;
const int XARRAY  = (x_pmt+5)*2/dPos;
const int YARRAY  = (y_max+5)*2/dPos;
int TH2D_TUNA_HitPos[XARRAY*YARRAY] = {};
int TH2D_TUNA_TOF_Pos[ARRAY1*XARRAY] = {};

/* histgram for Neutron Energy Spector ([TOF][Ene]) */
const double dEn   = 0.005; /*MeV*/
const double maxEn = 2.50;
const int EnARRAY  = maxEn/dEn;
int TH2D_TUNA_n[ARRAY1*EnARRAY] = {};
int TH2D_MAGRO_n[ARRAY1*EnARRAY]= {};
int TH2D_TUNA_p[ARRAY1*EnARRAY] = {};
int TH2D_MAGRO_p[ARRAY1*EnARRAY]= {};


/* neutron initialize */
void glNeutInit(void){
	nCounter = NeutN;
	/* initialize time */
	t = 0.0;
	/* initialize start time */
	tn0[0] = -Ntau*log(1.0-randf());
	/* neutron direction */
	double Ntheta;
	double Nphi;
	for(int i=0;i<NeutN;++i){
		do{
			En[i] = NeutronEnergy_17N();
		}while(En[i] <= 0.0);
		tn0[i+1] = tn0[i]-Ntau*log(1.0-randf());
		InFlag[i]= false;
		Tcount[i]= 0;
		range[i] = HitPosition(En[i]);
		vn[i]    = lightspeed*sqrt(1.0-pow(Mnc2/(En[i]+Mnc2),2.0));
		Ntheta   = 2.0*acos(sqrt(1.0-randf()));
		Nphi     = randf()*2.0*PI;
		vn_x[i]  = vn[i]*cos(Ntheta);
		vn_y[i]  = vn[i]*sin(Ntheta)*cos(Nphi);
		vn_z[i]  = vn[i]*sin(Ntheta)*sin(Nphi);
		Pn_x[i]  = x_17N;
		Pn_y[i]  = y_17N;
		Pn_z[i]  = z_17N;
	}
}


/* initialize each neutron */
void glEachNeutInit(int i, double time){
	++nCounter;
	/* initialize start time */
	tn0[i] = time-Ntau*log(1.0-randf());
	/* neutron direction */
	double Ntheta;
	double Nphi;
	do{
		En[i] = NeutronEnergy_17N();
	}while(En[i] <= 0.0);
	InFlag[i]= false;
	Tcount[i]= 0;
	range[i] = HitPosition(En[i]);
	vn[i]    = lightspeed*sqrt(1.0-pow(Mnc2/(En[i]+Mnc2),2.0));
	Ntheta   = 2.0*acos(sqrt(1.0-randf()));
	Nphi     = randf()*2.0*PI;
	vn_x[i]  = vn[i]*cos(Ntheta);
	vn_y[i]  = vn[i]*sin(Ntheta)*cos(Nphi);
	vn_z[i]  = vn[i]*sin(Ntheta)*sin(Nphi);
	Pn_x[i]  = x_17N;
	Pn_y[i]  = y_17N;
	Pn_z[i]  = z_17N;
}


#define FillTUNA() {\
		int ch1 = (t-tn0[i])/bin1;\
		if(0<=ch1&&ch1<ARRAY1){\
			int ch2 = En[i]/dEn;     /* Neutron */\
			if(0<=ch2&&ch2<EnARRAY) ++TH2D_TUNA_n[ch1*EnARRAY+ch2];\
			ch2 = En[i]*randf()/dEn; /* Proton */\
			if(0<=ch2&&ch2<EnARRAY) ++TH2D_TUNA_p[ch1*EnARRAY+ch2];\
			int chx = (Pn_x[i]+x_pmt+5)/dPos;\
			int chy = (Pn_y[i]+y_max+5)/dPos;\
			if(0<=chx&&chx<=XARRAY && 0<=chy&&chy<=YARRAY){\
				++TH2D_TUNA_HitPos[chx*YARRAY+chy];\
				++TH2D_TUNA_TOF_Pos[ch1*XARRAY+chx];\
			}\
		}\
}
/* neutron montecarlo animation and fill histogram */
void glNeutMonte(void){
	/* initialize start time */
	double Tinit;
	if(AMODE==0) Tinit = t_max;
	if(AMODE==1) Tinit = 30.0;

	glDisable(GL_LIGHTING);
	glColor3d(0.0,0.6,0.0);
	glPointSize(0.1);
	glBegin(GL_POINTS);
	for(int i=0;i<NeutN;++i){

		if(t-tn0[i]>Tinit) glEachNeutInit(i,t);

		if(tn0[i]<t){

			glVertex3d(Pn_x[i],Pn_y[i],Pn_z[i]);

			if(AFLAG && Tcount[i]==0){

				/* neutron move */
				if(AMODE==0){
					Pn_x[i] += vn_x[i]*bin1;
					Pn_y[i] += vn_y[i]*bin1;
					Pn_z[i] += vn_z[i]*bin1;
				}
				if(AMODE==1){
					Pn_x[i] += vn_x[i]*bin2;
					Pn_y[i] += vn_y[i]*bin2;
					Pn_z[i] += vn_z[i]*bin2;
				}

				/* into TUNA */	
				if(-x_max<=Pn_x[i]&&Pn_x[i]<=x_max){
					if(-y_max<=Pn_y[i]&&Pn_y[i]<=y_max){
						if(-z_max<=Pn_z[i]&&Pn_z[i]<=z_max){
							if(InFlag[i]!=true){
								++TUNAincident;
								InFlag[i]=true;
							}
							if(range[i]<vn[i]*z_max*2/vn_z[i]){
							//	double Tover = range[i]/vn[i]-(Pn_z[i]+z_max)/vn_z[i];
							//	Pn_x[i] += vn_x[i]*Tover;
							//	Pn_y[i] += vn_y[i]*Tover;
							//	Pn_z[i] += vn_z[i]*Tover;
							//	int ch1 = (t+Tover-tn0[i])/bin1;
								FillTUNA();
								++Tcount[i];
							}

						}
					}
				}
				if(Tcount[i]==0){
					if(-x_lgd<=Pn_x[i] && Pn_x[i]<=-x_max){
						double guide_y = (x_lgd+Pn_x[i])*tan(psi)+y_lgd;
						double guide_z = (x_lgd+Pn_x[i])*tan(xi)+z_lgd;
						if(-guide_y<=Pn_y[i]&&Pn_y[i]<=guide_y\
							&& -guide_z<=Pn_z[i]&&Pn_z[i]<=guide_z){

							if(InFlag[i]!=true){
								++TUNAincident;
								InFlag[i]=true;
							}
							if(range[i]<vn[i]*z_max*2/vn_z[i]){
								FillTUNA();
								++Tcount[i];
							}

						}
					}
					if( x_max<=Pn_x[i] && Pn_x[i]<= x_lgd){
						double guide_y = (x_lgd-Pn_x[i])*tan(psi)+y_lgd;
						double guide_z = (x_lgd-Pn_x[i])*tan(xi)+z_lgd;
						if(-guide_y<=Pn_y[i]&&Pn_y[i]<=guide_y\
							&& -guide_z<=Pn_z[i]&&Pn_z[i]<=guide_z){

							if(InFlag[i]!=true){
								++TUNAincident;
								InFlag[i]=true;
							}
							if(range[i]<vn[i]*z_max*2/vn_z[i]){
								FillTUNA();
								++Tcount[i];
							}

						}
					}
				}

				/* into MAGRO */
				double dMAGRO = 2.0;
				double rad = 26*PI/180;
				double R0  = 150.0;
				double R   = sqrt(pow(R0*cos(rad)-(z_17N+150),2)+pow(R0*sin(rad),2));
				double x0  = Pn_x[i];
				double y0  = cos(rad)*Pn_y[i]+sin(rad)*Pn_z[i];
				double z0  =-sin(rad)*Pn_y[i]+cos(rad)*Pn_z[i];
				double Rn  = sqrt(pow(x0-x_17N,2)+pow(z0-z_17N,2));
				if(R/cos(rad)-dMAGRO<Rn&&Rn<R/cos(rad)){
					double L = R0*sin(30.6*PI/180);
					double y =-R0*sin(rad);
					double dy;
					if(fabs(x0)<L-20) dy = 20-fabs(x0)*5.0/(L-20.0);
					else dy = 15-(fabs(x0)-L+20)*12.5/20.0;
					if(y-dy<y0&&y0<y+dy){
						if(z_17N<Pn_z[i]){

							if(InFlag[i]!=true){
								++MAGROincident;
								InFlag[i]=true;
							}
							if(range[i]<dMAGRO){
							//	double Tover = (range[i])/vn[i];
							//	Pn_x[i] += vn_x[i]*Tover;
							//	Pn_y[i] += vn_y[i]*Tover;
							//	Pn_z[i] += vn_z[i]*Tover;
							//	int ch1 = (t+Tover-tn0[i])/bin1;
								int ch1 = (t-tn0[i])/bin1;
								int ch2 = En[i]/dEn;     /* Neutron */
								if(0<=ch2&&ch2<EnARRAY) ++TH2D_MAGRO_n[ch1*EnARRAY+ch2];
								ch2 = En[i]*randf()/dEn; /* Proton */
								if(0<=ch2&&ch2<EnARRAY) ++TH2D_MAGRO_p[ch1*EnARRAY+ch2];
								++Tcount[i];
							}
						}
					}
				}

				/* into LiGlass */
				if(Pn_y[i]<=y_Li){
					double x,z;
					x = Pn_x[i] - x_Li;
					z = Pn_z[i] - z_Li;
					if(x*x+z*z<=R_Li*R_Li){
						int ch = (t-tn0[i])/bin2;
						if(0<=ch&&ch<ARRAY2) ++hist_Li[ch];
						++Tcount[i];
					}
				}
			}
		}/* end of (AFLAG) */
	}
	glEnd();
	if(AFLAG){
		/* advance time */
		if(AMODE==0) t += bin1;
		if(AMODE==1) t += bin2;
	}
	glEnable(GL_LIGHTING);
}
#undef FillTUNA





/********* Scintillation Photon *********/

/* photon animation speed */
double Pspeed = 20.0;

/* photon starting time */
double t0[PhotN] = {0.0};

/* photon velocity vector */
double vx[PhotN] = {0.0};
double vy[PhotN] = {0.0};
double vz[PhotN] = {0.0};

/* photon position vector */
double Px[PhotN] = {0.0};
double Py[PhotN] = {0.0};
double Pz[PhotN] = {0.0};

/* tempolary position vector for flight length */
double Tpx[PhotN] = {0.0};
double Tpy[PhotN] = {0.0};
double Tpz[PhotN] = {0.0};

/* photon flight length */
double Length[PhotN] = {0.0};

/* reach PMT flag */
bool  EndFlag[PhotN] = {false};

/* montecarlo loop counter */
int LoopCounter    = 1;
int PreLoopCounter = 1;

/* histogram for TOF Spector */
const double bin3     =  0.1;
const int ARRAY3      = 25.0/bin3;
int hist_rPMT[ARRAY3] = {0};
int hist_lPMT[ARRAY3] = {0};
int PreHistR[ARRAY3]  = {0};
int PreHistL[ARRAY3]  = {0};

/* previous position */
double ppx = 0.0;
double ppy = 0.0;


/* histogram initialize */
void glHistInit(void){
	if(AMODE==0){
		TUNAincident  = 0;
		MAGROincident = 0;
		memset(TH2D_TUNA_HitPos,0,sizeof(TH2D_TUNA_HitPos));
		memset(TH2D_TUNA_n,0,sizeof(TH2D_TUNA_n));
		memset(TH2D_MAGRO_n,0,sizeof(TH2D_MAGRO_n));
		memset(TH2D_TUNA_p,0,sizeof(TH2D_TUNA_p));
		memset(TH2D_MAGRO_p,0,sizeof(TH2D_MAGRO_p));
		memset(TH2D_TUNA_TOF_Pos,0,sizeof(TH2D_TUNA_TOF_Pos));
	}
	if(AMODE==1){
		memset(hist_Li,0,sizeof(hist_Li));
	}
	if(AMODE==2){
		LoopCounter   = 1;
		memset(hist_rPMT,0,sizeof(hist_rPMT));
		memset(hist_lPMT,0,sizeof(hist_lPMT));
		PMThistgramInit();
	}
}


/* initialize photon animation */
void glPhotInit(void){
	/* initialize PMT output */
	PMToutputInit();
	/* initialize time */
	t = 0.0;
	/* neutron direction */
	double theta;
	double phi;
	for(int i=0;i<PhotN;++i){
		/* for PMT initialize */
		PMTtimeR[i]   = 0.0;
		PMTtimeL[i]   = 0.0;
		heightR[i]    = 0.0;
		heightL[i]    = 0.0;
		reachtimeR[i] = 0.0;
		reachtimeL[i] = 0.0;
		/* for Montecarlo */
		t0[i]  = -tau*(log(1.0-randf())); 
		theta  = 2.0*acos(sqrt(1.0-randf()));
		phi    = randf()*2.0*PI;
		vx[i]  = lightspeed*cos(theta);
		vy[i]  = lightspeed*sin(theta)*cos(phi);
		vz[i]  = lightspeed*sin(theta)*sin(phi);
		Px[i]  = px;
		Py[i]  = py;
		Pz[i]  = 2*z_max*(randf()-0.5);
		Tpx[i] = Px[i];
		Tpy[i] = Py[i];
		Tpz[i] = Pz[i];
		Length[i]  = 0.0;
		EndFlag[i] = false;
	}
}


/* reflection function for photons */
void Reflection(double Tover, double *Px, double *Py, double *Pz, double *vx, double *vy, double *vz, double e_n[3]){
	(*Px)-= (*vx)*Tover;
	(*Py)-= (*vy)*Tover;
	(*Pz)-= (*vz)*Tover;
	double dot = (*vx)*e_n[0]+(*vy)*e_n[1]+(*vz)*e_n[2];
	(*vx) = (*vx)-2.0*dot*e_n[0];
	(*vy) = (*vy)-2.0*dot*e_n[1];
	(*vz) = (*vz)-2.0*dot*e_n[2];
	(*Px)+= (*vx)*Tover;
	(*Py)+= (*vy)*Tover;
	(*Pz)+= (*vz)*Tover;
}


double Flength(double x1, double y1, double z1, double *x0, double *y0, double *z0){
	double x = x1-(*x0);
	double y = y1-(*y0);
	double z = z1-(*z0);
	(*x0) = x1;
	(*y0) = y1;
	(*z0) = z1;
	return sqrt(x*x+y*y+z*z);
}


/* photon montecarlo animation in TUNA */
void glPhotMonte(void){
	if(t>=ARRAY3*bin3){
		glPhotInit();
		++LoopCounter;
	}
	glDisable(GL_LIGHTING);

	/* Montecarlo */
	glColor3d(0.0,0.0,0.8);
	glPointSize(1.2);
	glBegin(GL_POINTS);
	for(int i=0;i<PhotN;++i){
		if(EndFlag[i]) continue;
		else glVertex3d(Px[i],Py[i],Pz[i]);
		if(AFLAG && t0[i]<=t){
			/* neutron move */
			if(t<t_max){
				Px[i] += vx[i]*dt*Pspeed;
				Py[i] += vy[i]*dt*Pspeed;
				Pz[i] += vz[i]*dt*Pspeed;
			}

			for(int j=0;j<10;++j){
				if(-x_max<Px[i] && Px[i]< x_max){
					if(-y_max<Py[i] && Py[i]< y_max){
						if(-z_max<Pz[i] && Pz[i]< z_max){
							break;
						}
					}
				}
				if(-x_lgd<Px[i] && Px[i]<-x_max){
					double guide_y = (x_lgd+Px[i])*tan(psi)+y_lgd;
					double guide_z = (x_lgd+Px[i])*tan(xi)+z_lgd;
					if(-guide_y<Py[i] && Py[i]<guide_y){
						if(-guide_z<Pz[i] && Pz[i]<guide_z){
							break;
						}
					}
				}
				if( x_max<Px[i] && Px[i]<x_lgd){
					double guide_y = (x_lgd-Px[i])*tan(psi)+y_lgd;
					double guide_z = (x_lgd-Px[i])*tan(xi)+z_lgd;
					if(-guide_y<Py[i] && Py[i]<guide_y){
						if(-guide_z<Pz[i] && Pz[i]<guide_z){
							break;
						}
					}
				}
				if((-x_pmt<Px[i] && Px[i]<-x_lgd) || (x_lgd<Px[i] && Px[i]< x_pmt)){
					if(-y_pmt<Py[i] && Py[i]< y_pmt){
						if(-z_pmt<Pz[i] && Pz[i]< z_pmt){
							break;
						}
					}
				}

				/* judgement */
				if(-x_max <= Px[i] && Px[i] <= x_max){
					if( y_max <= Py[i]){
						double Tover = (Py[i]-y_max)/vy[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] = 1;
						e_n[2] = 0;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-y_max >= Py[i]){
						double Tover = (Py[i]+y_max)/vy[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] =-1;
						e_n[2] = 0;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if( z_max <= Pz[i]){
						double Tover = (Pz[i]-z_max)/vz[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] = 0;
						e_n[2] = 1;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-z_max >= Pz[i]){
						double Tover = (Pz[i]+z_max)/vz[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] = 0;
						e_n[2] =-1;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
				}
				if(-x_lgd<=Px[i] && Px[i]<-x_max){
					double guide_y = (x_lgd+Px[i])*tan(psi)+y_lgd;
					double guide_z = (x_lgd+Px[i])*tan(xi)+z_lgd;
					if( guide_y <= Py[i]){
						double e_n[3];
						e_n[0] =-sin(psi);
						e_n[1] = cos(psi);
						e_n[2] = 0;
						double D = fabs(e_n[0]*(Px[i]+x_max)+e_n[1]*(Py[i]-y_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-guide_y >= Py[i]){
						double e_n[3];
						e_n[0] =-sin(psi);
						e_n[1] =-cos(psi);
						e_n[2] = 0;
						double D = fabs(e_n[0]*(Px[i]+x_max)+e_n[1]*(Py[i]+y_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if( guide_z <= Pz[i]){
						double e_n[3];
						e_n[0] =-sin(xi);
						e_n[1] = 0;
						e_n[2] = cos(xi);
						double D = fabs(e_n[0]*(Px[i]+x_max)+e_n[2]*(Pz[i]-z_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-guide_z >= Pz[i]){
						double e_n[3];
						e_n[0] =-sin(xi);
						e_n[1] = 0;
						e_n[2] =-cos(xi);
						double D = fabs(e_n[0]*(Px[i]+x_max)+e_n[2]*(Pz[i]+z_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
				}
				if( x_max<Px[i] && Px[i]<= x_lgd){
					double guide_y = (x_lgd-Px[i])*tan(psi)+y_lgd;
					double guide_z = (x_lgd-Px[i])*tan(xi)+z_lgd;
					if( guide_y <= Py[i]){
						double e_n[3];
						e_n[0] = sin(psi);
						e_n[1] = cos(psi);
						e_n[2] = 0;
						double D = fabs(e_n[0]*(Px[i]-x_max)+e_n[1]*(Py[i]-y_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-guide_y >= Py[i]){
						double e_n[3];
						e_n[0] = sin(psi);
						e_n[1] =-cos(psi);
						e_n[2] = 0;
						double D = fabs(e_n[0]*(Px[i]-x_max)+e_n[1]*(Py[i]+y_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if( guide_z <= Pz[i]){
						double e_n[3];
						e_n[0] = sin(xi);
						e_n[1] = 0;
						e_n[2] = cos(xi);
						double D = fabs(e_n[0]*(Px[i]-x_max)+e_n[2]*(Pz[i]-z_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-guide_z >= Pz[i]){
						double e_n[3];
						e_n[0] = sin(xi);
						e_n[1] = 0;
						e_n[2] =-cos(xi);
						double D = fabs(e_n[0]*(Px[i]-x_max)+e_n[2]*(Pz[i]+z_max));
						double Tover = D/fabs(vx[i]*e_n[0]+vy[i]*e_n[1]+vz[i]*e_n[2]);
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
				}
				if((-x_pmt<=Px[i]&&Px[i]<-x_lgd) || (x_lgd<Px[i]&&Px[i]<= x_pmt)){
					if( y_pmt <= Py[i]){
						double Tover = (Py[i]-y_pmt)/vy[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] = 1;
						e_n[2] = 0;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-y_pmt >= Py[i]){
						double Tover = (Py[i]+y_pmt)/vy[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] =-1;
						e_n[2] = 0;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if( z_pmt <= Pz[i]){
						double Tover = (Pz[i]-z_pmt)/vz[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] = 0;
						e_n[2] = 1;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
					if(-z_pmt >= Pz[i]){
						double Tover = (Pz[i]+z_pmt)/vz[i];
						double e_n[3];
						e_n[0] = 0;
						e_n[1] = 0;
						e_n[2] =-1;
						Reflection(Tover,&Px[i],&Py[i],&Pz[i],&vx[i],&vy[i],&vz[i],e_n);
						Length[i]+= Flength(Px[i],Py[i],Pz[i],&Tpx[i],&Tpy[i],&Tpz[i]);
						if(randf()>p_reflect) EndFlag[i] = true;
					}
				}
			}
			/* Reach Right PMT */
			if(-x_pmt>Px[i]){
				EndFlag[i] = true;
				double P_attenuation = exp(-Length[i]/L_a);
				if(randf()<P_attenuation){
					reachtimeR[i] = t-fabs((Px[i]+x_pmt)/vx[i]);
					if(reachtimeR[i]<0){
						reachtimeR[i] = 0.0;
						continue;
					}
					int ch = reachtimeR[i]/bin3;
					if(0<=ch && ch<ARRAY3) ++hist_rPMT[ch];
				}
			}
			/* Reach  Left PMT */
			if( x_pmt<Px[i]){
				EndFlag[i] = true;
				double P_attenuation = exp(-Length[i]/L_a);
				if(randf()<P_attenuation){
					reachtimeL[i] = t-fabs((Px[i]-x_pmt)/vx[i]);
					if(reachtimeL[i]<0){
						reachtimeL[i] = 0.0;
						continue;
					}
					int ch = reachtimeL[i]/bin3;
					if(0<=ch && ch<ARRAY3) ++hist_lPMT[ch];
				}
			}
		}/* end of (AFLAG) */
	}
	glEnd();
	if(AFLAG){
		/* advance time */
		if(t<t_max) t += dt*Pspeed;
	}
	glEnable(GL_LIGHTING);
}





/* Size of Animation Window */
int AwindowW;
int AwindowH;

/* Camera Target Point */
void glCameraTarget(void){
	if(SFLAG){
		glDisable(GL_LIGHTING);
		double Lx = 10/(double)AwindowW;
		double Ly = 10/(double)AwindowH;
		glColor3d(0.0,0.0,0.0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1, 1,-1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glBegin(GL_LINES);
		glVertex2d(-Lx,0);
		glVertex2d( Lx,0);
		glVertex2d(0,-Ly);
		glVertex2d(0, Ly);
		glEnd();

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHTING);
	}
}


/* Draw String Information in Animation Display */
void glDisplayStrings(void){
	if(SFLAG){
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, AwindowW, AwindowH, 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		/* Canvas */
		glColor3d(1.0,1.0,1.0);
		bool FLAG = false;
		if(PFLAG==false){
			PFLAG = true;
			FLAG  = true;
		}
		glRectangular(280,120,1,140,55,0);
		if(FLAG) PFLAG = false;

		/* String */
		glColor3d(0.0,0.0,0.0);
		char anim[50];
		if(AMODE==0) sprintf(anim,"Animation : Neutron (TUNA)");
		if(AMODE==1) sprintf(anim,"Animation : Neutron (Li Glass)");
		if(AMODE==2) sprintf(anim,"Animation : Scintillation Photon");
		glDrawString(anim,10,15);

		char disp[50];
		if(PFLAG) sprintf(disp,"Display   : Polygon");
		else      sprintf(disp,"Display   : Line");
		glDrawString(disp,10,30);

		char rtmd[50];
		if(RTFLAG) sprintf(rtmd,"Mouse     : Rotate");
		else       sprintf(rtmd,"Mouse     : Translate");
		glDrawString(rtmd,10,45);

		char trns[50];
		if(AMODE==0) sprintf(trns,"Translate : %+6.1f %+6.1f",xc,zc);
		if(AMODE==1) sprintf(trns,"Translate : %+6.1f %+6.1f",xc,zc);
		if(AMODE==2) sprintf(trns,"Translate : %+6.1f %+6.1f",px,py);
		glDrawString(trns,10,60);

		char dist[50];
		sprintf(dist,"Distance  : %.1f cm",dstnc);
		glDrawString(dist,10,75);

		char angl[50];
		sprintf(angl,"Angle     : %+4.0f deg,%+3.0f deg",theta*180/PI,phi*180/PI);
		glDrawString(angl,10,90); 

		char time[50];
		if(AMODE==0) sprintf(time,"Abs Time  : %.2f micro sec",t/1000);
		if(AMODE==1) sprintf(time,"Abs Time  : %.2f micro sec",t/1000);
		if(AMODE==2) sprintf(time,"Time      : %.2f nsec",t);
		glDrawString(time,10,105);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHTING);
	}
}



/* Draw Button into Animation Display */
void glDisplayButtons(void){
	if(SFLAG){
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, AwindowW, AwindowH, 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		/* Button(change speed) */
		if(AMODE==2){
			glPushMatrix();
			glTranslated(AwindowW/2.0-50.0,AwindowH/1.05,0);
			glColor3d(0.2,0.2,0.2);
			char s[50];
			double S = Pspeed/20;
			sprintf(s,"x%.0f",S);
			if(S<10)glDrawString2(s,5,12);
			else    glDrawString2(s,4,12);
			glBegin(GL_TRIANGLES);
			glVertex2d(-12, 8);
			glVertex2d(-12,-8);
			glVertex2d(  0, 0);
			glVertex2d(  0, 8);
			glVertex2d(  0,-8);
			glVertex2d( 12, 0);
			glEnd();
			glLineWidth(2.0);
			glColor3d(0.0,0.0,0.0);
			glBegin(GL_LINE_LOOP);
			glVertex2d(-20,-15);
			glVertex2d( 20,-15);
			glVertex2d( 20, 15);
			glVertex2d(-20, 15);
			glEnd();
			glLineWidth(0.1);
			glColor3d(0.8,0.8,0.8);
			glBegin(GL_QUADS);
			glVertex2d(-20,-15);
			glVertex2d( 20,-15);
			glVertex2d( 20, 15);
			glVertex2d(-20, 15);
			glEnd();
			glPopMatrix();
		}

		/* Button(Play/Stop) */
		glPushMatrix();
		glTranslated(AwindowW/2.0,AwindowH/1.05,0);
		glColor3d(0.2,0.2,0.2);
		if(AFLAG){
			glBegin(GL_QUADS);
			glVertex2d(-8.5,-8.0);
			glVertex2d(-2.5,-8.0);
			glVertex2d(-2.5, 8.0);
			glVertex2d(-8.5, 8.0);
			glVertex2d( 2.5,-8.0);
			glVertex2d( 8.5,-8.0);
			glVertex2d( 8.5, 8.0);
			glVertex2d( 2.5, 8.0);
			glEnd();
		}
		else{
			glBegin(GL_TRIANGLES);
			glVertex2d(-10, 7);
			glVertex2d(-10,-7);
			glVertex2d( 10, 0);
			glEnd();
		}
		glLineWidth(2.0);
		glColor3d(0.0,0.0,0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2d(-20,-15);
		glVertex2d( 20,-15);
		glVertex2d( 20, 15);
		glVertex2d(-20, 15);
		glEnd();
		glLineWidth(0.1);
		glColor3d(0.8,0.8,0.8);
		glBegin(GL_QUADS);
		glVertex2d(-20,-15);
		glVertex2d( 20,-15);
		glVertex2d( 20, 15);
		glVertex2d(-20, 15);
		glEnd();
		glPopMatrix();

		/* Button(Reset) */
		glPushMatrix();
		glTranslated(AwindowW/2.0+50.0,AwindowH/1.05,0);
		glLineWidth(2.0);
		glColor3d(0.0,0.0,0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2d(-20,-15);
		glVertex2d( 20,-15);
		glVertex2d( 20, 15);
		glVertex2d(-20, 15);
		glEnd();
		glLineWidth(0.1);
		glBegin(GL_QUADS);
		glColor3d(0.2,0.2,0.2);
		glVertex2d(-7.5,-7.0);
		glVertex2d( 7.5,-7.0);
		glVertex2d( 7.5, 7.0);
		glVertex2d(-7.5, 7.0);
		glColor3d(0.8,0.8,0.8);
		glVertex2d(-20,-15);
		glVertex2d( 20,-15);
		glVertex2d( 20, 15);
		glVertex2d(-20, 15);
		glEnd();
		glPopMatrix();

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHTING);
	}
}



/***************************/

#endif //"DecExp_h_"
