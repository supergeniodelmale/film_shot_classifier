//
//  FilmStatisticEval.cpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 21.05.2025.
//

#include "FilmStatisticEval.hpp"

void FilmStatistics::oversampleInputData()
{
    if (oversampling_sliding_window.empty()) return;

    std::map<ShotType, double> aggregated_probs;

    // Sum probabilities from the sliding window
    for (const auto& sample : oversampling_sliding_window) {
        for (const auto& [type, prob] : sample.probabilities) {
            aggregated_probs[type] += prob;
        }
    }

    // Normalize
    double total = 0.0;
    for (const auto& [type, value] : aggregated_probs) {
        total += value;
    }

    std::map<ShotType, double> averaged_probs;
    for (const auto& [type, value] : aggregated_probs) {
        averaged_probs[type] = value / total;
    }

    // Determine most probable type
    auto max_it = std::max_element(
        averaged_probs.begin(),
        averaged_probs.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    if (max_it != averaged_probs.end()) {
        ShotType most_likely_type = max_it->first;

        current_sample_oversampled.predictedType = most_likely_type;
        current_sample_oversampled.probabilities = averaged_probs;
    }
}

void FilmStatistics::computeEnthropy()
{
    if (enthropy_sliding_window.empty()) return;

    std::map<ShotType, double> aggregated_probs;

    // Aggregate probabilities over the sliding window
    for (const auto& sample : enthropy_sliding_window) {
        for (const auto& [type, prob] : sample.probabilities) {
            aggregated_probs[type] += prob;
        }
    }

    // Normalize
    double total_probability = 0.0;
    for (const auto& [type, value] : aggregated_probs) {
        total_probability += value;
    }

    std::map<ShotType, double> normalized_probs;
    for (const auto& [type, value] : aggregated_probs) {
        normalized_probs[type] = value / total_probability;
    }

    // Compute entropy
   
    for (const auto& [type, prob] : normalized_probs) {
        if (prob > 0.0) {
            entropy -= prob * std::log2(prob);
        }
    }

}


void FilmStatistics::addFrameResult(const double timestampMs, const classification_result& result)
{
    total_frames++;
    
    if (total_frames%config.input_step == 0)
    {
        oversampling_sliding_window.push_back(result);
        enthropy_sliding_window.push_back(result);
        
        if(oversampling_sliding_window.size() > config.input_oversample)
            oversampling_sliding_window.pop_front();
        
        if(enthropy_sliding_window.size() > config.enthropy_window_size)
            enthropy_sliding_window.pop_front();
        
        evaluated_frames ++;
        oversampleInputData();
        computeEnthropy();
        timestamp_ms = timestampMs;
    }
    
    
}

void FilmStatistics::addConfigurationStruct(FilmStatisticsEvalConfig cfg)
{
    config = cfg;
}
