/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#include "CDataHandler.h"
#include "CCoverageDataManager.h"
#include "CResultsDataManager.h"
#include "CChangesDataManager.h"

namespace sodatools {

CDataHandler::CDataHandler() :
    m_bPrintInfo(true), m_bWithPassFail(true), m_bWithNames(false),
    m_eReadFormat(rfUnknown), m_pChanges(NULL), m_pCoverage(NULL),
    m_pResults(NULL), m_pSelection(NULL), m_pTestcases(NULL),
    m_pCodeElements(NULL)
{}

CDataHandler::~CDataHandler()
{
    delete m_pChanges;
    delete m_pCoverage;
    delete m_pCodeElements;
    delete m_pResults;
    delete m_pSelection;
    delete m_pTestcases;
}

CCoverageDataManager CDataHandler::getCoverageDataMgr()
{
    return CCoverageDataManager(this);
}

CChangesDataManager CDataHandler::getChangesDataMgr()
{
    return CChangesDataManager(this);
}

CResultsDataManager CDataHandler::getResultsDataMgr()
{
    return CResultsDataManager(this);
}

bool CDataHandler::createSelection()
{
    return (m_pSelection = new CSelectionData());
}

CChangeset* CDataHandler::getChanges(bool create)
{
    if (create && !m_pChanges)
        m_pChanges = new CChangeset(GET(m_pTestcases));

    return m_pChanges;
}

CCoverageMatrix* CDataHandler::getCoverage(bool create)
{
    if (create && !m_pCoverage)
        m_pCoverage = new CCoverageMatrix(GET(m_pTestcases), GET(m_pCodeElements));

    return m_pCoverage;
}

void CDataHandler::setCoverage(CCoverageMatrix *coverage)
{
    m_pCoverage = coverage;
}

CResultsMatrix* CDataHandler::getResults(bool create)
{
    if(create && !m_pResults)
        m_pResults = new CResultsMatrix(GET(m_pTestcases));

    return m_pResults;
}

void CDataHandler::setResults(CResultsMatrix *results)
{
    m_pResults = results;
}

CSelectionData* CDataHandler::getSelection()
{
    return m_pSelection;
}

CIDManager* CDataHandler::getTestcases()
{
    return m_pTestcases;
}

CIDManager* CDataHandler::getCodeElements()
{
    return m_pCodeElements;
}

bool CDataHandler::getPrintInfo() const
{
    return m_bPrintInfo;
}

void CDataHandler::setPrintInfo(bool set)
{
    m_bPrintInfo = set;
}

bool CDataHandler::getWithPassFail() const
{
    return m_bWithPassFail;
}

void CDataHandler::setWithPassFail(bool set)
{
    m_bWithPassFail = set;
}

bool CDataHandler::getWithNames() const
{
    return m_bWithNames;
}

void CDataHandler::setWithNames(bool set)
{
    m_bWithNames = set;
}

ReadFormat CDataHandler::getReadFormat() const
{
    return m_eReadFormat;
}

void CDataHandler::setReadFormat(ReadFormat format)
{
    m_eReadFormat = format;
}

} // namespace sodatools