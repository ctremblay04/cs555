#ifndef GRAY_IMAGE_OP
#define GRAY_IMAGE_OP

#include <opencv2/opencv.hpp>
#include <vector>
#include <stack>

using namespace cv;
using namespace std;

const unsigned int GRAY_SHADES = 1<<8;

class GrayImageOperations
{
  public:
    static Mat getNegative(const Mat&);
    static Mat getEqualized(const Mat&);
    static Mat getThresholded(const Mat&);
    static Mat fillThreeLargestRegions(const Mat& orig);
    static int getThresholdInt(const Mat&);
    static int* getHistogram(const Mat&);
    static string getHistogramStr(const Mat& orig);
  private:
    struct Pixel { 
      int x,y,row,col;
      Pixel(int a, int b, int r, int c) : x(a), y(b), row(r), col(c) {};
      int getMatVal() { return 3*(x*col+y); }
      bool validate() { return x > -1 and y > -1 and x < row and y < col; };
    };
    GrayImageOperations() {};
};

#endif
