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

#include <stdexcept>
#include "defaultcommandhandlers.h"

namespace cenisys
{

DefaultCommandHandlers::DefaultCommandHandlers(Server &server)
    : _server(server), _commandMap({{"stop", [this](const std::string &)
                                     {
                                         _server.terminate();
                                     }}})
{
    _handle = _server.registerCommand(std::bind(
        &DefaultCommandHandlers::handleCommand, this, std::placeholders::_1));
}

DefaultCommandHandlers::~DefaultCommandHandlers()
{
    _server.unregisterCommand(_handle);
}

bool DefaultCommandHandlers::handleCommand(const std::string &command)
{
    try
    {
        _commandMap.at(command.substr(0, command.find(' ')))(command);
        return true;
    }
    catch(std::out_of_range)
    {
        return false;
    }
}

} // namespace cenisys
