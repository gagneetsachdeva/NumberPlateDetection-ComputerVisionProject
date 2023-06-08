#ifndef PLATE_DETECTION_H
#define PLATE_DETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Main.h"
#include "PossiblePlate.h"
#include "PossibleChar.h"
#include "Preprocess.h"
#include "CharDetection.h"

/**
 * Factor to adjust the width of the detected license plate region.
 */
const double PLATE_WIDTH_PADDING_FACTOR = 1.3;

/**
 * Factor to adjust the height of the detected license plate region.
 */
const double PLATE_HEIGHT_PADDING_FACTOR = 1.5;

/**
 * Detects license plates in the given scene image.
 * @param imgOriginalScene The original scene image.
 * @return A vector of PossiblePlate objects representing the detected license plates.
 */
std::vector<PossiblePlate> PlateDetectionInScene(cv::Mat& imgOriginalScene);

/**
 * Finds possible characters in the given thresholded image.
 * @param imgThresh The thresholded image.
 * @return A vector of PossibleChar objects representing the possible characters.
 */
std::vector<PossibleChar> findPossibleCharsInScene(cv::Mat& imgThresh);

/**
 * Extracts the license plate from the original image based on the matched characters.
 * @param imgOriginal The original image.
 * @param vectorOfMatchingChars A vector of matched PossibleChar objects.
 * @return A PossiblePlate object representing the extracted license plate.
 */
PossiblePlate extractPlate(cv::Mat& imgOriginal, std::vector<PossibleChar>& vectorOfMatchingChars);

#endif // PLATE_DETECTION_H
