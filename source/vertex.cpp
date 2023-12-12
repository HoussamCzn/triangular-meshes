#include "tml/vertex.hpp"

#include "tml/vec3.hpp" // tml::vec3

using tml::vertex;

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
vertex::vertex(float x, float y, float z) noexcept : m_x{x}, m_y{y}, m_z{z} {}

auto vertex::x() const noexcept -> float { return m_x; }

auto vertex::y() const noexcept -> float { return m_y; }

auto vertex::z() const noexcept -> float { return m_z; }

auto vertex::neighbors() const noexcept -> std::vector<std::size_t> const& { return m_neighbors; }

auto vertex::translate(vec3 const& offset) noexcept -> vertex&
{
    m_x += offset.x();
    m_y += offset.y();
    m_z += offset.z();

    return *this;
}

auto vertex::scale(float factor) noexcept -> vertex&
{
    m_x *= factor;
    m_y *= factor;
    m_z *= factor;

    return *this;
}

auto vertex::add_neighbor(std::size_t index) noexcept -> void
{
    if (std::ranges::find(m_neighbors, index) == m_neighbors.end())
    {
        m_neighbors.push_back(index);
    }
}

auto vertex::operator==(vertex const& other) const noexcept -> bool
{
    return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
}

auto vertex::operator!=(vertex const& other) const noexcept -> bool { return !(*this == other); }
