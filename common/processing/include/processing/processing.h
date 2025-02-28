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
        hash += static_cast<uint32_t>(data[i]);
    }
    return hash;
}

template <typename T>
std::vector<uint8_t> divideDataIntoBytes(const T& data, uint32_t type) {
    std::vector<uint8_t> bytesOfData;
    bytesOfData.resize(sizeof(data));
    memcpy(bytesOfData.data(), &data, sizeof(data));

    if constexpr (std::is_invocable_v<decltype(&T::data), T>) {
        std::cout << "i have some data within" << std::endl;
        // std::vector<uint8_t> ContainerData;
        // memcpy(ContainerData.data(), data.data(), data.size());
        // bytesOfData.insert(bytesOfData.end(), ContainerData.begin(), ContainerData.end());
    }

    return bytesOfData;
}


std::any getDataFromBytes(const std::vector<uint8_t>& bytes, uint32_t type) {
    uint8_t data[bytes.size()];
    for (size_t i = 0; i < bytes.size(); ++i) {
        data[i] = bytes[i];
    }
    std::cout << std::endl;
    switch (type) {
        case 1:
            return *reinterpret_cast<std::vector<double>*>(data);
        case 2:
            return *reinterpret_cast<double*>(data);
        case 3:
            return *reinterpret_cast<int*>(data);
        case 4:
            return *reinterpret_cast<std::string*>(data);
    }
}



#endif
