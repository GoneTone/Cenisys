/*
 * CenisysServerLogger
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
#include <boost/locale/format.hpp>
#include <boost/locale/message.hpp>
#include "server/cenisysserverlogger.h"

namespace cenisys
{

CenisysServerLogger::CenisysServerLogger()
{
}

CenisysServerLogger::~CenisysServerLogger()
{
}

void CenisysServerLogger::log(const boost::locale::format &content)
{
    std::lock_guard<std::mutex> lock(_backendListLock);
    for(ServerLogger::LoggerBackend backend : _backends)
        std::get<ServerLogger::LogFormat>(backend)(content);
}

void CenisysServerLogger::log(const boost::locale::message &content)
{
    std::lock_guard<std::mutex> lock(_backendListLock);
    for(ServerLogger::LoggerBackend backend : _backends)
        std::get<ServerLogger::LogMessage>(backend)(content);
}

ServerLogger::RegisteredLoggerBackend
CenisysServerLogger::registerBackend(ServerLogger::LoggerBackend backend)
{
    std::lock_guard<std::mutex> lock(_backendListLock);
    _backends.push_back(backend);
    return std::prev(_backends.end());
}

void CenisysServerLogger::unregisterBackend(
    ServerLogger::RegisteredLoggerBackend handle)
{
    std::lock_guard<std::mutex> lock(_backendListLock);
    _backends.erase(handle);
}

} // namespace cenisys
