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
#include <istream>
#include <vector>

namespace dhash {
    
    uint64_t compute(const std::string& filename);
    
    uint64_t compute(const std::vector<uint8_t>& data);
}

#endif	/* DHASH_H */

