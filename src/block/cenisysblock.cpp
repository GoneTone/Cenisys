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
#include <memory>
#include "material/material.h"
#include "block/blockface.h"
#include "server/world.h"
#include "cenisysblock.h"

namespace cenisys
{

CenisysBlock::CenisysBlock(const Location &location,
                           std::shared_ptr<BlockMaterial> &material,
                           std::mutex &mutex, unsigned char &skyLight,
                           unsigned char &blockLight)
    : _material(material), _mutex(mutex), _location(location),
      _skyLight(skyLight), _blockLight(blockLight)
{
}

CenisysBlock::~CenisysBlock()
{
    _mutex.unlock();
}

const BlockMaterial &CenisysBlock::getMaterial() const
{
    return *_material;
}

BlockMaterial &CenisysBlock::getMaterial()
{
    return *_material;
}

void CenisysBlock::setMaterial(BlockMaterial *material, bool updatePhysics)
{
    setMaterial(std::shared_ptr<BlockMaterial>(material), updatePhysics);
}

void CenisysBlock::setMaterial(std::shared_ptr<BlockMaterial> material,
                               bool updatePhysics)
{
    if(!material)
    {
        // TODO: throw
    }
    std::shared_ptr<BlockMaterial> oldMaterial = std::move(_material);
    _material = std::move(material);
    if(updatePhysics)
    {
        if(oldMaterial->blockUpdate(*this))
        {
            blockUpdate();
        }
    }
}

std::shared_ptr<World> CenisysBlock::getWorld() const
{
    return _location.getWorld();
}

std::unique_ptr<Block> CenisysBlock::getRelative(const BlockFace &face) const
{
    return getRelative(face.getModX(), face.getModY(), face.getModZ());
}

std::unique_ptr<Block> CenisysBlock::getRelative(int modX, int modY,
                                                 int modZ) const
{
    return getWorld()->getBlockAt(static_cast<int>(_location.getX()) + modX,
                                  static_cast<unsigned int>(_location.getY()) +
                                      modY,
                                  static_cast<int>(_location.getZ()) + modZ);
}

const Location &CenisysBlock::getLocation() const
{
    return _location;
}

unsigned char CenisysBlock::getSkyLight() const
{
    return _skyLight;
}

unsigned char CenisysBlock::getBlockLight() const
{
    return _blockLight;
}

unsigned char CenisysBlock::getLightLevel() const
{
    return std::max(_skyLight, _blockLight);
}

bool CenisysBlock::blockUpdate()
{
    if(_material->blockUpdate(*this))
    {
        // Something changed, trigger further updates
        for(const BlockFace &direction :
            {BlockFace::UP, BlockFace::DOWN, BlockFace::NORTH, BlockFace::EAST,
             BlockFace::SOUTH, BlockFace::WEST})
        {
            getRelative(direction)->blockUpdate();
        }
        return true;
    }
    return false;
}
} // namespace cenisys
