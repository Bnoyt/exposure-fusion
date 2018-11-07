#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "image.h"

int main(int argc, char** argv){
	Mat I = imread("../road.jpg");
	Mat Ig;
	Mat Ihsv;
	cvtColor(I, Ihsv, CV_BGR2HSV);
	cvtColor(I, Ig, CV_BGR2GRAY);
	Mat S, L;
	Laplacian(I,L);
	Saturation(I, S);
	cout << L.at<float>(2, 2)<<endl;
	cout << S.at<float>(2, 2) << endl;
    return 0;
}