#ifndef Graph_h_
#define Graph_h_
/**************************************************************/

#include<glut.h>
#include"MyGLUT.h"


class Graph1D{
	private:
		double margin;
		double xmin,xmax,xgrid;
		double ymin,ymax,ygrid;
		double wx,wy,xoff,yoff;
		int Nx,Ny;
		int MAXhist;
		int Numhist;
		int ID;
		double Color[3][3];
	public:
		Graph1D(double XMIN, double XMAX);
		void EntryHist1D(int *hist, int ARRAY);
		void DrawHist1D(int *hist, double bin, int ARRAY);
		void DrawLineHist1D(int *hist,double bin, int ARRAY);
		void Canvas1D(const char *title, const char *xlabel);
};


Graph1D::Graph1D(double XMIN, double XMAX){
	/* margin of canvas (%) */
	margin = 8.0;
	xmin  = XMIN;
	xmax  = XMAX*1.01;
	xgrid = (xmax-xmin)/5/1.01;
	Nx    = (xmax-xmin)/xgrid;
	/* draw area */
	wx    = (xmax-xmin)*(100+margin*2)/100;
	xoff  = wx*margin/100;
	/* initialize each parameters */
	MAXhist = 0;
	Numhist = 0;
	ID = 0;
	/* histgram color ([Cid][RGB]) */
	Color[0][0] = 0.00;
	Color[0][1] = 0.65;
	Color[0][2] = 0.80;
	Color[1][0] = 0.80;
	Color[1][1] = 0.00;
	Color[1][2] = 0.00;
	Color[2][0] = 0.20;
	Color[2][1] = 0.80;
	Color[2][2] = 0.20;
}


void Graph1D::EntryHist1D(int *hist, int ARRAY){
	for(int ch=0;ch<ARRAY;++ch){
		if(MAXhist<hist[ch]) MAXhist = hist[ch];
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
	/* Y-coordinate */
	ymin  = 0.0;
	ymax  = scale*5;
	ygrid = scale;
	Ny    = (ymax-ymin)/ygrid;
	/* draw area */
	wy    = (ymax-ymin)*(100+margin*2)/100;
	yoff  = wy*margin/100;
	/* add number of histgram */
	++Numhist;
}


void Graph1D::DrawHist1D(int *hist, double bin, int ARRAY){
	glPushMatrix();
	glTranslated(0.02,0.0,0.0);
	/* set color */
	int Cid = (Numhist-ID-1)%3;
	glColor3d(Color[Cid][0],Color[Cid][1],Color[Cid][2]);
	/* draw histogram */
	glBegin(GL_QUADS);
	for(int ch=0; ch<ARRAY; ++ch){
		if(xmin<=ch*bin&&(ch+1)*bin<=xmax){
			glVertex2d((ch*bin+xoff)/wx,yoff/wy);
			glVertex2d((ch*bin+xoff)/wx,(hist[ch]+yoff)/wy);
			glVertex2d(((ch+1)*bin+xoff)/wx,(hist[ch]+yoff)/wy);
			glVertex2d(((ch+1)*bin+xoff)/wx,yoff/wy);
		}
	}
	glEnd();
	++ID;
	glPopMatrix();
}


void Graph1D::DrawLineHist1D(int *hist, double bin, int ARRAY){
	glPushMatrix();
	glTranslated(0.02,0.0,0.0);
	/* set color */
	int Cid = (Numhist-ID-1)%3;
	glColor3d(Color[Cid][0],Color[Cid][1],Color[Cid][2]);
	/* draw histgram */
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1.0, 0x5555);
	bool FLAG = false;
	glBegin(GL_LINE_STRIP);
	for(int ch=0;ch<ARRAY;++ch){
		if(xmin<=ch*bin&&(ch+1)*bin<=xmax){
			if(hist[ch]==0){
				FLAG = true;
				glVertex2d((ch*bin+xoff)/wx,(-ymin+yoff)/wy);
				glVertex2d(((ch+1)*bin+xoff)/wx,(-ymin+yoff)/wy);
			}
			else{
				if(FLAG){
					FLAG = false;
					glVertex2d((ch*bin+xoff)/wx,(-ymin+yoff)/wy);
					glVertex2d((ch*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
					glVertex2d(((ch+1)*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
				}
				else{
					glVertex2d((ch*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
					glVertex2d(((ch+1)*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
				}
			}
		}
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	++ID;
	glPopMatrix();
}


void Graph1D::Canvas1D(const char *title,const char *xlabel){
	glPushMatrix();
	glTranslated(0.02,0.0,0.0);
	/* title */
	glColor3d(0.0,0.0,0.0);
	int size = strlen(title);
	glDrawString(title,0.5-(double)size*0.01,0.95);
	/* text */
	glColor3d(0.0,0.0,0.0);
	double Xtxt = -(xmax-xmin)/100.0;
	double Ytxt = -(ymax-ymin)/100.0;
	glDrawString("0",(Xtxt*3+xoff)/wx,(Ytxt*4+yoff)/wy);	
	char s[10];
	for(int i=1; i<=Nx; ++i){
		sprintf(s,"%d",i*(int)xgrid);
		double size = (double)strlen(s);
		glDrawString(s,(i*xgrid+Xtxt*size+xoff)/wx,(Ytxt*4+yoff)/wy);
	}
	for(int i=1; i<=Ny; ++i){
		if(1000<=i*ygrid) sprintf(s,"%3.1fk",ymin+i*ygrid/1000.0);
		else sprintf(s,"%4d",(int)(ymin+i*ygrid));
		glDrawString(s,(Xtxt*10+xoff)/wx,(i*ygrid+Ytxt+yoff)/wy);
	}
	glDrawString(xlabel,(xmax/2+xoff)/wx,(Ytxt*8+yoff)/wy);
	/* coordinate line */
	glBegin(GL_LINE_LOOP);
	glVertex2d((xmin+xoff)/wx,(ymin+yoff)/wy);
	glVertex2d((xmax+xoff)/wx,(ymin+yoff)/wy);
	glVertex2d((xmax+xoff)/wx,(ymax+yoff)/wy);
	glVertex2d((xmin+xoff)/wx,(ymax+yoff)/wy);
	glEnd();
	/* grid line */
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1.0, 0x5555);
	glColor3d(0.5,0.5,0.5);
	glBegin(GL_LINES);
	for(int i=1; i<=Nx; ++i){
		glVertex2d((xgrid*i+xoff)/wx,(ymin+yoff)/wy);
		glVertex2d((xgrid*i+xoff)/wx,(ymax+yoff)/wy);
	}
	for(int i=1; i<=Ny; ++i){
		glVertex2d((xmin+xoff)/wx,(ygrid*i+yoff)/wy);
		glVertex2d((xmax+xoff)/wx,(ygrid*i+yoff)/wy);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glPopMatrix();
}


/**************************************************************/


class Graph2D{
	protected:
		double margin;
		double xmin,xmax,xgrid;
		double ymin,ymax,ygrid;
		double wx,wy,xoff,yoff;
		double hmax;
		int Nx,Ny;
		int MAXhist;
		int Numhist;
		int ID;
		double Color[3][3];
	public:
		Graph2D(double XMIN, double XMAX, double YMIN, double YMAX);
		void EntryHist2D(int N);
		void DrawHist2D(int *hist, double bin1, int ARRAY1, double bin2, int ARRAY2);
		void Canvas2D(const char *title, const char *xlabel);
};


Graph2D::Graph2D(double XMIN, double XMAX, double YMIN, double YMAX){
	/* margin of canvas (%) */
	margin = 8.0;
	/* X-coordinate */
	xmin  = XMIN;
	xmax  = XMAX*1.01;
	xgrid = (xmax-xmin)/5/1.01;
	Nx    = (xmax-xmin)/xgrid;
	/* Y-coordinate */
	ymin  = YMIN;
	ymax  = YMAX*1.01;
	ygrid = (ymax-ymin)/5/1.01;
	Ny    = (ymax-ymin)/ygrid;
	/* draw area */
	wx    = (xmax-xmin)*(100+margin*2)/100;
	xoff  = wx*(margin-2)/100;
	wy    = (ymax-ymin)*(100+margin*2)/100;
	yoff  = wy*margin/100;
	/* initialize parameters */
	hmax = 10.0;
	Numhist = 0;
	ID = 0;
	/* histgram color ([Cid][RGB]) */
	Color[0][0] = 0.00;
	Color[0][1] = 0.65;
	Color[0][2] = 0.80;
	Color[1][0] = 0.80;
	Color[1][1] = 0.00;
	Color[1][2] = 0.00;
	Color[2][0] = 0.20;
	Color[2][1] = 0.80;
	Color[2][2] = 0.20;
}


void Graph2D::EntryHist2D(int N){
	/* add number of histgram */
	Numhist = N;
}


#define HUE(off,c){\
	double coef = c;\
	if(coef<0)   coef = 0.0;\
	if(360<coef) coef = 359.9;\
	double hue = -h/hmax*coef+off;\
	while(hue<0) hue += 360.0;\
	double R = 1.0;\
	double G = 1.0;\
	double B = 1.0;\
	if(0<=hue&&hue<60){\
		R = 1.0;\
		G = hue/60.0;\
		B = 0.0;\
	}\
	else if(60<=hue&&hue<120){\
		R = (120-hue)/60;\
		G = 1.0;\
		B = 0.0;\
	}\
	else if(120<=hue&&hue<180){\
		R = 0.0;\
		G = 1.0;\
		B = (hue-120)/60;\
	}\
	else if(180<=hue&&hue<240){\
		R = 0.0;\
		G = (240-hue)/60;\
		B = 1.0;\
	}\
	else if(240<=hue&&hue<300){\
		R = (hue-240)/60;\
		G = 0.0;\
		B = 1.0;\
	}\
	else if(300<=hue&&hue<360){\
		R = 1.0;\
		G = 0.0;\
		B = (360-hue)/60;\
	}\
	glColor3d(R,G,B);\
}
void Graph2D::DrawHist2D(int *hist, double bin1, int ARRAY1, double bin2, int ARRAY2){
	for(int ch1=0;ch1<ARRAY1;++ch1)
		for(int ch2=0;ch2<ARRAY2;++ch2)
			if(hmax<hist[ch1*ARRAY2+ch2])
				hmax = hist[ch1*ARRAY2+ch2];
	int base  = 10;
	int digit = 0;
	int temp  = 1;
	while(temp*5*pow(base,digit)<hmax){
		++temp;
		if(temp==9){
			temp = 1;
			++digit;
		}
	}
	hmax = (double)temp*pow(base,digit)*5.0;

	glPushMatrix();
	glTranslated(0.02,0.0,0.0);
	//int Cid = abs(Numhist-ID-1)%3;
	glBegin(GL_QUADS);
	for(int ch1=0;ch1<ARRAY1;++ch1){
		if(xmin<=ch1*bin1&&(ch1+1)*bin1<=xmax){
			for(int ch2=0;ch2<ARRAY2;++ch2){
				if(ymin<=ch2*bin2&&(ch2+1)*bin2<=ymax){
					double h = hist[ch1*ARRAY2+ch2];
					if(0<h){
						HUE(220,220);
						glVertex2d((ch1*bin1+xoff)/wx,(ch2*bin2+yoff)/wy);
						glVertex2d(((ch1+1)*bin1+xoff)/wx,(ch2*bin2+yoff)/wy);
						glVertex2d(((ch1+1)*bin1+xoff)/wx,((ch2+1)*bin2+yoff)/wy);
						glVertex2d((ch1*bin1+xoff)/wx,((ch2+1)*bin2+yoff)/wy);
					}
				}
			}
		}
	}
	glEnd();

	/* Color Bar */
	double hbin = hmax/100.0;
	for(double h=0.0;h<hmax;h+=hbin){
		HUE(220,220);
		glBegin(GL_QUADS);
		glVertex2d(0.93,(h*ymax/hmax+yoff)/wy);
		glVertex2d(0.97,(h*ymax/hmax+yoff)/wy);
		glVertex2d(0.97,((h+hbin)*ymax/hmax+yoff)/wy);
		glVertex2d(0.93,((h+hbin)*ymax/hmax+yoff)/wy);
		glEnd();
	}
	++ID;
	glPopMatrix();
}
#undef HUE


void Graph2D::Canvas2D(const char *title, const char *xlabel){
	glPushMatrix();
	glTranslated(0.02,0.0,0.0);
	/* title */
	glColor3d(0.0,0.0,0.0);
	int size = strlen(title);
	glDrawString(title,0.5-(double)size*0.01,0.95);
	/* text */
	double Xtxt = -(xmax-xmin)/100.0;
	double Ytxt = -(ymax-ymin)/100.0;
	glDrawString("0",(Xtxt*3+xoff)/wx,(Ytxt*4+yoff)/wy);	
	char s[10];
	for(int i=1; i<=Nx; ++i){
		sprintf(s,"%d",i*(int)xgrid);
		double size = (double)strlen(s);
		glDrawString(s,(i*xgrid+Xtxt*size+xoff)/wx,(Ytxt*4+yoff)/wy);
	}
	for(int i=1; i<=Ny; ++i){
		if(Ny*ygrid<10) sprintf(s,"%3.1f",(double)i*ygrid);
		else sprintf(s,"%3.0f",(double)i*ygrid);
		glDrawString(s,(Xtxt*8+xoff)/wx,(i*ygrid+Ytxt+yoff)/wy);
		sprintf(s,"%d",(int)(i*hmax/Ny));
		double size = (double)strlen(s);
		glDrawString2(s,0.94-(size/2)/wx,(i*ygrid+Ytxt+yoff)/wy);
	}
	glDrawString(xlabel,(xmax/2+xoff)/wx,(Ytxt*8+yoff)/wy);
	/* coordinate line */
	glBegin(GL_LINE_LOOP);
	glVertex2d((xmin+xoff)/wx,(ymin+yoff)/wy);
	glVertex2d((xmax+xoff)/wx,(ymin+yoff)/wy);
	glVertex2d((xmax+xoff)/wx,(ymax+yoff)/wy);
	glVertex2d((xmin+xoff)/wx,(ymax+yoff)/wy);
	glEnd();
	/* grid line */
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1.0, 0x5555);
	glColor3d(0.5,0.5,0.5);
	glBegin(GL_LINES);
	for(int i=1; i<=Nx; ++i){
		glVertex2d((xgrid*i+xoff)/wx,(ymin+yoff)/wy);
		glVertex2d((xgrid*i+xoff)/wx,(ymax+yoff)/wy);
	}
	for(int i=1; i<=Ny; ++i){
		glVertex2d((xmin+xoff)/wx,(ygrid*i+yoff)/wy);
		glVertex2d((xmax+xoff)/wx,(ygrid*i+yoff)/wy);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glPopMatrix();
}


/**************************************************************/


//class Graph3D:public Graph2D{
//	private:
//		double zmin,zmax,zgrid;
//		double wz,zoff;
//		int Nz;
//	public:
//		Graph3D(double XMIN, double XMAX, double YMIN, double YMAX):Graph2D(XMIN,XMAX,YMIN,YMAX){};
//		void EntryHist3D(int *hist, int ARRAY1, int ARRAY2);
//		void DrawHist3D();
//		void DrawHist3D(int *hist, double bin1, int ARRAY1, double bin2, int ARRAY2);
//		void Canvas3D(const char *title);
//};
//
//
////Graph3D::Graph3D(double XMIN, double XMAX, double YMIN, double YMAX):Graph2D(XMIN,XMAX,YMIN,YMAX){
////	/* margin of canvas (%) */
////	margin = 8.0;
////	/* X-coordinate */
////	xmin  = XMIN;
////	xmax  = XMAX*1.01;
////	xgrid = (xmax-xmin)/5/1.01;
////	Nx    = (xmax-xmin)/xgrid;
////	/* Y-coordinate */
////	ymin  = YMIN;
////	ymax  = YMAX*1.01;
////	ygrid = (ymax-ymin)/5/1.01;
////	Ny    = (ymax-ymin)/ygrid;
////	/* draw area */
////	wx    = (xmax-xmin)*(100+margin*2)/100;
////	xoff  = wx*margin/100;
////	wy    = (ymax-ymin)*(100+margin*2)/100;
////	yoff  = wy*margin/100;
////	/* initialize parameters */
////	Numhist = 0;
////	ID = 0;
////	/* histgram color ([Cid][RGB]) */
////	Color[0][0] = 0.00;
////	Color[0][1] = 0.65;
////	Color[0][2] = 0.80;
////	Color[1][0] = 0.80;
////	Color[1][1] = 0.00;
////	Color[1][2] = 0.00;
////	Color[2][0] = 0.20;
////	Color[2][1] = 0.80;
////	Color[2][2] = 0.20;
////}
//
//
//void Graph3D::EntryHist3D(int *hist, int ARRAY1, int ARRAY2){
//	for(int ch1=0;ch1<ARRAY1;++ch1){
//		for(int ch2=0;ch2<ARRAY2;++ch2){
//			if(MAXhist<hist[ch1*ARRAY2+ch2]) MAXhist = hist[ch1*ARRAY2+ch2];
//		}
//	}
//	/* setting drawing scale */
//	int base  = 10;
//	int digit = 0;
//	int temp  = 1;
//	while(temp*5*pow(base,digit)<MAXhist){
//		++temp;
//		if(temp==9){
//			temp = 1;
//			++digit;
//		}
//	}
//	double scale = (double)temp*pow(base,digit);
//	/* Z-coordinate */
//	zmin  = 0.0;
//	zmax  = scale*5;
//	zgrid = scale;
//	Nz    = (zmax-zmin)/zgrid;
//	/* draw area */
//	wz    = (zmax-zmin)*(100+margin*2)/100;
//	zoff  = wz*margin/100;
//	/* add number of histgram */
//	++Numhist;
//}
//
//
//void Graph3D::DrawHist3D(int *hist, double bin1, int ARRAY1, double bin2, int ARRAY2){
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
//	glEnable(GL_LIGHT2);
//	static GLfloat LightClr0[] = { 0.2, 0.2, 0.2, 1.0 };
//	static GLfloat LightClr1[] = { 0.7, 0.7, 0.7, 1.0 };
//	static GLfloat LightClr2[] = { 1.0, 1.0, 1.0, 1.0 };
//	static GLfloat LightPos[]  = { 100.0, 100.0, 100.0, 1.0 };
//	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
//	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightClr0);
//	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
//	glLightfv(GL_LIGHT1, GL_DIFFUSE,  LightClr1);
//	glLightfv(GL_LIGHT2, GL_POSITION, LightPos);
//	glLightfv(GL_LIGHT2, GL_SPECULAR, LightClr2);
//	
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//		
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ms_ruby.specular);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,ms_ruby.shininess);	
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  ms_ruby.diffuse);	
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ms_ruby.ambient);	
//
//	glPushMatrix();
//	glRotated(-20,1,0,0);
//	
//	for(int ch1=0;ch1<ARRAY1;++ch1){
//		if(xmin<=ch1*bin1&&(ch1+1)*bin1<=xmax){
//			for(int ch2=0;ch2<ARRAY2;++ch2){
//				if(ymin<=ch2*bin2&&(ch2+1)*bin2<=ymax){
//					if(0<hist[ch1*ARRAY2+ch2]){
//						glRectangular(bin2,hist[ch1*ARRAY2+ch2],bin2,(ch1*bin1+xoff)/wx-0.5,hist[ch1*ARRAY2+ch2]/wz,(ch2*bin2+yoff)/wy-0.5);
//					}
//				}
//			}
//		}
//	}
//
//	glPopMatrix();
//
//	glPopMatrix();
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//
//	glDisable(GL_LIGHTING);	
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
//}
//


/**************************************************************/
#endif //Graph_h_
