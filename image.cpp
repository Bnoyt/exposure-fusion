#include "image.h"
#include <iostream>
#include <cmath>
// Correlation
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

//Saturation

void Saturation(const Mat& Ic, Mat& S){
	Mat I;
	cvtColor(Ic, I, CV_BGR2HSV);
	int m = I.rows;
	int n = I.cols;
	S = Mat(m, n, CV_32F);

	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			S.at<float>(i,j) = (float)I.at<Vec3b>(i,j)[1];
		}
	}
}

//Laplacian

/*
void Laplacian(const Mat&F, Mat& L) { 
	// F is a CV_32F grayscale image
	int m = F.rows, n = F.cols;
	L = Mat(m, n, CV_32F);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
				L.at<float>(i, j) = 0;
			}
			else {
				L.at<float>(i, j) = abs(F.at<float>(i-1, j)+ F.at<float>(i+1, j)+ F.at<float>(i, j-1)+ F.at<float>(i, j+1) -4*F.at<float>(i, j));
			}
		}
	}
}
*/

// Well-exposedness

void WellExposedness(const Mat&Ic, Mat& E){
	float sigma = 0.2;
	Mat I, F;
	cvtColor(Ic, I, CV_BGR2GRAY);
	I.convertTo(F, CV_32F);
	int m = I.rows, n = I.cols;
	E = Mat(m, n, CV_32F);
	for(int i = 0;i < m; i++){
		for(int j=0;j<n;j++){
			E.at<float>(i,j) = exp(-(F.at<float>(i,j)/255 - 0.5)*(F.at<float>(i,j)/255 - 0.5)/(2*sigma*sigma));
		}
	}

}

double a = .4;
double b = .25;
double c = .25 - a / 2;

double w(int n) {
	if (n == 0) return a;
	if (n == 1 || n==-1) return b;
	return c;
}
double w(int n, int m) {
	return w(n)*w(m);
}

void reduceImage(const Mat& F, Mat& F2) {
	int m = F.rows / 2;
	int n = F.cols / 2;
	F2 = Mat(m, n, CV_8U);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (i < 2 || i>m-2 || j < 2 || j > n - 2) {
				F2.at<uchar>(i, j) = 0;
			}
			else {
				double value = 0.;
				for (int m = -2; m <= 2; m++){
					for (int n = -2; n <= 2; n++) {
						value += w(n, m) * (int) F.at<uchar>(2 * i + m, 2 * j + n);
						F2.at<uchar>(i, j) = (char)value;
					}
				}
			}
		}
	}
}