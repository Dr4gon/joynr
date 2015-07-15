/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2015 BMW Car IT GmbH
 * %%
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * #L%
 */
#include <gtest/gtest.h>

#include "PrettyPrint.h"
#include "joynr/joynrlogging.h"

#include "joynr/types/Word.h"
#include "joynr/types/Vowel.h"
#include "joynr/types/TestTypes/StdTEverythingStruct.h"
#include "joynr/types/TestTypes/StdTEverythingExtendedStruct.h"
#include "joynr/types/TestTypes/StdTEverythingExtendedExtendedStruct.h"
using namespace joynr::types;

class StdComplexDataTypeTest : public testing::Test {
public:
    StdComplexDataTypeTest() :
            tInt8(-20),
            tUInt8(0x42),
            tInt16(-32768),
            tUInt16(65535),
            tInt32(-2147483648),
            tUInt32(4294967295),
            tInt64(6294967295),
            tUInt64(-61474836480),
            tDouble(0.00000000001),
            tFloat(0.00001),
            tString("München"),
            tBoolean(true),
            tByteBuffer({0x01, 0x02, 0x03, 0x04, 0x05}),
            tUInt8Array({0xFF, 0xFE, 0xFD, 0xFC}),
            tEnum(TEnum::TLITERALA),
            tEnumArray({TEnum::TLITERALA, TEnum::TLITERALB}),
            tStringArray({"New York", "London", "Berlin", "Tokio"}),
            word(StdWord()),
            wordArray({StdWord()}),
            tEverything1(
                    tInt8,
                    tUInt8,
                    tInt16,
                    tUInt16,
                    tInt32,
                    tUInt32,
                    tInt64,
                    tUInt64,
                    tDouble,
                    tFloat,
                    tString,
                    tBoolean,
                    tByteBuffer,
                    tUInt8Array,
                    tEnum,
                    tEnumArray,
                    tStringArray,
                    word,
                    wordArray),
            tBooleanExtended(false),
            tStringExtended("extended"),
            tEverythingExtended1(
                    tInt8,
                    tUInt8,
                    tInt16,
                    tUInt16,
                    tInt32,
                    tUInt32,
                    tInt64,
                    tUInt64,
                    tDouble,
                    tFloat,
                    tString,
                    tBoolean,
                    tByteBuffer,
                    tUInt8Array,
                    tEnum,
                    tEnumArray,
                    tStringArray,
                    word,
                    wordArray,
                    tBooleanExtended,
                    tStringExtended)
    {}

    virtual ~StdComplexDataTypeTest() {
    }

protected:
    static joynr::joynr_logging::Logger* logger;

    int8_t tInt8;
    uint8_t tUInt8;
    int16_t tInt16;
    uint16_t tUInt16;
    int32_t tInt32;
    uint32_t tUInt32;
    int64_t tInt64;
    uint64_t tUInt64;
    double tDouble;
    float tFloat;
    std::string tString;
    bool tBoolean;
    std::vector<uint8_t> tByteBuffer;
    std::vector<uint8_t>  tUInt8Array;
    TEnum::Enum tEnum;
    std::vector<joynr::types::TEnum::Enum>  tEnumArray;
    std::vector<std::string>  tStringArray;
    StdWord word;
    std::vector<StdWord>  wordArray;
    StdTEverythingStruct tEverything1;

    bool tBooleanExtended;
    std::string tStringExtended;
    StdTEverythingExtendedStruct tEverythingExtended1;
};

joynr::joynr_logging::Logger* StdComplexDataTypeTest::logger(
        joynr::joynr_logging::Logging::getInstance()->getLogger("TST", "StdComplexDataTypeTest")
);


TEST_F(StdComplexDataTypeTest, createComplexDataType) {
    EXPECT_EQ(tEverything1.getTBoolean(), tBoolean);
    EXPECT_EQ(tEverything1.getTByteBuffer(), tByteBuffer);
    EXPECT_EQ(tEverything1.getTDouble(), tDouble);
    EXPECT_EQ(tEverything1.getTEnum(), tEnum);
    EXPECT_EQ(tEverything1.getTEnumArray(), tEnumArray);
    EXPECT_EQ(tEverything1.getTEnumArray()[1], tEnumArray[1]);
    EXPECT_EQ(tEverything1.getTFloat(), tFloat);
    EXPECT_EQ(tEverything1.getTInt8(), tInt8);
    EXPECT_EQ(tEverything1.getTInt16(), tInt16);
    EXPECT_EQ(tEverything1.getTInt32(), tInt32);
    EXPECT_EQ(tEverything1.getTInt64(), tInt64);
    EXPECT_EQ(tEverything1.getTString(), tString);
    EXPECT_EQ(tEverything1.getTStringArray(), tStringArray);
    EXPECT_EQ(tEverything1.getTStringArray()[1], tStringArray[1]);
    EXPECT_EQ(tEverything1.getTUInt8(), tUInt8);
    EXPECT_EQ(tEverything1.getTUInt8Array(), tUInt8Array);
    EXPECT_EQ(tEverything1.getTUInt8Array()[1], tUInt8Array[1]);
    EXPECT_EQ(tEverything1.getTUInt16(), tUInt16);
    EXPECT_EQ(tEverything1.getTUInt32(), tUInt32);
    EXPECT_EQ(tEverything1.getTUInt64(), tUInt64);
}

TEST_F(StdComplexDataTypeTest, assignComplexDataType) {
    joynr::types::StdTEverythingStruct tEverything2 = tEverything1;
    EXPECT_EQ(tEverything2, tEverything1);
    EXPECT_EQ(tEverything2.getTBoolean(), tBoolean);
    EXPECT_EQ(tEverything2.getTByteBuffer(), tByteBuffer);
    EXPECT_EQ(tEverything2.getTDouble(), tDouble);
    EXPECT_EQ(tEverything2.getTEnum(), tEnum);
    EXPECT_EQ(tEverything2.getTEnumArray(), tEnumArray);
    EXPECT_EQ(tEverything2.getTEnumArray()[1], tEnumArray[1]);
    EXPECT_EQ(tEverything2.getTFloat(), tFloat);
    EXPECT_EQ(tEverything2.getTInt8(), tInt8);
    EXPECT_EQ(tEverything2.getTInt16(), tInt16);
    EXPECT_EQ(tEverything2.getTInt32(), tInt32);
    EXPECT_EQ(tEverything2.getTInt64(), tInt64);
    EXPECT_EQ(tEverything2.getTString(), tString);
    EXPECT_EQ(tEverything2.getTStringArray(), tStringArray);
    EXPECT_EQ(tEverything2.getTStringArray()[1], tStringArray[1]);
    EXPECT_EQ(tEverything2.getTUInt8(), tUInt8);
    EXPECT_EQ(tEverything2.getTUInt8Array(), tUInt8Array);
    EXPECT_EQ(tEverything2.getTUInt8Array()[1], tUInt8Array[1]);
    EXPECT_EQ(tEverything2.getTUInt16(), tUInt16);
    EXPECT_EQ(tEverything2.getTUInt32(), tUInt32);
    EXPECT_EQ(tEverything2.getTUInt64(), tUInt64);
}

TEST_F(StdComplexDataTypeTest, copyConstructorComplexDataTypeCopiesValues) {
    joynr::types::StdTEverythingStruct tEverything2(tEverything1);
    EXPECT_EQ(tEverything2, tEverything1);
    tEverything1.setTBoolean(!tEverything1.getTBoolean());
    EXPECT_NE(tEverything2, tEverything1);

    joynr::types::StdTEverythingStruct tEverything3(tEverything1);
    EXPECT_EQ(tEverything3, tEverything1);
    tEverything1.setTStringArray({"one", "two", "three"});
    EXPECT_NE(tEverything3, tEverything1);

    // make sure vectors are copied and not just assigned as references:
    // tEverything1 and tEverything4 should not have a reference or
    // pointer to changingVector
    std::vector<std::string> changingVector = {"one"};
    tEverything1.setTStringArray(changingVector);
    joynr::types::StdTEverythingStruct tEverything4(tEverything1);
    EXPECT_EQ(tEverything4, tEverything1);
    changingVector.push_back("two");
    tEverything4.setTStringArray(changingVector);
    EXPECT_NE(tEverything4, tEverything1);
}

TEST_F(StdComplexDataTypeTest, equalsOperator) {
    EXPECT_EQ(tEverything1, tEverything1);

    joynr::types::StdTEverythingStruct tEverything2(tEverything1);
    EXPECT_EQ(tEverything2, tEverything1);
    EXPECT_EQ(tEverything2, tEverything1);
    EXPECT_EQ(tEverything2.getTBoolean(), tBoolean);
    EXPECT_EQ(tEverything2.getTByteBuffer(), tByteBuffer);
    EXPECT_EQ(tEverything2.getTDouble(), tDouble);
    EXPECT_EQ(tEverything2.getTEnum(), tEnum);
    EXPECT_EQ(tEverything2.getTEnumArray(), tEnumArray);
    EXPECT_EQ(tEverything2.getTEnumArray()[1], tEnumArray[1]);
    EXPECT_EQ(tEverything2.getTFloat(), tFloat);
    EXPECT_EQ(tEverything2.getTInt8(), tInt8);
    EXPECT_EQ(tEverything2.getTInt16(), tInt16);
    EXPECT_EQ(tEverything2.getTInt32(), tInt32);
    EXPECT_EQ(tEverything2.getTInt64(), tInt64);
    EXPECT_EQ(tEverything2.getTString(), tString);
    EXPECT_EQ(tEverything2.getTStringArray(), tStringArray);
    EXPECT_EQ(tEverything2.getTStringArray()[1], tStringArray[1]);
    EXPECT_EQ(tEverything2.getTUInt8(), tUInt8);
    EXPECT_EQ(tEverything2.getTUInt8Array(), tUInt8Array);
    EXPECT_EQ(tEverything2.getTUInt8Array()[1], tUInt8Array[1]);
    EXPECT_EQ(tEverything2.getTUInt16(), tUInt16);
    EXPECT_EQ(tEverything2.getTUInt32(), tUInt32);
    EXPECT_EQ(tEverything2.getTUInt64(), tUInt64);
}

TEST_F(StdComplexDataTypeTest, notEqualsOperator) {
    joynr::types::StdTEverythingStruct tEverything2;
    EXPECT_NE(tEverything1, tEverything2);

    tEverything2 = tEverything1;
    EXPECT_EQ(tEverything1, tEverything2);

    tEverything1.setTBoolean(true);
    tEverything2.setTBoolean(false);
    EXPECT_NE(tEverything1, tEverything2);
}

TEST_F(StdComplexDataTypeTest, createExtendedComplexDataType) {
    EXPECT_EQ(tEverythingExtended1.getTBoolean(), tBoolean);
    EXPECT_EQ(tEverythingExtended1.getTByteBuffer(), tByteBuffer);
    EXPECT_EQ(tEverythingExtended1.getTDouble(), tDouble);
    EXPECT_EQ(tEverythingExtended1.getTEnum(), tEnum);
    EXPECT_EQ(tEverythingExtended1.getTEnumArray(), tEnumArray);
    EXPECT_EQ(tEverythingExtended1.getTEnumArray()[1], tEnumArray[1]);
    EXPECT_EQ(tEverythingExtended1.getTFloat(), tFloat);
    EXPECT_EQ(tEverythingExtended1.getTInt8(), tInt8);
    EXPECT_EQ(tEverythingExtended1.getTInt16(), tInt16);
    EXPECT_EQ(tEverythingExtended1.getTInt32(), tInt32);
    EXPECT_EQ(tEverythingExtended1.getTInt64(), tInt64);
    EXPECT_EQ(tEverythingExtended1.getTString(), tString);
    EXPECT_EQ(tEverythingExtended1.getTStringArray(), tStringArray);
    EXPECT_EQ(tEverythingExtended1.getTStringArray()[1], tStringArray[1]);
    EXPECT_EQ(tEverythingExtended1.getTUInt8(), tUInt8);
    EXPECT_EQ(tEverythingExtended1.getTUInt8Array(), tUInt8Array);
    EXPECT_EQ(tEverythingExtended1.getTUInt8Array()[1], tUInt8Array[1]);
    EXPECT_EQ(tEverythingExtended1.getTUInt16(), tUInt16);
    EXPECT_EQ(tEverythingExtended1.getTUInt32(), tUInt32);
    EXPECT_EQ(tEverythingExtended1.getTUInt64(), tUInt64);
    EXPECT_EQ(tEverythingExtended1.getTUInt64(), tUInt64);
    EXPECT_EQ(tEverythingExtended1.getTBooleanExtended(), tBooleanExtended);
    EXPECT_EQ(tEverythingExtended1.getTStringExtended(), tStringExtended);
}

TEST_F(StdComplexDataTypeTest, equalsOperatorExtended) {
    EXPECT_EQ(tEverythingExtended1, tEverythingExtended1);

    joynr::types::StdTEverythingExtendedStruct tEverythingExtended2(tEverythingExtended1);
    EXPECT_EQ(tEverythingExtended2, tEverythingExtended1);
    EXPECT_EQ(tEverythingExtended2.getTBoolean(), tBoolean);
    EXPECT_EQ(tEverythingExtended2.getTByteBuffer(), tByteBuffer);
    EXPECT_EQ(tEverythingExtended2.getTDouble(), tDouble);
    EXPECT_EQ(tEverythingExtended2.getTEnum(), tEnum);
    EXPECT_EQ(tEverythingExtended2.getTEnumArray(), tEnumArray);
    EXPECT_EQ(tEverythingExtended2.getTEnumArray()[1], tEnumArray[1]);
    EXPECT_EQ(tEverythingExtended2.getTFloat(), tFloat);
    EXPECT_EQ(tEverythingExtended2.getTInt8(), tInt8);
    EXPECT_EQ(tEverythingExtended2.getTInt16(), tInt16);
    EXPECT_EQ(tEverythingExtended2.getTInt32(), tInt32);
    EXPECT_EQ(tEverythingExtended2.getTInt64(), tInt64);
    EXPECT_EQ(tEverythingExtended2.getTString(), tString);
    EXPECT_EQ(tEverythingExtended2.getTStringArray(), tStringArray);
    EXPECT_EQ(tEverythingExtended2.getTStringArray()[1], tStringArray[1]);
    EXPECT_EQ(tEverythingExtended2.getTUInt8(), tUInt8);
    EXPECT_EQ(tEverythingExtended2.getTUInt8Array(), tUInt8Array);
    EXPECT_EQ(tEverythingExtended2.getTUInt8Array()[1], tUInt8Array[1]);
    EXPECT_EQ(tEverythingExtended2.getTUInt16(), tUInt16);
    EXPECT_EQ(tEverythingExtended2.getTUInt32(), tUInt32);
    EXPECT_EQ(tEverythingExtended2.getTUInt64(), tUInt64);
    EXPECT_EQ(tEverythingExtended2.getTBooleanExtended(), tBooleanExtended);
    EXPECT_EQ(tEverythingExtended2.getTStringExtended(), tStringExtended);

    tEverythingExtended2.setTBooleanExtended(!tEverythingExtended2.getTBooleanExtended());
    EXPECT_NE(tEverythingExtended2, tEverythingExtended1);
}

TEST_F(StdComplexDataTypeTest, assignExtendedComplexDataType) {
    StdTEverythingExtendedStruct tEverythingExtended2;
    tEverythingExtended2 = tEverythingExtended1;
    EXPECT_EQ(tEverythingExtended1, tEverythingExtended2);
}

TEST_F(StdComplexDataTypeTest, copyExtendedComplexDataType) {
    StdTEverythingExtendedStruct tEverythingExtended2(tEverythingExtended1);
    StdTEverythingExtendedStruct tEverythingExtended3 = tEverythingExtended2;
    EXPECT_EQ(tEverythingExtended1, tEverythingExtended2);
    EXPECT_EQ(tEverythingExtended2, tEverythingExtended3);
}

TEST_F(StdComplexDataTypeTest, equalsExtendedComplexDataTypeNotEqualBaseType) {
    std::string tStringExtendedExtended("extendedextended");
    StdTEverythingExtendedExtendedStruct tEverythingExtendedExtended(
            tInt8,
            tUInt8,
            tInt16,
            tUInt16,
            tInt32,
            tUInt32,
            tInt64,
            tUInt64,
            tDouble,
            tFloat,
            tString,
            tBoolean,
            tByteBuffer,
            tUInt8Array,
            tEnum,
            tEnumArray,
            tStringArray,
            word,
            wordArray,
            tBooleanExtended,
            tStringExtended,
            tStringExtendedExtended);

    EXPECT_NE(tEverything1, tEverythingExtended1);
    EXPECT_NE(tEverything1, tEverythingExtendedExtended);
    EXPECT_NE(tEverythingExtended1, tEverythingExtendedExtended);
    // currently a compile error
    //    EXPECT_NE(tEverythingExtendedExtended, tEverythingExtended1);

}
