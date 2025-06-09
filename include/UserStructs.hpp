//
//  UserStructs.hpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 06.06.2025.
//

#ifndef UserStructs_hpp
#define UserStructs_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

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
 * @struct ShotFeatures
 * @brief Represents extracted geometric and area-based properties from a video frame.
 *
 * ShotFeatures holds intermediate data used for shot type classification. It includes
 * statistics like the number of detected objects, the total and largest object area,
 * and the coordinates of their centers.
 *
 * The `object_centers` vector is ordered by object size, with the largest first.
 */
struct ShotFeatures {
    int object_count = 0;                ///< Number of detected objects in the frame

    double largest_object_area = 0.;     ///< Area of the largest object detected
    double total_object_area = 0.;       ///< Sum of all object areas
    double total_area = 0.;              ///< Total area of the frame (width * height)

    // for good shot classification + we can add some statistical metrics
    std::vector<cv::Point2f> object_centers; ///< Center points of detected objects, ordered by size
};

/**
 * @enum ShotType
 * @brief Enumeration representing the type of cinematic shot.
 *
 * Used to categorize frames into typical shot sizes.
 */
enum class ShotType {
    CLOSE_UP,   ///< A close-up shot, typically a large face
    MEDIUM,     ///< A medium shot, e.g., upper body
    WIDE,       ///< A wide shot, showing full figures or environment
    UNKNOWN     ///< Could not determine shot type
};

/**
 * @struct ClassificationResult
 * @brief Contains the result of shot type classification.
 *
 * Holds both the most likely predicted shot type and a probability
 * distribution over all possible shot types.
 *
 *Set probability of detected ShotType to 1 if there is not better metrics
 */
struct classification_result {
    ShotType predictedType = ShotType::UNKNOWN;             ///< Most probable shot type, redundant but efective
    std::map<ShotType, double> probabilities;               ///< Probability distribution across shot types
};

struct FilmStatisticsEvalConfig {
    size_t input_step = 1; ///< Frame step size – allows skipping frames during analysis
    size_t input_oversample = 1; ///< Oversampling buffer size
    ///<
    size_t entropy_window_size = 20; ///< enthropy window size
    size_t entropy_variance_window_size = 30; ///< 0-not computed
    double cut_detect_entropy_threshold = 1;
    
    bool output_ratios_series = false;
    size_t output_prob_time_series = 1; ///< 0-not computed, 1-x output oversampling
    size_t output_entropy_time_series = 1; ///< 0-not computed, 1-x output oversampling
    size_t output_entropy_variance_time_series = 1; ///< 0-not computed, 1-x output oversampling
    
    
};

#endif /* UserStructs_hpp */
