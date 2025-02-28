#ifndef PROCESSING
#define PROCESSING

#include <any>
#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>

inline uint32_t getDataHash(const std::vector<uint8_t>& data) {
    // в планах использовать для хеширования CRC методику, с полиномом CRC-8-CCITT:
    // x^8 + x^2 + x + 1 = 100000111
    uint32_t hash = 0;

    // ...но пока использую банальную контрольную сумму
    for (size_t i = 0; i < data.size(); ++i) {
        hash += data[i];
    }
    std::cout << std::endl;

    return hash;
}

template <typename T>
std::vector<uint8_t> divideDataIntoBytes(const T& data) {
    std::vector<uint8_t> bytesOfData;
    bytesOfData.resize(sizeof(data));

    auto* byte = reinterpret_cast<const uint8_t*>(&data);
    for (size_t i = 0; i < sizeof(data); ++i) {
        bytesOfData[i] = *byte++;
    }

    return bytesOfData;
}

//     double getDataFromBytes(const std::vector<uint8_t>& bytes, uint32_t type) {
//     uint8_t data[bytes.size()];
//     for (size_t i = 0; i < bytes.size(); ++i) {
//         data[i] = bytes[i];
//         std::cout << data[i] << " ";
//     }
//     std::cout << std::endl;
//     double* doublePrt = reinterpret_cast<double*>(data);
//
//     return *doublePrt;
// }

std::any getDataFromBytes(const std::vector<uint8_t>& bytes, uint32_t type) {
    uint8_t data[bytes.size()];
    for (size_t i = 0; i < bytes.size(); ++i) {
        data[i] = bytes[i];
    }
    switch (type) {
        case 1:
            return *reinterpret_cast<std::vector<double>*>(data);
        case 2:
            return *reinterpret_cast<double*>(data);
        case 3:
            return *reinterpret_cast<int*>(data);
    }
}



#endif
