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

static void help(char* progName)
{
    cout
        << "This program shows how to filter images with mask:\n"
        << "the write it yourself and the filter2d way.\n"
        << "Usage:\n"
        << progName << " [image path][G]\n" << endl;
}

void Sharpen(const Mat& Image, Mat& Result);

int main(int argc, char* argv[])
{
    help(argv[0]);
    Mat src, dst0, dst1;

    if (argc == 3 && !strcmp("G", argv[2]))
    {
        src = imread(argv[1], IMREAD_GRAYSCALE);
    }
    else
    {
        src = imread(argv[1], IMREAD_COLOR);
    }

    if (src.empty())
    {
        cout << "Can't open image [" << argv[1] << "]" << endl;
        return -1;
    }


    namedWindow("Input", WINDOW_AUTOSIZE);
    namedWindow("Hand written", WINDOW_AUTOSIZE);
    namedWindow("Build in", WINDOW_AUTOSIZE);

    imshow("Input", src);
    double t = (double)getTickCount();

    Sharpen(src, dst0);

    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Hand written function time passed: " << t << " s." << endl;

    imshow("Hand written", dst0);
    waitKey(0);

    Mat kernel = (Mat_<char>(3, 3) << 
        0, -1, 0,
        -1, 5, -1,
        0, -1, 0);
    t = (double)getTickCount();
    filter2D(src, dst1, src.depth(), kernel);
    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Built-in filter2D time passed: " << t << " s." << endl;
    imshow("Build in", dst1);
    waitKey();

    return 0;
}


void Sharpen(const Mat& Image, Mat& Result)
{
    //! [8_bit]
    CV_Assert(Image.depth() == CV_8U);  // accept only uchar images
    //! [8_bit]

    //! [create_channels]
    const int nChannels = Image.channels();
    Result.create(Image.size(), Image.type());
    //! [create_channels]

    //! [basic_method_loop]
    for (int j = 1; j < Image.rows - 1; ++j)
    {
        const uchar* previous = Image.ptr<uchar>(j - 1);
        const uchar* current = Image.ptr<uchar>(j);
        const uchar* next = Image.ptr<uchar>(j + 1);

        uchar* output = Result.ptr<uchar>(j);

        for (int k = nChannels; k < nChannels * (Image.cols - 1); ++k)
        {
            *output++ = saturate_cast<uchar>(5 * current[k]
                - current[k - nChannels] - current[k + nChannels] - previous[k] - next[k]);
        }
    }
    //! [basic_method_loop]

    //! [borders]
    Result.row(0).setTo(Scalar(0));
    Result.row(Result.rows - 1).setTo(Scalar(0));
    Result.col(0).setTo(Scalar(0));
    Result.col(Result.cols - 1).setTo(Scalar(0));
    //! [borders]
}
//! [basic_method]