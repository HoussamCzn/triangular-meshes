#pragma once

#include <cstddef> // std::size_t
#include <functional> // std::hash

namespace tml
{
    struct edge
    {
        std::size_t v1, v2;

        constexpr auto operator==(edge const& other) const noexcept -> bool { return v1 == other.v1 && v2 == other.v2; }

        constexpr auto operator!=(edge const& other) const noexcept -> bool { return !(*this == other); }
    };
} // namespace tml

template <>
struct std::hash<tml::edge>
{
    auto operator()(tml::edge const& edge) const noexcept -> std::size_t
    {
        return std::hash<std::size_t>{}(edge.v1) ^ std::hash<std::size_t>{}(edge.v2);
    }
};
