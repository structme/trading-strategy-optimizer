#include "models.h"
#include <sstream>

// OttParams implementation
std::size_t OttParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(support_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool OttParams::operator==(const OttParams& other) const {
    return StrategyParams::operator==(other) &&
           support_length == other.support_length &&
           ott_multiplier == other.ott_multiplier;
}

std::string OttParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-SupportLength=" << support_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// TottParams implementation
std::size_t TottParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(support_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(band_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool TottParams::operator==(const TottParams& other) const {
    return StrategyParams::operator==(other) &&
           support_length == other.support_length &&
           ott_multiplier == other.ott_multiplier &&
           band_multiplier == other.band_multiplier;
}

std::string TottParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-SupportLength=" << support_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-BandMultiplier=" << band_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// OttChannelParams implementation
std::size_t OttChannelParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(ma_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(upper_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(lower_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<std::string>{}(channel_type) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool OttChannelParams::operator==(const OttChannelParams& other) const {
    return StrategyParams::operator==(other) &&
           ma_length == other.ma_length &&
           ott_multiplier == other.ott_multiplier &&
           upper_multiplier == other.upper_multiplier &&
           lower_multiplier == other.lower_multiplier &&
           channel_type == other.channel_type;
}

std::string OttChannelParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-ChannelType=" << channel_type
              << "-MALength=" << ma_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-UpperMultiplier=" << upper_multiplier
              << "-LowerMultiplier=" << lower_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// RisottoParams implementation
std::size_t RisottoParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(rsi_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>{}(support_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool RisottoParams::operator==(const RisottoParams& other) const {
    return StrategyParams::operator==(other) &&
           rsi_length == other.rsi_length &&
           support_length == other.support_length &&
           ott_multiplier == other.ott_multiplier;
}

std::string RisottoParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-RSILength=" << rsi_length
              << "-SupportLength=" << support_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// SottParams implementation
std::size_t SottParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(stoch_k_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>{}(stoch_d_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool SottParams::operator==(const SottParams& other) const {
    return StrategyParams::operator==(other) &&
           stoch_k_length == other.stoch_k_length &&
           stoch_d_length == other.stoch_d_length &&
           ott_multiplier == other.ott_multiplier;
}

std::string SottParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-StochKLength=" << stoch_k_length
              << "-StochDLength=" << stoch_d_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// HottLottParams implementation
std::size_t HottLottParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(hl_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<bool>{}(use_sum) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>{}(sum_n_bars) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool HottLottParams::operator==(const HottLottParams& other) const {
    return StrategyParams::operator==(other) &&
           hl_length == other.hl_length &&
           ott_multiplier == other.ott_multiplier &&
           use_sum == other.use_sum &&
           sum_n_bars == other.sum_n_bars;
}

std::string HottLottParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-HLLength=" << hl_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-UseSumNBars=" << (use_sum ? "on" : "off");
    if (use_sum) {
        params_ss << "-SumNBars=" << sum_n_bars;
    }
    params_ss << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// RottParams implementation
std::size_t RottParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(support_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool RottParams::operator==(const RottParams& other) const {
    return StrategyParams::operator==(other) &&
           support_length == other.support_length &&
           ott_multiplier == other.ott_multiplier;
}

std::string RottParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-SupportLength=" << support_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// FtParams implementation
std::size_t FtParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(support_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(major_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(minor_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool FtParams::operator==(const FtParams& other) const {
    return StrategyParams::operator==(other) &&
           support_length == other.support_length &&
           major_multiplier == other.major_multiplier &&
           minor_multiplier == other.minor_multiplier;
}

std::string FtParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-SupportLength=" << support_length
              << "-MajorOTTMultiplier=" << major_multiplier
              << "-MinorOTTMultiplier=" << minor_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// RtrParams implementation
std::size_t RtrParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(atr_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>{}(ma_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool RtrParams::operator==(const RtrParams& other) const {
    return StrategyParams::operator==(other) &&
           atr_length == other.atr_length &&
           ma_length == other.ma_length;
}

std::string RtrParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-ATRLength=" << atr_length
              << "-MALength=" << ma_length
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// MottParams implementation
std::size_t MottParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(support_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>{}(hl_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>{}(reference) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool MottParams::operator==(const MottParams& other) const {
    return StrategyParams::operator==(other) &&
           support_length == other.support_length &&
           hl_length == other.hl_length &&
           ott_multiplier == other.ott_multiplier &&
           reference == other.reference;
}

std::string MottParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-SupportLength=" << support_length
              << "-HLLength=" << hl_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-Reference=" << reference
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}

// BootsParams implementation
std::size_t BootsParams::hash() const {
    std::size_t h = StrategyParams::hash();
    h ^= std::hash<int>{}(support_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>{}(bb_length) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<double>{}(ott_multiplier) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool BootsParams::operator==(const BootsParams& other) const {
    return StrategyParams::operator==(other) &&
           support_length == other.support_length &&
           bb_length == other.bb_length &&
           ott_multiplier == other.ott_multiplier;
}

std::string BootsParams::getParamString() const {
    std::ostringstream params_ss;
    params_ss << "Strategy=" << strategy_name
              << "-SupportLength=" << support_length
              << "-BBLength=" << bb_length
              << "-OTTMultiplier=" << ott_multiplier
              << "-SL=" << (use_sl ? std::to_string(sl_percent) : "off")
              << "-TP=" << (use_tp ? std::to_string(tp_percent) : "off")
              << "-Pyramiding=" << (pyramiding ? "on" : "off");
    return params_ss.str();
}