/*#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;


//Function for printing images
void printimage(string windowname, Mat image) {

	namedWindow(windowname, CV_WINDOW_AUTOSIZE);
	imshow(windowname, image);
	//waitKey(0);
}


int main()
{
    Mat image = imread("sample.bmp",IMREAD_GRAYSCALE);
    int rows = image.rows;
    int cols = image.cols;
    int size = rows * cols;
    Mat skel = Mat::zeros(image.rows,image.cols,CV_8U);
    threshold(image, image, 100, 255, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_CROSS, Size(3,3));
    bool done = false;
    while(done == false){
        Mat eroded;
        erode(image, eroded, element);
        Mat temp;
        dilate(eroded, temp, element);
        subtract(image, temp, temp);
        bitwise_or(skel, temp, skel);
        eroded.copyTo(image);

        int zeros = size - countNonZero(image);
        if(zeros == size){
            done = true;
        }
    }

    imshow("skel",skel);
    waitKey(0);
    destroyAllWindows();

	waitKey(0);
	
	return 0;
}*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

cv::Mat makeCanvas(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows)
{
    int N = vecMat.size();
    nRows  = nRows > N ? N : nRows; 
    int edgeThickness = 10;
    int imagesPerRow = ceil(double(N) / nRows);
    int resizeHeight = floor(2.0 * ((floor(double(windowHeight - edgeThickness) / nRows)) / 2.0)) - edgeThickness;
    int maxRowLength = 0;

    std::vector<int> resizeWidth;
    for (int i = 0; i < N;)
    {
        int thisRowLen = 0;
        for (int k = 0; k < imagesPerRow; k++)
        {
            double aspectRatio = double(vecMat[i].cols) / vecMat[i].rows;
            int temp = int( ceil(resizeHeight * aspectRatio));
            resizeWidth.push_back(temp);
            thisRowLen += temp;
            if (++i == N) break;
        }
        if ((thisRowLen + edgeThickness * (imagesPerRow + 1)) > maxRowLength)
        {
                maxRowLength = thisRowLen + edgeThickness * (imagesPerRow + 1);
        }
    }
    int windowWidth = maxRowLength;
    cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, Scalar(0, 0, 0));

    for (int k = 0, i = 0; i < nRows; i++)
    {
        int y = i * resizeHeight + (i + 1) * edgeThickness;
        int x_end = edgeThickness;
        for (int j = 0; j < imagesPerRow && k < N; k++, j++)
        {
            int x = x_end;
            cv::Rect roi(x, y, resizeWidth[k], resizeHeight);
            cv::Size s = canvasImage(roi).size();
            // change the number of channels to three
            cv::Mat target_ROI(s, CV_8UC3);
            if (vecMat[k].channels() != canvasImage.channels())
            {
                if (vecMat[k].channels() == 1)
                {
                    cv::cvtColor(vecMat[k], target_ROI, COLOR_GRAY2BGR);
                }
            } else 
            {             
                vecMat[k].copyTo(target_ROI);
            }
            cv::resize(target_ROI, target_ROI, s);
            if (target_ROI.type() != canvasImage.type())
            {
                target_ROI.convertTo(target_ROI, canvasImage.type());
            }
            target_ROI.copyTo(canvasImage(roi));
            x_end += resizeWidth[k] + edgeThickness;
        }
    }
    return canvasImage;
}

int main ( int argc, char **argv )
{
    Mat img_erode, img_final;   // no nead to load the Mat with anything when declaring it.
    // Mat img_input  = imread("cqm.jpg", 0);
    // Mat img_input  = imread("pep.png", 0);
    Mat img_input  = imread("sample.bmp", 0);
    // Mat img_input  = imread("lena.png", 0);
    // Mat img_input  = imread("im5.jpg", 0);
    

    

    bool keepProcessing=true;
    int type = MORPH_RECT;
    int size = 3;
    int threshold_var=10;
    char  windowdName[] = "Boundary Extraction";
    char  original_img[] = "Original image";

    namedWindow( windowdName, cv::WINDOW_NORMAL );
    namedWindow( original_img, cv::WINDOW_NORMAL );
    createTrackbar( "threshold", windowdName, &threshold_var, 200 );
    createTrackbar("Size filter n*n", windowdName, &size, 6);
    
    Mat element,img_thresholded;

    while(keepProcessing)
    {
        imshow( original_img, img_input);
        if(size<2) size=2;
        threshold(img_input, img_thresholded, threshold_var, 255.0, THRESH_BINARY);
        element = getStructuringElement( type, Size(size, size));   
        erode( img_thresholded, img_erode, element );

        img_final = img_thresholded - img_erode;
        imshow( windowdName, img_final );

        if (cv::waitKey(40) == 'e')
            keepProcessing = false;
    } 
}  