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
#ifndef CENISYS_STDINREADER_H
#define CENISYS_STDINREADER_H

#include <atomic>
#include <thread>
#include <boost/asio/io_service.hpp>
#include "server/server.h"

namespace cenisys
{

class StdinReader
{
public:
    StdinReader(Server &server, boost::asio::io_service &ioService);
    ~StdinReader();

private:
    void asyncWorker();

    Server &_server;
    boost::asio::io_service &_ioService;
    std::atomic_bool _running;
    std::thread _asyncThread;
};

} // namespace cenisys
#endif // CENISYS_STDINREADER_H
