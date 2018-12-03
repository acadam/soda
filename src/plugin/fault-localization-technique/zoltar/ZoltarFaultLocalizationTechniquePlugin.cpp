/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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
#include "ZoltarFaultLocalizationTechniquePlugin.h"
#include "util/CTestSuiteScore.h"

namespace soda {

ZoltarFaultLocalizationTechniquePlugin::ZoltarFaultLocalizationTechniquePlugin() :
    m_distribution(new FLDistribution()),
    m_flScore(new FLScore()),
    m_data(NULL),
    clusterList(NULL),
    m_revision(0)
{
}

ZoltarFaultLocalizationTechniquePlugin::~ZoltarFaultLocalizationTechniquePlugin()
{
    delete m_distribution;
}

String ZoltarFaultLocalizationTechniquePlugin::getName()
{
    return "Zoltar";
}

String ZoltarFaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the Zoltar value for each code element.";
}

StringVector ZoltarFaultLocalizationTechniquePlugin::getDependency()
{
    return { "common" };
}

void ZoltarFaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision, IntVector failedCodeElements)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
    m_failedCodeElements = failedCodeElements;
}

ZoltarFaultLocalizationTechniquePlugin::FLDistribution& ZoltarFaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

ZoltarFaultLocalizationTechniquePlugin::FLScore& ZoltarFaultLocalizationTechniquePlugin::getFlScore()
{
    return *m_flScore;
}

void ZoltarFaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
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

            double zoltar = 0;
            IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
            IndexType nrOfPassedTestcases = passedCovered + passedNotCovered;
            if (failedCovered > 0) {
                double denominator = (double)nrOfFailedTestcases + (double)passedCovered + (10000 * (((double)failedNotCovered * (double)passedCovered) / (double)failedCovered));
                if (denominator > 0) {
                    zoltar = (double)failedCovered / denominator;
                }
            }

            ceMetrics.AddMember("zoltar", zoltar, res.GetAllocator());

            (*m_distribution)[zoltar]++;
        }

        for (IndexType i = 0; i < m_failedCodeElements.size(); i++) {
            IndexType cid = m_failedCodeElements[i];
            String ceIdStr = std::to_string(cid);
            double zoltar = res[it->first.c_str()][ceIdStr.c_str()]["zoltar"].GetDouble();

            (*m_flScore)[it->first][cid] = CTestSuiteScore::flScore(it->second, zoltar, *m_distribution);

        }

    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new ZoltarFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
