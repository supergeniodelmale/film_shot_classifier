//
//  FilmStatisticEval.cpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 21.05.2025.
//

#include "FilmStatisticEval.hpp"


// input data computing functions
void FilmStatistics::normalizeProbs(std::map<ShotType, double> & aggregated_probs)
{
    if(!aggregated_probs.empty())
    {
        double total = 0.0;
        for (const auto& [type, value] : aggregated_probs)
            total += value;
        
        if(total == 0.)
        {
            double uniform_prob = 1.0 / aggregated_probs.size();
            
            for (auto& [type, prob] : aggregated_probs)
                prob = uniform_prob;
            
            return;
        }
        
        std::map<ShotType, double> averaged_probs;
        
        for (const auto& [type, value] : aggregated_probs)
            aggregated_probs[type] = value / total;
        
    }
}

void FilmStatistics::aggregateSlidingWindowProbs(std::deque<classification_result> & sliding_window, std::map<ShotType, double> & aggregated_probs)
{
    aggregated_probs.clear();
    
    if(!sliding_window.empty())
    {
        for (const auto& sample : sliding_window)
        {
            for (const auto& [type, prob] : sample.probabilities)
                aggregated_probs[type] += prob;
            
        }
    }
}

void FilmStatistics::findShotTypeMaxProb(std::map<ShotType, double> & probs, classification_result & sample)
{
    auto max_it = std::max_element(
        probs.begin(),
        probs.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    if (max_it != probs.end()) {
        ShotType most_likely_type = max_it->first;

        sample.predictedType = most_likely_type;
        sample.probabilities = probs;
    }
    
}

void FilmStatistics::oversampleInputData()
{

    std::map<ShotType, double> aggregated_probs;

    aggregateSlidingWindowProbs(oversampling_sliding_window, aggregated_probs);
    normalizeProbs(aggregated_probs);
    findShotTypeMaxProb(aggregated_probs, current_sample_oversampled);
   
}

void FilmStatistics::computeEntropy()
{

    std::map<ShotType, double> aggregated_probs;

    aggregateSlidingWindowProbs(entropy_sliding_window, aggregated_probs);
    normalizeProbs(aggregated_probs);

    entropy = 0.;

    for (const auto& [type, prob] : aggregated_probs) {
        if (prob > 0.0) {
            entropy -= prob * std::log2(prob);
        }
    }
}

void FilmStatistics::computeEntropyVariance()
{
    entropy_variance_sliding_window.push_back(entropy);
    
    if(entropy_variance_sliding_window.size() > config.entropy_variance_window_size)
        entropy_variance_sliding_window.pop_front();

    if(entropy_variance_sliding_window.size() <= 1)
    {
        entropy_variance = 0;
        return;
    }
        
    
    double mean_entropy = std::accumulate(entropy_variance_sliding_window.begin(), entropy_variance_sliding_window.end(), 0.0) / entropy_variance_sliding_window.size();

    for (const auto& e : entropy_variance_sliding_window) {
        entropy_variance += (e - mean_entropy) * (e - mean_entropy);
    }

        entropy_variance /= (entropy_variance_sliding_window.size() - 1);

}

// musím vyřešit aby byla okna synchronní,
//void FilmStatistics::detectCuts()
//{
//    if (evaluated_frames > 1) {
//        if (entropy > config.cut_detect_entropy_threshold ||
//            std::abs(entropy - last_entropy) > config.entropy_jump_threshold) {
//            is_scene_boundary = true;
//        }
//    }
//}






// main function with all calls
void FilmStatistics::addFrameResult(const double timestampMs, const classification_result& result)
{
    total_frames++;
    
    // startup delay
    if(start_delay == 0)
    {
        size_t max_s = (std::max(config.entropy_variance_window_size, config.entropy_window_size));
        start_delay = static_cast<int>(std::max(max_s, config.input_oversample));
        start_delay *= 2;
    }
    
    if (total_frames%config.input_step == 0)
    {
        evaluated_frames ++;
        
        // compute time needed for one frame -> needed for sync
        if(evaluated_frames == start_delay)
        {
            frame_time_measurement = timestampMs/(start_delay-1);
            start_delay = -1;
        }
            
        timestamp_ms = timestampMs;
        
        oversampling_sliding_window.push_back(result);
        entropy_sliding_window.push_back(result);
        
        
        if(oversampling_sliding_window.size() > config.input_oversample)
            oversampling_sliding_window.pop_front();
        
        if(entropy_sliding_window.size() > config.entropy_window_size)
            entropy_sliding_window.pop_front();
        
        
        oversampleInputData();
        computeEntropy();
        computeEntropyVariance();
        outputStatistics();
        
    }
    
    if(config.output_ratios_series)
       shot_counts[result.predictedType]++;

}



void FilmStatistics::addConfigurationStruct(FilmStatisticsEvalConfig const & cfg)
{
    config = cfg;
}


// --- functions for outputing ---

// calling of all statistic functions
void FilmStatistics::outputStatistics()
{
    if(evaluated_frames > start_delay)
    {
        if(config.output_prob_time_series > 0)
        {
            appendSampleToTimeline(prob_timeline);
            appendSampleToTimeline(shot_type_timeline);
        }
        
        if(config.output_entropy_time_series)
            appendEntropyToTimeline(enthropy_timeline);
        
        if(config.output_entropy_variance_time_series)
            appendEntropyVarianceToTimeline(enthropy_variance_timeline);
    }
    
}

void FilmStatistics::appendSampleToTimeline(std::vector<std::pair<double, std::map<ShotType, double>>> & timeline)
{
    if(config.output_prob_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_prob_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.input_oversample)/2.);
        
        std::pair<double, std::map<ShotType, double>> sample;
        sample.first = timestamp;
        sample.second = current_sample_oversampled.probabilities;
        
        timeline.push_back(sample);
    }
}

void FilmStatistics::appendSampleToTimeline(std::vector<std::pair<double, ShotType>> & timeline)
{
    if(config.output_prob_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_prob_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.input_oversample)/2.);
        
        std::pair<double, ShotType> sample;
        sample.first = timestamp;
        sample.second = current_sample_oversampled.predictedType;
        
        timeline.push_back(sample);
    }
}

void FilmStatistics::appendEntropyToTimeline(std::vector<std::pair<double, double>> & timeline)
{
    if(config.output_entropy_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_entropy_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.entropy_window_size)/2.);
        
        std::pair<double, double> sample;
        sample.first = timestamp;
        sample.second = entropy;
        
        timeline.push_back(sample);
    }
}

void FilmStatistics::appendEntropyVarianceToTimeline(std::vector<std::pair<double, double>> & timeline)
{
    if(config.output_entropy_variance_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_entropy_variance_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.entropy_variance_window_size)/2.);
        
        std::pair<double, double> sample;
        sample.first = timestamp;
        sample.second = entropy_variance;
        
        timeline.push_back(sample);
    }
}

