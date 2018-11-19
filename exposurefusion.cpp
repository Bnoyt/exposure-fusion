#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "image.h"


int main(int argc, char** argv){
	Mat I1 = imread("../image1.jpg");
	Mat Ires = imread("../image1.jpg");

	
	vector<Mat> gaussianPyramid;
	gaussianPyramid.push_back(I1);
	Mat M1 = I1;
	Mat M2;
	for (int i = 0; i < (int)log2(fmin(I1.cols, I1.rows)); i++) {
		pyrDown(M1, M2);
		M1 = M2;
		gaussianPyramid.push_back(M2);
	} 
	cout << "Size: " << gaussianPyramid.at(gaussianPyramid.size() - 1).size << endl;
	Mat M3;
	imshow("A", gaussianPyramid.at(1));
	imshow("B", I1);
	vector<Mat> laplacianPyramid;
	laplacianPyramid.push_back(gaussianPyramid.at(gaussianPyramid.size()-1));
	/*
	for (int i = (int)log2(fmin(I1.cols, I1.rows)) - 1; i>0; i--) {
		cout << i << endl;
		pyrUp(gaussianPyramid.at(i), M1);
		subtract(gaussianPyramid.at(i - 1), M1, M1);
		laplacianPyramid.push_back(M1);
	}
	*/

	cout << gaussianPyramid.at(gaussianPyramid.size() - 1).size << endl;
	pyrUp(gaussianPyramid.at(gaussianPyramid.size() - 1), M1);
	cout << gaussianPyramid.at(gaussianPyramid.size() - 2).size << endl;
	cout << M1.size << endl;

	

	waitKey(0);
    return 0;
}