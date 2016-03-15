/*
 * Interface represents a world.
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
#ifndef CENISYS_WORLD_H
#define CENISYS_WORLD_H

#include <memory>

namespace cenisys
{

class Block;
class Chunk;

class World
{
public:
    virtual ~World() = default;

    virtual std::unique_ptr<Block> getBlockAt(int x, int y, int z) = 0;
    virtual std::shared_ptr<Chunk> getChunkAt(int x, int z) = 0;
};

} // namespace cenisys

#endif // CENISYS_WORLD_H
