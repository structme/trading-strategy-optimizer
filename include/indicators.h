#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include "models.h"

// Indicator cache class
class IndicatorCache {
private:
    // Cache for VAR (VIDYA) calculations with different lengths
    std::unordered_map<int, std::vector<double>> var_cache;
    
    // Cache for OTT calculations with different multiplier values
    std::unordered_map<std::pair<int, double>, std::vector<double>, PairHash> ott_cache;
    
    // General purpose indicator cache for stochastic, RSI, etc.
    std::unordered_map<std::string, std::vector<double>> indicator_cache;
    
    // Common calculations used by multiple indicators
    std::unordered_map<int, std::vector<double>> abs_change_cache;
    std::unordered_map<int, std::vector<double>> sum_abs_changes_cache;
    std::unordered_map<int, std::vector<double>> highest_cache;
    std::unordered_map<int, std::vector<double>> lowest_cache;
    std::unordered_map<int, std::vector<double>> atr_cache;
    
    // Thread safety
    std::mutex cache_mutex;

public:
    // Get or calculate Stochastic indicator
    const std::vector<double>& getStochastic(const std::vector<double>& closes, 
                                           const std::vector<double>& highs, 
                                           const std::vector<double>& lows, 
                                           int k_length);
    
    // Get or calculate RSI indicator
    const std::vector<double>& getRSI(const std::vector<double>& closes, int length);
    
    // Get or calculate VAR indicator (VIDYA)
    const std::vector<double>& getVAR(const std::vector<double>& data, int length);
    
    // Get or calculate OTT indicator
    const std::vector<double>& getOTT(const std::vector<double>& data, double multiplier);
    
    // Get or calculate absolute change
    const std::vector<double>& getAbsChange(const std::vector<double>& data, int period);
    
    // Get or calculate sum of absolute changes
    const std::vector<double>& getSumAbsChanges(const std::vector<double>& data, int period);
    
    // Get or calculate highest over period
    const std::vector<double>& getHighest(const std::vector<double>& data, int period);
    
    // Get or calculate lowest over period
    const std::vector<double>& getLowest(const std::vector<double>& data, int period);
    
    // Get or calculate ATR (Average True Range)
    const std::vector<double>& getATR(const std::vector<double>& highs, 
                                    const std::vector<double>& lows, 
                                    const std::vector<double>& closes, 
                                    int period);
    
    // Get or calculate Bollinger Bands upper
    const std::vector<double>& getBBUpper(const std::vector<double>& data, int length, double multiplier);
    
    // Get or calculate Bollinger Bands lower
    const std::vector<double>& getBBLower(const std::vector<double>& data, int length, double multiplier);
    
    // Clear all caches to free memory
    void clear();
};