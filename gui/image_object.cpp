/* Copyright 2017 Eric Quackenbush */

#include <string>
#include <algorithm>
#include <vector>
#include <random>
#include <numeric>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_object.h>
#include <read_mar.h>

using myType = uint32_t;

cv::Mat ImageObject::get_input_image()
{
	cv::Mat original_image;
	inputImage.convertTo(original_image, CV_8UC1);
	return original_image;
}

cv::Mat ImageObject::get_output_image()
{
	return outputImage;
}

void ImageObject::load_file(std::string filename)
{
	if (filename.substr(filename.find_last_of(".") + 1) == "mar3450")
	{
		ReadMar new_file(filename);
		std::vector<int> output = new_file.read_file();
		int size = new_file.get_image_size();
		inputImage = cv::Mat(size, size, CV_32SC1);
		memcpy(inputImage.data, output.data(), output.size() * sizeof(int));
	}
	else if (filename.substr(filename.find_last_of(".") + 1) == "png")
	{
		inputImage = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	}
	else
	{
		inputImage = cv::imread(filename, CV_LOAD_IMAGE_ANYDEPTH);
	}
	outputImage = inputImage;

}

void ImageObject::weak_peaks()
{
	int width = inputImage.cols;
	int height = inputImage.rows;
	int numPts = 100;
	int radius = 20;
	int tol = 1;
	double min, max;
	int min_pos = 0;
	int max_pos = 0;

	std::vector<int> x(numPts);
	std::vector<int> y(numPts);
	std::vector<double> avg(numPts);
	std::vector<double> sd(numPts);

	std::random_device rd;
	std::mt19937_64 generator( rd() );
	std::uniform_int_distribution<> dist( 1, width - 1 );
	for (int i = 0; i < numPts; ++i)
	{
		x[i] = dist(generator);
		y[i] = dist(generator);

	}


	for (int i = 0; i < numPts; ++i)
	{
		std::vector<int> values;

		cv::Mat mask = cv::Mat::zeros(height, width, CV_8UC1);
		cv::Point center = cv::Point(x[i], y[i]);
		cv::circle(mask, center, radius, cv::Scalar(255), -1);

		for (unsigned int ix = 0; ix < height; ++ix)
		{
			for (unsigned int iy = 0; iy < width; ++iy)
			{
				if (mask.at<uchar>(iy, ix) > 0)
				{
					values.push_back(inputImage.at<int>(iy, ix));
				}
			}
		}


		// perform average
		double sum = std::accumulate(values.begin(), values.end(), 0.0);
		avg[i] = sum / values.size();

		// find standard deviation
		double sq_sum = std::inner_product(values.begin(), values.end(), values.begin(), 0.0);
		sd[i] = std::sqrt(sq_sum / values.size() - avg[i] * avg[i]);

		values.clear();
	}

	// find minimum and maximum values and their respective positions
	for (unsigned i = 0; i < avg.size(); ++i)
	{
		if (avg[i] < avg[min_pos])
			min_pos = i;
		if (avg[i] > avg[max_pos])
			max_pos = i;
	}

	min = avg[min_pos] - 3 * sd[min_pos];
	max = avg[max_pos] + 5 * sd[max_pos];

	qDebug() << "min: " << min << " " << "max: " << max << "\n";

	cv::normalize(inputImage, outputImage, 0, 500, cv::NORM_MINMAX, CV_32SC1);
	outputImage.convertTo(outputImage, CV_8UC1);
}

void ImageObject::fully_automatic()
{
	inputImage.convertTo(outputImage, CV_8UC1);
	cv::equalizeHist(outputImage, outputImage);
}

void ImageObject::display_image(int a, int b)
{
	cv::Size size(a,b);
	resize(outputImage,outputImage,size);

	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
	cv::imshow( "Display window", outputImage );
	cv::waitKey(0);
}

void ImageObject::get_image_type()
{
	std::string r;
	int type = inputImage.type();

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	printf("Matrix: %s %dx%d \n", r.c_str(), inputImage.cols, inputImage.rows );
}

void ImageObject::adjust_brightness(int beta)
{
	cv::Mat temp;
	outputImage.convertTo(outputImage, CV_8UC1);
	temp = outputImage + cv::Scalar(beta);
	outputImage = temp;
}

void ImageObject::adjust_contrast(double alpha)
{
	cv::Mat temp;
	outputImage.convertTo(outputImage, CV_8UC1);
	outputImage.convertTo(temp, -1, alpha, 0);
	outputImage = temp;
}