/*
Project Title - Automatic License Plate Detection System using OpenCV and KNN
Authors - Nikhita Tithi, Harika Chadalavada, Gagneet Sachdeva, Shubham Patil
Description - This research developed a real-time license plate detection system using OpenCV 
and the K-nearest neighbors (KNN) algorithm. It aimed for high accuracy (>87%) and versatility 
in handling various conditions. Techniques included preprocessing, KNN training, and hyperparameter 
tuning. With K=1, an accuracy of 91.6% was achieved. The research contributes to a robust license 
plate detection system for real-world applications.
*/
#include "Main.h"

void drawRedRectangleAroundPlate(cv::Mat& imgOriginalScene, PossiblePlate& licPlate);
void writeLicensePlateCharsOnImage(cv::Mat& imgOriginalScene, PossiblePlate& licPlate);
cv::Mat gammaCorrection(const cv::Mat& imgOriginalScene, float gamma)
{
    // Create a lookup table for gamma correction
    cv::Mat lookupTable(1, 256, CV_8UC1);
    uchar* lut = lookupTable.ptr();
    for (int i = 0; i < 256; ++i)
    {
        lut[i] = cv::saturate_cast<uchar>(pow((i / 255.0), gamma) * 255.0);
    }

    // Apply gamma correction using the lookup table
    cv::Mat correctedImage;
    cv::LUT(imgOriginalScene, lookupTable, correctedImage);

    return correctedImage;
}
int main(void)
{
    bool blnKNNTrainingSuccessful = loadKNNDataAndTrainKNN(); // attempt KNN training

    if (!blnKNNTrainingSuccessful)
    {
        // if KNN training was not successful, show error message and exit program
        std::cout << std::endl << std::endl << "error: error: KNN training was not successful" << std::endl << std::endl;
        return 0;
    }

    cv::Mat imgOriginalScene; // input image
   
    imgOriginalScene = cv::imread("image23.jpg"); // open image

    if (imgOriginalScene.empty())
    {
        // if unable to open image, show error message and exit program
        std::cout << "error: image not read from file\n\n";
        _getch(); // may have to modify this line if not using Windows
        return 0;
    }
    // Perform gamma correction with a gamma value of 1.5 (adjust gamma value as needed)
    float gammaValue = 1.5;
    cv::Mat correctedImage = gammaCorrection(imgOriginalScene, gammaValue);

    // Display the original and corrected images
    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Original Image", imgOriginalScene);

    cv::namedWindow("Corrected Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Corrected Image", correctedImage);

    cv::waitKey(0);
    cv::destroyAllWindows();


    std::vector<PossiblePlate> vectorOfPossiblePlates = PlateDetectionInScene(correctedImage); // detect plates

    vectorOfPossiblePlates = CharDetectionInPlates(vectorOfPossiblePlates); // detect chars in plates

    cv::imshow("imgOriginalScene", correctedImage); // show scene image

    if (vectorOfPossiblePlates.empty())
    {
        // if no plates were found, inform user and exit program
        std::cout << std::endl << "no license plates were detected" << std::endl;
    }
    else
    {
        // if at least one plate is found
        std::sort(vectorOfPossiblePlates.begin(), vectorOfPossiblePlates.end(), PossiblePlate::sortDescendingByNumberOfChars);
        PossiblePlate licPlate = vectorOfPossiblePlates.front(); // assume the plate with the most recognized chars is the actual plate

        cv::imshow("imgPlate", licPlate.imgPlate); // show crop of plate and threshold of plate
        cv::imshow("imgThresh", licPlate.imgThresh);

        if (licPlate.strChars.empty())
        {
            // if no chars were found in the plate, show message and exit program
            std::cout << std::endl << "no characters were detected" << std::endl << std::endl;
            return 0;
        }

        drawRedRectangleAroundPlate(correctedImage, licPlate); // draw red rectangle around plate

        std::cout << std::endl << "license plate read from image = " << licPlate.strChars << std::endl;
        std::cout << std::endl << "-----------------------------------------" << std::endl;

        writeLicensePlateCharsOnImage(correctedImage, licPlate); // write license plate text on the image

        cv::imshow("imgOriginalScene", correctedImage); // re-show scene image

        cv::imwrite("imgOriginalScene.png", correctedImage); // write image out to file
    }

    cv::waitKey(0); // hold windows open until user presses a key

    return 0;
}

void drawRedRectangleAroundPlate(cv::Mat& correctedImage, PossiblePlate& licPlate)
{
    cv::Point2f p2fRectPoints[4];
    licPlate.rrLocationOfPlateInScene.points(p2fRectPoints); // get 4 vertices of rotated rect

    for (int i = 0; i < 4; i++)
    {
        // draw 4 red lines
        cv::line(correctedImage, p2fRectPoints[i], p2fRectPoints[(i + 1) % 4], SCALAR_RED, 2);
    }
}

void writeLicensePlateCharsOnImage(cv::Mat& correctedImage, PossiblePlate& licPlate)
{
    cv::Point ptCenterOfTextArea; // center of the area the text will be written to
    cv::Point ptLowerLeftTextOrigin; // bottom left of the area that the text will be written to

    int intFontFace = cv::FONT_HERSHEY_SIMPLEX; // font type
    double dblFontScale = (double)licPlate.imgPlate.rows / 30.0; // font scale based on height of plate area
    int intFontThickness = (int)std::round(dblFontScale * 1.5); // font thickness based on font scale
    int intBaseline = 0;

    cv::Size textSize = cv::getTextSize(licPlate.strChars, intFontFace, dblFontScale, intFontThickness, &intBaseline); // get text size

    ptCenterOfTextArea.x = (int)licPlate.rrLocationOfPlateInScene.center.x; // horizontal location of the text area is the same as the plate

    if (licPlate.rrLocationOfPlateInScene.center.y < (correctedImage.rows * 0.75))
    {
        // if the license plate is in the upper 3/4 of the image, write the chars below the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) + (int)std::round((double)licPlate.imgPlate.rows * 1.6);
    }
    else
    {
        // if the license plate is in the lower 1/4 of the image, write the chars above the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) - (int)std::round((double)licPlate.imgPlate.rows * 1.6);
    }

    ptLowerLeftTextOrigin.x = (int)(ptCenterOfTextArea.x - (textSize.width / 2)); // calculate the lower left origin of the text area
    ptLowerLeftTextOrigin.y = (int)(ptCenterOfTextArea.y + (textSize.height / 2)); // based on the text area center, width, and height

    // write the text on the image
    cv::putText(correctedImage, licPlate.strChars, ptLowerLeftTextOrigin, intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);
}
