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
#include <functional>
#include <mutex>
#include <iostream>
#include <locale>
#include <boost/locale/format.hpp>
#include <boost/locale/generator.hpp>
#include <boost/locale/message.hpp>
#include "config.h"
#include "config/configsection.h"
#include "server/cenisysserver.h"

namespace cenisys
{

CenisysServer::CenisysServer(const boost::filesystem::path &dataDir,
                             boost::locale::generator &localeGen)
    : _dataDir(dataDir), _localeGen(localeGen),
      _termSignals(_ioService, SIGINT, SIGTERM),
      _configManager(*this, _dataDir / "config")
{
    _oldCoutLoc = std::cout.imbue(std::locale());
    std::cout << boost::locale::format(
                     boost::locale::translate("Starting Cenisys {1}")) %
                     SERVER_VERSION
              << std::endl;
}

CenisysServer::~CenisysServer()
{
    std::cout << boost::locale::translate("Cenisys successfully stopped")
              << std::endl;
    std::cout.imbue(_oldCoutLoc);
}

int CenisysServer::run()
{
    _config = _configManager.getConfig("cenisys");
    start();
    unsigned int threads =
        _config->getUInt(ConfigSection::Path() / "threads", 0);
    if(threads == 0)
        threads = std::thread::hardware_concurrency();
    if(threads == 0)
        threads = 1;
    for(unsigned int i = 1; i < threads; i++)
    {
        _threads.emplace_back(
            static_cast<std::size_t (boost::asio::io_service::*)()>(
                &boost::asio::io_service::run),
            &_ioService);
    }
    _ioService.run();
    for(std::thread &t : _threads)
    {
        t.join();
    }
    _config.reset();
    return 0;
}

void CenisysServer::terminate()
{
    std::call_once(_stopFlag, std::bind(&CenisysServer::stop, this));
}

std::locale CenisysServer::getLocale(std::string locale)
{
    return _localeGen(locale);
}

bool CenisysServer::dispatchCommand(std::string command)
{
    for(Server::CommandHandler &handler : _commandList)
        if(handler(command))
            return true;
    // TODO: command sender
    log(boost::locale::format(boost::locale::translate("Unknown command {1}")) %
        command);
    return false;
}

Server::RegisteredCommandHandler
CenisysServer::registerCommand(Server::CommandHandler handler)
{
    std::lock_guard<std::mutex> lock(_registerCommandLock);
    _commandList.push_front(handler);
    return _commandList.before_begin();
}

void CenisysServer::unregisterCommand(Server::RegisteredCommandHandler handle)
{
    std::lock_guard<std::mutex> lock(_registerCommandLock);
    _commandList.erase_after(handle);
}

void CenisysServer::log(const boost::locale::format &content)
{
    std::lock_guard<std::mutex> lock(_loggerBackendListLock);
    for(Server::LoggerBackend backend : _loggerBackends)
        std::get<Server::LogFormat>(backend)(content);
}

void CenisysServer::log(const boost::locale::message &content)
{
    std::lock_guard<std::mutex> lock(_loggerBackendListLock);
    for(Server::LoggerBackend backend : _loggerBackends)
        std::get<Server::LogFormat>(backend)(content);
}

Server::RegisteredLoggerBackend
CenisysServer::registerBackend(Server::LoggerBackend backend)
{
    std::lock_guard<std::mutex> lock(_loggerBackendListLock);
    _loggerBackends.push_front(backend);
    return _loggerBackends.before_begin();
}

void CenisysServer::unregisterBackend(Server::RegisteredLoggerBackend handle)
{
    std::lock_guard<std::mutex> lock(_loggerBackendListLock);
    _loggerBackends.erase_after(handle);
}

std::shared_ptr<ConfigSection> CenisysServer::getConfig(const std::string &name)
{
    return _configManager.getConfig(name);
}

void CenisysServer::start()
{
    _ioService.post([this]
                    {
                        _stdoutLogger =
                            std::make_unique<StdoutLogger>(*this, _ioService);
                    });
    _ioService.post([this]
                    {
                        _stdinReader =
                            std::make_unique<StdinReader>(*this, _ioService);
                    });
    _ioService.post([this]
                    {
                        _defaultCommands =
                            std::make_unique<DefaultCommandHandlers>(*this);
                    });
    _termSignals.async_wait(std::bind(&Server::terminate, this));
}

void CenisysServer::stop()
{
    _termSignals.cancel();
    _ioService.post([this]
                    {
                        _defaultCommands.reset();
                    });
    _ioService.post([this]
                    {
                        _stdinReader.reset();
                    });
    _ioService.post([this]
                    {
                        _stdoutLogger.reset();
                    });
}

} // namespace cenisys
