# TDigest Algorithm in C++

This project provides a C++ implementation of the t-digest algorithm, a streaming approximation algorithm for efficient quantile estimation. It includes the core algorithm as well as a test example to demonstrate its usage.

## Introduction to TDigest

The t-digest algorithm is designed for calculating approximate quantiles (percentiles) over streaming data. It maintains a dynamic set of centroids, each representing a cluster of data points. The algorithm works by merging new data into these centroids while keeping the centroids' sizes and positions such that they provide an accurate estimate of the data distribution, especially at the tails.

### Key Features:
- **Streaming Algorithm:** Handles data streams efficiently without needing to store the entire dataset in memory.
- **Approximate Quantiles:** Provides quick estimates of any quantile with a high degree of accuracy.
- **Time Complexity:** 
  - **Insertion/Update:** O(log K), where K is the number of centroids.
  - **Percentile Calculation:** O(K), where K is the number of centroids.

## How to Use

### Parameters: Delta and K

- **Delta:** Controls the compression factor. A lower delta value results in fewer centroids, leading to higher compression but potentially lower accuracy. A higher delta value increases the number of centroids, offering better accuracy at the cost of higher memory usage.
- **K:** Determines the maximum number of centroids. A larger K allows the algorithm to maintain more centroids, which can improve the accuracy of percentile calculations but increases the computational cost.

### Recommendations:
- **Small datasets:** Use a higher delta and smaller K for fast computation with acceptable accuracy.
- **Large datasets:** Adjust delta and K according to your memory and accuracy needs. For higher accuracy, use a lower delta and larger K.

## Running the Example

To compile and run the provided test example, use the following commands:

```sh
g++ -std=c++17 -o tdigest_test tdigest.cpp tdigest_test.cpp
./tdigest_test

### Example Output
Total number of centroids: 859
5th percentile: 499.514 (calculation time: 7.624 microseconds)
10th percentile: 1000.2 (calculation time: 7.713 microseconds)
15th percentile: 1498.86 (calculation time: 7.814 microseconds)
20th percentile: 2007.01 (calculation time: 8.367 microseconds)
25th percentile: 2499.79 (calculation time: 8.893 microseconds)
30th percentile: 2994.4 (calculation time: 9.474 microseconds)
35th percentile: 3512.5 (calculation time: 10.035 microseconds)
40th percentile: 4016.05 (calculation time: 10.696 microseconds)
45th percentile: 4534.72 (calculation time: 11.195 microseconds)
50th percentile: 5013.92 (calculation time: 12.144 microseconds)
55th percentile: 5520.65 (calculation time: 12.367 microseconds)
60th percentile: 5992.84 (calculation time: 12.982 microseconds)
65th percentile: 6512.12 (calculation time: 13.715 microseconds)
70th percentile: 6977.11 (calculation time: 14.394 microseconds)
75th percentile: 7496.15 (calculation time: 14.898 microseconds)
80th percentile: 7995.46 (calculation time: 15.685 microseconds)
85th percentile: 8494.68 (calculation time: 16.442 microseconds)
90th percentile: 8995.59 (calculation time: 17.095 microseconds)
95th percentile: 9499.52 (calculation time: 18.657 microseconds)
