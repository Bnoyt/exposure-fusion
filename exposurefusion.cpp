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
		src_B.push_back(bgr[0]);
		src_G.push_back(bgr[1]);
		src_R.push_back(bgr[2]);
	}
	
	
	/*
	int m = weights[0].rows;
	int n = weights[0].cols;

	cout << m << " " << n << endl;

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

	
	vector<Mat> finalLaplacianPyramid;  //L{R} in the text
	vector<vector<Mat> > laplacianPyramids;  //L{I} in the text
	vector<vector<Mat> > gaussianPyramids;  //G{W} in the text
	

	// Initialize the Gaussian pyramid of weights
	
	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		vector<Mat> gaussianPyramid;
		computeGaussianPyramid(weights[picture], gaussianPyramid);
		gaussianPyramids.push_back(gaussianPyramid);
	}

	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		vector<Mat> laplacianPyramid;
		computeLaplacianPyramid(weights[picture], laplacianPyramid);
		laplacianPyramids.push_back(laplacianPyramid);
	}

	*/

	/*
	for (int k = 0; k < (int) log2(min(m, n)); k++) {
		Mat M1, M2, M3;
		pyrDown(gaussianPyramids[0][k], M1);
		pyrDown(gaussianPyramids[1][k], M2);
		pyrDown(gaussianPyramids[2][k], M3);
		gaussianPyramids[0].push_back(M1);
		gaussianPyramids[1].push_back(M2);
		gaussianPyramids[2].push_back(M3);
		size_m.push_back(M1.rows);
		size_n.push_back(M2.cols);

		Mat N1, N2, N3;
		Size size = Size(size_n[k], size_m[k]);

		pyrUp(laplacian_b_1, N1, size);
		pyrUp(laplacian_b_1, N1, size);
		pyrUp(laplacian_b_1, N1, size);

		cout << "Pyramid up done" << endl;
		Mat O = Mat(size_m[k], size_n[k], CV_32F);
		for (int i = 0; i < size_m[k]; i++) {
			for (int j = 0; j < size_n[k]; j++) {
				O.at<float>(i, j) = gaussianPyramid[k].at<float>(i, j) - N.at<float>(i, j);
			}
		}
		laplacianPyramid.push_back(O);
	}
	*/



	waitKey(0);


    return 0;
}
