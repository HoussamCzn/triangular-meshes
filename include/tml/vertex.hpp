#pragma once

#include "tml/config.hpp" // TML_EXPORT

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

        auto translate(vec3 const& offset) noexcept -> vertex&;

        [[nodiscard]] auto neighbors() const noexcept -> std::vector<std::size_t> const&;

        auto add_neighbor(std::size_t index) noexcept -> void;

    private:

        float m_x, m_y, m_z;
        std::vector<std::size_t> m_neighbors;
    };
} // namespace tml
