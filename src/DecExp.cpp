/* DecExp.cpp
*
*	Simulation of Experiment at RCNP, December 2016
*	(for GLUT and OpenGL test program)
*		(c) S.Suzuki 2017.1.23
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

#include<GL/glut.h>
#include<DecExp.h>

/* Number of Windows */
const int WinNum = 2;
int WindowID[WinNum];

/* Function Prototype Declaration */
void WindowAnime(void);
void WindowGraph(void);
void PopUpMenu(void);
void Controler(void);
void init1(void);
void init2(void);
void idle(void);
void display1(void);
void display2(void);
void resize1(int w, int h);
void resize2(int w, int h);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void menu(int val);
void keyboard(unsigned char key, int x, int y);
void keyboard_sp(int key, int x, int y);
void SettingTUNA(void);
void SettingLiGlass(void);
void SettingPhoton(void);


/* main function */
int main(int argc, char *argv[]){
	printf("\n\033[30m"
			" \033[47m+———————————————————————[ USAGE ]——————————————————————+\033[49m\n"
			" \033[47m| mouse     … rotate/tranlate(drag) popup(right-click) |\033[49m\n"
			" \033[47m| arrow key … zoom/unzoom(↑,↓) animation mode(←,→)     |\033[49m\n"
			" \033[47m| space key … play/stop animation                      |\033[49m\n"
			" \033[47m| m         … change mouse mode                        |\033[49m\n"
			" \033[47m| r         … reset animation                          |\033[49m\n"
			" \033[47m| _         … reset angle                              |\033[49m\n"
			" \033[47m| p         … polygon mode                             |\033[49m\n"
			" \033[47m| s         … status on/off                            |\033[49m\n"
			" \033[47m| h         … stock histgram for photon animation      |\033[49m\n"
			" \033[47m| t         … set threshold for photon animation       |\033[49m\n"
			" \033[47m| n         … noise on/off for photon animation        |\033[49m\n"
			" \033[47m| q (esc)   … quit                                     |\033[49m\n"
			" \033[47m+——————————————————————————————————————————————————————+\033[49m\n"
			"\033[39m");

	srand((unsigned) time(NULL));
	glutInit(&argc, argv);

	/* histogram window */
	WindowGraph();
	PopUpMenu();
	Controler();
	/* animation window */
	WindowAnime();
	PopUpMenu();
	Controler();

	glutMainLoop();
	return 0;
}


/******** Functions *********/

void WindowAnime(void){
	glutInitWindowPosition(900,0);
	//glutInitWindowSize(700,500);
	glutInitWindowSize(600,450);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	WindowID[1] = glutCreateWindow("Animation");
	glutDisplayFunc(display1);
	glutReshapeFunc(resize1);
	init1();
}


void WindowGraph(void){
	glutInitWindowPosition(350,0);
	glutInitWindowSize(400,400);
	glutInitDisplayMode(GLUT_RGBA);
	WindowID[0] = glutCreateWindow("Graph");
	glutDisplayFunc(display2);
	glutReshapeFunc(resize2);
	init2();
}


void PopUpMenu(void){
	/* sub menu */
	int MenuID[2];
	MenuID[0] = glutCreateMenu(menu);
	glutAddMenuEntry("Neutron (TUNA)",13);
	glutAddMenuEntry("Neutron (Li Glass)",14);
	glutAddMenuEntry("Scintillation Photon",15);
	glutAddMenuEntry("Oscillo Scope",22);
	glutAddMenuEntry("PMT Histgram",23);
	MenuID[1] = glutCreateMenu(menu);
	glutAddMenuEntry("Create Stock",18);
	glutAddMenuEntry("Delete Stock",19);
	/* main menu */
	glutCreateMenu(menu);
	glutAddMenuEntry("Play/Stop",16);
	glutAddSubMenu("Animation Mode",MenuID[0]);
	glutAddMenuEntry("Reset Animation",17);
	glutAddMenuEntry("Display Mode",11);
	glutAddMenuEntry("Mouse Mode",10);
	glutAddMenuEntry("Status On/Off",12);
	glutAddSubMenu("Stock Histgram",MenuID[1]);
	glutAddMenuEntry("Set Threshold",20);
	glutAddMenuEntry("Set Noise",21);
	glutAddMenuEntry("Log Scale",24);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void Controler(void){
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_sp);
}


void init1(void){
	glClearColor( 0.8, 0.8, 0.8, 1.0 );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);
	glEnable(GL_LIGHT7);

	glNeutInit();
	glPhotInit();
}


void init2(void){
	if(AMODE==0||AMODE==1) glClearColor(1.0,1.0,1.0,1.0);
	if(AMODE==2) glClearColor(0.1,0.1,0.1,1.0);
}


void idle(void){
	for(int i=0; i<WinNum; ++i){
		glutSetWindow(WindowID[i]);
		if(i==0){
			if(AMODE==2) glutReshapeWindow(400,800);
			else         glutReshapeWindow(400,400);
		}
		glutPostRedisplay();
	}
}


void display1(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	/* Light Color */
	GLfloat LightClr0[]  = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat LightClr1[]  = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat LightClr2[]  = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat LightPos0[]  = {   0.0, 500.0,-150.0, 1.0 };
	static GLfloat LightPos1[]  = { 500.0, 500.0, 350.0, 1.0 };
	static GLfloat LightPos2[]  = { 500.0, 500.0,-650.0, 1.0 };
	static GLfloat LightPos3[]  = {-500.0, 500.0, 350.0, 1.0 };
	static GLfloat LightPos4[]  = {-500.0, 500.0,-650.0, 1.0 };
	static bool lflag = true;
	float lspeed = 5.0;
	if(LightPos1[1]<200) lflag = false;
	if(800<LightPos1[1]) lflag = true;
	if(lflag){
		LightPos1[1] -= lspeed;
		LightPos2[1] -= lspeed;
		LightPos3[1] -= lspeed;
		LightPos4[1] -= lspeed;
	}
	else{
		LightPos1[1] += lspeed;
		LightPos2[1] += lspeed;
		LightPos3[1] += lspeed;
		LightPos4[1] += lspeed;
	}
	/* Ambient Light */
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos0);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightClr0);
	/* Diffused Light */
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  LightClr1);
	glLightfv(GL_LIGHT2, GL_POSITION, LightPos1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  LightClr1);
	glLightfv(GL_LIGHT3, GL_POSITION, LightPos2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE,  LightClr1);
	glLightfv(GL_LIGHT4, GL_POSITION, LightPos3);
	glLightfv(GL_LIGHT4, GL_DIFFUSE,  LightClr1);
	glLightfv(GL_LIGHT7, GL_POSITION, LightPos4);
	glLightfv(GL_LIGHT7, GL_DIFFUSE,  LightClr1);
	/* Specular Light */
	glLightfv(GL_LIGHT5, GL_POSITION, LightPos1);
	glLightfv(GL_LIGHT5, GL_SPECULAR, LightClr2);
	glLightfv(GL_LIGHT6, GL_POSITION, LightPos4);
	glLightfv(GL_LIGHT6, GL_SPECULAR, LightClr2);

	/* Draw Objects (define in "DecExp.h") */	
	switch(AMODE){
		case 0:
		case 1:
			glNeutMonte();
			break;
		case 2:
			glPhotMonte();
			glPushMatrix();
			glTranslated(px,py,-150.5);
			glRotated(180,0,1,0);
			glBetaPlastic();
			glPopMatrix();
			break;
		default:
			break;
	}
	glTUNA();
	glMAGRO();
	glBetaPlastic();
	glChibitaku();
	glLiGlass();
	glENcourse();
	glCameraTarget();
	glDisplayStrings();
	glDisplayButtons();

	glutSwapBuffers();
}


void display2(void){
	glClear(GL_COLOR_BUFFER_BIT);	
	glGraph();
	glFlush();
}


void resize1(int w, int h){
	AwindowW = w;
	AwindowH = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, w/(double)h, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	double ex = xc+dstnc*cos(phi)*sin(theta);
	double ey = yc+dstnc*sin(phi);
	double ez = zc+dstnc*cos(phi)*cos(theta);
	gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
}


void resize2(int w, int h){
	glViewport(0, 0, w ,h);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	if(AMODE==0) glutReshapeWindow(400,400);
	if(AMODE==1) glutReshapeWindow(400,400);
	if(AMODE==2) glutReshapeWindow(400,800);
}


/* mouse click */
void mouse(int button, int state, int x, int y){
	int ID = glutGetWindow();
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if(state==GLUT_UP){
				MFLAG = false;
				if(ID==WindowID[1] && AMODE==2 && RTFLAG==false){
					glPhotInit();
					glHistInit();
					PMThistgramInit();
				}
			}
			if(state==GLUT_DOWN){
				if(SFLAG){
					double Xbutton1 = AwindowW/2.0;
					double Ybutton1 = AwindowH/1.05;
					if(Xbutton1-20<=x && x<=Xbutton1+20){
						if(Ybutton1-15<=y && y<=Ybutton1+15){
							if(AFLAG) AFLAG = false;
							else      AFLAG = true;
						}
					}
					double Xbutton2 = AwindowW/2.0+50.0;
					double Ybutton2 = Ybutton1;
					if(Xbutton2-20<=x && x<=Xbutton2+20){
						if(Ybutton2-15<=y && y<=Ybutton2+15){
							AFLAG = false;
							glHistInit();
							if(AMODE==0) glNeutInit();
							if(AMODE==1) glNeutInit();
							if(AMODE==2){
								glPhotInit();
								PMThistgramInit();
							}
						}
					}
					double Xbutton3 = AwindowW/2.0-50.0;
					double Ybutton3 = AwindowH/1.05;
					if(Xbutton3-20<=x && x<=Xbutton3+20){
						if(Ybutton3-15<=y && y<=Ybutton3+15){
							Pspeed += 20.0;
							if(200<Pspeed) Pspeed = 20.0;
						}
					}

				}
			}
			glutIdleFunc(idle);
			break;
		default:
			break;
	}
}


/* mouse motion */
void motion(int x, int y){
	const double Rspeed = 0.2; /* Rotate Speed */
	double dTrans = dstnc/400; /* Translate Speed */
	static int xmouse;
	static int ymouse;

	if(MFLAG){
		int ID = glutGetWindow();
		if(ID==WindowID[0]){ /* for Histgram Window */
			if(TFLAG && AMODE==2 && OMODE==1){
				Threshold += (double)(ymouse-y)*fabs(ThresholdMAX)/1000;
				if(Threshold<=ThresholdMAX) Threshold = ThresholdMAX;
				if(Threshold>=0.0)          Threshold = 0.0;
				glutIdleFunc(idle);
			}
		}
		if(ID==WindowID[1]){ /* for Animation Window */
			if(RTFLAG){
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				theta += (xmouse-x)*PI/180*Rspeed;
				if(theta>2*PI) theta -= 2*PI;
				if(theta<0)    theta += 2*PI;
				phi +=-(ymouse-y)*PI/180*Rspeed;
				if(phi>= PI/2) phi = PI/2-0.1*PI/180;
				if(phi<=-PI/2) phi =-PI/2+0.1*PI/180;
				double ex = xc+dstnc*cos(phi)*sin(theta);
				double ey = yc+dstnc*sin(phi);
				double ez = zc+dstnc*cos(phi)*cos(theta);
				if(ey<=-Hbeam+10) ey = -Hbeam+10.0;
				gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
			}
			else{
				if(AMODE==2){
					px += -(double)(xmouse-x)*cos(theta)/10;
					py +=  (double)(ymouse-y)*cos(phi)/10;
					if( x_max-5<=px) px= x_max-5;
					if(-x_max+5>=px) px=-x_max+5;
					if( y_max-1<=py) py= y_max-1;
					if(-y_max+1>=py) py=-y_max+1;
				}
				else{
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					xc += ((xmouse-x)*cos(-theta)+(ymouse-y)*sin(theta))*dTrans;
					zc += ((xmouse-x)*sin(-theta)+(ymouse-y)*cos(theta))*dTrans;
					double ex = xc+dstnc*cos(phi)*sin(theta);
					double ey = yc+dstnc*sin(phi);
					double ez = zc+dstnc*cos(phi)*cos(theta);
					gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
				}
			}
		}
	}
	glutIdleFunc(idle);
	xmouse = x;
	ymouse = y;
	MFLAG  = true;
}


void menu(int val){
	switch(val){
		case 0:  /* Quit */
			exit(0);
			break;
		case 10: /* Change Mouse Mode */
			if(RTFLAG) RTFLAG = false;
			else       RTFLAG = true;
			glutIdleFunc(idle);
			break;
		case 11: /* Polygon On/Off */
			if(PFLAG)  PFLAG = false;
			else       PFLAG = true;
			glutIdleFunc(idle);
			break; 
		case 12: /* Status On Off */
			if(SFLAG)  SFLAG = false;
			else       SFLAG = true;
			glutIdleFunc(idle);
			break;
		case 13: /* Setting for Neutron Animation(TUNA) */
			AMODE = 0;
			AFLAG = false;
			SettingTUNA();
			glHistInit();
			glNeutInit();
			glutIdleFunc(idle);
			break;
		case 14: /* Setting for Neutron Animation(Li-Glass) */
			AMODE = 1;
			AFLAG = false;
			SettingLiGlass();
			glHistInit();
			glNeutInit();
			glutIdleFunc(idle);
			break;
		case 15: /* Setting for Photon Animation in TUNA */
			if(AMODE==2&&OMODE==0){
				glutIdleFunc(idle);
			}
			else{
				if(AMODE!=2){
					AFLAG = false;
					SettingPhoton();
					glPhotInit();
					glHistInit();
				}
				AMODE = 2;
				OMODE = 0;
				glutIdleFunc(idle);
			}
			break;
		case 16: /* Play or Stop Animation */
			if(AFLAG) AFLAG = false;
			else      AFLAG = true;
			glutIdleFunc(idle);
			break;
		case 17: /* Reset */
			AFLAG = false;
			if(AMODE==0) glNeutInit();
			if(AMODE==1) glNeutInit();
			if(AMODE==2) glPhotInit();
			glHistInit();
			glutIdleFunc(idle);
			break;
		case 18: /* Create Stock */
			if(AMODE==2&&OMODE==0){
				HFLAG = true;
				ppx   = px;
				ppy   = py;
				PreLoopCounter = LoopCounter;
				for(int ch=0;ch<ARRAY3;++ch){
					PreHistR[ch] = hist_rPMT[ch];
					PreHistL[ch] = hist_lPMT[ch];
				}
			}
			glutIdleFunc(idle);
			break;
		case 19: /* Delete Stock */
			if(AMODE==2&&OMODE==0){
				HFLAG = false;
				ppx   = 0.0;
				ppy   = 0.0;
				PreLoopCounter = 1;
				for(int ch=0;ch<ARRAY3;++ch){
					PreHistR[ch] = 0;
					PreHistL[ch] = 0;
				}
			}
			break;
		case 20: /* Set Threshold */
			if(AMODE==2&&OMODE==1){
				if(TFLAG){
					TFLAG = false;
					PMThistgramInit();
				}
				else{
					TFLAG = true;
					Threshold = ThresholdMAX/5;
				}
			}
			glutIdleFunc(idle);
			break;
		case 21: /* Noise On_Off */
			if(AMODE==2){
				if(NFLAG) NFLAG = false;
				else NFLAG = true;
				PMThistgramInit();
			}
			glutIdleFunc(idle);
			break;
		case 22: /* Oscillo Mode */
			if(AMODE==2&&OMODE==1){
				glutIdleFunc(idle);
			}
			else{
				if(AMODE!=2){
					AFLAG = false;
					SettingPhoton();
					glPhotInit();
					glHistInit();
				}
				AMODE = 2;
				OMODE = 1;
				glutIdleFunc(idle);
			}
			break;
		case 23: /* Oscillo Histgram Mode */
			if(AMODE==2&&OMODE==2){
				glutIdleFunc(idle);
			}
			else{
				if(AMODE!=2){
					SettingPhoton();
					glPhotInit();
					glHistInit();
				}
				AFLAG = false;
				AMODE = 2;
				OMODE = 2;
				glutIdleFunc(idle);
			}
			break;
		case 24: /* Log Scale for PMT Histgram */
			if(AMODE==2&&OMODE==2){
				if(LFLAG) LFLAG = false;
				else LFLAG = true;
				glutIdleFunc(idle);
			}
			break;
		default:
			break;
	}
}


void keyboard(unsigned char key, int x, int y){
	double ex,ey,ez;
	switch(key){
		case 'q': /* Quit */
		case 'Q':
		case '\033':
			printf("\033[39m\033[49m\n");
			exit(0);
		case 'm': /* Change Mouse Mode */
			if(RTFLAG) RTFLAG = false;
			else       RTFLAG = true;
			glutIdleFunc(idle);
			break;
		case 's': /* Status On Off */
		case 'S':
			if(SFLAG) SFLAG = false;
			else      SFLAG = true;
			glutIdleFunc(idle);
			break;
		case '_': /* Angle Reset */
			switch(AMODE){
				case 0:
					SettingTUNA();
					break;
				case 1:
					SettingLiGlass();
					break;
				case 2:
					glutSetWindow(WindowID[1]);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					dstnc = 120.0;
					theta = 180*PI/180;
					phi   =   0*PI/180;
					xc = 0.0;
					yc = 0.0;
					zc = 0.0;
					ex = xc+dstnc*cos(phi)*sin(theta);
					ey = yc+dstnc*sin(phi);
					ez = zc+dstnc*cos(phi)*cos(theta);
					gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
					break;
				default:
					break;
			}
			glutPostRedisplay();
			break;
		case 'p': /* Polygon Mode */
		case 'P':
			if(PFLAG) PFLAG = false;
			else      PFLAG = true;
			glutIdleFunc(idle);
			break;
		case ' ': /* Animation Play */
			if(AFLAG) AFLAG = false;
			else      AFLAG = true;
			glutIdleFunc(idle);
			break;
		case 'r': /* Reset Animation */
		case 'R':
			AFLAG = false;
			glHistInit();
			if(AMODE==0) glNeutInit();
			if(AMODE==1) glNeutInit();
			if(AMODE==2) glPhotInit();
			glHistInit();
			glutIdleFunc(idle);
			break;
		case 'h': /* Stock Histgram */
		case 'H':
			if(AMODE==2&&OMODE==0){
				if(HFLAG){
					HFLAG = false;
					ppx   = 0.0;
					ppy   = 0.0;
					PreLoopCounter = 1;
					for(int ch=0;ch<ARRAY3;++ch){
						PreHistR[ch] = 0;
						PreHistL[ch] = 0;
					}
				}
				else{
					HFLAG = true;
					ppx   = px;
					ppy   = py;
					PreLoopCounter = LoopCounter;
					for(int ch=0;ch<ARRAY3;++ch){
						PreHistR[ch] = hist_rPMT[ch];
						PreHistL[ch] = hist_lPMT[ch];
					}
				}
			}
			glutIdleFunc(idle);
			break;
		case 't': /* Set Threshold */
		case 'T':
			if(AMODE==2){
				if(TFLAG){
					TFLAG = false;
					PMThistgramInit();
				}
				else{
					TFLAG = true;
					Threshold = ThresholdMAX/5;
				}
			}
			glutIdleFunc(idle);
			break;
		case 'n': /* Noise On Off */
		case 'N':
			if(AMODE==2){
				if(NFLAG) NFLAG = false;
				else NFLAG = true;
				PMThistgramInit();
			}
			glutIdleFunc(idle);
			break;
		case 'o': /* Oscillo Mode */
		case 'O':
			if(AMODE==2){
				if(OMODE<2) ++OMODE;
				else OMODE = 0;
				glutIdleFunc(idle);
			}
			break;
		case 'l': /* Log Scale for PMT Histgram */
		case 'L':
			if(AMODE==2 && OMODE==2){
				if(LFLAG) LFLAG = false;
				else LFLAG = true;
				glutIdleFunc(idle);
			}
			break;
		default:
			break;
	}
}


void keyboard_sp(int key, int x, int y){
	int ID = glutGetWindow();
	double ex,ey,ez;
	double dDstnc = dstnc/ 50;
	switch (key) {
		case GLUT_KEY_RIGHT:
			if(AMODE<2){
				++AMODE;
				AFLAG = false;
				if(AMODE==1){
					SettingLiGlass();
					glNeutInit();
				}
				if(AMODE==2){
					SettingPhoton();
					glPhotInit();
					MAXpmt = -100;
				}
				glHistInit();
			}
			glutIdleFunc(idle);
			break;
		case GLUT_KEY_LEFT:
			if(AMODE>0){
				--AMODE;
				if(AFLAG) AFLAG = false;
				if(AMODE==0) SettingTUNA();
				if(AMODE==1) SettingLiGlass();
				glNeutInit();
				glHistInit();
			}
			glutIdleFunc(idle);
			break;
		case GLUT_KEY_UP:
			if(ID==WindowID[0]){
				if(TFLAG){
					Threshold+=0.1;
					if(Threshold>=0.0) Threshold = 0.0;
				}
			}
			if(ID==WindowID[1]){
				dstnc -= dDstnc;
				if(dstnc<10) dstnc = 10;
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				ex = xc+dstnc*cos(phi)*sin(theta);
				ey = yc+dstnc*sin(phi);
				ez = zc+dstnc*cos(phi)*cos(theta);
				if(ey<=-Hbeam+10) ey = -Hbeam+10.0;
				gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
			}
			glutIdleFunc(idle);
			break;
		case GLUT_KEY_DOWN:
			if(ID==WindowID[0]){
				if(TFLAG){
					Threshold-=0.1;
					if(Threshold<=ThresholdMAX) Threshold = ThresholdMAX;
				}
			}
			if(ID==WindowID[1]){
				dstnc += dDstnc;
				if(dstnc>700) dstnc = 700;
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				ex = xc+dstnc*cos(phi)*sin(theta);
				ey = yc+dstnc*sin(phi);
				ez = zc+dstnc*cos(phi)*cos(theta);
				if(ey<=-Hbeam+10) ey = -Hbeam+10.0;
				gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
			}
			glutIdleFunc(idle);
			break;
		default:
			break;
	}
}


void SettingTUNA(void){
	glutSetWindow(WindowID[1]);
	double ex,ey,ez;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	speed = 1000;
	dstnc = 300.0;
	theta = 230*PI/180;
	phi   =  30*PI/180;
	xc =   0.0;
	yc =   0.0;
	zc =-150.0;
	ex = xc+dstnc*cos(phi)*sin(theta);
	ey = yc+dstnc*sin(phi);
	ez = zc+dstnc*cos(phi)*cos(theta);
	gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
}


void SettingLiGlass(void){
	glutSetWindow(WindowID[1]);
	double ex,ey,ez;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	speed =   100;
	dstnc =  50.0;
	theta = 320*PI/180;
	phi   =  30*PI/180;
	xc =   0.0;
	yc =   0.0;
	zc =-150.0;
	ex = xc+dstnc*cos(phi)*sin(theta);
	ey = yc+dstnc*sin(phi);
	ez = zc+dstnc*cos(phi)*cos(theta);
	gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
}


void SettingPhoton(void){
	glutSetWindow(WindowID[1]);
	double ex,ey,ez;
	px = 0.0;
	py = 0.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	dstnc = 120.0;
	theta = 180*PI/180;
	phi   =   0*PI/180;
	xc = 0.0;
	yc = 0.0;
	zc = 0.0;
	ex = xc+dstnc*cos(phi)*sin(theta);
	ey = yc+dstnc*sin(phi);
	ez = zc+dstnc*cos(phi)*cos(theta);
	gluLookAt(ex,ey,ez,xc,yc,zc,0,1,0);
}


