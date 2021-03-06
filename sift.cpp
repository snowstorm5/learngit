// sift.cpp: 定义控制台应用程序的入口点。
//


#include <opencv.hpp>
#include <xfeatures2d.hpp>
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;


int main()
{
	Mat img = imread("1-3.png");
	imshow("img", img);
	Mat img_model = imread("model1.png");
	Mat mask = imread("mask.png");
	Mat mask_Gray;
	cvtColor(mask, mask_Gray, CV_BGR2GRAY);
	threshold(mask_Gray, mask_Gray, 250, 255,THRESH_BINARY);
	//imshow("mask_Gray", mask_Gray);

	Ptr<SIFT> sift = SIFT::create();
	vector<KeyPoint> keypoints1, keypoints2;
	sift->detect(img, keypoints1);
	sift->detect(img_model, keypoints2,mask_Gray);
	cout << "img total" << keypoints1.size() << endl;
	cout << "img_model total" << keypoints2.size() << endl;
	Mat keypoints1_img, keypoints2_img;
	drawKeypoints(img, keypoints1, keypoints1_img);
	drawKeypoints(img_model, keypoints2, keypoints2_img);
	imshow("keypoints1_img", keypoints1_img);
	imshow("keypoints2_img", keypoints2_img);
	Mat descriptions1, descriptions2;
	sift->compute(img, keypoints1, descriptions1);
	sift->compute(img_model, keypoints2, descriptions2);

	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
	vector<vector<DMatch>> matchPoints;
	vector<DMatch> matches;
	matcher->knnMatch(descriptions2, descriptions1, matchPoints, 2);
	cout << "total match points" << matchPoints.size() << endl;
	for (int i = 0; i < matchPoints.size(); i++)
	{
		if (matchPoints[i][0].distance < 0.6* matchPoints[i][1].distance)
		{
			matches.push_back(matchPoints[i][0]);
		}
	}

	Mat match_img;
	drawMatches(img_model, keypoints2, img, keypoints1, matches, match_img);
	imshow("match_img", match_img);


	waitKey(0);
    return 0;
}

