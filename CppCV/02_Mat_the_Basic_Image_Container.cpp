#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <initializer_list>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using std::string;
using std::vector;
using std::initializer_list;
using std::cin;
using std::cout;
using std::endl;

int main()
{
	// here A, B, C, D, E use the same (part of) data.
	// creates just the header parts
	Mat A = imread("D:/Mywork/OpenCV/test.jpg", IMREAD_COLOR);
	// Use the copy constructor
	Mat B(A);
	// Assignment operator
	Mat C = A;
	Mat D(A, Rect(160, 120, 320, 240));
	Mat E = A(Range::all(), Range(160, 480));

	// imshow("A", A);
	// imshow("B", B);
	// imshow("C", C);
	// imshow("D", D);
	// imshow("E", E);
	// waitKey(0);

	// here F, G use the copied data.
	Mat F = A.clone();
	Mat G;
	A.copyTo(G);


	// CV_[The number of bits per item][Signed or Unsigned][Type Prefix]C[The channel number]
	Mat M(2, 2, CV_8UC3, (0, 0, 255));
	// cout can only be used for 2D mat
	cout << "M = \n" << M << endl << endl;


	int size[3] = { 2, 2, 2 };
	// it can not be output by cout
	// dim, size of each dim, channels, values
	Mat L(3, size, CV_8UC(1), Scalar::all(0));


	Mat N;
	N.create(4, 4, CV_8UC2);
	cout << "N = \n" << N << endl << endl;


	// Diagonal
	Mat Eye = Mat::eye(4, 6, CV_64F);
	cout << "Eye = \n" << Eye << endl << endl;
	// all one matrix
	Mat One = Mat::ones(2, 2, CV_32F);
	cout << "One = \n" << One << endl << endl;
	// all zero matrix
	Mat Zero = Mat::zeros(3, 3, CV_8UC1);
	cout << "Zero = \n" << Zero << endl << endl;


	// initializer list
	Mat Init = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	cout << "Init = \n" << Init << endl << endl;
	Init = (Mat_<double>({ 0, -1, 0, -1, 5, -1, 0, -1, 0 })).reshape(3);
	cout << "Init = \n" << Init << endl << endl;
	Init = (Mat_<double>(initializer_list<double>({ 0, -1, 0, -1, 5, -1, 0, -1, 0 }))).reshape(3);
	cout << "Init = \n" << Init << endl << endl;


	// clone of row or col
	Mat RowClone = Init.row(1).clone();
	cout << "RowClone = \n" << RowClone << endl << endl;


	// random value
	Mat R = Mat(3, 2, CV_8UC3);
	randu(R, Scalar::all(0), Scalar::all(255));
	// format output
	cout << "R (default) = \n" << R << endl << endl;
	cout << "R (python)  = \n" << format(R, Formatter::FMT_PYTHON) << endl << endl;
	cout << "R (csv)     = \n" << format(R, Formatter::FMT_CSV) << endl << endl;
	cout << "R (numpy)   = \n" << format(R, Formatter::FMT_NUMPY) << endl << endl;
	cout << "R (c)       = \n" << format(R, Formatter::FMT_C) << endl << endl;


	// other CV data structure
	Point2f P2f(5, 1);
	cout << "Point (2D) = \n" << P2f << endl << endl;
	Point3f P3f(2, 6, 7);
	cout << "Point (3D) = \n" << P3f << endl << endl;

	vector<float> v;
	v.push_back((float)CV_PI);
	v.push_back(2);
	v.push_back(3.01f);
	cout << "Vector of floats via Mat = \n" << Mat(v) << endl << endl;

	vector<Point2f> vPoints(20);
	for (size_t i = 0; i < vPoints.size(); ++i)
		vPoints[i] = Point2f((float)(i * 5), (float)(i % 7));
	cout << "A vector of 2D Points = \n" << vPoints << endl << endl;

	return 0;
}