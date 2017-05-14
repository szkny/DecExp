#ifndef Define_h_
#define Define_h_

/***************************/

#ifndef PI
#define PI 3.14159265359
#endif

//BC-408の屈折率
#ifndef n_scinti
#define n_scinti 1.58
#endif

//BC-630(optical grease)の屈折率
#ifndef n_grease
#define n_grease 1.465
#endif

//EJ-560(optical pad)の屈折率
#ifndef n_pad
#define n_pad 1.43
#endif

//ライトガイド(ポリメタクリル酸メチル樹脂)の屈折率
//#define n_guide 1.49
#ifndef n_guide
#define n_guide 1.58
#endif

//BC-408の発光時定数 [ns]
#ifndef tau
#define tau 2.1
#endif

//BC-408のBulk Light Attenuation Length [cm]
#ifndef L_a
#define L_a 380
#endif

//光速 [cm/ns]
#ifndef c
#define c 29.9792458
#endif

//反射材の反射率
#ifndef p_reflect
#define p_reflect 0.98
#endif

//時間刻み 1[ps]  -> シンチレータ内の光子は1[ps]あたりに0.1897[mm]進む
#ifndef dt
#define dt 0.001
#endif

//発光数のFactor
#ifndef Factor_a
#define Factor_a 4.0/3.0
#endif

//中性子の静止質量 [MeV/c^2]
#ifndef Mnc2
#define Mnc2 939.5654133 
#endif

//陽子の静止質量   [MeV/c^2]
#ifndef Mpc2
#define Mpc2 938.2720813
#endif


#ifndef Randf_
#define Randf_
//0~1の一様乱数の作成する関数
double randf(void){
    rand();rand();rand();rand();rand();rand();
    return rand()*(1.0/RAND_MAX);
}
#endif

#ifndef Poisson_
#define Poisson_
/* random function (poison distribution) */
int poisson(double lambda){
	if( lambda<30 ){
		double x;
		int k = 0;
		x = randf();
		while( x>=exp(-lambda) ) {
			x = x*randf();
			k = k+1;
		}
		return k;
	}
	else{
		double sigma = sqrt(lambda);
		double z = sqrt(-2.0*log(randf()))*sin(2.0*PI*randf());
		int k = lambda+sigma*z;
		return k;
	}
}
#endif


/***************************/

#endif //"Define_h_"
