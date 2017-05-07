#ifndef DecExp_h_
#define DecExp_h_

/***************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>

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
bool HFLAG  = false; /* Stock Histgram */
bool OFLAG  = false; /* Oscillo Mode */
bool LFLAG  = true;  /* Log Scale for PMT Histgram */
int  OMODE  = 0; /* Oscillo Mode (0:reach 1:signal 2:hist) */


/* a number of neutron,photon */
const int NeutN = 100000;
const int PhotN =   1000;

/* animation maximum time */
const double t_max = 300;

/* animation speed */
double speed = 1000;

/* define time */
double t = 0.0;

/* neutron energy (neutron.h) */
double En[NeutN]    = {0.0};
double range[NeutN] = {0.0}; 

/* neutron velocity vector */
double vn[NeutN]   = {0.0};
double vn_x[NeutN] = {0.0};
double vn_y[NeutN] = {0.0};
double vn_z[NeutN] = {0.0};

/* neutron position vector */
double Pn_x[NeutN];
double Pn_y[NeutN];
double Pn_z[NeutN];

/* neutron initial position */	
double x_17N = 0.0;
double y_17N = 0.0;
double z_17N =-150.0;

/* detect flag */
int Tcount[NeutN] = {0};

/* neutron montecalro counter */
int nLoopCounter   = 1;
int TUNAincident   = 0;
int MAGROincident  = 0;
bool InFlag[NeutN] = {false};

/* histogram for TOF Spector */
const double bin1     = 0.5;
const int ARRAY1      = t_max/bin1;
int hist_TUNA[ARRAY1] = {0};
int hist_MAGRO[ARRAY1]= {0};
const double bin2     = 0.1;
const int ARRAY2      = t_max/bin2;
int hist_Li[ARRAY2]   = {0};


/* neutron initialize */
void glNeutInit(void){
	/* initialize time */
	t = 0.0;
	/* neutron direction */
	double theta;
	double phi;
	for(int i=0;i<NeutN;++i){
		do{
			En[i] = NeutronEnergy_17N();
		}while(En[i] <= 0.0);
		InFlag[i]= false;
		Tcount[i]= 0;
		range[i] = HitPosition(En[i]);
		vn[i]    = c*sqrt(1.0-pow(Mnc2/(En[i]+Mnc2),2.0));
		theta    = 2.0*acos(sqrt(1.0-randf()));
		phi      = randf()*2.0*PI;
		vn_x[i]  = vn[i]*cos(theta);
		vn_y[i]  = vn[i]*sin(theta)*cos(phi);
		vn_z[i]  = vn[i]*sin(theta)*sin(phi);
		Pn_x[i]  = x_17N;
		Pn_y[i]  = y_17N;
		Pn_z[i]  = z_17N;
	}
}


/* neutron montecarlo animation and fill histogram */
void glNeutMonte(void){

	if(AMODE==0){
		if(t>=t_max){
			glNeutInit();
			++nLoopCounter;
		}
	}
	if(AMODE==1){
		if(t>=30.0){
			glNeutInit();
		}
	}

	glDisable(GL_LIGHTING);
	glColor3d(0.0,0.6,0.0);
	glPointSize(0.1);
	glBegin(GL_POINTS);
	for(int i=0;i<NeutN;++i){
		glVertex3d(Pn_x[i],Pn_y[i],Pn_z[i]);

		if(AFLAG){
			
			/* into TUNA */	
			if(-x_max<=Pn_x[i]&&Pn_x[i]<=x_max){
				if(-y_max<=Pn_y[i]&&Pn_y[i]<=y_max){
					if(-z_max<=Pn_z[i]&&Pn_z[i]<=z_max){

						if(Tcount[i]==0){
							if(InFlag[i]!=true){
								++TUNAincident;
								InFlag[i]=true;
							}
							if(range[i]<vn[i]*z_max*2/vn_z[i]){
								if(t<t_max){
									int ch = t/bin1;
									++hist_TUNA[ch];
									++Tcount[i];
								}
							}

						}
					}
				}
			}

			/* into MAGRO */
			double dMAGRO = 2.0;
			double rad = 26*PI/180;
			double R0  = 148.0;
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

						if(Tcount[i]==0){
							if(InFlag[i]!=true){
								++MAGROincident;
								InFlag[i]=true;
							}
							if(range[i]<dMAGRO){
								if(t<t_max){
									int ch = t/bin1;
									++hist_MAGRO[ch];
									++Tcount[i];
								}
							}
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
					if(t<t_max){
						if(Tcount[i]==0){
							int ch = t/bin2;
							++hist_Li[ch];
						}
						++Tcount[i];
					}

				}
			}
			/* neutron move */
			if(Tcount[i] == 0){
				//if(t<t_max){
				//	Pn_x[i] += vn_x[i]*dt*speed;	
				//	Pn_y[i] += vn_y[i]*dt*speed;	
				//	Pn_z[i] += vn_z[i]*dt*speed;
				//}
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
			}
		}/* end of (AFLAG) */
	}
	glEnd();
	if(AFLAG){
		/* advance time */
		//if(t<t_max) t += dt*speed;
		if(AMODE==0) t += bin1;
		if(AMODE==1) t += bin2;
	}
	glEnable(GL_LIGHTING);
}





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
	TUNAincident  = 0;
	MAGROincident = 0;
	for(int ch=0;ch<ARRAY1;++ch){
		hist_TUNA[ch] = 0;
		hist_MAGRO[ch]= 0;
	}
	for(int ch=0;ch<ARRAY2;++ch) hist_Li[ch] = 0;
	for(int ch=0;ch<ARRAY3;++ch){
		LoopCounter   = 1;
		hist_rPMT[ch] = 0;
		hist_lPMT[ch] = 0;
	}
	PMThistgramInit();
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
		vx[i]  = c*cos(theta);
		vy[i]  = c*sin(theta)*cos(phi);
		vz[i]  = c*sin(theta)*sin(phi);
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


//void ReflectionOld(double *vx, double *vy, double *vz, double e_n[3]){
//	double dot = (*vx)*e_n[0]+(*vy)*e_n[1]+(*vz)*e_n[2];
//	(*vx) = (*vx)-2.0*dot*e_n[0];
//	(*vy) = (*vy)-2.0*dot*e_n[1];
//	(*vz) = (*vz)-2.0*dot*e_n[2];
//}


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


/* Draw String into Animation Display */
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
		if(AMODE==0) sprintf(time,"Time      : %.1f nsec",t);
		if(AMODE==1) sprintf(time,"Time      : %.1f nsec",t);
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













/* Draw Histgram */
void glGraph(void){

	/* canvas color setting */
	if(AMODE==2 && OMODE==1) glClearColor(0.1,0.1,0.1,1.0);
	else glClearColor(1.0,1.0,1.0,1.0);

	/* Window Margin (%) */
	double MARGIN = 8.0;

	/* draw current time */
	if(SFLAG && OMODE!=2){
		if(AMODE==2 && OMODE==1) glColor3d(1.0,1.0,1.0);
		else glColor3d(0.0,0.0,0.0);
		char time[50];
		sprintf(time,"time %6.2f",t);
		glDrawString(time,0.705,0.900);
	}


	/* TUNA(&MAGRO) TOF Spector */
	if(AMODE==0){
		int MAXhist    = 0;
		int TUNAtotal  = 0;
		int MAGROtotal = 0;
		for(int ch=0;ch<ARRAY1;++ch){
			if(MAXhist<hist_TUNA[ch]) MAXhist = hist_TUNA[ch];
			if(SFLAG && MAXhist<hist_MAGRO[ch]) MAXhist = hist_MAGRO[ch];
			TUNAtotal  += hist_TUNA[ch];
			MAGROtotal += hist_MAGRO[ch];
		}
		/* draw efficiency */
		if(SFLAG){
			/* TUNA */
			glColor3d(0.0,0.0,0.0);
			char txt[50];
			double Eint;
			double Eamb;
			Eint = (TUNAincident==0)?  0:(double)TUNAtotal/TUNAincident*100;
			Eamb = (double)TUNAtotal/(nLoopCounter*NeutN)*100;
			sprintf(txt,"Eamb%5.2f%%, Eint%5.1f%%",Eamb,Eint);
			glDrawString(txt,0.50,0.85);
			/* MAGRO */
			glColor3d(0.9,0.0,0.0);
			Eint = (MAGROincident==0)? 0:(double)MAGROtotal/MAGROincident*100;
			Eamb = (double)MAGROtotal/(nLoopCounter*NeutN)*100;
			sprintf(txt,"Eamb%5.2f%%, Eint%5.1f%%",Eamb,Eint);
			glDrawString(txt,0.50,0.80);
		}
		/* setting drawing scale */
		int base  = 10;
		int digit = 0;
		int temp  = 1;
		while(temp*5*pow(base,digit)<MAXhist){
			++temp;
			if(temp==9){
				temp = 1;
				++digit;
			}
		}
		double scale = (double)temp*pow(base,digit);

		/* draw range */
		double XMIN  = 0.0;
		double XMAX  = 250.0*1.01;
		double XGRID = (XMAX-XMIN)/5/1.01;
		int Nx = (XMAX-XMIN)/XGRID;

		double YMIN  = 0.0;
		double YMAX  = scale*5;
		double YGRID = scale;
		int Ny = (YMAX-YMIN)/YGRID;

		/* window size */
		double WX    = (XMAX-XMIN)*(100+MARGIN*2)/100;
		double WY    = (YMAX-YMIN)*(100+MARGIN*2)/100;

		/* offset */
		double Xoff  = WX*(MARGIN+2)/100;
		double Yoff  = WY*MARGIN/100;

		/* grid line */
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1.0, 0x5555);
		glColor3d(0.5,0.5,0.5);
		glBegin(GL_LINES);
		for(int i=1; i<=Nx; ++i){
			glVertex2d((XGRID*i+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XGRID*i+Xoff)/WX,(YMAX+Yoff)/WY);
		}
		for(int i=1; i<=Ny; ++i){
			glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		/* text */
		double XtxtA1 = -(XMAX-XMIN)*0.01;
		double XtxtA2 = -(XMAX-XMIN)*0.02;
		double XtxtA3 = -(XMAX-XMIN)*0.03;
		double YtxtA  = -(YMAX-YMIN)*0.04;
		double XtxtB  = -(XMAX-XMIN)*0.10;
		double YtxtB  = -(YMAX-YMIN)*0.01;
		glColor3d(0.0,0.0,0.0);
		char title[100];
		double x1;
		if(bin1==1.0){
			sprintf(title,"TOF Spector [counts/nsec]");
			x1 = 0.30;
		}
		else{
			sprintf(title,"TOF Spector [counts/%3.1fnsec]",bin1);
			x1 = 0.20;
		}
		glDrawString(title,x1,0.95);
		glDrawString("0",(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
		char s[10];
		for(int i=1; i<=Nx; ++i){
			sprintf(s,"%d",i*(int)XGRID);
			int size = strlen(s);
			if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
			if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
			if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
		}
		for(int i=1; i<=Ny; ++i){
			if(i*YGRID<1000) sprintf(s,"%3d",i*(int)YGRID);
			else sprintf(s,"%3.1fk",i*YGRID/1000);
			glDrawString(s,(XtxtB+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
		}
		glDrawString("nsec",(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);

		/* draw histogram (MAGRO) */
		if(SFLAG){
			glColor3d(0.9,0.0,0.0);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0x5555);
			bool FLAG = false;
			glBegin(GL_LINE_STRIP);
			for(int ch=0;ch<ARRAY1;++ch){
				if(XMIN<=ch*bin1&&(ch+1)*bin1<=XMAX){
					if(hist_MAGRO[ch]==0){
						FLAG = true;
						glVertex2d((ch*bin1+Xoff)/WX,(-YMIN+Yoff)/WY);
						glVertex2d(((ch+1)*bin1+Xoff)/WX,(-YMIN+Yoff)/WY);
					}
					else{
						if(FLAG){
							FLAG = false;
							glVertex2d((ch*bin1+Xoff)/WX,(-YMIN+Yoff)/WY);
							glVertex2d((ch*bin1+Xoff)/WX,(-YMIN+hist_MAGRO[ch]+Yoff)/WY);
							glVertex2d(((ch+1)*bin1+Xoff)/WX,(-YMIN+hist_MAGRO[ch]+Yoff)/WY);
						}
						else{
							glVertex2d((ch*bin1+Xoff)/WX,(-YMIN+hist_MAGRO[ch]+Yoff)/WY);
							glVertex2d(((ch+1)*bin1+Xoff)/WX,(-YMIN+hist_MAGRO[ch]+Yoff)/WY);
						}
					}
				}
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
		/* draw histogram (TUNA) */
		glColor3d(0.0,0.65,0.8);
		glBegin(GL_QUADS);
		for(int ch=0;ch<ARRAY1;++ch){
			if(XMIN<=ch*bin1&&(ch+1)*bin1<=XMAX){
				glVertex2d((ch*bin1+Xoff)/WX,Yoff/WY);
				glVertex2d((ch*bin1+Xoff)/WX,(hist_TUNA[ch]+Yoff)/WY);
				glVertex2d(((ch+1)*bin1+Xoff)/WX,(hist_TUNA[ch]+Yoff)/WY);
				glVertex2d(((ch+1)*bin1+Xoff)/WX,Yoff/WY);
			}
		}
		glEnd();

		/* coordinate line */
		glColor3d(0.0,0.0,0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2d((XMIN+Xoff)/WX,(YMIN+Yoff)/WY);
		glVertex2d((XMAX+Xoff)/WX,(YMIN+Yoff)/WY);
		glVertex2d((XMAX+Xoff)/WX,(YMAX+Yoff)/WY);
		glVertex2d((XMIN+Xoff)/WX,(YMAX+Yoff)/WY);
		glEnd();
	}






	/* LiGlass TOF Spector */
	if(AMODE==1){

		int MAXhist = 0;
		for(int ch=0;ch<ARRAY2;++ch){
			if(MAXhist<hist_Li[ch]) MAXhist = hist_Li[ch];
		}
		int digit = 0;
		int temp  = 1;
		while(temp*5*pow(5,digit) < MAXhist){
			++temp;
			if(temp==9){
				temp = 1;
				++digit;
			}
		}
		double scale = temp*pow(5,digit);

		/* draw range */	
		double XMIN  = 0.0;
		double XMAX  = 30.0*1.01;
		double XGRID = XMAX/6/1.01;
		int Nx = (XMAX-XMIN)/XGRID;

		double YMIN  = 0.0;
		double YMAX  = scale*5;
		double YGRID = scale;
		int Ny = (YMAX-YMIN)/YGRID;

		/* window size */
		double WX    = (XMAX-XMIN)*(100+MARGIN*2)/100;
		double WY    = (YMAX-YMIN)*(100+MARGIN*2)/100;

		/* offset */
		double Xoff  = WX*MARGIN/100;
		double Yoff  = WY*MARGIN/100;

		/* grid line */
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1.0, 0x5555);
		glColor3d(0.5,0.5,0.5);
		glBegin(GL_LINES);
		for(int i=1; i<=Nx; ++i){
			glVertex2d((XGRID*i+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XGRID*i+Xoff)/WX,(YMAX+Yoff)/WY);
		}
		for(int i=1; i<=Ny; ++i){
			glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		/* text */
		double XtxtA1 = -(XMAX-XMIN)*0.010000;
		double XtxtA2 = -(XMAX-XMIN)*0.020000;
		double XtxtA3 = -(XMAX-XMIN)*0.030769;
		double YtxtA  = -(YMAX-YMIN)*0.039215;
		double XtxtB  = -(XMAX-XMIN)*0.076923;
		double YtxtB  = -(YMAX-YMIN)*0.007843;
		glColor3d(0.0,0.0,0.0);
		char title[100];
		double x2;
		if(bin2==1.0){
			sprintf(title,"Li Glass TOF Spector [counts/nsec]");
			x2 = 0.15;
		}
		else{
			sprintf(title,"Li Glass TOF Spector [counts/%3.1fnsec]",bin2);
			x2 = 0.15;
		}
		glDrawString(title,x2,0.95);
		glDrawString("0",(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
		char s[10];
		for(int i=1; i<=Nx; ++i){
			sprintf(s,"%d",i*(int)XGRID);
			int size = strlen(s);
			if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
			if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
			if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
		}
		for(int i=1; i<=Ny; ++i){
			sprintf(s,"%3d",i*(int)YGRID);
			glDrawString(s,(XtxtB+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
		}
		glDrawString("nsec",(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);

		/* draw histogram */
		glColor3d(0.0,0.65,0.8);
		glBegin(GL_QUADS);
		for(int ch=0; ch<ARRAY2; ++ch){
			if(XMIN<=ch*bin2&&(ch+1)*bin2<=XMAX){
				glVertex2d((ch*bin2+Xoff)/WX,Yoff/WY);
				glVertex2d((ch*bin2+Xoff)/WX,(hist_Li[ch]+Yoff)/WY);
				glVertex2d(((ch+1)*bin2+Xoff)/WX,(hist_Li[ch]+Yoff)/WY);
				glVertex2d(((ch+1)*bin2+Xoff)/WX,Yoff/WY);
			}
		}
		glEnd();

		/* coordinate line */
		glColor3d(0.0,0.0,0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2d((XMIN+Xoff)/WX,(YMIN+Yoff)/WY);
		glVertex2d((XMAX+Xoff)/WX,(YMIN+Yoff)/WY);
		glVertex2d((XMAX+Xoff)/WX,(YMAX+Yoff)/WY);
		glVertex2d((XMIN+Xoff)/WX,(YMAX+Yoff)/WY);
		glEnd();
	}








	/* Scintillation Photon Spector */
	if(AMODE==2){

		switch(OMODE){
			case 0:
				glColor3d(0.0,0.0,0.0);
				glDrawString("Reaching Histgram",0.02,0.98);
				break;
			case 1:
				glColor3d(1.0,1.0,1.0);
				glDrawString("Oscillo Scope",0.02,0.98);
				break;
			case 2:
				glColor3d(0.0,0.0,0.0);
				char s[100];
				if(TFLAG) sprintf(s,"TDC Histgram (Threshold:%-.1fmV)",Threshold);
				else      sprintf(s,"TDC Histgram (Threshold:off)");
				glDrawString(s,0.02,0.98);
				break;
			default:
				break;
		}

		/* noise level */
		if(SFLAG){
			if(OMODE==1) glColor3d(1.0,1.0,1.0);
			else glColor3d(0.0,0.0,0.0);
			if(NFLAG) glDrawString("Noise  on",0.78,0.98);
			else      glDrawString("Noise off",0.78,0.98);
		}


		/* make PMT signals */
		if(AFLAG){
			PMToutputInit();
			BaselineNoise(0.5);
			PMTH11934();
			LeadingEdge();
		}
		ThresholdInit();







		/*** reach histgram ***/
		if(OMODE==0){
			glColor3d(0.0,0.0,0.0);
			int MAXhist  = 0;
			int MAXphist = 0;
			for(int ch=0;ch<ARRAY3;++ch){
				if(MAXhist<hist_rPMT[ch]) MAXhist = hist_rPMT[ch];
				if(MAXhist<hist_lPMT[ch]) MAXhist = hist_lPMT[ch];
				if(MAXphist<PreHistR[ch]) MAXphist = PreHistR[ch];
				if(MAXphist<PreHistL[ch]) MAXphist = PreHistL[ch];
			}
			int digit = 0;
			int temp  = 1;
			while(temp*5*pow(5,digit) < MAXhist){
				++temp;
				if(temp==9){
					temp = 1;
					++digit;
				}
			}
			double scale = temp*pow(5,digit);
			/* draw range */	
			double XMIN  = 0.0;
			double XMAX  = ARRAY3*bin3*1.01;
			double XGRID = XMAX/5/1.01;
			int Nx = (XMAX-XMIN)/XGRID;
			double YMIN  = 0.0;
			double YMAX  = scale*5;
			double YGRID = scale;
			int Ny = (YMAX-YMIN)/YGRID;
			/* window size */
			double WX    = (XMAX-XMIN)*(100+MARGIN*2)/100;
			double WY    = (YMAX-YMIN)*(245+MARGIN*2)/100;
			/* offset */
			double Xoff  = WX*MARGIN/100;
			double Yoff  = WY*MARGIN/100;

			/* Right PMT */
			glTranslated(0, 0.48,0);
			/* grid line */
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0x5555);
			glColor3d(0.5,0.5,0.5);
			glBegin(GL_LINES);
			for(int i=1; i<=Nx; ++i){
				glVertex2d((XGRID*i+Xoff)/WX,(YMIN+Yoff)/WY);
				glVertex2d((XGRID*i+Xoff)/WX,(YMAX+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			/* text */
			double XtxtA1 = -(XMAX-XMIN)*0.010000;
			double XtxtA2 = -(XMAX-XMIN)*0.020000;
			double XtxtA3 = -(XMAX-XMIN)*0.030769;
			double YtxtA  = -(YMAX-YMIN)*0.060000;
			double XtxtB  = -(XMAX-XMIN)*0.076923;
			double YtxtB  = -(YMAX-YMIN)*0.007843;
			glColor3d(0.0,0.0,0.0);
			char titleR[100];
			sprintf(titleR,"Right PMT [counts/%4.2fnsec]",bin3);
			glDrawString(titleR,0.24,0.47);
			glDrawString("0",(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
			char s[10];
			for(int i=1; i<=Nx; ++i){
				sprintf(s,"%d",i*(int)XGRID);
				int size = strlen(s);
				if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				sprintf(s,"%3d",i*(int)YGRID);
				glDrawString(s,(XtxtB+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
			}
			glDrawString("nsec",(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);

			/* draw histogram */
			glColor3d(0.0,0.65,0.8);
			glBegin(GL_QUADS);
			for(int ch=0; ch<ARRAY3; ++ch){
				if(XMIN<=ch*bin3&&(ch+1)*bin3<=XMAX){
					glVertex2d((ch*bin3+Xoff)/WX,Yoff/WY);
					glVertex2d((ch*bin3+Xoff)/WX,(hist_rPMT[ch]+Yoff)/WY);
					glVertex2d(((ch+1)*bin3+Xoff)/WX,(hist_rPMT[ch]+Yoff)/WY);
					glVertex2d(((ch+1)*bin3+Xoff)/WX,Yoff/WY);
				}
			}
			glEnd();
			/* draw pre-histogram */
			if(SFLAG&&MAXphist!=0){
				double coef = (MAXhist>0)?(double)MAXhist/MAXphist:5.0/MAXphist;
				bool FLAG = false;
				int gain  = 0;
				glColor3d(1.0,0.2,0.2);
				glBegin(GL_LINE_STRIP);
				for(int ch=0; ch<ARRAY3; ++ch){
					if(XMIN<=ch*bin3&&(ch+1)*bin3<=XMAX){
						if(PreHistR[ch]==0){
							FLAG = true;
							glVertex2d((ch*bin3+Xoff)/WX,Yoff/WY);
							glVertex2d(((ch+1)*bin3+Xoff)/WX,Yoff/WY);
						}
						else{
							if(FLAG){
								FLAG = false;
								glVertex2d((ch*bin3+Xoff)/WX,Yoff/WY);
								glVertex2d((ch*bin3+Xoff)/WX,(coef*PreHistR[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*bin3+Xoff)/WX,(coef*PreHistR[ch]+Yoff)/WY);
							}
							else{
								glVertex2d((ch*bin3+Xoff)/WX,(coef*PreHistR[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*bin3+Xoff)/WX,(coef*PreHistR[ch]+Yoff)/WY);
							}
						}
					}
					gain += PreHistR[ch];
				}
				/* gain of stock */
				glEnd();
				char Gain[50];
				sprintf(Gain,"Gain  %4.2f%%",(double)gain/(PreLoopCounter*PhotN)*100);
				glDrawString(Gain,0.705,0.40);
				char Posi[50];
				sprintf(Posi,"(%+5.1f,%+5.1f)",ppx,ppy);
				glDrawString(Posi,0.68,0.38);
			}

			/* coordinate line */
			glColor3d(0.0,0.0,0.0);
			glBegin(GL_LINE_LOOP);
			glVertex2d((XMIN+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMAX+Yoff)/WY);
			glVertex2d((XMIN+Xoff)/WX,(YMAX+Yoff)/WY);
			glEnd();

			/* gain */
			if(AMODE==2){
				int gain = 0;
				for(int ch=0;ch<ARRAY3;++ch){
					gain += hist_rPMT[ch];
				}
				char Gain[50];
				sprintf(Gain,"Gain  %4.2f%%",(double)gain/(LoopCounter*PhotN)*100);
				glDrawString(Gain,0.705,0.44);
			}

			glTranslated(0,-0.48,0);



			/* Left PMT */
			/* grid line */
			glTranslated(0,-0.02,0);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0x5555);
			glColor3d(0.5,0.5,0.5);
			glBegin(GL_LINES);
			for(int i=1; i<=Nx; ++i){
				glVertex2d((XGRID*i+Xoff)/WX,(YMIN+Yoff)/WY);
				glVertex2d((XGRID*i+Xoff)/WX,(YMAX+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			/* text */
			glColor3d(0.0,0.0,0.0);
			char titleL[100];
			sprintf(titleL,"Left PMT [counts/%4.2fnsec]",bin3);
			glDrawString(titleL,0.25,0.47);
			glDrawString("0",(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
			for(int i=1; i<=Nx; ++i){
				sprintf(s,"%d",i*(int)XGRID);
				int size = strlen(s);
				if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				sprintf(s,"%3d",i*(int)YGRID);
				glDrawString(s,(XtxtB+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
			}
			glDrawString("nsec",(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);
			/* draw histogram */
			glColor3d(0.0,0.65,0.8);
			glBegin(GL_QUADS);
			for(int ch=0; ch<ARRAY3; ++ch){
				if(XMIN<=ch*bin3&&(ch+1)*bin3<=XMAX){
					glVertex2d((ch*bin3+Xoff)/WX,Yoff/WY);
					glVertex2d((ch*bin3+Xoff)/WX,(hist_lPMT[ch]+Yoff)/WY);
					glVertex2d(((ch+1)*bin3+Xoff)/WX,(hist_lPMT[ch]+Yoff)/WY);
					glVertex2d(((ch+1)*bin3+Xoff)/WX,Yoff/WY);
				}
			}
			glEnd();
			/* draw pre-histogram */
			if(SFLAG&&MAXphist!=0){
				double coef = (MAXhist>0)?(double)MAXhist/MAXphist:5.0/MAXphist;
				bool FLAG = false;
				int gain = 0;
				glColor3d(1.0,0.0,0.0);
				glBegin(GL_LINE_STRIP);
				for(int ch=0; ch<ARRAY3; ++ch){
					if(XMIN<=ch*bin3&&(ch+1)*bin3<=XMAX){
						if(PreHistL[ch]==0){
							FLAG = true;
							glVertex2d((ch*bin3+Xoff)/WX,Yoff/WY);
							glVertex2d(((ch+1)*bin3+Xoff)/WX,Yoff/WY);
						}
						else{
							if(FLAG){
								FLAG = false;
								glVertex2d((ch*bin3+Xoff)/WX,Yoff/WY);
								glVertex2d((ch*bin3+Xoff)/WX,(coef*PreHistL[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*bin3+Xoff)/WX,(coef*PreHistL[ch]+Yoff)/WY);
							}
							else{
								glVertex2d((ch*bin3+Xoff)/WX,(coef*PreHistL[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*bin3+Xoff)/WX,(coef*PreHistL[ch]+Yoff)/WY);
							}
						}
					}
					gain += PreHistL[ch];
				}
				glEnd();
				/* gain of stock */
				char Gain[50];
				sprintf(Gain,"Gain  %4.2f%%",(double)gain/(PreLoopCounter*PhotN)*100);
				glDrawString(Gain,0.705,0.40);
				char Posi[50];
				sprintf(Posi,"(%+5.1f,%+5.1f)",ppx,ppy);
				glDrawString(Posi,0.68,0.38);
			}
			/* coordinate line */
			glColor3d(0.0,0.0,0.0);
			glBegin(GL_LINE_LOOP);
			glVertex2d((XMIN+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMAX+Yoff)/WY);
			glVertex2d((XMIN+Xoff)/WX,(YMAX+Yoff)/WY);
			glEnd();

			/* gain */
			if(AMODE==2){
				int gain = 0;
				for(int ch=0;ch<ARRAY3;++ch){
					gain += hist_lPMT[ch];
				}
				char Gain[50];
				sprintf(Gain,"Gain  %4.2f%%",(double)gain/(LoopCounter*PhotN)*100);
				glDrawString(Gain,0.705,0.44);
			}

			glTranslated(0,0.02,0);
		}





		/*** Oscillo Scope Mode ***/
		if(OMODE==1){
			glColor3d(1.0,1.0,1.0);

			for(int ch=0;ch<PMTarray;++ch){
				if(MAXpmt>outputR[ch]) MAXpmt = outputR[ch];
				if(MAXpmt>outputL[ch]) MAXpmt = outputL[ch];
			}
			int digit = 0;
			int temp  = 1;
			while(temp*5*pow(10,digit)<fabs(MAXpmt)){
				++temp;
				if(temp==9){
					temp = 1;
					++digit;
				}
			}
			double scale = (double)temp*pow(10,digit);

			/* draw range */	
			double XMIN  = 0.0;
			double XMAX  = PMTrange*1.01;
			double XGRID = XMAX/5/1.01;
			int Nx = (XMAX-XMIN)/XGRID;

			//double YMIN  =-250.0;
			//double YMAX  =  50.0;
			//double YGRID = (YMAX-YMIN)/6;
			double YMIN  =-scale*5;
			double YMAX  = scale;
			double YGRID = scale;
			int Ny = (YMAX-YMIN)/YGRID;
			ThresholdMAX = YMIN;

			/* window size */
			double WX    = (XMAX-XMIN)*(105+MARGIN*2)/100;
			double WY    = (YMAX-YMIN)*(245+MARGIN*2)/100;

			/* offset */
			double Xoff  = WX*(MARGIN+5)/100;
			double Yoff  = WY*MARGIN/100;



			/*** Right PMT ***/
			glPushMatrix();
			glTranslated(0, 0.48,0);
			/* grid line */
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0x5555);
			glColor3d(0.5,0.5,0.5);
			glBegin(GL_LINES);
			for(int i=1; i<=Nx; ++i){
				glVertex2d((XGRID*i+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
				glVertex2d((XGRID*i+Xoff)/WX,Yoff/WY);
			}
			for(int i=1; i<=Ny; ++i){
				glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			/* text */
			double XtxtA1 = -(XMAX-XMIN)*0.01;
			double XtxtA2 = -(XMAX-XMIN)*0.02;
			double XtxtA3 = -(XMAX-XMIN)*0.03;
			double YtxtA  = -(YMAX-YMIN)*0.06;
			double XtxtB  = -(XMAX-XMIN)*0.13;
			double YtxtB  = -(YMAX-YMIN)*0.01;
			glColor3d(1.0,1.0,1.0);
			char titleR[100];
			sprintf(titleR,"Right PMT [mV]");
			glDrawString(titleR,0.40,0.47);
			glDrawString("0",0.02+(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
			char s[10];
			for(int i=1; i<=Nx; ++i){
				sprintf(s,"%d",i*(int)XGRID);
				int size = strlen(s);
				if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
			}
			for(int i=1; i<=Ny+1; ++i){
				if(YMIN+(i-1)*YGRID>-1000) sprintf(s,"%4.0f",YMIN+(i-1)*YGRID);
				else sprintf(s,"%4.1fk",(YMIN+(i-1)*YGRID)/1000);
				glDrawString(s,(XtxtB+Xoff)/WX,((i-1)*YGRID+YtxtB+Yoff)/WY);
			}
			glDrawString("nsec",-0.05+(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);

			/* draw waveform of oscillo */
			bool FLAG = false;
			glColor3d(0.8,0.8,0.0);
			glBegin(GL_LINE_STRIP);
			for(int ch=0;ch<PMTarray;++ch){
				if(XMIN<=ch*PMTdt&&(ch+1)*PMTdt<=XMAX){
					if(YMIN<outputR[ch] && outputR[ch]<YMAX){
						if(outputR[ch]==0){
							FLAG = true;
							glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
						}
						else{
							if(FLAG){
								FLAG = false;
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+outputR[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+outputR[ch]+Yoff)/WY);
							}
							else{
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+outputR[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+outputR[ch]+Yoff)/WY);
							}
						}
					}
					else if(YMAX<outputR[ch]){
						if(FLAG){
							FLAG = false;
							glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
							glVertex2d((ch*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
						}
						else{
							glVertex2d((ch*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
						}
					}
					else{
						if(outputR[ch]==0){
							FLAG = true;
							glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
						}
						else{
							if(FLAG){
								FLAG = false;
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
								glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
							}
							else{
								glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
							}
						}
					}
				}
			}
			glEnd();

			/* coordinate line */
			glColor3d(1.0,1.0,1.0);
			glBegin(GL_LINE_LOOP);
			glVertex2d((XMIN+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(Yoff)/WY);
			glVertex2d((XMIN+Xoff)/WX,(Yoff)/WY);
			glEnd();

			/* gain */
			if(AMODE==2){
				double Charge = 0;
				for(int ch=0;ch<PMTarray;++ch){
					Charge += outputR[ch]/R_L*PMTdt;
				}
				char chrg[50];
				sprintf(chrg,"gain -%4.1fpC",fabs(Charge));
				glDrawString(chrg,0.705,0.44);
			}

			/* threshold */
			if(TFLAG){
				/* threshold line */
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1.0, 0xf0f0);
				glColor3d(0.9,0.2,0.2);
				glBegin(GL_LINES);
				glVertex2d((XMIN+Xoff)/WX,(-YMIN+Threshold+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(-YMIN+Threshold+Yoff)/WY);
				glEnd();
				char thr[50];
				sprintf(thr,"%6.1fmV",Threshold);
				glDrawString(thr,0.78,(-YMIN+Threshold+Yoff)/WY+0.005);
			}
			glPopMatrix();



			/*** Left PMT ***/
			/* grid line */
			glPushMatrix();
			glTranslated(0, 0.00,0);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0x5555);
			glColor3d(0.5,0.5,0.5);
			glBegin(GL_LINES);
			for(int i=1; i<=Nx; ++i){
				glVertex2d((XGRID*i+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
				glVertex2d((XGRID*i+Xoff)/WX,Yoff/WY);
			}
			for(int i=1; i<=Ny; ++i){
				glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			/* text */
			glColor3d(1.0,1.0,1.0);
			char titleL[100];
			sprintf(titleL," Left PMT [mV]");
			glDrawString(titleL,0.40,0.47);
			glDrawString("0",0.02+(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
			for(int i=1; i<=Nx; ++i){
				sprintf(s,"%d",i*(int)XGRID);
				int size = strlen(s);
				if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
			}
			for(int i=1; i<=Ny+1; ++i){
				if(YMIN+(i-1)*YGRID>-1000) sprintf(s,"%4.0f",YMIN+(i-1)*YGRID);
				else sprintf(s,"%4.1fk",(YMIN+(i-1)*YGRID)/1000);
				glDrawString(s,(XtxtB+Xoff)/WX,((i-1)*YGRID+YtxtB+Yoff)/WY);
			}
			glDrawString("nsec",-0.05+(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);

			/* draw waveform of oscillo */
			FLAG = false;
			glColor3d(0.0,0.6,0.8);
			glBegin(GL_LINE_STRIP);
			for(int ch=0;ch<PMTarray;++ch){
				if(XMIN<=ch*PMTdt&&(ch+1)*PMTdt<=XMAX){
					if(YMIN<outputL[ch] && outputL[ch]<YMAX){
						if(outputL[ch]==0){
							FLAG = true;
							glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
						}
						else{
							if(FLAG){
								FLAG = false;
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+outputL[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+outputL[ch]+Yoff)/WY);
							}
							else{
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+outputL[ch]+Yoff)/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+outputL[ch]+Yoff)/WY);
							}
						}
					}
					else if(YMAX<outputL[ch]){
						if(FLAG){
							FLAG = false;
							glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
							glVertex2d((ch*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
						}
						else{
							glVertex2d((ch*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
						}
					}
					else{
						if(outputL[ch]==0){
							FLAG = true;
							glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
						}
						else{
							if(FLAG){
								FLAG = false;
								glVertex2d((ch*PMTdt+Xoff)/WX,(-YMIN+Yoff)/WY);
								glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
							}
							else{
								glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
								glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
							}
						}
					}
				}
			}
			glEnd();

			/* coordinate line */
			glColor3d(1.0,1.0,1.0);
			glBegin(GL_LINE_LOOP);
			glVertex2d((XMIN+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMAX-YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(Yoff)/WY);
			glVertex2d((XMIN+Xoff)/WX,(Yoff)/WY);
			glEnd();

			/* gain */
			if(AMODE==2){
				double Charge = 0;
				for(int ch=0;ch<PMTarray;++ch){
					Charge += outputL[ch]/R_L*PMTdt;
				}
				char chrg[50];
				sprintf(chrg,"gain -%4.1fpC",fabs(Charge));
				glDrawString(chrg,0.705,0.44);
			}

			/* threshold */
			if(TFLAG){
				/* threshold line */
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1.0, 0xf0f0);
				glColor3d(0.9,0.2,0.2);
				glBegin(GL_LINES);
				glVertex2d((XMIN+Xoff)/WX,(-YMIN+Threshold+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(-YMIN+Threshold+Yoff)/WY);
				glEnd();
				char thr[50];
				sprintf(thr,"%6.1fmV",Threshold);
				glDrawString(thr,0.78,(-YMIN+Threshold+Yoff)/WY+0.005);
				/* cross time line */
				if(crosstimeR!=XMAX){
					glColor3d(0.9,0.2,0.2);
					glBegin(GL_LINES);
					glVertex2d((crosstimeR+Xoff)/WX,0.00);
					glVertex2d((crosstimeR+Xoff)/WX,0.50);
					glVertex2d((crosstimeR+Xoff)/WX,0.53);
					glVertex2d((crosstimeR+Xoff)/WX,2.00);
					glEnd();
				}
				if(crosstimeL!=XMAX){
					glColor3d(0.9,0.2,0.2);
					glBegin(GL_LINES);
					glVertex2d((crosstimeL+Xoff)/WX,0.00);
					glVertex2d((crosstimeL+Xoff)/WX,0.50);
					glVertex2d((crosstimeL+Xoff)/WX,0.53);
					glVertex2d((crosstimeL+Xoff)/WX,2.00);
					glEnd();
				}
				glDisable(GL_LINE_STIPPLE);
				if(crosstimeR!=XMAX && crosstimeL!=XMAX){
					/* time difference */
					glColor3d(0.9,0.2,0.2);
					double tave = (crosstimeR+crosstimeL)/2.0;
					glBegin(GL_LINE_LOOP);
					glVertex2d((tave+Xoff)/WX-0.12,0.50);
					glVertex2d((tave+Xoff)/WX+0.12,0.50);
					glVertex2d((tave+Xoff)/WX+0.12,0.53);
					glVertex2d((tave+Xoff)/WX-0.12,0.53);
					glEnd();
					sprintf(s,"%4.2f nsec",fabs(crosstimeR-crosstimeL));
					glDrawString(s,(tave+Xoff/2)/WX,0.51);
				}
			}
			glPopMatrix();
		}





		/******** PMT histgram ********/
		if(OMODE==2){

			glColor3d(0.0,0.0,0.0);
			int MAXhist = 0;
			for(int ch=0;ch<PMTarray;++ch){
				if(MAXhist<pmthistR[ch]) MAXhist = pmthistR[ch];
				if(MAXhist<pmthistL[ch]) MAXhist = pmthistL[ch];
			}
			int digit = 0;
			int temp  = 1;
			double scale;
			if(LFLAG){
				while(pow(10,digit)<MAXhist){
					++digit;
				}
				scale = pow(10,digit);
				if(scale<1000) scale = 1000;
			}
			else{
				while(temp*5*pow(10,digit)<MAXhist){
					++temp;
					if(temp==9){
						temp = 1;
						++digit;
					}
				}
				scale = temp*pow(10,digit);
			}

			/* draw range */
			double XMIN  = 0.0;
			double XMAX  = 25*1.01;
			double XGRID = XMAX/5/1.01;
			int Nx = (XMAX-XMIN)/XGRID;

			double YMIN;
			double YMAX;
			double YGRID;
			if(LFLAG){
				YMIN  = 0.0;
				YMAX  = log10(scale);
				YGRID = 1;
			}
			else{
				YMIN  = 0.0;
				YMAX  = scale*5;
				YGRID = scale;
			}

			int Ny = (YMAX-YMIN)/YGRID;
			/* window size */
			double WX    = (XMAX-XMIN)*(104+MARGIN*2)/100;
			double WY    = (YMAX-YMIN)*(245+MARGIN*2)/100;
			/* offset */
			double Xoff  = WX*(MARGIN+5)/100;
			double Yoff  = WY*MARGIN/100;


			/* Right PMT */
			glTranslated(0, 0.48,0);
			/* grid line */
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0x5555);
			glColor3d(0.5,0.5,0.5);
			glBegin(GL_LINES);
			for(int i=1; i<=Nx; ++i){
				glVertex2d((XGRID*i+Xoff)/WX,(YMIN+Yoff)/WY);
				glVertex2d((XGRID*i+Xoff)/WX,(YMAX+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			/* text */
			double XtxtA1 = -(XMAX-XMIN)*0.01;
			double XtxtA2 = -(XMAX-XMIN)*0.02;
			double XtxtA3 = -(XMAX-XMIN)*0.03;
			double YtxtA  = -(YMAX-YMIN)*0.06;
			double XtxtB  = -(XMAX-XMIN)*0.11;
			double YtxtB  = -(YMAX-YMIN)*0.01;
			glColor3d(0.0,0.0,0.0);
			char titleR[100];
			sprintf(titleR,"Right PMT [counts/%4.2fnsec]",PMTdt);
			glDrawString(titleR,0.24,0.47);
			if(LFLAG){
				glDrawString("    1",(XtxtB-0.5+Xoff)/WX,(YtxtB+Yoff)/WY);	
				glDrawString("0",(XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);	
			}
			else glDrawString("0",(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
			char s[10];
			for(int i=1; i<=Nx; ++i){
				sprintf(s,"%d",i*(int)XGRID);
				int size = strlen(s);
				if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				if(LFLAG){
					sprintf(s,"%5d",(int)pow(10,YGRID*i));
					glDrawString(s,(XtxtB-0.5+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
				}
				else{
					sprintf(s,"%4d",i*(int)YGRID);
					glDrawString(s,(XtxtB+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
				}
			}
			glDrawString("nsec",(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);

			/* draw histogram */
			glColor3d(0.0,0.65,0.8);
			glBegin(GL_QUADS);
			for(int ch=0;ch<PMTarray;++ch){
				if(XMIN<=ch*PMTdt&&(ch+1)*PMTdt<=XMAX){
					glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
					if(LFLAG){
						if(pmthistR[ch]==0){
							glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
						}
						else{
							glVertex2d((ch*PMTdt+Xoff)/WX,(log10(pmthistR[ch])+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(log10(pmthistR[ch])+Yoff)/WY);
						}
					}
					else{
						glVertex2d((ch*PMTdt+Xoff)/WX,(pmthistR[ch]+Yoff)/WY);
						glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(pmthistR[ch]+Yoff)/WY);
					}
					glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
				}
			}
			glEnd();

			/* coordinate line */
			glColor3d(0.0,0.0,0.0);
			glBegin(GL_LINE_LOOP);
			glVertex2d((XMIN+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMAX+Yoff)/WY);
			glVertex2d((XMIN+Xoff)/WX,(YMAX+Yoff)/WY);
			glEnd();

			glTranslated(0,-0.48,0);


			/* Left PMT */
			/* grid line */
			glTranslated(0,-0.02,0);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1.0, 0x5555);
			glColor3d(0.5,0.5,0.5);
			glBegin(GL_LINES);
			for(int i=1; i<=Nx; ++i){
				glVertex2d((XGRID*i+Xoff)/WX,(YMIN+Yoff)/WY);
				glVertex2d((XGRID*i+Xoff)/WX,(YMAX+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				glVertex2d((XMIN+Xoff)/WX,(YGRID*i+Yoff)/WY);
				glVertex2d((XMAX+Xoff)/WX,(YGRID*i+Yoff)/WY);
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			/* text */
			glColor3d(0.0,0.0,0.0);
			char titleL[100];
			sprintf(titleL,"Left PMT [counts/%4.2fnsec]",PMTdt);
			glDrawString(titleL,0.25,0.47);
			if(LFLAG){
				glDrawString("    1",(XtxtB-0.5+Xoff)/WX,(YtxtB+Yoff)/WY);	
				glDrawString("0",(XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);	
			}
			else glDrawString("0",(XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);	
			for(int i=1; i<=Nx; ++i){
				sprintf(s,"%d",i*(int)XGRID);
				int size = strlen(s);
				if(size == 1) glDrawString(s,(i*XGRID+XtxtA1+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 2) glDrawString(s,(i*XGRID+XtxtA2+Xoff)/WX,(YtxtA+Yoff)/WY);
				if(size == 3) glDrawString(s,(i*XGRID+XtxtA3+Xoff)/WX,(YtxtA+Yoff)/WY);
			}
			for(int i=1; i<=Ny; ++i){
				if(LFLAG){
					sprintf(s,"%5d",(int)pow(10,YGRID*i));
					glDrawString(s,(XtxtB-0.5+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
				}
				else{
					sprintf(s,"%4d",i*(int)YGRID);
					glDrawString(s,(XtxtB+Xoff)/WX,(i*YGRID+YtxtB+Yoff)/WY);
				}
			}
			glDrawString("nsec",(XMAX/2+Xoff)/WX,(YtxtA*2+Yoff)/WY);
			/* draw histogram */
			glColor3d(0.0,0.65,0.8);
			glBegin(GL_QUADS);
			for(int ch=0;ch<PMTarray;++ch){
				if(XMIN<=ch*PMTdt&&(ch+1)*PMTdt<=XMAX){
					glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
					if(LFLAG){
						if(pmthistL[ch]==0){
							glVertex2d((ch*PMTdt+Xoff)/WX,Yoff/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
						}
						else{
							glVertex2d((ch*PMTdt+Xoff)/WX,(log10(pmthistL[ch])+Yoff)/WY);
							glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(log10(pmthistL[ch])+Yoff)/WY);
						}
					}
					else{
						glVertex2d((ch*PMTdt+Xoff)/WX,(pmthistL[ch]+Yoff)/WY);
						glVertex2d(((ch+1)*PMTdt+Xoff)/WX,(pmthistL[ch]+Yoff)/WY);
					}
					glVertex2d(((ch+1)*PMTdt+Xoff)/WX,Yoff/WY);
				}
			}
			glEnd();
			/* coordinate line */
			glColor3d(0.0,0.0,0.0);
			glBegin(GL_LINE_LOOP);
			glVertex2d((XMIN+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMIN+Yoff)/WY);
			glVertex2d((XMAX+Xoff)/WX,(YMAX+Yoff)/WY);
			glVertex2d((XMIN+Xoff)/WX,(YMAX+Yoff)/WY);
			glEnd();

			glTranslated(0,0.02,0);
		}

	}
}


/***************************/

#endif //"DecExp_h_"
