#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>

 
// Rearrange the quadrants of a Fourier image so that the origin is at
// the image center

//Laplacian
float laplacian[3][3] = { 0.0, 1.0, 0.0,
					  1.0, -4.0, 1.0,
					  0.0, 1.0, 0.0 };

void shiftDFT(cv::Mat &fImage )
{
    cv::Mat tmp, q0, q1, q2, q3;

    // first crop the image, if it has an odd number of rows or columns

    fImage = fImage(cv::Rect(0, 0, fImage.cols & -2, fImage.rows & -2));

    int cx = fImage.cols / 2;
    int cy = fImage.rows / 2;

    // rearrange the quadrants of Fourier image
    // so that the origin is at the image center

    q0 = fImage(cv::Rect(0, 0, cx, cy));
    q1 = fImage(cv::Rect(cx, 0, cx, cy));
    q2 = fImage(cv::Rect(0, cy, cx, cy));
    q3 = fImage(cv::Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

////////////////////////////////////////////////////////////
// return a floating point spectrum magnitude image scaled for user viewing
// complexImg- input dft (2 channel floating point, Real + Imaginary fourier image)
// rearrange - perform rearrangement of DFT quadrants if true

// return value - pointer to output spectrum magnitude image scaled for user viewing

cv::Mat create_spectrum_magnitude_display(cv::Mat &complexImg, bool rearrange)
{
    cv::Mat planes[2];

    // compute magnitude spectrum (N.B. for display)
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))

    cv::split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);

    cv::Mat mag = (planes[0]).clone();
    mag += cv::Scalar::all(1);
    log(mag, mag);

    if (rearrange)
    {
        // re-arrange the quaderants
        shiftDFT(mag);
    }

    normalize(mag, mag, 0, 1, CV_MINMAX);

    return mag;

}

/******************************************************************************/

// create a 2-channel butterworth low-pass filter with radius D, order n
// (assumes pre-aollocated size of dft_Filter specifies dimensions)

void create_ILPF(cv::Mat &dft_Filter, int D, int n, int W)
{
    cv::Mat tmp = cv::Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    cv::Point centre = cv::Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for (int i = 0; i < dft_Filter.rows; i++)
    {
        for (int j = 0; j < dft_Filter.cols; j++)
        {
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
        
            //Ideal Low Pass Filter:
            if(radius <= D) {tmp.at<float>(i,j) = 1;}
            else if(radius > D){tmp.at<float>(i,j) = 0;}
        }
    }

    cv::Mat toMerge[] = {tmp, tmp};
    cv::merge(toMerge, 2, dft_Filter);
}

void create_IHPF(cv::Mat &dft_Filter, int D, int n, int W)
{
    cv::Mat tmp = cv::Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    cv::Point centre = cv::Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for (int i = 0; i < dft_Filter.rows; i++)
    {
        for (int j = 0; j < dft_Filter.cols; j++)
        {
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
        
            //Ideal High Pass Filter:
            if(radius <= D) {tmp.at<float>(i,j) = 0;}
            else if(radius > D){tmp.at<float>(i,j) = 1;}
        }
    }

    cv::Mat toMerge[] = {tmp, tmp};
    cv::merge(toMerge, 2, dft_Filter);
}

cv::Mat spatialFilter(cv::Mat image, float kernel[3][3]){
    
    cv::Mat imageRes;
    imageRes = cv::Mat::zeros(image.size(), CV_8UC1); //va a ser el resultado; en la imagen resultante el resultado es 0

    float sum; int a=-1; int b=-1;

    //FILTERING OPERTTION

    for(int row = 1; row < image.rows-1; row++){
        for(int col=1;col<image.cols-1;col++){
            sum=0.0;

            for(int k=a;k<=1;k++){
                for(int j=a;j<=1;j++){
                    sum+=kernel[j+1][k+1]*image.at<uchar>(row-j,col-k);
                }

            }
            imageRes.at<uchar>(row,col)=sum;
        }
    }

    return imageRes;
}
////////
int main( int argc, char** argv ) {
  
  cv::Mat image, imageRes;
   
  //image = cv::imread("../imgs/Lab4/Barbara512.jpg" , CV_LOAD_IMAGE_GRAYSCALE);//**
  image = cv::imread("lena.jpg" , CV_LOAD_IMAGE_GRAYSCALE);//**
  cv::Mat spatialFilteredImage = spatialFilter(image,laplacian);
 
   bool keepProcessing = true;   // loop control flag
    
  if(! image.data ) {
      std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }

    cv::Mat padded;       // fourier image objects and arrays
    cv::Mat complexImg, complexImg2, filter, filterOutput,filterOutput2,filterOutputLow,filterLow;
    cv::Mat planes[2], mag, mag2;

    int N, M; // fourier image sizes

    int radius = 20;              // low pass filter parameter
    int order = 1;                // low pass filter parameter
    int width = 3;

    char  originalName[] = "Original image"; // window name
    char spectrumMagName[] = "Magnitude Image (log transformed)- spectrum"; // window name
    char lowPassName[] = "Butterworth Low Pass Filtered (grayscale)"; // window name
    char filterName[] = "Filter Image"; // window nam
    char spatialFilterName[] = "LaplacianFilter(Spatial)";

  
    cv::namedWindow( originalName, cv::WINDOW_NORMAL );
    cv::resizeWindow(originalName, 450,450);

    cv::namedWindow( spectrumMagName, cv::WINDOW_NORMAL );
    cv::resizeWindow(spectrumMagName, 450,450);

    cv::namedWindow( lowPassName, cv::WINDOW_NORMAL );
    cv::resizeWindow(lowPassName, 450,450);

    cv::namedWindow( filterName, cv::WINDOW_NORMAL );
    cv::resizeWindow(filterName, 450,450);

    cv::namedWindow(spatialFilterName, cv::WINDOW_NORMAL );
    cv::resizeWindow(spatialFilterName, 450,450);

    cv::imshow( originalName, image );
    cv::imshow(spatialFilterName, spatialFilteredImage);

    
    // setup the DFT image sizes

   M = cv::getOptimalDFTSize( image.rows );
   N = cv::getOptimalDFTSize( image.cols );

   //////Create
   // add adjustable trackbar for low pass filter threshold parameter

   cv::createTrackbar("Radius", lowPassName, &radius, (cv::min(M, N) / 2));
   

   //////////////// SPECTRAL FILTER ON IMAGE
   while (keepProcessing)
  {
   // setup the DFT images

	    //copyMakeBorder( src, dst, top, bottom, left, right, borderType, value );
            cv::copyMakeBorder(image, padded, 0, M - image.rows, 0,
                           N - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
            planes[0] = cv::Mat_<float>(padded);
            planes[1] = cv::Mat::zeros(padded.size(), CV_32F);
	      
            cv::merge(planes, 2, complexImg);

            // do the DFT

            cv::dft(complexImg, complexImg);

            // construct the filter (same size as complex image)

            filter = complexImg.clone();
            filterLow = complexImg.clone();
            complexImg2 = complexImg.clone();
            
            create_ILPF(filterLow, radius, order, width);
            cv::Mat filterE;
            cv::idft(filter, filterE);

            create_IHPF(filter, radius, order, width);


            shiftDFT(complexImg2);
            //void cvMulSpectrums(const CvArr* src1, const CvArr* src2, CvArr* dst, int flags) 
            cv::mulSpectrums(complexImg2, filterLow, complexImg2, 0);
            shiftDFT(complexImg2);

            // apply filter
            shiftDFT(complexImg);
            //void cvMulSpectrums(const CvArr* src1, const CvArr* src2, CvArr* dst, int flags) 
            cv::mulSpectrums(complexImg, filter, complexImg, 0);
            shiftDFT(complexImg);

            // create magnitude spectrum for display

            mag = create_spectrum_magnitude_display(complexImg, true);
            mag2 = create_spectrum_magnitude_display(complexImg2, true);

            // do inverse DFT on filtered image
       	    cv::Mat result;
            cv::idft(complexImg, result);

            cv::Mat result2;
            cv::idft(complexImg2, result2);

            // split into planes and extract plane 0 as output image
            cv::Mat myplanes[2]={
				cv::Mat::zeros(complexImg.size(), CV_32F),
				cv::Mat::zeros(complexImg.size(), CV_32F)
				};
            split(result, myplanes);
     
	        magnitude(myplanes[0], myplanes[1], result);
            cv::normalize(result, result, 0, 1, CV_MINMAX);
            imageRes = result;

            // do the same with the filter image

            cv::Mat myplanes2[2]={
				cv::Mat::zeros(complexImg2.size(), CV_32F),
				cv::Mat::zeros(complexImg2.size(), CV_32F)
				};
            split(result2, myplanes2);
     
	        magnitude(myplanes2[0], myplanes2[1], result2);
            cv::normalize(result2, result2, 0, 1, CV_MINMAX);
            hconcat(imageRes,result2,imageRes);

            

            // do the same with the filter image

            hconcat(mag,mag2,mag);
            cv::split(filter, planes);
	        cv::normalize(planes[0], filterOutput, 0, 1, CV_MINMAX);
            cv::split(filterLow, planes);
	        cv::normalize(planes[0], filterOutput2, 0, 1, CV_MINMAX);
            hconcat(filterOutput,filterOutput2,filterOutput);


            // display all images in window
            imshow(spectrumMagName, mag);
            imshow(lowPassName, imageRes);
            imshow(filterName, filterOutput);
       
       if (cv::waitKey(40) == 'e')
                keepProcessing = false;
            
      }
  //cv::waitKey(0);
  return 0;
}