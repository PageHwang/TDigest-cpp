#ifndef TDIGEST_H
#define TDIGEST_H

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

class Centroid {
public:
    double mean;
    double count;

    Centroid(double mean, double count);

    void update(double x, double weight);

    friend std::ostream& operator<<(std::ostream &os, const Centroid &centroid);

    bool operator==(const Centroid &other) const;
};

class TDigest {
public:
    std::map<double, Centroid> C;
    double n;
    double delta;
    int K;

    TDigest(double delta = 0.5, int K = 10);

    size_t getCentroidCount() const;

    TDigest operator+(const TDigest &other_digest) const;

    void update(double x, double w = 1);

    double percentile(double p);

private:
    std::vector<std::map<double, Centroid>::iterator> find_closest_centroids(double x);

    double compute_centroid_quantile(const Centroid &centroid);

    double threshold(double q);

    void compress();
};

#endif // TDIGEST_H
