
/*
    Created by Shirish Kishore Kumar 
*/
#include <opencv2/opencv.hpp>

// Alternaticve grayscale (Key press = H)
int greyscale( cv::Mat &src, cv::Mat &dst )
{
    for (int i = 0; i < src.rows; i++) 
    {
        for (int j = 0; j < src.cols; j++) 
        {
            // accessing every channel 
            cv::Vec3b img = src.at<cv::Vec3b>(i, j);
            // Modifying the color channel weights
            uchar blue = 0.5*img[0];
            uchar green = img[1];
            uchar red = 2*img[2];
            // Converting the image from 3 channel to Single channel i.e) greyscale
            uchar gray = (blue + green + red) / 3;
            dst.at<uchar>(i, j) = gray;
        }
    }
    return 0;
}
// SOBEL X ( Key press =X)
int sobelX3(cv::Mat &src, cv::Mat &dst)
{
    // Declaring the scaled image Mat
    cv::Mat dis;
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication
    cv::Mat first = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication
    cv::Mat dst1 = cv::Mat::zeros(src.size(), CV_16SC3);
    //Performing 3*1 Multiplication
    for (int i = 1; i<src.rows-1;i++)
    {
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

        cv::Vec3s *dptr = first.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr[j][c] = (1*rptrm1[j-1][c]  
                            +2*rptr[j-1][c] 
                            +1*rptrp1[j-1][c])/4;
            }
        }  
    }
    //Performing 3*1 Multiplication
    for (int i = 1; i<first.rows-1;i++)
    {
        cv::Vec3s *rptrm12 = first.ptr<cv::Vec3s>(i-1);
        cv::Vec3s *rptr2 = first.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp12 = first.ptr<cv::Vec3s>(i+1);
        
        cv::Vec3s *dptr2 = dst1.ptr<cv::Vec3s>(i);

        for (int j = 1; j<first.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr2[j][c] = (-1*rptrm12[j-1][c]   +0*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
            }
        }  
    }
    //Scaling back the values
    cv::convertScaleAbs(first,dis);
    cv::convertScaleAbs(dst1,dst);
    return 0;
}


// SOBEL Y ( Key press =Y)
int sobelY3(cv::Mat &src, cv::Mat &dst)
{
    // Declaring the scaled image Mat
    cv::Mat diss;
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication
    cv::Mat dss = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication
    cv::Mat dss1 = cv::Mat::zeros(src.size(), CV_16SC3);
    //Performing 1*3 Multiplication
    for (int i = 1; i<src.rows-1;i++)
        {
            cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
            cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
            cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

            cv::Vec3s *dptr = dss.ptr<cv::Vec3s>(i);

            for (int j = 1; j<src.cols-1;j++)
            {
                for (int c = 0; c<3; c++)
                {
                    dptr[j][c] = (-1*rptrm1[j-1][c]  
                                +0*rptr[j-1][c] 
                                +1*rptrp1[j-1][c])/4;
                }
            }  
        }
        //Performing 3*1 Multiplication
        for (int i = 1; i<dss.rows-1;i++)
        {
            cv::Vec3s *rptrm12 = dss.ptr<cv::Vec3s>(i-1);
            cv::Vec3s *rptr2 = dss.ptr<cv::Vec3s>(i);
            cv::Vec3s *rptrp12 = dss.ptr<cv::Vec3s>(i+1);
            
            cv::Vec3s *dptr2 = dss1.ptr<cv::Vec3s>(i);

            for (int j = 1; j<dss.cols-1;j++)
            {
                for (int c = 0; c<3; c++)
                {
                    dptr2[j][c] = (1*rptrm12[j-1][c]   +2*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
                }
            }  
        }
        //Scaling back the values
        cv::convertScaleAbs(dss,diss);
        cv::convertScaleAbs(dss1,dst);    
        return 0;
}


// Magnitude image from X and Y sobel images ( Key press = M)
int magnitude(cv::Mat &src, cv::Mat &dst)
{
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel x
    cv::Mat first = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel x
    cv::Mat dst1 = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel y
    cv::Mat dss = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel y
    cv::Mat dss1 = cv::Mat::zeros(src.size(), CV_16SC3);
    //Performing SOBEL X 
    for (int i = 1; i<src.rows-1;i++)
    {
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

        cv::Vec3s *dptr = first.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr[j][c] = (1*rptrm1[j-1][c]  
                            +2*rptr[j-1][c] 
                            +1*rptrp1[j-1][c])/4;
            }
        }  
    }
    for (int i = 1; i<first.rows-1;i++)
    {
        cv::Vec3s *rptrm12 = first.ptr<cv::Vec3s>(i-1);
        cv::Vec3s *rptr2 = first.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp12 = first.ptr<cv::Vec3s>(i+1);
        
        cv::Vec3s *dptr2 = dst1.ptr<cv::Vec3s>(i);

        for (int j = 1; j<first.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr2[j][c] = (-1*rptrm12[j-1][c]   +0*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
            }
        }  
    }
    //Performing SOBEL Y 
    for (int i = 1; i<src.rows-1;i++)
    {
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

        cv::Vec3s *dptr = dss.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr[j][c] = (-1*rptrm1[j-1][c]  
                            +0*rptr[j-1][c] 
                            +1*rptrp1[j-1][c])/4;
            }
        }  
    }
    for (int i = 1; i<dss1.rows-1;i++)
    {
        cv::Vec3s *rptrm12 = dss.ptr<cv::Vec3s>(i-1);
        cv::Vec3s *rptr2 = dss.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp12 = dss.ptr<cv::Vec3s>(i+1);
        
        cv::Vec3s *dptr2 = dss1.ptr<cv::Vec3s>(i);

        for (int j = 1; j<dss.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr2[j][c] = (1*rptrm12[j-1][c]   +2*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
            }
        }  
    }
        //Declaring Mat for scaling calculations
        cv::Mat dis;
        cv::Mat dis1;
        cv::Mat diss;
        cv::Mat diss1;
    
        //X SOBEL
        cv::convertScaleAbs(first,dis);
        cv::convertScaleAbs(dst1,dis1);
        //Y SOBEL
        cv::convertScaleAbs(dss,diss);
        cv::convertScaleAbs(dss1,diss1);

        cv::Mat xsob = dis1;
        cv::Mat ysob = diss1;
        //Converting thier data type and taking thier sum and square root

        dis1.convertTo(dis1, CV_32F);
        diss1.convertTo(diss1, CV_32F);

        cv::multiply(dis1,dis1,dis1);
        cv::multiply(diss1,diss1,diss1);
        cv::addWeighted(dis1, 0.5, diss1, 0.5, 0, dst);
        cv::sqrt(dst,dst);
        
        dst.convertTo(dst, CV_8U);
        return 0;
}

//Blur quantize (Key press =B)
int blurQuantize(cv::Mat &src, cv::Mat &dst )
{
    for (int i = 0; i < src.rows; i++) 
    {
        for (int j = 0; j < src.cols; j++) 
        {
            cv::Vec3b img = src.at<cv::Vec3b>(i, j);
            //Quantizing the image into a fixed number of levels 
            float b;
            b = 255/5;
            //changing the each color channel to only **3 possible color values
            img[0] = img[0]/b;
            img[0] = img[0]*b;

            img[1] = img[1]/b;
            img[1] = img[1]*b;

            img[2] = img[2]/b;
            img[2] = img[2]*b;
            dst.at<cv::Vec3b>(i, j) = img;
        }
    }
    return 0;
}

//Cartoon (Key press =C)
int cartoon(cv::Mat &src, cv::Mat &dst )
{
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel x
    cv::Mat first = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel x
    cv::Mat dst1 = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the first processed image i.e) after perfroming 3*1 Multiplication for sobel y
    cv::Mat dss = cv::Mat::zeros(src.size(), CV_16SC3);
    // Declaring the second processed image i.e) after performing 1*3 Multiplication for sobel y
    cv::Mat dss1 = cv::Mat::zeros(src.size(), CV_16SC3);
    //Performing SOBEL X 
    for (int i = 1; i<src.rows-1;i++)
    {
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

        cv::Vec3s *dptr = first.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr[j][c] = (1*rptrm1[j-1][c]  
                            +2*rptr[j-1][c] 
                            +1*rptrp1[j-1][c])/4;
            }
        }  
    }
    for (int i = 1; i<first.rows-1;i++)
    {
        cv::Vec3s *rptrm12 = first.ptr<cv::Vec3s>(i-1);
        cv::Vec3s *rptr2 = first.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp12 = first.ptr<cv::Vec3s>(i+1);
        
        cv::Vec3s *dptr2 = dst1.ptr<cv::Vec3s>(i);

        for (int j = 1; j<first.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr2[j][c] = (-1*rptrm12[j-1][c]   +0*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
            }
        }  
    }
    //Performing SOBEL Y
    for (int i = 1; i<src.rows-1;i++)
    {
        cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

        cv::Vec3s *dptr = dss.ptr<cv::Vec3s>(i);

        for (int j = 1; j<src.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr[j][c] = (-1*rptrm1[j-1][c]  
                            +0*rptr[j-1][c] 
                            +1*rptrp1[j-1][c])/4;
            }
        }  
    }
    for (int i = 1; i<dss1.rows-1;i++)
    {
        cv::Vec3s *rptrm12 = dss.ptr<cv::Vec3s>(i-1);
        cv::Vec3s *rptr2 = dss.ptr<cv::Vec3s>(i);
        cv::Vec3s *rptrp12 = dss.ptr<cv::Vec3s>(i+1);
        
        cv::Vec3s *dptr2 = dss1.ptr<cv::Vec3s>(i);

        for (int j = 1; j<dss.cols-1;j++)
        {
            for (int c = 0; c<3; c++)
            {
                dptr2[j][c] = (1*rptrm12[j-1][c]   +2*rptrm12[j][c]  +1*rptrm12[j+1][c])/1;
            }
        }  
    }
    //Declaring Mat for scaling calculations
    cv::Mat dis;
    cv::Mat dis1;
    cv::Mat diss;
    cv::Mat diss1;

    //X SOBEL
    cv::convertScaleAbs(first,dis);
    cv::convertScaleAbs(dst1,dis1);
    //Y SOBEL
    cv::convertScaleAbs(dss,diss);
    cv::convertScaleAbs(dss1,diss1);

    cv::Mat xsob = dis1;
    cv::Mat ysob = diss1;

    cv::Mat result1;

    //Converting thier data type and taking thier sum and square root
    dis1.convertTo(dis1, CV_32F);
    diss1.convertTo(diss1, CV_32F);

    cv::multiply(dis1,dis1,dis1);
    cv::multiply(diss1,diss1,diss1);
    cv::addWeighted(dis1, 0.5, diss1, 0.5, 0, result1);
    cv::sqrt(result1,result1);

    result1.convertTo(result1, CV_8U);

    cv::Mat before = src;

    for (int i = 0; i < before.rows; i++) 
    {
        for (int j = 0; j < before.cols; j++) 
        {
            cv::Vec3b thres = result1.at<cv::Vec3b>(i, j);
            cv::Vec3b img = before.at<cv::Vec3b>(i, j);
            //Quantizing the image into a fixed number of levels 
            float b;
            b = 25.5;
            //changing the each color channel to only **3 possible color values
            img[0] = img[0]/b;
            img[0] = img[0]*b;
            //Thresholding the magnitude values
            if (thres[0] > 10)
            {
                img[0] = 0;
            }

            img[1] = img[1]/b;
            img[1] = img[1]*b;
            //Thresholding the magnitude values
            if (thres[1] > 10)
            {
                img[1] = 0;
            }

            img[2] = img[2]/b;
            img[2] = img[2]*b;
            //Thresholding the magnitude values
            if ( thres[2] > 10)
            {
                img[2] = 0;
            }

            dst.at<cv::Vec3b>(i, j) = img;
        }
    }
    return 0;
}
