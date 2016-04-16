/*
 * Template class for NBT operations.
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
#ifndef CENISYS_NBT_H
#define CENISYS_NBT_H

#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <boost/endian/buffers.hpp>

namespace cenisys
{

namespace NBT
{

enum class TagType : std::int_least8_t
{
    End = 0,
    Byte = 1,
    Short = 2,
    Int = 3,
    Long = 4,
    Float = 5,
    Double = 6,
    ByteArray = 7,
    String = 8,
    List = 9,
    Compound = 10,
    IntArray = 11,
};

class Tag
{
public:
    TagType getType() const { return _type; }

protected:
    Tag(TagType type) : _type(type) {}

private:
    TagType _type;
};

template <typename T>
class BasicTag : public Tag
{
public:
    // Making use of SFINAE, it will crash if no specialization available
    BasicTag(T data = {}) {}
    BasicTag(TagType type, T data = {}) : Tag(type), _data(data) {}
    operator T &() { return _data; }
    operator const T &() const { return _data; }

private:
    T _data;
};

template <>
BasicTag<int_least8_t>::BasicTag(int_least8_t data)
    : BasicTag(TagType::Byte, data)
{
}

template <>
BasicTag<int_least16_t>::BasicTag(int_least16_t data)
    : BasicTag(TagType::Short, data)
{
}

template <>
BasicTag<int_least32_t>::BasicTag(int_least32_t data)
    : BasicTag(TagType::Int, data)
{
}

template <>
BasicTag<int_least64_t>::BasicTag(int_least64_t data)
    : BasicTag(TagType::Long, data)
{
}

template <>
BasicTag<float>::BasicTag(float data)
    : BasicTag(TagType::Float, data)
{
}

template <>
BasicTag<double>::BasicTag(double data)
    : BasicTag(TagType::Double, data)
{
}

class ListTag : public Tag, public std::vector<std::unique_ptr<Tag>>
{
public:
    ListTag() : ListTag(TagType::End, {}) {}
    ListTag(TagType type, std::vector<std::unique_ptr<Tag>> vector)
        : Tag(type), std::vector<std::unique_ptr<Tag>>(std::move(vector))
    {
    }
};

class StringTag : public Tag, public std::string
{
public:
    StringTag(std::string string = {})
        : Tag(TagType::String), std::string(std::move(string))
    {
    }
};

class CompoundTag : public Tag,
                    public std::unordered_map<std::string, std::unique_ptr<Tag>>
{
public:
    CompoundTag(std::unordered_map<std::string, std::unique_ptr<Tag>> map = {})
        : Tag(TagType::Compound),
          std::unordered_map<std::string, std::unique_ptr<Tag>>(std::move(map))
    {
    }
};

class InvalidFormatException : public std::exception
{
public:
    explicit InvalidFormatException(const std::string &what_arg)
        : _what(what_arg)
    {
    }
    explicit InvalidFormatException(const char *what_arg) : _what(what_arg) {}
    const char *what() const noexcept { return _what.c_str(); }
private:
    std::string _what;
};

template <TagType type>
class TagTypeToClass;

template <>
class TagTypeToClass<TagType::Byte>
{
public:
    using value_type = int_least8_t;
    using type = BasicTag<value_type>;
};
template <>
class TagTypeToClass<TagType::Short>
{
public:
    using value_type = int_least16_t;
    using type = BasicTag<value_type>;
};
template <>
class TagTypeToClass<TagType::Int>
{
public:
    using value_type = int_least32_t;
    using type = BasicTag<value_type>;
};
template <>
class TagTypeToClass<TagType::Long>
{
public:
    using value_type = int_least64_t;
    using type = BasicTag<value_type>;
};
template <>
class TagTypeToClass<TagType::Float>
{
public:
    using value_type = float;
    using type = BasicTag<value_type>;
};
template <>
class TagTypeToClass<TagType::Double>
{
public:
    using value_type = double;
    using type = BasicTag<value_type>;
};
template <>
class TagTypeToClass<TagType::ByteArray>
{
public:
    using type = ListTag;
};
template <>
class TagTypeToClass<TagType::String>
{
public:
    using value_type = std::string;
    using type = StringTag;
};
template <>
class TagTypeToClass<TagType::List>
{
public:
    using type = ListTag;
};
template <>
class TagTypeToClass<TagType::IntArray>
{
public:
    using type = ListTag;
};
template <>
class TagTypeToClass<TagType::Compound>
{
public:
    using type = CompoundTag;
};

template <boost::endian::order endian>
std::unique_ptr<Tag> parsePayload(std::istream &input, TagType type);

template <boost::endian::order endian>
std::pair<std::string, std::unique_ptr<Tag>> parse(std::istream &input);

TagType parseTagType(char type)
{
    if(type < 0 || type > 11)
    {
        throw InvalidFormatException("Tag type out of range");
    }
    return static_cast<TagType>(type);
}

template <boost::endian::order endian, typename T>
std::unique_ptr<BasicTag<T>> parseBasic(std::istream &input)
{
    boost::endian::endian_buffer<endian, T, sizeof(T) * 8> buf;
    input.read(reinterpret_cast<char *>(&buf), sizeof(buf));
    return std::make_unique<BasicTag<T>>(buf.value());
}

// HACK: Workaround boost endian issue
template <boost::endian::order endian>
std::unique_ptr<BasicTag<float>> parseFloat(std::istream &input)
{
    boost::endian::endian_buffer<endian, int_least32_t, 32> buf;
    input.read(reinterpret_cast<char *>(&buf), sizeof(buf));
    int_least32_t value = buf.value();
    return std::make_unique<BasicTag<float>>(reinterpret_cast<float &>(value));
}

template <boost::endian::order endian>
std::unique_ptr<BasicTag<double>> parseDouble(std::istream &input)
{
    boost::endian::endian_buffer<endian, int_least64_t, 64> buf;
    input.read(reinterpret_cast<char *>(&buf), sizeof(buf));
    int_least64_t value = buf.value();
    return std::make_unique<BasicTag<double>>(
        reinterpret_cast<double &>(value));
}

template <boost::endian::order endian>
std::unique_ptr<ListTag> parseListPayload(std::istream &input, TagType listType,
                                          TagType elementType)
{
    boost::endian::endian_buffer<endian, int_least32_t, 32> size;
    input.read(reinterpret_cast<char *>(&size), sizeof(size));
    std::vector<std::unique_ptr<Tag>> list;
    for(int i = 0; i < size.value(); i++)
    {
        list.push_back(std::move(parsePayload<endian>(input, elementType)));
    }
    return std::make_unique<ListTag>(listType, std::move(list));
}

template <boost::endian::order endian>
std::unique_ptr<ListTag> parseList(std::istream &input)
{
    return parseListPayload<endian>(input, TagType::List,
                                    parseTagType(input.get()));
}

template <boost::endian::order endian>
std::unique_ptr<StringTag> parseString(std::istream &input)
{
    boost::endian::endian_buffer<endian, uint_least16_t, 16> size;
    input.read(reinterpret_cast<char *>(&size), sizeof(size));
    std::string str(size.value(), '\0');
    input.read(&str[0], size.value());
    return std::make_unique<StringTag>(std::move(str));
}

template <boost::endian::order endian>
std::unique_ptr<CompoundTag> parseCompound(std::istream &input)
{
    std::unordered_map<std::string, std::unique_ptr<Tag>> compound;
    std::pair<std::string, std::unique_ptr<Tag>> elem;
    while((elem = parse<endian>(input)).second != nullptr)
    {
        compound[elem.first] = std::move(elem.second);
    }
    return std::make_unique<CompoundTag>(std::move(compound));
}

template <boost::endian::order endian>
std::unique_ptr<Tag> parsePayload(std::istream &input, TagType type)
{
    std::unique_ptr<Tag> result;
    switch(type)
    {
    case TagType::Byte:
        result = parseBasic<endian, int_least8_t>(input);
        break;
    case TagType::Short:
        result = parseBasic<endian, int_least16_t>(input);
        break;
    case TagType::Int:
        result = parseBasic<endian, int_least32_t>(input);
        break;
    case TagType::Long:
        result = parseBasic<endian, int_least64_t>(input);
        break;
    case TagType::Float:
        result = parseFloat<endian>(input);
        break;
    case TagType::Double:
        result = parseDouble<endian>(input);
        break;
    case TagType::ByteArray:
        result =
            parseListPayload<endian>(input, TagType::ByteArray, TagType::Byte);
        break;
    case TagType::String:
        result = parseString<endian>(input);
        break;
    case TagType::List:
        result = parseList<endian>(input);
        break;
    case TagType::Compound:
        result = parseCompound<endian>(input);
        break;
    case TagType::IntArray:
        result =
            parseListPayload<endian>(input, TagType::IntArray, TagType::Int);
        break;
    }
    return result;
}

template <boost::endian::order endian>
std::pair<std::string, std::unique_ptr<Tag>> parse(std::istream &input)
{
    TagType type = parseTagType(input.get());
    if(type == TagType::End)
        return std::make_pair("", nullptr);
    std::string name =
        static_cast<StringTag &>(*parsePayload<endian>(input, TagType::String));
    std::unique_ptr<Tag> result(parsePayload<endian>(input, type));
    return std::make_pair(name, std::move(result));
}

template <boost::endian::order endian>
void emit(std::ostream &output, const std::string &name, const Tag &tag);

template <boost::endian::order endian>
void emitPayload(std::ostream &output, const Tag &tag);

template <boost::endian::order endian, typename T>
void emitBasic(std::ostream &output, const BasicTag<T> &tag)
{
    boost::endian::endian_buffer<endian, T, sizeof(T) * 8> buf(tag);
    output.write(reinterpret_cast<char *>(&buf), sizeof(buf));
}

// HACK: Boost endian workaround
template <boost::endian::order endian>
void emitFloat(std::ostream &output, const BasicTag<float> &tag)
{
    float value = tag;
    boost::endian::endian_buffer<endian, int_least32_t, 32> buf(
        reinterpret_cast<int_least32_t &>(value));
    output.write(reinterpret_cast<char *>(&buf), sizeof(buf));
}
template <boost::endian::order endian>
void emitDouble(std::ostream &output, const BasicTag<double> &tag)
{
    double value = tag;
    boost::endian::endian_buffer<endian, int_least64_t, 64> buf(
        reinterpret_cast<int_least64_t &>(value));
    output.write(reinterpret_cast<char *>(&buf), sizeof(buf));
}

template <boost::endian::order endian>
void emitListPayload(std::ostream &output, const ListTag &tag)
{
    boost::endian::endian_buffer<endian, int_least32_t, 32> size(tag.size());
    output.write(reinterpret_cast<char *>(&size), sizeof(size));
    for(const auto &item : tag)
    {
        emitPayload<endian>(output, *item);
    }
}

template <boost::endian::order endian>
void emitList(std::ostream &output, const ListTag &tag)
{
    TagType type = TagType::End;
    if(!tag.empty())
    {
        type = tag[0]->getType();
    }
    output.put(static_cast<char>(tag.getType()));
}

template <boost::endian::order endian>
void emitString(std::ostream &output, const StringTag &tag)
{
    boost::endian::endian_buffer<endian, uint_least16_t, 16> size(tag.size());
    output.write(reinterpret_cast<char *>(&size), sizeof(size));
    output.write(&tag[0], tag.size());
}

template <boost::endian::order endian>
void emitCompound(std::ostream &output, const CompoundTag &tag)
{
    for(const auto &item : tag)
    {
        emit<endian>(output, item.first, *(item.second));
    }
    output.put(static_cast<char>(TagType::End));
}

template <boost::endian::order endian>
void emitPayload(std::ostream &output, const Tag &tag)
{
    switch(tag.getType())
    {
    case TagType::Byte:
        emitBasic<endian>(output,
                          static_cast<const BasicTag<int_least8_t> &>(tag));
        break;
    case TagType::Short:
        emitBasic<endian>(output,
                          static_cast<const BasicTag<int_least16_t> &>(tag));
        break;
    case TagType::Int:
        emitBasic<endian>(output,
                          static_cast<const BasicTag<int_least32_t> &>(tag));
        break;
    case TagType::Long:
        emitBasic<endian>(output,
                          static_cast<const BasicTag<int_least64_t> &>(tag));
        break;
    case TagType::Float:
        emitFloat<endian>(output, static_cast<const BasicTag<float> &>(tag));
        break;
    case TagType::Double:
        emitDouble<endian>(output, static_cast<const BasicTag<double> &>(tag));
        break;
    case TagType::String:
        emitString<endian>(output, static_cast<const StringTag &>(tag));
        break;
    case TagType::Compound:
        emitCompound<endian>(output, static_cast<const CompoundTag &>(tag));
        break;
    case TagType::List:
        emitList<endian>(output, static_cast<const ListTag &>(tag));
    case TagType::ByteArray:
    case TagType::IntArray:
        emitListPayload<endian>(output, static_cast<const ListTag &>(tag));
        break;
    }
}

template <boost::endian::order endian>
void emit(std::ostream &output, const std::string &name, const Tag &tag)
{
    output.put(static_cast<char>(tag.getType()));
    emitString<endian>(output, name);
    emitPayload<endian>(output, tag);
}
}
}

#endif // CENISYS_NBT_H
