#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "GrayImageOperations.h"

using namespace cv;
using namespace std;

string
Problem1() {
  string ret = "";
  Mat houseOrig = imread("TestingImages3/House_width_times4.bmp");
  Mat houseNeg = GrayImageOperations::getNegative(houseOrig);
  Mat houseEq = GrayImageOperations::getEqualized(houseOrig);
  Mat nycOrig = imread("TestingImages3/NYC_width_4times.bmp");
  Mat nycEq = GrayImageOperations::getEqualized(nycOrig);

  imwrite("houseNeg.bmp", houseNeg);
  imwrite("houseEq.bmp", houseEq);
  imwrite("nycEq.bmp", nycEq);
  
  ret += "House original histogram:\n"+GrayImageOperations::getHistogramStr(houseOrig)+"\n";
  ret += "\nHouse negative histogram:\n"+GrayImageOperations::getHistogramStr(houseNeg)+"\n";
  ret += "\nHouse equalzed histogram:\n"+GrayImageOperations::getHistogramStr(houseEq)+"\n";
  ret += "\nNYC original histogram:\n"+GrayImageOperations::getHistogramStr(nycOrig)+"\n";
  ret += "\nNYC equalized histogram:\n"+GrayImageOperations::getHistogramStr(nycEq)+"\n";
  
  return ret;
}

string
Problem2() {
  string ret = "";
  Mat shapesOrig = imread("TestingImages3/shapes2.1.bmp");
  Mat shapesThreshold = GrayImageOperations::getThresholded(shapesOrig);
  Mat shapesLabeledReigons = GrayImageOperations::fillThreeLargestRegions(shapesOrig);
  string shapesThresholdVal = to_string(GrayImageOperations::getThresholdInt(shapesOrig));
  Mat hitchhikeOrig = imread("TestingImages3/guide_8bits.bmp");
  Mat hitchhikeThreshold = GrayImageOperations::getThresholded(hitchhikeOrig);
  Mat hitchhikeLabeledReigons = GrayImageOperations::fillThreeLargestRegions(hitchhikeOrig);
  string hitchhikeThresholdVal = to_string(GrayImageOperations::getThresholdInt(hitchhikeOrig));
  imwrite("shapesBinary.bmp", shapesThreshold);
  imwrite("hitchhikeBinary.bmp", hitchhikeThreshold);
  imwrite("shapesLabeled.bmp", shapesLabeledReigons);
  imwrite("hitchhikeLabeled.bmp", hitchhikeLabeledReigons);

  ret += "\nShapes threshold gray value: "+shapesThresholdVal+"\n";
  ret += "\nHitchhikers threshold gray value: "+hitchhikeThresholdVal+"\n";
  
  return ret;
}

int 
main(void)
{
  ofstream myfile;
  myfile.open("output.txt");
  string writeToFile = "";
  writeToFile += Problem1();
  writeToFile += Problem2();
  myfile << writeToFile;
  myfile.close();
  return 0;
}
