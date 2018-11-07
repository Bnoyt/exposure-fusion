#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "image.h"

int main(int argc, char** argv){
	Mat I = imread("../road.jpg");
	cout << I.at<Vec3b>(2,2);
	Mat Ihsv;
	cvtColor(I, Ihsv, CV_BGR2HSV);
	cout << Ihsv.at<Vec3b>(2, 2);
    return 0;
}