# Trading Strategy Optimizer

A multi-strategy trading backtester and optimizer for OTT-based indicators with parallel processing support. This tool allows you to backtest and optimize various trading strategies with customizable parameters and comprehensive result analysis.

## Features

- Supports multiple OTT-based trading strategies:
  - OTT (Optimized Trend Tracker)
  - TOTT (True OTT)
  - OTT_CHANNEL
  - RISOTTO (RSI OTT)
  - SOTT (Stochastic OTT)
  - HOTT-LOTT (Highest/Lowest OTT)
  - ROTT (Robust OTT)
  - FT (Filtered Trend)
  - RTR (Robust Trend Range)
  - MOTT (Modified OTT)
  - BOOTS (Bollinger OTT System)
  
- Multi-threaded optimization with parameter grid search
- Stop-loss and take-profit options
- Configurable optimization criteria
- Detailed backtest results and trade analysis
- Results export to CSV for further analysis

## Building from Source

### Prerequisites

- C++17 compatible compiler (GCC 8+, Clang 7+, or MSVC 2019+)
- CMake 3.10 or higher
- Git

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/trading-strategy-optimizer.git
cd trading-strategy-optimizer

# Create a build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .
```

## Usage

```
./optimizer <csv_file> [options]
```

### Command Line Options

- `--strategies=s1,s2,...` - Strategies to optimize (default: OTT)
- `--threads=N` - Number of threads to use (default: CPU cores)
- `--min-trades=N` - Minimum trades filter (default: 5)
- `--min-winrate=N` - Minimum win rate filter (default: 55)
- `--no-sl` - Disable stop loss
- `--no-tp` - Disable take profit
- `--pyramiding` - Enable pyramiding
- `--exclude-sl` - Exclude stop loss trades from win rate calculation

### Examples

```bash
# Optimize OTT strategy with default parameters
./optimizer data.csv

# Optimize multiple strategies with 8 threads
./optimizer data.csv --strategies=OTT,SOTT,MOTT --threads=8

# Optimize with custom criteria
./optimizer data.csv --min-trades=10 --min-winrate=60 --no-tp
```

## Input Data Format

The program expects CSV files with the following columns:
- Date (YYYY-MM-DD format)
- Open
- High
- Low
- Close
- Volume (optional)

Example:
```
Date,Open,High,Low,Close,Volume
2023-01-01,100.0,105.0,99.0,104.0,1000000
2023-01-02,104.0,106.0,102.0,105.0,1200000
...
```

## Output

Results are saved in the `results` directory, organized by strategy:
- `results/{strategy}/{strategy}_optimization_results.csv` - Contains all optimization results
- `results/{strategy}/trades/` - Contains detailed trade information for top parameter sets

## Strategy Parameters

Each strategy has its own set of parameters that can be optimized:

### OTT (Optimized Trend Tracker)
- `support_length`: Length parameter for the VIDYA moving average
- `ott_multiplier`: Percentage multiplier for the OTT calculation

### TOTT (True OTT)
- `support_length`: Length parameter for the VIDYA moving average
- `ott_multiplier`: Percentage multiplier for the OTT calculation
- `band_multiplier`: Multiplier for the band calculation

... (details for other strategies)

## License

[MIT](LICENSE)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.