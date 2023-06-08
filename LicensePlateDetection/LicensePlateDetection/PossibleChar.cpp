#include "PossibleChar.h"
/**
 * Constructs a PossibleChar object with the given contour.
 * @param _contour The contour of the character.
 */
PossibleChar::PossibleChar(std::vector<cv::Point> _contour) {
    contour = _contour;

    boundingRect = cv::boundingRect(contour);

    intCenterX = (boundingRect.x + boundingRect.x + boundingRect.width) / 2;
    intCenterY = (boundingRect.y + boundingRect.y + boundingRect.height) / 2;

    dblDiagonalSize = sqrt(pow(boundingRect.width, 2) + pow(boundingRect.height, 2));

    dblAspectRatio = static_cast<float>(boundingRect.width) / static_cast<float>(boundingRect.height);
}
