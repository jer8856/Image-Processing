/*
    Describing color has always been a complex challenge. Cameras provide their image data in the common 
    sRGB color space, so chances are high that any image you receive without any additional information 
    is represented by the colors in sRGB. Other color spaces, such as those relating to human vision,
     are used for the analysis of color.

    As this analysis is a frequent task, we have created a color cheat sheet, which provides a 
    quick overview of sRGB and CIE-XYZ and the equations for their conversions.


    sRGB...The "s" in sRGB stands for "standard" and it is the lowest common denominator in 
    color spaces. It was developed in the 1990s to provide a universal usable color space
     for display and printers of the time. While there are many different and larger color 
     spaces available, sRGB is still the de facto standard for RGB image data.


    CIE-XYZ...CIE (Commission Internationale de l'Éclairage) stands for the International
     Commission on Illumination, which established color spaces based on colors that can be
      perceived by the human eye. XYZ does not incorporate negative numbers and instead
       uses tristimulus values to define the color space that can be seen by the human eye. 
       X represents the linear combination of the cone response non-negative curves created. 
       Y represents luminance and Z is defined as almost equal to blue.

    Can be used in food classification



*/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void RGB_to_CIEXYZ(double &r, double &g, double &b, double &x, double &y, double &z)
{
    double r_prime, g_prime, b_prime;
    
    if (r<= 0.04045)
    {
        r_prime = r/12.92;
    }else
    {
        r_prime = pow((r+0.055)/1.055, 2.4);
    }
    if (g<= 0.04045)
    {
        g_prime = r/12.92;
    }else
    {
        g_prime = pow((g+0.055)/1.055, 2.4);
    }
    
    if (b<= 0.04045)
    {
        b_prime = r/12.92;
    }else
    {
        b_prime = pow((b+0.055)/1.055, 2.4);
    }

    x = 0.4124564*r_prime + 0.3575761*g_prime + 0.1804375*b_prime;
    y = 0.2126729*r_prime + 0.7151522*g_prime + 0.0721750*b_prime;
    z = 0.0193339*r_prime + 0.1191920*g_prime + 0.9503041*b_prime;
}

void CIEXYZ_to_RGB(double &r, double &g, double &b, double &x, double &y, double &z)
{
    double r_prime, g_prime, b_prime;

    r_prime = 3.2404542 *x + -1.5371385*y +  -0.4985314*z;
    g_prime = -0.9692660*x + 1.8760108 *y +  0.0415560 *z;
    b_prime = 0.0556434 *x + -0.2040259*y +  1.0572252 *z;

    if (r_prime<= 0.0031308)
    {
        r = r_prime/12.92;
    }else
    {
        r = pow(r_prime*1.055, 1/2.4) - 0.055;
    }

    if (g_prime<= 0.0031308)
    {
        g = g_prime/12.92;
    }else
    {
        g = pow(g_prime*1.055, 1/2.4) - 0.055;
    }

    if (b_prime<= 0.0031308)
    {
        b = b_prime/12.92;
    }else
    {
        b = pow(b_prime*1.055, 1/2.4) - 0.055;
    }

}
int main ( int argc, char **argv )
{  
    Mat imageI  = imread("bola.png",1);
    // Mat imageI  = imread("pep.png",1);
    
    cv::namedWindow( "original", WINDOW_NORMAL );
    imshow("original", imageI);
    waitKey(0);

	double r,g,b,x,y,z;
    Mat temp = imageI.clone ();
	
    for (int row=0; row<imageI.rows; row++)
    {   
        for (int col=0; col<imageI.cols; col++)
        { 
        	r = imageI.at<Vec3b>(row,col)[2]/255.0;
        	g = imageI.at<Vec3b>(row,col)[1]/255.0;
        	b = imageI.at<Vec3b>(row,col)[0]/255.0;

            RGB_to_CIEXYZ(r,g,b,x,y,z);

            temp.at<Vec3b>(row,col)[2] = x*255;
            temp.at<Vec3b>(row,col)[1] = y*255;
            temp.at<Vec3b>(row,col)[0] = z*255;
        }
    }

    cv::namedWindow("rgb to ciexyz", WINDOW_NORMAL );
    imshow("rgb to ciexyz", temp);
    waitKey(0);

    Mat temp2 = temp.clone();
	
    for (int row=0; row<temp.rows; row++)
    {   
        for (int col=0; col<temp.cols; col++)
        { 
        	x = temp2.at<Vec3b>(row,col)[2]/255.0;
        	y = temp2.at<Vec3b>(row,col)[1]/255.0;
        	z = temp2.at<Vec3b>(row,col)[0]/255.0;

            CIEXYZ_to_RGB(r,g,b,x,y,z);

            temp2.at<Vec3b>(row,col)[2] = r*255;
            temp2.at<Vec3b>(row,col)[1] = g*255;
            temp2.at<Vec3b>(row,col)[0] = b*255;
        }
    }

    namedWindow("ciexyz to rgb", WINDOW_NORMAL);
    imshow("ciexyz to rgb", temp2);
    waitKey(0);
}  
