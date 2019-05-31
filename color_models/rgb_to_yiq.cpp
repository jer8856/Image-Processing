/*
    RGB to YIQ conversion is used in the NTSC encoder where the RGB inputs from the camera are 
    converted to luminance (Y) and chrominance (I,Q) information. In the NTSC encoder, the I,Q 
    signals are modulated by a subcarrier and added to the Y signal. Historically, when color 
    TVs appeared in the market, they had to coexist with the existing monochrome TVs and this 
    was made possible with the NTSC signal structure. The chrominance signals are averaged out 
    as a fine mesh of invisible signals in the monochrome TV sets. YUV used in the PAL standard 
    and YCbCr used in the JPEG standard have different encodings. All three standards share the 
    same luminance signal Y but the chrominance calculations are different. The matrix calculation 
    scheme used in the RGB to YIQ can be applied to these standards too. In the actual products, 
    this trivial calculation is usually performed in dedicated hardware, especially in digital 
    video products. For cost saving and flexibility, this algorithm can be implemented in software 
    if the CPU is powerful enough and where the digital image is a still picture.
*/
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void RGB_to_yiq(double &R, double &G, double &B, double &Y, double &I, double &Q)
{
    Y = 0.299*R + 0.587*G + 0.114*B; 
    I = 0.596*R - 0.275*G - 0.321*B;
    Q = 0.212*R - 0.523*G + 0.311*B;
}

void yiq_to_RGB(double &R, double &G, double &B, double &Y, double &I, double &Q)
{
    R = 1*Y + 0.95569 *I + 0.61986 *Q;
    G = 1*Y + -0.27158*I + -0.64687*Q; 
    B = 1*Y + -1.10818*I + 1.70506 *Q;
    
}
int main ( int argc, char **argv )
{  
    // Mat imageI  = imread("bola.jpeg",1);
    // Mat imageI  = imread("ff.png",1);
    Mat imageI  = imread("pep.png",1);
    
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

            RGB_to_yiq(r,g,b,x,y,z);

            temp.at<Vec3b>(row,col)[2] = x*255;
            temp.at<Vec3b>(row,col)[1] = y*255;
            temp.at<Vec3b>(row,col)[0] = z*255;
        }
    }

    cv::namedWindow("rgb to yiq o1o2o3", WINDOW_NORMAL );
    imshow("rgb to yiq o1o2o3", temp);
    waitKey(0);

    Mat temp2 = temp.clone();
	
    for (int row=0; row<temp.rows; row++)
    {   
        for (int col=0; col<temp.cols; col++)
        { 
        	x = temp2.at<Vec3b>(row,col)[2]/255.0;
        	y = temp2.at<Vec3b>(row,col)[1]/255.0;
        	z = temp2.at<Vec3b>(row,col)[0]/255.0;

            yiq_to_RGB(r,g,b,x,y,z);

            temp2.at<Vec3b>(row,col)[2] = r*255;
            temp2.at<Vec3b>(row,col)[1] = g*255;
            temp2.at<Vec3b>(row,col)[0] = b*255;
        }
    }

    namedWindow("yiq o1o2o3 to rgb", WINDOW_NORMAL);
    imshow("yiq o1o2o3 to rgb", temp2);
    waitKey(0);
}  