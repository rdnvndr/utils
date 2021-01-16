#pragma once

#include <cstddef>
#include <array>
#include <memory>
#include <type_traits>

namespace RTPTechGroup {
namespace Utils {

//! Конвертирует объект в бинарные данные
template<typename T>
std::array<std::byte, sizeof(T)> to_bytes(const T& object)
{
    std::array<byte, sizeof(T)> bytes;

    const byte *begin = reinterpret_cast<const byte *>(std::addressof(object));
    const byte   *end = begin + sizeof(T);
    std::copy(begin, end, std::begin(bytes));

    return bytes;
}

//! Конвертирует бинарные данные в объект
template<typename T>
T& from_bytes(const std::array<byte, sizeof(T)> &bytes)
{
    // http://en.cppreference.com/w/cpp/types/is_trivially_copyable
    static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

    byte *begin_object = reinterpret_cast<byte *>(std::addressof(object));
    std::copy(std::begin(bytes), std::end(bytes), begin_object);

    return object;
}

}}
