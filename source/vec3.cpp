#include "tml/vec3.hpp"

#include <cmath> // std::sqrt, std::pow

using tml::vec3;

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
vec3::vec3(float x, float y, float z) noexcept : m_x{x}, m_y{y}, m_z{z} {}

auto vec3::x() const noexcept -> float { return m_x; }

auto vec3::y() const noexcept -> float { return m_y; }

auto vec3::z() const noexcept -> float { return m_z; }

auto vec3::norm() const noexcept -> float
{
    return static_cast<float>(std::sqrt(std::pow(m_x, 2) + std::pow(m_y, 2) + std::pow(m_z, 2)));
}

auto vec3::cross(vec3 const& other) const noexcept -> vec3
{
    return {
        m_y * other.z() - m_z * other.y(),
        m_z * other.x() - m_x * other.z(),
        m_x * other.y() - m_y * other.x(),
    };
}

auto tml::operator+(vec3 const& lhs, vec3 const& rhs) noexcept -> vec3
{
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z()};
}

auto tml::operator-(vec3 const& lhs, vec3 const& rhs) noexcept -> vec3
{
    return {lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z()};
}

auto tml::operator+=(vec3& lhs, vec3 const& rhs) noexcept -> vec3&
{
    lhs.m_x += rhs.m_x;
    lhs.m_y += rhs.m_y;
    lhs.m_z += rhs.m_z;

    return lhs;
}

auto tml::operator-=(vec3& lhs, vec3 const& rhs) noexcept -> vec3&
{
    lhs.m_x -= rhs.m_x;
    lhs.m_y -= rhs.m_y;
    lhs.m_z -= rhs.m_z;

    return lhs;
}