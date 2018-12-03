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
#include "Op2FaultLocalizationTechniquePlugin.h"
#include "util/CTestSuiteScore.h"

namespace soda {

Op2FaultLocalizationTechniquePlugin::Op2FaultLocalizationTechniquePlugin() :
    m_distribution(new FLDistribution()),
    m_flScore(new FLScore()),
    m_data(NULL),
    clusterList(NULL),
    m_revision(0)
{
}

Op2FaultLocalizationTechniquePlugin::~Op2FaultLocalizationTechniquePlugin()
{
    delete m_distribution;
}

String Op2FaultLocalizationTechniquePlugin::getName()
{
    return "Op2";
}

String Op2FaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the Op2 value for each code element.";
}

StringVector Op2FaultLocalizationTechniquePlugin::getDependency()
{
    return { "common" };
}

void Op2FaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision, IntVector failedCodeElements)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
    m_failedCodeElements = failedCodeElements;
}

Op2FaultLocalizationTechniquePlugin::FLDistribution& Op2FaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

Op2FaultLocalizationTechniquePlugin::FLScore& Op2FaultLocalizationTechniquePlugin::getFlScore()
{
    return *m_flScore;
}

void Op2FaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
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
            IndexType passedNotCovered = ceMetrics["np"].GetUint64();

            double Op2 = 0;
            IndexType nrOfPassedTestcases = passedCovered + passedNotCovered;
            if (nrOfPassedTestcases != -1) {
                double denominator = ((double)nrOfPassedTestcases + 1);
                if (denominator > 0) {
                    Op2 = (double)failedCovered - ((double)passedCovered / denominator);
                }
            }

            ceMetrics.AddMember("Op2", Op2, res.GetAllocator());

            (*m_distribution)[Op2]++;
        }

        for (IndexType i = 0; i < m_failedCodeElements.size(); i++) {
            IndexType cid = m_failedCodeElements[i];
            String ceIdStr = std::to_string(cid);
            double Op2 = res[it->first.c_str()][ceIdStr.c_str()]["Op2"].GetDouble();

            (*m_flScore)[it->first][cid] = CTestSuiteScore::flScore(it->second, Op2, *m_distribution);

        }

    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new Op2FaultLocalizationTechniquePlugin());
}

} /* namespace soda */
