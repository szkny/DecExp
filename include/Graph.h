#ifndef Graph_h_
#define Graph_h_
/**************************************************************/

#include<GL/glut.h>
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
		void Canvas1D(const char *title);
};


Graph1D::Graph1D(double XMIN, double XMAX){
	/* margin of canvas (%) */
	margin = 8.0;
	/* X-coordinate */
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


void Graph1D::Canvas1D(const char *title){
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
	glDrawString("nsec",(xmax/2+xoff)/wx,(Ytxt*8+yoff)/wy);
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
		int Nx,Ny;
		int MAXhist;
		int Numhist;
		int ID;
		double Color[3][3];
	public:
		Graph2D(double XMIN, double XMAX, double YMIN, double YMAX);
		void EntryHist2D(int N);
		void DrawHist2D(int *hist, double bin1, int ARRAY1, double bin2, int ARRAY2);
		void Canvas2D(const char *title);
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
	xoff  = wx*margin/100;
	wy    = (ymax-ymin)*(100+margin*2)/100;
	yoff  = wy*margin/100;
	/* initialize parameters */
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


void Graph2D::DrawHist2D(int *hist, double bin1, int ARRAY1, double bin2, int ARRAY2){
	glPushMatrix();
	glTranslated(0.02,0.0,0.0);
	int Cid = (Numhist-ID-1)%3;
	glColor3d(Color[Cid][0],Color[Cid][1],Color[Cid][2]);
	glBegin(GL_QUADS);
	for(int ch1=0;ch1<ARRAY1;++ch1){
		if(xmin<=ch1*bin1&&(ch1+1)*bin1<=xmax){
			for(int ch2=0;ch2<ARRAY2;++ch2){
				if(ymin<=ch2*bin2&&(ch2+1)*bin2<=ymax){
					if(0<hist[ch1*ARRAY2+ch2]){
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
	++ID;
	glPopMatrix();
}


void Graph2D::Canvas2D(const char *title){
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
		sprintf(s,"%3.1f",(double)i*ygrid);
		glDrawString(s,(Xtxt*8+xoff)/wx,(i*ygrid+Ytxt+yoff)/wy);
	}
	glDrawString("nsec",(xmax/2+xoff)/wx,(Ytxt*8+yoff)/wy);
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
#endif //Graph_h_
