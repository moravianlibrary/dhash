#include "dhash.h"

#include <cmath>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

inline bool is_power_of_two(int x) {
    return !(x == 0) && !(x & (x - 1));
}

static inline void addOneBit(uint64_t& value, const int pos) {
    value |= (uint64_t) 1 << (63 - pos);
}

vector<uint64_t> compute_internal(Mat& image, int blocksize) {
    resize(image, image, Size(blocksize + 1, blocksize), 0, 0, INTER_AREA);
    
    vector<uint64_t> result;
    uint64_t item;
    int pos = 0;
    
    for (int row = 0; row < blocksize; row++) {
        for (int col = 0; col < blocksize; col++) {
            uchar lbright = image.at<uchar>(row, col);
            uchar rbright = image.at<uchar>(row, col + 1);
            
            if (lbright > rbright ) {
                addOneBit(item, pos);
            }
            pos++;
            if (pos == 64) {
                pos = 0;
                result.push_back(item);
                item = 0;
            }
        }
    }
    return result;
}

vector<uint64_t> dhash::compute(const std::string& filename, int size) {
    if (!is_power_of_two(size)) {
        throw invalid_argument("Size must be power of 2.");
    }
    
    Mat image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    return compute_internal(image, sqrt(size));
}
    
vector<uint64_t> dhash::compute(const std::vector<uint8_t>& data, int size) {
    if (!is_power_of_two(size)) {
        throw invalid_argument("Size must be power of 2.");
    }
    
    Mat image = imdecode(data, CV_LOAD_IMAGE_GRAYSCALE);
    return compute_internal(image, sqrt(size));
}
