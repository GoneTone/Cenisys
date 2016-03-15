/*
 * Interface represents a chunk.
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
#ifndef CENISYS_CHUNK_H
#define CENISYS_CHUNK_H

#include <memory>

namespace cenisys
{

class World;
class Block;

class Chunk
{
public:
    virtual ~Chunk() = default;
    virtual std::shared_ptr<World> getWorld() = 0;
    virtual std::unique_ptr<Block> getBlock(int x, int y, int z) = 0;

    virtual int getX() = 0;
    virtual int getZ() = 0;
};

} // namespace cenisys

#endif // CENISYS_CHUNK_H
