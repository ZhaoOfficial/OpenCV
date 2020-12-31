#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <vector>
#include <initializer_list>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using std::string;
using std::stringstream;
using std::vector;
using std::initializer_list;
using std::cin;
using std::cout;
using std::endl;

static void help();

Mat& ScanImageAndReduceC(Mat& I, const uchar* const table);
Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table);
Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar* const table);


int main(int argc, char* argv[])
{
    help();
    if (argc < 3)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }

    Mat I, J;
    if (argc == 4 && !strcmp(argv[3], "G"))
    {
        I = imread(argv[1], IMREAD_GRAYSCALE);
    }
    else
    {
        I = imread(argv[1], IMREAD_COLOR);
    }

    if (I.empty())
    {
        cout << "The image " << argv[1] << " could not be loaded." << endl;
        return -1;
    }


    //! [dividewith]
    // convert our input string to number - C++ style
    int divideWith = 0; 
    stringstream s;
    s << argv[2];
    s >> divideWith;
    if (!s || !divideWith)
    {
        cout << "Invalid number entered for dividing. " << endl;
        return -1;
    }


    // How to go through each and every pixel of an image ?
    // How are OpenCV matrix values stored ?
    // How to measure the performance of our algorithm ?
    // What are lookup tables and why use them ?
	//! color space reduction algorithm
    uchar table[256];
    for (int i = 0; i < 256; ++i)
        table[i] = (uchar)(divideWith * (i / divideWith));
    //! [dividewith]
    
    const int REPEAT_TIMES = 20;
    double t = 0.0;


    cout << "Scan image and reduce in C style.\n";
    t = (double)getTickCount();

    // cout << "Shape of I:" << I.size << endl;
    for (int i = 0; i < REPEAT_TIMES; ++i)
    {
        cv::Mat clone_i = I.clone();
        J = ScanImageAndReduceC(clone_i, table);
    }
    // cout << "Shape of J:" << J.size << endl;

    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Times passed: " << t << " s." << endl;


    cout << "Scan image and reduce with iterator.\n";
    t = (double)getTickCount();

    for (int i = 0; i < REPEAT_TIMES; ++i)
    {
        cv::Mat clone_i = I.clone();
        J = ScanImageAndReduceIterator(clone_i, table);
    }

    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Times passed: " << t << " s." << endl;


    cout << "Scan image and reduce with random access.\n";
    t = (double)getTickCount();

    for (int i = 0; i < REPEAT_TIMES; ++i)
    {
        cv::Mat clone_i = I.clone();
        J = ScanImageAndReduceRandomAccess(clone_i, table);
    }

    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Times passed: " << t << " s." << endl;

    // a multi-thread way
    //! [table-init]
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = table[i];
    //! [table-init]
    cout << "Scan image and reduce with look up table.\n";
    t = (double)getTickCount();

    for (int i = 0; i < REPEAT_TIMES; ++i)
    {
        LUT(I, lookUpTable, J);
    }

    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Times passed: " << t << " s." << endl;

	return 0;
}

static void help()
{
    cout
        << "--------------------------------------------------------------------------\n"
        << "This program shows how to scan image objects in OpenCV (cv::Mat).\n"
        << "As use case we take an input image and divide the native color palette (255) with the input.\n"
        << "Shows C operator[] method, iterators and at function for on-the-fly item address calculation.\n"
        << "Usage:" << endl
        << "./how_to_scan_images <imageNameToUse> <divideWith> [G]\n"
        << "if you add a G parameter the image is processed in gray scale.\n"
        << "--------------------------------------------------------------------------\n"
        << endl;
}

Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    int channels = I.channels();
    int nRows = I.rows;
    int nCols = I.cols * channels;
    // if Mat is continuous
    // then it stores continuously
    // otherwise, it stores row by row
    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    // reducing
    uchar* p;
    for (int i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for (int j = 0; j < nCols; ++j)
        {
            p[j] = table[p[j]];
        }
    }
    return I;
}

Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);
    const int channels = I.channels();
    switch (channels)
    {
        case 1:
        {
            MatIterator_<uchar> it, end;
            for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
                *it = table[*it];
            break;
        }
        case 3:
        {
            MatIterator_<Vec3b> it, end;
            for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
            {
                (*it)[0] = table[(*it)[0]];
                (*it)[1] = table[(*it)[1]];
                (*it)[2] = table[(*it)[2]];
            }
        }
    }
    return I;
}

Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);
    const int channels = I.channels();
    switch (channels)
    {
        case 1:
        {
            for (int i = 0; i < I.rows; ++i)
                for (int j = 0; j < I.cols; ++j)
                    I.at<uchar>(i, j) = table[I.at<uchar>(i, j)];
            break;
        }
        case 3:
        {
            Mat_<Vec3b> _I = I;
            for (int i = 0; i < I.rows; ++i)
                for (int j = 0; j < I.cols; ++j)
                {
                    _I(i, j)[0] = table[_I(i, j)[0]];
                    _I(i, j)[1] = table[_I(i, j)[1]];
                    _I(i, j)[2] = table[_I(i, j)[2]];
                }
            I = _I;
            break;
        }
    }
    return I;
}
