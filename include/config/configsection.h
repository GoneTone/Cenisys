/*
 * A section of a config.
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
#ifndef CENISYS_CONFIGSECTION_H
#define CENISYS_CONFIGSECTION_H

#include <string>
#include <vector>

namespace cenisys
{

//!
//! \brief Represents a section of a config, or a subsection of a section.
//!
class ConfigSection
{
public:
    class Path
    {
    public:
        const std::vector<std::string> &getItems() const { return _items; }
        Path up() const
        {
            Path ret(*this);
            ret._items.pop_back();
            return ret;
        }
        friend Path operator/(const Path &lhs, const std::string &rhs)
        {
            Path ret(lhs);
            ret._items.push_back(rhs);
            return ret;
        }

    private:
        std::vector<std::string> _items;
    };

    virtual ~ConfigSection() = default;

    virtual bool getBool(const Path &path, bool defaultValue) = 0;
    virtual int getInt(const Path &path, int defaultValue) = 0;
    virtual unsigned int getUInt(const Path &path,
                                 unsigned int defaultValue) = 0;
    virtual double getDouble(const Path &path, double defaultValue) = 0;
    virtual std::string getString(const Path &path,
                                  const std::string &defaultValue) = 0;
    virtual std::vector<bool>
    getBoolList(const Path &path,
                const std::vector<bool> &defaultValue = {}) = 0;
    virtual std::vector<int>
    getIntList(const Path &path, const std::vector<int> &defaultValue = {}) = 0;
    virtual std::vector<unsigned int>
    getUIntList(const Path &path,
                const std::vector<unsigned int> &defaultValue = {}) = 0;
    virtual std::vector<double>
    getDoubleList(const Path &path,
                  const std::vector<double> &defaultValue = {}) = 0;
    virtual std::vector<std::string>
    getStringList(const Path &path,
                  const std::vector<std::string> &defaultValue = {}) = 0;
    virtual void setBool(const Path &path, bool value) = 0;
    virtual void setInt(const Path &path, int value) = 0;
    virtual void setUInt(const Path &path, unsigned int value) = 0;
    virtual void setDouble(const Path &path, double value) = 0;
    virtual void setString(const Path &path, const std::string &value) = 0;
    virtual void setBoolList(const Path &path,
                             const std::vector<bool> &value) = 0;
    virtual void setIntList(const Path &path,
                            const std::vector<int> &value) = 0;
    virtual void setDoubleList(const Path &path,
                               const std::vector<double> &value) = 0;
    virtual void setStringList(const Path &path,
                               const std::vector<std::string> &value) = 0;
    virtual std::vector<Path> getKeys(const Path &path) = 0;
};

} // namespace cenisys

#endif // CENISYS_CONFIGSECTION_H
