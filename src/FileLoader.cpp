//
//  FileLoader.cpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 20.05.2025.
//

#include "../include/FileLoader.hpp"
#include <opencv2/opencv.hpp>
#include <stdexcept>

// ImageLoader Implementation
bool ImageLoader::hasNextFrame() const {
    static bool frameReturned = false;
    return !frameReturned;
}

cv::Mat& ImageLoader::nextFrame() {
    static cv::Mat image;
    static bool loaded = false;

    if (!loaded) {
        image = cv::imread(source_path);
        if (image.empty()) {
            throw std::runtime_error("Failed to load image: " + source_path);
        }
        loaded = true;
    }

    if (!hasNextFrame()) {
        throw std::runtime_error("No more frames available");
    }

    static bool frameReturned = false;
    frameReturned = true;
    return image;
}

double ImageLoader::getCurrentTimestamp() const {
    return 0.0; // Single images don't have timestamps
}

// Preprocessing Implementation
Preprocessing::~Preprocessing() = default;

void Preprocessing::LoadFrame(cv::Mat& new_image) {
    image = new_image.clone();
}

cv::Mat& Preprocessing::GetProcessedImage() {
    return image;
}

