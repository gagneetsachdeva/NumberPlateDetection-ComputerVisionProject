// PossiblePlate.h

#ifndef POSSIBLE_PLATE_H
#define POSSIBLE_PLATE_H

#include <string>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

/////////////////////////////////////////////////
class PossiblePlate {
public:
    // member variables ////////////////////////
   // The image of the plate
    cv::Mat imgPlate;

    // The grayscale image of the plate
    cv::Mat imgGrayscale;

    // The thresholded image of the plate
    cv::Mat imgThresh;

    // The location of the plate in the scene (as a rotated rectangle)
    cv::RotatedRect rrLocationOfPlateInScene;

    // The characters recognized in the plate
    std::string strChars;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    // A static function to sort PossiblePlate objects in descending order by the number of characters
    static bool sortDescendingByNumberOfChars(const PossiblePlate& ppLeft, const PossiblePlate& ppRight) {
        return (ppLeft.strChars.length() > ppRight.strChars.length());
    }

};


#endif		// end #ifndef POSSIBLE_PLATE_H

