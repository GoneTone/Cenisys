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
#include "block/blockface.h"

namespace cenisys
{

BlockFace::BlockFace(int modX, int modY, int modZ)
    : _modX(modX), _modY(modY), _modZ(modZ)
{
}

BlockFace BlockFace::operator!()
{
    return BlockFace(-_modX, -_modY, -_modZ);
}

int BlockFace::getModX() const
{
    return _modX;
}

int BlockFace::getModY() const
{
    return _modY;
}

int BlockFace::getModZ() const
{
    return _modZ;
}

BlockFace operator+(const BlockFace &a, const BlockFace &b)
{
    return BlockFace(a._modX + b._modX, a._modY + b._modY, a._modZ + b._modZ);
}

} // namespace cenisys
