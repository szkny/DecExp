#ifndef PMT_h_
#define PMT_h_

/**********************/

#include<math.h>
#include"define.h"

/* electric charge */
#ifndef Qe
#define Qe (-1.60217662*1e-19)
#endif

/*** parameters of H11934-200 ***/
/* timing range */
#ifndef PMTrange
#define PMTrange 50.0
#endif

/* sampling rate */
#ifndef PMTarray
#define PMTarray 1000
#endif

/* ∆t */
#ifndef PMTdt
#define PMTdt (PMTrange/PMTarray)
#endif

/* PMAmp gain */
#ifndef PMamp
#define PMamp 10.0
#endif

/* tarmination register */
#ifndef R_L
#define R_L 50
#endif

#ifndef risetime
#define risetime 1.3
#endif

#ifndef falltime
#define falltime 5.8
#endif

/* sigma for rise time (gaussian) */
const double sigma1_pmt = risetime/fabs(sqrt(2*log(1/0.9))-sqrt(2*log(1/0.1)));

/* sigma for fall time (gaussian) */
const double sigma2_pmt = falltime/fabs(sqrt(2*log(1/0.9))-sqrt(2*log(1/0.1)));

/* gamma for fall time (lorentzian) */
const double gamma_pmt = falltime*3/8.0;

/* normalization factor */
const double normal_pmt = sqrt(2*PI)*sigma1_pmt+(sqrt(2*PI)*sigma2_pmt+PI*gamma_pmt)/2.0;


/* random function of transit time (Box–Muller's method) */
double randtransit(void){
	double transittime = 5.8;
	double sigma = 0.27;
	int sign = rand()%2*2-1;
	double z = sqrtf(-2.0*log(randf()))*sin(2.0*PI*randf());
	return transittime+(double)sign*sigma*z;
}


/* gain distribution */
int PMTgain(void){
	/* parameters */
	const int total_gain  = 480000;
	const double HV       = 950.0;
	const int dinode      = 13;
	const double Factor_k = 0.7548923;
	/* ratio of dinode suply voltage */
	double Ratio[dinode] = {0};
	Ratio[0] = 3.3;
	Ratio[1] = 1.6;
	for (int i=2;i<=10;++i){
		Ratio[i] = 1.0;
	}
	Ratio[11] = 2.7;
	Ratio[12] = 1.3;
	/* sum of ratio */
	double sum_Ratio = 0;
	for(int i=0;i<dinode;++i){
		sum_Ratio += Ratio[i];
	}
	/* each dinode suply voltage */
	double Supply_Volt[dinode];
	for (int i=0;i<dinode;++i){
		Supply_Volt[i] = HV*Ratio[i]/sum_Ratio;
	}
	/* dicistion of Factor_A */
	double V_k = powf(Supply_Volt[0],Factor_k);
	for(int i=1;i<dinode;++i){
		V_k *= powf(Supply_Volt[i],Factor_k);
	}
	const double Factor_A = powf(total_gain/V_k,1.0f/dinode);
	/* each dinode gain */
	double gain[dinode];
	for(int i=0;i<dinode;++i){
		gain[i] = Factor_A*powf(Supply_Volt[i],Factor_k);
	}
	/* counter for multiplicated electron */
	int counter = 1;
	for (int j=0; j<dinode; ++j){
		counter = poisson(gain[j]*counter);
	}
	return counter;
}


/* ∆sum for output */
double dsum(double height, double PMTtime, int ch){
	if(ch*PMTdt<PMTtime){
		return height*exp(-pow(ch*PMTdt-PMTtime,2)/(2*pow(sigma1_pmt,2)))*Qe/(PMTdt*1e-9)*R_L*1000;
	}
	else{
		return height*(exp(-pow(ch*PMTdt-PMTtime,2)/(2*pow(sigma2_pmt,2)))/4+1/(1+pow((ch*PMTdt-PMTtime)/gamma_pmt,2))*3/4)*Qe/(PMTdt*1e-9)*R_L*1000;
	}
}


#define TOTALPHOTON 20000

/* PMT(H11934-200) signal output */
double reachtimeR[TOTALPHOTON] = {0.0};
double reachtimeL[TOTALPHOTON] = {0.0};
double PMTtimeR[TOTALPHOTON]   = {0.0};
double PMTtimeL[TOTALPHOTON]   = {0.0};
double heightR[TOTALPHOTON]    = {0.0};
double heightL[TOTALPHOTON]    = {0.0};
double outputR[PMTarray]       = {0.0};
double outputL[PMTarray]       = {0.0};

/* oscillo scope maximam range */
double MAXpmt = -100.0;

/* thereshold (mV) */
bool TFLAG = false;
double Threshold    = MAXpmt/5;
double preThreshold = Threshold;
double ThresholdMAX = MAXpmt;

/* output × threshold timing histgram */
double crosstimeR = PMTrange*1.01;
double crosstimeL = PMTrange*1.01;
int pmthistR[PMTarray] = {0};
int pmthistL[PMTarray] = {0};


/* output initialize */
void PMToutputInit(void){
	crosstimeR  = PMTrange*1.01;
	crosstimeL  = PMTrange*1.01;
	for(int ch=0;ch<PMTarray;++ch){
		outputR[ch] = 0.0;
		outputL[ch] = 0.0;
	}
}

/* pmthist initialize */
void PMThistgramInit(void){
	for(int ch=0;ch<PMTarray;++ch){
		pmthistR[ch] = 0;
		pmthistL[ch] = 0;
	}
}


/* noise flag */
bool NFLAG  = false;

/* white noise (Box-Muller's method) */
void BaselineNoise(double NoiseLevel){
	if(NFLAG){
		for(int ch=0;ch<PMTarray;++ch){
			outputR[ch] += (rand()%2*2-1)*sqrt(-2.0*log(randf()))*sin(2.0*PI*randf())*NoiseLevel*PMamp;
			outputL[ch] += (rand()%2*2-1)*sqrt(-2.0*log(randf()))*sin(2.0*PI*randf())*NoiseLevel*PMamp;
		}
	}
}


/* PMT output */
void PMTH11934(void){

	/* fill PMT output */
	for(int i=0;i<TOTALPHOTON;++i){
		if(reachtimeR[i]!=0.0){
			/* PMT time initialize */
			//if(PMTtimeR[i]==0.0)
			PMTtimeR[i]  = reachtimeR[i]+randtransit();

			/* pulse height initialize */
			//if(heightR[i]==0.0)
			heightR[i]  = PMTgain()*2.0*PMTdt/normal_pmt;
			for(int ch=0; ch<PMTarray; ++ch){
				outputR[ch] += dsum(heightR[i],PMTtimeR[i],ch)*PMamp;
			}
		}
		if(reachtimeL[i]!=0.0){
			/* PMT time initialize */
			//if(PMTtimeL[i]==0.0)
			PMTtimeL[i] = reachtimeL[i]+randtransit();
			/* pulse height initialize */
			//if(heightL[i]==0.0)
			heightL[i] = PMTgain()*2.0*PMTdt/normal_pmt;
			for(int ch=0; ch<PMTarray; ++ch){
				outputL[ch] += dsum(heightL[i],PMTtimeL[i],ch)*PMamp;
			}
		}
	}
}


/* leading edge for PMT Histgram */
void LeadingEdge(void){
	if(TFLAG){
		bool FLAG = true;
		for(int ch=0;ch<PMTarray;++ch){
			if(FLAG){
				if(outputR[ch]<=Threshold){
					crosstimeR = ch*PMTdt;
					++pmthistR[ch];
					FLAG = false;
				}
			}
		}
		if(FLAG) crosstimeR = PMTrange*1.01;
		else FLAG = true;

		for(int ch=0;ch<PMTarray;++ch){
			if(FLAG){
				if(outputL[ch]<=Threshold){
					crosstimeL = ch*PMTdt;
					++pmthistL[ch];
					FLAG = false;
				}
			}
		}
		if(FLAG) crosstimeL = PMTrange*1.01;
	}
}



/* initialize PMT histgram (for changeing threshold) */
void ThresholdInit(void){
	if(TFLAG){
		if(preThreshold!=Threshold){
			PMThistgramInit();
			bool FLAG = true;
			for(int ch=0;ch<PMTarray;++ch){
				if(FLAG){
					if(outputR[ch]<=Threshold){
						crosstimeR = ch*PMTdt;
						FLAG = false;
					}
				}
			}
			if(FLAG) crosstimeR = PMTrange*1.01;
			else FLAG = true;
			for(int ch=0;ch<PMTarray;++ch){
				if(FLAG){
					if(outputL[ch]<=Threshold){
						crosstimeL = ch*PMTdt;
						FLAG = false;
					}
				}
			}
			if(FLAG) crosstimeL = PMTrange*1.01;
		}
		preThreshold = Threshold;
	}
}


/**********************/
#endif //"PMT_h_"
