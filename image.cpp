#include "image.h"
#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
// Correlation
using namespace cv;


const double alphaE = 0.5;
const double alphaL = 0.5;
const double alphaS = 0.5;


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



void calcul_mat(vector<Mat> v,Mat &Ires){
	vector<Mat> S, L, E, Pond;
	int m = v.at(0).rows, n = v.at(0).cols;
	for (vector<Mat>::iterator it = v.begin() ; it != v.end(); ++it){
		Mat s,l,e,p;
		Laplacian(*it,l);
		Saturation(*it,s);
		WellExposedness(*it,e);

		S.push_back(s);
		E.push_back(e);
		L.push_back(l);

		p = Mat(m, n, CV_32F);


		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (abs(l.at<float>(i,j)) == numeric_limits<float>::infinity()) l.at<float>(i,j) = 1000000000000000;
				p.at<float>(i,j) = pow(e.at<float>(i,j),alphaE) + pow(s.at<float>(i,j),alphaS) + 0.1*log(pow(l.at<float>(i,j),alphaL))  ;
				cout << log(pow(l.at<float>(i,j),alphaL)) << endl;
				if (log(pow(l.at<float>(i,j),alphaL)) > 100.) cout << pow(l.at<float>(i,j),alphaL) << endl;

			}
		}
		Pond.push_back(p);


	}

	Mat kaz = imread("../grandcanal_under.jpg");

	Ires = kaz;

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {

			double pondsum = 0;

			Ires.at<Vec3b>(i, j)[0] = 0.;
			Ires.at<Vec3b>(i, j)[1] = 0.;
			Ires.at<Vec3b>(i, j)[2] = 0.;

			for (int k = 0;k<v.size();k++){
				pondsum = pondsum + Pond.at(k).at<float>(i,j);
			}

			for (int k = 0;k<v.size();k++){
				pondsum = pondsum + Pond.at(k).at<float>(i,j);
				Ires.at<Vec3b>(i, j)[0] = Ires.at<Vec3b>(i, j)[0] + Pond.at(k).at<float>(i,j) * (double) v.at(k).at<Vec3b>(i, j)[0] / pondsum;
				Ires.at<Vec3b>(i, j)[1] = Ires.at<Vec3b>(i, j)[1] + Pond.at(k).at<float>(i,j) * (double) v.at(k).at<Vec3b>(i, j)[1] / pondsum;
				Ires.at<Vec3b>(i, j)[2] = Ires.at<Vec3b>(i, j)[2] + Pond.at(k).at<float>(i,j) * (double) v.at(k).at<Vec3b>(i, j)[2] / pondsum;
			}
			

			Ires.at<Vec3b>(i, j)[0] = Ires.at<Vec3b>(i, j)[0]*2;
			Ires.at<Vec3b>(i, j)[1] = Ires.at<Vec3b>(i, j)[1]*2;
			Ires.at<Vec3b>(i, j)[2] = Ires.at<Vec3b>(i, j)[2]*2;

		}
	}
	cout << v.at(0).at<Vec3b>(0,0) << endl;
	cout << v.at(1).at<Vec3b>(0,0) << endl;
	cout << v.at(2).at<Vec3b>(0,0) << endl;
	cout << Ires.at<Vec3b>(0,0) << endl;
	


}
