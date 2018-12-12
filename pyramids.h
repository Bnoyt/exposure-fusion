#pragma once

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void computeGaussianPyramid(Mat& originalPicture, vector<Mat>& gaussianPyramid);
void computeLaplacianPyramid(Mat& originalPicture, vector<Mat>& laplacianPyramid);
void reconstructPictureWithLaplacianPyramid(vector<Mat>&laplacianPyramid, Mat& finalPicture);