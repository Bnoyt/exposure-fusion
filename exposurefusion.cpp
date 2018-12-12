#include "image.h"
#include "pyramids.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<vector>
#include <fstream>
#include <cmath>


using namespace cv;
using namespace std;



int main(int argc, char** argv){

	const int NUMBER_OF_PICTURES = 3;

	vector<Mat> src_color;
	vector<Mat> src_B;
	vector<Mat> src_G;
	vector<Mat> src_R;
	vector<Mat> weights;

	Mat src_color_1 = imread("../image1.jpg");
	Mat src_color_2 = imread("../image2.jpg");
	Mat src_color_3 = imread("../image3.jpg");
	src_color.push_back(src_color_1);
	src_color.push_back(src_color_2);
	src_color.push_back(src_color_3);

	Mat weight;
	
	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		compute_Weigth_Mat(src_color[picture], weight);
		weights.push_back(weight);
		Mat bgr[3];
		split(src_color[picture], bgr);
		bgr[0].convertTo(bgr[0], CV_32F);
		bgr[1].convertTo(bgr[1], CV_32F);
		bgr[2].convertTo(bgr[2], CV_32F);
		src_B.push_back(bgr[0]);
		src_G.push_back(bgr[1]);
		src_R.push_back(bgr[2]);
	}
	
	
	int m = weights[0].rows;
	int n = weights[0].cols;

	
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			double w = 0;
			for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
				w += weights[picture].at<float>(i, j);
			}
			for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
				weights[picture].at<float>(i, j) = weights[picture].at<float>(i, j) / w;
			}
		}
	}


	
	vector<vector<Mat> > blueLaplacianPyramids;  //L{I} in the text
	vector<vector<Mat> > greenLaplacianPyramids;  //L{I} in the text
	vector<vector<Mat> > redLaplacianPyramids;  //L{I} in the text
	vector<vector<Mat> > gaussianPyramids;  //G{W} in the text
	

	// Initialize the Gaussian pyramid of weights
	
	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		vector<Mat> gaussianPyramid;
		computeGaussianPyramid(weights[picture], gaussianPyramid);
		gaussianPyramids.push_back(gaussianPyramid);
	}

	// Initialize the Laplacian pyramid of the 3 colors pictures

	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		vector<Mat> laplacianPyramid;
		computeLaplacianPyramid(src_B[picture], laplacianPyramid);
		blueLaplacianPyramids.push_back(laplacianPyramid);
		computeLaplacianPyramid(src_G[picture], laplacianPyramid);
		greenLaplacianPyramids.push_back(laplacianPyramid);
		computeLaplacianPyramid(src_R[picture], laplacianPyramid);
		redLaplacianPyramids.push_back(laplacianPyramid);
	}

	vector<Mat> blueFinalLaplacianPyramid;  //L{R} in the text
	vector<Mat> greenFinalLaplacianPyramid;  //L{R} in the text
	vector<Mat> redFinalLaplacianPyramid;  //L{R} in the text




	



	waitKey(0);


    return 0;
}
