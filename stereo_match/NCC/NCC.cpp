#include<iostream>
#include<cv.h>
#include<highgui.h>
#include <cmath>
using namespace std;

int main(){
    /*Half of the window size for the census transform*/
    int hWin = 5;
    int compareLength = (2*hWin+1)*(2*hWin+1);
 
    cout<<"hWin: "<<hWin<<";  "<<"compare length:  "<<compareLength<<endl;  
    cout<<"NCC test"<<endl;
  /*  IplImage * leftImage = cvLoadImage("l2.jpg",0);
    IplImage * rightImage = cvLoadImage("r2.jpg",0);*/

    IplImage * leftImage = cvLoadImage("../tsukuba_l.png",0);
    IplImage * rightImage = cvLoadImage("../tsukuba_r.png",0);

    int imageWidth = leftImage->width;
    int imageHeight =leftImage->height;

    IplImage * NCCImage = cvCreateImage(cvGetSize(leftImage),leftImage->depth,1);
    IplImage * MatchLevelImage = cvCreateImage(cvGetSize(leftImage),leftImage->depth,1);

    int minDBounds = 0;
    int maxDBounds = 31;
   
    cvNamedWindow("Left",1);
    cvNamedWindow("Right",1);
    cvNamedWindow("Census",1);
    cvNamedWindow("MatchLevel",1);

    cvShowImage("Left",leftImage);
    cvShowImage("Right",rightImage);



    /*Census Transform */
    int i,j ,m,n,k;
    unsigned char centerPixel = 0; 
    unsigned char neighborPixel = 0;
    int bitCount = 0;
    unsigned int bigger = 0;

    unsigned int sum =0;
    unsigned int leftSquareSum = 0;
    unsigned int rightSquareSum = 0; 
   
    double *matchLevel = new double[maxDBounds - minDBounds  + 1];
    double tempMax = 0;
    int tempIndex = 0;
   
    unsigned char* dst;
    unsigned char* leftSrc  = NULL;
    unsigned char* rightSrc = NULL;

    unsigned char leftPixel = 0;
    unsigned char rightPixel =0;
    unsigned char subPixel = 0;
    unsigned char meanLeftPixel  = 0;
    unsigned char meanRightPixel = 0;
 

    for(i = 0 ; i < leftImage->height;i++){
        for(j = 0; j< leftImage->width;j++){

            /*均值计算 */
            for (k = minDBounds;k <= maxDBounds;k++)
            {
                sum = 0;
                leftSquareSum  = 0;
                rightSquareSum = 0;

                 for (m = i-hWin; m <= i + hWin;m++)
                {
                    for (n = j - hWin; n <= j + hWin;n++)
                    {
                          if (m < 0 || m >= imageHeight || n <0 || n >= imageWidth )
                          {
                             
                              leftPixel = 0;
                          }else {
                              leftSrc = (unsigned char*)leftImage->imageData 
                                  + m*leftImage->widthStep + n + k; 
                              leftPixel = *leftSrc;
                              
                          }
                          
                          
                          if (m < 0 || m >= imageHeight || n + k <0 || n +k >= imageWidth)
                          {
                              
                              rightPixel = 0;
                          }else
                          { 
                              rightSrc = (unsigned char*)rightImage->imageData 
                                           + m*rightImage->widthStep + n;
                              rightPixel = *rightSrc;
                               
                          }

                          sum +=  leftPixel*rightPixel;
                          leftSquareSum  += leftPixel*leftPixel;
                          rightSquareSum += rightPixel*rightPixel;

                    }
                }
                matchLevel[k] = (double)sum/(sqrt(double(leftSquareSum))*sqrt((double)rightSquareSum));
            
            }

         
            tempMax = 0;
            tempIndex = 0;
            for ( m = 1;m < maxDBounds - minDBounds + 1;m++)
            {
              
                if (matchLevel[m] > matchLevel[tempIndex])
                {
                    tempMax = matchLevel[m];
                    tempIndex = m;
                }
            }
            dst = (unsigned char *)NCCImage->imageData + i*NCCImage->widthStep + j;
           
            *dst = tempIndex*8;
            dst = (unsigned char *)MatchLevelImage->imageData + i*MatchLevelImage->widthStep + j;
            *dst = (unsigned char)(tempMax*255);
          
        }

    }
   
    cvShowImage("Census",NCCImage);
    cvShowImage("MatchLevel",MatchLevelImage);
    cvSaveImage("depth.jpg",NCCImage);
    cvSaveImage("matchLevel.jpg",MatchLevelImage);


    cout<<endl<<"Over"<<endl;
    cvWaitKey(0);
    cvDestroyAllWindows();
    cvReleaseImage(&leftImage);
    cvReleaseImage(&rightImage);
    return 0;
}

