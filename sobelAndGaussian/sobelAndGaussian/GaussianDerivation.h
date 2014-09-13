#ifndef GAUSSIAN_DERIVATION
#define GAUSSIAN_DERIVATION

#include<iostream>
#include "../3rdparty/include/opencv.hpp"
#include<math.h>
using namespace std;
using namespace cv;

enum deviation_type
{
	XDEVIATION = 0 ,
	YDEVIATION ,
	XXDEVIATION ,
	YYDEVIATION ,
	XYDEVIATION
};

class GaussianDerivation
{
public:

	GaussianDerivation(int WIN_SIZE = 1 , float SIGMA = 10);
	~GaussianDerivation();
	int win_size;
	float sigma ;
	float** arr;
	float** m_xDerviation;
	float** m_yDerviation;
	float** m_xxDerviation;
	float** m_yyDerviation;
	float** m_xyDerviation;

	double sigma2;  
	double sigma4;  

	void genGaussianFunc();
	void xDerivation();
	void yDerivation();
	void xxDerivation();
	void yyDerivation();
	void xyDerivation();

	void normalizeData(float** data,int win_size);

	void processderivation(int width, int height, Mat& src, Mat& dst, deviation_type deviationType);


};

#endif