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
	string filename = "D:/Mywork/OpenCV/SampleImage/palace.jpg";

    // Input/Output
    {
        //! [Load an image from a file]
        Mat img = imread(filename);
        if (img.empty())
        {
            cout << "Could not open " << filename << endl;
            return -1;
        }
        imshow("palace", img);
        waitKey(0);
        //! [Load an image from a file]
        CV_UNUSED(img);
    }
    {
        //! [Load an image from a file in grayscale]
        Mat img = imread(filename, IMREAD_GRAYSCALE);
        imshow("palace", img);
        waitKey(0);
        //! [Load an image from a file in grayscale]
        CV_UNUSED(img);
    }
    {
        // Mat img(4, 4, CV_8U);
        //! [Save image]
        // imwrite(filename, img);
        //! [Save image]
    }
    // Accessing pixel intensity values
    {
        Mat img(4, 4, CV_8UC3);
        cout << "img:\n" << img << endl;
        int y = 0, x = 0;
        {
            //! [Pixel access 1]
            Scalar intensity = img.at<uchar>(y, x);
            cout << "Intensity: " << intensity << endl;
            //! [Pixel access 1]
            CV_UNUSED(intensity);
        }
        {
            //! [Pixel access 2]
            Scalar intensity = img.at<uchar>(Point(x, y));
            cout << "Intensity: " << intensity << endl;
            //! [Pixel access 2]
            CV_UNUSED(intensity);
        }
        {
            //! [Pixel access 3]
            Vec3b intensity = img.at<Vec3b>(y, x);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];
            cout << "BGR: " << (int)blue << (int)green << (int)red << endl;
            //! [Pixel access 3]
            CV_UNUSED(blue);
            CV_UNUSED(green);
            CV_UNUSED(red);
        }
        {
            //! [Pixel access 5]
            img.at<uchar>(y, x) = 128;
            cout << "img:\n" << img << endl;
            //! [Pixel access 5]
        }
        {
            int i = 0;
            //! [Mat from points vector]
            vector<Point2d> points;
            for (int j = 0; j < 8; j++)
            {
                points.push_back(Point2d(j, 0));
            }
            Mat pointsMat = Mat(points);
            //! [Mat from points vector]

            //! [Point access]
            Point2d point = pointsMat.at<Point2d>(i, 0);
            cout << "point:\n" << point << endl;
            //! [Point access]
            CV_UNUSED(point);
        }
    }
    // Memory management and reference counting
    {
        //! [Reference counting 1]
        std::vector<Point3f> points;
        for (int i = 0; i < 8; i++) {
            points.push_back(Point3f(i, 2 * i, 3 * i));
        }
        Mat pointsMat = Mat(points).reshape(1);
        cout << "pointsMat:\n" << pointsMat << endl;
        //! [Reference counting 1]
        CV_UNUSED(pointsMat);
    }
    {
        //! [Reference counting 2]
        Mat img = imread(filename);
        Mat img1 = img.clone();
        //! [Reference counting 2]
        CV_UNUSED(img1);
    }
    {
        //! [Reference counting 3]
        Mat img = imread(filename);
        Mat sobelx;
        Sobel(img, sobelx, CV_32F, 1, 0);
        imshow("sobelx", sobelx);
        waitKey(0);
        //! [Reference counting 3]
    }
    // Primitive operations
    {
        Mat img = imread(filename);
        {
            //! [Set image to black]
            //! set all the value to be 0
            img = Scalar(0);
            //! [Set image to black]
        }
        {
            //! [Select ROI]
            Rect r(10, 10, 100, 100);
            Mat smallImg = img(r);
            imshow("smallImg", smallImg);
            waitKey(0);
            //! [Select ROI]
            CV_UNUSED(smallImg);
        }
    }
    {
        //! [BGR to Gray]
        Mat img = imread(filename); // loading a 8UC3 image
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        //! [BGR to Gray]
    }
    {
        Mat dst, src;
        //! [Convert to CV_32F]
        src.convertTo(dst, CV_32F);
        //! [Convert to CV_32F]
    }
    // Visualizing images
    {
        //! [imshow 1]
        Mat img = imread(filename);
        namedWindow("image", WINDOW_AUTOSIZE);
        imshow("image", img);
        waitKey(0);
        //! [imshow 1]
    }
    {
        //! [imshow 2]
        Mat img = imread(filename);
        Mat grey;
        cvtColor(img, grey, COLOR_BGR2GRAY);
        Mat sobelx;
        Sobel(grey, sobelx, CV_32F, 1, 0);

        //find minimum and maximum intensities
        double minVal, maxVal;
        minMaxLoc(sobelx, &minVal, &maxVal);

        Mat draw;
        sobelx.convertTo(draw, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
        namedWindow("image", WINDOW_AUTOSIZE);
        imshow("image", draw);
        waitKey();
        //! [imshow 2]
    }

    return 0;
}