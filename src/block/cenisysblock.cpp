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
                           std::unique_ptr<BlockMaterial> &material,
                           std::mutex &mutex, char &skyLight, char &blockLight)
    : _material(material), _mutex(mutex), _location(location),
      _skyLight(skyLight), _blockLight(blockLight)
{
    _mutex.lock();
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

void CenisysBlock::setMaterial(BlockMaterial *material, bool blockUpdate)
{
    setMaterial(std::unique_ptr<BlockMaterial>(material), blockUpdate);
}

void CenisysBlock::setMaterial(std::unique_ptr<BlockMaterial> &&material,
                               bool blockUpdate)
{
    // TODO: Should we check for nullptr?
    // TODO: Block updates
    _material = std::forward<std::unique_ptr<BlockMaterial>>(material);
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
    return getWorld()->getBlockAt(_location.getX() + modX,
                                  _location.getY() + modY,
                                  _location.getZ() + modZ);
}

const Location &CenisysBlock::getLocation() const
{
    return _location;
}

char CenisysBlock::getSkyLight() const
{
    return _skyLight;
}

char CenisysBlock::getBlockLight() const
{
    return _blockLight;
}

char CenisysBlock::getLightLevel() const
{
    return std::max(_skyLight, _blockLight);
}
} // namespace cenisys
