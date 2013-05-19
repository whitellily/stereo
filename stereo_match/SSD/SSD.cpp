#include<iostream>
#include<cv.h>
#include<highgui.h>

using namespace std;
int GetHammingWeight(unsigned int value);
int main(){
    /*Half of the window size for the census transform*/
    int hWin = 5;
    int compareLength = (2*hWin+1)*(2*hWin+1);
 
    cout<<"hWin: "<<hWin<<";  "<<"compare length:  "<<compareLength<<endl;  
    cout<<"SAD test"<<endl;
    // char stopKey;
    IplImage * leftImage = cvLoadImage("../tsukuba_l.png",0);
    IplImage * rightImage = cvLoadImage("../tsukuba_r.png",0);

    int imageWidth = leftImage->width;
    int imageHeight =leftImage->height;

    IplImage * SADImage = cvCreateImage(cvGetSize(leftImage),leftImage->depth,1);
    IplImage * MatchLevelImage = cvCreateImage(cvGetSize(leftImage),leftImage->depth,1);

    int minDBounds = 0;
    int maxDBounds = 31;
   
    cvNamedWindow("Left",1);
    cvNamedWindow("Right",1);
    cvNamedWindow("SSD",1);
    cvNamedWindow("MatchLevel",1);

    cvShowImage("Left",leftImage);
    cvShowImage("Right",rightImage);



    /*Census Transform */
    int i,j ,m,n,k;
    unsigned char centerPixel = 0; 
    unsigned char neighborPixel = 0;
    int bitCount = 0;
    unsigned int bigger = 0;


  
    int sum = 0;
    unsigned int *matchLevel = new unsigned int[maxDBounds - minDBounds  + 1];
    int tempMin = 0;
    int tempIndex = 0;
   
    unsigned char* dst;
    unsigned char* leftSrc  = NULL;
    unsigned char* rightSrc = NULL;

    unsigned char leftPixel = 0;
    unsigned char rightPixel =0;
    unsigned char subPixel = 0;
 

    for(i = 0 ; i < leftImage->height;i++){
        for(j = 0; j< leftImage->width;j++){

            for (k = minDBounds;k <= maxDBounds;k++)
            {
                sum = 0;
                for (m = i-hWin; m <= i + hWin;m++)
                {
                    for (n = j - hWin; n <= j + hWin;n++)
                    {
                          if (m < 0 || m >= imageHeight || n <0 || n >= imageWidth )
                          {
                              subPixel  = 0;
                          }else if (n + k >= imageWidth)
                          {
                              subPixel = 0;
                          }else
                          {
                              leftSrc = (unsigned char*)leftImage->imageData 
                                          + m*leftImage->widthStep + n + k; 
                              rightSrc = (unsigned char*)rightImage->imageData 
                                           + m*rightImage->widthStep + n;

                              leftPixel = *leftSrc;
                              rightPixel = *rightSrc;
                              if (leftPixel > rightPixel)
                              {
                                   subPixel = leftPixel - rightPixel;
                              }else 
                              {
                                   subPixel = rightPixel -leftPixel;
                              }
                             
                          }

                          sum += subPixel*subPixel;
                    }
                }
                matchLevel[k] = sum;
                //cout<<sum<<endl;
            }

            /*寻找最佳匹配点*/
           // matchLevel[0] = 1000000;

            tempMin = 0;
            tempIndex = 0;
            for ( m = 1;m < maxDBounds - minDBounds + 1;m++)
            {
                //cout<<matchLevel[m]<<endl;
                if (matchLevel[m] < matchLevel[tempIndex])
                {
                    tempMin = matchLevel[m];
                    tempIndex = m;
                }
            }
            dst = (unsigned char *)SADImage->imageData + i*SADImage->widthStep + j;
            //cout<<"index: "<<tempIndex<<"  ";

            *dst = tempIndex*8;

            dst = (unsigned char *)MatchLevelImage->imageData + i*MatchLevelImage->widthStep + j;
            *dst = tempMin;
            //cout<<"min:  "<<tempMin<<"  ";
            //cout<< tempIndex<<"  " <<tempMin<<endl;
        }
        //cvWaitKey(0);
    }
   
    cvShowImage("SSD",SADImage);
    cvShowImage("MatchLevel",MatchLevelImage);
    cvSaveImage("depth.jpg",SADImage);
    cvSaveImage("matchLevel.jpg",MatchLevelImage);

    cvWaitKey(0);
    cvDestroyAllWindows();
    cvReleaseImage(&leftImage);
    cvReleaseImage(&rightImage);
    return 0;
}

