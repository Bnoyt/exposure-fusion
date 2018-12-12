#include "pyramids.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<vector>
#include <fstream>
#include <cmath>
#include "image.h"

using namespace cv;
using namespace std;


void computeGaussianPyramid(Mat& originalPicture, vector<Mat>& gaussianPyramid) {
	int m = originalPicture.rows;
	int n = originalPicture.cols;
	gaussianPyramid.push_back(originalPicture);
	Mat* largePicture = new Mat();
	Mat* smallPicture = new Mat();
	*largePicture = originalPicture;
	for (int k = 0; k < (int)log2(min(m, n)); k++) {
		pyrDown(*largePicture, *smallPicture);
		gaussianPyramid.push_back(*smallPicture);
		*largePicture = *smallPicture;
	}
}

void computeLaplacianPyramid(Mat& originalPicture, vector<Mat>& laplacianPyramid) {
	int m = originalPicture.rows;
	int n = originalPicture.cols;
	int new_m, new_n;
	Mat bigReconstruction, difference;
	Mat* largePicture = new Mat();
	*largePicture = originalPicture;
	Mat* smallPicture = new Mat();
	for (int k = 0; k < (int)log2(min(m, n)); k++) {
		pyrDown(*largePicture, *smallPicture);
		new_m = (*largePicture).rows;
		new_n = (*largePicture).cols;
		Size dstsize = Size(new_n, new_m);
		pyrUp(*smallPicture, bigReconstruction, dstsize);
		difference = Mat(new_m, new_n, CV_32F);
		for (int i = 0; i < new_m; i++) {
			for (int j = 0; j < new_n; j++) {
				difference.at<float>(i, j) = (*largePicture).at<float>(i, j) - bigReconstruction.at<float>(i, j);
			}
		}
		laplacianPyramid.push_back(difference);
		*largePicture = *smallPicture;
	}
	laplacianPyramid.push_back(*largePicture);
}

void reconstructPictureWithLaplacianPyramid(vector<Mat>&laplacianPyramid, Mat& finalPicture) {
	int N = (int)laplacianPyramid.size();
	Mat* largePicture = new Mat();
	Mat* smallPicture = new Mat();
	Mat bigReconstruction, difference;
	int new_m, new_n;
	*smallPicture = laplacianPyramid[N-1];
	for (int k = N - 2; k > -1; k--) {
		difference = laplacianPyramid[k];
		new_m = difference.rows;
		new_n = difference.cols;
		Size dstsize = Size(new_n, new_m);
		pyrUp(*smallPicture, bigReconstruction, dstsize);
		*largePicture = Mat(new_m, new_n, CV_32F);
		for (int i = 0; i < new_m; i++) {
			for (int j = 0; j < new_n; j++) {
				(*largePicture).at<float>(i, j) = bigReconstruction.at<float>(i, j) + difference.at<float>(i, j);
			}
		}
		*smallPicture = *largePicture;
	}
	finalPicture = *largePicture;
}