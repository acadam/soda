/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors:
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
#include "Wong3FaultLocalizationTechniquePlugin.h"
#include "util/CTestSuiteScore.h"

namespace soda {

Wong3FaultLocalizationTechniquePlugin::Wong3FaultLocalizationTechniquePlugin() :
    m_distribution(new FLDistribution()),
    m_flScore(new FLScore()),
    m_data(NULL),
    clusterList(NULL),
    m_revision(0)
{
}

Wong3FaultLocalizationTechniquePlugin::~Wong3FaultLocalizationTechniquePlugin()
{
    delete m_distribution;
}

String Wong3FaultLocalizationTechniquePlugin::getName()
{
    return "Wong3";
}

String Wong3FaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the Wong3 value for each code element.";
}

StringVector Wong3FaultLocalizationTechniquePlugin::getDependency()
{
    return { "common" };
}

void Wong3FaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision, IntVector failedCodeElements)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
    m_failedCodeElements = failedCodeElements;
}

Wong3FaultLocalizationTechniquePlugin::FLDistribution& Wong3FaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

Wong3FaultLocalizationTechniquePlugin::FLScore& Wong3FaultLocalizationTechniquePlugin::getFlScore()
{
    return *m_flScore;
}

void Wong3FaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
{
    CCoverageMatrix *coverageMatrix = m_data->getCoverage();

    std::map<String, CClusterDefinition>::iterator it;
    for (it = clusterList->begin(); it != clusterList->end(); it++) {

        m_distribution->clear();

        IntVector codeElementIds = it->second.getCodeElements();

        // group for cluster data
        if (!res.HasMember(it->first.c_str())) {
            res.AddMember(rapidjson::Value(it->first.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
        }

        for (IndexType i = 0; i < codeElementIds.size(); i++) {
            IndexType cid = codeElementIds[i];
            String ceIdStr = std::to_string(cid);

            // holds the metric values for one code element
            if (!res[it->first.c_str()].HasMember(ceIdStr.c_str())) {
                res[it->first.c_str()].AddMember(rapidjson::Value(ceIdStr.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
            }
            rapidjson::Value &ceMetrics = res[it->first.c_str()][ceIdStr.c_str()];

            IndexType failedCovered = ceMetrics["ef"].GetUint64();
            IndexType passedCovered = ceMetrics["ep"].GetUint64();
            IndexType failedNotCovered = ceMetrics["nf"].GetUint64();
            IndexType passedNotCovered = ceMetrics["np"].GetUint64();

            double wong3 = 0;
            IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
            IndexType nrOfPassedTestcases = passedCovered + passedNotCovered;
            if (passedCovered <= 2) {
                wong3 = (double)failedCovered - (double)passedCovered;
            } else if (passedCovered > 2 && passedCovered <= 10) {
                wong3 = (double)failedCovered - (2 + (0.1 * ((double)passedCovered - 2)));
            } else {
                wong3 = (double)failedCovered - (2.8 + (0.001 * ((double)passedCovered - 10)));
            }

            ceMetrics.AddMember("wong3", wong3, res.GetAllocator());

            (*m_distribution)[wong3]++;
        }

        for (IndexType i = 0; i < m_failedCodeElements.size(); i++) {
            IndexType cid = m_failedCodeElements[i];
            String ceIdStr = std::to_string(cid);
            double wong3 = res[it->first.c_str()][ceIdStr.c_str()]["wong3"].GetDouble();

            (*m_flScore)[it->first][cid] = CTestSuiteScore::flScore(it->second, wong3, *m_distribution);

        }

    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new Wong3FaultLocalizationTechniquePlugin());
}

} /* namespace soda */
