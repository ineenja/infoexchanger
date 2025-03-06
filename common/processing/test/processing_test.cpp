#include <gtest/gtest.h>
#include <processing/processing.h>

TEST(ProcessingTests, DivideDataIntoBytesCheckWithVector) {

    std::vector<int> testData1 = {1,2,3};
    std::vector<uint8_t> checkVector1 = divideDataIntoBytes(testData1);
    EXPECT_EQ(checkVector1.size(), 3 * 4);

    std::vector<double> testData2 = {1,2,3,4,5};
    std::vector<uint8_t> checkVector2 = divideDataIntoBytes(testData2);
    EXPECT_EQ(checkVector2.size(), 8 * 5);

    std::vector<double> testData3;
    std::vector<uint8_t> checkVector3 = divideDataIntoBytes(testData3);
    EXPECT_EQ(checkVector3.size(), 0);
}

TEST(ProcessingTests, DivideDataIntoBytesCheckWithString) {

    std::string testData1 = "penis";
    std::vector<uint8_t> checkVector1 = divideDataIntoBytes(testData1);
    EXPECT_EQ(checkVector1.size(), 5);

    std::string testData2 = "society";
    std::vector<uint8_t> checkVector2 = divideDataIntoBytes(testData2);
    EXPECT_EQ(checkVector2.size(), 7);
}

TEST(ProcessingTests, DivideDataIntoBytesCheckWithDouble) {

    double testData1 = 228;
    std::vector<uint8_t> checkVector1 = divideDataIntoBytes(testData1);
    EXPECT_EQ(checkVector1.size(), 8);

    double testData2 = 1337;
    std::vector<uint8_t> checkVector2 = divideDataIntoBytes(testData2);
    EXPECT_EQ(checkVector2.size(), 8);
}

TEST(ProcessingTests, getDataFromBytesCheckWithVector) {

    std::vector<double> testData1 = {1,2,3};
    std::vector<uint8_t> checkVector1 = divideDataIntoBytes(testData1);
    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(checkVector1, 1));
    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }

    std::vector<int> testData2 = {1,2,3,4,5};
    std::vector<uint8_t> checkVector2 = divideDataIntoBytes(testData2);
    auto checkData2 = any_cast<std::vector<int>>(getDataFromBytes(checkVector2, 2));
    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData2[i], checkData2[i]);
    }
}

TEST(ProcessingTests, getDataFromBytesCheckWithString) {

    std::string testData1 = "playboi carti";
    std::vector<uint8_t> checkVector1 = divideDataIntoBytes(testData1);
    auto checkData1 = any_cast<std::string>(getDataFromBytes(checkVector1, 5));

    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }
}