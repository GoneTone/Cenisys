/*
 * DefaultCommandHandlers
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
#ifndef CENISYS_DEFAULTCOMMANDHANDLERS_H
#define CENISYS_DEFAULTCOMMANDHANDLERS_H

#include <functional>
#include <unordered_map>
#include "server/server.h"

namespace cenisys
{

class DefaultCommandHandlers
{
public:
    DefaultCommandHandlers(Server &server);
    ~DefaultCommandHandlers();

private:
    bool handleCommand(const std::string &command);
    Server &_server;
    Server::RegisteredCommandHandler _handle;
    std::unordered_map<std::string, std::function<void(const std::string &)>>
        _commandMap;
};

} // namespace cenisys

#endif // CENISYS_DEFAULTCOMMANDHANDLERS_H
