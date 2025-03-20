#include <gtest/gtest.h>
#include "buffermanager/buffermanager.h"

TEST(BufferManagerTests, BufferManagerPocketsCreationTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10;

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, pocketSize);

    std::vector<uint8_t> dataToWrite = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

    std::vector<std::vector<uint8_t>> createdPockets = bufferManager.createPockets(dataToWrite);

    EXPECT_EQ(createdPockets.size(), 4);
}

TEST(BufferManagerTests, BufferManagerPocketsHeaderTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, pocketSize);

    std::vector<uint8_t> dataToWrite = {11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

    std::vector<std::vector<uint8_t>> createdPockets = bufferManager.createPockets(dataToWrite);

    for (size_t i = 0; i < createdPockets.size(); ++i) {
        EXPECT_EQ(createdPockets[i][0], 228);
        EXPECT_EQ(createdPockets[i][1], 1);
        EXPECT_EQ(createdPockets[i][2], i+1);
        EXPECT_EQ(createdPockets[i][3], 3);
    }
    EXPECT_EQ(createdPockets[0][4], 10);
    EXPECT_EQ(createdPockets[1][4], 10);
    EXPECT_EQ(createdPockets[2][4], 1);

    for (size_t i = 0; i < createdPockets[0][4]; ++i) {
        EXPECT_EQ(createdPockets[0][i+5], i+11);
    }
    for (size_t i = 0; i < createdPockets[1][4]; ++i) {
        EXPECT_EQ(createdPockets[1][i+5], i+21);
    }
    for (size_t i = 0; i < createdPockets[2][4]; ++i) {
        EXPECT_EQ(createdPockets[2][i+5], i+31);
    }
}

///// тест инициализации памяти и менеджера буфера
TEST(BufferManagerTests, BufferManagerReadWriteOnePocketTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite = {1,2,3,4,5};
    bufferManager.writePocket(dataToWrite, 5); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData = bufferManager.readPocket(5);

    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(readData[i], i+1);
    }
    EXPECT_EQ(readData.size(), 5);

    std::cout << std::endl;
}

TEST(BufferManagerTests, BufferManagerReadWriteManyPocketsOneMsgTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    bufferManager.writePocket(dataToWrite, 5); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData = bufferManager.readPocket(5);

    for (size_t i = 0; i < dataToWrite.size(); ++i) {
        EXPECT_EQ(readData[i], dataToWrite[i]);
    }
}

