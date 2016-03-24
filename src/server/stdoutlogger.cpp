/*
 * Logger for standard output.
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
#include <iostream>
#include <boost/locale/format.hpp>
#include <boost/locale/message.hpp>
#include "server/stdoutlogger.h"

namespace cenisys
{
StdoutLogger::StdoutLogger(Server &server, boost::asio::io_service &ioService)
    : _server(server), _ioService(ioService), _running(true),
      _asyncThread(std::bind(&StdoutLogger::asyncWorker, this))
{
    _backendHandle = _server.getLogger().registerBackend(std::make_tuple(
        std::bind(
            static_cast<void (StdoutLogger::*)(const boost::locale::format &)>(
                &StdoutLogger::log),
            this, std::placeholders::_1),
        std::bind(
            static_cast<void (StdoutLogger::*)(const boost::locale::message &)>(
                &StdoutLogger::log),
            this, std::placeholders::_1)));
}

StdoutLogger::~StdoutLogger()
{
    _server.getLogger().unregisterBackend(_backendHandle);
    std::unique_lock<std::mutex> lock(_writeQueueLock);
    _running = false;
    lock.unlock();
    _queueNotifier.notify_all();
    _asyncThread.join();
}

void StdoutLogger::log(const boost::locale::format &content)
{
    std::unique_lock<std::mutex> lock(_writeQueueLock);
    _writeQueue.push(content.str());
    lock.unlock();
    _queueNotifier.notify_one();
}

void StdoutLogger::log(const boost::locale::message &content)
{
    std::unique_lock<std::mutex> lock(_writeQueueLock);
    _writeQueue.push(content.str());
    lock.unlock();
    _queueNotifier.notify_one();
}

void StdoutLogger::asyncWorker()
{
    while(_running)
    {
        std::unique_lock<std::mutex> lock(_writeQueueLock);
        if(_running && _writeQueue.empty())
            _queueNotifier.wait(lock);
        // Note: destructor may notify the thread
        if(!_running)
            break;
        std::string buf = _writeQueue.front();
        _writeQueue.pop();
        lock.unlock();
        std::cout << buf << std::endl;
    }
}

} // namespace cenisys
