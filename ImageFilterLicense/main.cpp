#include<opencv2/opencv.hpp>

int main()

{
//Load the input image
	cv::Mat image = cv::imread("image20.jpg");
	
	//Check if image variable has input image or not
	if (image.empty())
	{
		std::cout << "Failed to load the image" << std::endl;
			return -1;
	}

	cv::imshow("Input Image",image);
    // Convert the image to grayscale
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    cv::imshow("Grayscale Image", grayImage);

   /* // Maximize contrast
    cv::Mat contrastImage;
    cv::equalizeHist(grayImage, contrastImage);
    cv::imshow("Contrast Image", contrastImage);*/
    
    // Enhance the contrast using CLAHE (Contrast Limited Adaptive Histogram Equalization)
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(4.0);
    cv::Mat contrastImage;
    clahe->apply(grayImage, contrastImage);
    cv::imshow("Contrast Image", contrastImage);

    // Apply adaptive thresholding
    cv::Mat thresholdImage;
    cv::adaptiveThreshold(contrastImage, thresholdImage, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
    cv::imshow("Threshold Image", thresholdImage);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresholdImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Draw contours on the original image
    cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 2);
    cv::imshow("Contours", image);

    // Create a mask to draw contours on
    cv::Mat contourMask = cv::Mat::zeros(image.size(), CV_8UC1);

    // Draw contours on the mask
    cv::drawContours(contourMask, contours, -1, cv::Scalar(255), cv::FILLED);

    // Apply the mask to the original image
    cv::Mat resultImage;
    image.copyTo(resultImage, contourMask);

    // Show the final result
    cv::imshow("Masked Contours", resultImage);
	cv::waitKey(0);
	return 0;
}