#ifndef PROCESSING
#define PROCESSING

#include <cstdint>
#include <vector>
#include <iostream>

inline uint32_t getDataHash(const std::vector<uint8_t>& data) {
    // в планах использовать для хеширования CRC методику, с полиномом CRC-8-CCITT:
    // x^8 + x^2 + x + 1 = 100000111
    uint32_t hash = 0;

    // ...но пока использую банальную контрольную сумму
    for (size_t i = 0; i < data.size(); ++i) {
        hash += data[i];
        std::cout << data[i] << " ";
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




#endif
