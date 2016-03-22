/*
 * Read commands from standard input.
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
#include <iostream>
#include "server/stdinreader.h"

namespace cenisys
{

StdinReader::StdinReader(Server &server, boost::asio::io_service &ioService)
    : _server(server), _ioService(ioService), _running(true),
      _asyncThread(std::bind(&StdinReader::asyncWorker, this))
{
}

StdinReader::~StdinReader()
{
    _running = false;
    if(std::cin)
        std::cerr << "Please press Enter to continue..." << std::endl;
    _asyncThread.join();
}

void StdinReader::asyncWorker()
{
    while(_running)
    {
        std::string buf;
        std::getline(std::cin, buf);
        if(!buf.empty())
            _ioService.post(std::bind(&Server::dispatchCommand, &_server, buf));
        if(!std::cin)
        {
            _ioService.post(std::bind(&Server::terminate, &_server));
            break;
        }
    }
}

} // namespace cenisys
