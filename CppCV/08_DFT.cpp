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

static void help(char** argv)
{
    cout 
        << "This program demonstrated the use of the discrete Fourier transform (DFT).\n"
        << "The dft of an image is taken and it's power spectrum is displayed.\n"
        << "Usage:\n"
        << argv[0] << " [image_name]\n" << endl;
}

int main(int argc, char** argv)
{
    help(argv);
    string filename = (argc >= 2) ? argv[1] : (samplePath + "lena.jpg");
    Mat I = imread(filename, IMREAD_GRAYSCALE);
    if (I.empty())
    {
        cout << "Could not open image." << endl;
        return EXIT_FAILURE;
    }

    //expand input image to optimal size
    Mat padded;
    // on the border add zero values
    int m = getOptimalDFTSize(I.rows);
    int n = getOptimalDFTSize(I.cols);

    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
    // real and image parts of an image
    // 2 * (n * m) -> n * m * 2
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);

    // this way the result may fit in the source matrix
    dft(complexI, complexI);

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    split(complexI, planes);
    // planes[0] = magnitude
    magnitude(planes[0], planes[1], planes[0]);
    Mat magI = planes[0];
    // switch to logarithmic scale
    magI += Scalar::all(1);
    log(magI, magI);


    // crop the spectrum, if it has an odd number of rows or columns
    // -2 == 0b...1110
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    // Top-Left - Create a ROI per quadrant
    Mat q0(magI, Rect(0, 0, cx, cy));
    // Top-Right
    Mat q1(magI, Rect(cx, 0, cx, cy));
    // Bottom-Left
    Mat q2(magI, Rect(0, cy, cx, cy));
    // Bottom-Right
    Mat q3(magI, Rect(cx, cy, cx, cy));
    Mat tmp; 
    // swap quadrants (Top-Left q0 with Bottom-Right q3)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    // swap quadrant (Top-Right q1 with Bottom-Left q2)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
    // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    normalize(magI, magI, 0, 1, NORM_MINMAX);
    
    imshow("Input Image", I);
    imshow("spectrum magnitude", magI);
    waitKey(0);
    return EXIT_SUCCESS;
}