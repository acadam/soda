/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <lango@inf.u-szeged.hu>
 *          Tamás Gergely <gertom@inf.u-szeged.hu>
 *
 * This file is part of SoDA.
 *
 *  SoDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoDA is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtest/gtest.h"
#include "io/CSoDAio.h"
#include "data/CCoverageMatrix.h"
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"
#include "exception/CException.h"

using namespace soda;

class CCoverageMatrixTest : public testing::Test
{
protected:
    CCoverageMatrix* coverageMatrix;

    virtual void SetUp() {
        coverageMatrix = new CCoverageMatrix();
    }

    virtual void TearDown() {
        delete coverageMatrix;
    }
};

TEST(CCoverageMatrix, Constructor)
{
    CIDManager *testcases = new CIDManager();
    CIDManager *codeElements = new CIDManager();
    IBitMatrix *bitMatrix = new CBitMatrix();
    CCoverageMatrix *coverage = new CCoverageMatrix(testcases, codeElements, bitMatrix);
    coverage->addTestcaseName("testcase1");
    coverage->addCodeElementName("codeElement1");
    coverage->refitMatrixSize();
    EXPECT_TRUE(testcases->size() == coverage->getTestcases().size());
    EXPECT_TRUE(codeElements->size() == coverage->getCodeElements().size());
    EXPECT_TRUE((*bitMatrix) == coverage->getBitMatrix());
    delete coverage;
    delete testcases;
    delete codeElements;
    delete bitMatrix;
}

TEST_F(CCoverageMatrixTest, SaveAndLoad)
{
    CCoverageMatrix* coverageMatrixLoaded = new CCoverageMatrix();
    EXPECT_NO_THROW(coverageMatrix->load("sample/CoverageMatrixSampleBit"));
    EXPECT_NO_THROW(coverageMatrix->save("sample/coverageMatrix.saved"));
    EXPECT_NO_THROW(coverageMatrixLoaded->load(String("sample/coverageMatrix.saved")));
    EXPECT_EQ(coverageMatrix->getBitMatrix(), coverageMatrixLoaded->getBitMatrix());
    EXPECT_NO_THROW(coverageMatrix->save(String("sample/coverageMatrix.saved")));
    delete coverageMatrixLoaded;
}

TEST_F(CCoverageMatrixTest, BackwardCompatibility)
{
    EXPECT_NO_THROW(coverageMatrix->load("sample/CoverageMatrixSampleBit"));
    EXPECT_EQ(824u, coverageMatrix->getCodeElements().size());
    EXPECT_EQ(3u, coverageMatrix->getTestcases().size());
}

TEST_F(CCoverageMatrixTest, GetCodeElements)
{
    coverageMatrix->addCodeElementName("codeElement1");
    coverageMatrix->addCodeElementName("codeElement2");
    coverageMatrix->addTestcaseName("testName1");
    coverageMatrix->refitMatrixSize();
    coverageMatrix->setRelation("testName1", "codeElement1", true);
    coverageMatrix->setRelation("testName1", "codeElement2", true);
    EXPECT_EQ(2u, coverageMatrix->getCodeElements("testName1").size());
    EXPECT_EQ(0u, coverageMatrix->getCodeElements("unknown test").size());
}

TEST_F(CCoverageMatrixTest, GetTestcases)
{
    coverageMatrix->addTestcaseName("testName1");
    coverageMatrix->addTestcaseName("testName2");
    coverageMatrix->addCodeElementName("codeElement1");
    coverageMatrix->refitMatrixSize();
    coverageMatrix->setRelation("testName1", "codeElement1", true);
    coverageMatrix->setRelation(coverageMatrix->getTestcases().getID("testName2"), coverageMatrix->getCodeElements().getID("codeElement1"), true);
    EXPECT_EQ(2u, coverageMatrix->getTestcases("codeElement1").size());
    EXPECT_EQ(0u, coverageMatrix->getTestcases("unknown code element").size());
}

TEST_F(CCoverageMatrixTest, GetRelation)
{
    coverageMatrix->addCodeElementName("codeElement1");
    coverageMatrix->addTestcaseName("testName1");
    coverageMatrix->refitMatrixSize();
    coverageMatrix->setRelation("testName1", "codeElement1", true);

    EXPECT_TRUE(coverageMatrix->getRelation("testName1", "codeElement1"));
    EXPECT_THROW(coverageMatrix->getRelation("unknown test", "codeElement1"), CException);
    EXPECT_THROW(coverageMatrix->getRelation("testName1", "unknown code element"), CException);
}

TEST_F(CCoverageMatrixTest, SetRelation)
{
    coverageMatrix->addCodeElementName("codeElement1");
    coverageMatrix->addTestcaseName("testName1");
    coverageMatrix->refitMatrixSize();
    EXPECT_NO_THROW(coverageMatrix->setRelation("testName1", "codeElement1", true));
    EXPECT_TRUE(coverageMatrix->getRelation("testName1", "codeElement1"));
    EXPECT_THROW(coverageMatrix->setRelation("unknown test", "codeElement1", true), CException);
    EXPECT_THROW(coverageMatrix->setRelation("testName1", "unknown code element", true), CException);
}

TEST_F(CCoverageMatrixTest, AddOrSetRelation)
{
    coverageMatrix->addOrSetRelation("testName1", "codeElement1", true);
    EXPECT_TRUE(coverageMatrix->getRelation("testName1", "codeElement1"));
    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(1u, coverageMatrix->getNumOfCodeElements());
    coverageMatrix->addOrSetRelation("testName1", "codeElement1", false);
    EXPECT_FALSE(coverageMatrix->getRelation("testName1", "codeElement1"));
    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(1u, coverageMatrix->getNumOfCodeElements());
}
