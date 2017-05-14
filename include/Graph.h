#ifndef Graph_h_
#define Graph_h_
/*********************************/
#include<GL/glut.h>
#include"MyGLUT.h"


class Graph{
	private:
		double margin;
		double xmin,xmax,xgrid;
		double ymin,ymax,ygrid;
		double wx,wy,xoff,yoff;
		int Nx,Ny;
		int MAXhist;
		int Numhist;
	public:
		Graph(const char *title, double XMIN, double XMAX);
		void EntryHist(int *hist, int ARRAY);
		void DrawHist1D(int *hist, double bin, int ARRAY, double R, double G, double B);
		void Canvas1D();
};


Graph::Graph(const char *title, double XMIN, double XMAX){
	/* Margin of Canvas (%) */
	margin = 8.0;
	/* title */
	glColor3d(0.0,0.0,0.0);
	int size = strlen(title);
	glDrawString(title,0.5-(double)size*0.01,0.95);
	/* X-Coordinate */
	xmin  = XMIN;
	xmax  = XMAX*1.01;
	xgrid = (xmax-xmin)/5/1.01;
	Nx    = (xmax-xmin)/xgrid;
	MAXhist = 0;
	Numhist = 0;
}


void Graph::EntryHist(int *hist, int ARRAY){
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
	/* Y-Coordinate */
	ymin  = 0.0;
	ymax  = scale*5;
	ygrid = scale;
	Ny    = (ymax-ymin)/ygrid;
	/* Draw Area */
	wx    = (xmax-xmin)*(100+margin*2)/100;
	wy    = (ymax-ymin)*(100+margin*2)/100;
	xoff  = wx*margin/100;
	yoff  = wy*margin/100;
}


void Graph::DrawHist1D(int *hist, double bin, int ARRAY, double R, double G, double B){
	/* draw histogram */
	glColor3d(R,G,B);
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
}


void Graph::Canvas1D(){
	glPushMatrix();
	glTranslated(0.02,0.0,0.0);
	/* text */
	glColor3d(0.0,0.0,0.0);
	double Xtxt = -(xmax-xmin)/100.0;
	double Ytxt = -(ymax-ymin)/100.0;
	glDrawString("0",(Xtxt*3+xoff)/wx,(Ytxt*4+yoff)/wy);	
	char s[10];
	for(int i=1; i<=Nx; ++i){
		sprintf(s,"%d",i*(int)xgrid);
		int size = strlen(s);
		if(size == 1) glDrawString(s,(i*xgrid+Xtxt*1+xoff)/wx,(Ytxt*4+yoff)/wy);
		if(size == 2) glDrawString(s,(i*xgrid+Xtxt*2+xoff)/wx,(Ytxt*4+yoff)/wy);
		if(size == 3) glDrawString(s,(i*xgrid+Xtxt*3+xoff)/wx,(Ytxt*4+yoff)/wy);
	}
	for(int i=1; i<=Ny; ++i){
		if(1000<=i*ygrid) sprintf(s,"%3.1fk",ymin+i*ygrid/1000.0);
		else sprintf(s,"%4d",(int)(ymin+i*ygrid));
		glDrawString(s,(Xtxt*11+xoff)/wx,(i*ygrid+Ytxt+yoff)/wy);
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



//void Graph::DrawHist1D2(int *hist,double bin, int ARRAY){
//	glColor3d(0.9,0.0,0.0);
//	glEnable(GL_LINE_STIPPLE);
//	glLineStipple(1.0, 0x5555);
//	bool FLAG = false;
//	glBegin(GL_LINE_STRIP);
//	for(int ch=0;ch<ARRAY;++ch){
//		if(xmin<=ch*bin&&(ch+1)*bin<=xmax){
//			if(hist[ch]==0){
//				FLAG = true;
//				glVertex2d((ch*bin+xoff)/wx,(-ymin+yoff)/wy);
//				glVertex2d(((ch+1)*bin+xoff)/wx,(-ymin+yoff)/wy);
//			}
//			else{
//				if(FLAG){
//					FLAG = false;
//					glVertex2d((ch*bin+xoff)/wx,(-ymin+yoff)/wy);
//					glVertex2d((ch*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
//					glVertex2d(((ch+1)*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
//				}
//				else{
//					glVertex2d((ch*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
//					glVertex2d(((ch+1)*bin+xoff)/wx,(-ymin+hist[ch]+yoff)/wy);
//				}
//			}
//		}
//	}
//	glEnd();
//	glDisable(GL_LINE_STIPPLE);
//}


/*********************************/
#endif //Graph_h_
