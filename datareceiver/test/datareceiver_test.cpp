#include <gtest/gtest.h>

#include "buffermanager/buffermanager.h"
#include "msg/msg.h"
#include "mylist/mylist.h"
#include "datareceiver/datareceiver.h"


///// тест создания одного и того же сообщения раз в интервал
TEST(DataReceiverTests, MessagePullingTest) {
    std::vector<double> testData1 = {1,2,3,4,5};
    std::vector<int> testData2 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    double testData3 = 4124;
    int testData4 = 41421;
    std::string testData5 = "dollbyopp";

    void* arr[5];
    arr[0] = &testData1;
    arr[1] = &testData2;
    arr[2] = &testData3;
    arr[3] = &testData4;
    arr[4] = &testData5;

    MyList<Message> MessagesQueue;

    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // засунули все сообщения в очередь
    for (int i = 0; i < 5; i++) {
        Message msg(arr[MessagesQueue.getSize()], MessagesQueue.getSize()+1);
        MessagesQueue.pushBack(msg);
    }

    // передаем сообщения в память
    size_t currentPosition = 0;
    for (int i = 0; i < 5; i++) {
        std::vector<uint8_t> tempMessageSequenced = divideMessageIntoBytes(MessagesQueue.front());
        MessagesQueue.popFront(); // удаляем его из очереди
        bufferManager.writePocket(tempMessageSequenced, currentPosition);
        currentPosition = (uint8_t)ceil((double)tempMessageSequenced.size() / (double)pocketSize) + currentPosition;
    }

    std::vector<Message> ReceiverMessagesQueue;

    DataReceiver dataReceiver = DataReceiver(sharedMemoPtr, std::chrono::milliseconds(500), &ReceiverMessagesQueue, pocketSize);

    dataReceiver.tryPullMsg();
    std::chrono::milliseconds duration(4000);

    auto StartTime = std::chrono::steady_clock::now();
    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        bool msgWasPulled = dataReceiver.tryPullMsg();;
        if (msgWasPulled == true){
            std::cout << "msg was pulled" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "pulling finished" << std::endl;

    EXPECT_EQ(ReceiverMessagesQueue.size(), 5);

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(ReceiverMessagesQueue[0].getPayload(), ReceiverMessagesQueue[0].getMessageType()));
    int i = 0;
    for (const double value : testData1) {
        EXPECT_EQ(checkData1[i++], value);
    }
    std::cout << "first msg is OK" << std::endl;

    auto checkData2 = any_cast<std::vector<int>>(getDataFromBytes(ReceiverMessagesQueue[1].getPayload(), ReceiverMessagesQueue[1].getMessageType()));
    i = 0;
    for (const double value : testData2) {
        EXPECT_EQ(checkData2[i++], value);
    }
    std::cout << "second msg is OK" << std::endl;

    auto checkData3 = any_cast<double>(getDataFromBytes(ReceiverMessagesQueue[2].getPayload(), ReceiverMessagesQueue[2].getMessageType()));
    EXPECT_EQ(checkData3, testData3);
    std::cout << "third msg is OK" << std::endl;

    auto checkData4 = any_cast<int>(getDataFromBytes(ReceiverMessagesQueue[3].getPayload(), ReceiverMessagesQueue[3].getMessageType()));
    EXPECT_EQ(checkData4, testData4);
    std::cout << "fourth msg is OK" << std::endl;

    auto checkData5 = any_cast<std::string>(getDataFromBytes(ReceiverMessagesQueue[4].getPayload(), ReceiverMessagesQueue[4].getMessageType()));
    EXPECT_EQ(checkData5, testData5);
    std::cout << "fifth msg is OK" << std::endl;
}
