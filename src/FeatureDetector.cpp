//
//  FeatureDetector.cpp
//  Film_type_classifier
//
//  Main developer: Mateusz Miroslaw Lis
//

#include "FeatureDetector.hpp"

FeatureDetector::FeatureDetector(const std::string& cascade_path) {
    if (!cascade.load(cascade_path)) {
        throw std::runtime_error("Failed to load Haar cascade from: " + cascade_path);
    }
}

// Returns an array of bounding boxes
std::vector<cv::Rect> FeatureDetector::detect(const cv::Mat& image) {
    std::vector<cv::Rect> faces;
    cv::Mat gray;

    // Convert to grayscale if needed
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    // Detect faces
    cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

    return faces;
}

