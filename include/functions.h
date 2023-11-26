


#define curK 1.07843 
#define curB 1.65

#define curK2 0.72368
#define curB2 1.65

std::ofstream time_calc("/home/baris/roboleg2/time_calc.txt");

double dmdCur2ADCV(double dmdcur) {   // dmdcur :demand current for ball screw motor
    
    return curK*dmdcur + curB;
}

double dmdCur2ADCV2(double dmdcur2) {   // dmdcur 2:demand current for double sided motor 
    
    return curK2*dmdcur2 + curB2;
}


double Chirp(double w1, double w2, double A,double M,double time)
{
	double res;
	res = A*sin(w1*time + (w2-w1)*time*time/(2*M));
	return res;
}

double ChirpCos(double w1, double w2, double A,double M,double time)
{
	double res;
	res = A*cos(w1*time + (w2-w1)*time*time/(2*M));
	return res;
}














