//Example usage of SUACE contrast enhancement algorithm
// #include "stdafx.h"
// #pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
// #include "suace.h"
using namespace cv;
using namespace std;

int a = 21;
int b = 36;
int intensityMax = 255;

void performSUACE(Mat &src, Mat &dst, int distance, double sigma) {

	CV_Assert(src.type() == CV_8UC1);
	if (!(distance > 0 && sigma > 0)) {
		CV_Error(CV_StsBadArg, "distance and sigma must be greater 0");
	}
	dst = Mat(src.size(), CV_8UC1);
	Mat smoothed;
	int val;
	int a, b;
	int adjuster;
	int half_distance = distance / 2;
	double distance_d = distance;

	GaussianBlur(src, smoothed, cv::Size(0, 0), sigma);

	for (int x = 0;x<src.cols;x++)
		for (int y = 0;y < src.rows;y++) {
			val = src.at<uchar>(y, x);
			adjuster = smoothed.at<uchar>(y, x);
			if ((val - adjuster) > distance_d)adjuster += (val - adjuster)*0.5;
			adjuster = adjuster < half_distance ? half_distance : adjuster;
			b = adjuster + half_distance;
			b = b > 255 ? 255 : b;
			a = b - distance;
			a = a < 0 ? 0 : a;

			if (val >= a && val <= b)
			{
				dst.at<uchar>(y, x) = (int)(((val - a) / distance_d) * 255);
			}
			else if (val < a) {
				dst.at<uchar>(y, x) = 0;
			}
			else if (val > b) {
				dst.at<uchar>(y, x) = 255;
			}
		}
}

int main()
{
	namedWindow("SUACE", 1);
	createTrackbar("distance", "SUACE", &a, intensityMax);
	createTrackbar("sigma", "SUACE", &b, intensityMax);
	char filename[100];
	Mat suaceResult;
	Mat frame;

	while (true)
	{
		frame = imread("/home/jer/YachayTech/8vo/Procesamiento de Imagenes/Labs/Lab1/Sample imagery/cameraman.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		performSUACE(frame, suaceResult, a, (b + 1) / 8.0); //perform SUACE with the parameters
		imshow("SUACE", suaceResult);
		imshow("Original", frame);
		int response = waitKey(0);//press key to update
		if (response == 32) //exit when spacebar key pressed;
			break;
	}
	return 0;
}
