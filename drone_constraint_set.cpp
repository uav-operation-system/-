#include<iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\imgproc\types_c.h>

#include<vector>
#include<fstream>
#include<string>

using namespace std;
using namespace cv;

int main()
{	
	int th = 100;
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));//开运算的核
	Mat sample = imread("E:\\H_counter\\H_counter.png");

	vector<vector<Point>> contours_sample;
	vector<Vec4i>hierarchy;

	//FileStorage fswrite_contours("contours_sample.xml", FileStorage::WRITE);
	//FileStorage fswrite_hierarchy("hierarchy.xml", FileStorage::WRITE);


	cvtColor(sample, sample, CV_BGR2GRAY);
	threshold(sample, sample, th, 255, THRESH_BINARY);
	morphologyEx(sample, sample, MORPH_OPEN, element);
	findContours(sample, contours_sample, hierarchy, CV_RETR_LIST, CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours_sample.size(); ++i)
	{
		cout << "第" << i << "行：";
		for (size_t j = 0; j < contours_sample[i].size(); j++)
		{
			cout << contours_sample[i][j];
		}
		cout << endl;
	}

	//圆，圆心，半径
	vector<Vec3f> circles;

	vector<int> radius;

	//初始图象，处理图像，掩膜图像(ROI)
	//Mat src_image = imread("E:\\H_counter\\target.jpg");
	Mat src_image;
	Mat mid_image;

	Size sz = src_image.size();//size

	vector<vector<Point>> contours_mask;
	vector<Vec4i> hierarchy_mask;



	VideoCapture capture;
	capture.open(0);
	//capture.open("E:\\H_counter\\fortarget.mp4");
	while (true) {
		capture >> src_image;
		namedWindow("src", WINDOW_GUI_NORMAL);
		imshow("src", src_image);
		

		//imshow("src", src_image);
		//waitKey(0);
		cvtColor(src_image, mid_image, CV_BGR2GRAY);//gray
		GaussianBlur(mid_image, mid_image, Size(9, 9), 2, 2);

		//霍夫变换
		HoughCircles(mid_image, circles, CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);
		cout << circles.size();
		
		Mat mask = Mat::zeros(src_image.size(), CV_8U);
		Mat dst = Mat::zeros(src_image.size(), src_image.type());

		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			//center.push_back(cvRound(circles[i][0]), cvRound(circles[i][1]));
			radius.push_back(cvRound(circles[i][2]));
			//绘制圆心
			//circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
			//绘制圆轮廓
			//circle(mask_image, center[i], radius[i], Scalar(255), 3, 8, 0);

			circle(mask, center, radius[i], Scalar(255), -1);
			mid_image.copyTo(dst, mask);

			namedWindow("mask", WINDOW_GUI_NORMAL);
			namedWindow("dst", WINDOW_GUI_NORMAL);
			namedWindow("image", WINDOW_GUI_NORMAL);
			imshow("image", mid_image);
			imshow("mask", mask);
			imshow("dst", dst);
			//waitKey(0);

			threshold(dst, dst, th, 255, THRESH_BINARY);
			morphologyEx(dst, dst, MORPH_OPEN, element);//开 MORPH_OPEN
			findContours(dst, contours_mask, hierarchy_mask, CV_RETR_LIST, CHAIN_APPROX_SIMPLE);
			namedWindow("All Contours", WINDOW_GUI_NORMAL);

			drawContours(dst, contours_mask, -1, Scalar(0, 0, 255), 1, 8, hierarchy_mask);
			imshow("All Contours", dst);
			/*for (size_t i = 0; i < contours_mask.size(); ++i)
			{
				cout << "第" << i << "行：";
				for (size_t j = 0; j < contours_mask[i].size(); j++)
				{
					cout << contours_mask[i][j];
				}
				cout << endl;
			}*/

			if ((contours_sample.size() == 0) || (contours_mask.size() == 0))
			{
				cout << "There are no contours to match" << endl;
			
			}

			double hu = 1.0, huTmp = 0.0;
			const int SIZE = contours_mask.size();
			int m = -1, n = -1;

			for (int i = 0; i < contours_sample.size(); i++)
			{
				for (int j = 0; j < SIZE; j++)
				{
					huTmp = matchShapes(contours_sample[i], contours_mask[j], CV_CONTOURS_MATCH_I1, 0);

					if (huTmp < hu)
					{
						hu = huTmp;

						m = i;
						n = j;
					}
				}
			}

			cout << "************m = " << m << "; n = " << n << "; hu = " << hu << endl;

			Point2f rect[4];
			RotatedRect box = minAreaRect(Mat(contours_mask[n]));
			box.points(rect);

			for (int j = 0; j < 4; j++)
			{
				line(src_image, rect[j], rect[(j + 1) % 4], Scalar(50, 255, 50), 2, 8);  //绘制最小外接矩形每条边
			}
			namedWindow("rect", WINDOW_GUI_NORMAL);
			imshow("rect", src_image);



			//if (i = 0) break;
		}
		if (waitKey(1)==999999999999999999)
			break;
	}
	return 0;
}
