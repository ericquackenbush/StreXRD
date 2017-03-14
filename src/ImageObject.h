/*
load_file: opens up a file and creates a MAT object from it
adjust_brightness
adjust_contrast
weak_peaks
fully_automatic
*/

#include <opencv2\opencv.hpp>

class ImageObject {
	String filename;
public:
	cv::Mat inputImage;		// original image
	cv::Mat outputImage;	// displayed image
	void load_file(String);
	void adjust_brightness(int);
	void adjust_contrast(double);
	void weak_peaks();
	void fully_automatic();
}