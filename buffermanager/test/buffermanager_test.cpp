#include <gtest/gtest.h>
#include "buffermanager/buffermanager.h"

///// тест инициализации памяти и менеджера буфера
TEST(BufferManagerTests, BufferManagerReadWriteTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t offset = 100;

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, offset);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении
    bufferManager.writePocket(dataToWrite, 5, pocketSize); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> read_data = bufferManager.readPocket(dataToWrite.size(), 5, pocketSize);
    int i = 1;
    for (uint8_t byte : read_data) {
        EXPECT_EQ(static_cast<int>(byte), i++);
    }
    std::cout << std::endl;
}

TEST(BufferManagerTests, BufferManagerPocketsCreationTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t offset = 100;

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, offset);

    std::vector<uint8_t> dataToWrite = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

    size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении
    std::vector<std::vector<uint8_t>> createdPockets = bufferManager.createPockets(dataToWrite, pocketSize);

    EXPECT_EQ(createdPockets.size(), 4);
}

TEST(BufferManagerTests, BufferManagerPocketsHeaderTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t offset = 100;

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, offset);

    std::vector<uint8_t> dataToWrite = {11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

    size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении
    std::vector<std::vector<uint8_t>> createdPockets = bufferManager.createPockets(dataToWrite, pocketSize);

    for (size_t i = 0; i < createdPockets.size(); ++i) {
        EXPECT_EQ(createdPockets[i][0], 1);
        EXPECT_EQ(createdPockets[i][1], i);
    }

}
