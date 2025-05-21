//
//  FeatureDetector.hpp
//  Film_type_classifier
//
//  Created by Miroslaw on 20.05.2025.
//

#ifndef FeatureDetector_hpp
#define FeatureDetector_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

/**
 * @struct DetectedFeature
 * @brief Represents a single detected feature in an image, with label and bounding box.
 *
 * This structure is used to return information about each detected object,
 * such as its class label and location in the image.
 */
struct DetectedFeature {
    std::string label;    ///< Type or name of the detected object (e.g., "face")
    cv::Rect boundingBox; ///< Bounding box of the detected object in image coordinates
};

/**
 * @class FeatureDetector
 * @brief Detects visual features (e.g., faces) in an image using Haar cascade models.
 *
 * This class provides a simple interface for loading a Haar cascade model and applying it
 * to an input image to detect features such as faces, bodies, etc. The detector returns
 * a list of `DetectedFeature` results, each containing a label and bounding box.
 *
 * It may be beneficial to sort the output vector by bounding box size (e.g., largest first),
 * to simplify downstream feature selection or analysis.
 *
 * @see DetectedFeature
 */
class FeatureDetector
{
    cv::CascadeClassifier cascade; ///< The loaded Haar cascade classifier used for detection.

public:
    /**
     * @brief Constructs the detector and immediately loads the model.
     * @param modelPath Path to the Haar cascade XML model file.
     */
    FeatureDetector(std::string& modelPath) { loadModel(modelPath); }

    /**
     * @brief Constructs the detector without loading a model initially.
     */
    FeatureDetector() {}

    /**
     * @brief destructor.
     */
    ~FeatureDetector() = default;

    /**
     * @brief Loads a Haar cascade model from the specified file path.
     * @param modelPath Path to the Haar cascade XML model file.
     */
    void loadModel(const std::string& modelPath);

    /**
     * @brief Detects features in the given image.
     *
     * Applies the currently loaded Haar cascade model to the image and
     * returns a list of bounding boxes with associated labels.
     *
     * @param image The image in which to detect features.
     * @return A vector of `DetectedFeature` representing all detected objects.
     */
    std::vector<DetectedFeature> detect(const cv::Mat& image); // maybe it will be fine to sort the vector from biggest BB, so we gonna have easier job afterwards
};

#endif /* FeatureDetector_hpp */

