/*
 * %{Cpp:License:ClassName}
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
#ifndef CENISYS_SERVERLOGGER_H
#define CENISYS_SERVERLOGGER_H

#include <functional>
#include <list>
#include <string>

namespace cenisys
{

class ServerLogger
{
public:
    using LoggerBackend = std::function<void(std::string)>;
    using BackendList = std::list<LoggerBackend>;
    using RegisteredLoggerBackend = BackendList::iterator;

    virtual ~ServerLogger() {}

    //!
    //! \brief Log some text to the server log.
    //! \param content The content in string. May contain color codes.
    //!
    virtual void log(const std::string &content) = 0;

    //!
    //! \brief Register a logger backend.
    //! \param backend The function to call to log. Must not block.
    //! \return A handle to the registered backend.
    //!
    virtual RegisteredLoggerBackend registerBackend(LoggerBackend backend) = 0;

    //!
    //! \brief Unregister the logger backend.
    //! \param backend The handle returned by registerBackend.
    //!
    virtual void unregisterBackend(RegisteredLoggerBackend handle) = 0;
};
}

#endif // CENISYS_SERVERLOGGER_H
