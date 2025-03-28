#include "indicators.h"
#include <cmath>
#include <algorithm>
#include <limits>

const std::vector<double>& IndicatorCache::getStochastic(const std::vector<double>& closes, 
                                                      const std::vector<double>& highs, 
                                                      const std::vector<double>& lows, 
                                                      int k_length) {
    std::string cache_key = "stoch_" + std::to_string(k_length);
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = indicator_cache.find(cache_key);
        if (it != indicator_cache.end()) {
            return it->second;
        }
    }
    
    // Calculate Stochastic %K
    std::vector<double> result(closes.size(), 0.0);
    
    for (int i = k_length; i < closes.size(); ++i) {
        // Find lowest low and highest high over k_length
        double lowest_low = std::numeric_limits<double>::max();
        double highest_high = std::numeric_limits<double>::lowest();
        
        for (int j = i - k_length + 1; j <= i; ++j) {
            if (j >= 0) {
                lowest_low = std::min(lowest_low, lows[j]);
                highest_high = std::max(highest_high, highs[j]);
            }
        }
        
        // Calculate %K
        if (highest_high - lowest_low > 0) {
            result[i] = (closes[i] - lowest_low) / (highest_high - lowest_low) * 100.0;
        } else {
            result[i] = 100.0; // Default value when there's no range
        }
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        indicator_cache[cache_key] = result;
        return indicator_cache[cache_key];
    }
}

const std::vector<double>& IndicatorCache::getRSI(const std::vector<double>& closes, int length) {
    std::string cache_key = "rsi_" + std::to_string(length);
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = indicator_cache.find(cache_key);
        if (it != indicator_cache.end()) {
            return it->second;
        }
    }
    
    // Calculate RSI
    std::vector<double> result(closes.size(), 0.0);
    std::vector<double> changes(closes.size(), 0.0);
    std::vector<double> gains(closes.size(), 0.0);
    std::vector<double> losses(closes.size(), 0.0);
    
    // Calculate price changes
    for (size_t i = 1; i < closes.size(); i++) {
        changes[i] = closes[i] - closes[i-1];
        if (changes[i] > 0) {
            gains[i] = changes[i];
            losses[i] = 0;
        } else {
            gains[i] = 0;
            losses[i] = -changes[i];
        }
    }
    
    // Calculate initial averages
    double avg_gain = 0;
    double avg_loss = 0;
    
    for (int i = 1; i <= length; i++) {
        avg_gain += gains[i];
        avg_loss += losses[i];
    }
    
    avg_gain /= length;
    avg_loss /= length;
    
    // Calculate RSI
    for (size_t i = length + 1; i < closes.size(); i++) {
        // Update average gain and loss
        avg_gain = (avg_gain * (length - 1) + gains[i]) / length;
        avg_loss = (avg_loss * (length - 1) + losses[i]) / length;
        
        if (avg_loss == 0) {
            result[i] = 100;
        } else {
            double rs = avg_gain / avg_loss;
            result[i] = 100 - (100 / (1 + rs));
        }
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        indicator_cache[cache_key] = result;
        return indicator_cache[cache_key];
    }
}

const std::vector<double>& IndicatorCache::getVAR(const std::vector<double>& data, int length) {
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = var_cache.find(length);
        if (it != var_cache.end()) {
            return it->second;
        }
    }
    
    // Calculate VAR (VIDYA)
    std::vector<double> result(data.size(), 0.0);
    
    // Get or calculate momentum (period 9)
    std::vector<double> momentum = getAbsChange(data, 9);
    
    // Get or calculate volatility (period 9)
    std::vector<double> volatility = getSumAbsChanges(data, 9);
    
    // Calculate efficiency ratio
    std::vector<double> efficiencyRatio(data.size(), 0.0);
    for (int i = 0; i < data.size(); ++i) {
        if (volatility[i] != 0) {
            efficiencyRatio[i] = momentum[i] / volatility[i];
        } else {
            efficiencyRatio[i] = 0.0;
        }
    }
    
    // Calculate alpha
    double alpha = 2.0 / (length + 1.0);
    
    // Calculate VIDYA
    for (int i = 0; i < data.size(); ++i) {
        if (i == 0) {
            result[i] = data[i];
        } else if (length == 1) {
            result[i] = data[i];
        } else {
            result[i] = efficiencyRatio[i] * alpha * (data[i] - result[i-1]) + result[i-1];
        }
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        var_cache[length] = result;
        return var_cache[length];
    }
}

const std::vector<double>& IndicatorCache::getOTT(const std::vector<double>& data, double multiplier) {
    auto key = std::make_pair(data.size(), multiplier);
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = ott_cache.find(key);
        if (it != ott_cache.end()) {
            return it->second;
        }
    }
    
    std::vector<double> result(data.size(), 0.0);
    
    double a = multiplier / 100.0;
    std::vector<double> c(data.size(), 0.0);
    std::vector<double> d(data.size(), 0.0);
    std::vector<double> e(data.size(), 0.0);
    std::vector<double> h(data.size(), 0.0);
    
    for (int i = 0; i < data.size(); ++i) {
        double b = data[i] * a;
        
        if (i == 0) {
            c[i] = data[i] - b;
            d[i] = data[i] + b;
            e[i] = 0.0;
        } else {
            c[i] = (data[i] - b) > c[i-1] || data[i] < c[i-1] ? (data[i] - b) : c[i-1];
            d[i] = (data[i] + b) < d[i-1] || data[i] > d[i-1] ? (data[i] + b) : d[i-1];
            e[i] = data[i] > e[i-1] ? c[i] : data[i] < e[i-1] ? d[i] : e[i-1];
        }
        
        double f = 1.0 + a / 2.0;
        double g = 1.0 - a / 2.0;
        
        h[i] = data[i] > e[i] ? e[i] * f : e[i] * g;
        
        if (i >= 2) {
            result[i] = h[i-2];
        } else {
            result[i] = 0.0;
        }
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        ott_cache[key] = result;
        return ott_cache[key];
    }
}

const std::vector<double>& IndicatorCache::getAbsChange(const std::vector<double>& data, int period) {
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = abs_change_cache.find(period);
        if (it != abs_change_cache.end()) {
            return it->second;
        }
    }
    
    std::vector<double> result(data.size(), 0.0);
    
    for (int i = period; i < data.size(); ++i) {
        result[i] = std::abs(data[i] - data[i - period]);
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        abs_change_cache[period] = result;
        return abs_change_cache[period];
    }
}

const std::vector<double>& IndicatorCache::getSumAbsChanges(const std::vector<double>& data, int period) {
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = sum_abs_changes_cache.find(period);
        if (it != sum_abs_changes_cache.end()) {
            return it->second;
        }
    }
    
    std::vector<double> result(data.size(), 0.0);
    std::vector<double> changes(data.size(), 0.0);
    
    for (size_t i = 1; i < data.size(); ++i) {
        changes[i] = std::abs(data[i] - data[i - 1]);
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += changes[i];
        if (i >= period) {
            sum -= changes[i - period];
        }
        result[i] = sum;
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        sum_abs_changes_cache[period] = result;
        return sum_abs_changes_cache[period];
    }
}

const std::vector<double>& IndicatorCache::getHighest(const std::vector<double>& data, int period) {
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = highest_cache.find(period);
        if (it != highest_cache.end()) {
            return it->second;
        }
    }
    
    std::vector<double> result(data.size(), 0.0);
    
    for (size_t i = 0; i < data.size(); ++i) {
        double highest = std::numeric_limits<double>::lowest();
        
        for (int j = std::max(0, static_cast<int>(i) - period + 1); j <= i; ++j) {
            highest = std::max(highest, data[j]);
        }
        
        result[i] = highest;
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        highest_cache[period] = result;
        return highest_cache[period];
    }
}

const std::vector<double>& IndicatorCache::getLowest(const std::vector<double>& data, int period) {
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = lowest_cache.find(period);
        if (it != lowest_cache.end()) {
            return it->second;
        }
    }
    
    std::vector<double> result(data.size(), 0.0);
    
    for (size_t i = 0; i < data.size(); ++i) {
        double lowest = std::numeric_limits<double>::max();
        
        for (int j = std::max(0, static_cast<int>(i) - period + 1); j <= i; ++j) {
            lowest = std::min(lowest, data[j]);
        }
        
        result[i] = lowest;
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        lowest_cache[period] = result;
        return lowest_cache[period];
    }
}

const std::vector<double>& IndicatorCache::getATR(const std::vector<double>& highs, 
                                                const std::vector<double>& lows, 
                                                const std::vector<double>& closes, 
                                                int period) {
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = atr_cache.find(period);
        if (it != atr_cache.end()) {
            return it->second;
        }
    }
    
    std::vector<double> result(highs.size(), 0.0);
    std::vector<double> tr(highs.size(), 0.0);
    
    for (size_t i = 1; i < highs.size(); ++i) {
        double tr1 = highs[i] - lows[i];
        double tr2 = std::abs(highs[i] - closes[i-1]);
        double tr3 = std::abs(lows[i] - closes[i-1]);
        tr[i] = std::max({tr1, tr2, tr3});
    }
    
    // Calculate first ATR as simple average of TR over period
    if (highs.size() > period) {
        double sum = 0;
        for (int i = 1; i <= period; ++i) {
            sum += tr[i];
        }
        result[period] = sum / period;
        
        // Calculate subsequent ATRs using smoothing formula
        for (size_t i = period + 1; i < highs.size(); ++i) {
            result[i] = (result[i-1] * (period - 1) + tr[i]) / period;
        }
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        atr_cache[period] = result;
        return atr_cache[period];
    }
}

const std::vector<double>& IndicatorCache::getBBUpper(const std::vector<double>& data, int length, double multiplier) {
    std::string cache_key = "bb_upper_" + std::to_string(length) + "_" + std::to_string(multiplier);
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = indicator_cache.find(cache_key);
        if (it != indicator_cache.end()) {
            return it->second;
        }
    }
    
    // Calculate BB Upper using VAR as the basis
    const auto& basis = getVAR(data, length);
    std::vector<double> result(data.size(), 0.0);
    
    // Calculate standard deviation
    for (size_t i = length; i < data.size(); ++i) {
        double sum_sq = 0.0;
        for (int j = i - length + 1; j <= i; ++j) {
            sum_sq += (data[j] - basis[i]) * (data[j] - basis[i]);
        }
        double stdev = std::sqrt(sum_sq / length);
        result[i] = basis[i] + (multiplier * stdev);
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        indicator_cache[cache_key] = result;
        return indicator_cache[cache_key];
    }
}

const std::vector<double>& IndicatorCache::getBBLower(const std::vector<double>& data, int length, double multiplier) {
    std::string cache_key = "bb_lower_" + std::to_string(length) + "_" + std::to_string(multiplier);
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = indicator_cache.find(cache_key);
        if (it != indicator_cache.end()) {
            return it->second;
        }
    }
    
    // Calculate BB Lower using VAR as the basis
    const auto& basis = getVAR(data, length);
    std::vector<double> result(data.size(), 0.0);
    
    // Calculate standard deviation
    for (size_t i = length; i < data.size(); ++i) {
        double sum_sq = 0.0;
        for (int j = i - length + 1; j <= i; ++j) {
            sum_sq += (data[j] - basis[i]) * (data[j] - basis[i]);
        }
        double stdev = std::sqrt(sum_sq / length);
        result[i] = basis[i] - (multiplier * stdev);
    }
    
    // Cache and return
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        indicator_cache[cache_key] = result;
        return indicator_cache[cache_key];
    }
}

void IndicatorCache::clear() {
    std::lock_guard<std::mutex> lock(cache_mutex);
    var_cache.clear();
    ott_cache.clear();
    indicator_cache.clear();
    abs_change_cache.clear();
    sum_abs_changes_cache.clear();
    highest_cache.clear();
    lowest_cache.clear();
    atr_cache.clear();
}