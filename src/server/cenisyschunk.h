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
#ifndef CENISYS_CENISYSCHUNK_H
#define CENISYS_CENISYSCHUNK_H

#include <array>
#include <memory>
#include <mutex>
#include "server/chunk.h"

namespace cenisys
{

class BlockMaterial;

class CenisysChunk : public Chunk, std::enable_shared_from_this<Chunk>
{
public:
    CenisysChunk(std::shared_ptr<World> world, int x, int z);
    ~CenisysChunk();

    std::shared_ptr<World> getWorld() const;
    std::unique_ptr<const Block> getBlock(unsigned int x, unsigned int y,
                                          unsigned int z) const;
    std::unique_ptr<Block> getBlock(unsigned int x, unsigned int y,
                                    unsigned int z);
    int getX() const;
    int getZ() const;

private:
    class Section
    {
    public:
        Section();
        Section(Section &other);
        static std::shared_ptr<Section> getEmptySection();

        std::shared_ptr<BlockMaterial> &
        getMaterial(unsigned int x, unsigned int y, unsigned int z);
        unsigned char &getBlockLight(unsigned int x, unsigned int y,
                                     unsigned int z);
        unsigned char &getSkyLight(unsigned int x, unsigned int y,
                                   unsigned int z);
        std::mutex &getMutex(unsigned int x, unsigned int y, unsigned int z);

    private:
        std::array<
            std::array<std::array<std::shared_ptr<BlockMaterial>, 16>, 16>, 16>
            _materials;
        std::array<std::array<std::array<unsigned char, 16>, 16>, 16>
            _blockLight;
        std::array<std::array<std::array<unsigned char, 16>, 16>, 16> _skyLight;
        std::array<std::array<std::array<std::mutex, 16>, 16>, 16> _mutex;
        static const std::shared_ptr<Section> _empty;
    };
    std::shared_ptr<World> _world;
    int _x;
    int _z;
    std::array<std::shared_ptr<Section>, 16> _sections;
    mutable std::mutex _sectionLock;
};

} // namespace cenisys

#endif // CENISYS_CENISYSCHUNK_H
