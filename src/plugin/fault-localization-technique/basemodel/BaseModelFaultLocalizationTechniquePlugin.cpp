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
#include "BaseModelFaultLocalizationTechniquePlugin.h"
#include "util/CTestSuiteScore.h"

namespace soda {

BasemodelFaultLocalizationTechniquePlugin::BasemodelFaultLocalizationTechniquePlugin() :
    m_distribution(new FLDistribution()),
    m_flScore(new FLScore()),
    m_data(NULL),
    clusterList(NULL),
    m_revision(0)
{
}

BasemodelFaultLocalizationTechniquePlugin::~BasemodelFaultLocalizationTechniquePlugin()
{
    delete m_distribution;
}

String BasemodelFaultLocalizationTechniquePlugin::getName()
{
    return "Basemodel";
}

String BasemodelFaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the Basemodel value for each code element.";
}

StringVector BasemodelFaultLocalizationTechniquePlugin::getDependency()
{
    return { "common" };
}

void BasemodelFaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision, IntVector failedCodeElements)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
    m_failedCodeElements = failedCodeElements;
}

BasemodelFaultLocalizationTechniquePlugin::FLDistribution& BasemodelFaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

BasemodelFaultLocalizationTechniquePlugin::FLScore& BasemodelFaultLocalizationTechniquePlugin::getFlScore()
{
    return *m_flScore;
}

void BasemodelFaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
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

            double basemodel = 0;
            IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
            IndexType nrOfPassedTestcases = passedCovered + passedNotCovered;
            //double edgeWeight = number of neighbors of a failed test
            double multiplier = ((double)failedCovered + (double)passedCovered);
            if (multiplier > 0) {
                //double bm = (edgeWeight / multiplier);
                //double failedEdgeMultiplier = ((double)failedCovered / number of neighbors of all failed test)
                //basemodel = bm * failedEdgeMultiplier
            }

            ceMetrics.AddMember("basemodel", basemodel, res.GetAllocator());

            (*m_distribution)[basemodel]++;
        }

        for (IndexType i = 0; i < m_failedCodeElements.size(); i++) {
            IndexType cid = m_failedCodeElements[i];
            String ceIdStr = std::to_string(cid);
            double basemodel = res[it->first.c_str()][ceIdStr.c_str()]["basemodel"].GetDouble();

            (*m_flScore)[it->first][cid] = CTestSuiteScore::flScore(it->second, basemodel, *m_distribution);

        }

    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new BasemodelFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
