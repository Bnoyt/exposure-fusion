#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "image.h"
using namespace cv;
using namespace std;







int main(int argc, char** argv){
	
	Mat Ires;
	vector<Mat> v =  vector<Mat>();
	v.push_back(imread("../grandcanal_under.jpg"));
	v.push_back(imread("../grandcanal_over.jpg"));
	v.push_back(imread("../grandcanal_mean.jpg"));
	calcul_mat(v,Ires);
	imshow("under",v.at(0));
	imshow("over",v.at(1));
	imshow("mean",v.at(2));
	imshow("res",Ires);
	waitKey(0);
    return 0;
}