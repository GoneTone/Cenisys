/*
 * CenisysConfigManager
 * Copyright (C) 2016 iTX Technologies
 *
 * This file is part of Cenisys.
 *
 * Cenisys is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cenisys is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cenisys.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/locale/format.hpp>
#include <boost/locale/message.hpp>
#include <yaml-cpp/parser.h>
#include "config/cenisysconfigsection.h"
#include "cenisysconfigmanager.h"

namespace cenisys
{

CenisysConfigManager::CenisysConfigManager(
    Server &server, const boost::filesystem::path &basepath)
    : _server(server), _basepath(basepath)
{
}

CenisysConfigManager::~CenisysConfigManager()
{
}

std::shared_ptr<ConfigSection>
CenisysConfigManager::getConfig(const std::string &name)
{
    boost::filesystem::path target =
        (_basepath / name).replace_extension("yml");
    std::shared_ptr<ConfigSection> result = _loadedConfig[name].lock();
    if(!result)
    {
        result = std::make_shared<CenisysConfigSection>(_server, target);
        _loadedConfig[name] = result;
    }
    return result;
}

} // namespace cenisys
