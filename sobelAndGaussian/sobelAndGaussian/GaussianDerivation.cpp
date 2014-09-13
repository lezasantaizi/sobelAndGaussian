#include "GaussianDerivation.h"

GaussianDerivation::GaussianDerivation()
{
	win_size = 1;
	sigma = 10;
	sigma2 = sigma * sigma;  
	sigma4 = sigma2 * sigma2;  
	arr = new float*[2*win_size+1];
	m_xDerviation = new float*[2*win_size+1];
	m_yDerviation = new float*[2*win_size+1];
	m_xxDerviation = new float*[2*win_size+1];
	m_yyDerviation = new float*[2*win_size+1];
	m_xyDerviation = new float*[2*win_size+1];

	for (int i = 0;i< 2*win_size+1;i++)
	{
		arr[i] = new float[2*win_size+1];
		m_xDerviation[i] = new float[2*win_size+1];
		m_yDerviation[i] = new float[2*win_size+1];
		m_xxDerviation[i] = new float[2*win_size+1];
		m_yyDerviation[i] = new float[2*win_size+1];
		m_xyDerviation[i] = new float[2*win_size+1];

		for (int j = 0 ;j < 2*win_size+1;j++)
		{
			arr[i][j] = 0; 
			m_xDerviation[i][j] = 0;
			m_yDerviation[i][j] = 0;
			m_xxDerviation[i][j] = 0;
			m_yyDerviation[i][j] = 0;
			m_xyDerviation[i][j] = 0;
		}
	}
	genGaussianFunc();
}

GaussianDerivation::~GaussianDerivation()
{
	for (int i = 0;i< 2*win_size+1;i++)
	{
		delete[] arr[i];
		delete[] m_xDerviation[i];
		delete[] m_yDerviation[i];
		delete[] m_xxDerviation[i];
		delete[] m_yyDerviation[i];
		delete[] m_xyDerviation[i];
	}
	delete[] arr;
	delete[] m_xDerviation;
	delete[] m_yDerviation;
	delete[] m_xxDerviation;
	delete[] m_yyDerviation;
	delete[] m_xyDerviation;
}

void GaussianDerivation::genGaussianFunc()
{
	for (int i = -win_size ; i <= win_size; i++)
	{
		for (int j = -win_size ; j<= win_size; j++)
		{
			int r = i*i + j*j;
			float k = -r/(2*sigma*sigma);
			arr[i+win_size][j+win_size] = exp(k);
		}
	}
}
void GaussianDerivation::xDerivation()
{
	for (int i = -win_size; i <=win_size; i++)
	{
		float c = -i/(sigma*sigma);
		for (int j = 0; j< 2*win_size+1;j++)
		{
			m_xDerviation[j][i+win_size] = c*arr[j][i+win_size];
		}
	}
	//normalizeData(x,win_size);
	//for (int i = 0;i<2*win_size+1;i++)
	//{
	//	for (int j = 0 ;j<2*win_size+1;j++)
	//	{
	//		printf("%f ",x[i][j]);
	//	}
	//	printf("\n");
	//}
}

void GaussianDerivation::yDerivation()
{
	for (int i = -win_size; i <=win_size; i++)
	{
		float c = -i/(sigma*sigma);
		for (int j = 0; j< 2*win_size+1;j++)
		{
			m_yDerviation[i+win_size][j] = c*arr[i+win_size][j];	
		}
	}
}

void GaussianDerivation::xxDerivation()
{
	for(int x=-win_size; x<=win_size; x++)  
	{  
		double c = -((x - sigma2)/sigma4);  
		for(int i=0; i<2*win_size+1; i++)  
		{  
			m_xxDerviation[i][x + win_size] = c * arr[i][x + win_size];      
		} 
	}  
}

void GaussianDerivation::yyDerivation()
{
	for(int y=-win_size; y<=win_size; y++)  
	{  
		double c = -((y - sigma2)/sigma4);  
		for(int i=0; i<win_size * 2 + 1; i++)  
		{  
			m_yyDerviation[y + win_size][i] = c * arr[y + win_size][i];               
		}  
	} 
}


void GaussianDerivation::xyDerivation()
{
	for(int x=-win_size; x<=win_size; x++)  
	{  
		for(int y=-win_size; y<=win_size; y++)  
		{  
			double c = -((x*y)/sigma4);  
			m_xyDerviation[x + win_size][y + win_size] = c * arr[x + win_size][y + win_size];  
		}	
	}  
	normalizeData(m_xyDerviation,win_size);
}

void GaussianDerivation::normalizeData(float** data,int win_size)
{
		double min = data[0][0];  
		for(int x=-win_size; x<=win_size; x++)  
		{  
			for(int y=-win_size; y<=win_size; y++)  
			{  
				if(min > data[x + win_size][y + win_size])  
				{  
					min = data[x + win_size][y + win_size];  
				}  
			}  
		}  

		for(int x=-win_size; x<=win_size; x++)  
		{  
			for(int y=-win_size; y<=win_size; y++)  
			{  
				data[x + win_size][y + win_size] = data[x + win_size][y + win_size] /min;  
			}  
		}  
}

void GaussianDerivation::processderivation(int width, int height, Mat& src, Mat& dst, deviation_type deviationType)
{
	Mat_<Vec3b> src_ = src;
	Mat_<Vec3b> dst_ = dst;


	switch (deviationType)
	{
	case 0:
		xDerivation();
		break;
	case 1:
		yDerivation();
		break;
	case 2:
		xxDerivation();
		break;
	case 3:
		yyDerivation();
		break;
	case 4:
		xyDerivation();
		break;
	default:
		printf("error\n");
		return ;
	}
	float** deviationArr[5] = {m_xDerviation,m_yDerviation,m_xxDerviation,m_yyDerviation,m_xxDerviation};

	for(int row=0; row<height; row++) 
	{  
		for(int col=0; col<width; col++) 
		{  
			double xred = 0;  
			double xgreen = 0;  
			double xblue = 0;  

			for(int subrow = -win_size; subrow <= win_size; subrow++) 
			{  
				for(int subcol = -win_size; subcol <= win_size; subcol++) 
				{  
					int newRow = row + subrow;  
					int newCol = col + subcol;  
					if(newRow < 0 || newRow >= height) 
					{  
						newRow = row;  
					}  
					if(newCol < 0 || newCol >= width) 
					{  
						newCol = col;  
					}  

					int tr = src_(newRow,newCol)[2];  
					int tg = src_(newRow,newCol)[1];  
					int tb = src_(newRow,newCol)[0];  

					xred += (deviationArr[deviationType][subrow + win_size][subcol + win_size] * tr);  
					xgreen += (deviationArr[deviationType][subrow + win_size][subcol + win_size] * tg);  
					xblue += (deviationArr[deviationType][subrow + win_size][subcol + win_size] * tb);  
				}  
			}  
			int temp1 = xred * 10 + 50; 
			int temp2 = xgreen * 10 + 50;
			int temp3 = xblue * 10 + 50;
			dst_(row,col)[2] = temp1 < 0 ? 0 : (temp1 > 255 ? 255 : temp1);
			dst_(row,col)[1] = temp2 < 0 ? 0 : (temp2 > 255 ? 255 : temp2);
			dst_(row,col)[0] = temp3 < 0 ? 0 : (temp3 > 255 ? 255 : temp3);
		}  
	} 
}