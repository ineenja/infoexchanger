#include "common/msg/include/msg/msg.h"
#include "common/mylist/include/mylist/mylist.h"

#include "buffermanager/buffermanager.h"
#include "datareceiver/datareceiver.h"
#include "datasource/datasource.h"


int main()
{
    std::string testData = "dollbyopp";

    MyList<Message> MessagesQueue;

    const std::string shmName = "MySharedMemory";
    const size_t shmSize = 1024;
    const size_t pocketSize = 10; // количество байт информации, содержащейся в одном сообщении

    SharedMemMemorySupplier sharedMemo(shmName, shmSize);
    SharedMemMemorySupplier* sharedMemoPtr = &sharedMemo;

    // Создаем менеджер буферов
    BufferManager bufferManager(sharedMemoPtr, pocketSize);

    // засунули все сообщения в очередь
    Message msg(testData);
    MessagesQueue.pushBack(msg);

    // передаем сообщения в память
    size_t currentPosition = 0;
    std::vector<uint8_t> tempMessageSequenced = divideMessageIntoBytes(MessagesQueue.front());
    MessagesQueue.popFront(); // удаляем его из очереди
    bufferManager.writePocket(tempMessageSequenced, currentPosition);
    currentPosition = (uint8_t)ceil((double)tempMessageSequenced.size() / (double)pocketSize) + currentPosition;

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

    auto checkData = any_cast<std::string>(getDataFromBytes(ReceiverMessagesQueue[0].getPayload(), ReceiverMessagesQueue[0].getMessageType()));

    std::cout << "the message is: " << checkData << std::endl;

    return 0;
}

