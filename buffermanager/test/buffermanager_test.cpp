#include <gtest/gtest.h>
#include "buffermanager/buffermanager.h"

///// тест инициализации памяти и менеджера буфера
TEST(BufferManagerTests, BufferManagerTest) {
    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t offset = 100;

    // Создаем менеджер буферов
    BufferManager bufferManager(shmName, shmSize, offset);

    // Пример записи данных в shared memory
    std::vector<uint8_t> dataToWrite = {0x01, 0x02, 0x03, 0x04, 0x05};
    bufferManager.writePocket(dataToWrite); // Записываем начиная с индекса 0

    // Пример чтения данных из shared memory
    std::vector<uint8_t> read_data = bufferManager.readPocket(dataToWrite.size());
    std::cout << "Read from shared memory: ";
    int i = 1;
    for (uint8_t byte : read_data) {
        EXPECT_EQ(static_cast<int>(byte), i++);
    }
    std::cout << std::endl;
}