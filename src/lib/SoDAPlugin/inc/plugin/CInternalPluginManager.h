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

#ifndef CINTERNALPLUGINMANAGER_H
#define CINTERNALPLUGINMANAGER_H

#include <map>
#include <string>

#include "boost/filesystem.hpp"
#include "plugin/CPlugin.h"

namespace fs = boost::filesystem;

namespace soda {

/**
 * @brief Internal plugin manager. Loads plugins from a directory and stores their represenation.
 */
template<typename TPluginManager>
class CInternalPluginManager {
public:
    /**
     * @brief Creates a new plugin manager. does not load any plugins.
     * @param pluginDirectory The directory where the plugins are.
     */
    CInternalPluginManager(const std::string &pluginDirectory) :
        m_pluginMap(new PluginMap()),
        m_pluginDirectory(pluginDirectory)
    {
    }

    ~CInternalPluginManager()
    {
        typename PluginMap::iterator it;
        for (it = m_pluginMap->begin(); it != m_pluginMap->end(); it++) {
            delete it->second;
        }
        delete m_pluginMap;
    }

    /**
     * @brief Loads the plugins.
     * @param pluginManager The plugin manager that handles the plugins.
     */
    void loadPlugins(TPluginManager &pluginManager)
    {
        fs::path p(m_pluginDirectory);
        loadPluginsFromDirectory(p, pluginManager);
    }

private:
    /**
     * @brief Loads plugins recursively from the given path.
     * @param path The path where the plugins are.
     * @param pluginManager The plugin manager that handles the plugins.
     */
    void loadPluginsFromDirectory(const fs::path &path, TPluginManager &pluginManager)
    {
        if (fs::exists(path) && fs::is_directory(path)) {
            fs::directory_iterator endIt;
            for (fs::directory_iterator it(path); it != endIt; it++) {
                if (fs::is_directory(it->status())) {
                    loadPluginsFromDirectory(it->path(), pluginManager);
                }
                if (!fs::is_regular_file(it->status())) {
                    continue;
                }
                if (it->path().extension() != ".so") {
                    continue;
                }
                std::string pluginPath = it->path().string();
                if (m_pluginMap->find(pluginPath) == m_pluginMap->end()) {
                    CPlugin<TPluginManager> *plugin = new CPlugin<TPluginManager>(pluginPath);
                    (*m_pluginMap)[pluginPath] = plugin;
                    plugin->registerPlugin(pluginManager);
                }
            }
        }
    }

private:
    typedef std::map<std::string, CPlugin<TPluginManager>* > PluginMap;

     PluginMap *m_pluginMap;

     std::string m_pluginDirectory;
};

} /* namespace soda */

#endif /* CINTERNALPLUGINMANAGER_H */
