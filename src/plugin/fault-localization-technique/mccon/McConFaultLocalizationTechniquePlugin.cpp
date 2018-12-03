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
#include "McConFaultLocalizationTechniquePlugin.h"
#include "util/CTestSuiteScore.h"

namespace soda {

McConFaultLocalizationTechniquePlugin::McConFaultLocalizationTechniquePlugin() :
    m_distribution(new FLDistribution()),
    m_flScore(new FLScore()),
    m_data(NULL),
    clusterList(NULL),
    m_revision(0)
{
}

McConFaultLocalizationTechniquePlugin::~McConFaultLocalizationTechniquePlugin()
{
    delete m_distribution;
}

String McConFaultLocalizationTechniquePlugin::getName()
{
    return "McCon";
}

String McConFaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the McCon value for each code element.";
}

StringVector McConFaultLocalizationTechniquePlugin::getDependency()
{
    return { "common" };
}

void McConFaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision, IntVector failedCodeElements)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
    m_failedCodeElements = failedCodeElements;
}

McConFaultLocalizationTechniquePlugin::FLDistribution& McConFaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

McConFaultLocalizationTechniquePlugin::FLScore& McConFaultLocalizationTechniquePlugin::getFlScore()
{
    return *m_flScore;
}

void McConFaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
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

            double mccon = 0;
            IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
            double denominator = (double)nrOfFailedTestcases * ((double)failedCovered + (double)passedCovered);
            if (denominator > 0) {
                mccon = (((double)failedCovered * (double)failedCovered) - ((double)failedNotCovered * (double)passedCovered)) / denominator;
            }

            ceMetrics.AddMember("mccon", mccon, res.GetAllocator());

            (*m_distribution)[mccon]++;
        }

        for (IndexType i = 0; i < m_failedCodeElements.size(); i++) {
            IndexType cid = m_failedCodeElements[i];
            String ceIdStr = std::to_string(cid);
            double mccon = res[it->first.c_str()][ceIdStr.c_str()]["mccon"].GetDouble();

            (*m_flScore)[it->first][cid] = CTestSuiteScore::flScore(it->second, mccon, *m_distribution);

        }

    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new McConFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
