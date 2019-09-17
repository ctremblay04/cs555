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

// I hate c++. This would be so much easier in python.
Mat GrayImageOperations :: fillThreeLargestRegions(const Mat& orig) {
  struct P {
    vector<int> filled;
    P(vector<int>& f) { filled = f; }
    bool operator<(P other) const { return filled.size() > other.filled.size(); }
  };
  Mat thresholded = getThresholded(orig);
  Mat ret = thresholded.clone();
  vector<vector<int>> regions;
  size_t size = thresholded.rows * thresholded.step;
  bool checked[thresholded.rows*thresholded.cols] = {false};
  
  // BFS to discover regions, not ideal code however this is the only way that I could think
  // to implement without defining an interface for getting pixel values for Mat. 
  for (int i = 0; i < size; i+=3) {
    if (checked[i/3] or thresholded.data[i] == 0) { continue; }
    stack<int> queue;
    queue.push(i);
    checked[i/3] = true;
    vector<int> filled;
    filled.push_back(i);
    while (not queue.empty()) {
      int curr = queue.top();
      queue.pop();
      int left = curr-3;
      int right = curr+3;
      int up = curr-thresholded.step;
      int down = curr+thresholded.step;
      if (left/thresholded.step == curr/thresholded.step and thresholded.data[left] != 0 and not checked[left/3]) {
        checked[left/3] = true;
        queue.push(left);
        filled.push_back(left);
      }
      if (right/thresholded.step == curr/thresholded.step and thresholded.data[right] != 0 and not checked[right/3]) {
        checked[right/3] = true;
        queue.push(right);
        filled.push_back(right);
      }
      if (up > 0 and thresholded.data[up] != 0 and not checked[up/3]) {
        checked[up/3] = true;
        queue.push(up);
        filled.push_back(up);
      }
      if (down < size and thresholded.data[down] != 0 and not checked[down/3]) {
        checked[down/3] = true;
        queue.push(down);
        filled.push_back(down);
      }
    }
    regions.push_back(filled);
  }
  sort(regions.begin(), regions.end(), [](vector<int>& lhs, vector<int>& rhs)->bool {return lhs.size() > rhs.size();});
  int c = 0; // Count of the largest regions
  for (auto i = regions.begin(); i != regions.end(); ++i) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
      if (c == 0) { ret.data[(*j)] = 34; ret.data[(*j)+1] = 58; ret.data[(*j)+2] = 216; }
      else if (c == 1) { ret.data[(*j)] = 34; ret.data[(*j)+1] = 216; ret.data[(*j)+2] = 88; }
      else if (c == 2) { ret.data[(*j)] = 216; ret.data[(*j)+1] = 34; ret.data[(*j)+2] = 34; }
      else { ret.data[(*j)] = 0; ret.data[(*j)+1] = 0; ret.data[(*j)+2] = 0;}  
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

