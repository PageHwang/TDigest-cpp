#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "json.hpp"
#include "csv.h"
#include <fstream>

#include "tdigest.h"

Centroid::Centroid(double mean, double count)
    : mean(mean), count(count) {}

void Centroid::update(double x, double weight) {
    count += weight;
    mean += weight * (x - mean) / count;
}

std::ostream& operator<<(std::ostream &os, const Centroid &centroid) {
    os << "<Centroid: mean=" << centroid.mean << ", count=" << centroid.count << ">";
    return os;
}

bool Centroid::operator==(const Centroid &other) const {
    return mean == other.mean && count == other.count;
}

TDigest::TDigest(double delta, int K)
    : n(0), delta(delta), K(K) {}

size_t TDigest::getCentroidCount() const {
    return C.size();
}

TDigest TDigest::operator+(const TDigest &other_digest) const {
    std::vector<Centroid> data;
    data.reserve(C.size() + other_digest.C.size());

    for (const auto& pair : C) {
        data.push_back(pair.second);
    }

    for (const auto& pair : other_digest.C) {
        data.push_back(pair.second);
    }

    TDigest new_digest(delta, K);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);

    for (const auto& centroid : data) {
        new_digest.update(centroid.mean, centroid.count);
    }

    return new_digest;
}

void TDigest::update(double x, double w) {
    n += w;

    if (C.empty()) {
        C.insert({x, Centroid(x, w)});
        return;
    }

    auto it = C.find(x);
    if (it != C.end()) {
        it->second.update(x, w);
        return;
    }

    auto S = find_closest_centroids(x);

    while (!S.empty() && w > 0) {
        int j = rand() % S.size();
        auto it = S[j];
        Centroid &c_j = it->second;

        double q = compute_centroid_quantile(c_j);

        if (c_j.count + w > threshold(q)) {
            S.erase(S.begin() + j);
            continue;
        }

        double delta_w = std::min(threshold(q) - c_j.count, w);
        c_j.update(x, delta_w);
        w -= delta_w;
        S.erase(S.begin() + j);
    }

    if (w > 0) {
        C.emplace(x, Centroid(x, w));
    }

    if (C.size() > static_cast<size_t>(K / delta)) {
        compress();
    }
}

double TDigest::percentile(double p) {
    if (p < 0 || p > 100) {
        throw std::invalid_argument("Percentile must be between 0 and 100.");
    }

    p = p / 100.0 * n;

    double cumulative_count = 0.0;

    auto prev_it = C.begin();
    auto it = C.begin();

    for (it = C.begin(); it != C.end(); ++it) {
        double current_count = it->second.count;
        double k = 0;

        if (it == C.begin()) {
            k = current_count / 2.0;
        } else {
            k = (prev_it->second.count + current_count) / 2.0;
        }

        if (p <= cumulative_count + k) {
            double z1 = p - cumulative_count;
            double z2 = cumulative_count + k - p;
            return (prev_it->second.mean * z2 + it->second.mean * z1) / k;
        }

        cumulative_count += k;
        prev_it = it;
    }

    return C.rbegin()->second.mean;
}

std::vector<std::map<double, Centroid>::iterator> TDigest::find_closest_centroids(double x) {
    std::vector<std::map<double, Centroid>::iterator> result;

    auto lower = C.lower_bound(x);
    auto upper = C.upper_bound(x);

    if (lower != C.end()) {
        result.push_back(lower);
    }

    if (lower != C.begin()) {
        result.push_back(--lower);
    }

    if (upper != C.end() && upper != lower) {
        result.push_back(upper);
    }

    return result;
}

double TDigest::compute_centroid_quantile(const Centroid &centroid) {
    double cumulative_sum = 0.0;
    
    for (auto it = C.begin(); it != C.end(); ++it) {
        if (it->first == centroid.mean) {
            break;
        }
        cumulative_sum += it->second.count;
    }

    return (centroid.count / 2.0 + cumulative_sum) / n;
}

double TDigest::threshold(double q) {
    return 4 * n * delta * q * (1 - q);
}

void TDigest::compress() {
    TDigest T(delta, K);
    std::vector<Centroid> centroids;

    for (const auto& pair : C) {
        centroids.push_back(pair.second);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(centroids.begin(), centroids.end(), g);

    for (const auto& centroid : centroids) {
        T.update(centroid.mean, centroid.count);
    }

    C = T.C;
}
