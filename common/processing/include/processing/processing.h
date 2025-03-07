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
    for (const int i : data) {
        hash += static_cast<uint32_t>(data[i]);
    }
    return hash;

}

template <typename T>
concept HasDataMethod = requires(T obj) {
    { obj.data() } -> std::same_as<typename T::value_type*>; // data() должен возвращать указатель на value_type
};

template <typename T>
std::vector<uint8_t> divideDataIntoBytes(const T& data) {
    std::vector<uint8_t> bytesOfData;

    if constexpr (HasDataMethod<T>) {

        // Получаем указатель на данные контейнера
        auto* dataPointer = data.data();

        // Проверяем, что контейнер не пустой
        if (data.size() > 0) {
            // Вычисляем общий размер данных в байтах
            size_t dataSize = data.size() * sizeof(typename T::value_type);

            // Подготавливаем вектор для копирования данных
            bytesOfData.resize(dataSize);

            // Копируем данные с помощью memcpy
            std::memcpy(bytesOfData.data(), dataPointer, dataSize);

        } else {
        }
    } else {
        bytesOfData.resize(sizeof(data));
        memcpy(bytesOfData.data(), &data, sizeof(data));
    }

    return bytesOfData;
}


inline std::any getDataFromBytes(const std::vector<uint8_t>& bytes, uint8_t type) {
    uint8_t data[bytes.size()];
    for (size_t i = 0; i < bytes.size(); ++i) {
        data[i] = bytes[i];
    }
    switch (type) {
        case 1: {
            std::vector<double> recoveredVector(bytes.size(), 0.0);
            unsigned int recoveredSize = bytes.size() / sizeof(double);
            for (size_t i = 0; i < recoveredSize; ++i) {
                uint8_t tempValue[sizeof(double)] = {0};
                for (size_t j = 0; j < sizeof(double); ++j) {
                    tempValue[j] = data[sizeof(double) * i + j];
                }
                recoveredVector[i] = *reinterpret_cast<double*>(tempValue);
            }
            return recoveredVector;
        }
        case 2: {
            std::vector<int> recoveredVector(bytes.size(), 0);
            unsigned int recoveredSize = bytes.size() / sizeof(int);
            for (size_t i = 0; i < recoveredSize; ++i) {
                uint8_t tempValue[sizeof(int)] = {0};
                for (size_t j = 0; j < sizeof(int); ++j) {
                    tempValue[j] = data[sizeof(int) * i + j];
                }
                recoveredVector[i] = *reinterpret_cast<int*>(tempValue);
            }
            return recoveredVector;
        }
        case 3:
            {return *reinterpret_cast<double*>(data);}
        case 4:
            {return *reinterpret_cast<int*>(data);}
        case 5:{
            std::string recoveredString;
            recoveredString.resize(bytes.size());
            for (size_t i = 0; i < bytes.size(); ++i) {
                recoveredString[i] = static_cast<char>(data[i]);
            }
            return recoveredString;
        }
        default: return 0;
    }
}



#endif
