#include <gtest/gtest.h>
#include <processing/processing.h>

TEST(ProcessingTests, DivideDataIntoBytesCheckWithVector) {

    std::vector<int> testData1 = {1,2,3};

    std::vector<uint8_t> checkVector1 = divideDataIntoBytes(testData1);

    EXPECT_EQ(checkVector1.size(), 24 + 3 * 4);

    std::vector<double> testData2 = {1,2,3,4,5};

    std::vector<uint8_t> checkVector2 = divideDataIntoBytes(testData2);

    EXPECT_EQ(checkVector2.size(), 24 + 8 * 5);
}

TEST(ProcessingTests, DivideDataIntoBytesCheckWithString) {

    std::string testData1 = "penis";

    std::vector<uint8_t> checkVector1 = divideDataIntoBytes(testData1);

    EXPECT_EQ(checkVector1.size(), 32 + 5);

    std::string testData2 = "society";

    std::vector<uint8_t> checkVector2 = divideDataIntoBytes(testData2);

    EXPECT_EQ(checkVector2.size(), 32 + 7);
}