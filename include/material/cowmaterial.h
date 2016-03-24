/*
 * Copy-on-Write material implementation.
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
#ifndef CENISYS_COWMATERIAL_H
#define CENISYS_COWMATERIAL_H

#include <memory>

namespace cenisys
{

template <typename T>
class CoWMaterial
{
public:
    static constexpr std::shared_ptr<T> getDefaultPtr() { return _default; }
private:
    static const std::shared_ptr<T> _default;
};

template <typename T>
const std::shared_ptr<T> CoWMaterial<T>::_default = std::make_shared<T>();

} // namespace cenisys

#endif // CENISYS_COWMATERIAL_H
