#ifndef Object_h_
#define Object_h_

/*****************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#ifdef __APPLE__
#include<GLUT/glut.h>
#endif

#ifdef linux
#include<GL/glut.h>
#endif

#include"MyGLUT.h"
#include"Colors.h"
#include"define.h"
#include"PMT.h"

/* Animation Mode (0:Neutron(TUNA) 1:Li Glass 2:Photon) */
int  AMODE  = 0;

/* parameters for camera */
double dstnc = 300.0;
double theta = 230*PI/180;
double phi   =  30*PI/180;
double xc    =   0.0;
double yc    =   0.0;
double zc    =-150.0;

/* photon initial position */
double px    =   0.0;
double py    =   0.0;

/* height of beamline */
const double Hbeam = 144.0;

/* size of scintillator */
const double x_max = 100.0/2.0;
const double y_max =  23.0/2.0;
const double z_max =   3.0/2.0;

/* size of light guide */
const double guide_length = 13.0;
const double x_lgd = x_max + guide_length;
const double y_lgd = 2.3/2.0;
const double z_lgd = 2.3/2.0;

const double x_pmt = x_lgd + 3.0;
const double y_pmt = y_lgd;
const double z_pmt = z_lgd;

/* slope of light guide */
const double psi   = atan((y_max - y_lgd)/(x_lgd - x_max));
const double xi    = atan((z_max - z_lgd)/(x_lgd - x_max));

/* size of Li glass */
const double R_Li  =   2.40;
const double L_Li  =   1.00;
const double d_Li  =   0.20;
const double r1_Li =   2.30;
const double r2_Li =   3.50;
const double r3_Li =   3.40;
const double r4_Li =   3.00;
const double l1_Li =   3.00;
const double l2_Li =   1.00;
const double l3_Li =  16.60;
const double l4_Li =  23.80;
const double x_Li  =   0.00;
const double y_Li  = -20.00;
const double z_Li  =-150.00;


/* floor of ENcourse */
void glENfloor(void){
	if(PFLAG){
		glMaterialColor(ms_floor);
		glRectangular(2000,2,2000,0,-Hbeam-1,0);
	}
}


/* F2chamber */
void glF2beamline(void){
	glMaterialColor(ms_metal);
	double r1 = 5.0;
	double r2 = 3.0;
	double r3 = 5.5;
	/* F2 Slit Box */
	glRectangular( 15.0, 32.0, 32.0,160.450, 0.0,-150.00);
	/* F2 Up */
	glPipe(11.50,6.70,1.2, 143.55, 0.0,-150.0);
	glRectangular( 20.0,45.0,30.0,132.950, 0.0,-150.00);
	glRectangular( 20.0,30.0, 1.5,132.950, 0.0,-165.75);
	glRectangular( 20.0,30.0, 1.5,132.950, 0.0,-134.25);
	glPipe(11.50,6.70,1.2, 122.35, 0.0,-150.0);
	/* Vacuum Pump */
	glRotated( 90,0,1,0);
	glCylinder( 9.25, 1.2, 132.90, 0.0,132.950);
	glCylinder(   r1, 7.8, 128.40, 0.0,132.950);
	glCylinder( 9.25, 3.0, 123.00, 0.0,132.950);
	glCylinder(   r1, 5.0, 119.00, 0.0,132.950);
	glCylinder( 6.00,14.0, 109.50, 0.0,132.950);
	glCylinder( 9.25, 3.0, 101.00, 0.0,132.950);
	glCylinder(   r1,13.0,  93.00, 0.0,132.950);
	glCylinder( 0.50,13.0,  93.00, 0.0+r3*cos(PI*1/4),132.950+r3*sin(PI*1/4));
	glCylinder( 0.50,13.0,  93.00, 0.0+r3*cos(PI*3/4),132.950+r3*sin(PI*3/4));
	glCylinder( 0.50,13.0,  93.00, 0.0+r3*cos(PI*5/4),132.950+r3*sin(PI*5/4));
	glCylinder( 0.50,13.0,  93.00, 0.0+r3*cos(PI*7/4),132.950+r3*sin(PI*7/4));
	glCylinder( 6.00, 2.0,  85.50, 0.0,132.950);
	glCylinder( 1.50, 2.0,103.00,-32.5,132.950);
	glCylinder( 2.50,12.0, 96.00,-32.5,132.950);
	glCylinder( 3.50, 1.0, 97.50,-32.5,132.950);
	glCylinder( 3.50, 1.0, 89.50,-32.5,132.950);
	glCylinder( 0.50, 2.0, 88.00,-32.5,132.950);
	glRotated(-90,0,1,0);
	glRotated( 90,0,0,1);
	glCylinder(   r1, 9.0, -8.00,-132.95,-109.5);
	glCylinder( 9.25, 2.5,-13.75,-132.95,-109.5);
	glCylinder( 6.00,11.5,-20.75,-132.95,-109.5);
	glCylinder(   r1, 8.5,-30.75,-132.95,-109.5);
	glCylinder( 2.00, 5.0,-33.50,-132.95,-86.00);
	glRotated(-90,0,0,1);
	glRectangular( 5.0, 5.0, 1.0,132.95,-32.50,-104.5);
	glRectangular( 3.0,10.0, 6.0,132.95,   0.0, -81.5);
	/* F2 Down */
	glPipe(11.50,6.70,1.2, 103.55, 0.0,-150.0);
	glRectangular( 20.0, 50.0, 30.0, 92.950, 0.0,-150.00);
	glRectangular( 20.0, 30.0,  1.5, 92.950, 0.0,-165.75);
	glRectangular( 20.0, 30.0,  1.5, 92.950, 0.0,-134.25);
	glRotated( 90,0,1,0);
	glCylinder( 9.25, 1.2, 167.10, 0.0,92.950);
	glRotated(-90,0,1,0);
	glPipe(11.50,6.70,1.2,  82.35, 0.0,-150.0);
	/* Gate valve */
	glPipe( 9.25,4.45,9.0,  74.35, 0.0,-150.0);
	glRectangular(  3.5, 14.0, 10.0, 74.35, 0.0,-138.95);
	glRectangular(  5.5,  8.0,  2.0, 74.35, 0.0,-132.95);
	glRotated( 90,0,1,0);
	glCylinder( 2.75, 19.0, 122.45, 0.0,74.35);
	glRotated(-90,0,1,0);
	/* F2 Slit Box */
	glRectangular( 15.9, 30.0, 30.0, 59.200, 0.0,-150.0);
	/* F2 Box PL */
	glRectangular( 10.0, 11.0, 11.0, 41.625, 0.0,-150.0);
	glRotated( 90,0,0,1);
	glPipe(  r2,0.20,13.5, 12.25,-41.625,-150.0);
	glPipe(  r2,0.20,13.5,-12.25,-41.625,-150.0);
	glCylinder( 6.00, 2.0, 20.00,-41.625,-150.0);
	glCylinder( 6.00, 2.0,-20.00,-41.625,-150.0);
	glRotated(-90,0,0,1);
	glRotated( 90,0,1,0);
	glPipe(  r1,0.20,28.5, 130.25, 0.0,41.625);
	glPipe(  r1,0.20,28.5, 169.75, 0.0,41.625);
	glCylinder( 9.25, 0.8, 184.40, 0.0,41.625);
	glCylinder( 9.25, 0.8, 185.70, 0.0,41.625);
	glCylinder( 9.25, 0.8, 115.60, 0.0,41.625);
	glCylinder( 9.25, 0.8, 114.30, 0.0,41.625);
	glRotated(-90,0,1,0);
	/* pipe duct */
	glPipe(  r1,0.20,170.0, 122.0, 0.0,-150.0);
	glPipe( 9.25,6.45,1.00,36.125, 0.0,-150.0);
	glPipe( 9.25,6.45,1.00,35.125, 0.0,-150.0);
	glPipe(r2,0.20,12.625,28.3125, 0.0,-150.0);
	glPipe( 6.00,3.20,1.00, 21.50, 0.0,-150.0);
	glCylinder( 6.00, 1.00,-21.50, 0.0,-150.0);
	glPipe( 9.25,4.45,5.00,182.45, 0.0,-150.0);
	glPipe( 6.00,1.20,0.80,207.40, 0.0,-150.0);
	/* square duct */	
	double L  = sqrt(50);	
	double y0 =-150*sqrt(2-2*cos(PI/4))*cos(PI/8);
	double z0 =-150*sqrt(2-2*cos(PI/4))*sin(PI/8);
	glPushMatrix();
	glTranslated(0.0, y0, z0);
	glRotated( 45,1,0,0);
	glRectangular(250.0, L, L,325.950, 0.0,-150.00);
	glPopMatrix();
	/* black plastic flange */	
	glMaterialColor(ms_black_plastic);
	glRotated( 90,0,1,0);
	glCylinder( 9.25,  0.5, 185.05, 0.0,41.625);
	glCylinder( 9.25,  0.5, 114.95, 0.0,41.625);
	glRotated(-90,0,1,0);
}


/* Q Magnet */
void glQmagnet(void){
	/* Frame */
	glMaterialColor(ms_Qframe);
	double y0 =-150*sqrt(2-2*cos(PI/4))*cos(PI/8);
	double z0 =-150*sqrt(2-2*cos(PI/4))*sin(PI/8);
	glPushMatrix();
	glTranslated(0.0, y0, z0);
	glRotated( 45,1,0,0);
	glRectangular( 40.0, 57.0, 57.0,235.950, 0.0,-150.00);
	glPopMatrix();
	/* Magnets */
	glMaterialColor(ms_Qmagnet);
	double R = 28.4;
	double L = (37.0-sqrt(50))/2;
	double offset = 15;
	glPushMatrix();
	glTranslated(0.0, y0, z0);
	glRotated( 45,1,0,0);
	glRotated( 90,0,0,1);
	glCylinder(R,L, offset,-235.95,-150);
	glCylinder(R,L,-offset,-235.95,-150);
	glRotated(-90,0,0,1);
	glRotated( 90,0,1,0);
	glCylinder(R,L,-(-150+offset),0,235.95);
	glCylinder(R,L,-(-150-offset),0,235.95);
	glRotated(-90,0,1,0);
	glPopMatrix();
}


/* acryl chamber */
void glAcrylChamber(void){
	/* screw */
	glMaterialColor(ms_metal);
	glRotated( 90,0,1,0);
	for(int i=0;i<12;++i){
		double r = 4.20;
		double t = 2.0*PI*(double)i/12.0+PI/12;
		glCylinder( 0.18, 0.2,152.80, r*sin(t), r*cos(t));
		glCylinder( 0.10, 0.5,152.45, r*sin(t), r*cos(t));
	}
	glRotated(-90,0,1,0);
	/* O-ring */
	glMaterialColor(ms_black_rubber);
	glRotated( 90,0,1,0);
	glPipe( 3.90,0.20,0.20,152.45, 0.0, 0.0);
	glRotated(-90,0,1,0);
	/* stopper */
	glPushMatrix();
	glTranslated(0,0,-150);
	glRotated( 45,0,1,0);
	glCylinder(1.50,0.11,0,0,0);
	glMaterialColor(ms_metal);
	glRectangular(0.1,3.5,4.0,0,0,0);
	glPopMatrix();
	/* kapton film */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialColor(ms_kapton);
	glRotated( 90,0,1,0);
	glCylinder( 3.40, 0.05,152.70, 0.0, 0.0);
	glRotated(-90,0,1,0);
	/* acryl */
	glMaterialColor(ms_acryl);
	for(int i=0;i<2;++i){
		glPipe(1.50,0.10,6.3,pow(-1,i)* 7.35, 0.0,-150.0);
		glPipe(2.35,0.85,0.2,pow(-1,i)*10.40, 0.0,-150.0);
		glPipe(2.35,0.10,9.5,pow(-1,i)*15.25, 0.0,-150.0);
		glPipe(6.00,3.75,1.0,pow(-1,i)*20.50, 0.0,-150.0);
	}
	glDisable(GL_CULL_FACE);
	glRotated( 90,0,1,0);
	glPipe(4.50,0.10,4.90,150.000, 0.0, 0.0);
	glCylinder( 4.50,0.25,152.575, 0.0, 0.0);
	glCylinder( 4.50,0.25,147.425, 0.0, 0.0);
	glRotated(-90,0,1,0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}


/* Al Frame Function */
void glAlFrame(double L, double x, double y, double z){
	if(L<=0){
		printf("error : the argument L of glAlFrame() must be positive.\n");
		exit(1);
	}
	double m = 0.01;
	L += m;
	const double l = 4.0;
	/* Aluminium */
	glMaterialColor(ms_Aluminium);
	glRectangular(L,l/3,l/3,x,y+l/3,z+l/3);
	glRectangular(L,l/3,l/3,x,y+l/3,z-l/3);
	glRectangular(L,l/3,l/3,x,y-l/3,z+l/3);
	glRectangular(L,l/3,l/3,x,y-l/3,z-l/3);
	glMaterialColor(ms_Aluminium2);
	glRectangular(L,l/3,l/3,x,y,z);
	/* rid */	
	glMaterialColor(ms_Alframerid);
	const double d = 0.3;
	glRectangular(d,l-m,l-m,x+(L+d)/2,y,z);
	glRectangular(d,l-m,l-m,x-(L+d)/2,y,z);
}


/* L-Shaped Mold Steel Function */
void glMoldSteelL(double L, int e1, double theta, int e2, double phi, double x, double y, double z){
	glMaterialColor(ms_moldsteel);
	glPushMatrix();
	glTranslated(x,y,z);
	switch(e2){
		case 1:
			glRotated(phi,1,0,0);
			break;
		case 2:
			glRotated(phi,0,1,0);
			break;
		case 3:
			glRotated(phi,0,0,1);
			break;
		default:
			break;
	}
	switch(e1){
		case 1:
			glRotated(theta,1,0,0);
			break;
		case 2:
			glRotated(theta,0,1,0);
			break;
		case 3:
			glRotated(theta,0,0,1);
			break;
		default:
			break;
	}
	double d = 0.2;
	double l = 4.0;
	glRectangular(L,l,d,0,0,(-l+d)/2);
	glRectangular(L,d,l,0,(-l+d)/2,0);
	glPopMatrix();
}


/* ENcourse Framework Foundation */
void glFoundation(void){
	/* Beamline Foundation */	
	//	GLfloat SPEColor0[] = { 0.2, 0.1, 0.0, 1.0};
	//	GLfloat SHIColor0[] = { 10.0};
	//	GLfloat DIFColor0[] = { 0.5, 0.4, 0.3, 1.0};
	//	GLfloat AMBColor0[] = { 0.3, 0.2, 0.1, 1.0};
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPEColor0);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,SHIColor0);	
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  DIFColor0);	
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  AMBColor0);	
	//	glRectangular( 12.0,Hbeam+16, 1.0,173.950,-60.0,-135.50);
	//	glRectangular( 12.0,Hbeam+16, 1.0,173.950,-60.0,-164.50);

	/* TUNA & MAGRO Foundation */
	double L1 = 170.0;
	double L2 = Hbeam+40;
	double L3 = 40.0;
	double L4 = 30.0;
	glAlFrame(L1,0, 30,8);
	glAlFrame(L1,0,-30,8);
	glAlFrame(L1,0,-85,-(L3+L4)/2);
	glAlFrame(L1,0,-Hbeam+2, (L3+4)/2);
	glAlFrame(L1,0,-Hbeam+2,-(L3+4)/2);
	glRotated( 90,0,1,0);
	for(int i=0;i<2;++i){
		glAlFrame(L3,0, 34,pow(-1,i)*70);
		glAlFrame(L3,0,-34,pow(-1,i)*70);
		glAlFrame(L3,0,-Hbeam+2,pow(-1,i)*70);
		glAlFrame(L3,0,-Hbeam+2,pow(-1,i)*(L1/2-2));
		glAlFrame(L4,(L3+L4)/2+4,-81,pow(-1,i)*70);
		glAlFrame(L4,(L3+L4)/2-4,-81,pow(-1,i)*43);
		glAlFrame(L4,(L3+L4)/2+4,-Hbeam+2,pow(-1,i)*20);
	}
	glRotated(-90,0,1,0);
	glRotated( 90,0,0,1);
	glAlFrame(56,0, 52,8);
	glAlFrame(56,0,-52,8);
	for(int i=0;i<4;++i){
		glAlFrame(L2,-(L2/2-44),pow(-1,i/2)*70,pow(-1,i)*(L3+4)/2);
	}
	glRotated(-90,0,0,1);
	if(AMODE==2){
		glRotated( 90,0,1,0);
		glAlFrame(30,-18,py-6,px);
		glRotated(-90,0,1,0);
		glRotated( 90,0,0,1);
		glAlFrame(64,7,-px-4,12);
		glRotated(-90,0,0,1);
		glMaterialColor(ms_metal);
		glRectangular(4,8,8,px,py,8);
		glRectangular(0.2,3,3,px+1.5,py,8);
		glRectangular(0.2,3,3,px-1.5,py,8);
		glRotated( 90,0,0,1);
		glPipe(1.0,0.1,0.8,py+3.55,-px,8);
		glPipe(1.0,0.7,0.1,py+3.10,-px,8);
		glCylinder(1.2,0.1,py+4.05,-px,8);
		glCylinder(0.5,0.6,py+3.55,-px,8);
		glRotated(-90,0,0,1);
		glRotated( 90,0,1,0);
		glPipe(1.2,1.0,0.1,-3.95,py,px);
		glRotated(-90,0,1,0);
	}
	/* prop */
	glMaterialColor(ms_metal);
	for(int i=0;i<2;++i){
		glRectangular(4.2,y_max*3/4,0.6,pow(-1,i)*50.9, y_max*5/8,5.7);
		glRectangular(3.0,y_max*2  ,0.6,pow(-1,i)*51.5,       0.0,5.7);
		glRectangular(4.2,y_max*3/4,0.6,pow(-1,i)*50.9,-y_max*5/8,5.7);
		glRotated( 90,0,1,0);
		for(int j=0;j<2;++j){
			glCylinder(0.25,4.50,-4.30,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glHexagon( 0.60,0.50,-2.85,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glHexagon( 0.60,0.50,-5.15,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glHexagon( 0.60,0.50,-6.25,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glPipe(0.60,0.3,0.50,-5.15,pow(-1,j)*(y_max-0.6),pow(-1,i)*(x_max+2.0));
			glPipe(0.65,0.5,0.06,-5.37,pow(-1,j)*(y_max-0.6),pow(-1,i)*(x_max+2.0));
			glCylinder(0.60,0.20,-5.30,pow(-1,j)*(y_max-0.6),pow(-1,i)*(x_max+2.0));
			glCylinder(0.25,1.50,-6.15,pow(-1,j)*(y_max-0.6),pow(-1,i)*(x_max+2.0));
			glRectangular(1.3,2,1.3,-6.6,pow(-1,j)*(y_max-0.6),pow(-1,i)*(x_max+2.0));
		}
		glRotated(-90,0,1,0);
	}
	/* lead block */
	glMaterialColor(ms_lead);
	for(int i=0;i<4;++i){
		glRectangular(20,5,10,pow(-1,i/2)*(L1/2-10),-Hbeam+2.5,pow(-1,i)*(L3/2+9));
		glRectangular(10,5,20,pow(-1,i/2)*(L1/2+5.3),-Hbeam+2.5,pow(-1,i)*(L3/2+4));
	}
	/* Chibitaku Foundation */
	double L5 = 55.0;
	double L6 = Hbeam-52;
	for(int i=0;i<2;++i){
		glAlFrame(L5,0,-68,-196+pow(-1,i)*(L5/2+2));
		glAlFrame(L5,0,-Hbeam+7,-196+pow(-1,i)*(L5/2+2));
		glRotated( 90,0,1,0);
		glAlFrame(L5,196,-68,pow(-1,i)*(L5/2+2));
		glAlFrame(L5,196,-Hbeam+7,pow(-1,i)*(L5/2+2));
		glRotated(-90,0,1,0);
	}
	glPushMatrix();
	glTranslated(0,0,-196);
	glRotated( 45,0,1,0);
	for(int i=0;i<2;++i){
		glAlFrame(L5*4/7,0,-68,pow(-1,i)*(L5*3/7));
		glRotated( 90,0,1,0);
		glAlFrame(L5*4/7,0,-68,pow(-1,i)*(L5*3/7));
		glRotated(-90,0,1,0);
	}
	glPopMatrix();
	glRotated( 90,0,0,1);
	for(int i=0;i<4;++i){
		glAlFrame(L6,-L6/2-47,pow(-1,i)*(L5/2+2),-196+pow(-1,i/2)*(L5/2+2));
	}
	glRotated(-90,0,0,1);
	/* Caster */
	glMaterialColor(ms_metal);
	for(int i=0;i<4;++i){
		glRectangular(12,0.2,16,pow(-1,i)*(L5/2-2),-Hbeam+4.9,-196+pow(-1,i/2)*(L5/2-4));	
		glRotated( 90,0,0,1);
		glCylinder(2.5, 1.6,-Hbeam+0.8,pow(-1,i)*(L5/2-2),-196+pow(-1,i/2)*(L5/2-2));
		glCylinder(0.5,10.0,-Hbeam+6.0,pow(-1,i)*(L5/2-2),-196+pow(-1,i/2)*(L5/2-2));
		glRotated(-90,0,0,1);
	}
	/* Acryl Board */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialColor(ms_acryl);
	glRectangular(L5+5.0,1.0,L5-5.0,0,-65.5,-196);
	glDisable(GL_BLEND);
	/* Li Glass Foundation */
	double L7 = 20.0;
	double L8 = Hbeam+y_Li-l3_Li;
	double y0 = y_Li-L_Li-l3_Li;
	glMoldSteelL(L7,3,90,2,45, 0,y0+L7/2,-150-R_Li);
	glMoldSteelL(L7,2, 45,0,0, 5,y0+2,-145);
	glMoldSteelL(L7,2,225,0,0,-4,y0+2,-154);
	for(int i=0;i<4;++i){
		glMoldSteelL(L7,1,90,2,90*i,(L7/2-2)*sin(-PI/2*i),y0-2,-150-(L7/2-2)*cos(-PI/2*i));
		glMoldSteelL(L7,0, 0,2,90*i,(L7/2-2)*sin(-PI/2*i),-Hbeam+2,-150-(L7/2-2)*cos(-PI/2*i));
		double alpha = (90*i+135)*PI/180;
		double A = sin(alpha)/sin(PI/4);
		double B = cos(alpha)/cos(PI/4);
		glMoldSteelL(L8,3,90,2,90*i,(L7/2-2.2)*A,y0-L8/2,-150+(L7/2-2.2)*B);
	}
	/* Beta Pla Foundation */
	glMaterialColor(ms_white_plastic);
	glRectangular(1.5,1.0,6.5,0.0,4.0,-160.0);
}


/* ENcourse */
void glENcourse(void){
	glENfloor();
	glQmagnet();

	glPushMatrix();
	glTranslated(72,0,0);
	glQmagnet();
	glTranslated(72,0,0);
	glQmagnet();
	glPopMatrix();

	glF2beamline();
	glFoundation();
	glAcrylChamber();
}


/* neutron detector TUNA */
void glTUNA(void){
	glMaterialColor(ms_TUNA);
	/* define vertex */
	double pla[8][3];
	double lgd[8][3];
	double pmt[8][3];
	double d = 0.1;
	/* vertex(points) asign */
	for (int i = 0; i < 8; ++i){
		pla[i][0] = pow(-1,1-i/4)*(x_max);
		pla[i][1] = pow(-1,1-i/2)*(y_max+d);
		pla[i][2] = pow(-1,i)*(z_max+d);
		lgd[i][0] = pow(-1,1-i/4)*(x_lgd);
		lgd[i][1] = pow(-1,1-i/2)*(y_lgd+d);
		lgd[i][2] = pow(-1,i)*(z_lgd+d);
		pmt[i][0] = pow(-1,1-i/4)*(x_pmt);
		pmt[i][1] = pow(-1,1-i/2)*(y_pmt+d);
		pmt[i][2] = pow(-1,i)*(z_pmt+d);
	}
	GLdouble vertex[][3] = {             /* No.*/
		{pla[0][0],pla[0][1],pla[0][2]}, /*  0 */ 
		{pla[1][0],pla[1][1],pla[1][2]}, /*  1 */
		{pla[2][0],pla[2][1],pla[2][2]}, /*  2 */
		{pla[3][0],pla[3][1],pla[3][2]}, /*  3 */
		{pla[4][0],pla[4][1],pla[4][2]}, /*  4 */
		{pla[5][0],pla[5][1],pla[5][2]}, /*  5 */
		{pla[6][0],pla[6][1],pla[6][2]}, /*  6 */
		{pla[7][0],pla[7][1],pla[7][2]}, /*  7 */
		{lgd[0][0],lgd[0][1],lgd[0][2]}, /*  8 */
		{lgd[1][0],lgd[1][1],lgd[1][2]}, /*  9 */
		{lgd[2][0],lgd[2][1],lgd[2][2]}, /* 10 */
		{lgd[3][0],lgd[3][1],lgd[3][2]}, /* 11 */
		{lgd[4][0],lgd[4][1],lgd[4][2]}, /* 12 */
		{lgd[5][0],lgd[5][1],lgd[5][2]}, /* 13 */
		{lgd[6][0],lgd[6][1],lgd[6][2]}, /* 14 */
		{lgd[7][0],lgd[7][1],lgd[7][2]}, /* 15 */			
		{pmt[0][0],pmt[0][1],pmt[0][2]}, /* 16 */
		{pmt[1][0],pmt[1][1],pmt[1][2]}, /* 17 */
		{pmt[2][0],pmt[2][1],pmt[2][2]}, /* 18 */
		{pmt[3][0],pmt[3][1],pmt[3][2]}, /* 19 */
		{pmt[4][0],pmt[4][1],pmt[4][2]}, /* 20 */
		{pmt[5][0],pmt[5][1],pmt[5][2]}, /* 21 */
		{pmt[6][0],pmt[6][1],pmt[6][2]}, /* 22 */
		{pmt[7][0],pmt[7][1],pmt[7][2]}  /* 23 */
	};
	/* edge asign */
	int edge[][2] = {
		{  0, 1 },{  0, 2 },{  1, 3 },{  2, 3 }, /*  Scinti Body  ( 0~11) */
		{  4, 5 },{  4, 6 },{  5, 7 },{  6, 7 },
		{  0, 4 },{  1, 5 },{  2, 6 },{  3, 7 },
		{  0, 8 },{  1, 9 },{  2,10 },{  3,11 }, /* Light Guide L (12~19) */
		{  8, 9 },{  9,11 },{ 11,10 },{ 10, 8 },
		{  4,12 },{  5,13 },{  6,14 },{  7,15 }, /* Light Guide R (20~27) */
		{ 12,13 },{ 13,15 },{ 15,14 },{ 14,12 },
		{  8,16 },{  9,17 },{ 11,19 },{ 10,18 }, /*     Tube L    (28~35) */
		{ 16,17 },{ 17,19 },{ 19,18 },{ 18,16 },
		{ 12,20 },{ 13,21 },{ 14,22 },{ 15,23 }, /*     Tube R    (36~43) */
		{ 20,21 },{ 21,23 },{ 23,22 },{ 22,20 }
	};
	/* face asign */
	int face[][4] = {
		{  0, 2, 6, 4 }, /*  Scinti Body  ( 0~ 3) */
		{  1, 5, 7, 3 },
		{  0, 4, 5, 1 },
		{  2, 3, 7, 6 },
		{  0, 1, 9, 8 }, /* Light Guide L ( 4~ 7) */
		{  0, 8,10, 2 },
		{  2,10,11, 3 },
		{  1, 3,11, 9 },
		{  4,12,13, 5 }, /* Light Guide R ( 8~11) */
		{  4, 6,14,12 },
		{  6, 7,15,14 },
		{  5,13,15, 7 },
		{  8,16,18,10 }, /*     Tube L    (12~15) */
		{ 10,18,19,11 },
		{  9,11,19,17 },
		{  9,17,16, 8 },
		{ 12,14,22,20 }, /*     Tube R    (16~19 ) */
		{ 14,15,23,22 },
		{ 15,13,21,23 },
		{ 13,12,20,21 }
	};
	/* normal vector asign (for shadow) */
	GLdouble normal[][3] = {
		{      0.0,      0.0,      1.0 }, /*  Scinti Body  ( 0~ 3) */
		{      0.0,      0.0,     -1.0 },
		{      0.0,     -1.0,      0.0 },
		{      0.0,      1.0,      0.0 },
		{-sin(psi),-cos(psi),      0.0 }, /* Light Guide L ( 4~ 7) */
		{ -sin(xi),      0.0,  cos(xi) },
		{-sin(psi), cos(psi),      0.0 },
		{ -sin(xi),      0.0, -cos(xi) },
		{ sin(psi),-cos(psi),      0.0 }, /* Light Guide R ( 8~11) */
		{  sin(xi),      0.0,  cos(xi) },
		{ sin(psi), cos(psi),      0.0 },
		{  sin(xi),      0.0, -cos(xi) },
		{      0.0,      0.0,      1.0 }, /*     Tube L    (12~15) */
		{      0.0,      1.0,      0.0 },
		{      0.0,      0.0,     -1.0 },
		{      0.0,     -1.0,      0.0 },
		{      0.0,      0.0,      1.0 }, /*     Tube R    (16~19) */
		{      0.0,      1.0,      0.0 },
		{      0.0,      0.0,     -1.0 },
		{      0.0,     -1.0,      0.0 }
	};
	/* construction(lines) */
	if( PFLAG != true ){
		glBegin(GL_LINES);
		for(int i=0;i<44;++i){
			glVertex3dv(vertex[edge[i][0]]);
			glVertex3dv(vertex[edge[i][1]]);
		}
		glEnd();
	}
	else{ /* construction(polygon) */
		glBegin(GL_QUADS);
		for(int j=0;j<20;++j){
			glNormal3dv(normal[j]);
			for(int i=0;i<4;++i){
				glVertex3dv(vertex[face[j][i]]);
			}
		}
		glEnd();
	}
	/* PMT (H-11934) */
	glMaterialColor(ms_PMT);
	double lx = 3.25;
	double ly = 3.00;
	double lz = 3.00;
	glRectangular(lx,ly,lz, (x_pmt+lx/2), 0.0, 0.0);
	glRectangular(lx,ly,lz,-(x_pmt+lx/2), 0.0, 0.0);
	/* cooling system (duct) */
	glMaterialColor(ms_duct);
	glPipe( 3.50,0.1,18.90, (x_lgd+11.95), 0.0, 0.0);
	glPipe( 3.50,0.1,18.90,-(x_lgd+11.95), 0.0, 0.0);
	glRotated( 90,0,0,1);
	glPipe( 0.30,0.10,2.50,-4.45, (x_lgd+ 4.40), 0.0);
	glPipe( 0.30,0.10,2.50, 4.45, (x_lgd+18.90), 0.0);
	glRotated(-90,0,0,1);
	glPushMatrix();
	glRotated(-45,1,0,0);
	glRotated( 90,0,0,1);
	glPipe( 0.30,0.10,2.50, 4.45,-(x_lgd+ 4.40), 0.0);
	glPipe( 0.30,0.10,2.50,-4.45,-(x_lgd+18.90), 0.0);
	glPopMatrix();
	/* cooling system (metal parts) */
	glMaterialColor(ms_metal);
	for(int i=0;i<8;++i){
		glHexagon( 0.60,0.5,pow(-1,1-i/4)*(x_lgd+1.45),pow(-1,1-i/2)*5.0,pow(-1,1-i)*2.2);
	}
	for(int i=0;i<2;++i){
		for(int j=0;j<2;++j){
			glRectangular(1.20, 0.40,4.70,pow(-1,i)*(x_max-0.60),pow(-1,j)*11.90,-0.25);
			glRectangular(1.20, 3.70,0.40,pow(-1,i)*(x_max-0.60),pow(-1,j)*10.25, 1.90);
			glRectangular(1.20, 9.50,0.90,pow(-1,i)*(x_max-0.60),pow(-1,j)*4.800, 2.15);
			glRectangular(1.20,11.65,0.90,pow(-1,i)*(x_max-0.60),pow(-1,j)*5.855,-2.15);
		}
		for(int j=0;j<4;++j){
			glRectangular(1.20, 0.50,1.20,pow(-1,i)*(x_max-0.60),pow(-1,j/2)*0.3,pow(-1,j)*(z_max+1.7));
		}
		glRectangular(0.7,11.4,9.3,pow(-1,i)*(x_lgd+0.85), 0.0, 0.0);
		glCylinder( 4.65, 0.60,pow(-1,i)*(x_lgd+ 2.20), 0.0, 0.0);
		glCylinder( 4.65, 0.50,pow(-1,i)*(x_lgd+21.65), 0.0, 0.0);
	}
	glRotated( 90,0,0,1);
	for(int i=0;i<4;++i){
		glCylinder( 0.30, 2.20, 0.0,pow(-1,1-i/2)*(x_max-0.6),pow(-1,1-i)*(z_max+d+1.7));
	}
	for(int i=0;i<8;++i){
		glHexagon( 0.60,0.50,pow(-1,1-i/4)*0.8,pow(-1,1-i/2)*(x_max-0.6),pow(-1,1-i)*(z_max+d+1.7));
	}
	glRotated(-90,0,0,1);
	/* handle */
	for(int i=0;i<2;++i){
		for(int j=0;j<2;++j){
			glRotated( 90,0,1,0);
			glPipe(0.60,0.3,0.50,3.01,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glPipe(0.65,0.5,0.06,2.73,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glCylinder(0.60,0.20,2.80,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glCylinder(0.25,0.60,2.40,pow(-1,j)*7.0,pow(-1,i)*(x_max-0.6));
			glRotated(-90,0,1,0);
			glRectangular(1.2, 3.0,0.2,pow(-1,j)*(x_max-0.6),pow(-1,i)*7.0,-2.60);
			glRectangular(1.2, 0.2,5.5,pow(-1,j)*(x_max-0.6),pow(-1,i)*5.5,-5.25);
		}
		glRectangular(1.2,11.2,0.2,pow(-1,i)*(x_max-0.6), 0.0,-8.00);
	}
	/* Connecter */	
	glPushMatrix();
	glRotated( 45,1,0,0);
	glCylinder(0.80, 0.10, (x_lgd+22.65), 0.00, 0.00);
	glCylinder(1.15, 0.14, (x_lgd+22.67), 2.30, 0.00);
	glCylinder(1.15, 0.14, (x_lgd+22.67),-2.30, 0.00);
	glHexagon( 0.95, 0.14, (x_lgd+22.67), 0.00,-2.15);
	glHexagon( 0.95, 0.14, (x_lgd+22.67), 1.10, 1.85);
	glHexagon( 0.95, 0.14, (x_lgd+22.67),-1.10, 1.85);
	glPipe(0.50,0.10,3.00, (x_lgd+22.15), 2.30, 0.00);
	glPipe(0.50,0.10,3.00, (x_lgd+22.15),-2.30, 0.00);
	glPipe(0.40,0.05,2.50, (x_lgd+22.15), 0.00, 0.00);
	glPipe(0.40,0.05,2.50, (x_lgd+22.15), 0.00,-2.15);
	glPipe(0.40,0.05,2.50, (x_lgd+22.15), 1.10, 1.85);
	glPipe(0.40,0.05,2.50, (x_lgd+22.15),-1.10, 1.85);
	for(int i=0;i<4;++i){
		glCylinder(0.20, 0.20, (x_lgd+22.76), 2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
		glCylinder(0.20, 0.20, (x_lgd+22.76),-2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
		glCylinder(0.15, 0.64, (x_lgd+22.42), 2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
		glCylinder(0.15, 0.64, (x_lgd+22.42),-2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
	}
	glPopMatrix();

	glCylinder(0.80, 0.10,-(x_lgd+22.65), 0.00, 0.00);
	glCylinder(1.15, 0.14,-(x_lgd+22.67), 2.30, 0.00);
	glCylinder(1.15, 0.14,-(x_lgd+22.67),-2.30, 0.00);
	glPipe(0.50,0.10,2.00,-(x_lgd+22.67), 2.30, 0.00);
	glPipe(0.50,0.10,2.00,-(x_lgd+22.67),-2.30, 0.00);
	glHexagon( 0.95, 0.14,-(x_lgd+22.67), 0.00,-2.15);
	glHexagon( 0.95, 0.14,-(x_lgd+22.67), 1.10, 1.85);
	glHexagon( 0.95, 0.14,-(x_lgd+22.67),-1.10, 1.85);
	glPipe(0.50,0.10,4.00,-(x_lgd+21.65), 2.30, 0.00);
	glPipe(0.50,0.10,4.00,-(x_lgd+21.65),-2.30, 0.00);
	glPipe(0.40,0.05,3.50,-(x_lgd+21.65), 0.00, 0.00);
	glPipe(0.40,0.05,3.50,-(x_lgd+21.65), 0.00,-2.15);
	glPipe(0.40,0.05,3.50,-(x_lgd+21.65), 1.10, 1.85);
	glPipe(0.40,0.05,3.50,-(x_lgd+21.65),-1.10, 1.85);
	for(int i=0;i<4;++i){
		glCylinder(0.20, 0.20,-(x_lgd+22.76), 2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
		glCylinder(0.20, 0.20,-(x_lgd+22.76),-2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
		glCylinder(0.15, 0.64,-(x_lgd+22.42), 2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
		glCylinder(0.15, 0.64,-(x_lgd+22.42),-2.30+0.9*sin(PI/2*i+PI/4),0.9*cos(PI/2*i+PI/4));
	}

	for(int i=0;i<8;++i){
		glCylinder( 0.21, 0.70,pow(-1,i/4)*(x_lgd+2.15),4.15*sin(PI/2*i),4.15*cos(PI/2*i));
		glCylinder( 0.42, 0.05,pow(-1,i/4)*(x_lgd+2.53),4.15*sin(PI/2*i),4.15*cos(PI/2*i));
		glCylinder( 0.30, 0.30,pow(-1,i/4)*(x_lgd+2.70),4.15*sin(PI/2*i),4.15*cos(PI/2*i));
	}
	for(int i=0;i<8;++i){
		double alpha = PI/2*i+PI/4*floor(i/4); 
		glCylinder( 0.21, 0.70,pow(-1,i/4)*(x_lgd+22.25),4.15*sin(alpha),4.15*cos(alpha));
		glCylinder( 0.42, 0.05,pow(-1,i/4)*(x_lgd+22.63),4.15*sin(alpha),4.15*cos(alpha));
		glCylinder( 0.30, 0.30,pow(-1,i/4)*(x_lgd+22.80),4.15*sin(alpha),4.15*cos(alpha));
	}
	/* protect plate */
	double w1 = 2.5;
	double w2 = 0.3;
	GLdouble vertex2[][3] = {                            /* No.*/
		{          x_max+w2, y_max+d-w1, (z_max+d+0.1)}, /*  0 */   
		{ x_lgd-w1*cos(psi),        0.0, (z_lgd+d+0.1)}, /*  1 */
		{          x_max+w2,-y_max-d+w1, (z_max+d+0.1)}, /*  2 */
		{          x_max+w2, y_max+d-w1,-(z_max+d+0.1)}, /*  3 */
		{          x_max+w2,-y_max-d+w1,-(z_max+d+0.1)}, /*  4 */
		{ x_lgd-w1*cos(psi),        0.0,-(z_lgd+d+0.1)}, /*  5 */
		{         -x_max-w2, y_max+d-w1, (z_max+d+0.1)}, /*  6 */
		{         -x_max-w2,-y_max-d+w1, (z_max+d+0.1)}, /*  7 */
		{-x_lgd+w1*cos(psi),        0.0, (z_lgd+d+0.1)}, /*  8 */
		{         -x_max-w2, y_max+d-w1,-(z_max+d+0.1)}, /*  9 */
		{-x_lgd+w1*cos(psi),        0.0,-(z_lgd+d+0.1)}, /* 10 */
		{         -x_max-w2,-y_max-d+w1,-(z_max+d+0.1)}  /* 11 */
	};
	int edge2[][2] = {
		{ 0, 1},{ 1, 2},{ 2, 0},
		{ 3, 4},{ 4, 5},{ 5, 3},
		{ 6, 7},{ 7, 8},{ 8, 6},
		{ 9,10},{10,11},{11, 9}
	};
	int face2[][3] = {
		{ 6, 7, 8},{ 9,10,11},
		{ 0, 1, 2},{ 3, 4, 5}
	};
	GLdouble normal2[][3] = {
		{ -sin(xi),      0.0,  cos(xi) },
		{ -sin(xi),      0.0, -cos(xi) },
		{  sin(xi),      0.0,  cos(xi) },
		{  sin(xi),      0.0, -cos(xi) }
	};
	if(PFLAG!=true){
		glBegin(GL_LINES);
		for(int i=0;i<12;++i){
			glVertex3dv(vertex2[edge2[i][0]]);
			glVertex3dv(vertex2[edge2[i][1]]);
		}
		glEnd();
	}
	else{
		glBegin(GL_TRIANGLES);
		for(int j=0;j<4;++j){
			glNormal3dv(normal2[j]);
			for(int i=0;i<3;++i){
				glVertex3dv(vertex2[face2[j][i]]);
			}
		}
		glEnd();
	}
	/* brass hex */
	glMaterialColor(ms_brass);
	double Lhex = x_lgd - x_max + 0.5;
	for(int i=0;i<8;++i){
		glHexagon( 0.30,Lhex,pow(-1,i/4)*(x_max+Lhex/2),pow(-1,i/2)*5.0,pow(-1,i)*2.2);
		glCylinder(0.30,1.30,pow(-1,i/4)*(x_max-0.60),pow(-1,i/2)*5.0,pow(-1,i)*2.2);
		glCylinder(0.30,1.40,pow(-1,i/4)*(x_max+Lhex+0.7),pow(-1,i/2)*5.0,pow(-1,i)*2.2);
	}
	/* Derlin flange */	
	glMaterialColor(ms_white_plastic);
	glCylinder( 4.65, 0.70, (x_lgd+ 1.55), 0.0, 0.0);
	glCylinder( 4.65, 0.70,-(x_lgd+ 1.55), 0.0, 0.0);
	/* End flange */
	glMaterialColor(ms_black_plastic);
	glCylinder( 4.65, 0.70, (x_lgd+22.25), 0.0, 0.0);
	glCylinder( 4.65, 0.70,-(x_lgd+22.25), 0.0, 0.0);
}


/* neutron detector MAGRO */
void glMAGRO(void){
	glPushMatrix();
	glTranslated(0.0,0.0,-150.0);
	glRotated( 26,1,0,0);
	glMaterialColor(ms_MAGRO);
	/* define vertex */
	const int N   =   300; /* N-sided polygon */
	double R      = 150.0;
	double R0     = R;
	double theta  = 61.2*PI/360.0;
	double dtheta = 2*theta/(double)N;
	double L      = R*sin(theta);
	double d      =   2.0;
	double y0     =  20.0;
	GLdouble vertex[4*N+1][3];
	for(int i=0;i<4;++i){
		theta = 61.2*PI/360.0;
		if(i>=2){
			R = R0+d;
			L = R*sin(theta);
		}
		for(int j=0;j<N;++j){
			vertex[i*N+j][0] = R*sin(theta);
			double x = fabs(R*sin(theta));
			if(x<L-20){
				double y = y0-x*5.0/(L-20.0);
				vertex[i*N+j][1] = pow(-1,i)*y;
			}
			else{
				double y = y0-5.0-(x-L+20.0)*12.5/20.0;
				vertex[i*N+j][1] = pow(-1,i)*y;
			}
			vertex[i*N+j][2] = R*cos(theta);
			theta -= dtheta;
		}
	}
	int edge[4*N+1][2];
	for(int i=0;i<4;++i){
		for(int j=0;j<N;++j){
			edge[i*N+j][0] = i*N+j;
			edge[i*N+j][1] = i*N+j+1;
		}
		edge[i*N+N-1][0] = i*N+N-1;
		edge[i*N+N-1][1] = i*N+N-1;
	}
	int face[2*(N-1)+1][4];
	for(int i=0;i<2;++i){
		for(int j=0;j<N-1;++j){
			if(i==0){
				face[i*(N-1)+j][0] = (i*2)*N+j;
				face[i*(N-1)+j][1] = (i*2)*N+j+1;
				face[i*(N-1)+j][2] = (i*2+1)*N+j+1;
				face[i*(N-1)+j][3] = (i*2+1)*N+j;
			}
			else{
				face[i*(N-1)+j][0] = (i*2)*N+j;
				face[i*(N-1)+j][3] = (i*2)*N+j+1;
				face[i*(N-1)+j][2] = (i*2+1)*N+j+1;
				face[i*(N-1)+j][1] = (i*2+1)*N+j;
			}
		}
	}
	GLdouble normal[2*(N-1)+1][3];
	theta = 61.2*PI/360;
	for(int i=0;i<2;++i){
		for(int j=0;j<N-1;++j){
			normal[i*(N-1)+j][0] = pow(-1,i+1)*sin(theta);
			normal[i*(N-1)+j][1] = 0.0;
			normal[i*(N-1)+j][2] = pow(-1,i+1)*cos(theta);
			theta -= dtheta;
		}
	}
	if(PFLAG != true){
		for(int i=0;i<4;++i){
			glBegin(GL_LINES);
			for(int j=0;j<N;++j){
				glVertex3dv(vertex[edge[i*N+j][0]]);
				glVertex3dv(vertex[edge[i*N+j][1]]);
			}
			glEnd();
		}
		int area[] = {0,N*14/100,N*50/100,N*86/100,N-1};
		for(int i=0;i<5;++i){
			glBegin(GL_LINE_LOOP);
			glVertex3d(vertex[    area[i]][0],vertex[    area[i]][1],vertex[    area[i]][2]);
			glVertex3d(vertex[  N+area[i]][0],vertex[  N+area[i]][1],vertex[  N+area[i]][2]);
			glVertex3d(vertex[3*N+area[i]][0],vertex[3*N+area[i]][1],vertex[3*N+area[i]][2]);
			glVertex3d(vertex[2*N+area[i]][0],vertex[2*N+area[i]][1],vertex[2*N+area[i]][2]);
			glEnd();
		}
	}
	else{
		glBegin(GL_QUADS);
		for(int i=0;i<2*(N-1);++i){
			glNormal3dv(normal[i]);
			for(int j=0;j<4;++j){
				glVertex3dv(vertex[face[i][j]]);
			}
		}
		glEnd();
		glBegin(GL_QUAD_STRIP);
		for(int i=0;i<N;++i){
			glVertex3d(vertex[i][0],vertex[i][1],vertex[i][2]);
			glVertex3d(vertex[i+2*N][0],vertex[i+2*N][1],vertex[i+2*N][2]);
		}
		glEnd();
		glBegin(GL_QUAD_STRIP);
		for(int i=0;i<N;++i){
			glVertex3d(vertex[i+3*N][0],vertex[i+3*N][1],vertex[i+3*N][2]);
			glVertex3d(vertex[i+N][0],vertex[i+N][1],vertex[i+N][2]);
		}
		glEnd();
	}
	/* PMT(H1161) */
	glMaterialColor(ms_PMT);
	glRotated( 30.6,0,1,0);
	glCylinder(3.5, 1.5,  0.0,0.0,R-d/2);
	glCylinder(3.0,20.0, 10.0,0.0,R-d/2);
	glRotated(-30.6*2,0,1,0);
	glCylinder(3.5, 1.5,  0.0,0.0,R-d/2);
	glCylinder(3.0,20.0,-10.0,0.0,R-d/2);
	glRotated( 30.6,0,1,0);
	glPopMatrix();
}


/* Beta Plastic */
void glBetaPlastic(void){
	double xoff =    0.0;
	double yoff =    4.3;
	double zoff = -153.3;
	glTranslated(+xoff,+yoff,+zoff);

	glMaterialColor(ms_betapla);
	double d   = 0.2;
	double lx1 = 2.0;
	double lx2 = 4.0;
	double lx3 = 2.7;
	double ly1 = 8.0;
	double ly2 = 5.0;
	/* scinti part */
	glPushMatrix();
	glTranslated(0,-0.1,-0.01);
	GLdouble vertex1[][3] = {	/* No.*/
		{ lx3,   0, 0},			/*  0 */			
		{ lx2,-ly2, 0},			/*  1 */
		{ lx1,-ly1, 0},			/*  2 */
		{-lx1,-ly1, 0},			/*  3 */
		{-lx2,-ly2, 0},			/*  4 */
		{-lx3,   0, 0},			/*  5 */
		{ lx3,   0,-d},			/*  6 */
		{ lx2,-ly2,-d},			/*  7 */
		{ lx1,-ly1,-d},			/*  8 */
		{-lx1,-ly1,-d},			/*  9 */
		{-lx2,-ly2,-d},			/* 10 */
		{-lx3,   0,-d}			/* 11 */
	};
	int edge1[][2] = {
		{ 0, 1},{ 1, 2},{ 2, 3},
		{ 3, 4},{ 4, 5},{ 5, 0},
		{ 0, 6},{ 1, 7},{ 2, 8},
		{ 3, 9},{ 4,10},{ 5,11},
		{ 6, 7},{ 7, 8},{ 8, 9},
		{ 9,10},{10,11},{11, 6}
	};
	int face1[][4] = {
		{ 0, 1, 4, 5},{ 1, 2, 3, 4},
		{ 0, 6, 7, 1},{ 1, 7, 8, 2},
		{ 2, 8, 9, 3},{ 3, 9,10, 4},
		{ 4,10,11, 5},{ 5,11, 6, 0},
		{ 6,11,10, 7},{ 7,10, 9, 8}
	};
	double t1 = atan((lx2-lx3)/ly2);
	double t2 = atan((lx2-lx1)/(ly2-ly1));
	GLdouble norm1[][3] = {
		{       0,       0, 1},{       0,       0, 1},
		{-cos(t1), sin(t1), 0},{-cos(t2), sin(t2), 0},
		{       0,      -1, 0},{ cos(t2), sin(t2), 0},
		{ cos(t1), sin(t1), 0},{       0,       1, 0},
		{       0,       0,-1},{       0,       0,-1}
	};
	if(PFLAG!=true){
		glBegin(GL_LINES);
		for(int i=0;i<18;++i){
			glVertex3dv(vertex1[edge1[i][0]]);
			glVertex3dv(vertex1[edge1[i][1]]);
		}
		glEnd();
	}
	else{
		glBegin(GL_QUADS);
		for(int i=0;i<10;++i){
			glNormal3dv(norm1[i]);
			for(int j=0;j<4;++j){
				glVertex3dv(vertex1[face1[i][j]]);
			}
		}
		glEnd();
	}
	glPopMatrix();
	/* curve part */
	glPushMatrix();
	glTranslated(0.0,-0.2,-0.8);
	const int N     = 100; /* N-sided polygon */
	const int array = 4*N+1;
	double R = 0.8;
	GLdouble vertex2[array][3];
	for(int j=0;j<N;++j){
		vertex2[j    ][0] =-lx3;
		vertex2[j    ][1] = R*cos((PI/2*(double)j)/(double)N);
		vertex2[j    ][2] = R*sin((PI/2*(double)j)/(double)N);
		vertex2[j+  N][0] = lx3;
		vertex2[j+  N][1] = vertex2[j][1];
		vertex2[j+  N][2] = vertex2[j][2];
		vertex2[j+2*N][0] =-lx3;
		vertex2[j+2*N][1] = (R-d)*cos((PI/2*(double)j)/(double)N);
		vertex2[j+2*N][2] = (R-d)*sin((PI/2*(double)j)/(double)N);
		vertex2[j+3*N][0] = lx3;
		vertex2[j+3*N][1] = vertex2[j+2*N][1];
		vertex2[j+3*N][2] = vertex2[j+2*N][2];
	}
	int face2[N+1][4];
	for(int j=0;j<N-1;++j){
		face2[j][0] = j    ;
		face2[j][1] = j+N  ;
		face2[j][2] = j+N+1;
		face2[j][3] = j  +1;
	}
	face2[N-1][0] =   N-1;
	face2[N-1][1] = 2*N-1;
	face2[N-1][2] =   N+1;
	face2[N-1][3] =     1;
	int face3[N+1][4];
	for(int j=0;j<N-1;++j){
		face3[j][0] = j+2*N;
		face3[j][1] = j+2*N+1;
		face3[j][2] = j+3*N+1;
		face3[j][3] = j+3*N;
	}
	face3[N-1][0] = 3*N-1;
	face3[N-1][1] = 2*N+1;
	face3[N-1][2] = 3*N+1;
	face3[N-1][3] = 4*N-1;
	int face4[N+1][4];
	for(int j=0;j<N-1;++j){
		face4[j][0] = j;
		face4[j][1] = j+1;
		face4[j][2] = j+2*N+1;
		face4[j][3] = j+2*N;
	}
	face4[N-1][0] =   N-1;
	face4[N-1][1] =     0;
	face4[N-1][2] =   2*N;
	face4[N-1][3] = 3*N-1;
	int face5[N+1][4];
	for(int j=0;j<N-1;++j){
		face5[j][0] = j+N;
		face5[j][1] = j+3*N;
		face5[j][2] = j+3*N+1;
		face5[j][3] = j+N+1;
	}
	face5[N-1][0] = 2*N-1;
	face5[N-1][1] = 4*N-1;
	face5[N-1][2] =   3*N;
	face5[N-1][3] =     N;
	GLdouble normal2[N][3];
	for(int j=0;j<N;++j){
		normal2[j][0] = 0.0;
		normal2[j][1] = cos((PI/2*(double)j)/(double)N);
		normal2[j][2] = sin((PI/2*(double)j)/(double)N);
	}
	GLdouble normal3[N][3];
	for(int j=0;j<N;++j){
		normal3[j][0] =-0.0;
		normal3[j][1] =-cos((PI/2*(double)j)/(double)N);
		normal3[j][2] =-sin((PI/2*(double)j)/(double)N);
	}
	if(PFLAG){
		glBegin(GL_QUADS);
		for(int j=0;j<N-1;++j){
			glNormal3dv(normal2[j]);
			for(int i=0;i<4;++i){
				glVertex3dv(vertex2[face2[j][i]]);
			}
			glNormal3dv(normal3[j]);
			for(int i=0;i<4;++i){
				glVertex3dv(vertex2[face3[j][i]]);
			}
			glNormal3d(-1,0,0);
			for(int i=0;i<4;++i){
				glVertex3dv(vertex2[face4[j][i]]);
			}
			glNormal3d( 1,0,0);
			for(int i=0;i<4;++i){
				glVertex3dv(vertex2[face5[j][i]]);
			}
		}
		glEnd();
	}
	else{
		for(int i=0;i<4;++i){
			if(i==2) R=R-d;
			glBegin(GL_LINE_STRIP);
			for(int j=0;j<N;++j){
				double x = pow(-1,i)*lx3;
				double y = R*cos((PI/2*(double)j)/(double)N);
				double z = R*sin((PI/2*(double)j)/(double)N);
				glVertex3d(x,y,z);
			}
			glEnd();
		}
	}
	glPopMatrix();
	/* PMT connect part */
	glRectangular(lx3*2,d,1.0,0.0,0.5,-1.3);
	glPushMatrix();
	glTranslated(0,0.5,-1.8);
	double lz = 2.2;
	double r  = 0.5;
	double l  = r/sqrt(2);
	GLdouble vertex3[][3] = { /*No.*/
		{ lx3, d/2,  0},      /* 0 */ 
		{   l,   l,-lz},      /* 1 */ 
		{  -l,   l,-lz},      /* 2 */ 
		{-lx3, d/2,  0},      /* 3 */ 
		{ lx3,-d/2,  0},      /* 4 */ 
		{   l,  -l,-lz},      /* 5 */ 
		{  -l,  -l,-lz},      /* 6 */ 
		{-lx3,-d/2,  0}       /* 7 */ 
	};
	int edge6[][2] = {
		{0,1},{1,2},{2,3},{3,0},
		{4,5},{5,6},{6,7},{7,4},
		{0,4},{1,5},{2,6},{3,7}
	};
	int face6[][4] = {
		{0,1,5,4},{0,3,2,1},
		{2,3,7,6},{4,5,6,7}
	};
	double t4 = atan((l-d/2)/lz);
	double t5 = atan((lx3-l)/lz);
	GLdouble norm6[][3] = {
		{ cos(t5),       0,-sin(t5)},
		{       0, cos(t4), sin(t4)},
		{-cos(t5),       0,-sin(t5)},
		{       0,-cos(t4), sin(t4)}
	};
	if(PFLAG != true){
		glBegin(GL_LINES);
		for(int i=0;i<12;++i){
			glVertex3dv(vertex3[edge6[i][0]]);
			glVertex3dv(vertex3[edge6[i][1]]);
		}
		glEnd();
	}
	else{
		glBegin(GL_QUADS);
		for(int i=0;i<4;++i){
			glNormal3dv(norm6[i]);
			for(int j=0;j<4;++j){
				glVertex3dv(vertex3[face6[i][j]]);
			}
		}
		glEnd();
	}
	glPopMatrix();
	/* PMT part */
	glRotated( 90,0,1,0);
	glCylinder(r,6.0,7.0,r,0.0);
	glRotated(-90,0,1,0);
	glTranslated(-xoff,-yoff,-zoff);
}


/* Ge detector Chibitaku */
void glChibitaku(void){
	/* Dewar */
	double r = 21.0;
	glMaterialColor(ms_dewar);
	/* define vertex */
	const int N   =    100; /* N-sided polygon */
	const int M   =    200;
	double l      =    8.0;
	double R      = (r*r+l*l)/(2*l);
	double theta  = acos(r/R);
	double dtheta = (PI/2-theta)/(double)M;
	double phi    =    0.0;
	double dphi   = 2.0*PI/(double)N;
	double x0     =    0.0;
	double y0     = -25.0-R*sin(theta);
	double z0     = -196.0;
	GLdouble vertex[M*N+1][3];
	for(int i=0;i<M;++i){
		phi = 0.0;
		for(int j=0;j<N;++j){
			vertex[i*N+j][0] = x0+R*cos(theta)*cos(phi);
			vertex[i*N+j][1] = y0+R*sin(theta);
			vertex[i*N+j][2] = z0+R*cos(theta)*sin(phi);
			phi += dphi;
		}
		theta += dtheta;
	}
	int edge[M*N+1][2];
	for(int j=0;j<M*N;++j){
		edge[j][0] =   j;
		edge[j][1] = j+1;
	}
	int face[M*N+1][4];
	for(int i=0;i<M-1;++i){
		for(int j=0;j<N;++j){
			face[i*N+j][0] =       i*N+j;
			face[i*N+j][1] =     i*N+j+1;
			face[i*N+j][2] = (i+1)*N+j+1;
			face[i*N+j][3] =   (i+1)*N+j;
		}
		face[(i+1)*N][0] = (i+1)*N-1;
		face[(i+1)*N][1] =       i*N;
		face[(i+1)*N][2] =   (i+1)*N;
		face[(i+1)*N][3] = (i+2)*N-1;
	}
	GLdouble normal[M*N+1][3];
	theta = asin(r/R);
	for(int i=0;i<M-1;++i){
		phi = 0.0;
		for(int j=0;j<N;++j){
			normal[i*N+j][0] = cos(theta)*cos(phi);
			normal[i*N+j][1] = sin(theta);
			normal[i*N+j][2] = cos(theta)*sin(phi);
			phi += dphi;
		}
		theta += dtheta;
	}
	if(PFLAG != true){
		glBegin(GL_LINES);
		for(int i=0;i<5;++i){
			int k = N/5*i;
			for(int j=0;j<M-1;++j){
				glVertex3dv(vertex[edge[M*k+j][0]]);
				glVertex3dv(vertex[edge[M*k+j][1]]);
			}
		}
		glEnd();
	}
	else{
		glBegin(GL_QUADS);
		for(int i=0;i<M-2;++i){
			for(int j=0;j<N;++j){
				glNormal3dv(normal[i*N+j]);
				for(int k=0;k<4;++k){
					glVertex3dv(vertex[face[i*N+j][k]]);
				}
			}
		}
		glEnd();
	}
	glRotated( 90,0,0,1);
	glCylinder(  r,40.0,-45.0,0.0,z0);
	glCylinder(4.0, 8.0,-16.3,0.0,z0+4.3);
	glCylinder(3.0, 4.0,-10.3,0.0,z0+4.3);
	glRotated(-90,0,0,1);
	/* Paint */
	glMaterialColor(ms_paint);
	glRotated( 90,0,0,1);
	glPipe(r+0.01,0.1,5.0,-33.0,0.0,z0);
	glPipe(r+0.01,0.1,5.0,-55.0,0.0,z0);
	glRotated(-90,0,0,1);
	/* Crystal */	
	glMaterialColor(ms_white_plastic);
	glRotated( 90,0,0,1);
	glCylinder(4.0, 1.5, -7.3,0.0,z0+4.3);
	glRotated(-90,0,0,1);
	glMaterialColor(ms_Ge);
	glRotated( 90,0,1,0);
	glCylinder(3.5,10.0,-(z0+36.3),0.0,0.0);
	glCylinder(3.8,10.0,-(z0+26.3),0.0,0.0);
	glCylinder(1.7,20.0,-(z0+14.3),0.0,0.0);
	glRotated(-90,0,1,0);
	glRotated( 90,0,0,1);
	glCylinder(2.5,20.0, -8.3,0.0,z0+4.3);
	glCylinder(4.0, 1.5, -7.3+1.5,0.0,z0+4.3);
	glCylinder(4.0, 1.5, -7.3-1.5,0.0,z0+4.3);
	glRotated(-90,0,0,1);
	/* Window Cap */
	glMaterialColor(ms_Gecap);
	glRotated( 90,0,1,0);
	glCylinder(3.8, 2.0,-(z0+41.3),0.0,0.0);
	glRotated(-90,0,1,0);
}



/* LiGlass */
void glLiGlass(void){
	/* Body */
	glMaterialColor(ms_Liglass);
	glRotated( 90,0,0,1);
	glPipe(R_Li+d_Li,d_Li,L_Li,y_Li-L_Li/2,x_Li,z_Li);	
	glPipe(r3_Li,d_Li,l3_Li,y_Li-L_Li-l3_Li/2,x_Li,z_Li);
	glRotated(-90,0,0,1);
	/* Magnetic Shield */
	glMaterialColor(ms_magshield);
	glRotated( 90,0,0,1);
	glPipe(r2_Li,d_Li,l2_Li,y_Li-l2_Li/2,x_Li,z_Li);
	glRotated(-90,0,0,1);
	/* Magnetic Shield */
	const int N     =    200; /* N-sided polygon */
	const int array =  4*N+1;
	GLdouble verLi[array][3];
	for(int j=0;j<N;++j){
		verLi[j    ][0] = x_Li+r2_Li*cos((2.0*PI*(double)j)/(double)N);
		verLi[j    ][1] = y_Li;
		verLi[j    ][2] = z_Li+r2_Li*sin((2.0*PI*(double)j)/(double)N);
		verLi[j+  N][0] = x_Li+r1_Li*cos((2.0*PI*(double)j)/(double)N);
		verLi[j+  N][1] = y_Li+l1_Li;
		verLi[j+  N][2] = z_Li+r1_Li*sin((2.0*PI*(double)j)/(double)N);
		verLi[j+2*N][0] = x_Li+(r2_Li-d_Li)*cos((2.0*PI*(double)j)/(double)N);
		verLi[j+2*N][1] = y_Li;
		verLi[j+2*N][2] = z_Li+(r2_Li-d_Li)*sin((2.0*PI*(double)j)/(double)N);
		verLi[j+3*N][0] = x_Li+(r1_Li-d_Li)*cos((2.0*PI*(double)j)/(double)N);
		verLi[j+3*N][1] = y_Li+l1_Li;
		verLi[j+3*N][2] = z_Li+(r1_Li-d_Li)*sin((2.0*PI*(double)j)/(double)N);
	}
	int LiGlass1[N+1][4];
	for(int j=0;j<N-1;++j){
		LiGlass1[j][0] = j    ;
		LiGlass1[j][1] = j  +1;
		LiGlass1[j][2] = j+N+1;
		LiGlass1[j][3] = j+N  ;
	}
	LiGlass1[N-1][0] =   N-1;
	LiGlass1[N-1][1] =     0;
	LiGlass1[N-1][2] =     N;
	LiGlass1[N-1][3] = 2*N-1;
	int LiGlass2[N+1][4];
	for(int j=0;j<N-1;++j){
		LiGlass2[j][0] = j+2*N  ;
		LiGlass2[j][1] = j+3*N  ;
		LiGlass2[j][2] = j+3*N+1;
		LiGlass2[j][3] = j+2*N+1;
	}
	LiGlass2[N-1][0] = 3*N-1;
	LiGlass2[N-1][1] = 4*N-1;
	LiGlass2[N-1][2] = 3*N+1;
	LiGlass2[N-1][3] = 2*N+1;
	int LiGlass3[N+1][4];
	for(int j=0;j<N-1;++j){
		LiGlass3[j][0] = j+N  ;
		LiGlass3[j][1] = j+N+1;
		LiGlass3[j][2] = j+3*N+1;
		LiGlass3[j][3] = j+3*N  ;
	}
	LiGlass3[N-1][0] = 2*N-1;
	LiGlass3[N-1][1] =   N+1;
	LiGlass3[N-1][2] = 3*N+1;
	LiGlass3[N-1][3] = 4*N-1;
	double phi = atan((r2_Li-r1_Li)/l1_Li);
	GLdouble normLi1[N+1][3];
	for(int j=0;j<N;++j){
		normLi1[j][0] = cos(phi)*cos((2.0*PI*(double)j)/(double)N);
		normLi1[j][1] = sin(phi);
		normLi1[j][2] = cos(phi)*sin((2.0*PI*(double)j)/(double)N);
	}
	GLdouble normLi2[N+1][3];
	for(int j=0;j<N;++j){
		normLi2[j][0] =-cos(phi)*cos((2.0*PI*(double)j)/(double)N);
		normLi2[j][1] =-sin(phi);
		normLi2[j][2] =-cos(phi)*sin((2.0*PI*(double)j)/(double)N);
	}
	if(PFLAG != true){
		glBegin(GL_LINES);
		for(int i=0;i<N-1;++i){
			glVertex3dv(verLi[i  ]);
			glVertex3dv(verLi[i+1]);
			glVertex3dv(verLi[i+N  ]);
			glVertex3dv(verLi[i+N+1]);
			glVertex3dv(verLi[i+2*N  ]);
			glVertex3dv(verLi[i+2*N+1]);
			glVertex3dv(verLi[i+3*N  ]);
			glVertex3dv(verLi[i+3*N+1]);
		}
		int n = 4;
		for(int i=0;i<n;++i){
			glVertex3dv(verLi[(int)N/n*i]);
			glVertex3dv(verLi[(int)N/n*i+N]);
			glVertex3dv(verLi[(int)N/n*i+2*N+(int)N/(2*n)]);
			glVertex3dv(verLi[(int)N/n*i+3*N+(int)N/(2*n)]);
		}
		glEnd();
	}
	else{
		glBegin(GL_QUADS);
		for(int j=0;j<N;++j){
			glNormal3dv(normLi1[j]);
			for(int i=0;i<4;++i){
				glVertex3dv(verLi[LiGlass1[j][i]]);
			}
			glNormal3dv(normLi2[j]);
			for(int i=0;i<4;++i){
				glVertex3dv(verLi[LiGlass2[j][i]]);
			}
			glNormal3d(0,0,1);
			for(int i=0;i<4;++i){
				glVertex3dv(verLi[LiGlass3[j][i]]);
			}
		}
		glEnd();
	}
	/* Li */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialColor(ms_Li);
	glRotated( 90,0,0,1);
	glCylinder(R_Li,L_Li,y_Li-L_Li/2,x_Li,z_Li);
	glRotated(-90,0,0,1);
	glDisable(GL_BLEND);
	/* PMT */
	glMaterialColor(ms_PMT);
	glRotated( 90,0,0,1);
	glCylinder(r4_Li,l4_Li,y_Li-L_Li-l4_Li/2,x_Li,z_Li);
	glRotated(-90,0,0,1);
}

/*****************************/

#endif //"Object_h_"
