#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <unordered_map>

// Basic data structures
struct Bar {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

struct Trade {
    int entry_index;
    int exit_index;
    double entry_price;
    double exit_price;
    double profit;
    bool is_long;
    std::string exit_reason;
};

struct BacktestResult {
    double net_profit;
    double profit_factor;
    int total_trades;
    int winning_trades;
    int losing_trades;
    double win_rate;
    double max_drawdown;
    double profit_percent;
    std::vector<Trade> trades;
    std::string params_str;
    std::string strategy_name;
    int sl_trades;         // Number of trades that hit stop loss
    double sl_win_rate;    // Win rate excluding stop loss trades
};

// Custom hash for pair
struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

// Base Strategy Parameters class
struct StrategyParams {
    double sl_percent;     // Stop loss percentage
    double tp_percent;     // Take profit percentage
    bool use_sl;           // Use stop loss flag
    bool use_tp;           // Use take profit flag
    bool pyramiding;       // Allow consecutive signals
    std::string strategy_name; // Name of the strategy
    
    virtual ~StrategyParams() = default;
    
    // Base hash function for parameter caching
    virtual std::size_t hash() const {
        std::size_t h = std::hash<double>{}(sl_percent);
        h ^= std::hash<double>{}(tp_percent) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<bool>{}(use_sl) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<bool>{}(use_tp) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<bool>{}(pyramiding) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<std::string>{}(strategy_name) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
    
    virtual bool operator==(const StrategyParams& other) const {
        return sl_percent == other.sl_percent &&
               tp_percent == other.tp_percent &&
               use_sl == other.use_sl &&
               use_tp == other.use_tp &&
               pyramiding == other.pyramiding &&
               strategy_name == other.strategy_name;
    }
    
    // Virtual function to get parameter string
    virtual std::string getParamString() const {
        std::ostringstream params_ss;
        params_ss << "Strategy=" << strategy_name
                  << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
                  << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
                  << "-Pyramiding=" << (pyramiding ? "on" : "off");
        return params_ss.str();
    }
};

// Specific strategy parameter classes
struct OttParams : public StrategyParams {
    int support_length;    // ott_u
    double ott_multiplier; // ott_k
    
    OttParams() {
        strategy_name = "OTT";
    }
    
    std::size_t hash() const override;
    bool operator==(const OttParams& other) const;
    std::string getParamString() const override;
};

struct TottParams : public StrategyParams {
    int support_length;     // tott_u
    double ott_multiplier;  // tott_k1
    double band_multiplier; // tott_k2
    
    TottParams() {
        strategy_name = "TOTT";
    }
    
    std::size_t hash() const override;
    bool operator==(const TottParams& other) const;
    std::string getParamString() const override;
};

struct OttChannelParams : public StrategyParams {
    int ma_length;              // ottc_u
    double ott_multiplier;      // ottc_k1
    double upper_multiplier;    // ottc_k2
    double lower_multiplier;    // ottc_k3
    std::string channel_type;   // ottc_t
    
    OttChannelParams() {
        strategy_name = "OTT_CHANNEL";
        channel_type = "Half Channel"; // Default value
    }
    
    std::size_t hash() const override;
    bool operator==(const OttChannelParams& other) const;
    std::string getParamString() const override;
};

struct RisottoParams : public StrategyParams {
    int rsi_length;        // risotto_u1
    int support_length;    // risotto_u2
    double ott_multiplier; // risotto_k
    
    RisottoParams() {
        strategy_name = "RISOTTO";
    }
    
    std::size_t hash() const override;
    bool operator==(const RisottoParams& other) const;
    std::string getParamString() const override;
};

struct SottParams : public StrategyParams {
    int stoch_k_length;    // sott_u1
    int stoch_d_length;    // sott_u2
    double ott_multiplier; // sott_k
    
    SottParams() {
        strategy_name = "SOTT";
    }
    
    std::size_t hash() const override;
    bool operator==(const SottParams& other) const;
    std::string getParamString() const override;
};

struct HottLottParams : public StrategyParams {
    int hl_length;         // hl_u
    double ott_multiplier; // hl_k
    bool use_sum;          // hl_sum
    int sum_n_bars;        // hl_sum_n
    
    HottLottParams() {
        strategy_name = "HOTT-LOTT";
        use_sum = false;
        sum_n_bars = 3;
    }
    
    std::size_t hash() const override;
    bool operator==(const HottLottParams& other) const;
    std::string getParamString() const override;
};

struct RottParams : public StrategyParams {
    int support_length;    // rott_u
    double ott_multiplier; // rott_k
    
    RottParams() {
        strategy_name = "ROTT";
    }
    
    std::size_t hash() const override;
    bool operator==(const RottParams& other) const;
    std::string getParamString() const override;
};

struct FtParams : public StrategyParams {
    int support_length;        // ft_u
    double major_multiplier;   // ft_k1
    double minor_multiplier;   // ft_k2
    
    FtParams() {
        strategy_name = "FT";
    }
    
    std::size_t hash() const override;
    bool operator==(const FtParams& other) const;
    std::string getParamString() const override;
};

struct RtrParams : public StrategyParams {
    int atr_length;    // rtr_u1
    int ma_length;     // rtr_u2
    
    RtrParams() {
        strategy_name = "RTR";
    }
    
    std::size_t hash() const override;
    bool operator==(const RtrParams& other) const;
    std::string getParamString() const override;
};

struct MottParams : public StrategyParams {
    int support_length;    // mott_u1
    int hl_length;         // mott_u2
    double ott_multiplier; // mott_k
    int reference;         // mott_r
    
    MottParams() {
        strategy_name = "MOTT";
    }
    
    std::size_t hash() const override;
    bool operator==(const MottParams& other) const;
    std::string getParamString() const override;
};

struct BootsParams : public StrategyParams {
    int support_length;    // boots_u1
    int bb_length;         // boots_u2
    double ott_multiplier; // boots_k
    
    BootsParams() {
        strategy_name = "BOOTS";
    }
    
    std::size_t hash() const override;
    bool operator==(const BootsParams& other) const;
    std::string getParamString() const override;
};