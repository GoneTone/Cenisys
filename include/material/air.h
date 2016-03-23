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
#ifndef CENISYS_AIR_H
#define CENISYS_AIR_H

#include "material/material.h"

namespace cenisys
{

class Air : public BlockMaterial, ItemMaterial
{
public:
    Air();
    ~Air();

    Air *clone() const;
    std::string getName() const;

    int getMaxStackSize() const;
    void rightClickAir(Player &player, ItemStack &holding);
    void rightClickBlock(Player &player, Block &target, const BlockFace &face,
                         ItemStack &holding, const Vector &clickedLoc);

    std::vector<ItemStack> getDrops(const ItemStack &tool) const;
    bool canAbsorb();
    bool canOverride();
    bool canPlaceAt(const Block &target, const BlockFace &face) const;
    void placeBlock(Player &player, Block &target, const BlockFace &face,
                    const Vector &clickedLoc);
    bool blockInteract(Player &player, Block &target, const BlockFace &face,
                       const Vector &clickedLoc);
    void destroyBlock(Player &player, Block &target, const BlockFace &face);
    void blockUpdate(Block &target);
};

} // namespace cenisys

#endif // CENISYS_AIR_H