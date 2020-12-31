#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <vector>
#include <initializer_list>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using std::string;
using std::stringstream;
using std::vector;
using std::initializer_list;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	double alpha = 0.5;
	double beta = 0.0;
	double gamma = 1.0;

	Mat src1, src2, dst;
	string filePath = "D:/Mywork/OpenCV/SampleImage/";

	cout << " Simple Linear Blender " << endl;
	cout << "-----------------------" << endl;
	cout << "* Enter alpha [0.0-1.0]: ";
	double input;
	cin >> input;
	// We use the alpha provided by the user if it is between 0 and 1
	if (input >= 0 && input <= 1)
	{
		alpha = input;
	}

	src1 = imread(filePath + "LinuxLogo.jpg");
	src2 = imread(filePath + "WindowsLogo.jpg");
	if (src1.empty()) 
	{
		cout << "Error loading src1" << endl;
		return EXIT_FAILURE;
	}
	if (src2.empty()) 
	{
		cout << "Error loading src2" << endl;
		return EXIT_FAILURE;
	}
	cout << "Size of LinuxLogo.jpg: " << src1.size << endl;
	cout << "Size of WindowsLogo.jpg: " << src2.size << endl;

	beta = (1.0 - alpha);
	addWeighted(src1, alpha, src2, beta, gamma, dst);
	imshow("Linear Blend", dst);
	waitKey(0);
	return 0;
}