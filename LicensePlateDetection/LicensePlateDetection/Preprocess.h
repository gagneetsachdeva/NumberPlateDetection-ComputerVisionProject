// Preprocess.h

#ifndef PREPROCESS_H
#define PREPROCESS_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

// global variables /////////////////////////////////////////////////////////////////
const cv::Size GAUSSIAN_SMOOTH_FILTER_SIZE = cv::Size(5, 5);
const int ADAPTIVE_THRESH_BLOCK_SIZE = 19;
const int ADAPTIVE_THRESH_WEIGHT = 9;

// function prototypes //////////////////////////////////////////////////////////////
/**
 * Preprocess the image by applying various transformations.
 * @param imgOriginal The original input image.
 * @param imgGrayscale The grayscale image after preprocessing.
 * @param imgThresh The thresholded image after preprocessing.
 */
void preprocess(cv::Mat &imgOriginal, cv::Mat &imgGrayscale, cv::Mat &imgThresh);
/**
 * Extract the value channel from the original image.
 * @param imgOriginal The original input image.
 * @return The extracted value channel image.
 */
cv::Mat extractValue(cv::Mat &imgOriginal);
/**
 * Maximize the contrast of the grayscale image.
 * @param imgGrayscale The grayscale image to maximize contrast.
 * @return The image with maximized contrast.
 */
cv::Mat maximizeContrast(cv::Mat &imgGrayscale);


#endif	// PREPROCESS_H
