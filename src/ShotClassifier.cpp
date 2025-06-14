#include "ShotClassifier.hpp"

int ShotClassifier::classify(const std::vector<cv::Rect>& shot_features)
{
    int area_threshold = 100
    
    if (shot_features.empty()) {
        return 0; // no faces detected
    }

    // shot_features expected sorted descending by area,
    // so smallest face is last element
    const cv::Rect& smallest_face = shot_features.back();

    if (smallest_face.area() < area_threshold) {
        return 0; // smallest face too small
    }

    // Dummy logic: if largest face area > 4* smallest face area return 2 else 1
    int largest_area = shot_features.front().area();
    int smallest_area = smallest_face.area();

    if (largest_area > 4 * smallest_area) {
        return 2;
    }
    return 1;
}
