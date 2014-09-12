#include<iostream>
#include "../3rdparty/include/opencv.hpp"
#include<math.h>
using namespace std;
using namespace cv;

#pragma comment(lib, "opencv_highgui249d.lib")
#pragma comment(lib, "opencv_imgproc249d.lib")
#pragma comment(lib, "opencv_core249d.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
#pragma comment(lib, "opencv_core249.lib")

void normalizeData(float** data,int win_size)  
{  
	// normalization the data  
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

int main(int argc, char** argv)
{
	int win_size = 3;
	float sigma = 1.8;
	float** arr;
	float** x;
	float** y;
	float** xyDeviation;
	float** xx;
	float** yy;


	arr = new float*[2*win_size+1];
	x = new float*[2*win_size+1];
	y = new float*[2*win_size+1];
	xyDeviation = new float*[2*win_size+1];
	xx = new float*[2*win_size+1];
	yy = new float*[2*win_size+1];

	const char* filename = argc >= 2 ? argv[1] : "hello3.jpg";
	Mat src = imread(filename,0);
	if(src.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}
	Mat dst, cdst;
	namedWindow("hello");
	imshow("hello",src);

	Mat_<Vec3b> src_ = src;

	src.copyTo(dst);
	Mat_<Vec3b> dst_ = dst;

	for (int i = 0;i< 2*win_size+1;i++)
	{
		arr[i] = new float[2*win_size+1];
		x[i] = new float[2*win_size+1];
		y[i] = new float[2*win_size+1];
		xyDeviation[i] = new float[2*win_size+1];
		xx[i] = new float[2*win_size+1];
		yy[i] = new float[2*win_size+1];

		for (int j = 0 ;j < 2*win_size+1;j++)
		{
			arr[i][j] = 0; 
			x[i][j] = 0;
			y[i][j] = 0;
			xyDeviation[i][j] = 0;
			xx[i][j] = 0;
			yy[i][j] = 0;
		}
	}
	
	for (int i = -win_size ; i <= win_size; i++)
	{
		for (int j = -win_size ; j<= win_size; j++)
		{
			int r = i*i + j*j;
			float k = -r/(2*sigma*sigma);
			arr[i+win_size][j+win_size] = exp(k);
			printf("%f ",arr[i+win_size][j+win_size]);
		}
		printf("\n");
	}
	printf("\n");

	for (int i = -win_size; i <=win_size; i++)
	{
		float c = -i/(sigma*sigma);
		for (int j = 0; j< 2*win_size+1;j++)
		{
			x[j][i+win_size] = c*arr[j][i+win_size];
		}
	}
	//normalizeData(x,win_size);
	for (int i = 0;i<2*win_size+1;i++)
	{
		for (int j = 0 ;j<2*win_size+1;j++)
		{
			printf("%f ",x[i][j]);
		}
		printf("\n");
	}
	for (int i = -win_size; i <=win_size; i++)
	{
		float c = -i/(sigma*sigma);
		for (int j = 0; j< 2*win_size+1;j++)
		{
			y[i+win_size][j] = c*arr[i+win_size][j];	
		}
		
	}
	//normalizeData(y,win_size);

	double sigma2 = sigma * sigma;  
	double sigma4 = sigma2 * sigma2;  
	for(int x=-win_size; x<=win_size; x++)  
	{  
		for(int y=-win_size; y<=win_size; y++)  
		{  
			double c = -((x*y)/sigma4);  
			xyDeviation[x + win_size][y + win_size] = c * arr[x + win_size][y + win_size];  
		}	
	}  
	normalizeData(xyDeviation,win_size);

  


	for(int x=-win_size; x<=win_size; x++)  
	{  
		double c = -((x - sigma2)/sigma4);  
		for(int i=0; i<2*win_size+1; i++)  
		{  
			xx[i][x + win_size] = c * arr[i][x + win_size];      
		} 
	}  
	//normalizeData(xx,win_size);

	for(int y=-win_size; y<=win_size; y++)  
	{  
		double c = -((y - sigma2)/sigma4);  
		for(int i=0; i<win_size * 2 + 1; i++)  
		{  
			yy[y + win_size][i] = c * arr[y + win_size][i];               
		}  
	} 
	//normalizeData(yy,win_size);

	int width = src.cols;  
	int height = src.rows;  
 
	double xred = 0, xgreen = 0, xblue = 0;  
	double yred = 0, ygreen = 0, yblue = 0;  
	int newRow, newCol;  

		for(int row=0; row<height; row++) 
		{  
			int ta = 255, tr = 0, tg = 0, tb = 0;  
			for(int col=0; col<width; col++) 
			{  
 
				for(int subrow = -win_size; subrow <= win_size; subrow++) 
				{  
					for(int subcol = -win_size; subcol <= win_size; subcol++) 
					{  
						newRow = row + subrow;  
						newCol = col + subcol;  
						if(newRow < 0 || newRow >= height) 
						{  
							newRow = row;  
						}  
						if(newCol < 0 || newCol >= width) 
						{  
							newCol = col;  
						}  

						//tr = src_(newRow,newCol)[2];  
						//tg = src_(newRow,newCol)[1];  
						//tb = src_(newRow,newCol)[0];  
						tr = src.at<char>(newRow,newCol);
						xred += (yy[subrow + win_size][subcol + win_size] * tr);  
						//xgreen +=(x[subrow + win_size][subcol + win_size] * tg);  
						//xblue +=(x[subrow + win_size][subcol + win_size] * tb);  
					}  
				}  
				//int temp1 = xred * 10 + 50; 
				dst.at<char>(row,col) = xred;
				//int temp2 = xgreen*10 +50;
				//int temp3 = xblue*10 +50;
				//dst_(row,col)[2] = temp1 < 0 ? 0 : (temp1 > 255 ? 255 : temp1);
				//dst_(row,col)[1] = temp2 < 0 ? 0 : (temp2 > 255 ? 255 : temp2);
				//dst_(row,col)[0] = temp3 < 0 ? 0 : (temp3 > 255 ? 255 : temp3);

				newRow = newCol = 0;  
				xred = xgreen = xblue = 0;  
			}  
		}  

	namedWindow("hello2");
	imshow("hello2",dst);
	waitKey(0);

	for (int i = 0;i< 2*win_size+1;i++)
	{
		delete[] arr[i];
		delete[] x[i];
		delete[] y[i];
		delete[] xyDeviation[i];
		delete[] xx[i];
		delete[] yy[i];
	}
	delete[] arr;
	delete[] x;
	delete[] y;
	delete[] xyDeviation;
	delete[] xx;
	delete[] yy;

	return 0;
}