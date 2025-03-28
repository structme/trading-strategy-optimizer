#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include "models.h"
#include "indicators.h"
#include "backtester.h"
#include "optimizers.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <csv_file> [options]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  --strategies=s1,s2,...  Strategies to optimize (default: OTT)" << std::endl;
        std::cout << "  --threads=N             Number of threads to use (default: CPU cores)" << std::endl;
        std::cout << "  --min-trades=N          Minimum trades filter (default: 5)" << std::endl;
        std::cout << "  --min-winrate=N         Minimum win rate filter (default: 55)" << std::endl;
        std::cout << "  --no-sl                 Disable stop loss" << std::endl;
        std::cout << "  --no-tp                 Disable take profit" << std::endl;
        std::cout << "  --pyramiding            Enable pyramiding" << std::endl;
        std::cout << "  --exclude-sl            Exclude stop loss trades from win rate calculation" << std::endl;
        std::cout << "Available strategies: OTT, TOTT, OTT_CHANNEL, RISOTTO, SOTT, HOTT-LOTT, ROTT, FT, RTR, MOTT, BOOTS" << std::endl;
        std::cout << "Example: " << argv[0] << " data.csv --strategies=OTT,SOTT,MOTT --threads=8" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    std::vector<std::string> strategies = {"OTT"}; // Default strategy
    int num_threads = std::thread::hardware_concurrency();
    int min_trades = 5;
    double min_win_rate = 55.0;
    bool use_sl = true;
    bool use_tp = true;
    bool pyramiding = false;
    bool exclude_sl_from_winrate = false;
    
    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg.find("--strategies=") == 0) {
            std::string strats_str = arg.substr(13);
            strategies.clear();
            
            // Split comma-separated strategy list
            std::stringstream ss(strats_str);
            std::string strat;
            while (std::getline(ss, strat, ',')) {
                strategies.push_back(strat);
            }
        }
        else if (arg.find("--threads=") == 0) {
            num_threads = std::stoi(arg.substr(10));
        }
        else if (arg.find("--min-trades=") == 0) {
            min_trades = std::stoi(arg.substr(13));
        }
        else if (arg.find("--min-winrate=") == 0) {
            min_win_rate = std::stod(arg.substr(14));
        }
        else if (arg == "--no-sl") {
            use_sl = false;
        }
        else if (arg == "--no-tp") {
            use_tp = false;
        }
        else if (arg == "--pyramiding") {
            pyramiding = true;
        }
        else if (arg == "--exclude-sl") {
            exclude_sl_from_winrate = true;
        }
    }
    
    // Load price data
    std::cout << "Loading data from " << filename << "..." << std::endl;
    auto bars = StrategyBacktester::loadCSV(filename);
    
    if (bars.empty()) {
        std::cerr << "Failed to load data or file is empty." << std::endl;
        return 1;
    }
    
    std::cout << "Loaded " << bars.size() << " bars from " << bars.front().date << " to " << bars.back().date << std::endl;
    
    // Define SL/TP ranges
    std::vector<double> sl_percents;
    for (double i = 0.5; i <= 3.0; i += 0.5) {
        sl_percents.push_back(i);
    }
    
    std::vector<double> tp_percents;
    for (double i = 0.4; i <= 1.0; i += 0.1) {
        tp_percents.push_back(i);
    }
    
    // Create and run multi-strategy optimizer
    MultiStrategyOptimizer optimizer(
        bars,
        strategies,
        sl_percents,
        tp_percents,
        use_sl,
        use_tp,
        pyramiding,
        10000.0, // initial capital
        min_trades,
        min_win_rate,
        exclude_sl_from_winrate,
        num_threads
    );
    
    optimizer.optimizeAll();
    
    return 0;
}