/*
 * ItemStack
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
#include "material/material.h"
#include "inventory/itemstack.h"

namespace cenisys
{

ItemStack::ItemStack(ItemMaterial *material, int amount)
    : ItemStack(std::unique_ptr<ItemMaterial>(material), amount)
{
}

ItemStack::ItemStack(std::unique_ptr<ItemMaterial> &&material, int amount)
    : _amount(amount),
      _material(std::forward<std::unique_ptr<ItemMaterial>>(material))
{
}

ItemStack::ItemStack(const ItemStack &other)
    : ItemStack(other._material->clone(), other._amount)
{
}

ItemStack::~ItemStack()
{
}

ItemStack &ItemStack::operator=(const ItemStack &other)
{
    _material = std::unique_ptr<ItemMaterial>(other._material->clone());
    _amount = other._amount;
    return *this;
}

int ItemStack::getAmount() const
{
    return _amount;
}

void ItemStack::setAmount(int amount)
{
    _amount = amount;
}

const ItemMaterial &ItemStack::getMaterial() const
{
    return *_material;
}

ItemMaterial &ItemStack::getMaterial()
{
    return *_material;
}

void ItemStack::setMaterial(ItemMaterial *material)
{
    setMaterial(std::unique_ptr<ItemMaterial>(material));
}

void ItemStack::setMaterial(std::unique_ptr<ItemMaterial> &&material)
{
    _material = std::forward<std::unique_ptr<ItemMaterial>>(material);
}

} // namespace cenisys
