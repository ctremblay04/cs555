#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	// Load two copies of the image. One to leave as the original, and one to be modified.
	Mat originalImage = imread("input.tif");
	Mat modifiedImage = imread("input.tif");

	// Create a pointer so that we can quickly toggle between which image is being displayed
	Mat *image = &originalImage;

	// Check if the images loaded
	if (originalImage.empty() || modifiedImage.empty())
	{
		cout << "fail to load image !" << endl;
		return -1;
	}

	// Replace center third of the image with white
	// This can be replaced with whatever filtering you need to do.
	modifiedImage(Range(image->rows / 3, image->rows / 3 * 2), Range(image->cols / 3, image->cols / 3 * 2)) = (255, 255, 255);

	// Create a black window to show the letters
	Mat letters = Mat::zeros(image->rows, image->cols, CV_8UC1);
	putText(letters, "Hello OpenCV",
		Point(20, 50),
		FONT_HERSHEY_COMPLEX, 1, // font face and scale
		Scalar(255, 255, 255), // white
		1, LINE_AA); // line thickness and type
	image = &letters;

	// Output the modified image
	imwrite("output.tif", modifiedImage);

	//Create the display window
	namedWindow("opencv sample", CV_WINDOW_AUTOSIZE);

	//Display loop
	while (true)
	{
		imshow("opencv sample", *image);
		switch (cvWaitKey(15))
		{
		case 27:  //Exit display loop if ESC is pressed
			return 0;
		case 32:  //Swap image pointer if space is pressed
			if (image == &letters) image = &originalImage;
			else if (image == &originalImage) image = &modifiedImage;
			else image = &letters;
			break;
		default:
			break;
		}
	}
}

