/*
 * CenisysBlock
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
#ifndef CENISYS_CENISYSBLOCK_H
#define CENISYS_CENISYSBLOCK_H

#include <mutex>
#include "block/block.h"

namespace cenisys
{

class CenisysBlock : public Block
{
public:
    CenisysBlock(const Location &location,
                 std::shared_ptr<BlockMaterial> material, std::mutex &mutex,
                 unsigned char &skyLight, unsigned char &blockLight);
    ~CenisysBlock();

    const BlockMaterial &getMaterial() const;
    BlockMaterial &getMaterial();
    void setMaterial(BlockMaterial *material, bool updatePhysics = true);
    void setMaterial(std::shared_ptr<BlockMaterial> material,
                     bool updatePhysics = true);
    std::shared_ptr<World> getWorld() const;
    std::unique_ptr<Block> getRelative(const BlockFace &face) const;
    std::unique_ptr<Block> getRelative(int modX, int modY, int modZ) const;

    const Location &getLocation() const;
    unsigned char getSkyLight() const;
    unsigned char getBlockLight() const;
    unsigned char getLightLevel() const;

    bool blockUpdate();

private:
    std::shared_ptr<BlockMaterial> _material;
    std::mutex &_mutex;
    const Location _location;
    unsigned char &_skyLight;
    unsigned char &_blockLight;
};

} // namespace cenisys

#endif // CENISYS_CENISYSBLOCK_H
