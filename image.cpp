#include "image.h"
#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>

// Correlation
using namespace cv;


double mean(const Image<float>& I,Point m,int n) {
	double s=0;
	for (int j=-n;j<=n;j++)
		for (int i=-n;i<=n;i++)
			s+=I(m+Point(i,j));
	return s/(2*n+1)/(2*n+1);
}

double corr(const Image<float>& I1,Point m1,const Image<float>& I2,Point m2,int n) {
	double M1=mean(I1,m1,n);
	double M2=mean(I2,m2,n);
	double rho=0;
	for (int j=-n;j<=n;j++)
		for (int i=-n;i<=n;i++) {
			rho+=(I1(m1+Point(i,j))-M1)*(I2(m2+Point(i,j))-M2);
		}
		return rho;
}

double NCC(const Image<float>& I1,Point m1,const Image<float>& I2,Point m2,int n) {
	if (m1.x<n || m1.x>=I1.width()-n || m1.y<n || m1.y>=I1.height()-n) return -1;
	if (m2.x<n || m2.x>=I2.width()-n || m2.y<n || m2.y>=I2.height()-n) return -1;
	double c1=corr(I1,m1,I1,m1,n);
	if (c1==0) return -1;
	double c2=corr(I2,m2,I2,m2,n);
	if (c2==0) return -1;
	return corr(I1,m1,I2,m2,n)/sqrt(c1*c2);
}

// Laplacian
void Laplacian(const Mat&src_gray, Mat& posL) {
	int m = src_gray.rows;
	int n = src_gray.cols;
	Mat L;
	Laplacian(src_gray, L, CV_8U, 3);
	convertScaleAbs(L, L);
	posL = Mat(m, n, CV_32F);
	for (int i = 0;i < m;i++) {
		for (int j = 0;j < n;j++) {
			posL.at<float>(i, j) = ((float) L.at<uchar>(i,j)) + 0.01;
		}
	}
}

double minSat = 256.;
double maxSat = 0.;

//Saturation
void Saturation(const Mat& src_color, Mat& S){
	Mat I;
	cvtColor(src_color, I, CV_BGR2HSV);
	int m = I.rows;
	int n = I.cols;
	S = Mat(m, n, CV_32F);
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			S.at<float>(i, j) = (float)I.at<Vec3b>(i, j)[1] + 0.1;
		}
	}
}

// Well-exposedness
void WellExposedness(const Mat&src_gray, Mat& E){
	float sigma = 0.2;
	int m = src_gray.rows, n = src_gray.cols;
	E = Mat(m, n, CV_32F);
	for(int i = 0;i < m; i++){
		for(int j=0;j<n;j++){
			E.at<float>(i,j) = exp(-((float)src_gray.at<char>(i,j)/255. - 0.5)*((float)src_gray.at<char>(i,j)/255. - 0.5)/(2*sigma*sigma));
		}
	}

}
void printMinSat() {
	cout << minSat << endl;
}

void printMaxSat() {
	cout << maxSat << endl;
}

const double alphaS = 1.;
const double alphaE = 1.;
const double alphaL = 1.;

void compute_Weigth_Mat(Mat& src_color, Mat &W) {
	Mat L, S, E;
	Mat src_gray, F;
	cvtColor(src_color, src_gray, CV_RGB2GRAY);
	Laplacian(src_gray, L);
	Saturation(src_color, S);
	WellExposedness(src_gray, E);
	int m = src_gray.rows, n = src_gray.cols;
	W = Mat(m, n, CV_32F);
	for (int i = 0;i < m; i++) {
		for (int j = 0;j < n;j++) {
			W.at<float>(i, j) = L.at<float>(i,j) * S.at<float>(i,j) * E.at<float>(i,j);
			//W.at<float>(i, j) = S.at<float>(i, j) * E.at<float>(i, j);
		}
	}
}

