#include "image.h"

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

void Saturation(const Mat& Ic, Mat& L){
	Mat I;
	cvtColor(Ic, I, CV_BGR2HSV);
	Mat Res(m, n, CV_32F)
	int m = I.rows;
	int n = I.cols;
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			Res.at<float>(i,j) = I.at<vec3b>(i,j)[1];
		}
	}
	return Res;
//Laplacian
void Laplacian(const Mat&Ic, Mat& L) { 
	// Ic is not a necessarly grayscale version of the picture
	Mat I;
	cvtColor(Ic, I, CV_BGR2GRAY);
	int m = I.rows, n = I.cols;
	L = Mat(m, n, CV_32F);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
				L.at<float>(i, j) = 0;
			}
			else {
				float value = 0;
				for (int ivar = -1; ivar < 2; ivar++) {
					for (int jvar = -1; jvar < 2; jvar++) {
						value += I.at<float>(i + ivar, j + jvar) - I.at<float>(i, j);
					}
				}
				L.at<float>(i, j) = value;
				
			}
		}
	}

