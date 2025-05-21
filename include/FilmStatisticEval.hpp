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
#include "FeatureProccesorAndClassifier.hpp"

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
    size_t step = 1; ///< Frame step size – allows skipping frames during analysis
    std::map<ShotType, int> shot_counts; ///< Count of each shot type
    std::vector<std::pair<double, ShotType>> timeline; ///< Timeline of shot types (timestamp → type)
    int totalFrames = 0; ///< Total number of frames processed

public:
    /**
     * @brief Default constructor (analyzes every frame).
     */
    FilmStatistics() = default;

    /**
     * @brief Constructor with custom frame stride.
     * @param stride Number of frames to skip between evaluations (e.g. 1 = every frame, 5 = every 5th).
     */
    FilmStatistics(size_t stride) : step(stride) {};

    /**
     * @brief Adds the classification result for a single frame.
     *
     * @param timestampMs Timestamp of the frame in milliseconds.
     * @param result Classification result containing shot type.
     */
    void addFrameResult(double timestampMs, const ClassificationResult& result);

    /**
     * @brief Sets the number of frames to skip during analysis.
     *
     * @param stride New step size (e.g., 1 = no skipping).
     */
    void setFrameStep(size_t stride);

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
};

#endif /* FilmStatisticEval_hpp */

