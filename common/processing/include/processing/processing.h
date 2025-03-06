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
concept HasDataMethod = requires(T obj) {
    { obj.data() } -> std::same_as<typename T::value_type*>; // data() должен возвращать указатель на value_type
};

template <typename T>
std::vector<uint8_t> divideDataIntoBytes(const T& data) {
    std::vector<uint8_t> bytesOfData;
    bytesOfData.resize(sizeof(data));
    memcpy(bytesOfData.data(), &data, sizeof(data));

    std::cout << "size of data is " << bytesOfData.size() << std::endl;

    if constexpr (HasDataMethod<T>) {

        std::cout << "there is some data within!" << std::endl;
        // Получаем указатель на данные контейнера
        auto* dataPointer = data.data();

        std::vector<uint8_t> bytesOfDataInData;

        // Проверяем, что контейнер не пустой
        if (data.size() > 0) {
            // Вычисляем общий размер данных в байтах
            size_t dataSize = data.size() * sizeof(typename T::value_type);

            // Подготавливаем вектор для копирования данных
            bytesOfDataInData.resize(dataSize);

            // Копируем данные с помощью memcpy
            std::memcpy(bytesOfDataInData.data(), dataPointer, dataSize);

            std::cout << "inner data size is " << bytesOfDataInData.size() << std::endl;

            bytesOfData.insert(bytesOfData.end(), bytesOfDataInData.begin(), bytesOfDataInData.end());

            std::cout << "whole data size is " << bytesOfData.size() << std::endl << std::endl;
        } else {
            std::cout << "Container is empty, nothing to copy." << std::endl;
        }
    } else {
        std::cout << "Object has no data() method." << std::endl;
    }

    // было бы полезно сделать проверку на то, что передается в качестве data.
    // если это, например, контейнер vector, то вызовется его метод data(),
    // и в вектор байт бы скопировались данные на которые указывает вектор.
    return bytesOfData;
}


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
        case 4:
            return *reinterpret_cast<std::string*>(data);
    }
}



#endif
