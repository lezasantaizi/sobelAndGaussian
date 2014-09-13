#include "GaussianDerivation.h"

#pragma comment(lib, "opencv_highgui249d.lib")
#pragma comment(lib, "opencv_imgproc249d.lib")
#pragma comment(lib, "opencv_core249d.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
#pragma comment(lib, "opencv_core249.lib")



int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "hello3.jpg";
	Mat src = imread(filename,1);

	if(src.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}
	Mat dst;

	namedWindow("hello");
	imshow("hello",src);

	src.copyTo(dst);
	int width = src.cols;  
	int height = src.rows;  
 
	GaussianDerivation gd;
	gd.processderivation(width,height,src,dst,YDEVIATION);

	namedWindow("hello2");
	imshow("hello2",dst);
	waitKey(0);

	return 0;
}