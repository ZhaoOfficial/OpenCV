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
const string samplePath = "D:/Mywork/OpenCV/SampleImage/";

int main(int argc, char* argv[])
{
    Mat image = imread(samplePath + "lena.jpg");
    if (image.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    Mat new_image = Mat::zeros(image.size(), image.type());
    double alpha = 1.0; /*< Simple contrast control */
    int beta = 0;       /*< Simple brightness control */
    cout << "Basic Linear Transforms " << endl;
    cout << "-------------------------" << endl;
    cout << "* Enter the alpha value [1.0-3.0]: ";
    cin >> alpha;
    cout << "* Enter the beta value [0-100]: ";
    cin >> beta;


    double t = (double)getTickCount();
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                new_image.at<Vec3b>(y, x)[c] =
                    saturate_cast<uchar>(alpha * image.at<Vec3b>(y, x)[c] + beta);
            }
        }
    }
    t = ((double)getTickCount() - t) / (double)getTickFrequency();
    cout << "Time of transforms with random access: " << t << " s." << endl;
    imshow("Original Image", image);
    imshow("New Image", new_image);
    waitKey(0);


    t = (double)getTickCount();
    MatIterator_<Vec3b> it, end, nit;
    for (it = image.begin<Vec3b>(), end = image.end<Vec3b>(), nit = new_image.begin<Vec3b>(); it != end; ++it, ++nit)
    {
        (*nit)[0] = saturate_cast<uchar>(alpha * (*it)[0] + beta);
        (*nit)[1] = saturate_cast<uchar>(alpha * (*it)[1] + beta);
        (*nit)[2] = saturate_cast<uchar>(alpha * (*it)[2] + beta);
    }
    t = ((double)getTickCount() - t) / (double)getTickFrequency();
    cout << "Time of transforms with iterator: " << t << " s." << endl;
    imshow("Original Image", image);
    imshow("New Image", new_image);
    waitKey(0);


    t = (double)getTickCount();
    int channels = image.channels();
    int nRows = image.rows;
    int nCols = image.cols * channels;
    if (image.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }
    uchar* p;
    uchar* q;
    for (int i = 0; i < nRows; ++i)
    {
        p = image.ptr<uchar>(i);
        q = new_image.ptr<uchar>(i);
        for (int j = 0; j < nCols; ++j)
        {
            q[j] = saturate_cast<uchar>(alpha * p[j] + beta);
        }
    }
    t = ((double)getTickCount() - t) / (double)getTickFrequency();
    cout << "Time of transforms C: " << t << " s." << endl;
    imshow("Original Image", image);
    imshow("New Image", new_image);
    waitKey(0);


    t = (double)getTickCount();
    // -1 if it should be the same as the source type
    image.convertTo(new_image, -1, alpha, beta);
    t = ((double)getTickCount() - t) / (double)getTickFrequency();
    cout << "Time of transforms with build in function: " << t << " s." << endl;
    imshow("Original Image", image);
    imshow("New Image", new_image);

    // using LUT
    double gamma = 0.4;
    Mat lookUpTable(1, 256, CV_8U);
    uchar* lut = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        lut[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    Mat res = image.clone();
    LUT(image, lookUpTable, new_image);
    imshow("Original Image", image);
    imshow("New Image", new_image);
    waitKey(0);

    return 0;
}