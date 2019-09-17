#include "GrayImageOperations.h"


// Return negative image 
Mat GrayImageOperations :: getNegative(const Mat& orig) {
  Mat ret = orig.clone();
  size_t size = orig.rows * orig.step;
  for(size_t i = 0; i < size; i++) {
    ret.data[i] = (GRAY_SHADES-1)-ret.data[i];
  }
  return ret;
}

// Returns equalized image, stretching image based on CDF
Mat GrayImageOperations :: getEqualized(const Mat& orig) {
  Mat ret = orig.clone();
  int* hist = getHistogram(orig);
  size_t size = orig.rows * orig.step;

  // Calculate the CDF and cumulative sum of histogram
  int sum[GRAY_SHADES] = {0};
  int sFunc[GRAY_SHADES] = {0};
  sum[0] = hist[0];
  sFunc[0] = sum[0]/(GRAY_SHADES)-1;
  
  // Populate sFunc array
  for (int i = 1; i < GRAY_SHADES; i++) {
    sum[i] = sum[i-1]+hist[i];
    sFunc[i] = ((GRAY_SHADES-1)*sum[i])/(size);
  }
  
  // Populate output image with histogram equalized image
  for(size_t i = 0; i < size; i++) {
    ret.data[i] = sFunc[orig.data[i]];
  }
  delete[] hist;
  return ret;
}

// Get Image after performing thresholding operation
Mat GrayImageOperations :: getThresholded(const Mat& orig) {
  Mat ret = orig.clone();
  int threshold = getThresholdInt(orig);
  size_t size = orig.rows * orig.step;
  for(size_t i = 0; i < size; i++) {
    if (ret.data[i] <= threshold) { ret.data[i] = 0; }
    else { ret.data[i] = GRAY_SHADES-1; }
  }
  return ret;
}


Mat GrayImageOperations :: fillThreeLargestRegions(const Mat& orig) {
  
  Mat thresholded = getThresholded(orig);
  Mat ret = thresholded.clone();
  vector<vector<Pixel>> regions;
  int rows = thresholded.rows;
  int cols = thresholded.cols;
  bool checked[rows][cols] = {false};
  
  // DFS to discover regions, not ideal code however this is the only way that I could think
  // to implement without defining an interface for getting pixel values for Mat. 
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      Pixel pixel = Pixel(i,j,rows,cols);
      if (checked[pixel.x][pixel.y] or thresholded.data[pixel.getMatVal()] == 0) { continue; }
      checked[pixel.x][pixel.y] = true;
      stack<Pixel> queue;
      vector<Pixel> filled;
      queue.push(pixel);
      filled.push_back(pixel);
      while (not queue.empty()) { // DFS on successful pixel
        Pixel curr = queue.top();
        queue.pop();
        int arr[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
        for (auto p : arr) {
          Pixel adj = Pixel(curr.x+p[0], curr.y+p[1], rows, cols);
          if (not adj.validate() or checked[adj.x][adj.y] or thresholded.data[adj.getMatVal()] == 0) { continue; }
          checked[adj.x][adj.y] = true;
          filled.push_back(adj);
          queue.push(adj);
        }
      }
      regions.push_back(filled);
    }
  }
  // Sort regions based on size
  sort(regions.begin(), regions.end(), [](vector<Pixel>& lhs, vector<Pixel>& rhs)->bool {return lhs.size() > rhs.size();});
  int c = 0; // Count of the largest regions
  for (auto i = regions.begin(); i != regions.end(); ++i) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
      int val = (*j).getMatVal();
      if (c == 0) { ret.data[val] = 34; ret.data[val+1] = 58; ret.data[val+2] = 216; }
      else if (c == 1) { ret.data[val] = 34; ret.data[val+1] = 216; ret.data[val+2] = 88; }
      else if (c == 2) { ret.data[val] = 216; ret.data[val+1] = 34; ret.data[val+2] = 34; }
      else { ret.data[val] = 0; ret.data[val+1] = 0; ret.data[val+2] = 0;}  
    }
    if (c < 3) { c++; }
  }
  return ret;
}


// Balanced Histogram Thresholding, motivated from the Balanced histogram thresholding wikipedia page
// Returns thresholded Mat image
int GrayImageOperations :: getThresholdInt(const Mat& orig) {
  int* hist = getHistogram(orig);
  int left = 0;
  int right = GRAY_SHADES-1;
  int mid = left/2;
  int sum[GRAY_SHADES] = {0};
  sum[0] = hist[0];
  for (int i = 1; i < GRAY_SHADES; i++) { sum[i] = sum[i-1]+hist[i]; }
  int w_left = sum[mid];
  int w_right = sum[right]-sum[mid];
  while (left <= right) {
    if (w_right > w_left) {
      w_right -= hist[right--];
      if (left+(right-left)/2 < mid) {
        w_right += hist[mid];
        w_left -= hist[mid--];
      }
    }
    else {
      w_left -= hist[left++];
      if (left+(right-left)/2 >= mid) {
        w_right -= hist[++mid];
        w_left += hist[mid];
      }
    }
  }
  delete[] hist;
  return mid;
}

// Return integer array which is histogram
int* GrayImageOperations :: getHistogram(const Mat& orig) {
  int* ret = new int[GRAY_SHADES];
  for(int i = 0; i < GRAY_SHADES; i++) { ret[i] = 0; }
  size_t size = orig.rows * orig.step;
  for(size_t i = 0; i < size; i++) {
    ret[orig.data[i]]++; }
  return ret;
}

// Return string form of histogram, in CSV format
string GrayImageOperations :: getHistogramStr(const Mat& orig) {
  int* arr = getHistogram(orig); 
  string s = "";
  for (int i = 0; i < GRAY_SHADES; i++) {
    s += to_string(arr[i]);
    if (i != GRAY_SHADES-1) { s += ","; }
  }
  delete[] arr;
  return s;
}

