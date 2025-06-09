//
//  FilmStatisticEval.hpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 21.05.2025.
//

#ifndef FilmStatisticEval_hpp
#define FilmStatisticEval_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <numeric>
#include "UserStructs.hpp"

/**
 * @class FilmStatistics
 * @brief Collects and analyzes shot type data across a sequence of video frames.
 *
 * The FilmStatistics class is responsible for aggregating classification results
 * from multiple frames and producing summary statistics about shot types used
 * in a video. It also supports time-based analysis and exporting results.
 *
 * This class is typically used at the end of a processing pipeline, after each
 * frame has been classified. It stores both a timeline and cumulative counts of
 * different shot types.
 *
 * The analysis can be configured to skip frames using a configurable `step`,
 * which allows subsampling of the video.
 *
 * Example usage:
 * @code
 *   FilmStatistics stats(5); // analyze every 5th frame
 *   stats.addFrameResult(1.2, result);
 *   stats.printSummary();
 *   stats.exportToCSV("shots.csv");
 * @endcode
 *
 * @author Marek Tatýrek
 * @date 2025
 * @see ClassificationResult
 * @see ShotType
 */
class FilmStatistics
{
    std::map<ShotType, int> shot_counts; ///< Count of each shot type
    std::vector<std::pair<double, std::map<ShotType, double>>> prob_timeline;
    std::vector<std::pair<double, ShotType>> shot_type_timeline; ///< Timeline of shot types (timestamp -> type)
    std::vector<std::pair<double, double>> enthropy_timeline; ///<  timestamp -> enthropy in previous config.enthropy_window_size samples
    std::vector<std::pair<double, double>> enthropy_variance_timeline;
    
    std::deque<classification_result> oversampling_sliding_window;
    std::deque<classification_result> entropy_sliding_window;
    std::deque<double> entropy_variance_sliding_window;
    
    int total_frames = 0;///< Total number of frames processed
    int evaluated_frames = 0; ///< Number of samples in statistic
    
    classification_result current_sample_oversampled;
    double entropy = 0.0;
    double timestamp_ms = 0.0;
    double entropy_variance = 0.0;
    bool is_cut = false;
    
    FilmStatisticsEvalConfig config;
    
    int start_delay = 0; //in frames
    double frame_time_measurement;
    
    void normalizeProbs(std::map<ShotType, double> & aggregated_probs);
    void aggregateSlidingWindowProbs(std::deque<classification_result> & sliding_window, std::map<ShotType, double> & aggregated_probs);
    void findShotTypeMaxProb(std::map<ShotType, double> & aggregated_probs, classification_result & sample);
    
    void oversampleInputData();
    void computeEntropy();
    void computeEntropyVariance();
    void detectCuts();
    
    void outputStatistics();
    void appendSampleToTimeline(std::vector<std::pair<double, std::map<ShotType, double>>> & timeline);
    void appendSampleToTimeline(std::vector<std::pair<double, ShotType>> & timeline);
    void appendEntropyToTimeline(std::vector<std::pair<double, double>> & timeline);
    void appendEntropyVarianceToTimeline(std::vector<std::pair<double, double>> & timeline);
    
    

public:
    /**
     * @brief Default constructor (analyzes every frame).
     */
    FilmStatistics() = default;

//    /**
//     * @brief Constructor with custom frame stride.
//     * @param stride Number of frames to skip between evaluations (e.g. 1 = every frame, 5 = every 5th).
//     */
//    FilmStatistics(size_t stride) : step(stride) {};
    
    void addConfigurationStruct(FilmStatisticsEvalConfig const & cfg);

    /**
     * @brief Adds the classification result for a single frame.
     *
     * @param timestampMs Timestamp of the frame in milliseconds.
     * @param result Classification result containing shot type.
     */
    void addFrameResult(const double timestampMs, const classification_result & result);


    /**
     * @brief Exports all collected statistics to a CSV file.
     *
     * The CSV includes both timeline data and summary shot counts.
     *
     * @param path File path to export the data to.
     */
    void exportToCSV(const std::string& path) const;

    /**
     * @brief Prints a textual summary of the shot distribution to the console.
     */
    void printSummary() const;
    
    
    
    // -- temporary --
    std::vector<std::pair<double, double>> & getEntropy(){return enthropy_timeline; };
    std::vector<std::pair<double, double>> & getEntropyVAriance(){return enthropy_variance_timeline; };
    std::vector<std::pair<double, ShotType>> & getShotType(){return shot_type_timeline;}
    // -- temporary --
};

#endif /* FilmStatisticEval_hpp */

