/* 
 * File:   dhash.h
 * Author: dudae
 *
 * Created on Pondelok, 2015, júna 29, 16:42
 */

#ifndef DHASH_H
#define	DHASH_H

#include <stdint.h>
#include <string>
#include <vector>

namespace dhash {
    
    std::vector<uint64_t> compute(const std::string& filename, int size = 256);
    
    std::vector<uint64_t> compute(const std::vector<uint8_t>& data, int size = 256);
}

#endif	/* DHASH_H */

