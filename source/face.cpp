#include "tml/face.hpp"

using tml::face;

face::face(std::size_t v1, std::size_t v2, std::size_t v3) noexcept : m_indices{v1, v2, v3} {}

auto face::indices() const noexcept -> std::array<std::size_t, 3> const& { return m_indices; }

auto face::invert() noexcept -> face&
{
    std::swap(m_indices[0], m_indices[2]);

    return *this;
}
