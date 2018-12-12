#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "image.h"
#include <opencv2/photo.hpp>
using namespace cv;
using namespace std;



int main(int argc, char** argv) {

	Mat src_color_1 = imread("../image1.jpg");
	Mat src_color_2 = imread("../image2.jpg");
	Mat src_color_3 = imread("../image3.jpg");

	vector<Mat> images;
	images.push_back(src_color_1);
	images.push_back(src_color_2);
	images.push_back(src_color_3);
	Mat exposureFusion;
	Ptr<MergeMertens> mergeMertens = createMergeMertens();
	mergeMertens->process(images, exposureFusion);

	imwrite("exposure-fusion.jpg", exposureFusion * 255);



	return 0;
}
