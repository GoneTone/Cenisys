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

#include <memory>
#include <mutex>
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include "server/server.h"
#include "server/cenisysserverlogger.h"
#include "server/stdinreader.h"
#include "server/stdoutlogger.h"

namespace cenisys
{

//!
//! \brief The implementation of the Cenisys core.
//!
class CenisysServer : public Server
{
public:
    //!
    //! \brief Prepare for running server. Work are done in single thread.
    //!
    CenisysServer();
    //!
    //! \brief Cleanup a stopped instance.
    //!
    ~CenisysServer();

    //!
    //! \brief Start running the server.
    //! \return 0 if terminated gracefully, 1 if crashed.
    //!
    int run();

    //!
    //! \brief Stop running the server. The run function will return then.
    //!
    void stop();

    //!
    //! \brief Process a command.
    //! \param command The command and arguments.
    //! \return true if the command is registered.
    //!
    bool dispatchCommand(std::string command);

    //!
    //! \brief Register a command.
    //! \param handler The command handler function.
    //! \return A handle to unregister the handler.
    //!
    RegisteredCommandHandler registerCommand(CommandHandler handler);

    //!
    //! \brief Unregister the command.
    //! \param handle The function handle.
    //!
    void unregisterCommand(RegisteredCommandHandler handle);

    //!
    //! \brief Get the logger of the server.
    //! \return The server's logger object.
    //!
    ServerLogger &getLogger();

private:
    void start();

    boost::asio::io_service _ioService;
    boost::asio::signal_set _termSignals;
    CommandHandlerList _commandList;
    std::mutex _registerCommandLock;
    CenisysServerLogger _logger;
    std::unique_ptr<StdinReader> _stdinReader;
    std::unique_ptr<StdoutLogger> _stdoutLogger;
};

} // namespace cenisys

#endif // CENISYS_CENISYSSERVER_H
