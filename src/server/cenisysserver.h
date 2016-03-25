/*
 * Implementation of the main server.
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
#ifndef CENISYS_CENISYSSERVER_H
#define CENISYS_CENISYSSERVER_H

#include <locale>
#include <memory>
#include <mutex>
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/locale/generator.hpp>
#include "server/server.h"
#include "server/cenisysserverlogger.h"
#include "server/stdinreader.h"
#include "server/stdoutlogger.h"
#include "command/defaultcommandhandlers.h"

namespace cenisys
{

class CenisysServer : public Server
{
public:
    CenisysServer();
    ~CenisysServer();

    int run();
    void terminate();

    std::locale getLocale(std::string locale);

    bool dispatchCommand(std::string command);

    RegisteredCommandHandler registerCommand(CommandHandler handler);
    void unregisterCommand(RegisteredCommandHandler handle);

    ServerLogger &getLogger();

private:
    void start();
    void stop();

    std::once_flag _stopFlag;
    std::locale _oldCoutLoc;
    boost::locale::generator _localeGen;
    boost::asio::io_service _ioService;
    boost::asio::signal_set _termSignals;
    CommandHandlerList _commandList;
    std::mutex _registerCommandLock;
    CenisysServerLogger _logger;
    std::unique_ptr<StdinReader> _stdinReader;
    std::unique_ptr<StdoutLogger> _stdoutLogger;
    std::unique_ptr<DefaultCommandHandlers> _defaultCommands;
};

} // namespace cenisys

#endif // CENISYS_CENISYSSERVER_H
