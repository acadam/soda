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

#ifndef CCHANGESDATAMANAGER_H
#define CCHANGESDATAMANAGER_H

#include "CDataManager.h"

namespace soda {

/**
 * @brief The CChangesDataManager class contains methods for managing change set data files.
 */
class CChangesDataManager : public CDataManager
{
public:

    /**
     * @brief Creates a CChangesDataManager object with a specified handler.
     * @param handler  CDataHandler object.
     */
    CChangesDataManager(CDataHandler *handler);

    /**
     * @brief Destroys a CChangesDataManager object.
     */
    ~CChangesDataManager();

    /**
     * @brief Loads the content of a change set data from a specified file.
     * @param filepath  File path.
     * @throw Exception if filepath is not a regular file.
     */
    void load(const String &filepath);

    /**
     * @brief Dumps code element names to a specified file.
     * @param filepath  File path.
     */
    void dumpCodeElements(const String &filepath);

    /**
     * @brief Dumps code element changes for each revision to a specified file.
     * @param filepath  File path.
     */
    void dumpChanges(const String &filepath);
};

} // namespace soda

#endif /* CCHANGESDATAMANAGER_H */
