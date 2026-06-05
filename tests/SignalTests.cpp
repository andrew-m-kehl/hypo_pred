#include "gtest/gtest.h"

#include "Stle/PCSignal.hpp"
#include "Stle/CsvPCSignalReader.hpp"
#include "Stle/PCSeriesCsvPrinter.hpp"
#include "Stle/ArrayPCSignalReader.hpp"

#include "Stle/U/StrReader.hpp"
#include "Stle/U/StrPrinter.hpp"
#include "Stle/U/CsvReader.hpp"
#include "Stle/U/FileReader.hpp"
#include "Stle/U/Array2DRef.hpp"
#include "Stle/U/Array2DTRef.hpp"

#include <array>

using namespace Stle;

TEST(SignalTests, TestValid) {
    std::vector<PCSeries> signal;
    ASSERT_FALSE(PCSignal::valid(signal));
    
    signal.emplace_back();
    ASSERT_FALSE(PCSignal::valid(signal));
    
    PCSeries &series0 = signal[0];
    series0.add(1, 1);
    series0.add(1, 1);
    
    ASSERT_TRUE(PCSignal::valid(signal));
    
    signal.emplace_back();
    ASSERT_FALSE(PCSignal::valid(signal));
    
    PCSeries &series1 = signal[1];
    series1.add(1, 1);
    ASSERT_FALSE(PCSignal::valid(signal));
    series1.add(1, 1);
    ASSERT_TRUE(PCSignal::valid(signal));
}

TEST(SignalTests, TestReadCsvStringTimeFirst) {
    const char *csv0123 = "0,0\n1,1\n2,2\n3,3";
    const char *csv45 = "1,1\n2,2\n3,3";
    
    {
        PCSeries expectedS0;
        expectedS0.add(1, 0);
        expectedS0.add(2, 1);
        expectedS0.add(3, 2);
        expectedS0.add(3, 3);
        PCSignal expected0(std::move(expectedS0));
        
        U::CsvReader<U::StrReader> reader0(U::OF_READER, csv0123);
        PCSignal result0 = makeCsvPCSignalReader(reader0).read();
        ASSERT_EQ(expected0, result0);
    }    
    {
        PCSeries expectedS1;
        expectedS1.add(1, 0);
        expectedS1.add(1, 0);
        expectedS1.add(2, 1);
        expectedS1.add(2, 1);
        expectedS1.add(3, 2);
        expectedS1.add(3, 2);    
        PCSignal expected1(std::move(expectedS1));
        
        U::CsvReader<U::StrReader> reader1(U::OF_READER, csv0123);
        auto signalReader1 = makeCsvPCSignalReader(reader1);
        signalReader1.flags = READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result1 = signalReader1.read();
        ASSERT_EQ(expected1, result1);
    }
    {
        PCSeries expectedS2;
        expectedS2.add(0, 0);
        expectedS2.add(1, 1);
        expectedS2.add(2, 2);
        expectedS2.add(3, 3);
        expectedS2.add(3, 3);
        PCSignal expected2(std::move(expectedS2));
        
        U::CsvReader<U::StrReader> reader2(U::OF_READER, csv0123);
        auto signalReader2 = makeCsvPCSignalReader(reader2);
        signalReader2.flags = READ_SIGNAL_END_POINTS;
        PCSignal result2 = signalReader2.read();
        ASSERT_EQ(expected2, result2);
    }
    {
        PCSeries expectedS3;
        expectedS3.add(0, 0);
        expectedS3.add(1, 1);
        expectedS3.add(1, 1);
        expectedS3.add(2, 2);
        expectedS3.add(2, 2);
        expectedS3.add(3, 3);
        expectedS3.add(3, 3);
        PCSignal expected3(std::move(expectedS3));
        
        U::CsvReader<U::StrReader> reader3(U::OF_READER, csv0123);
        auto signalReader3 = makeCsvPCSignalReader(reader3);
        signalReader3.flags = READ_SIGNAL_END_POINTS | READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result3 = signalReader3.read();
        ASSERT_EQ(expected3, result3);
    }
    {
        PCSeries expectedS4;
        expectedS4.add(1, 1);
        expectedS4.add(2, 2);
        expectedS4.add(3, 3);
        expectedS4.add(3, 3);
        PCSignal expected4(std::move(expectedS4));
        
        U::CsvReader<U::StrReader> reader4(U::OF_READER, csv45);
        auto signalReader4 = makeCsvPCSignalReader(reader4);
        signalReader4.flags = READ_SIGNAL_END_POINTS;
        PCSignal result4 = signalReader4.read();
        ASSERT_EQ(expected4, result4);
    }
    {
        PCSeries expectedS5;
        expectedS5.add(1, 1);
        expectedS5.add(1, 1);
        expectedS5.add(2, 2);
        expectedS5.add(2, 2);
        expectedS5.add(3, 3);
        expectedS5.add(3, 3);
        PCSignal expected5(std::move(expectedS5));
        
        U::CsvReader<U::StrReader> reader5(U::OF_READER, csv45);
        auto signalReader5 = makeCsvPCSignalReader(reader5);
        signalReader5.flags = READ_SIGNAL_END_POINTS | READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result5 = signalReader5.read();
        ASSERT_EQ(expected5, result5);
    }
}

TEST(SignalTests, TestReadCsvFileTimeFirst) {
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(2, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);
        
        expectedV[1].add(1, 3);
        expectedV[1].add(3, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));
        
        U::CsvReader<U::FileReader<U::File>> reader(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeFirst.csv", "r");
        PCSignal result = makeCsvPCSignalReader(reader).read();
        ASSERT_EQ(expected, result);
    }
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(2, 1);
        expectedV[0].add(2, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);
        
        expectedV[1].add(1, 3);
        expectedV[1].add(1, 3);
        expectedV[1].add(3, 4);
        expectedV[1].add(3, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));
        
        U::CsvReader<U::FileReader<U::File>> reader(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeFirst.csv", "r");
        auto signalReader = makeCsvPCSignalReader(reader);
        signalReader.flags = READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result = signalReader.read();
        ASSERT_EQ(expected, result);
    }
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(1, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);        
        
        expectedV[1].add(0, 3);
        expectedV[1].add(2, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));
        
        U::CsvReader<U::FileReader<U::File>> reader(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeFirst.csv", "r");
        PCSignal result = readPCSignalCsv(reader, READ_SIGNAL_END_POINTS);
        ASSERT_EQ(expected, result);
    }
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(1, 1);
        expectedV[0].add(1, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);
        
        expectedV[1].add(0, 3);        
        expectedV[1].add(2, 4);
        expectedV[1].add(2, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));
        
        U::CsvReader<U::FileReader<U::File>> reader(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeFirst.csv", "r");
        PCSignal result = readPCSignalCsv(reader, READ_SIGNAL_END_POINTS | READ_SIGNAL_RIGHT_CLOSED);
        ASSERT_EQ(expected, result);
    }
}

TEST(SignalTests, TestReadCsvFileTimeLast) {
    {
        std::vector<PCSeries> expectedV6(2);
        
        expectedV6[0].add(2, 1);
        expectedV6[0].add(4, 2);
        expectedV6[0].add(4, 2);
        
        expectedV6[1].add(1, 3);
        expectedV6[1].add(3, 4);
        expectedV6[1].add(4, 5);
        expectedV6[1].add(4, 5);
        
        PCSignal expected6(std::move(expectedV6));
        
        U::CsvReader<U::FileReader<U::File>> reader6(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeLast.csv", "r");
        PCSignal result6 = readPCSignalCsv(reader6, READ_SIGNAL_TIME_LAST);
        ASSERT_EQ(expected6, result6);
    }
    {
        std::vector<PCSeries> expectedV7(2);
        
        expectedV7[0].add(2, 1);
        expectedV7[0].add(2, 1);
        expectedV7[0].add(4, 2);
        expectedV7[0].add(4, 2);
        
        expectedV7[1].add(1, 3);
        expectedV7[1].add(1, 3);
        expectedV7[1].add(3, 4);
        expectedV7[1].add(3, 4);
        expectedV7[1].add(4, 5);
        expectedV7[1].add(4, 5);
        
        PCSignal expected7(std::move(expectedV7));
        
        U::CsvReader<U::FileReader<U::File>> reader7(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeLast.csv", "r");
        auto signalReader7 = makeCsvPCSignalReader(reader7);
        signalReader7.flags = READ_SIGNAL_TIME_LAST | READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result7 = signalReader7.read();
        ASSERT_EQ(expected7, result7);
    }
    {
        std::vector<PCSeries> expectedV8(2);
        
        expectedV8[0].add(1, 1);
        expectedV8[0].add(4, 2);
        expectedV8[0].add(4, 2);        
        
        expectedV8[1].add(0, 3);
        expectedV8[1].add(2, 4);
        expectedV8[1].add(4, 5);
        expectedV8[1].add(4, 5);
        
        PCSignal expected8(std::move(expectedV8));
        
        U::CsvReader<U::FileReader<U::File>> reader8(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeLast.csv", "r");
        PCSignal result8 = readPCSignalCsv(reader8, READ_SIGNAL_TIME_LAST | READ_SIGNAL_END_POINTS);
        ASSERT_EQ(expected8, result8);
    }
    {
        std::vector<PCSeries> expectedV9(2);
        
        expectedV9[0].add(1, 1);
        expectedV9[0].add(1, 1);
        expectedV9[0].add(4, 2);
        expectedV9[0].add(4, 2);
        
        expectedV9[1].add(0, 3);        
        expectedV9[1].add(2, 4);
        expectedV9[1].add(2, 4);
        expectedV9[1].add(4, 5);
        expectedV9[1].add(4, 5);
        
        PCSignal expected9(std::move(expectedV9));
        
        U::CsvReader<U::FileReader<U::File>> reader9(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeLast.csv", "r");
        PCSignal result9 = readPCSignalCsv(reader9, READ_SIGNAL_TIME_LAST | READ_SIGNAL_END_POINTS | READ_SIGNAL_RIGHT_CLOSED);
        ASSERT_EQ(expected9, result9);
    }
}

TEST(SignalTests, TestWriteReadCsv) {
    {
        U::CsvReader<U::FileReader<U::File>> origReader(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvTimeFirst.csv", "r");
        PCSignal orig = readPCSignalCsv(origReader);
        U::StrPrinter cp;
        cp << PCSeriesCsvPrinter(orig[0]);
        U::CsvReader<U::StrReader> copyReader(U::OF_READER, cp.str());
        PCSignal copy = readPCSignalCsv(copyReader, READ_SIGNAL_END_POINTS | READ_SIGNAL_RIGHT_OPEN);
        ASSERT_EQ(orig[0], copy[0]);
    }
}

TEST(SignalTests, TestAdd) {
    PCSeries series0;
    series0.add(1, 1);
    series0.add(1, 1);
    series0.add(4, 2);
    series0.add(4, 2);
    
    PCSeries series1;
    series1.add(0, 3);        
    series1.add(2, 4);
    series1.add(2, 4);
    series1.add(4, 5);
    series1.add(4, 5);
    
    PCSignal result(std::move(series0));
    result.add(PCSignal(std::move(series1)));
    
    std::vector<PCSeries> expectedV(2);
    expectedV[0].add(1, 1);
    expectedV[0].add(1, 1);
    expectedV[0].add(4, 2);
    expectedV[0].add(4, 2);    
    expectedV[1].add(0, 3);        
    expectedV[1].add(2, 4);
    expectedV[1].add(2, 4);
    expectedV[1].add(4, 5);
    expectedV[1].add(4, 5);
    PCSignal expected(std::move(expectedV));
    
    ASSERT_EQ(expected, result);
}

TEST(SignalTests, TestReadCsvNewline) {
    U::CsvReader<U::FileReader<U::File>> reader(U::OF_READER, U::OF_FILE, "tests/data/SignalTests/TestReadCsvNewline.csv", "r");
    PCSignal signal = readPCSignalCsv(reader);
}

TEST(SignalTests, TestReadArrayTimeFirst) {    
    // Data by row.
    double data01[] = { 0, 0,  1, 1,  2, 2,  3, 3 };    
    // Same data but by column.
    double data23[] = {0, 1, 2, 3,  0, 1, 2, 3};
    
    double data45_0[] = {1, 1};
    double data45_1[] = {2, 2};
    double data45_2[] = {3, 3};
    double *data45[] = {data45_0, data45_1, data45_2};
    
    {
        PCSeries expectedS;
        expectedS.add(1, 0);
        expectedS.add(2, 1);
        expectedS.add(3, 2);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));
        
        PCSignal result = makeArrayPCSignalReader(U::makeArray2DRef(data01, 2), 4, 2).read();
        ASSERT_EQ(expected, result);
    }    
    {
        PCSeries expectedS;
        expectedS.add(1, 0);
        expectedS.add(1, 0);
        expectedS.add(2, 1);
        expectedS.add(2, 1);
        expectedS.add(3, 2);
        expectedS.add(3, 2);    
        PCSignal expected(std::move(expectedS));
        
        auto signalReader = makeArrayPCSignalReader(U::makeArray2DRef(data01, 2), 4, 2);
        signalReader.flags = READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result = signalReader.read();
        ASSERT_EQ(expected, result);
    }
    {
        PCSeries expectedS;
        expectedS.add(0, 0);
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));
        
        auto signalReader = makeArrayPCSignalReader(U::makeArray2DTRef(data23, 4), 4, 2);
        signalReader.flags = READ_SIGNAL_END_POINTS;
        PCSignal result = signalReader.read();
        ASSERT_EQ(expected, result);
    }
    {
        PCSeries expectedS;
        expectedS.add(0, 0);
        expectedS.add(1, 1);
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));
        
        auto signalReader = makeArrayPCSignalReader(U::makeArray2DTRef(data23, 4), 4, 2);
        signalReader.flags = READ_SIGNAL_END_POINTS | READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result = signalReader.read();
        ASSERT_EQ(expected, result);
    }
    {
        PCSeries expectedS;
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));
        
        auto signalReader = makeArrayPCSignalReader(data45, 3, 2);
        signalReader.flags = READ_SIGNAL_END_POINTS;
        PCSignal result = signalReader.read();
        ASSERT_EQ(expected, result);
    }
    {
        PCSeries expectedS;
        expectedS.add(1, 1);
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));        
        
        auto signalReader = makeArrayPCSignalReader(data45, 3, 2);
        signalReader.flags = READ_SIGNAL_END_POINTS | READ_SIGNAL_RIGHT_CLOSED;
        PCSignal result = signalReader.read();
        ASSERT_EQ(expected, result);
    }
}
