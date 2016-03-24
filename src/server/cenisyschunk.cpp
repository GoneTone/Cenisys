/*
 * CenisysChunk
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
#include <algorithm>
#include <memory>
#include "block/cenisysblock.h"
#include "material/air.h"
#include "material/material.h"
#include "server/cenisyschunk.h"

namespace cenisys
{

CenisysChunk::CenisysChunk(std::shared_ptr<World> world, int x, int z)
    : _world(world), _x(x), _z(z)
{
    _sections.fill(Section::getEmptySection());
}

CenisysChunk::~CenisysChunk()
{
}

std::shared_ptr<World> CenisysChunk::getWorld() const
{
    return _world;
}

std::unique_ptr<const Block>
CenisysChunk::getBlock(unsigned int x, unsigned int y, unsigned int z) const
{
    std::lock_guard<std::mutex> lock(_sectionLock);
    // Casting away const: const Block will protect us from modifying content
    const std::shared_ptr<Section> &section = _sections[y / 16];
    std::mutex &mutex = section->getMutex(x, y % 16, z);
    // Lock then pass: Secure the data of the section
    mutex.lock();
    return std::make_unique<CenisysBlock>(
        Location(const_cast<CenisysChunk *>(this)->shared_from_this(), x, y, z),
        section->getMaterial(x, y % 16, z), mutex,
        section->getSkyLight(x, y % 16, z),
        section->getBlockLight(x, y % 16, z));
}

std::unique_ptr<Block> CenisysChunk::getBlock(unsigned int x, unsigned int y,
                                              unsigned int z)
{
    std::lock_guard<std::mutex> lock(_sectionLock);
    std::shared_ptr<Section> &section = _sections[y / 16];
    std::mutex &mutex = section->getMutex(x, y % 16, z);
    mutex.lock();
    // Section Copy-on-Write
    if(!section.unique())
    {
        section = std::make_shared<Section>(*section);
        mutex.unlock();
        std::mutex &newMutex = section->getMutex(x, y % 16, z);
        newMutex.lock();
    }
    // Material CopyLess/Copy-on-Write
    std::shared_ptr<BlockMaterial> &material =
        section->getMaterial(x, y % 16, z);
    if(!material.unique())
    {
        material = std::shared_ptr<BlockMaterial>(material->clone());
    }
    return std::make_unique<CenisysBlock>(Location(shared_from_this(), x, y, z),
                                          material, mutex,
                                          section->getSkyLight(x, y % 16, z),
                                          section->getBlockLight(x, y % 16, z));
}

int CenisysChunk::getX() const
{
    return _x;
}

int CenisysChunk::getZ() const
{
    return _z;
}

CenisysChunk::Section::Section()
{
    std::fill(_materials.begin()->begin()->begin(),
              _materials.end()->end()->end(), Air::getDefaultPtr());
    std::fill(_blockLight.begin()->begin()->begin(),
              _blockLight.end()->end()->end(), 0);
    std::fill(_skyLight.begin()->begin()->begin(),
              _skyLight.end()->end()->end(), 15);
}

CenisysChunk::Section::Section(CenisysChunk::Section &other)
    : _materials(other._materials), _blockLight(other._blockLight),
      _skyLight(other._skyLight)
{
}

std::shared_ptr<CenisysChunk::Section> CenisysChunk::Section::getEmptySection()
{
    return _empty;
}

std::shared_ptr<BlockMaterial> &
CenisysChunk::Section::getMaterial(unsigned int x, unsigned int y,
                                   unsigned int z)
{
    return _materials[x][y][z];
}

unsigned char &CenisysChunk::Section::getBlockLight(unsigned int x,
                                                    unsigned int y,
                                                    unsigned int z)
{
    return _blockLight[x][y][z];
}

unsigned char &CenisysChunk::Section::getSkyLight(unsigned int x,
                                                  unsigned int y,
                                                  unsigned int z)
{
    return _skyLight[x][y][z];
}

std::mutex &CenisysChunk::Section::getMutex(unsigned int x, unsigned int y,
                                            unsigned int z)
{
    return _mutex[x][y][z];
}

const std::shared_ptr<CenisysChunk::Section> CenisysChunk::Section::_empty =
    std::make_shared<CenisysChunk::Section>();

} // namespace cenisys
