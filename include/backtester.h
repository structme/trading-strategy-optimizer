#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include "models.h"
#include "indicators.h"

// Base backtester class
class StrategyBacktester {
protected:
    const std::vector<Bar>& bars;
    const std::vector<double>& closes;
    const std::vector<double>& highs;
    const std::vector<double>& lows;
    const std::vector<double>& opens;
    double initial_capital;
    bool exclude_sl_from_winrate;
    std::shared_ptr<IndicatorCache> cache;
    
    // Common method to process trading logic
    std::vector<Trade> processTrades(const std::vector<int>& dir, bool use_sl, bool use_tp, 
                                   double sl_percent, double tp_percent, bool pyramiding);
    
    // Method to calculate backtest results
    BacktestResult calculateResults(const std::vector<Trade>& trades, const std::string& params_str, const std::string& strategy_name);
    
public:
    StrategyBacktester(const std::vector<Bar>& price_data, 
                     const std::vector<double>& close_prices,
                     const std::vector<double>& high_prices,
                     const std::vector<double>& low_prices,
                     const std::vector<double>& open_prices,
                     std::shared_ptr<IndicatorCache> indicator_cache,
                     double capital = 10000.0,
                     bool exclude_sl = false);
                  
    virtual ~StrategyBacktester() = default;
    
    // Static method to load data from CSV
    static std::vector<Bar> loadCSV(const std::string& filename);
    
    // Static method to preprocess price data vectors
    static void preprocessPriceData(const std::vector<Bar>& bars, 
                                  std::vector<double>& closes,
                                  std::vector<double>& highs,
                                  std::vector<double>& lows,
                                  std::vector<double>& opens);
};

// Strategy-specific backtester classes
class OttBacktester : public StrategyBacktester {
private:
    const OttParams& params;
    
public:
    OttBacktester(const std::vector<Bar>& price_data, 
                const std::vector<double>& close_prices,
                const std::vector<double>& high_prices,
                const std::vector<double>& low_prices,
                const std::vector<double>& open_prices,
                const OttParams& strategy_params, 
                std::shared_ptr<IndicatorCache> indicator_cache,
                double capital = 10000.0,
                bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class TottBacktester : public StrategyBacktester {
private:
    const TottParams& params;
    
public:
    TottBacktester(const std::vector<Bar>& price_data, 
                 const std::vector<double>& close_prices,
                 const std::vector<double>& high_prices,
                 const std::vector<double>& low_prices,
                 const std::vector<double>& open_prices,
                 const TottParams& strategy_params, 
                 std::shared_ptr<IndicatorCache> indicator_cache,
                 double capital = 10000.0,
                 bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class OttChannelBacktester : public StrategyBacktester {
private:
    const OttChannelParams& params;
    
public:
    OttChannelBacktester(const std::vector<Bar>& price_data, 
                       const std::vector<double>& close_prices,
                       const std::vector<double>& high_prices,
                       const std::vector<double>& low_prices,
                       const std::vector<double>& open_prices,
                       const OttChannelParams& strategy_params, 
                       std::shared_ptr<IndicatorCache> indicator_cache,
                       double capital = 10000.0,
                       bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class RisottoBacktester : public StrategyBacktester {
private:
    const RisottoParams& params;
    
public:
    RisottoBacktester(const std::vector<Bar>& price_data, 
                    const std::vector<double>& close_prices,
                    const std::vector<double>& high_prices,
                    const std::vector<double>& low_prices,
                    const std::vector<double>& open_prices,
                    const RisottoParams& strategy_params, 
                    std::shared_ptr<IndicatorCache> indicator_cache,
                    double capital = 10000.0,
                    bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class SottBacktester : public StrategyBacktester {
private:
    const SottParams& params;
    
public:
    SottBacktester(const std::vector<Bar>& price_data, 
                 const std::vector<double>& close_prices,
                 const std::vector<double>& high_prices,
                 const std::vector<double>& low_prices,
                 const std::vector<double>& open_prices,
                 const SottParams& strategy_params, 
                 std::shared_ptr<IndicatorCache> indicator_cache,
                 double capital = 10000.0,
                 bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class HottLottBacktester : public StrategyBacktester {
private:
    const HottLottParams& params;
    
public:
    HottLottBacktester(const std::vector<Bar>& price_data, 
                     const std::vector<double>& close_prices,
                     const std::vector<double>& high_prices,
                     const std::vector<double>& low_prices,
                     const std::vector<double>& open_prices,
                     const HottLottParams& strategy_params, 
                     std::shared_ptr<IndicatorCache> indicator_cache,
                     double capital = 10000.0,
                     bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class RottBacktester : public StrategyBacktester {
private:
    const RottParams& params;
    
public:
    RottBacktester(const std::vector<Bar>& price_data, 
                 const std::vector<double>& close_prices,
                 const std::vector<double>& high_prices,
                 const std::vector<double>& low_prices,
                 const std::vector<double>& open_prices,
                 const RottParams& strategy_params, 
                 std::shared_ptr<IndicatorCache> indicator_cache,
                 double capital = 10000.0,
                 bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class FtBacktester : public StrategyBacktester {
private:
    const FtParams& params;
    
public:
    FtBacktester(const std::vector<Bar>& price_data, 
               const std::vector<double>& close_prices,
               const std::vector<double>& high_prices,
               const std::vector<double>& low_prices,
               const std::vector<double>& open_prices,
               const FtParams& strategy_params, 
               std::shared_ptr<IndicatorCache> indicator_cache,
               double capital = 10000.0,
               bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class RtrBacktester : public StrategyBacktester {
private:
    const RtrParams& params;
    
public:
    RtrBacktester(const std::vector<Bar>& price_data, 
                const std::vector<double>& close_prices,
                const std::vector<double>& high_prices,
                const std::vector<double>& low_prices,
                const std::vector<double>& open_prices,
                const RtrParams& strategy_params, 
                std::shared_ptr<IndicatorCache> indicator_cache,
                double capital = 10000.0,
                bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class MottBacktester : public StrategyBacktester {
private:
    const MottParams& params;
    
public:
    MottBacktester(const std::vector<Bar>& price_data, 
                 const std::vector<double>& close_prices,
                 const std::vector<double>& high_prices,
                 const std::vector<double>& low_prices,
                 const std::vector<double>& open_prices,
                 const MottParams& strategy_params, 
                 std::shared_ptr<IndicatorCache> indicator_cache,
                 double capital = 10000.0,
                 bool exclude_sl = false);
    
    BacktestResult runBacktest();
};

class BootsBacktester : public StrategyBacktester {
private:
    const BootsParams& params;
    
public:
    BootsBacktester(const std::vector<Bar>& price_data, 
                  const std::vector<double>& close_prices,
                  const std::vector<double>& high_prices,
                  const std::vector<double>& low_prices,
                  const std::vector<double>& open_prices,
                  const BootsParams& strategy_params, 
                  std::shared_ptr<IndicatorCache> indicator_cache,
                  double capital = 10000.0,
                  bool exclude_sl = false);
    
    BacktestResult runBacktest();
};