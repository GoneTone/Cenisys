/*
 * BlockFace
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
#ifndef CENISYS_BLOCKFACE_H
#define CENISYS_BLOCKFACE_H

namespace cenisys
{

class BlockFace
{
public:
    BlockFace(int getModX, int getModY, int getModZ);
    BlockFace operator!();
    int getModX() const;
    int getModY() const;
    int getModZ() const;

    friend BlockFace operator+(const BlockFace &a, const BlockFace &b);

    static const BlockFace UP, DOWN, NORTH, EAST, SOUTH, WEST;

private:
    int _modX;
    int _modY;
    int _modZ;
};

const BlockFace BlockFace::UP(0, 1, 0);
const BlockFace BlockFace::DOWN(0, -1, 0);
const BlockFace BlockFace::NORTH(0, 0, -1);
const BlockFace BlockFace::EAST(1, 0, 0);
const BlockFace BlockFace::SOUTH(0, 0, 1);
const BlockFace BlockFace::WEST(-1, 0, 0);

} // namespace cenisys

#endif // CENISYS_BLOCKFACE_H
