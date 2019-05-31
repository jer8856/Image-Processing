#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <stdarg.h>

using namespace cv;
using namespace std;
/**
 * @brief makeCanvas Makes composite image from the given images
 * @param vecMat Vector of Images.
 * @param windowHeight The height of the new composite image to be formed.
 * @param nRows Number of rows of images. (Number of columns will be calculated
 *              depending on the value of total number of images).
 * @return new composite image.
 */

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
                    cv::cvtColor(vecMat[k], target_ROI, CV_GRAY2BGR);
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

Mat func1(Mat m1, Mat m2, int ind1, int ind2)
{
    int cols, rows, x, y;
    cols = m1.cols;
    rows = m1.rows;
    // printf("rows%d cols%d \n", m1.rows, m1.cols);
    
    Mat out1(rows, cols, CV_8UC1, Scalar(0));


    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            out1.at<uchar>(y, x) = pow(2, ind1)*(m1.at<uchar>(y, x)) + pow(2, ind2)*(m2.at<uchar>(y, x)); //Here's where I AND by either 1,2,4,8,16,32,64, or 128 to get the corresponding bit planes
        }
    }
    return out1;
}
int main(int argc, char** argv)
{
    cv::Mat input_img(cv::imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE));

    int32_t rows(input_img.rows), cols(input_img.cols);

    cv::Mat bit_mask(cv::Mat::ones(rows, cols, CV_8UC1));

    cv::Mat work_img(input_img.clone());

    std::vector<Mat> matrix_of_images;
    for (uint32_t i(0); i < 8; ++i)
    {
        cv::Mat out;
        cv::bitwise_and(work_img, bit_mask, out);
        out *= 255;
        matrix_of_images.push_back(out);
        work_img = work_img / 2;
    }
    
    matrix_of_images.push_back(input_img);
    Mat img_fin;
    img_fin=makeCanvas(matrix_of_images,600,3);
    imshow("LAB1 A.1", img_fin);
    waitKey(0);

    std::vector<Mat> matrix_of_images2;
    Mat sum_img,sum_img1,sum_img2,sum_img3,sum_img4,sum_img5,sum_img6,sum_img7;    

    sum_img = func1(matrix_of_images[0], matrix_of_images[1],0,1);
    matrix_of_images2.push_back(sum_img);
    
    sum_img1 = func1(matrix_of_images2[0], matrix_of_images[2],0,2);
    matrix_of_images2.push_back(sum_img1); 

    sum_img2 = func1(matrix_of_images2[1], matrix_of_images[3],0,3);
    matrix_of_images2.push_back(sum_img2); 
    
    sum_img3 = func1(matrix_of_images2[2], matrix_of_images[4],0,4);
    matrix_of_images2.push_back(sum_img3); 
    
    sum_img4 = func1(matrix_of_images2[3], matrix_of_images[5],0,5);
    matrix_of_images2.push_back(sum_img4); 

    sum_img5 = func1(matrix_of_images2[4], matrix_of_images[6],0,6);
    matrix_of_images2.push_back(sum_img5);
    
    sum_img6 = func1(matrix_of_images2[5], matrix_of_images[7],0,7);
    matrix_of_images2.push_back(sum_img6);

    matrix_of_images2.push_back(input_img);
    img_fin=makeCanvas(matrix_of_images2,600,2);
    imshow("LAB1 A.2", img_fin);
    waitKey(0);    
}