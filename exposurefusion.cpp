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

	const int NUMBER_OF_PICTURES = 16;

	vector<Mat> src_color;
	vector<Mat> src_B;
	vector<Mat> src_G;
	vector<Mat> src_R;
	vector<Mat> weights;

	
	cout << "Loading the pictures..." << endl;
	
	src_color.push_back(imread("../memorial0061.jpg"));
	src_color.push_back(imread("../memorial0062.jpg"));
	src_color.push_back(imread("../memorial0063.jpg"));
	src_color.push_back(imread("../memorial0064.jpg"));
	src_color.push_back(imread("../memorial0065.jpg"));
	src_color.push_back(imread("../memorial0066.jpg"));
	src_color.push_back(imread("../memorial0067.jpg"));
	src_color.push_back(imread("../memorial0068.jpg"));
	src_color.push_back(imread("../memorial0069.jpg"));
	src_color.push_back(imread("../memorial0070.jpg"));
	src_color.push_back(imread("../memorial0071.jpg"));
	src_color.push_back(imread("../memorial0072.jpg"));
	src_color.push_back(imread("../memorial0073.jpg"));
	src_color.push_back(imread("../memorial0074.jpg"));
	src_color.push_back(imread("../memorial0075.jpg"));
	src_color.push_back(imread("../memorial0076.jpg"));
	/*
	src_color.push_back(imread("../image1.jpg"));
	src_color.push_back(imread("../image2.jpg"));
	src_color.push_back(imread("../image3.jpg"));
	*/
	cout << "Done." << endl;


	Mat weight;

	cout << "Computing src_B, src_G, src_R..." << endl;
	
	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		//cout << "A" << endl;
		compute_Weigth_Mat(src_color[picture], weight);
		//cout << "B" << endl;
		weights.push_back(weight);
		//cout << "C" << endl;
		Mat bgr[3];
		split(src_color[picture], bgr);
		bgr[0].convertTo(bgr[0], CV_32F);
		bgr[1].convertTo(bgr[1], CV_32F);
		bgr[2].convertTo(bgr[2], CV_32F);
		src_B.push_back(bgr[0]);
		src_G.push_back(bgr[1]);
		src_R.push_back(bgr[2]);
	}
	cout << "Done." << endl;
	
	
	int m = weights[0].rows;
	int n = weights[0].cols;


	cout << "Computing weights..." << endl;
	
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			float w = 0;
			for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
				w += weights[picture].at<float>(i, j);
			}
			for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
				weights[picture].at<float>(i, j) = weights[picture].at<float>(i, j) / w;
			}
		}
	}
	cout << "Done." << endl;


	
	vector<vector<Mat> > blueLaplacianPyramids;  //L{I} in the text
	vector<vector<Mat> > greenLaplacianPyramids;  //L{I} in the text
	vector<vector<Mat> > redLaplacianPyramids;  //L{I} in the text
	vector<vector<Mat> > gaussianPyramids;  //G{W} in the text
	

	// Initialize the Gaussian pyramid of weights
	
	cout << "Computing Gaussian pyramid of weigths..." << endl;
	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		vector<Mat> gaussianPyramid;
		computeGaussianPyramid(weights[picture], gaussianPyramid);
		gaussianPyramids.push_back(gaussianPyramid);
	}
	cout << "Done." << endl;

	// Initialize the Laplacian pyramids of the 3 colors pictures

	cout << "Computing Laplacian pyramids of the 3 colors pictures..." << endl;

	for (int picture = 0; picture < NUMBER_OF_PICTURES; picture++) {
		vector<Mat> blueLaplacianPyramid, greenLaplacianPyramid, redLaplacianPyramid;

		computeLaplacianPyramid(src_B[picture], blueLaplacianPyramid);
		blueLaplacianPyramids.push_back(blueLaplacianPyramid);

		computeLaplacianPyramid(src_G[picture], greenLaplacianPyramid);
		greenLaplacianPyramids.push_back(greenLaplacianPyramid);

		computeLaplacianPyramid(src_R[picture], redLaplacianPyramid);
		redLaplacianPyramids.push_back(redLaplacianPyramid);
	}

	cout << "Done." << endl;

	vector<Mat> blueFinalLaplacianPyramid;  //L{R} in the text
	vector<Mat> greenFinalLaplacianPyramid;  //L{R} in the text
	vector<Mat> redFinalLaplacianPyramid;  //L{R} in the text

	int L = blueLaplacianPyramids[0].size();

	cout << "Computing final Laplacian pyramids for each of the 3 colors..." << endl;

	for (int l = 0; l < L; l++) {
		int m = blueLaplacianPyramids[0][l].rows;
		int n = blueLaplacianPyramids[0][l].cols;

		Mat bluePicture = Mat(m, n, CV_32F);
		Mat greenPicture = Mat(m, n, CV_32F);
		Mat redPicture = Mat(m, n, CV_32F);
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				bluePicture.at<float>(i, j) = 0.;
				greenPicture.at<float>(i, j) = 0.;
				redPicture.at<float>(i, j) = 0.;
				for (int k = 0; k < NUMBER_OF_PICTURES; k++) {
					bluePicture.at<float>(i, j) += gaussianPyramids[k][l].at<float>(i, j) * blueLaplacianPyramids[k][l].at<float>(i, j);
					greenPicture.at<float>(i, j) += gaussianPyramids[k][l].at<float>(i, j) * greenLaplacianPyramids[k][l].at<float>(i, j);
					redPicture.at<float>(i, j) += gaussianPyramids[k][l].at<float>(i, j) * redLaplacianPyramids[k][l].at<float>(i, j);
				}
			}
		}
		blueFinalLaplacianPyramid.push_back(bluePicture);
		greenFinalLaplacianPyramid.push_back(greenPicture);
		redFinalLaplacianPyramid.push_back(redPicture);
	}

	cout << "Done." << endl;

	cout << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 1].rows << " " << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 1].cols << endl;
	cout << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 2].rows << " " << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 2].cols << endl;

	cout << endl << "Blue" << endl;

	cout << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 1].at<float>(0, 0) << " " << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 1].at<float>(0, 1) << endl;
	cout << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 1].at<float>(1, 0) << " " << blueFinalLaplacianPyramid[blueFinalLaplacianPyramid.size() - 1].at<float>(1, 1) << endl;

	cout << endl << "Green" << endl;
	cout << greenFinalLaplacianPyramid[greenFinalLaplacianPyramid.size() - 1].at<float>(0, 0) << " " << greenFinalLaplacianPyramid[greenFinalLaplacianPyramid.size() - 1].at<float>(0, 1) << endl;
	cout << greenFinalLaplacianPyramid[greenFinalLaplacianPyramid.size() - 1].at<float>(1, 0) << " " << greenFinalLaplacianPyramid[greenFinalLaplacianPyramid.size() - 1].at<float>(1, 1) << endl;

	
	Mat blueFinalPicture, greenFinalPicture, redFinalPicture;
	reconstructPictureWithLaplacianPyramid(blueFinalLaplacianPyramid, blueFinalPicture);
	reconstructPictureWithLaplacianPyramid(greenFinalLaplacianPyramid, greenFinalPicture);
	reconstructPictureWithLaplacianPyramid(redFinalLaplacianPyramid, redFinalPicture);

	/*
	cout << endl;
	cout << "Blue " << blueFinalPicture.rows << " " << blueFinalPicture.cols << endl;
	cout << "Green " << greenFinalPicture.rows << " " << greenFinalPicture.cols << endl;
	cout << "Red " << redFinalPicture.rows << " " << redFinalPicture.cols << endl;
	cout << "Picture " << src_color_1.rows << " " << src_color_1.cols << endl;
	*/
	
	Mat finalPicture = src_color[0];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			finalPicture.at<Vec3b>(i, j)[0] = (uchar)blueFinalPicture.at<float>(i, j);
			finalPicture.at<Vec3b>(i, j)[1] = (uchar)greenFinalPicture.at<float>(i, j);
			finalPicture.at<Vec3b>(i, j)[2] = (uchar)redFinalPicture.at<float>(i, j);
		}
	}

	imshow("Final Picture", finalPicture);
	imwrite("FinalPicture.png", finalPicture);
	
	


	/*
	cout << endl << "Color intensities in (100, 20):" << endl;
	cout << "Blue: " << blueFinalPicture.at<float>(100, 20) << endl;
	cout << "Green: " << greenFinalPicture.at<float>(100, 20) << endl;
	cout << "Red: " << redFinalPicture.at<float>(100, 20) << endl;
	
	cout << endl << (int)src_color_3.at<Vec3b>(100, 20)[0] << " " << src_color_3.at<Vec3b>(100, 20) << endl;


	cout << endl << "Program over" << endl;
	*/

	printMinSat();
	printMaxSat();

	waitKey(0);


    return 0;
}
