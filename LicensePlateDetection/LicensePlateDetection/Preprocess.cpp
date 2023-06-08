#include "Preprocess.h"
#include <opencv2/opencv.hpp>

/**
 * Visualizes the Histogram of Oriented Gradients (HOG) on the original image.
 * @param imgOriginal The original image.
 */
void visualizeHOG(cv::Mat& imgOriginal) {
    std::cout << "Visualizing HOG" << std::endl;
    std::vector<cv::Point> foundLocations;
    std::vector<double> foundWeights;
    cv::HOGDescriptor hog;

    hog.detect(imgOriginal, foundLocations, foundWeights);

    for (size_t i = 0; i < foundLocations.size(); ++i) {
        cv::Rect rect(foundLocations[i].x, foundLocations[i].y, hog.winSize.width, hog.winSize.height);
        cv::rectangle(imgOriginal, rect, cv::Scalar(0, 255, 0), 2);
    }

   // cv::imshow("HOG Visualization", imgOriginal);
}

/**
 * Preprocesses the original image to obtain the grayscale and thresholded images.
 * @param imgOriginal The original image.
 * @param imgGrayscale The output grayscale image.
 * @param imgThresh The output thresholded image.
 */
void preprocess(cv::Mat& imgOriginal, cv::Mat& imgGrayscale, cv::Mat& imgThresh) {
    visualizeHOG(imgOriginal);
    imgGrayscale = extractValue(imgOriginal);
    cv::Mat imgMaxContrastGrayscale = maximizeContrast(imgGrayscale);
    cv::Mat imgBlurred;
    cv::GaussianBlur(imgMaxContrastGrayscale, imgBlurred, GAUSSIAN_SMOOTH_FILTER_SIZE, 0);
    cv::adaptiveThreshold(imgBlurred, imgThresh, 255.0, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE, ADAPTIVE_THRESH_WEIGHT);
}

/**
 * Extracts the value channel from the original image in HSV color space.
 * @param imgOriginal The original image.
 * @return The extracted value channel image.
 */
cv::Mat extractValue(cv::Mat& imgOriginal) {
    cv::Mat imgHSV;
    std::vector<cv::Mat> vectorOfHSVImages;
    cv::Mat imgValue;

    cv::cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV);
    cv::split(imgHSV, vectorOfHSVImages);
    imgValue = vectorOfHSVImages[2];

    return imgValue;
}
/**
 * Enhances the contrast of the grayscale image using top-hat and black-hat morphological operations.
 * @param imgGrayscale The grayscale image.
 * @return The contrast-enhanced image.
 */
cv::Mat maximizeContrast(cv::Mat& imgGrayscale) {
    cv::Mat imgTopHat;
    cv::Mat imgBlackHat;
    cv::Mat imgGrayscalePlusTopHat;
    cv::Mat imgGrayscalePlusTopHatMinusBlackHat;

    cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::morphologyEx(imgGrayscale, imgTopHat, cv::MORPH_TOPHAT, structuringElement);
    cv::morphologyEx(imgGrayscale, imgBlackHat, cv::MORPH_BLACKHAT, structuringElement);

    imgGrayscalePlusTopHat = imgGrayscale + imgTopHat;
    imgGrayscalePlusTopHatMinusBlackHat = imgGrayscalePlusTopHat - imgBlackHat;

    return imgGrayscalePlusTopHatMinusBlackHat;
}
