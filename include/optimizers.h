#pragma once

#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include "models.h"
#include "indicators.h"
#include "backtester.h"

// Base optimizer class
class StrategyOptimizer {
protected:
    const std::vector<Bar>& bars;
    std::vector<double> closes; // Pre-extracted price data
    std::vector<double> highs;
    std::vector<double> lows;
    std::vector<double> opens;
    
    std::vector<double> sl_percents;
    std::vector<double> tp_percents;
    bool use_sl;
    bool use_tp;
    bool pyramiding;
    double initial_capital;
    int min_trades;
    double min_win_rate;
    bool exclude_sl_from_winrate;
    std::shared_ptr<IndicatorCache> cache;
    
    std::atomic<int> progress;
    int total_combinations;
    
    std::mutex result_mutex;
    std::mutex progress_mutex;
    
    // Result deduplication
    std::unordered_map<std::string, bool> result_deduplication;
    std::mutex dedup_mutex;
    
public:
    StrategyOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    virtual ~StrategyOptimizer() = default;
    
    // Method to save results to CSV by strategy
    static void saveResultsToCSV(const std::vector<BacktestResult>& results, 
                               const std::string& strategy_name, 
                               const std::string& base_dir = "results");
    
    // Method to save trades for top results
    static void saveTradesForTopResults(const std::vector<BacktestResult>& results, 
                                      const std::vector<Bar>& bars, 
                                      const std::string& strategy_name,
                                      const std::string& sort_by = "win_rate", 
                                      int num_top = 10,
                                      const std::string& base_dir = "results");
    
    virtual std::vector<BacktestResult> optimize(int num_threads = 4) = 0;
};

// Strategy-specific optimizer classes
class OttOptimizer : public StrategyOptimizer {
private:
    std::vector<int> support_lengths;
    std::vector<double> ott_multipliers;
    
public:
    OttOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& support_lens = {10, 20, 30, 40, 50},
        const std::vector<double>& ott_mults = {0.5, 0.7, 0.9, 1.1, 1.3, 1.5},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class TottOptimizer : public StrategyOptimizer {
private:
    std::vector<int> support_lengths;
    std::vector<double> ott_multipliers;
    std::vector<double> band_multipliers;
    
public:
    TottOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& support_lens = {20, 30, 40, 50},
        const std::vector<double>& ott_mults = {0.3, 0.4, 0.5, 0.6},
        const std::vector<double>& band_mults = {0.0004, 0.0005, 0.0006},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class SottOptimizer : public StrategyOptimizer {
private:
    std::vector<int> stoch_k_lengths;
    std::vector<int> stoch_d_lengths;
    std::vector<double> ott_multipliers;
    
public:
    SottOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& stoch_k_lens = {200, 300, 400, 500},
        const std::vector<int>& stoch_d_lens = {100, 150, 200},
        const std::vector<double>& ott_mults = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class OttChannelOptimizer : public StrategyOptimizer {
private:
    std::vector<int> ma_lengths;
    std::vector<double> ott_multipliers;
    std::vector<double> upper_multipliers;
    std::vector<double> lower_multipliers;
    std::vector<std::string> channel_types;
    
public:
    OttChannelOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& ma_lens = {10, 20, 30, 40, 50},
        const std::vector<double>& ott_mults = {0.3, 0.5, 0.7, 0.9},
        const std::vector<double>& upper_mults = {0.1, 0.2, 0.3, 0.4, 0.5},
        const std::vector<double>& lower_mults = {0.1, 0.2, 0.3, 0.4, 0.5},
        const std::vector<std::string>& channel_type_options = {"Half Channel", "Full Channel"},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class RisottoOptimizer : public StrategyOptimizer {
private:
    std::vector<int> rsi_lengths;
    std::vector<int> support_lengths;
    std::vector<double> ott_multipliers;
    
public:
    RisottoOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& rsi_lens = {8, 12, 16, 20, 24},
        const std::vector<int>& support_lens = {10, 20, 30, 40, 50},
        const std::vector<double>& ott_mults = {0.5, 0.7, 0.9, 1.1, 1.3, 1.5},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class HottLottOptimizer : public StrategyOptimizer {
private:
    std::vector<int> hl_lengths;
    std::vector<double> ott_multipliers;
    std::vector<bool> use_sum_values;
    std::vector<int> sum_n_bars_values;
    
public:
    HottLottOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& hl_lens = {5, 10, 15, 20, 25, 30},
        const std::vector<double>& ott_mults = {0.5, 0.7, 0.9, 1.1, 1.3, 1.5},
        const std::vector<bool>& use_sum_opts = {false, true},
        const std::vector<int>& sum_n_bars_opts = {2, 3, 4, 5},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class RottOptimizer : public StrategyOptimizer {
private:
    std::vector<int> support_lengths;
    std::vector<double> ott_multipliers;
    
public:
    RottOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& support_lens = {10, 15, 20, 25, 30, 35, 40, 45, 50},
        const std::vector<double>& ott_mults = {0.5, 0.7, 0.9, 1.1, 1.3, 1.5},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class FtOptimizer : public StrategyOptimizer {
private:
    std::vector<int> support_lengths;
    std::vector<double> major_multipliers;
    std::vector<double> minor_multipliers;
    
public:
    FtOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& support_lens = {10, 20, 30, 40, 50},
        const std::vector<double>& major_mults = {0.5, 0.7, 0.9, 1.1, 1.3, 1.5},
        const std::vector<double>& minor_mults = {0.1, 0.3, 0.5, 0.7, 0.9},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class RtrOptimizer : public StrategyOptimizer {
private:
    std::vector<int> atr_lengths;
    std::vector<int> ma_lengths;
    
public:
    RtrOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& atr_lens = {5, 10, 15, 20, 25, 30},
        const std::vector<int>& ma_lens = {10, 15, 20, 25, 30, 35, 40, 45, 50},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class MottOptimizer : public StrategyOptimizer {
private:
    std::vector<int> support_lengths;
    std::vector<int> hl_lengths;
    std::vector<double> ott_multipliers;
    std::vector<int> reference_values;
    
public:
    MottOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& support_lens = {10, 20, 30, 40, 50},
        const std::vector<int>& hl_lens = {5, 10, 15, 20, 25, 30},
        const std::vector<double>& ott_mults = {0.5, 0.7, 0.9, 1.1, 1.3, 1.5},
        const std::vector<int>& ref_values = {0, 5, 10, 15},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

class BootsOptimizer : public StrategyOptimizer {
private:
    std::vector<int> support_lengths;
    std::vector<int> bb_lengths;
    std::vector<double> ott_multipliers;
    
public:
    BootsOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<int>& support_lens = {10, 20, 30, 40, 50},
        const std::vector<int>& bb_lens = {10, 20, 30, 40, 50},
        const std::vector<double>& ott_mults = {0.5, 0.7, 0.9, 1.1, 1.3, 1.5},
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false
    );
    
    std::vector<BacktestResult> optimize(int num_threads = 4) override;
};

// Multi-strategy optimizer class
class MultiStrategyOptimizer {
private:
    std::vector<Bar> bars;
    std::vector<std::string> selected_strategies;
    std::vector<double> sl_percents;
    std::vector<double> tp_percents;
    bool use_sl;
    bool use_tp;
    bool pyramiding;
    double initial_capital;
    int min_trades;
    double min_win_rate;
    bool exclude_sl_from_winrate;
    int num_threads;
    
public:
    MultiStrategyOptimizer(
        const std::vector<Bar>& price_data,
        const std::vector<std::string>& strategies,
        const std::vector<double>& sl_pcts = {1.0, 2.0, 3.0},
        const std::vector<double>& tp_pcts = {2.0, 3.0, 5.0},
        bool enable_sl = true,
        bool enable_tp = true,
        bool enable_pyramiding = false,
        double capital = 10000.0,
        int minimum_trades = 5,
        double minimum_win_rate = 50.0,
        bool exclude_sl = false,
        int threads = 4
    );
    
    void optimizeAll();
};