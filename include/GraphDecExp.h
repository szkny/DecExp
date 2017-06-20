#ifndef GraphDecExp_h_
#define GraphDecExp_h_
/************************************/
#include"DecExp.h"
#include"Graph.h"

bool HFLAG  = false; /* Stock Histogram */
bool OFLAG  = false; /* Oscillo Mode */
bool LFLAG  = true;  /* Log Scale for PMT Histogram */
bool DFLAG  = false; /* TOF vs. En 2D Histogram */
bool PosFLAG= false; /* Position Histogram */
int  OMODE  = 0; /* Oscillo Mode (0:reach 1:signal 2:hist) */
int  PMODE  = 0; /* Particle Mode for 2D (0:Neutron 1:Proton) */


/* Draw Histgram */
void glGraph(void){

	/* canvas color setting */
	if(AMODE==2 && OMODE==1) glClearColor(0.1,0.1,0.1,1.0);
	else glClearColor(1.0,1.0,1.0,1.0);

	/* Window Margin (%) */
	double MARGIN = 8.0;

	/* TUNA(&MAGRO) TOF Spector */
	if(AMODE==0){
		int TUNAtotal  = 0;
		int MAGROtotal = 0;
		for(int ch1=0;ch1<ARRAY1;++ch1){
			for(int ch2=0;ch2<EnARRAY;++ch2){
				TUNAtotal  += TH2D_TUNA_n[ch1*EnARRAY+ch2];
				MAGROtotal += TH2D_MAGRO_n[ch1*EnARRAY+ch2];
			}
		}
		/* draw efficiency */
		if(SFLAG){
			/* TUNA */
			glColor3d(0.0,0.0,0.0);
			char txt[50];
			double Eint;
			double Eamb;
			Eint = (TUNAincident==0)?  0:(double)TUNAtotal/TUNAincident*100;
			Eamb = (double)TUNAtotal/nCounter*100;
			sprintf(txt,"Eamb%5.2f%%, Eint%5.1f%%",Eamb,Eint);
			glDrawString(txt,0.50,0.90);
			/* MAGRO */
			glColor3d(0.9,0.0,0.0);
			Eint = (MAGROincident==0)? 0:(double)MAGROtotal/MAGROincident*100;
			Eamb = (double)MAGROtotal/nCounter*100;
			sprintf(txt,"Eamb%5.2f%%, Eint%5.1f%%",Eamb,Eint);
			glDrawString(txt,0.50,0.85);
		}

		if(!DFLAG){
			if(!PosFLAG){
				int TOFtuna[ARRAY1] = {0};
				int TOFmagro[ARRAY1] = {0};
				for(int ch1=0;ch1<ARRAY1;++ch1){
					for(int ch2=0;ch2<EnARRAY;++ch2){
						TOFtuna[ch1] += TH2D_TUNA_n[ch1*EnARRAY+ch2];
						TOFmagro[ch1] += TH2D_MAGRO_n[ch1*EnARRAY+ch2];
					}
				}
				/* Draw Histgram */
				Graph1D *hst = new Graph1D(0.0,250.0);
				hst->EntryHist1D(TOFtuna,ARRAY1);
				if(SFLAG){
					hst->EntryHist1D(TOFmagro,ARRAY1);
					hst->DrawLineHist1D(TOFmagro,bin1,ARRAY1);
				}
				hst->DrawHist1D(TOFtuna,bin1,ARRAY1);
				hst->Canvas1D("TOF Spector [counts/0.5nsec]","nsec");
				delete hst;
			}
			else{
				int TUNA_HitPosX[XARRAY] = {0};
				for(int chx=0;chx<XARRAY;++chx){
					for(int chy=0;chy<YARRAY;++chy){
						TUNA_HitPosX[chx] += TH2D_TUNA_HitPos[chx*YARRAY+chy];
					}
				}
				/* Draw Histgram */
				Graph1D *hst = new Graph1D(0.0,150.0);
				hst->EntryHist1D(TUNA_HitPosX,XARRAY);
				hst->DrawHist1D(TUNA_HitPosX,dPos,XARRAY);
				char title[64];
				sprintf(title,"Hit Position x [counts/%3.1fcm]",dPos);
				hst->Canvas1D(title,"cm");
				delete hst;
			}
		}

		/* TOF vs. Ene 2D Histgram */
		else{ // DFLAG == true
			if(!PosFLAG){
				Graph2D *hst = new Graph2D(0.0,250.0,0.0,maxEn);
				if(SFLAG) hst->EntryHist2D(2);
				else hst->EntryHist2D(1);
				if(PMODE==0){
					if(SFLAG) hst->DrawHist2D(TH2D_MAGRO_n,bin1,ARRAY1,dEn,EnARRAY);
					hst->DrawHist2D(TH2D_TUNA_n,bin1,ARRAY1,dEn,EnARRAY);
					hst->Canvas2D("TOF vs. Neutron Energy","nsec");
				}
				else if(PMODE==1){
					if(SFLAG) hst->DrawHist2D(TH2D_MAGRO_p,bin1,ARRAY1,dEn,EnARRAY);
					hst->DrawHist2D(TH2D_TUNA_p,bin1,ARRAY1,dEn,EnARRAY);
					hst->Canvas2D("TOF vs.  Proton Energy","nsec");
				}
				delete hst;
			}
			else{
				Graph2D *hst = new Graph2D(0.0,250.0,0.0,150);
				hst->EntryHist2D(1);
				hst->DrawHist2D(TH2D_TUNA_TOF_Pos,bin1,ARRAY1,dPos,XARRAY);
				hst->Canvas2D("TOF vs. Hit Position","nsec");
				delete hst;
			}
		}
	}


	/* LiGlass TOF Spector */
	if(AMODE==1){
		Graph1D *hst = new Graph1D(0.0,30.0);
		hst->EntryHist1D(hist_Li,ARRAY2);
		hst->DrawHist1D(hist_Li,bin2,ARRAY2);
		hst->Canvas1D("Li TOF Spector [counts/0.1nsec]","nsec");
		delete hst;
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


/************************************/
#endif //"Graph_h_"
