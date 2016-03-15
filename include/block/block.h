/*
 * Block
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
#ifndef CENISYS_BLOCK_H
#define CENISYS_BLOCK_H

#include <memory>
#include <mutex>
#include "util/location.h"

namespace cenisys
{

class Material;
class BlockMaterial;
class BlockFace;

//!
//! \brief The interface of a block.
//!
class Block
{
public:
    virtual ~Block() = default;

    virtual const BlockMaterial &getMaterial() const = 0;
    virtual BlockMaterial &getMaterial() = 0;
    virtual void setMaterial(BlockMaterial *material,
                             bool blockUpdate = true) = 0;
    virtual void setMaterial(std::unique_ptr<BlockMaterial> &&material,
                             bool blockUpdate = true) = 0;

    virtual const Location &getLocation() const = 0;
    virtual std::shared_ptr<World> getWorld() const = 0;
    virtual std::unique_ptr<Block> getRelative(const BlockFace &face) const = 0;
    virtual std::unique_ptr<Block> getRelative(int modX, int modY,
                                               int modZ) const = 0;

    virtual char getSkyLight() const = 0;
    virtual char getBlockLight() const = 0;
    virtual char getLightLevel() const = 0;

    // TODO: Block entities
};

} // namespace cenisys

#endif // CENISYS_BLOCK_H
