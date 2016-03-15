/*
 * Air
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
#include "inventory/itemstack.h"
#include "material/air.h"

namespace cenisys
{

Air::Air()
{
}

Air::~Air()
{
}

Air *Air::clone() const
{
    return new Air;
}

std::string Air::getName() const
{
    return "air";
}

int Air::getMaxStackSize() const
{
    return 0;
}

void Air::rightClickAir(Player &player, ItemStack &holding)
{
    // Do nothing
}

void Air::rightClickBlock(Player &player, Block &target, const BlockFace &face,
                          ItemStack &holding, const Vector &clickedLoc)
{
    // Do nothing
}

std::vector<ItemStack> Air::getDrops(const ItemStack &tool) const
{
    return {};
}

bool Air::canAbsorb()
{
    // TODO: throw
}

bool Air::canOverride()
{
    return true;
}

bool Air::canPlaceAt(const Block &target, const BlockFace &face) const
{
    // TODO: throw
}

void Air::placeBlock(Player &player, Block &target, const BlockFace &face,
                     const Vector &clickedLoc)
{
    // TODO: throw
}

bool Air::blockInteract(Player &player, Block &target, const BlockFace &face,
                        const Vector &clickedLoc)
{
    return false;
}

void Air::destroyBlock(Player &player, Block &target, const BlockFace &face)
{
    // TODO: throw
}

void Air::blockUpdate(Block &target)
{
}

} // namespace cenisys
