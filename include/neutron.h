#ifndef Neutron_h_
#define Neutron_h_

/**********************/

#include<math.h>
#include"define.h"


/* lorentzian に従って17Nの中性子のエネルギー[MeV]を作る関数(逆関数法) */
double NeutronEnergy_17N(){
	
	/* 17Nのβ遅発中性子のエネルギーと分岐比とガンマ幅 */
	const double Ene1   = 2.0828;
	const double Ene2   = 1.6904;
	const double Ene3   = 1.6246;
	const double Ene4   = 1.4963;
	const double Ene5   = 1.1635;
	const double Ene6   = 0.8864;
	const double Ene7   = 0.3866;

	const double Ratio1 =          0.0008;
	const double Ratio2 = Ratio1 + 0.0690;
	const double Ratio3 = Ratio2 + 0.0015;
	const double Ratio4 = Ratio3 + 0.0023;
	const double Ratio5 = Ratio4 + 0.5010;
	const double Ratio6 = Ratio5 + 0.0060;
	const double Ratio7 = Ratio6 + 0.3800;

	const double Gamma1 = 0.1240;
	const double Gamma2 = 0.0320;
	const double Gamma3 = 0.0066;
	const double Gamma4 = 0.0010;
	const double Gamma5 = 0.0280;
	const double Gamma6 = 0.0960;
	const double Gamma7 = 0.0400;
	
	double E     = 0.0;
	double gamma = 0.0;
	double P = randf();

	if        ( 0.0 < P && P <= Ratio1) {E = Ene1;gamma = Gamma1;} 
	else if( Ratio1 < P && P <= Ratio2) {E = Ene2;gamma = Gamma2;}
	else if( Ratio2 < P && P <= Ratio3) {E = Ene3;gamma = Gamma3;}
	else if( Ratio3 < P && P <= Ratio4) {E = Ene4;gamma = Gamma4;}
	else if( Ratio4 < P && P <= Ratio5) {E = Ene5;gamma = Gamma5;}
	else if( Ratio5 < P && P <= Ratio6) {E = Ene6;gamma = Gamma6;}
	else if( Ratio6 < P && P <= Ratio7) {E = Ene7;gamma = Gamma7;}

	if(randf() < 0.5) return E + gamma * fabs(tanf(PI*(randf()-0.5)));
	else return E - gamma * fabs(tanf(PI*(randf()-0.5)));
}


/* p-n Scattering Cross Section [barn] (Neutron Energy [MeV]) */
double pnCrossSection(double E){
	/* Parametors [m] */
	double R0   = 4.31e-15;
	double ret  = 1.704e-15/2.0;
	double res  = 2.49e-15/2.0;
	double as   = -23.806e-15;
	/* Dirac's Constant [eV•s] */
	double hbar = 6.58212e-16;
	/* Mass [eV/c^2] */
	double Mn   = Mnc2*1.0e6;
	double Mp   = Mpc2*1.0e6;
	double myu  = (Mn*Mp)/(Mn+Mp);
	/* Wave Number [1/m] */
	double k    = sqrt(E*1.0e6*myu)/(hbar*c*1.0e7);
	double A1   = 3.0*PI/(k*k+pow(1.0/R0-(ret*(k*k+1.0/(R0*R0))),2));
	double A2   = PI/(k*k+pow(1.0/as-res*k*k,2));
	return (A1+A2)*1.0e28;
}


/* Return Neutron Hit Podition (BC408) [cm] */
double HitPosition(double E){
	return -log(1.0-randf())/(pnCrossSection(E)*5.23e-2);
}


/**********************/

#endif //"Neutron_h_"
