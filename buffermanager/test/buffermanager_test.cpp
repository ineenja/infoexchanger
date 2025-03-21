#include <gtest/gtest.h>
#include "buffermanager/buffermanager.h"

TEST(BufferManagerTests, BufferManagerPocketsCreationTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10;

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    std::vector<uint8_t> dataToWrite = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

    std::vector<std::vector<uint8_t>> createdPockets = bufferManager.createPockets(dataToWrite);

    EXPECT_EQ(createdPockets.size(), 4);
}

TEST(BufferManagerTests, BufferManagerPocketsHeaderTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

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

TEST(BufferManagerTests, BufferManagerReadWriteOnePocketOneMsgTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite = {1,2,3,4,5};
    bufferManager.writePocket(dataToWrite, 5); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData = bufferManager.readPocket(5);

    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(readData[i], i+1);
    }
    EXPECT_EQ(readData.size(), 5);

    std::cout << "One pocket one msg test is over" << std::endl << std::endl;
}

TEST(BufferManagerTests, BufferManagerReadWriteManyPocketsOneMsgTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    bufferManager.writePocket(dataToWrite, 5); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData = bufferManager.readPocket(5);

    for (size_t i = 0; i < dataToWrite.size(); ++i) {
        EXPECT_EQ(readData[i], dataToWrite[i]);
    }

    std::cout << "many pockets one msg test is over" << std::endl << std::endl;
}

TEST(BufferManagerTests, BufferManagerReadWriteOnePocketManyMsgsNextToEachOtherTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite1 = {1,2,3,4,5};
    bufferManager.writePocket(dataToWrite1, 5);

    std::vector<uint8_t> dataToWrite2 = {1,2,3,4,5,6,7};
    bufferManager.writePocket(dataToWrite2, 6);

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData1 = bufferManager.readPocket(5);

    std::vector<uint8_t> readData2 = bufferManager.readPocket(6);

    for (size_t i = 0; i < dataToWrite1.size(); ++i) {
        EXPECT_EQ(readData1[i], i+1);
    }
    EXPECT_EQ(readData1.size(), 5);

    for (size_t i = 0; i < dataToWrite2.size(); ++i) {
        EXPECT_EQ(readData2[i], i+1);
    }
    EXPECT_EQ(readData2.size(), 7);

    std::cout << "One pocket many msg test next to each other is over" << std::endl << std::endl;
}

TEST(BufferManagerTests, BufferManagerReadWriteOnePocketManyMsgsSplitApartTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite1 = {1,2,3,4,5};
    bufferManager.writePocket(dataToWrite1, 5);

    std::vector<uint8_t> dataToWrite2 = {1,2,3,4,5,6,7};
    bufferManager.writePocket(dataToWrite2, 10);

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData1 = bufferManager.readPocket(5);

    std::vector<uint8_t> readData2 = bufferManager.readPocket(10);

    for (size_t i = 0; i < dataToWrite1.size(); ++i) {
        EXPECT_EQ(readData1[i], i+1);
    }
    EXPECT_EQ(readData1.size(), 5);

    for (size_t i = 0; i < dataToWrite2.size(); ++i) {
        EXPECT_EQ(readData2[i], i+1);
    }
    EXPECT_EQ(readData2.size(), 7);

    std::cout << "One pocket many msg test split apart is over" << std::endl << std::endl;
}

TEST(BufferManagerTests, BufferManagerReadWriteManyPocketsManyMsgsNextToEachOtherTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite1 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    bufferManager.writePocket(dataToWrite1, 5); // Записываем начиная с индекса 0

    std::vector<uint8_t> dataToWrite2 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
    bufferManager.writePocket(dataToWrite2, 9); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData1 = bufferManager.readPocket(5);
    for (size_t i = 0; i < dataToWrite1.size(); ++i) {
        EXPECT_EQ(readData1[i], dataToWrite1[i]);
    }

    std::vector<uint8_t> readData2 = bufferManager.readPocket(9);
    for (size_t i = 0; i < dataToWrite2.size(); ++i) {
        EXPECT_EQ(readData2[i], dataToWrite2[i]);
    }

    std::cout << "many pockets many msg test next to each other is over" << std::endl << std::endl;
}

TEST(BufferManagerTests, BufferManagerReadWriteManyPocketsManyMsgsSplitApartTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite1 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    bufferManager.writePocket(dataToWrite1, 5); // Записываем начиная с индекса 0

    std::vector<uint8_t> dataToWrite2 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
    bufferManager.writePocket(dataToWrite2, 10); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> readData1 = bufferManager.readPocket(5);
    for (size_t i = 0; i < dataToWrite1.size(); ++i) {
        EXPECT_EQ(readData1[i], dataToWrite1[i]);
    }

    std::vector<uint8_t> readData2 = bufferManager.readPocket(10);
    for (size_t i = 0; i < dataToWrite2.size(); ++i) {
        EXPECT_EQ(readData2[i], dataToWrite2[i]);
    }

    std::cout << "many pockets many msg test split apart is over" << std::endl << std::endl;
}