/*
 * Material and some others.
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
#include "block/block.h"
#include "inventory/itemstack.h"
#include "material/air.h"
#include "material/material.h"

namespace cenisys
{

int ItemMaterial::getMaxStackSize() const
{
    return 64;
}

void ItemMaterial::rightClickAir(Player &player, ItemStack &holding) const
{
    // Do nothing by default
}

void ItemMaterial::rightClickBlock(Player &player, Block &target,
                                   const BlockFace &face, ItemStack &holding,
                                   const Vector &clickedLoc) const
{
    if(const BlockMaterial *block = dynamic_cast<const BlockMaterial *>(this))
    {
        std::unique_ptr<Block> relative = target.getRelative(face);
        Block &targetPlace =
            target.getMaterial().canAbsorb() ? target : *relative;
        bool canBuild = block->canPlaceAt(targetPlace, face);
        // Todo: Events
        if(!canBuild)
            return;
        block->placeBlock(player, targetPlace, face, clickedLoc);
        holding.setAmount(holding.getAmount() - 1);
    }
    else
    {
        rightClickAir(player, holding);
    }
}

std::vector<ItemStack> BlockMaterial::getDrops(const ItemStack &tool) const
{
    if(const ItemMaterial *drop = dynamic_cast<const ItemMaterial *>(this))
    {
        return {
            ItemStack(const_cast<ItemMaterial *>(drop)->shared_from_this())};
    }
    else
    {
        return {};
    }
}

bool BlockMaterial::canAbsorb() const
{
    return false;
}

bool BlockMaterial::canOverride() const
{
    return false;
}

bool BlockMaterial::canPlaceAt(const Block &target, const BlockFace &face) const
{
    return true;
}

void BlockMaterial::placeBlock(Player &player, Block &target,
                               const BlockFace &face,
                               const Vector &clickedLoc) const
{
    // Casting away const: CoW won't write to this if properly implemented
    target.setMaterial(const_cast<BlockMaterial *>(this)->shared_from_this());
}

bool BlockMaterial::blockInteract(Player &player, Block &target,
                                  const BlockFace &face,
                                  const Vector &clickedLoc) const
{
    return false;
}

void BlockMaterial::destroyBlock(Player &player, Block &target,
                                 const BlockFace &face) const
{
    // Beware: The following line commits suicide, the behavior is well-defined
    // only when it's the last line
    target.setMaterial(Air::getDefaultPtr());
}

bool BlockMaterial::blockUpdate(Block &target) const
{
    return false;
}
}
