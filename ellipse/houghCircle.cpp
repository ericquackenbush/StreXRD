#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

float verifyCircle(cv::Mat dt, cv::Point2f center, float radius, std::vector<cv::Point2f> & inlierSet)
{
 unsigned int counter = 0;
 unsigned int inlier = 0;
 float minInlierDist = 2.0f;
 float maxInlierDistMax = 100.0f;
 float maxInlierDist = radius/25.0f;
 if(maxInlierDist<minInlierDist) maxInlierDist = minInlierDist;
 if(maxInlierDist>maxInlierDistMax) maxInlierDist = maxInlierDistMax;

 // choose samples along the circle and count inlier percentage
 for(float t =0; t<2*3.14159265359f; t+= 0.05f)
 {
     counter++;
     float cX = radius*cos(t) + center.x;
     float cY = radius*sin(t) + center.y;

     if(cX < dt.cols)
     if(cX >= 0)
     if(cY < dt.rows)
     if(cY >= 0)
     if(dt.at<float>(cY,cX) < maxInlierDist)
     {
        inlier++;
        inlierSet.push_back(cv::Point2f(cX,cY));
     }
 }

 return (float)inlier/float(counter);
}


inline void getCircle(cv::Point2f& p1,cv::Point2f& p2,cv::Point2f& p3, cv::Point2f& center, float& radius)
{
  float x1 = p1.x;
  float x2 = p2.x;
  float x3 = p3.x;

  float y1 = p1.y;
  float y2 = p2.y;
  float y3 = p3.y;

  // PLEASE CHECK FOR TYPOS IN THE FORMULA :)
  center.x = (x1*x1+y1*y1)*(y2-y3) + (x2*x2+y2*y2)*(y3-y1) + (x3*x3+y3*y3)*(y1-y2);
  center.x /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

  center.y = (x1*x1 + y1*y1)*(x3-x2) + (x2*x2+y2*y2)*(x1-x3) + (x3*x3 + y3*y3)*(x2-x1);
  center.y /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

  radius = sqrt((center.x-x1)*(center.x-x1) + (center.y-y1)*(center.y-y1));
}



std::vector<cv::Point2f> getPointPositions(cv::Mat binaryImage)
{
 std::vector<cv::Point2f> pointPositions;

 for(unsigned int y=0; y<binaryImage.rows; ++y)
 {
     //unsigned char* rowPtr = binaryImage.ptr<unsigned char>(y);
     for(unsigned int x=0; x<binaryImage.cols; ++x)
     {
         //if(rowPtr[x] > 0) pointPositions.push_back(cv::Point2i(x,y));
         if(binaryImage.at<unsigned char>(y,x) > 0) pointPositions.push_back(cv::Point2f(x,y));
     }
 }

 return pointPositions;
}

int main()
{
    //cv::Mat color = cv::imread("../inputData/semi_circle_contrast.png");
    cv::Mat color = cv::imread("test.png");
    cv::Mat gray;

    // convert to grayscale
    cv::cvtColor(color, gray, CV_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    // now map brightest pixel to 255 and smalles pixel val to 0. this is for easier finding of threshold
    double min, max;
    cv::minMaxLoc(gray,&min,&max);
    float sub = min;
    float mult = 255.0f/(float)(max-sub);
    cv::Mat normalized = gray - sub;
    normalized = mult * normalized;
    //cv::namedWindow("normalized", CV_WINDOW_NORMAL);
    //cv::imshow("normalized" , normalized);
    //--------------------------------


    // now compute threshold
    // TODO: this might ne a tricky task if noise differs...
    cv::Mat mask;
    //cv::threshold(input, mask, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cv::threshold(normalized, mask, 100, 255, CV_THRESH_BINARY);



    std::vector<cv::Point2f> edgePositions;
    edgePositions = getPointPositions(mask);

    // create distance transform to efficiently evaluate distance to nearest edge
    cv::Mat dt;
    cv::distanceTransform(255-mask, dt,CV_DIST_L1, 3);

    //TODO: maybe seed random variable for real random numbers.

    unsigned int nIterations = 0;

    cv::Point2f bestCircleCenter;
    float bestCircleRadius;
    float bestCirclePercentage = 0;
    float minRadius = 50;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion

    //float minCirclePercentage = 0.2f;
    float minCirclePercentage = 0.05f;  // at least 5% of a circle must be present? maybe more...

    int maxNrOfIterations = edgePositions.size();   // TODO: adjust this parameter or include some real ransac criteria with inlier/outlier percentages to decide when to stop

    for(unsigned int its=0; its< maxNrOfIterations; ++its)
    {
        //RANSAC: randomly choose 3 point and create a circle:
        //TODO: choose randomly but more intelligent, 
        //so that it is more likely to choose three points of a circle. 
        //For example if there are many small circles, it is unlikely to randomly choose 3 points of the same circle.
        unsigned int idx1 = rand()%edgePositions.size();
        unsigned int idx2 = rand()%edgePositions.size();
        unsigned int idx3 = rand()%edgePositions.size();

        // we need 3 different samples:
        if(idx1 == idx2) continue;
        if(idx1 == idx3) continue;
        if(idx3 == idx2) continue;

        // create circle from 3 points:
        cv::Point2f center; float radius;
        getCircle(edgePositions[idx1],edgePositions[idx2],edgePositions[idx3],center,radius);

        // inlier set unused at the moment but could be used to approximate a (more robust) circle from alle inlier
        std::vector<cv::Point2f> inlierSet;

        //verify or falsify the circle by inlier counting:
        float cPerc = verifyCircle(dt,center,radius, inlierSet);

        // update best circle information if necessary
        if(cPerc >= bestCirclePercentage)
            if(radius >= minRadius)
        {
            bestCirclePercentage = cPerc;
            bestCircleRadius = radius;
            bestCircleCenter = center;
        }

    }

    // draw if good circle was found
    if(bestCirclePercentage >= minCirclePercentage)
        if(bestCircleRadius >= minRadius);
	cv::cvtColor(gray,color, CV_GRAY2RGB);
	int thickness = 2;
	int lineType = 8;
        cv::circle(color, bestCircleCenter,bestCircleRadius, cv::Scalar(0,0,255),thickness, lineType);

	std::cout << "center: " << bestCircleCenter << " radius: " <<
	bestCircleRadius << std::endl;


    	cv::namedWindow("output", CV_WINDOW_NORMAL);
        cv::imshow("output",color);
	//cv::namedWindow("mask", CV_WINDOW_NORMAL);
        //cv::imshow("mask",mask);
        cv::waitKey(0);

        return 0;
}
