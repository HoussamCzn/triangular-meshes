#pragma once

#include "tml/config.hpp" // TML_EXPORT

#include <span> // std::span
#include <vector> // std::vector

namespace tml
{
    class vec3;

    class TML_EXPORT vertex
    {
    public:

        vertex(float x, float y, float z) noexcept;

        [[nodiscard]] auto x() const noexcept -> float;

        [[nodiscard]] auto y() const noexcept -> float;

        [[nodiscard]] auto z() const noexcept -> float;

        [[nodiscard]] auto neighbors() const noexcept -> std::span<std::size_t const>;

        auto translate(vec3 const& offset) noexcept -> vertex&;

        auto scale(float factor) noexcept -> vertex&;

        auto add_neighbor(std::size_t index) noexcept -> void;

        auto operator==(vertex const& other) const noexcept -> bool;

        auto operator!=(vertex const& other) const noexcept -> bool;

    private:

        float m_x, m_y, m_z;
        std::vector<std::size_t> m_neighbors;
    };
} // namespace tml

template <>
struct std::hash<tml::vertex>
{
    auto operator()(tml::vertex const& vertex) const noexcept -> std::size_t
    {
        return std::hash<float>{}(vertex.x()) ^ std::hash<float>{}(vertex.y()) ^ std::hash<float>{}(vertex.z());
    }
};
