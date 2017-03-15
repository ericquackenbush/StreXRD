/* Copyright 2017 Eric Quackenbush */

#define _GLIBCXX_USE_CXX11_ABI 0
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

class ImageObject {
	std::string filename;
	cv::Mat inputImage;
	cv::Mat outputImage;
public:
	cv::Mat get_input_image();
	cv::Mat get_output_image();
	void get_image_type();
	void display_image(int, int);
	void load_file(std::string);
	void weak_peaks();
	void fully_automatic();
};