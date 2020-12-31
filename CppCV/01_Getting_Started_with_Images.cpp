#include <iostream>
#include <cstring>
#include <cmath>
#include <direct.h>

// here defines the basic building blocks of the library
#include <opencv2/core.hpp>
// here provides functions for reading and writing
#include <opencv2/imgcodecs.hpp>
// here contains the functions to show an image in a window
#include <opencv2/highgui.hpp>

using namespace cv;
using std::string;
using std::cin;
using std::cout;
using std::endl;



int main()
{
	// using sample image provided by cv2
	// string image_path = samples::findFile("starry_night.jpg");

	// note here we use an absolute path
	string image_path = "D:/Mywork/OpenCV/SampleImage/palace.jpg";

	// IMREAD_COLOR loads the image in the BGR 8-bit format. This is the default that is used here.
	// IMREAD_UNCHANGED loads the image as is (including the alpha channel if present)
	// IMREAD_GRAYSCALE loads the image as an intensity one
	// Mat = numpy.ndarray
	Mat image = imread(image_path, IMREAD_COLOR);

	if (image.empty())
	{
		cout << "Could not read the image: " << image_path << endl;
		return 1;
	}

	imshow("Sample image", image);

	// wait for key down
	int k = waitKey(0);

	// wait 2000 ms
	// waitKey(2000);

	if (k == 'q')
	{
		cout << "Close sample image" << endl;
		destroyWindow("Sample image");
	}
	return 0;
}
