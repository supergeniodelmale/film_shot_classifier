#include <opencv2/opencv.hpp>
#include <iostream>
//#include <opencvi2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <filesystem>
#include "FilmShotClassifier.hpp"

int main()
{
// just a code prototype, comment out to build
    std::string data_path = "path";
    std::string haar_filter_path1 = "path";
    std::string haar_filter_path2 = "path";
    
    
//    ImageLoader image_loader(data_path);
//    Preprocessing preprocess;
//    FeatureDetector frontal_face_detector(haar_filter_path1);
//    FeatureDetector side_face_detector(haar_filter_path2);
//    ShotFeatureExtractor shot_feature_extractor;
//    ShotClassifier shot_classifier;
//    FilmStatistics film_stats;
//    
//    std::vector<DetectedFeature> detected_frontal_face_vect;
//    std::vector<DetectedFeature> detected_left_face_vect;
//    std::vector<DetectedFeature> features_vect;
//    
//    ShotFeatures shot_features;
//    ClassificationResult classification_result;
//    
//    while(image_loader.hasNextFrame())
//    {
//        preprocess.LoadFrame(image_loader.nextFrame());
//        //preprocessing methods calling...
//        
//        detected_frontal_face_vect = frontal_face_detector.detect(preprocess.GetProcessedImage());
//        detected_left_face_vect = side_face_detector.detect(preprocess.GetProcessedImage());
//        // concatenate vectors... -> features_vect
//        
//        shot_features = shot_feature_extractor.extract(preprocess.GetProcessedImage(), features_vect);
//        classification_result = shot_classifier.classify(shot_features);
//        film_stats.addFrameResult(image_loader.getCurrentTimestamp(), classification_result);
//    }
        
    return 0;
}
