#include "tdigest.h"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "csv.h"
#include <fstream>

int main(int argc, char *argv[]) {
    double delta = 0.01;
    int K = 10;

    if (argc > 1) {
        delta = std::stod(argv[1]);
    }
    if (argc > 2) {
        K = std::stoi(argv[2]);
    }

    std::vector<double> data;
    for (double i = 0.0; i <= 10000.0; i += 0.1) {
        data.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);

    TDigest T(delta, K);
    int n = 0;

    for (const auto& value : data) {
        auto start = std::chrono::high_resolution_clock::now(); 
        T.update(value);
        auto end = std::chrono::high_resolution_clock::now();   

        std::chrono::duration<double, std::micro> elapsed = end - start;
    }

    std::cout << "Total number of centroids: " << T.getCentroidCount() << std::endl;

    for (int p = 5; p <= 99; p += 5) {
        auto start = std::chrono::high_resolution_clock::now();
        double percentile_value = T.percentile(p);
        auto end = std::chrono::high_resolution_clock::now(); 

        std::chrono::duration<double, std::micro> elapsed = end - start;
        std::cout << p << "th percentile: " << percentile_value 
                  << " (calculation time: " << elapsed.count() << " microseconds)" << std::endl;
    }

    std::cout << "Final number of centroids after percentile calculations: " << T.getCentroidCount() << std::endl;

    return 0;
}
