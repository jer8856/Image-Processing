#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

//#define n 11
//int n=11;

//Function for printing images
void printimage(string windowname, Mat image) {

	namedWindow(windowname, CV_WINDOW_AUTOSIZE);
	imshow(windowname, image);
	//waitKey(0);
}
// 11*11 Erosion Function
Mat Erosion(Mat binaryMat, int mask[11][11]) {
	cv::Mat erosionMat(binaryMat.size(), binaryMat.type());
	for (int i = 0; i < binaryMat.rows; i++) {
		for (int j = 0; j < binaryMat.cols; j++) {
			erosionMat.at<uchar>(i, j) = 0;
		}
	}


	for (int i = 2; i < binaryMat.rows - 2; i++) {
		for (int j = 2; j < binaryMat.cols - 2; j++) {
			int currentpixelvalue = 255;
			int count = 0;
			int setflag = 0;
			for (int a = -2; a <= 2; a++) {

				for (int b = -2; b <= 2; b++) {

					if (mask[a + 2][b + 2] == binaryMat.at<uchar>(i + a, j + b)) {

					}
					else { setflag = 1; }

				}

			}
			if (setflag == 0)
				erosionMat.at<uchar>(i, j) = currentpixelvalue;

		}

	}

	return erosionMat;
}
//3*3 Erosion Function
/*Mat Erosion_small(Mat binaryMat, int mask[3][3]) {
	cv::Mat erosionMat(binaryMat.size(), binaryMat.type());
	for (int i = 0; i < binaryMat.rows; i++) {
		for (int j = 0; j < binaryMat.cols; j++) {
			erosionMat.at<uchar>(i, j) = 0;
		}
	}


	for (int i = 2; i < binaryMat.rows - 2; i++) {
		for (int j = 2; j < binaryMat.cols - 2; j++) {
			int currentpixelvalue = 255;
			int count = 0;
			int setflag = 0;
			for (int a = -2; a <= 2; a++) {

				for (int b = -2; b <= 2; b++) {

					if (mask[a + 2][b + 2] == binaryMat.at<uchar>(i + a, j + b)) {

					}
					else { setflag = 1; }

				}

			}
			if (setflag == 0)
				erosionMat.at<uchar>(i, j) = currentpixelvalue;

		}

	}

	return erosionMat;
}
//3*3 Dilation Function
Mat Dilation_small(Mat binaryMat, int mask[3][3]) {
	cv::Mat dilatedMat(binaryMat.size(), binaryMat.type());
	for (int i = 0; i < binaryMat.rows; i++) {
		for (int j = 0; j < binaryMat.cols; j++) {
			dilatedMat.at<uchar>(i, j) = 0;
		}
	}


	for (int i = 2; i < binaryMat.rows - 2; i++) {
		for (int j = 2; j < binaryMat.cols - 2; j++) {
			int currentpixelvalue = 255;
			int count = 0;
			int setflag = 1;
			for (int a = -2; a <= 2; a++) {


				for (int b = -2; b <= 2; b++) {

					if (mask[a + 2][b + 2] == binaryMat.at<uchar>(i + a, j + b)) {
						setflag = 0;
					}

				}

			}
			if (setflag == 0)
				dilatedMat.at<uchar>(i, j) = currentpixelvalue;

		}

	}
	return dilatedMat;
}*/
// 11*11 Dilation Function
Mat Dilation(Mat binaryMat, int mask[11][11]) {
	cv::Mat dilatedMat(binaryMat.size(), binaryMat.type());
	for (int i = 0; i < binaryMat.rows; i++) {
		for (int j = 0; j < binaryMat.cols; j++) {
			dilatedMat.at<uchar>(i, j) = 0;
		}
	}


	for (int i = 2; i < binaryMat.rows - 2; i++) {
		for (int j = 2; j < binaryMat.cols - 2; j++) {
			int currentpixelvalue = 255;
			int count = 0;
			int setflag = 1;
			for (int a = -2; a <= 2; a++) {


				for (int b = -2; b <= 2; b++) {

					if (mask[a + 2][b + 2] == binaryMat.at<uchar>(i + a, j + b)) {
						setflag = 0;
					}

				}

			}
			if (setflag == 0)
				dilatedMat.at<uchar>(i, j) = currentpixelvalue;

		}

	}
	return dilatedMat;
}


int main()
{
	int n = 11;
	//int m = 11;
	int o = 3;
	//int mask[5][5] = { {255,255,255,255,255},{255,255,255,255,255 },{255,255,255,255,255 },{ 255,255,255,255,255 },{ 255,255,255,255,255 } };
	// generating masks dynamically of a particular range
	int mask[11][11];
	for (int a = 0; a < n; a++) {
		for (int b = 0; b < n; b++) {
			mask[a][b] = 255;
		}
	}

	int mask2[3][3];
	for (int a = 0; a < o; a++) {
		for (int b = 0; b < o; b++) {
			mask2[a][b] = 255;
		}
	}

	Mat image;

	// LOAD image
	image = imread("Test_image.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	

	cv::Mat binaryMat(image.size(), image.type());
	cv::Mat dilatedMat(image.size(), image.type());
	cv::Mat erodedMat(image.size(), image.type());

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) < 170) {
				binaryMat.at<uchar>(i, j) = 0;
			}
			else binaryMat.at<uchar>(i, j) = 255;
		}
	}
	
	

	printimage("Original Image", image);
	//waitKey(0);
	printimage("Binary Image", binaryMat);
	//waitKey(0);

	erodedMat = Erosion(binaryMat,mask);
	printimage("Eroded Image", erodedMat);
	//createTrackbar("Size filter n*n", "Eroded Image", &n, 20);

	dilatedMat = Dilation(binaryMat, mask);
	printimage("Dilated Image", dilatedMat);
	//createTrackbar("Size filter n*n", "Dilated Image", &n, 20);

	waitKey(0);
	
	return 0;
}