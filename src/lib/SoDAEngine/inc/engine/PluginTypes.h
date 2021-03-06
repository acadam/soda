/*
 * Copyright (C): 2013-2015 Department of Software Engineering, University of Szeged
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

#ifndef PLUGINTYPES_H
#define PLUGINTYPES_H

#include "engine/CPluginManager.h"

#include "engine/plugin/IBugsetReaderPlugin.h"
#include "engine/plugin/IChangesetReaderPlugin.h"
#include "engine/plugin/ICoverageReaderPlugin.h"
#include "engine/plugin/IResultsReaderPlugin.h"
#include "engine/plugin/ITestSuiteClusterPlugin.h"
#include "engine/plugin/ITestSuiteMetricPlugin.h"
#include "engine/plugin/ITestSuitePrioritizationPlugin.h"
#include "engine/plugin/ITestSuiteReductionPlugin.h"
#include "engine/plugin/IFaultLocalizationTechniquePlugin.h"
#include "engine/plugin/IMutationMetricPlugin.h"

#ifdef WIN32
#define MSDLL_EXPORT __declspec(dllexport)
#else
#define MSDLL_EXPORT
#endif

namespace soda {

typedef CPluginManager<IBugsetReaderPlugin>               BugsetReaderPluginManager;
typedef CPluginManager<IChangesetReaderPlugin>            ChangesetReaderPluginManager;
typedef CPluginManager<ICoverageReaderPlugin>             CoverageReaderPluginManager;
typedef CPluginManager<IResultsReaderPlugin>              ResultsReaderPluginManager;
typedef CPluginManager<ITestSuiteClusterPlugin>           TestSuiteClusterPluginManager;
typedef CPluginManager<ITestSuiteMetricPlugin>            TestSuiteMetricPluginManager;
typedef CPluginManager<ITestSuitePrioritizationPlugin>    TestSuitePrioritizationPluginManager;
typedef CPluginManager<ITestSuiteReductionPlugin>         TestSuiteReductionPluginManager;
typedef CPluginManager<IFaultLocalizationTechniquePlugin> FaultLocalizationTechniquePluginManager;
using MutationMetricPluginManager = CPluginManager<IMutationMetricPlugin>;

} /* namespace soda */

#endif /* PLUGINTYPES_H */
