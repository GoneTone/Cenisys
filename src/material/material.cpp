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

void ItemMaterial::rightClickAir(Player &player, ItemStack &holding)
{
    // Do nothing by default
}

void ItemMaterial::rightClickBlock(Player &player, Block &target,
                                   const BlockFace &face, ItemStack &holding,
                                   const Vector &clickedLoc)
{
    if(BlockMaterial *block = dynamic_cast<BlockMaterial *>(this))
    {
        block->placeBlock(player, target, face, clickedLoc);
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
        return {ItemStack(drop->clone())};
    }
    else
    {
        return {};
    }
}

bool BlockMaterial::canAbsorb()
{
    return false;
}

bool BlockMaterial::canOverride()
{
    return false;
}

bool BlockMaterial::canPlaceAt(const Block &target, const BlockFace &face) const
{
    return true;
}

void BlockMaterial::placeBlock(Player &player, Block &target,
                               const BlockFace &face, const Vector &clickedLoc)
{
    target.setMaterial(clone());
}

bool BlockMaterial::blockInteract(Player &player, Block &target,
                                  const BlockFace &face,
                                  const Vector &clickedLoc)
{
    return false;
}

void BlockMaterial::destroyBlock(Player &player, Block &target,
                                 const BlockFace &face)
{
    target.setMaterial(new Air);
}

void BlockMaterial::blockUpdate(Block &target)
{
}
}
