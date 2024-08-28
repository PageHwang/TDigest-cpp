#include "tdigest.h"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

int main(int argc, char *argv[]) {
    double delta=1;
    int K = 10;

    if (argc>1){
        delta = std::stod(argv[1]);
    }
    if (argc>2){
        K = std::stoi(argv[2]);
    }
    
    std::string bid = "BTCUSDT";
    std::string ex = "U";
    std::string date = "2024-05-31";

    int64_t tradeid;
    int64_t tradets;
    double price, qty, amount;
    std::string is_sell_str;
    bool is_sell;

    std::string read_path = "/data/hdd0/binance/trades/" + bid + "/" + ex+ "/" + bid + "-trades-" + date + ".csv";
    io::CSVReader<6> in(read_path);
    in.read_header(io::ignore_extra_column, "id", "price", "qty", "quote_qty", "time", "is_buyer_maker");

    TDigest T(delta, K);
    auto start = std::chrono::high_resolution_clock::now();
    int trade_count = 0;
    double max_q = 0;
    while (in.read_row(tradeid, price, qty, amount, tradets, is_sell_str)) {
        T.update(qty,1);
        max_q = std::max(max_q,qty);
        trade_count ++;
    }

    for (double p = 5.0; p <= 100.0; p += 5.0) {
        double percentile_value = T.percentile(p);
        std::cout << p << "th percentile: " << percentile_value << std::endl;
    }

    int C_count = 0;
    for (const auto& pair : T.C) {
        C_count += pair.second.count;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;
    std::cout << " C_n = " << T.n << " C_count = " << C_count << " Max Qty = " << max_q << " Average Time = " << elapsed.count() / trade_count << " microseconds" << std::endl;

    std::cout << "Final number of centroids after percentile calculations: " << T.getCentroidCount() << std::endl;

    return 0;
}
