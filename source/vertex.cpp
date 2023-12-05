#include "tml/vertex.hpp"

#include <ranges> // std::ranges::find

using tml::vertex;

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
vertex::vertex(float x, float y, float z) noexcept : m_x{x}, m_y{y}, m_z{z} {}

auto vertex::add_adjacent(std::size_t index) noexcept -> void
{
    if (std::ranges::find(m_adjacent, index) == m_adjacent.end())
    {
        m_adjacent.push_back(index);
    }
}

auto vertex::x() const noexcept -> float { return m_x; }

auto vertex::y() const noexcept -> float { return m_y; }

auto vertex::z() const noexcept -> float { return m_z; }

auto vertex::adjacents() const noexcept -> std::vector<std::size_t> const& { return m_adjacent; }
