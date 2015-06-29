#include "dhash.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

static inline void addOneBit(uint64_t& value, const size_t pos) {
    value |= (uint64_t) 1 << (63 - pos);
}

uint64_t compute_internal(Mat& image) {
    Mat resized_image;
    resize(image, resized_image, Size(9, 8), 0, 0, INTER_LANCZOS4);
    
    uint64_t hash = 0;
    size_t pos = 0;
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            uchar lbright = resized_image.at<uchar>(row, col);
            uchar rbright = resized_image.at<uchar>(row, col + 1);
            
            if (lbright > rbright ) {
                addOneBit(hash, pos);
            }
            pos++;
        }
    }
    return hash;
}

uint64_t dhash::compute(const std::string& filename) {
    Mat image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    return compute_internal(image);
}
    
uint64_t dhash::compute(const std::vector<uint8_t>& data) {
    Mat image = imdecode(data, CV_LOAD_IMAGE_GRAYSCALE);
    return compute_internal(image);
}
