#pragma once

#include <opencv2\opencv.hpp>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

class Image
{
public:
	static void imadjust(const Mat1b& src, Mat1b& dst, int tol, Vec2i in, Vec2i out);
};